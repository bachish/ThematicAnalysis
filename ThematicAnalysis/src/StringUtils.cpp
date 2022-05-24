#include "StringUtils.h"

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


