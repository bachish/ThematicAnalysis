#include "pch.h"
#include "CppUnitTest.h"
#include "Lemmatizer.h"
#include "XmlSourceParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(LemmatizerTests)
	{

		TEST_METHOD(WordsTest)
		{
			Lemmatizer lemmatizer;
			std::vector<std::wstring> sourceWords = { L"Сижу", L"Работы", L"Гречневые", L"Забавно", L"Интегралов" };
			std::vector<std::wstring> normWords = { L"сидеть", L"работа", L"гречневый", L"забавный", L"интеграл" };
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
			std::wstring sourceText = L"Сижу Работы ГречнЕвые ЗабавНо Интегралов";
			std::vector<std::wstring> normWords = { L"сидеть", L"работа", L"гречневый", L"забавный", L"интеграл" };
			auto res = lemmatizer.lemmatizeText(sourceText);
			Assert::AreEqual((size_t)5, res.size());

			for (size_t i = 0; i < normWords.size(); i++)
				Assert::AreEqual(normWords[i], res[i]);
		}
	};
}

