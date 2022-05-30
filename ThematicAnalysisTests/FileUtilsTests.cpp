#include "pch.h"
#include "CppUnitTest.h"
#include "ArticlesNormalizer.h"
#include <fstream>
#include <filesystem>
#include "Utils/FileUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(FileUtilsTests)
	{
	public:

		TEST_METHOD(executeFile)
		{
			std::string output;
			auto res = FileUtils::executeExeWithParams("resources\\FileUtils\\Test.exe", " arg1  arg2 ", output);
			Assert::AreEqual(true, res);
			Assert::AreEqual({ "You give me resources\\FileUtils\\Test.exe arg1 arg2" }, output);
		}

		TEST_METHOD(canNotExecuteFile)
		{
			std::string output;
			FILE* f = _popen("no", "r");
			auto r = _pclose(f);
			auto res = FileUtils::executeExeWithParams(" notExist.exe ", "", output);
			Assert::AreEqual(false, res);
		}

		TEST_METHOD(cp1251File)
		{
			FileUtils::writeToFile("temp.txt", "Привет!");
			auto res = FileUtils::readAllFile("temp.txt");
			Assert::AreEqual({ "Привет!" }, res);
			std::filesystem::remove("temp.txt");
		}
		TEST_METHOD(UTF8File)
		{
			FileUtils::writeUTF8ToFile("temp.txt", "Привет!");
			auto res = FileUtils::readAllUTF8File("temp.txt");
			Assert::AreEqual({ "Привет!" }, res);
			std::filesystem::remove("temp.txt");
		}
	};
}
