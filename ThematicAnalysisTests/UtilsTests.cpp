#include "pch.h"
#include <vector>
#include "CppUnitTest.h"
#include "Hasher.h"
#include "StringUtils.h"

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


		TEST_METHOD(stringSplit)
		{
			std::string str = "всем Привет";
			std::vector<std::string> splitStr = { "всем", "Привет" };
			auto res = StringUtils::split(str);
			Assert::AreEqual(res.size(), splitStr.size());
			for(int i = 0; i < res.size(); i++)
				Assert::AreEqual(res[i], splitStr[i]);
		}

		TEST_METHOD(stringSplitExtraSpaces)
		{
			std::string str = " всем  Привет  ";
			std::vector<std::string> splitStr = { "", "всем", "", "Привет", "", ""};
			auto res = StringUtils::split(str);
			Assert::AreEqual(res.size(), splitStr.size());
			for (int i = 0; i < res.size(); i++)
				Assert::AreEqual(res[i], splitStr[i]);
		}

		TEST_METHOD(stringSplitExtraSpacesNoSaveBlanks)
		{
			std::string str = "  всем  Привет  ";
			std::vector<std::string> splitStr = { "всем", "Привет", };
			auto res = StringUtils::split(str, " ", true);
			Assert::AreEqual(res.size(), splitStr.size());
			for (int i = 0; i < res.size(); i++)
				Assert::AreEqual(res[i], splitStr[i]);
		}

		TEST_METHOD(stringSplitDelim)
		{
			std::string str = "всем|Привет";
			std::vector<std::string> splitStr = { "всем", "Привет" };
			auto res = StringUtils::split(str, "|");
			Assert::AreEqual(res.size(), splitStr.size());
			for (int i = 0; i < res.size(); i++)
				Assert::AreEqual(res[i], splitStr[i]);
		}

	};
}