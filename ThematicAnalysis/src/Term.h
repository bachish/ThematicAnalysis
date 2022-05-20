#pragma once
#include <string>
#include <vector>

class Term
{
public:
	std::vector<std::wstring> words;
	std::wstring view;
	double weight;
	size_t getHashCode();
private:
	size_t _hashCode;
};
