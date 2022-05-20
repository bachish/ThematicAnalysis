#pragma once
#include <string>
#include <vector>

class Utils
{
public:
	static size_t calculateHashCode(std::wstring const& text);
	static std::wstring readAllFile( std::ifstream const& fin);
	static std::wstring sortAndConcatWords(std::vector<std::wstring> words);
	static void clearString(std::wstring& s);
	static bool isLetter(wchar_t c);
	static bool isCyrillicLetter(wchar_t c);
};
