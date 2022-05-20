#include "pch.h"

#include <fstream>

#include "CppUnitTest.h"
#include "Utils.h"
#include "XmlSourceParser.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
TEST_CLASS(XmlSourceParserTests)
{
	std::wstring readTestFile(std::wstring name)
	{
		return Utils::readAllFile(std::ifstream(L"resources/XmlSourceParser/" + name));
	}
	TEST_METHOD(OnePaper)
	{
		auto parser = XmlSourceParser();
		auto [titles, contents] = parser.parseTitlesAndContents(readTestFile(L"OnePaper"));
		Assert::AreEqual(titles.size(), (size_t)1);
		Assert::AreEqual(contents.size(), (size_t)1);
		Assert::AreEqual(titles[0], std::wstring(L"АБАК"));
		Assert::AreEqual(contents[0], std::wstring(L"Счетная доска применявшаяся арифметич вычислений Древней Греции Риме затем"));

	}
};
