#include "Term.h"
#include "Utils.h"

Term::Term(std::vector<std::string> normalizedWords, std::string view)
	: normalizedWords(std::move(normalizedWords)),
	view(std::move(view)),
	weight(0),
	_hashCode(0),
	_hashCodeCalculated(false)
{
}

size_t Term::getHashCode() const
{
	if (!_hashCodeCalculated) {
		_hashCode = Utils::calculateHashCode(Utils::sortAndConcatWords(normalizedWords));
		_hashCodeCalculated = true;
	}
	return _hashCode;
}


