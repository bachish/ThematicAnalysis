#include "pch.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "CppUnitTest.h"
#include "DocumentReader.h"
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
			DocumentReader reader;
			return reader.readAndNormalizeText(text);
		}
		std::vector<std::string> getTextVector(std::string text)
		{
			DocumentReader reader;
			return reader.readAndNormalizeText(text);
		}
		double eps = 1e-5;
		TEST_METHOD(TwoTermTest)
		{
			auto graph = getMathGraph();

			auto terms = extractAndCalculateTerms(graph,
			                                      getTextVector(R"( лючевые  слова: система граничных
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
			Assert::AreEqual(0ull, terms[Hasher::sortAndCalcHash(term2)]);
			Assert::AreEqual(2ull, terms[Hasher::sortAndCalcHash(term)]);
		}
	};
}