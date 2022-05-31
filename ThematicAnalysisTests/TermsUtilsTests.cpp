#include "pch.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>

#include "CppUnitTest.h"
#include "ArticlesNormalizer.h"
#include "Utils/FileUtils.h"
#include "Hasher.h"
#include "SemanticGraph.h"
#include "SemanticGraphBuilder.h"
#include "ArticlesReader/MathArticlesReader.h"
#include "Utils/TermsUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
 * \brief
 *please, is you use file as test-source add it into ThematicAnalysisTests.vcxproj
 */
namespace ThematicAnalysisTests
{

	TEST_CLASS(TermsUtilsTests)
	{
		static SemanticGraph graph;
		TEST_CLASS_INITIALIZE(TermsUtilsInitialize)
		{
			graph.importFromFile("resources/coolAllMath.gr");
		}

		std::vector<std::string> getTextVectorFromFS(std::string pathFile)
		{
			auto text = FileUtils::readAllUTF8File(pathFile);
			TextNormalizer reader;
			return reader.normalize(text);
		}
		std::vector<std::string> getTextVector(std::string text)
		{
			TextNormalizer reader;
			return reader.normalize(text);
		}
		double eps = 1e-5;
		TEST_METHOD(TwoTermsExtractionTest)
		{
			auto terms = TermsUtils::extractTermsCounts(graph,
				getTextVector(R"( лючевые  слова: 
					регул€ризации, система граничных
					интегральных
					уравнений, метод
					регул€ризации, голоморфный
					вектор,
					методы
					теории
					потенциалов.
					используетс€
					метод
					регул€ризации, с
					)"));
			std::vector<std::string> term = { "метод", "регул€ризаци€" };
			std::vector<std::string> term2 = { "регул€ризаци€" };
			Assert::AreEqual(1ull, terms[Hasher::sortAndCalcHash(term2)]);
			Assert::AreEqual(2ull, terms[Hasher::sortAndCalcHash(term)]);
		}
		TEST_METHOD(tfIdfTest)
		{
			std::string atriclesText = R"(

¬џЅќ–ќ„Ќјя ƒ»—ѕ≈–—»я Ч дисперси€ эмпирического распределени€.

¬џЅќ–ќ„Ќјя  ¬јЌ“»Ћ№ Ч квантиль функции эмпирического распределени€.

¬џЅќ–ќ„Ќјя ћ≈ƒ»јЌј Ч медиана эмпирического распределени€.

ћ≈ƒ»јЌј Ч одна из числовых характеристик распределени€ веро€тностей, частный случай квантили.

 ¬јЌ“»Ћ№ Ч одна из числовых характеристик распределени€ веро€тностей.)";
			auto smallGraph = SemanticGraphBuilder().build(atriclesText, MathArticlesReader());
			auto articlesCount = smallGraph.nodes.size();
			auto countTerms = TermsUtils::extractTermsCounts(smallGraph,
				TextNormalizer().normalize(atriclesText));
			auto allTermsCount = std::transform_reduce(std::execution::par, countTerms.begin(),
				countTerms.end(), 0ull, [](size_t a, size_t b) {return a + b; }, [](auto const& pair) {return pair.second; });
			auto hash = Hasher::sortAndCalcHash({ "медиана" });
			auto idfs = TermsUtils::calcTfIdf(countTerms[hash], allTermsCount, 
				smallGraph.nodes[hash].term.numberOfArticlesThatUseIt, articlesCount);

			//7 терминов потому что "квантили" лемматизиуетс€ как "квантить"
			auto handIdf = (2. / 7.) * std::log10((double)articlesCount / 1.);
			Assert::IsTrue(idfs - handIdf < eps);

			hash = Hasher::sortAndCalcHash({ "выборочный", "медиана"});
			idfs = TermsUtils::calcTfIdf(countTerms[hash], allTermsCount,
				smallGraph.nodes[hash].term.numberOfArticlesThatUseIt, articlesCount);
			handIdf = (1. / 7.) * std::log10((double)(articlesCount + 1)  / 1.);
			Assert::IsTrue(idfs - handIdf < eps);
		}
	};
	SemanticGraph TermsUtilsTests::graph = SemanticGraph();
}