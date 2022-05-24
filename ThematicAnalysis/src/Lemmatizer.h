#pragma once
#include "lemmatizator_engine.h"
#include <string>
#include <vector>

class Lemmatizer
{
public:

	std::vector<std::string> lemmatizeText(const std::string& text) const;
	std::string lemmatizeWord(const std::string& word) const;

};


