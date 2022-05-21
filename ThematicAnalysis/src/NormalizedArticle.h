#pragma once
#include "Term.h"

class NormalizedArticle
{
public:
	Term title;
	std::vector<std::string> text;
	NormalizedArticle(Term Title, std::vector<std::string> Text);
};
