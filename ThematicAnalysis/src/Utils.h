#pragma once
#include <string>
#include <vector>

class Utils
{
public:
	static size_t calculateHashCode(std::string const& text);
	static size_t calculateTermHashCode(std::vector<std::string> words);
	static std::string readAllFile( std::ifstream & fin);
	static std::string sortAndConcatWords(std::vector<std::string> words);
	static void clearString(std::string& s);
	static bool isLetter(char c);
};
