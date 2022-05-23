#pragma once
#include <string>

class FileManager
{
public:
	static std::string readAllFile(const std::ifstream & fin);
	static void writeUTF8ToFile(std::string const& filePath, std::string const& text);
};
