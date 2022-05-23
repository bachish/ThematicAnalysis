#include "pch.h"
#include <fstream>
#include "CppUnitTest.h"
#include "DocumentReader.h"
#include "SemanticGraph.h"
#include "SemanticGraphBuilder.h"
#include "Utils.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
 * \brief
 *please, is you use file as test-source add it into ThematicAnalysisTests.vcxproj
 */
namespace ThematicAnalysisTests
{
	TEST_CLASS(SemanticGraphTests)
	{
		double eps = 1e-5;
		TEST_METHOD(AddTermTest)
		{
			auto graph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			graph.addTerm(term);
			Assert::IsTrue(graph.isTermExist(Utils::calculateHashCode(term.view)));
			Assert::IsFalse(graph.isTermExist(Utils::calculateHashCode("не" + term.view)));
		}

		TEST_METHOD(AddTwoTermsTest)
		{
			auto graph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			graph.addTerm(term);
			graph.addTerm(term2);
			Assert::IsTrue(graph.isTermExist(Utils::calculateTermHashCode(term.normalizedWords)));
			Assert::IsTrue(graph.isTermExist(Utils::calculateTermHashCode(term2.normalizedWords)));
		}
		TEST_METHOD(LinkTest)
		{
			auto graph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			graph.addTerm(term);
			graph.addTerm(term2);
			Assert::IsFalse(graph.isLinkExist(term.getHashCode(), term2.getHashCode()));
			graph.createLink(term.getHashCode(), term2.getHashCode());
			Assert::IsTrue(graph.isLinkExist(term.getHashCode(), term2.getHashCode()));
		}
		TEST_METHOD(LinkWeightTest)
		{
			double weight = 50;
			auto graph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			graph.addTerm(term);
			graph.addTerm(term2);
			graph.createLink(term.getHashCode(), term2.getHashCode());
			Assert::IsTrue(graph.getLinkWeight(term.getHashCode(), term2.getHashCode()) < eps);
			graph.addLinkWeight(term.getHashCode(), term2.getHashCode(), weight);
			Assert::IsTrue(graph.getLinkWeight(term.getHashCode(), term2.getHashCode()) - weight < eps);
		}
		TEST_METHOD(SubgraphR0Test)
		{
			auto graph = new SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			graph->addTerm(term);
			graph->addTerm(term2);
			graph->createLink(term.getHashCode(), term2.getHashCode());
			const auto copy = graph->getNeighborhood(term.getHashCode(), 0);
			Assert::IsTrue(copy.isTermExist(term.getHashCode()));
			Assert::IsFalse(copy.isTermExist(term2.getHashCode()));
		}

		TEST_METHOD(SubgraphR1Test)
		{
			auto graph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			auto term3 = Term({ "СТЕПЕНЬ" }, "СТЕПЕНИ");
			graph.addTerm(term);
			graph.addTerm(term2);
			graph.addTerm(term3);
			graph.createLink(term.getHashCode(), term2.getHashCode());
			graph.createLink(term3.getHashCode(), term2.getHashCode());
			const auto copy = graph.getNeighborhood(term.getHashCode(), 1);
			Assert::IsTrue(copy.isTermExist(term.getHashCode()));
			Assert::IsTrue(copy.isTermExist(term2.getHashCode()));
			Assert::IsTrue(copy.isLinkExist(term.getHashCode(), term2.getHashCode()));
			Assert::IsFalse(copy.isTermExist(term3.getHashCode()));
			Assert::IsFalse(copy.isLinkExist(term3.getHashCode(), term2.getHashCode()));
		}
		TEST_METHOD(SubgraphOfBigGraph)
		{
			auto builder = SemanticGraphBuilder();
			auto reader = DocumentReader();
			auto graph = builder.build(reader.read("resources/MiddleMath.txt"));
			Assert::IsTrue(graph.isTermExist(Utils::calculateHashCode("алгол")));
		}

		SemanticGraph getDefaultGraph()
		{
			auto graph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			auto term3 = Term({ "СТЕПЕНЬ" }, "СТЕПЕНИ");
			graph.addTerm(term);
			graph.addTerm(term2);
			graph.addTerm(term3);
			graph.createLink(term.getHashCode(), term2.getHashCode());
			graph.createLink(term3.getHashCode(), term2.getHashCode());
			graph.addLinkWeight(term3.getHashCode(), term2.getHashCode(), 1);
			return graph;
		}

		TEST_METHOD(exportTest)
		{
			auto graph = getDefaultGraph();

			std::stringstream exportSs;
			graph.exportToStream(exportSs);
			Assert::AreEqual(exportSs.str(), std::string("3\n"
				"АБЕЛЕВЫ ГРУППЫ\n"
				"0 2 АБЕЛЕВА ГРУППА \n"
				"СТЕПЕНИ\n"
				"0 1 СТЕПЕНЬ \n"
				"АБАК\n"
				"0 1 АБАК \n"
				"2\n"
				"0 1 1\n"
				"0 2 0\n"));
		}

		TEST_METHOD(exportAndImportTest)
		{
			auto exportedGraph = SemanticGraph();
			auto term = Term({ "АБАК" }, "АБАК");
			auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
			auto term3 = Term({ "СТЕПЕНЬ" }, "СТЕПЕНИ");
			exportedGraph.addTerm(term);
			exportedGraph.addTerm(term2);
			exportedGraph.addTerm(term3);
			exportedGraph.createLink(term.getHashCode(), term2.getHashCode());
			exportedGraph.createLink(term3.getHashCode(), term2.getHashCode(), 1);

			std::stringstream exportSs;
			exportedGraph.exportToStream(exportSs);
			SemanticGraph importedGraph;
			importedGraph.importFromStream(exportSs);
			Assert::IsTrue(importedGraph.isTermExist(term.getHashCode()));
			Assert::IsTrue(importedGraph.isTermExist(term2.getHashCode()));
			Assert::IsTrue(importedGraph.isTermExist(term3.getHashCode()));
			Assert::IsTrue(importedGraph.isLinkExist(term.getHashCode(), term2.getHashCode()));
			Assert::IsTrue(importedGraph.isLinkExist(term3.getHashCode(), term2.getHashCode()));
			Assert::IsFalse(importedGraph.isLinkExist(term3.getHashCode(), term.getHashCode()));
			Assert::AreEqual(0., importedGraph.getLinkWeight(term.getHashCode(), term2.getHashCode()));
			Assert::AreEqual(1., importedGraph.getLinkWeight(term2.getHashCode(), term3.getHashCode()));
		}
	};
}