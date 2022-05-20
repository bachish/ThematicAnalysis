#pragma once
#include "Lemmatizer.h"
#include "NormalizedArticle.h"

class DocumentReader
{
public:
	std::vector<NormalizedArticle> read(std::string filePath);
private:
	Lemmatizer _lemmatizer;
};
