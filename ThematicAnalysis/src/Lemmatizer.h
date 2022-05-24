#pragma once
#include "lemmatizator_engine.h"
#include <string>
#include <vector>

class Lemmatizer
{
public:

	std::vector<std::string> lemmatizeText(const std::string& text) const;
	static const size_t CHARS_THRESHOLD;

	std::vector<std::string> lemmatizeTextWithMyStem(const std::string& text) const;

	static const std::string RUS_DICTIONARY_DEFAULT_PATH;
	Lemmatizer();
	Lemmatizer(const std::string& pathToDictionary);
	std::vector<std::string> lemmatizeTextWithSol(const std::string& text) const;
	std::string lemmatizeWord(const std::string& word) const;
	~Lemmatizer();

private:
	HLEM _hEngine;
};


