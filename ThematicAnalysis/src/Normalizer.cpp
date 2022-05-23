#include "Normalizer.h"

std::vector<std::string> Normalizer::normalize(std::string const& text) const
{
	return _lemmatizer.lemmatizeText(text);
}
