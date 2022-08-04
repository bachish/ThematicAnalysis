#include "pch.h"
#include <algorithm>
#include <CppUnitTest.h>
#include <set>
#include "SemanticGraphBuilder.h"
#include "Utils/Hasher.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
 * \brief
 *please, is you use file as test-source add it into ThematicAnalysisTests.vcxproj
 */
namespace ThematicAnalysisTests
{
	TEST_CLASS(SemanticGraphBuilderTests)
	{
		TEST_METHOD(build)
		{
			SemanticGraphBuilder builder;
			std::vector<NormalizedArticle> articles = {
				{ {"терм", "первый"}, "первый терм", {"второй", "третий", "терм"}},
				{ {"второй"}, "второй", {"первый", "терм"} },
				{ {"третий"}, "третий", {"первый", "терм"} },
			};
			std::vector<size_t> articlesHashes;
			std::transform(articles.begin(), articles.end(), std::back_inserter(articlesHashes), [](auto a) {return Hasher::sortAndCalcHash(a.titleWords); });

			auto graph = builder.build(articles);
			Assert::AreEqual(3ull, graph.nodes.size());
			for (auto termHash : articlesHashes) {
				Assert::IsTrue(graph.isTermExist(termHash));
				Assert::IsFalse(graph.isLinkExist(termHash, termHash));
			}

			Assert::IsTrue(graph.isLinkExist(articlesHashes[0], articlesHashes[1]));
			Assert::IsTrue(graph.isLinkExist(articlesHashes[0], articlesHashes[2]));
			Assert::IsTrue(graph.isLinkExist(articlesHashes[1], articlesHashes[0]));
			Assert::IsFalse(graph.isLinkExist(articlesHashes[1], articlesHashes[2]));
			Assert::IsTrue(graph.isLinkExist(articlesHashes[2], articlesHashes[0]));
			Assert::IsFalse(graph.isLinkExist(articlesHashes[2], articlesHashes[1]));


			//Assert::AreEqual(TermsUtils::calcTfIdf(1,2,1,3), graph.getLinkWeight(articlesHashes[0], articlesHashes[1]), 0.0001);
			//Assert::AreEqual(TermsUtils::calcTfIdf(1, 2, 1, 3), graph.getLinkWeight(articlesHashes[0], articlesHashes[2]), 0.0001);
			//Assert::AreEqual(TermsUtils::calcTfIdf(1, 1, 2, 3), graph.getLinkWeight(articlesHashes[1], articlesHashes[0]), 0.0001);
			//Assert::AreEqual(TermsUtils::calcTfIdf(1, 1, 2, 3), graph.getLinkWeight(articlesHashes[2], articlesHashes[0]), 0.0001);
		}

		TEST_METHOD(buildWithDuplicates)
		{
			SemanticGraphBuilder builder;
			std::vector<NormalizedArticle> articles = {
				{ {"второй"}, "второй", {"третий", "терм"} },
				{ {"второй"}, "второй", {"второй", "терм"} },
				{ {"третий"}, "третий", {"второй", "терм"} },
			};
			std::vector articlesHashes = {Hasher::sortAndCalcHash(articles[0].titleWords), Hasher::sortAndCalcHash(articles[2].titleWords)};

			auto graph = builder.build(articles);
			Assert::AreEqual(2ull, graph.nodes.size());
			for (auto termHash : articlesHashes) {
				Assert::IsTrue(graph.isTermExist(termHash));
			Assert::IsFalse(graph.isLinkExist(termHash,termHash));
			}

			Assert::IsTrue(graph.isLinkExist(articlesHashes[0], articlesHashes[1]));
			Assert::IsTrue(graph.isLinkExist(articlesHashes[1], articlesHashes[0]));
		}
	};
}
