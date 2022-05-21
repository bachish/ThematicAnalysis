#include "pch.h"
#include <fstream>
#include "CppUnitTest.h"
#include "SemanticGraph.h"
#include "Utils.h"
#include "XmlSourceParser.h"
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
		TEST_METHOD(addTerm)
		{
			auto graph = SemanticGraph();
			auto term = Term({L"АБАК"}, L"АБАК");
			graph.addTerm(term);
			Assert::IsTrue(graph.isTermExist(Utils::calculateHashCode(term.view)));
			Assert::IsFalse(graph.isTermExist(Utils::calculateHashCode(L"не" + term.view)));
		}

		TEST_METHOD(addTwoTerm)
		{
			auto graph = SemanticGraph();
			auto term = Term({ L"АБАК" }, L"АБАК");
			auto term2 = Term({ L"АБЕЛЕВА", L"ГРУППА" }, L"АБЕЛЕВЫ ГРУППЫ");
			graph.addTerm(term);
			graph.addTerm(term2);
			Assert::IsTrue(graph.isTermExist(Utils::calculateTermHashCode(term.normalizedWords)));
			Assert::IsTrue(graph.isTermExist(Utils::calculateTermHashCode(term2.normalizedWords)));
		}
		TEST_METHOD(chekLink)
		{
			auto graph = SemanticGraph();
			auto term = Term({ L"АБАК" }, L"АБАК");
			auto term2 = Term({ L"АБЕЛЕВА", L"ГРУППА" }, L"АБЕЛЕВЫ ГРУППЫ");
			graph.addTerm(term);
			graph.addTerm(term2);
			Assert::IsFalse(graph.isLinkExist(term.getHashCode(), term2.getHashCode()));
			graph.addLink(term.getHashCode(), term2.getHashCode());
			Assert::IsTrue(graph.isLinkExist(term.getHashCode(), term2.getHashCode()));
		}
		TEST_METHOD(chekLinkWeight)
		{
			double weight = 50;
			auto graph = SemanticGraph();
			auto term = Term({ L"АБАК" }, L"АБАК");
			auto term2 = Term({ L"АБЕЛЕВА", L"ГРУППА" }, L"АБЕЛЕВЫ ГРУППЫ");
			graph.addTerm(term);
			graph.addTerm(term2);
			graph.addLink(term.getHashCode(), term2.getHashCode());
			Assert::IsTrue(graph.getLinkWeight(term.getHashCode(), term2.getHashCode()) < eps);
			graph.addLinkWeight(term.getHashCode(), term2.getHashCode(), weight);
			Assert::IsTrue(graph.getLinkWeight(term.getHashCode(), term2.getHashCode()) - weight < eps);
		}
	};
}