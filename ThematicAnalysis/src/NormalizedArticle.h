#pragma once
#include "Term.h"

class NormalizedArticle
{
public:
	Term title;
	std::vector<std::wstring> text;
	NormalizedArticle(Term Title, std::vector<std::wstring> Text);
};
