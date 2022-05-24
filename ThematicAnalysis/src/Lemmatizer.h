#pragma once
#include "lemmatizator_engine.h"
#include <string>
#include <vector>

class Lemmatizer
{
public:
	Lemmatizer();
	explicit Lemmatizer(const std::string& pathToDictionary);
	std::vector<std::string> lemmatizeText(const std::string& text) const;
	std::string lemmatizeWord(const std::string& word) const;
	~Lemmatizer();
	static const std::string RUS_DICTIONARY_DEFAULT_PATH;

private:
	HLEM _hEngine;
};


