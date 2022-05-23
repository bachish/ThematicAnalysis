#include "pch.h"
#include "CppUnitTest.h"
#include "DocumentReader.h"
#include <fstream>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(FocumentReaderTests)
	{
	public:

		TEST_METHOD(ExceptionOnOpenNotExistFile)
		{
			DocumentReader reader;
			Assert::ExpectException<std::ifstream::failure>([&reader]() {reader.readAndNormalizeArticles("not exist file");});
			Assert::ExpectException<std::ifstream::failure>([&reader]() {reader.readAndNormalizeText("not exist file");});
		}
	};
}
