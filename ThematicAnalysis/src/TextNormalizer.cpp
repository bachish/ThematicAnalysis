#include "TextNormalizer.h"

#include <algorithm>
#include <execution>
#include <functional>
#include <locale>

#include "Utils/StringUtils.h"


std::locale TextNormalizer::_ruLoc = std::locale("ru-RU");

std::string removeRepeatedSpaces(std::string text)
{
	text.erase(std::unique(text.begin(), text.end(), [](char ch, char ch2) {return ch == ch2 && ch == ' '; }), text.end());
	if (!text.empty() && text.front() == ' ') text.erase(text.begin());
	if (!text.empty() && text.back() == ' ') text.erase(text.end() - 1);

	return text;
}

std::string TextNormalizer::clearText(std::string text)
{
	std::transform(std::execution::par, text.begin(), text.end(), text.begin(), [](char ch) {return isalpha(ch, _ruLoc) || isdigit(ch, _ruLoc) ? ch : ' '; });
	return removeRepeatedSpaces(text);
}

std::string TextNormalizer::clearText(std::string text, std::function<bool(char)> isDeleteableChar) const
{
	std::transform(std::execution::par, text.begin(), text.end(), text.begin(),
		[&isDeleteableChar](char ch) {return isDeleteableChar(ch) ? ' ' : ch; });
	return removeRepeatedSpaces(text);
}


std::string TextNormalizer::eraseStopWords(std::string const& text) const
{
	auto words = StringUtils::split(text);
	words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word) { return word.size() < 2; }), words.end());
	return StringUtils::concat(words, " ");
}

std::string TextNormalizer::toLowerText(std::string  word) const
{
	std::transform(std::execution::par, word.begin(), word.end(), word.begin(), [](char ch) {return tolower(ch, _ruLoc); });
	return word;
}


std::vector<std::string> TextNormalizer::normalize(std::string text) const
{
	text = clearText(text);
	text = toLowerText(text);
	text = eraseStopWords(text);
	return _lemmatizer.lemmatizeText(text);
}
