#pragma once
#include "Lemmatizer.h"

class DocumentReader
{
public:
	std::vector<NormalizedArticle> read(std::string filePath);
private:
	Lemmatizer _lemmatizer;
};
