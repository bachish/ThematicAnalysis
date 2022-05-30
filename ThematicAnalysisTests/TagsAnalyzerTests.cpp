#include "pch.h"
#include "CppUnitTest.h"
#include "Hasher.h"

#include "TextAnalyzer.h"
#include "TextNormalizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(TagsAnalyzerTests)
	{
	public:

		TEST_METHOD_INITIALIZE(createGraph)
		{
			terms = { {"Центр", 0}, {"Сосед первый", 0},{"Сосед второй", 0},{"Сосед далеко", 0} };
			TextNormalizer normalizer;
			for (auto& [view, hash] : terms)
			{
				auto words = normalizer.normalize(view);
				hash = Hasher::sortAndCalcHash(words);
				gr.addTerm(Term(words, view, hash));
			}
			gr.createLink(terms["Центр"], terms["Сосед первый"], 1);
			gr.createLink(terms["Центр"], terms["Сосед второй"], 2);
			gr.createLink(terms["Сосед второй"], terms["Сосед далеко"], 1);
		}

		std::map<std::string, size_t> terms;
		SemanticGraph gr;

		TEST_METHOD(simpleAnalyze)
		{

			TextAnalyzer analyzer;
			analyzer.analyze("В тексте есть термин центр и нет соседей", gr);
			auto tagsGr = analyzer.tagsGraph;
			Assert::AreEqual(4ull, tagsGr.nodes.size());
			Assert::IsTrue(tagsGr.nodes.find(terms["Центр"]) != tagsGr.nodes.end());
			Assert::IsTrue(tagsGr.nodes.find(terms["Сосед первый"]) != tagsGr.nodes.end());
			Assert::IsTrue(tagsGr.nodes.find(terms["Сосед второй"]) != tagsGr.nodes.end());
			Assert::IsTrue(tagsGr.nodes.find(terms["Сосед далеко"]) != tagsGr.nodes.end());
		}

		TEST_METHOD(relevantTags)
		{

			TextAnalyzer analyzer;
			analyzer.analyze("В тексте есть термин центр и нет соседей", gr);
			auto tags = analyzer.getRelevantTags(3);
			std::vector<Tag> expected = { {"Центр", 0}, {"Сосед второй", 0}, {"Сосед первый", 0} };

			for (size_t i = 0; i < expected.size(); i++)
				Assert::AreEqual(expected[i].termView, tags[i].termView);
		}
	};
}
