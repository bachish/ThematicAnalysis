#include "Term.h"
#include "Utils.h"

Term::Term(std::vector<std::wstring> words, std::wstring view)
	: words(std::move(words)),
	view(std::move(view)),
	weight(0),
	_hashCode(0),
	_hashCodeCalculated(false)
{
}

size_t Term::getHashCode() const
{
	if (!_hashCodeCalculated) {
		_hashCode = Utils::calculateHashCode(Utils::sortAndConcatWords(words));
		_hashCodeCalculated = true;
	}
	return _hashCode;
}

bool Term::operator<(Term const& other) const
{
	return this->getHashCode() < other.getHashCode();
}
