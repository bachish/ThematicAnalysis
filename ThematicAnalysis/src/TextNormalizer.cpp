#include "TextNormalizer.h"

#include <algorithm>
#include <execution>
#include <functional>
#include <locale>

#include "Utils/StringUtils.h"

std::string TextNormalizer::clearText(std::string text)
{
	static std::locale loc("ru-RU");
	std::transform(std::execution::par, text.begin(), text.end(), text.begin(), [](char ch) {return isalpha(ch, loc) || isdigit(ch, loc) ? ch : ' '; });
	text.erase(std::unique(text.begin(), text.end(), [](char ch, char ch2) {return ch == ch2 && ch == ' '; }), text.end());
	if (text.size() > 0 && text.front() == ' ') text.erase(text.begin());
	if (text.size() > 0 && text.back() == ' ') text.erase(text.end() - 1);

	return text;
}

std::string TextNormalizer::clearText(std::string text, std::function<bool(char)> isDeleteableChar) const
{
	static std::locale loc("ru-RU");
	std::transform(std::execution::par, text.begin(), text.end(), text.begin(),
		[&isDeleteableChar](char ch) {return isDeleteableChar(ch) ? ' ' : ch; });
	text.erase(std::unique(text.begin(), text.end(), [](char ch, char ch2) {return ch == ch2 && ch == ' '; }), text.end());
	if (text.size() > 0 && text.front() == ' ') text.erase(text.begin());
	if (text.size() > 0 && text.back() == ' ') text.erase(text.end() - 1);

	return text;
}


std::string TextNormalizer::eraseStopWords(std::string text) const
{
	auto words = StringUtils::split(text);
	words.erase(std::remove_if(words.begin(), words.end(), [](std::string& word) { return word.size() < 2; }), words.end());
	return StringUtils::concat(words, " ");
}

std::string TextNormalizer::toLowerText(std::string  word) const
{
	static std::locale loc("ru-RU");
	std::transform(std::execution::par, word.begin(), word.end(), word.begin(), [](char ch) {return tolower(ch, loc); });
	return word;
}


std::vector<std::string> TextNormalizer::normalize(std::string text) const
{
	text = clearText(text);
	text = toLowerText(text);
	text = eraseStopWords(text);
	return _lemmatizer.lemmatizeText(text);
}
