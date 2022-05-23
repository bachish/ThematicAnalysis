#include "DocumentReader.h"

#include "XmlSourceParser.h"
#include <fstream>
#include <sstream>

#include "FileManager.h"


std::ifstream OpenFileWithUsingExceptions( std::string const& filePath)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fin.open(filePath);
	return fin;
}

NormalizedArticle DocumentReader::createNormalizedArticle(std::string const& title, std::string const& content) const
{
	return {_normalizer.normalize(title), title, _normalizer.normalize(content)};
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

std::vector<NormalizedArticle> DocumentReader::normalizeArticles(std::string const& xmlText) const
{
	auto [titles, contents] = XmlSourceParser().parseTitlesAndContents(xmlText);
	std::vector<NormalizedArticle> result;
	for(size_t i = 0; i < titles.size(); i++)
		result.push_back(createNormalizedArticle(titles[i], contents[i]));
	return result;
}

std::vector<std::string> DocumentReader::readAndNormalizeText(std::string const& filePath) const
{
	auto fin = OpenFileWithUsingExceptions(filePath);
	auto documentText = FileManager::readAllFile(fin);
	return _normalizer.normalize(documentText);
}




