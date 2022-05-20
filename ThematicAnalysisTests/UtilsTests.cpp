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
	};
}