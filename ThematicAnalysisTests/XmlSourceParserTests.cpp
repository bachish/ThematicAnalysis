#include "pch.h"
#include <fstream>
#include "CppUnitTest.h"
#include "Utils.h"
#include "XmlSourceParser.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
 * \brief
 *please, is you use file as test-source add it into ThematicAnalysisTests.vcxproj
 */
namespace ThematicAnalysisTests
{
	TEST_CLASS(XmlSourceParserTests)
	{
		std::wstring readTestFile(std::wstring name)
		{
			auto path = L"resources/XmlSourceParser/" + name;
			return Utils::readAllFile(std::wifstream(path));
		}
		TEST_METHOD(OnePaper)
		{
			auto parser = XmlSourceParser();
			auto text = readTestFile(L"OnePaper.txt");
			auto [titles, contents] = parser.parseTitlesAndContents(text);
			Assert::AreEqual((size_t)1, titles.size());
			Assert::AreEqual((size_t)1, contents.size());
			Assert::AreEqual(std::wstring(L"АБАК"), titles[0]);
			Assert::AreEqual(std::wstring(L"Счетная доска применявшаяся арифметич вычислений Древней Греции Риме затем"), contents[0]);

		}
		TEST_METHOD(TwoPaper)
		{
			auto parser = XmlSourceParser();
			auto text = readTestFile(L"TwoPaper.txt");
			auto [titles, contents] = parser.parseTitlesAndContents(text);
			Assert::AreEqual((size_t)2, titles.size());
			Assert::AreEqual((size_t)2, contents.size());
			Assert::AreEqual(std::wstring(L"АБАК"), titles[0]);
			Assert::AreEqual(std::wstring(L"Счетная доска применявшаяся арифметич вычислений Древней Греции Риме затем"), contents[0]);
			Assert::AreEqual(std::wstring(L"АБЕЛЕВ ДИФФЕРЕНЦИАЛ"), titles[1]);
			Assert::AreEqual(std::wstring(L"голоморфный мероморфный дифференциал Дифференциал римановой поверхности Пусть"), contents[1]);
		}
	};
}
