#pragma once
#include "Lemmatizer.h"
#include "NormalizedArticle.h"
#include "Normalizer.h"
#include "XmlSourceParser.h"

class DocumentReader
{
public:
	// throw std::ifstream::failure when i/o error
	std::vector<NormalizedArticle> readAndNormalizeArticles(std::string const& xmlText) const;
	std::vector<NormalizedArticle> readAndNormalizeArticles(std::string const& articlesText, IXmlConverter const& xmlConverter) const;
	std::vector<std::string> readAndNormalizeText(std::string const& text) const;
private:
	std::string concatTitlesAndContentsWithTags(std::vector<std::string> const& titles, std::vector<std::string> const& contents, size_t approxSize) const;
	std::vector<std::string> getTitle(std::vector<std::string> const& words, size_t& curPos);
	std::vector<NormalizedArticle> normalizeArticles(std::string const& xmlText) const;
	NormalizedArticle createNormalizedArticle(std::string const& title, std::string const& content) const;

	Normalizer _normalizer;
	XmlSourceParser _parser;

	std::string termEndTag = "termendtag";
	std::string contentEndTag = "contentendtag";
};
