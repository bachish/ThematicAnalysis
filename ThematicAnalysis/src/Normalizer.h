#pragma once
#include <string>
#include <vector>

#include "Lemmatizer.h"

class Normalizer
{
public:
	std::vector<std::string> normalize(std::string const& text) const;

private:
	Lemmatizer _lemmatizer;
};
