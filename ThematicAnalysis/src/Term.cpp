#include "Term.h"

Term::Term(std::vector<std::string> normalizedWords, std::string view, size_t hash)
	: normalizedWords(std::move(normalizedWords)),
	view(std::move(view)),
	numberOfArticlesThatUseIt(0),
	_hashCode(hash)
{
}

size_t Term::getHashCode() const
{
	return _hashCode;
}


