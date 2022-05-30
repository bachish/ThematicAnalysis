#include "pch.h"
#include "CppUnitTest.h"
#include "TextNormalizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(NormalizerTests)
	{
		TEST_CLASS_INITIALIZE(setLocale)
		{
			setlocale(LC_ALL, "rus");
		}
		TEST_METHOD(WordsTest)
		{
			TextNormalizer normalizer;
			std::string text = "Сижу x- и. <, *стою на \n дороге#";
			std::vector<std::string> normWords = { "сидеть",  "стоять", "на" , "дорога"};
			auto resWords = normalizer.normalize(text);
			
			Assert::AreEqual((size_t)4, resWords.size());

			for (size_t i = 0; i < resWords.size(); i++)
			{
				Assert::AreEqual(normWords[i], resWords[i]);
			}
		}
	};
}

