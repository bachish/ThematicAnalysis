#pragma once
#include <string>
#include <vector>

class Lemmatizer
{
public:

	std::vector<std::string> lemmatizeText(const std::string& text) const;
private: 
	std::vector<std::string> lemmatizeTextWithMyStem(const std::string& text) const;
};


