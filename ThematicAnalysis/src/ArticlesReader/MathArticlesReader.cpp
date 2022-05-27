#include <boost/regex.hpp>
#include <iostream>
#include <regex>
#include <sstream>

#include "MathArticlesReader.h"

struct SubString
{
	int position, lenght;
};

auto GetTermsPositions(std::string const& text)
{
	boost::regex regex(R"(\r?\n\r?\n([А-Я]{2,}(?:[—\-\s,.]{1,5}(?:[А-Я]{2,}|[\d]+))*)(?:[^а-я0-9][а-яA-Za-z,.;()\-\s\d]*)?—[\s]*[а-я\d])");
	std::stringstream res;
	boost::smatch match;
	std::vector<std::tuple<std::string::const_iterator, std::string::const_iterator>> terms;
	for (auto it = boost::sregex_iterator(text.begin(), text.end(), regex); it != boost::sregex_iterator(); ++it)
	{
		boost::regex_match(text.cbegin() + it->position(), text.cbegin() + it->position() + it->length(), match, regex);
		terms.emplace_back(match[1].begin(), match[1].end());
	}
	return terms;
}

std::tuple<std::vector<std::string>, std::vector<std::string>> MathArticlesReader::read(std::string const& sourceText) const
{
	auto text = sourceText;
	text.erase(std::remove(text.begin(), text.end(), '<'), text.end());
	text.erase(std::remove(text.begin(), text.end(), '>'), text.end());
	text.erase(std::remove(text.begin(), text.end(), '/'), text.end());
	auto termsPositions = GetTermsPositions(text);
	std::vector<std::string> titles, contents;
	titles.reserve(termsPositions.size()), contents.reserve(termsPositions.size());
	std::string::const_iterator prevTermEnd;
	for (auto it = termsPositions.cbegin(); it != termsPositions.cend(); ++it)
	{
		auto& [termStart, termEnd] = *it;
		auto& nextTermStart = std::next(it) == termsPositions.end() ? text.cend() : std::get<0>(*std::next(it));
		titles.emplace_back(termStart, termEnd);
		contents.emplace_back(termEnd, nextTermStart);
	}

	return { titles, contents };
}
