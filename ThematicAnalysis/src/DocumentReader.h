#pragma once
#include "Lemmatizer.h"
#include "NormalizedArticle.h"
#include "Normalizer.h"
#include "XmlSourceParser.h"

class DocumentReader
{
public:
	// throw std::ifstream::failure when i/o error
	std::vector<NormalizedArticle> readAndNormalizeArticles(std::string const& filePath) const;
	std::vector<std::string> readAndNormalizeText(std::string const& filePath) const;
private:

	NormalizedArticle createNormalizedArticle(std::string const& title, std::string const& content) const;

	Normalizer _normalizer;
	XmlSourceParser _parser;
};
