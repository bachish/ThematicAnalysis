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
			std::vector<std::wstring> words = { L"b", L"a", L"c", L"z" };
			Assert::AreEqual(Utils::sortAndConcatWords(words), std::wstring(L"abcz"));
		}

		TEST_METHOD(rusSortAndConcatWords)
		{
			std::vector<std::wstring> words = { L"б", L"а", L"в", L"я" };
			Assert::AreEqual(Utils::sortAndConcatWords(words), std::wstring(L"абвя"));
		}

		TEST_METHOD(engDistinctHashCodes)
		{
			auto str = L"abc";
			auto distinctStr = L"bca";
			Assert::AreNotEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(engEqualHashCodes)
		{
			auto str = L"abc";
			auto distinctStr = L"abc";
			Assert::AreEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(rusDistinctHashCodes)
		{
			auto str = L"абв";
			auto distinctStr = L"авб";
			Assert::AreNotEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(rusEqualHashCodes)
		{
			auto str = L"абв";
			auto distinctStr = L"абв";
			Assert::AreEqual(Utils::calculateHashCode(str), Utils::calculateHashCode(distinctStr));
		}

		TEST_METHOD(clearStringWithExtraSpaces)
		{
			std::wstring str = L"Это  строка с   лишними  пробелами";
			std::wstring expectedStr = L"Это строка с лишними пробелами";
			Utils::clearString(str);
			Assert::AreEqual(expectedStr, str);
		}

		TEST_METHOD(clearStringWithSpacesAtBeginAndEnd)
		{
			std::wstring str = L"  Это строка с лишними пробелами   ";
			std::wstring expectedStr = L"Это строка с лишними пробелами";
			Utils::clearString(str);
			Assert::AreEqual(expectedStr, str);
		}

		TEST_METHOD(clearStringWithDistinctSpaces)
		{
			std::wstring str = LR"(		Это строка	 с  лишними
											пробелами )";
			std::wstring expectedStr = L"Это строка с лишними пробелами";
			Utils::clearString(str);
			Assert::AreEqual(expectedStr, str);
		}

		TEST_METHOD(areLetters)
		{
			std::wstring letters = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			for (auto&& letter : letters)
				Assert::IsTrue(Utils::isLetter(letter), (std::wstring(L"is not letter: ") + letter).c_str());
		}

		TEST_METHOD(areNotLetters)
		{
			std::wstring notLetters = L"!@#$%^&*()1234567890\"№;:?_+-=*/~";
			for (auto&& notLetter : notLetters)
				Assert::IsFalse(Utils::isLetter(notLetter), (std::wstring(L"is letter: ") + notLetter).c_str());
		}

	};
}