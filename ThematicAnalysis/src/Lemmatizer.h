#pragma once
#include "lemmatizator_engine.h"
#include <string>
#include <vector>

class Lemmatizer
{
public:
	Lemmatizer();
	explicit Lemmatizer(const std::wstring& pathToDictionary);
	std::vector<std::wstring> lemmatizeText(const std::wstring& text) const;
	~Lemmatizer();
	static const std::wstring RUS_DICTIONARY_DEFAULT_PATH;

private:
	HLEM _hEngine;
};


