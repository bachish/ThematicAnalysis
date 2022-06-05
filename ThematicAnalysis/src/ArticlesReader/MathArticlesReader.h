#pragma once
#include "IArticlesReader.h"
#include <vector>

class MathArticlesReader : public IArticlesReader
{
public:
	std::tuple<std::vector<std::string>, std::vector<std::string>> read(std::string const& text) const override;
};
