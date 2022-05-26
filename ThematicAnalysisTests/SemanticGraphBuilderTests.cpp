﻿#include "pch.h"

#include <algorithm>
#include <CppUnitTest.h>

#include "Hasher.h"
#include "SemanticGraphBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
 * \brief
 *please, is you use file as test-source add it into ThematicAnalysisTests.vcxproj
 */
namespace ThematicAnalysisTests
{
	TEST_CLASS(SemanticGraphBuilderTests)
	{
		TEST_METHOD(buildTest)
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


			Assert::AreEqual(0.5493, graph.getLinkWeight(articlesHashes[0], articlesHashes[1]), 0.0001);
			Assert::AreEqual(0.5493, graph.getLinkWeight(articlesHashes[0], articlesHashes[2]), 0.0001);
			Assert::AreEqual(0.4055, graph.getLinkWeight(articlesHashes[1], articlesHashes[0]), 0.0001);
			Assert::AreEqual(0.4055, graph.getLinkWeight(articlesHashes[2], articlesHashes[0]), 0.0001);
		}


	};
}