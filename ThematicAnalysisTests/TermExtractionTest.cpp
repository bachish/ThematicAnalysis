#include "pch.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "CppUnitTest.h"
#include "ArticlesNormalizer.h"
#include "FileManager.h"
#include "Hasher.h"
#include "SemanticGraph.h"
#include "SemanticGraphBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
 * \brief
 *please, is you use file as test-source add it into ThematicAnalysisTests.vcxproj
 */
namespace ThematicAnalysisTests
{
	TEST_CLASS(TermExtractionTest)
	{

		SemanticGraph getMathGraph()
		{
			SemanticGraph graph;
			graph.importFromFile("resources/coolAllMath.gr");
			return graph;
		}
		std::vector<std::string> getTextVector()
		{
			auto text = FileManager::readAllUTF8File("resources/integral.txt");
			TextNormalizer reader;
			return reader.normalize(text);
		}
		std::vector<std::string> getTextVector(std::string text)
		{
			TextNormalizer reader;
			return reader.normalize(text);
		}
		double eps = 1e-5;
		TEST_METHOD(TwoTermTest)
		{
			auto graph = getMathGraph();

			auto terms = extractAndCalculateTerms(graph,
			                                      getTextVector(R"( лючевые  слова: регул€ризации, система граничных
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
}