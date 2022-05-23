#include "pch.h"
#include <vector>
#include "CppUnitTest.h"
#include "Hasher.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{

	TEST_CLASS(UtilsTests)
	{
		TEST_CLASS_INITIALIZE(locale)
		{
			setlocale(LC_ALL, "rus");
		}

		TEST_METHOD(engDistinctHashCodes)
		{
			std::vector<std::string> str = { "abc", "xy" };
			std::vector<std::string> distinctStr = { "yx", "bca" };
			Assert::AreNotEqual(Hasher::sortAndCalcHash(str), Hasher::sortAndCalcHash(distinctStr));
		}

		TEST_METHOD(engEqualHashCodes)
		{
			std::vector<std::string> str = { "abc", "xy" };
			std::vector<std::string> equalStr = { "xy", "abc" };
			Assert::AreEqual(Hasher::sortAndCalcHash(str), Hasher::sortAndCalcHash(equalStr));
		}

		TEST_METHOD(rusDistinctHashCodes)
		{
			std::vector<std::string> str = { "абв", "аы" };
			std::vector<std::string> distinctStr = { "ыа","авб" };
			Assert::AreNotEqual(Hasher::sortAndCalcHash(str), Hasher::sortAndCalcHash(distinctStr));
		}

		TEST_METHOD(rusEqualHashCodes)
		{
			std::vector<std::string> str = { "абв", "ыа" };
			std::vector<std::string> equalStr = { "ыа", "абв" };
			Assert::AreEqual(Hasher::sortAndCalcHash(str), Hasher::sortAndCalcHash(equalStr));
		}

	};
}