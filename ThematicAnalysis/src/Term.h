#pragma once
#include <string>
#include <vector>

class Term
{
public:
	std::vector<std::wstring> words;
	std::wstring view;
	double weight;

	Term(std::vector<std::wstring> words, std::wstring view);
	size_t getHashCode() const;
	bool operator<(Term const& other) const;
private:
	mutable size_t _hashCode;
	mutable bool _hashCodeCalculated;
};
