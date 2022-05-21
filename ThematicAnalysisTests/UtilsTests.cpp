#include "pch.h"
#include "Utils.h"
#include <vector>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{

	TEST_CLASS(UtilsTests)
	{
		TEST_METHOD(engSortAndConcatWords)
		{
			std::vector<std::string> words = { "b", "a", "c", "z" };
			Assert::AreEqual(Utils::sortAndConcatWords(words), std::string("abcz"));
		}

		TEST_METHOD(rusSortAndConcatWords)
		{
			std::vector<std::string> words = { "б", "а", "в", "я" };
			Assert::AreEqual(Utils::sortAndConcatWords(words), std::string("абвя"));
		}

		TEST_METHOD(engDistinctHashCodes)
		{
			auto str = "abc";
			auto distinctStr = "bca";
			Assert::AreNotEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(engEqualHashCodes)
		{
			auto str = "abc";
			auto distinctStr = "abc";
			Assert::AreEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(rusDistinctHashCodes)
		{
			auto str = "абв";
			auto distinctStr = "авб";
			Assert::AreNotEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(rusEqualHashCodes)
		{
			auto str = "абв";
			auto distinctStr = "абв";
			Assert::AreEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(clearStringWithExtraSpaces)
		{
			std::string str = "Это  строка с   лишними  пробелами";
			std::string expectedStr = "Это строка с лишними пробелами";
			Utils::clearString(str);
			Assert::AreEqual(expectedStr, str);
		}

		TEST_METHOD(clearStringWithSpacesAtBeginAndEnd)
		{
			std::string str = "  Это строка с лишними пробелами   ";
			std::string expectedStr = "Это строка с лишними пробелами";
			Utils::clearString(str);
			Assert::AreEqual(expectedStr, str);
		}

		TEST_METHOD(clearStringWithDistinctSpaces)
		{
			std::string str = R"(		Это строка	 с  лишними
											пробелами )";
			std::string expectedStr = "Это строка с лишними пробелами";
			Utils::clearString(str);
			Assert::AreEqual(expectedStr, str);
		}

		TEST_METHOD(areLetters)
		{
			std::string letters = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			for (auto&& letter : letters)
				Assert::IsTrue(Utils::isLetter(letter));
		}

		TEST_METHOD(areNotLetters)
		{
			std::string notLetters = "!@#$%^&*()1234567890\"№;:?_+-=*/~";
			for (auto&& notLetter : notLetters)
				Assert::IsFalse(Utils::isLetter(notLetter));
		}

	};
}