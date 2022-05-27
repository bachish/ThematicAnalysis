#include "pch.h"
#include <fstream>
#include "CppUnitTest.h"
#include "FileManager.h"
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
		TEST_CLASS_INITIALIZE(setLocale)
		{
			setlocale(LC_ALL, "rus");
		}
		std::string readTestFile(std::string name)
		{
			auto path = "resources/XmlSourceParser/" + name;
			return FileManager::readAllFile(std::ifstream(path));
		}
		TEST_METHOD(OnePaper)
		{
			auto parser = XmlSourceParser();
			auto text = readTestFile("OnePaper.txt");
			auto [titles, contents] = parser.parseTitlesAndContentsFromXml(text);
			Assert::AreEqual((size_t)1, titles.size());
			Assert::AreEqual((size_t)1, contents.size());
			Assert::AreEqual(std::string("АБАК"), titles[0]);
			Assert::AreEqual(std::string("Счетная доска применявшаяся арифметич вычислений Древней Греции Риме затем"), contents[0]);

		}
		TEST_METHOD(TwoPaper)
		{
			auto parser = XmlSourceParser();
			auto text = readTestFile("TwoPaper.txt");
			auto [titles, contents] = parser.parseTitlesAndContentsFromXml(text);
			Assert::AreEqual((size_t)2, titles.size());
			Assert::AreEqual((size_t)2, contents.size());
			Assert::AreEqual(std::string("АБАК"), titles[0]);
			Assert::AreEqual(std::string("Счетная доска применявшаяся арифметич вычислений Древней Греции Риме затем"), contents[0]);
			Assert::AreEqual(std::string("АБЕЛЕВ ДИФФЕРЕНЦИАЛ"), titles[1]);
			Assert::AreEqual(std::string("голоморфный мероморфный дифференциал Дифференциал римановой поверхности Пусть"), contents[1]);
		}
	};
}
