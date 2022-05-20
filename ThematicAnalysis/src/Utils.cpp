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

	s.erase(std::unique(s.begin(), s.end(), [](wchar_t a, wchar_t b)
		{return a == b && a == ' '; }), s.end());
	if (s[0] == ' ')
		s.erase(s.begin(), s.begin() + 1);
	const auto end = s.end();
	if (s[s.size() - 1] == ' ')
		s.erase(s.end() - 1, s.end());
}

/**todo fix for wchar 
 */
bool Utils::isLetter(wchar_t c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isCyrillicLetter(c);
}
bool Utils::isCyrillicLetter(wchar_t c)
{
	return (c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я');
}