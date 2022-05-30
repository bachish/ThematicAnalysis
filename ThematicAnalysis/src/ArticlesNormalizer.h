#pragma once
#include "Lemmatizer.h"
#include "NormalizedArticle.h"
#include "TextNormalizer.h"
#include "ArticlesReader/IArticlesReader.h"

class ArticlesNormalizer
{
public:
	// throw std::ifstream::failure when i/o error
	std::vector<NormalizedArticle> readAndNormalizeArticles(std::string const& articlesText, IArticlesReader const& articlesReader) const;
private:
	std::string concatTitlesAndContentsWithTags(std::vector<std::string> const& titles, std::vector<std::string> const& contents, size_t approxSize) const;
	std::vector<std::string> getTitle(std::vector<std::string> const& words, size_t& curPos);
	std::vector<NormalizedArticle> normalizeArticles(std::vector<std::string> titles, std::vector<std::string> const& contents) const;
	NormalizedArticle createNormalizedArticle(std::string const& title, std::string const& content) const;

	TextNormalizer _normalizer;

	std::string termEndTag = "termendtag";
	std::string contentEndTag = "contentendtag";
};
