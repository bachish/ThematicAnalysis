#include "Term.h"

Term::Term(std::vector<std::string> normalizedWords, std::string view, size_t hash)
	: normalizedWords(std::move(normalizedWords)),
	view(std::move(view)),
	_hashCode(hash),
	numberArticlesThatUseIt(0)
{
}

Term::Term()
{
}

size_t Term::getHashCode() const
{
	return _hashCode;
}


