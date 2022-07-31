#pragma once
#include "IArticlesReader.h"
#include <vector>
#include <boost/regex.hpp>
#include <regex>
class MathArticlesReader : public IArticlesReader
{
public:
	std::tuple<std::vector<std::string>, std::vector<std::string>> read(std::string const& text) const override;
private:
	std::string _mathRegex = R"(\r?\n\r?\n([А-ЯЁ]{2,}(?:[^А-ЯЁа-яёA-Za-z]{1,5}[А-ЯЁ\d]{2,})+|[А-ЯЁ]{3,})(?:[^А-ЯЁ\-—][А-ЯЁ]?)*[\-—:;])";
	std::vector<std::tuple<std::string::const_iterator, std::string::const_iterator>> getTermsPositions(std::string const& text) const;
};
