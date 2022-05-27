#include "DocumentReader.h"

#include "XmlSourceParser.h"
#include <fstream>
#include <iostream>



std::ifstream OpenFileWithUsingExceptions(std::string const& filePath)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fin.open(filePath);
	return fin;
}

NormalizedArticle DocumentReader::createNormalizedArticle(std::string const& title, std::string const& content) const
{
	return { _normalizer.normalize(title), title, _normalizer.normalize(content) };
}

std::vector<NormalizedArticle> DocumentReader::readAndNormalizeArticles(std::string const& xmlText) const
{
	return normalizeArticles(xmlText);
}

std::vector<NormalizedArticle> DocumentReader::readAndNormalizeArticles(std::string const& articlesText,
	IXmlConverter const& xmlConverter) const
{
	auto xmlText = xmlConverter.convertTextToXml(articlesText);
	return normalizeArticles(xmlText);
}

std::vector<std::string> DocumentReader::readAndNormalizeText(std::string const& text) const
{
	return _normalizer.normalize(text);
}



std::string DocumentReader::concatTitlesAndContentsWithTags(std::vector<std::string> const& titles, std::vector<std::string> const& contents, size_t approxSize) const
{
	std::string res;
	res.reserve(approxSize);
	for (int i = 0; i < titles.size(); i++)
	{
		res += titles[i];
		res += ' ' + termEndTag + ' ';
		res += contents[i];
		res += ' ' + contentEndTag + ' ';
	}
	return res;
}

using StrVecIt = std::vector<std::string>::const_iterator;
std::vector<std::string> readUntilWord(std::vector<std::string> const& words, StrVecIt& curIt, std::string const& stopWord)
{
	std::vector<std::string> title;
	auto stopWordIt = std::find(curIt, words.end(), stopWord);
	auto subWords = std::vector<std::string>{ curIt, stopWordIt };
	curIt = stopWordIt + 1;
	return subWords;
}

std::vector<NormalizedArticle> DocumentReader::normalizeArticles(std::string const& xmlText) const
{
	auto [titles, contents] = XmlSourceParser().parseTitlesAndContentsFromXml(xmlText);
	std::vector<NormalizedArticle> result;
	auto text = concatTitlesAndContentsWithTags(titles, contents, xmlText.size());
	auto words = _normalizer.normalize(text);

	size_t curTitle = 0;
	StrVecIt curIt = words.begin();
	while (curIt != words.end())
	{
		auto titleWords = readUntilWord(words, curIt, termEndTag);
		auto contentWords = readUntilWord(words, curIt, contentEndTag);
		result.emplace_back(titleWords, titles[curTitle], contentWords);
		curTitle++;
	}

	return result;
}






