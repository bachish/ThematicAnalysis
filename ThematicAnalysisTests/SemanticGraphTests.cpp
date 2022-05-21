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
			Assert::IsTrue(graph.isTermExist(Utils::calculateTermHashCode(term.words)));
			Assert::IsTrue(graph.isTermExist(Utils::calculateTermHashCode(term2.words)));
		}
	};
}