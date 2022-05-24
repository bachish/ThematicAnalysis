#include "pch.h"
#include "CppUnitTest.h"
#include "Lemmatizer.h"
#include "XmlSourceParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(LemmatizerTests)
	{
		TEST_CLASS_INITIALIZE(setLocale)
		{
			setlocale(LC_ALL, "rus");
		}
		TEST_METHOD(WordsTest)
		{
			Lemmatizer lemmatizer;
			std::vector<std::string> sourceWords = { "Сижу", "Работы", "Гречневые", "Забавно", "Интегралов" };
			std::vector<std::string> normWords = { "сидеть", "работа", "гречневый", "забавный", "интеграл" };
			for (size_t i = 0; i < sourceWords.size(); i++)
			{
				auto res = lemmatizer.lemmatizeText(sourceWords[i]);
				Assert::AreEqual((size_t)1, res.size());
				Assert::AreEqual(normWords[i], res[0]);
			}
		}



		TEST_METHOD(TextTest)
		{
			Lemmatizer lemmatizer;
			std::string sourceText = "Сижу Работы ГречнЕвые ЗабавНо Интегралов";
			std::vector<std::string> normWords = { "сидеть", "работа", "гречневый", "забавный", "интеграл" };
			auto res = lemmatizer.lemmatizeText(sourceText);
			Assert::AreEqual((size_t)5, res.size());

			for (size_t i = 0; i < normWords.size(); i++)
				Assert::AreEqual(normWords[i], res[i]);
		}
	};
}

