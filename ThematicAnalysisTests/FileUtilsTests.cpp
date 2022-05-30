#include "pch.h"
#include "CppUnitTest.h"
#include "ArticlesNormalizer.h"
#include <fstream>
#include <filesystem>

#include "FileManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThematicAnalysisTests
{
	TEST_CLASS(FileUtilsTests)
	{
	public:

		TEST_METHOD(executeFile)
		{
			std::string output;
			auto res = FileManager::executeExeWithParams("resources\\FileUtils\\Test.exe", " arg1  arg2 ", output);
			Assert::AreEqual(true, res);
			Assert::AreEqual({ "You give me resources\\FileUtils\\Test.exe arg1 arg2" }, output);
		}

		TEST_METHOD(canNotExecuteFile)
		{
			std::string output;
			FILE* f = _popen("no", "r");
			auto r = _pclose(f);
			auto res = FileManager::executeExeWithParams(" notExist.exe ", "", output);
			Assert::AreEqual(false, res);
		}

		TEST_METHOD(cp1251File)
		{
			FileManager::writeToFile("temp.txt", "Привет!");
			auto res = FileManager::readAllFile("temp.txt");
			Assert::AreEqual({ "Привет!" }, res);
			std::filesystem::remove("temp.txt");
		}
		TEST_METHOD(UTF8File)
		{
			FileManager::writeUTF8ToFile("temp.txt", "Привет!");
			auto res = FileManager::readAllUTF8File("temp.txt");
			Assert::AreEqual({ "Привет!" }, res);
			std::filesystem::remove("temp.txt");
		}
	};
}
