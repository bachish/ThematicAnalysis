#pragma once
#include <string>
#include <vector>

class StringUtils
{
public:
	static std::vector<std::string> split(std::string const& text, std::string const& delim = " ", bool removeBlanks = false);
	static std::string concat(std::vector<std::string> const& words, std::string const& delim = "");
};
