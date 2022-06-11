#pragma once
#include <functional>
#include <string>
#include <vector>

#include "Lemmatizer.h"

class TextNormalizer
{
public:
	static std::string clearText(std::string text);
	std::string clearText(std::string text, std::function<bool(char)> isDeleteableChar) const;
	std::string eraseStopWords(std::string text) const;
	std::string toLowerText(std::string word) const;
	std::vector<std::string> normalize(std::string text) const;

private:
	Lemmatizer _lemmatizer;
};
