#pragma once
#include "Lemmatizer.h"
#include "NormalizedArticle.h"
#include "Normalizer.h"
#include "XmlSourceParser.h"

class DocumentReader
{
public:
	// throw std::ifstream::failure when i/o error
	std::vector<NormalizedArticle> readAndNormalizeArticles(std::string const& xmlPath) const;
	std::vector<NormalizedArticle> readAndNormalizeArticles(std::string const& filePath, IXmlConverter const& xmlConverter) const;
	std::vector<std::string> readAndNormalizeText(std::string const& filePath) const;
private:
	std::vector<NormalizedArticle> normalizeArticles(std::string const& xmlText) const;
	NormalizedArticle createNormalizedArticle(std::string const& title, std::string const& content) const;

	Normalizer _normalizer;
	XmlSourceParser _parser;
};
