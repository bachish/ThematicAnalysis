#include "pch.h"

#include <fstream>


#include "CppUnitTest.h"
#include "Utils.h"
#include "XmlSourceParser.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
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
			Assert::AreEqual(std::wstring(L"����"), titles[0]);
			Assert::AreEqual(std::wstring(L"������� ����� ������������� ��������� ���������� ������� ������ ���� �����"), contents[0]);

		}
	};
}
