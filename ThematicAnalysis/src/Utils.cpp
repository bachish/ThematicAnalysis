#include "Utils.h"
#include <sstream>
#include <fstream>

#include <algorithm>
#include <numeric>
#include <string>


size_t Utils::calculateHashCode(std::wstring const& text)
{
	static std::hash<std::wstring> _hashCalculator;
	return _hashCalculator(text);
}



std::wstring Utils::readAllFile( std::ifstream const& fin)
{
	std::wstringstream wss;
	wss << fin.rdbuf();
	return wss.str();
}

std::wstring Utils::sortAndConcatWords(std::vector<std::wstring>  words)
{
	std::sort(words.begin(), words.end());
	return std::accumulate(words.begin(), words.end(), std::wstring(L""));
}

