#pragma once
#include <string>
#include <vector>

class Term
{
public:
	std::vector<std::string> normalizedWords;
	std::string view;
	double weight;

	Term(std::vector<std::string> normalizedWords, std::string view);
	size_t getHashCode() const;
	bool operator<(Term const& other) const;
private:
	mutable size_t _hashCode;
	mutable bool _hashCodeCalculated;
};

struct TermComparator
{
	using is_transparent = std::true_type;

	bool operator()(Term const& first, Term const& second) const
	{
		return first.getHashCode() < second.getHashCode();
	}

	bool operator()(Term const& first, size_t second) const
	{
		return first.getHashCode() < second;
	}

	bool operator()(size_t first, Term const& second) const
	{
		return first < second.getHashCode();
	}

};