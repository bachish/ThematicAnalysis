#pragma once
#include <string>
#include <vector>

class Utils
{
public:
	static size_t calculateHashCode(std::string const& text);
	static size_t calculateTermHashCode(std::vector<std::string> words);
	static size_t calculateNormalizedHashCode(std::string const& text);
	static std::string readAllFile( std::ifstream & fin);
	static void writeToFile(std::string const& filePath, std::string const& text);
	static std::string sortAndConcatWords(std::vector<std::string> words);
	static void clearString(std::string& s);
	static bool isLetter(char c);
	static void drawGraphImageFromDot(std::string dot, std::string imagePath);
};
