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
			auto path = L"resources/" + name;
			return Utils::readAllFile(std::ifstream(path));
		}
		TEST_METHOD(OnePaper)
		{
			auto parser = XmlSourceParser();
			auto text = LR"(<paper>
				<paperName>
				<name>
				АБАК 
				</name>
				<detail>
				</detail>
				</paperName>
				<paperDef>
				Счетная доска применявшаяся арифметич 
				</paperDef>
				<content>
				вычислений Древней Греции Риме затем 
				</content>
				</paper>)";
			auto [titles, contents] = parser.parseTitlesAndContents(text);
			Assert::AreEqual((size_t)1, titles.size());
			Assert::AreEqual((size_t)1, contents.size());
			Assert::AreEqual(std::wstring(L"АБАК"), titles[0]);
			Assert::AreEqual(std::wstring(L"Счетная доска применявшаяся арифметич вычислений Древней Греции Риме затем"), contents[0]);

		}
	};
}
