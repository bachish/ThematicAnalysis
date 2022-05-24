#include "StringUtils.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> StringUtils::split(std::string const& text, std::string const& delim, bool removeBlanks)
{
	auto curIt = text.begin();
	std::vector<std::string> words;

	auto start = 0U;
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
	size_t size = 0;
	for (auto&& word : vec)
		size += word.size();
	return size;
}

std::string StringUtils::concat(std::vector<std::string> const& words, std::string delim)
{
	std::string res;
	if (words.empty()) return res;

	res.reserve(getSumSize(words) + words.size() * delim.size());
	std::for_each(words.begin(), words.end() - 1, [&res, &delim](std::string const& word) {res.append(word + delim); });
	res.append(words.back());
	return res;
}



