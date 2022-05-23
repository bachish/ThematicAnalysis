#include "Hasher.h"

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>


size_t Hasher::sortAndCalcHash(std::vector<std::string> const& words)
{
	return calculateHashCode(sortAndConcatWords(words));
}

size_t Hasher::sortAndCalcHash(std::vector<std::string> const& words, size_t pos, size_t n)
{
	return sortAndCalcHash({ words.begin() + static_cast<ptrdiff_t>(pos), words.begin() + static_cast<ptrdiff_t>(pos + n) });
}

std::string Hasher::sortAndConcatWords(std::vector<std::string> words)
{
	std::sort(words.begin(), words.end());
	return std::accumulate(words.begin(), words.end(), std::string(""));
}

size_t Hasher::calculateHashCode(std::string const& text)
{
	static std::hash<std::string> _hashCalculator;
	return _hashCalculator(text);
}
