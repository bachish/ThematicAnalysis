#pragma once
#include <string>
#include <vector>

class Utils
{
public:
	static size_t calculateHashCode(std::wstring const& text);
	static std::wstring readAllFile( std::wifstream const& fin);
	static std::wstring sortAndConcatWords(std::vector<std::wstring> words);
};
