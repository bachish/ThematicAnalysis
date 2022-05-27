#pragma once
#include <string>

class IArticlesReader
{
public:
	virtual std::tuple<std::vector<std::string>, std::vector<std::string>> read(std::string const& text) const = 0;
	virtual ~IArticlesReader() = default;
};
