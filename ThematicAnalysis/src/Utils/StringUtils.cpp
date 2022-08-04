#include "StringUtils.h"
#include <algorithm>
#include <execution>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


std::vector<std::string> StringUtils::split(std::string const& text, std::string const& delim, bool removeBlanks)
{
	std::vector<std::string> words;
	words.reserve(text.size()/8);
	auto start = 0ull;
	auto end = text.find(delim);
	while (end != std::string::npos)
	{
		if (!removeBlanks || start != end)
			words.push_back(text.substr(start, end - start));
		start = end + delim.length();
		end = text.find(delim, start);
	}
	if (!removeBlanks || start != text.size())
		words.push_back(text.substr(start));

	return words;
}

size_t getSumSize(std::vector<std::string> const& vec)
{
	return std::transform_reduce(std::execution::par, vec.begin(), vec.end(), 0ull, 
		[](size_t a, size_t b){return a+b;},
		[](std::string const& str){return str.size();});
}


std::string StringUtils::concat(std::vector<std::string> const& words, std::string const& delim)
{
	std::string res;
	if (words.empty()) return res;

	res.reserve(words.size() * 10 + words.size() * delim.size());
	std::for_each(words.begin(), words.end() - 1, [&res, &delim](std::string const& word) {res.append(word + delim); });
	res.append(words.back());
	return res;
}

