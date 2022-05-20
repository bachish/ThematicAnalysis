#include "pch.h"
#include "CppUnitTest.h"
#include "XmlSourceParser.h"

TEST_CLASS(XmlSourceParserTests)
{
	TEST_METHOD(OnePaper)
	{
		auto parser = XmlSourceParser();
		auto [titles, contents] = parser.parseTitlesAndContents(LR"(");
	}
};
