#pragma once
#include <string>

class FileManager
{
public:
	static std::string readAllFile(const std::ifstream & fin);
	static std::string readAllUTF8File(std::string const& filePath);
	static std::string readAllFile(std::string const& filePath);
	static void writeUTF8ToFile(std::string const& filePath, std::string const& text);
	static void writeToFile(std::string const& filePath, std::string const& text);
	static bool executeExeWithParams(std::string exe, std::string params, std::string& output);
};
