#pragma once
#include <string>
#include <vector>

#include "Lemmatizer.h"

class TextNormalizer
{
public:
	std::string clearText(std::string text) const;
	std::vector<std::string> eraseStopWords(std::vector<std::string> words) const;
	std::string toLowerText(std::string word) const;
	std::vector<std::string> normalize(std::string text) const;

private:
	Lemmatizer _lemmatizer;
};
