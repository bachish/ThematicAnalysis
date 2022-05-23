#include "NormalizedArticle.h"

#include <utility>

NormalizedArticle::NormalizedArticle(std::vector<std::string> titleWords, std::string titleView, std::vector<std::string> text) :
	titleWords(std::move(titleWords)),
	titleView(std::move(titleView)),
	text(std::move(text))
{
}
