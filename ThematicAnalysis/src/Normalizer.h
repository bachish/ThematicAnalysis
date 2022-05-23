#pragma once
#include <string>
#include <vector>

#include "Lemmatizer.h"

class Normalizer
{
public:
	std::vector<std::string> normalize(std::string text) const;

private:
	Lemmatizer _lemmatizer;
};
