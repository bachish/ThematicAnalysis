#include "Hasher.h"
#include <algorithm>
#include <execution>
#include <string>
#include <sstream>
#include <vector>

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
	std::vector<long long> wordsIts(wordsEnd - wordsBegin);
	std::iota(wordsIts.begin(), wordsIts.end(), 0ll);
	std::sort(wordsIts.begin(), wordsIts.end(), [&wordsBegin](auto it1, auto it2) {return *(wordsBegin + it1) < *(wordsBegin + it2); });
	std::size_t seed = wordsEnd - wordsBegin;
	for (auto const& strIt : wordsIts)
	{
		static std::hash<std::string> hashCalculator;
		seed ^= hashCalculator(*(wordsBegin + strIt)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}



