#include "ArticlesNormalizer.h"
#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>

std::ifstream openFileWithUsingExceptions(std::string const& filePath)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fin.open(filePath);
	return fin;
}

NormalizedArticle ArticlesNormalizer::createNormalizedArticle(std::string const& title, std::string const& content) const
{
	return { _normalizer.normalize(title), title, _normalizer.normalize(content) };
}

std::vector<NormalizedArticle> ArticlesNormalizer::readAndNormalizeArticles(std::string const& articlesText,
	IArticlesReader const& articlesReader) const
{
	auto [titles, contents] = articlesReader.read(articlesText);
	return normalizeArticles(titles, contents);
}

std::string ArticlesNormalizer::concatTitlesAndContentsWithTags(std::vector<std::string> const& titles, std::vector<std::string> const& contents, size_t approxSize) const
{
	std::string res;
	res.reserve(approxSize);
	for (size_t i = 0; i < titles.size(); i++)
	{
		res += titles[i];
		res += ' ' + _termEndTag + ' ';
		res += contents[i];
		res += ' ' + _contentEndTag + ' ';
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

std::vector<NormalizedArticle> ArticlesNormalizer::normalizeArticles(std::vector<std::string> titles, std::vector<std::string> const& contents) const
{
	std::transform(std::execution::par, titles.begin(), titles.end(), titles.begin(), [](std::string const& title) {return
		               TextNormalizer::clearText(title); });
	auto text = concatTitlesAndContentsWithTags(titles, contents, titles.size());
	auto words = _normalizer.normalize(text);

	size_t curTitle = 0;
	StrVecIt curIt = words.begin();
	std::vector<NormalizedArticle> result;
	while (curIt != words.end())
	{
		auto titleWords = readUntilWord(words, curIt, _termEndTag);
		auto contentWords = readUntilWord(words, curIt, _contentEndTag);
		result.emplace_back(titleWords, titles[curTitle], contentWords);
		curTitle++;
	}

	return result;
}