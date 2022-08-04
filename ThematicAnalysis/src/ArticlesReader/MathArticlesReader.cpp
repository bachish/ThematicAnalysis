#include <boost/regex.hpp>
#include <regex>
#include <sstream>
#include "MathArticlesReader.h"
#include "TextNormalizer.h"

struct SubString
{
	int position, lenght;
};

std::vector<std::tuple<std::string::const_iterator, std::string::const_iterator>> MathArticlesReader::getTermsPositions(std::string const& text) const
{
	boost::regex regex(_mathRegex);
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

std::tuple<std::vector<std::string>, std::vector<std::string>> MathArticlesReader::read(std::string const& text) const
{
	static std::locale loc("ru-RU");
	std::string saveChars = "-—:\n\r";
	auto clearedText = TextNormalizer().clearText(text, [&saveChars](char ch){return !isalpha(ch, loc) && !isdigit(ch, loc) && saveChars.find(ch) == std::string::npos;});
	auto termsPositions = getTermsPositions(clearedText);
	std::vector<std::string> titles, contents;
	titles.reserve(termsPositions.size()), contents.reserve(termsPositions.size());
	std::string::const_iterator prevTermEnd;
	for (auto it = termsPositions.cbegin(); it != termsPositions.cend(); ++it)
	{
		auto& [termStart, termEnd] = *it;
		auto& nextTermStart = std::next(it) == termsPositions.end() ? clearedText.cend() : std::get<0>(*std::next(it));
		titles.emplace_back(termStart, termEnd);
		contents.emplace_back(termEnd, nextTermStart);
	}
	return { titles, contents };
}
