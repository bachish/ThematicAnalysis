#include "Utils.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <string>
#include <locale>

size_t Utils::calculateHashCode(std::wstring const& text)
{
	static std::hash<std::wstring> _hashCalculator;
	return _hashCalculator(text);
}
size_t Utils::calculateTermHashCode(std::vector<std::wstring> words)
{
	return calculateHashCode(sortAndConcatWords(words));
}

std::wstring Utils::readAllFile(std::wifstream& fin)
{
	std::wstringstream wss;
	fin.imbue(std::locale("ru-RU"));
	wss << fin.rdbuf();
	return wss.str();
}

std::wstring Utils::sortAndConcatWords(std::vector<std::wstring>  words)
{
	std::sort(words.begin(), words.end());
	return std::accumulate(words.begin(), words.end(), std::wstring(L""));
}
bool isStopChar(wchar_t c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}
void Utils::clearString(std::wstring& s)
{
	for (wchar_t& i : s)
	{
		if (i == '\t' || i == '\n')
			i = ' ';
	}

	auto isTwoSpaces = [](wchar_t a, wchar_t b) {return a == b && a == ' '; };
	s.erase(std::unique(s.begin(), s.end(), isTwoSpaces), s.end());

	if (s.front() == ' ')
		s.erase(s.begin(), s.begin() + 1);
	if (s.back() == ' ')
		s.erase(s.end() - 1, s.end());
}


bool Utils::isLetter(wchar_t c)
{
	return std::isalpha(c, std::locale("ru-RU"));
}
