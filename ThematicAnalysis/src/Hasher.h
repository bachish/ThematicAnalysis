#pragma once
#include <string>
#include <vector>

class Hasher
{
public:
	static size_t sortAndCalcHash(std::vector<std::string> const& words);
	static size_t sortAndCalcHash(std::vector<std::string> const& words, size_t pos, size_t n);

private:
	static std::string sortAndConcatWords(std::vector<std::string> words);
	static size_t calculateHashCode(std::string const& text);

};
