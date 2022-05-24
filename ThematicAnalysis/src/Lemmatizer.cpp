#include "Lemmatizer.h"

#include <algorithm>
#include <locale>
#include <lemmatizator_engine.h>
#include <memory>

const std::string Lemmatizer::RUS_DICTIONARY_DEFAULT_PATH = "external/lemmatizator/x64/lemmatizer.db";

Lemmatizer::Lemmatizer() :Lemmatizer(RUS_DICTIONARY_DEFAULT_PATH)
{
}

Lemmatizer::Lemmatizer(const std::string& pathToDictionary)
{
	_hEngine = sol_LoadLemmatizatorA(pathToDictionary.c_str(), LEME_DEFAULT);
	if(_hEngine == nullptr)
		throw std::ios_base::failure("Can not open dictionary");
}

std::vector<std::string> Lemmatizer::lemmatizeText(const std::string& text) const
{
	std::vector<std::string> words;
	int bufSize = static_cast<int>((text.length() + 100) * 1.5);
	auto strBuf = std::unique_ptr<char[]>(new char[bufSize]);
	auto hWords = sol_LemmatizePhraseA(_hEngine, text.c_str(), 0, L' ');
	if (hWords != nullptr)
	{
		auto wordsCount = sol_CountLemmas(hWords);

		for (int i = 0; i < wordsCount; ++i)
		{
			sol_GetLemmaStringA(hWords, i, strBuf.get(), bufSize);
			words.emplace_back(strBuf.get());
			auto& word = words.back();

			// word to lower
			std::transform(word.begin(), word.end(), word.begin(), [](wchar_t ch){return std::tolower(ch);});
		}

		sol_DeleteLemmas(hWords);
	}
	return words;
}

std::string Lemmatizer::lemmatizeWord(const std::string& word) const
{

	int bufSize = static_cast<int>((word.length() + 100) * 1.5);
	auto strBuf = std::unique_ptr<char[]>(new char[bufSize]);
	sol_GetLemmaA(_hEngine, word.c_str(), strBuf.get(), bufSize);
	std::string normWord = strBuf.get();
	return normWord;
}

Lemmatizer::~Lemmatizer()
{
	sol_DeleteLemmatizator(_hEngine);
}
