#include <boost/regex.hpp>
#include <iostream>
#include <regex>
#include <sstream>

#include "MathXmlConverter.h"

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

std::string MathXmlConverter::convertTextToXml(std::string const& sourceText) const
{
	auto text = sourceText;
	text.erase(std::remove(text.begin(), text.end(), '<'), text.end());
	text.erase(std::remove(text.begin(), text.end(), '>'), text.end());
	text.erase(std::remove(text.begin(), text.end(), '/'), text.end());
	auto termsPositions = GetTermsPositions(text);
	std::stringstream ss;
	std::string::const_iterator prevTermEnd;
	for (size_t i = 0; i < termsPositions.size(); i++)
	{
		auto& [termStart, termEnd] = termsPositions[i];
		if (i != 0)
			ss << "\n<content>\n" << std::string(prevTermEnd, termStart) << "\n</content>\n</paper>\n";
		ss << "\n<paper>\n<name>\n" << std::string(termStart, termEnd) << "\n</name>\n";
		prevTermEnd = termEnd;
	}
	ss << "\n<content>\n" << std::string(prevTermEnd, text.cend()) << "\n</content>\n</paper>\n";
	return ss.str();
}
