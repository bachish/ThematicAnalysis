#include "pch.h"
#include "CppUnitTest.h"
#include "ArticlesReader/XmlArticlesReader.h"
#include "ArticlesReader/MathArticlesReader.h"
#include <filesystem>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(ArticlesReaderTests)
	{
	public:
		TEST_CLASS_INITIALIZE(setLocale)
		{
			setlocale(LC_ALL, "rus");
		}

		TEST_METHOD(xmlArticlesReader)
		{
			XmlArticlesReader reader;
			auto [titles, contents] = reader.read(
				"<paper>"
				"<name>"
				"\t Термин\t\n		.\n"
				"</name>"
				"<content>"
				" Нет термина  важней, чем  термин  \n"
				"</content>"
				"</paper>"
				"<paper>"
				"<name>"
				"\t Термин два\t\n		.\n"
				"</name>"
				"<content>"
				"	Термин два лучше,\t чем просто термин \n \n"
				"</content>"
				"</paper>");
			Assert::AreEqual(2ull, titles.size());
			Assert::AreEqual(2ull, contents.size());
			Assert::AreEqual({ "\t Термин\t\n		.\n" }, titles[0]);
			Assert::AreEqual({ " Нет термина  важней, чем  термин  \n" }, contents[0]);
			Assert::AreEqual({ "\t Термин два\t\n		.\n" }, titles[1]);
			Assert::AreEqual({ "	Термин два лучше,\t чем просто термин \n \n" }, contents[1]);
		}

		TEST_METHOD(mathArticlesReader)
		{
			MathArticlesReader reader;
			auto [titles, contents] = reader.read(
				"\n\n"
				"ТЕРМИН — очень \t\n интересный Термин. \n\n"
			"ТЕРМИН ДВА три четыре — ля - ля.");
			Assert::AreEqual(2ull, titles.size());
			Assert::AreEqual(2ull, contents.size());
			Assert::AreEqual({ "ТЕРМИН" }, titles[0]);
			Assert::AreEqual({ " — очень \t\n интересный Термин. \n\n" }, contents[0]);
			Assert::AreEqual({ "ТЕРМИН ДВА" }, titles[1]);
			Assert::AreEqual({ " три четыре — ля - ля." }, contents[1]);
		}

	};
}
