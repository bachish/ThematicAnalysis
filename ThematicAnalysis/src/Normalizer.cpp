#include "Normalizer.h"

#include <algorithm>
#include <execution>
#include <locale>
#include <numeric>

#include "FileManager.h"
#include "StringUtils.h"

std::string deleteMultipleBlanks(std::string text)
{
	static std::locale loc("ru-RU");
	std::transform(std::execution::par, text.begin(), text.end(), text.begin(), [](char ch) {return isalpha(ch, loc) || isdigit(ch) ? ch : ' '; });
	text.erase(std::unique(text.begin(), text.end(), [](char ch, char ch2) {return ch == ch2 && ch == ' '; }), text.end());
	return text;
}


std::vector<std::string> eraseStopWords(std::vector<std::string> words)
{
	words.erase(std::remove_if(words.begin(), words.end(), [](std::string& word) { return word.size() < 2; }), words.end());
	return words;
}

std::string clearText(std::string  word)
{
	static std::locale loc("ru-RU");
	word.erase(std::remove_if(word.begin(), word.end(), [](char ch) {return !isblank(ch, loc) && !std::isdigit(ch, loc) && !std::isalpha(ch, loc); }), word.end());
	return word;
}

std::string toLowerText(std::string  word)
{
	static std::locale loc("ru-RU");
	std::transform(std::execution::par, word.begin(), word.end(), word.begin(), [](char ch) {return tolower(ch, loc); });
	return word;
}


std::vector<std::string> Normalizer::normalize(std::string text) const
{
	text = deleteMultipleBlanks(text);
	text = clearText(text);
	text = toLowerText(text);
	auto words = StringUtils::split(text);
	words = eraseStopWords(words);
	return _lemmatizer.lemmatizeText(StringUtils::concat(words, " "));
}
