#include "pch.h"
#include "CppUnitTest.h"
#include "ArticlesNormalizer.h"
#include <fstream>
#include <iostream>

#include "ArticlesReader/XmlArticlesReader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(ArticlesNormalizerTests)
	{
	public:

		TEST_METHOD(readXml)
		{
			ArticlesNormalizer reader;
			auto articles = reader.readAndNormalizeArticles(
				"<paper>\n"
				"<name>\n"
				"Термин		.\n"
				"</name>\n"
				"<content>\n"
				"	Нет термина  важней, чем  термин  \n"
				"</content>\n"
				"</paper>\n", XmlArticlesReader());
			Assert::AreEqual(1ull, articles.size());
			Assert::AreEqual(1ull, articles[0].titleWords.size());
			Assert::AreEqual(std::string("термин"), articles[0].titleWords[0]);
			Assert::AreEqual(std::string("Термин"), articles[0].titleView);
			Assert::AreEqual(5ull, articles[0].text.size());
			auto normText = TextNormalizer().normalize("	Нет термина  важней, чем  термин  \n");
			Assert::AreEqual(normText.size(), articles[0].text.size());
			for (size_t i = 0; i < normText.size(); i++)
				Assert::AreEqual(normText[i], articles[0].text[i]);
		}
	};
}
