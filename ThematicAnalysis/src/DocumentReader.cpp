#include "DocumentReader.h"

#include "XmlSourceParser.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "FileManager.h"


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

std::vector<NormalizedArticle> DocumentReader::readAndNormalizeArticles(std::string const& xmlPath) const
{
	auto fin = OpenFileWithUsingExceptions(xmlPath);
	auto xmlText = FileManager::readAllFile(fin);
	return normalizeArticles(xmlText);
}

std::vector<NormalizedArticle> DocumentReader::readAndNormalizeArticles(std::string const& filePath,
	IXmlConverter const& xmlConverter) const
{
	auto xmlText = xmlConverter.convertFileToXml(filePath);
	return normalizeArticles(xmlText);
}


std::string DocumentReader::concatTitlesAndContents(std::vector<std::string> const& titles, std::vector<std::string> const& contents, size_t approxSize) const
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

std::vector<std::string> readUntilWord(std::vector<std::string> const& words, size_t& curPos, std::string const& stopWord)
{
	std::vector<std::string> title;
	title.reserve(10);
	while (curPos < words.size())
	{
		auto curWord = words[curPos];
		curPos++;
		if (curWord == stopWord)
			return title;
		title.push_back(curWord);
	}
}





std::vector<NormalizedArticle> DocumentReader::normalizeArticles(std::string const& xmlText) const
{
	auto [titles, contents] = XmlSourceParser().parseTitlesAndContents(xmlText);
	std::vector<NormalizedArticle> result;
	auto text = concatTitlesAndContents(titles, contents, xmlText.size());
	auto words = _normalizer.normalize(text);

	

	size_t curWord = 1, curTitle = 0;
	while(curWord < words.size())
	{
		auto titleWords = readUntilWord(words, curWord, termEndTag);
		auto contentWords = readUntilWord(words, curWord, contentEndTag);
		result.emplace_back(titleWords, titles[curTitle], contentWords);
		curTitle++;
	}
	
	return result;
}

std::vector<std::string> DocumentReader::readAndNormalizeText(std::string const& filePath) const
{
	auto fin = OpenFileWithUsingExceptions(filePath);
	auto documentText = FileManager::readAllFile(fin);
	return _normalizer.normalize(documentText);
}




