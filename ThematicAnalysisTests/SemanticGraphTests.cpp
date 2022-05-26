#include "pch.h"
#include <fstream>
#include "CppUnitTest.h"
#include "DocumentReader.h"
#include "Hasher.h"
#include "SemanticGraph.h"
#include "SemanticGraphBuilder.h"
#include "StringUtils.h"
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
			std::vector<std::string> normWords = { "АБАК" };
			std::string view = "АБАК";
			auto term = Term(normWords, view, Hasher::sortAndCalcHash(normWords));
			graph.addTerm(term);
			Assert::IsTrue(graph.isTermExist(Hasher::sortAndCalcHash(normWords)));
			normWords.emplace_back("не");
			Assert::IsFalse(graph.isTermExist(Hasher::sortAndCalcHash(normWords)));
		}

		TEST_METHOD(AddTwoTermsTest)
		{
			auto graph = SemanticGraph();
			std::vector<std::vector<std::string>> normWords = { { "АБАК" },{ "АБЕЛЕВА", "ГРУППА" } };
			std::vector<std::string> views = { "АБАК", "АБЕЛЕВЫ ГРУППЫ" };
			std::vector<Term> terms;
			for (size_t i = 0; i < views.size(); i++)
			{
				terms.emplace_back(normWords[i], views[i], Hasher::sortAndCalcHash(normWords[i]));
				graph.addTerm(terms[i]);
				Assert::IsTrue(graph.isTermExist(Hasher::sortAndCalcHash(normWords[i])));
			}
		}

		TEST_METHOD(LinkTest)
		{
			auto graph = SemanticGraph();
			std::vector<std::vector<std::string>> normWords = { { "АБАК" },{ "АБЕЛЕВА", "ГРУППА" } };
			std::vector<std::string> views = { "АБАК", "АБЕЛЕВЫ ГРУППЫ" };
			std::vector<Term> terms;
			for (size_t i = 0; i < views.size(); i++)
			{
				terms.emplace_back(normWords[i], views[i], Hasher::sortAndCalcHash(normWords[i]));
				graph.addTerm(terms[i]);
			}
			Assert::IsFalse(graph.isLinkExist(terms[0].getHashCode(), terms[1].getHashCode()));
			graph.createLink(terms[0].getHashCode(), terms[1].getHashCode());
			Assert::IsTrue(graph.isLinkExist(terms[0].getHashCode(), terms[1].getHashCode()));
		}

		TEST_METHOD(LinkWeightTest)
		{
			double weight = 50;
			auto graph = SemanticGraph();
			std::vector<std::vector<std::string>> normWords = { { "АБАК" },{ "АБЕЛЕВА", "ГРУППА" }, {"СТЕПЕНЬ"}};
			std::vector<std::string> views = { "АБАК", "АБЕЛЕВЫ ГРУППЫ", "СТЕПЕНИ"};
			std::vector<Term> terms;
			for (size_t i = 0; i < views.size(); i++)
			{
				terms.emplace_back(normWords[i], views[i], Hasher::sortAndCalcHash(normWords[i]));
				graph.addTerm(terms[i]);
			}
			graph.createLink(terms[0].getHashCode(), terms[1].getHashCode());
			Assert::IsTrue(graph.getLinkWeight(terms[0].getHashCode(), terms[1].getHashCode()) < eps);
			graph.createLink(terms[1].getHashCode(), terms[2].getHashCode(), 0.5);
			Assert::IsTrue(graph.getLinkWeight(terms[1].getHashCode(), terms[2].getHashCode()) - 0.5 <  eps);
		}

		Term getTerm(std::vector<std::string> words = { "диплом" }) const
		{
			return { words, StringUtils::concat(words), Hasher::sortAndCalcHash(words) };
		}

		TEST_METHOD(AddTermTwoTimes)
		{
			auto graph = SemanticGraph();
			auto term = getTerm();
			graph.addTerm(term);
			graph.addTerm(term);
		}


		TEST_METHOD(AddLinkTwoTimes)
		{
			auto graph = SemanticGraph();
			auto term1 = getTerm({ "диплом" });
			auto term2 = getTerm({ "нет", "диплом" });
			graph.addTerm(term1);
			graph.addTerm(term2);
			graph.createLink(term1.getHashCode(), term2.getHashCode());
			Assert::ExpectException<std::logic_error>([&]
				{
					graph.createLink(term1.getHashCode(), term2.getHashCode());
				});
		}

		TEST_METHOD(SubgraphR0Test)
		{
			auto graph = SemanticGraph();
			std::vector<std::vector<std::string>> normWords = { { "АБАК" },{ "АБЕЛЕВА", "ГРУППА" } };
			std::vector<std::string> views = { "АБАК", "АБЕЛЕВЫ ГРУППЫ" };
			std::vector<Term> terms;
			for (size_t i = 0; i < views.size(); i++)
			{
				terms.emplace_back(normWords[i], views[i], Hasher::sortAndCalcHash(normWords[i]));
				graph.addTerm(terms[i]);
			}

			graph.createLink(terms[0].getHashCode(), terms[1].getHashCode());
			const auto copy = graph.getNeighborhood(terms[0].getHashCode(), 0);
			Assert::IsTrue(copy.isTermExist(terms[0].getHashCode()));
			Assert::IsFalse(copy.isTermExist(terms[1].getHashCode()));
		}

		TEST_METHOD(SubgraphR1Test)
		{
			auto graph = SemanticGraph();
			std::vector<std::vector<std::string>> normWords = { { "АБАК" },{ "АБЕЛЕВА", "ГРУППА" },{ "СТЕПЕНЬ" } };
			std::vector<std::string> views = { "АБАК", "АБЕЛЕВЫ ГРУППЫ","СТЕПЕНИ" };
			std::vector<Term> terms;
			for (size_t i = 0; i < views.size(); i++)
			{
				terms.emplace_back(normWords[i], views[i], Hasher::sortAndCalcHash(normWords[i]));
				graph.addTerm(terms[i]);
			}

			graph.createLink(terms[0].getHashCode(), terms[1].getHashCode());
			graph.createLink(terms[2].getHashCode(), terms[1].getHashCode());
			const auto copy = graph.getNeighborhood(terms[0].getHashCode(), 1);
			Assert::IsTrue(copy.isTermExist(terms[0].getHashCode()));
			Assert::IsTrue(copy.isTermExist(terms[1].getHashCode()));
			Assert::IsTrue(copy.isLinkExist(terms[0].getHashCode(), terms[1].getHashCode()));
			Assert::IsFalse(copy.isTermExist(terms[2].getHashCode()));
			Assert::IsFalse(copy.isLinkExist(terms[2].getHashCode(), terms[1].getHashCode()));
		}

		TEST_METHOD(SubgraphOfBigGraph)
		{
			auto builder = SemanticGraphBuilder();
			auto reader = DocumentReader();
			auto graph = builder.build(reader.readAndNormalizeArticles("resources/MiddleMath.txt"));
			Assert::IsTrue(graph.isTermExist(Hasher::sortAndCalcHash({ "алгол" })));
		}

		TEST_METHOD(exportAndImportTest)
		{
			auto exportedGraph = SemanticGraph();
			std::vector<std::vector<std::string>> normWords = { { "АБАК" },{ "АБЕЛЕВА", "ГРУППА" },{ "СТЕПЕНЬ" } };
			std::vector<std::string> views = { "АБАК", "АБЕЛЕВЫ ГРУППЫ","СТЕПЕНИ" };
			std::vector<Term> terms;
			for (size_t i = 0; i < views.size(); i++)
			{
				terms.emplace_back(normWords[i], views[i], Hasher::sortAndCalcHash(normWords[i]));
				exportedGraph.addTerm(terms[i]);
			}
			exportedGraph.createLink(terms[0].getHashCode(), terms[1].getHashCode());
			exportedGraph.createLink(terms[2].getHashCode(), terms[1].getHashCode(), 2);

			std::stringstream exportSs;
			exportedGraph.exportToStream(exportSs);
			SemanticGraph importedGraph;
			importedGraph.importFromStream(exportSs);

			Assert::IsTrue(importedGraph.isTermExist(terms[0].getHashCode()));
			Assert::IsTrue(importedGraph.isTermExist(terms[1].getHashCode()));
			Assert::IsTrue(importedGraph.isTermExist(terms[2].getHashCode()));
			Assert::IsTrue(importedGraph.isLinkExist(terms[0].getHashCode(), terms[1].getHashCode()));
			Assert::IsFalse(importedGraph.isLinkExist(terms[1].getHashCode(), terms[0].getHashCode()));
			Assert::IsTrue(importedGraph.isLinkExist(terms[2].getHashCode(), terms[1].getHashCode()));
			Assert::IsFalse(importedGraph.isLinkExist(terms[1].getHashCode(), terms[2].getHashCode()));
			Assert::IsFalse(importedGraph.isLinkExist(terms[2].getHashCode(), terms[0].getHashCode()));
			Assert::IsFalse(importedGraph.isLinkExist(terms[0].getHashCode(), terms[2].getHashCode()));
			Assert::AreEqual(0., importedGraph.getLinkWeight(terms[0].getHashCode(), terms[1].getHashCode()));
			Assert::AreEqual(2., importedGraph.getLinkWeight(terms[2].getHashCode(), terms[1].getHashCode()));
		}

		
	};
}