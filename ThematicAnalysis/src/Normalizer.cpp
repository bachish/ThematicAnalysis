#include "Normalizer.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <locale>
#include <numeric>
#include <sstream>

std::string deleteMultipleBlanks(std::string text)
{
	static std::locale loc("ru-RU");
	std::transform(text.begin(), text.end(), text.begin(), [](char ch) {return isalpha(ch, loc) || isdigit(ch) ? ch : ' '; });
	text.erase(std::unique(text.begin(), text.end(), [](char ch, char ch2) {return ch == ch2 && ch == ' '; }), text.end());
	return text;
}

inline std::vector<std::string> split(std::string const& text)
{
	std::stringstream ss(text);
	std::vector<std::string> words;
	while (!ss.eof())
	{
		std::string str;
		ss >> str;
		words.push_back(str);
	}
	return words;
}

std::vector<std::string> eraseStopWords(std::vector<std::string> words)
{
	words.erase(std::remove_if(words.begin(), words.end(), [](std::string& word) { return word.size() < 3; }), words.end());
	return words;
}

std::string clearText(std::string  word)
{
	static std::locale loc("ru-RU");
	word.erase(std::remove_if(word.begin(), word.end(), [](char ch) {return !std::isalpha(ch, loc) && !std::isdigit(ch) && !isblank(ch); }), word.end());
	return word;
}

std::string toLowerText(std::string  word)
{
	static std::locale loc("ru-RU");
	std::transform(word.begin(), word.end(), word.begin(), [](char ch) {return tolower(ch, loc); });
	return word;
}

std::vector<std::string> Normalizer::normalize(std::string text) const
{
	text = deleteMultipleBlanks(text);
	text = clearText(text);
	text = toLowerText(text);
	auto words = split(text);
	words = eraseStopWords(words);
	text = std::accumulate(words.begin(), words.end(), std::string(), [](std::string& str, std::string& word) {return str + word + ' '; });
	text.erase(text.end() - 1);
	return _lemmatizer.lemmatizeText(text);
}
