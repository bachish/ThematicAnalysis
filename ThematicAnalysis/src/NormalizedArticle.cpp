#include "NormalizedArticle.h"

#include <utility>

NormalizedArticle::NormalizedArticle(Term article_title, std::vector<std::wstring> article_text) :
	title(std::move(article_title)),
	text(std::move(article_text))
{
}
