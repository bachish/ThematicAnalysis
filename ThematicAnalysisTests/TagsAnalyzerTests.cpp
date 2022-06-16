#include "pch.h"
#include "CppUnitTest.h"
#include "TagsAnalyzer.h"
#include "TextNormalizer.h"
#include "Utils/Hasher.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(TagsAnalyzerTests)
	{
	public:

		TEST_METHOD_INITIALIZE(createGraph)
		{
			terms = { {"Центр", 0}, {"Сосед первый", 0},{"Сосед второй", 0},{"Сосед далеко", 0} };
			for (auto& [view, hash] : terms)
			{
				TextNormalizer normalizer{};
				auto words = normalizer.normalize(view);
				hash = Hasher::sortAndCalcHash(words);
				graph.addTerm(Term(words, view, hash));
			}
			graph.createLink(terms["Центр"], terms["Сосед первый"], 1);
			graph.createLink(terms["Центр"], terms["Сосед второй"], 2);
			graph.createLink(terms["Сосед второй"], terms["Сосед далеко"], 1);
		}

		std::map<std::string, size_t> terms;
		SemanticGraph graph;

		TEST_METHOD(simpleAnalyze)
		{

			TagsAnalyzer analyzer;
			analyzer.analyze("В тексте есть термин центр и нет соседей", graph);
			auto tagsGr = analyzer.tagsGraph;
			Assert::AreEqual(4ull, tagsGr.nodes.size());
			Assert::IsTrue(tagsGr.nodes.find(terms["Центр"]) != tagsGr.nodes.end());
			Assert::IsTrue(tagsGr.nodes.find(terms["Сосед первый"]) != tagsGr.nodes.end());
			Assert::IsTrue(tagsGr.nodes.find(terms["Сосед второй"]) != tagsGr.nodes.end());
			Assert::IsTrue(tagsGr.nodes.find(terms["Сосед далеко"]) != tagsGr.nodes.end());
		}

		TEST_METHOD(relevantTags)
		{

			TagsAnalyzer analyzer;
			analyzer.analyze("В тексте есть термин центр и нет соседей", graph);
			auto tags = analyzer.getRelevantTags(3);
			std::vector<Tag> expected = { {"Центр", 0}, {"Сосед второй", 0}, {"Сосед первый", 0} };

			for (size_t i = 0; i < expected.size(); i++)
				Assert::AreEqual(expected[i].termView, tags[i].termView);
		}

		TEST_METHOD(zeroDistribution)
		{

			TagsAnalyzer analyzer;
			analyzer.distributionCoef = 0;
			analyzer.analyze("В тексте есть термин центр, центр, центр 3 раза и второй сосед второй сосед 2 раза и сосед первый 1 раз", graph);
			auto tags = analyzer.getRelevantTags(4);
			std::vector<Tag> expected = { {"Центр", 0}, {"Сосед второй", 0}, {"Сосед первый", 0},{"Сосед далеко", 0} };

			for (size_t i = 0; i < expected.size(); i++) {
				Assert::AreEqual(expected[i].termView, tags[i].termView);
			}

			Assert::IsTrue(tags[0].weight > 0.01);
			Assert::IsTrue(tags[1].weight > 0.01);
			Assert::IsTrue(tags[2].weight > 0.01);
			Assert::IsTrue(tags[3].weight < 0.0001);
		}

		TEST_METHOD(fullAbsorptionAndOneRadius)
		{

			TagsAnalyzer analyzer;
			analyzer.absorptionCoef = 1;
			analyzer.linkRadius = 1;
			analyzer.analyze("В тексте есть только термин сосед второй", graph);
			auto tags = analyzer.getRelevantTags(2);
			std::vector<Tag> expected = { {"Сосед второй", 0}, {"Сосед далеко", 0} };

			for (auto& expectedTag : expected)
			{
				Assert::IsTrue(std::find_if(tags.begin(), tags.end(),
					[&expectedTag](auto tag) {return tag.termView == expectedTag.termView; }) != tags.end());
			}

			Assert::AreEqual(tags[0].weight, tags[1].weight, 0.00001);
		}
	};
}
