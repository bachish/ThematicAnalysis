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
			auto term = Term({"АБАК"}, "АБАК");
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
			graph.addLink(term.getHashCode(), term2.getHashCode());
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
			graph.addLink(term.getHashCode(), term2.getHashCode());
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
			graph->addLink(term.getHashCode(), term2.getHashCode());
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
			graph.addLink(term.getHashCode(), term2.getHashCode());
			graph.addLink(term3.getHashCode(), term2.getHashCode());
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
	};
}