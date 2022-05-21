#pragma once
#include "Lemmatizer.h"
#include "NormalizedArticle.h"

class DocumentReader
{
public:
	// throw std::ifstream::failure when i/o error
	std::vector<NormalizedArticle> read(std::string const& filePath) const;
private:

	NormalizedArticle createNormalizedArticle(std::string const& title, std::string const& content) const;

	Lemmatizer _lemmatizer;
};
