#pragma once
#include "Term.h"

class NormalizedArticle
{
public:
	std::vector<std::string> titleWords;
	std::string titleView;
	std::vector<std::string> text;
	NormalizedArticle(std::vector<std::string> titleWords, std::string titleView, std::vector<std::string> text);
};
