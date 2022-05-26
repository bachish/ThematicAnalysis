#include "Hasher.h"

#include <algorithm>
#include <execution>
#include <string>
#include <sstream>
#include <vector>

#include "StringUtils.h"


size_t Hasher::sortAndCalcHash(std::vector<std::string> const& words)
{
	return sortAndCalcHash(words.begin(), words.end());
}

size_t Hasher::sortAndCalcHash(std::vector<std::string> const& words, size_t pos, size_t n)
{
	return sortAndCalcHash(words.begin() + static_cast<ptrdiff_t>(pos), words.begin() + static_cast<ptrdiff_t>(pos + n));
}

size_t Hasher::sortAndCalcHash(std::vector<std::string>::const_iterator const& wordsBegin,
	std::vector<std::string>::const_iterator const& wordsEnd)
{
	std::vector<std::vector<std::string>::const_iterator> wordsIts;
	wordsIts.reserve(wordsEnd - wordsBegin);
	for (auto it = wordsBegin; it != wordsEnd; ++it)
		wordsIts.push_back(it);
	std::sort(wordsIts.begin(), wordsIts.end(), [](auto const& it1, auto const& it2) {return *it1 < *it2; });
	std::size_t seed = wordsEnd - wordsBegin;
	for (auto const& strIt : wordsIts)
	{
		static std::hash<std::string> _hashCalculator;
		seed ^= _hashCalculator(*strIt) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;

	//	return calculateHashCode(sortAndConcatWords({ wordsBegin, wordsEnd }));
}


size_t Hasher::calculateHashCode(std::string const& text)
{
	static std::hash<std::string> _hashCalculator;
	return _hashCalculator(text);
}
