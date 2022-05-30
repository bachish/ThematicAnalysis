#include "pch.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "CppUnitTest.h"
#include "ArticlesNormalizer.h"
#include "Utils/FileUtils.h"
#include "Hasher.h"
#include "SemanticGraph.h"
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
	};
	SemanticGraph TermsUtilsTests::graph = SemanticGraph();
}