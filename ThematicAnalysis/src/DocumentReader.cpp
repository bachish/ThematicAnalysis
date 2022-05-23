#include "DocumentReader.h"

#include "XmlSourceParser.h"
#include <fstream>
#include <sstream>

#include "Utils.h"


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

std::vector<NormalizedArticle> DocumentReader::readAndNormalizeArticles(std::string const& filePath) const
{
	auto fin = OpenFileWithUsingExceptions(filePath);
	auto documentText = Utils::readAllFile(fin);
	auto [titles, contents] = XmlSourceParser().parseTitlesAndContents(documentText);

	std::vector<NormalizedArticle> result;
	for(size_t i = 0; i < titles.size(); i++)
		result.push_back(createNormalizedArticle(titles[i], contents[i]));
	return result;
}


