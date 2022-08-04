#pragma once
#include <string>
#include <vector>

class Hasher
{
public:
	static size_t sortAndCalcHash(std::vector<std::string> const& words);
	static size_t sortAndCalcHash(std::vector<std::string> const& words, size_t pos, size_t n);
	static size_t sortAndCalcHash(std::vector<std::string>::const_iterator const& wordsBegin, std::vector<std::string>::const_iterator const& wordsEnd);
};
