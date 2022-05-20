﻿#include "DocumentReader.h"
#include "XmlSourceParser.h"
#include <fstream>
#include <sstream>


std::wstring ReadAllFile( std::ifstream const& fin)
{
	std::wstringstream wss;
	wss << fin.rdbuf();
	return wss.str();
}

std::ifstream OpenFileWithUsingExceptions( std::string const& filePath)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fin.open(filePath);
	return fin;
}

NormalizedArticle DocumentReader::createNormalizedArticle(std::wstring const& title, std::wstring const& content) const
{
	auto contentNormWords = _lemmatizer.lemmatizeText(content);
	auto titleTerm = Term(_lemmatizer.lemmatizeText(title), title);
	return {titleTerm, contentNormWords};
}

std::vector<NormalizedArticle> DocumentReader::read(std::string const& filePath) const
{
	auto fin = OpenFileWithUsingExceptions(filePath);
	auto documentText = ReadAllFile(fin);
	auto [titles, contents] = XmlSourceParser().parseTitlesAndContents(documentText);

	std::vector<NormalizedArticle> result;
	for(size_t i = 0; i < titles.size(); i++)
		result.push_back(createNormalizedArticle(titles[i], contents[i]));
	return result;
}

