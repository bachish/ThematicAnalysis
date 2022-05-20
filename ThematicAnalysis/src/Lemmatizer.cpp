#include "Lemmatizer.h"

#include <algorithm>
#include <locale>
#include <lemmatizator_engine.h>
#include <memory>

const std::wstring Lemmatizer::RUS_DICTIONARY_DEFAULT_PATH = L"external/lemmatizator/x64/lemmatizer.db";

Lemmatizer::Lemmatizer() :Lemmatizer(RUS_DICTIONARY_DEFAULT_PATH)
{
}

Lemmatizer::Lemmatizer(const std::wstring& pathToDictionary)
{
	_hEngine = sol_LoadLemmatizatorW(pathToDictionary.c_str(), LEME_DEFAULT);
}

std::vector<std::wstring> Lemmatizer::lemmatizeText(const std::wstring& text) const
{
	std::vector<std::wstring> words;
	auto bufSize = (text.length() + 100) * 1.5;
	auto strBuf = new wchar_t[bufSize];
	auto hWords = sol_LemmatizePhraseW(_hEngine, text.c_str(), 0, L' ');
	if (hWords != nullptr)
	{
		auto wordsCount = sol_CountLemmas(hWords);

		for (int i = 0; i < wordsCount; ++i)
		{
			sol_GetLemmaStringW(hWords, i, strBuf, bufSize);
			words.emplace_back(strBuf);
			auto& word = words.back();

			std::transform(word.begin(), word.end(), word.begin(), [](wchar_t ch){return std::tolower(ch, std::locale("ru-RU"));});

		}

		sol_DeleteLemmas(hWords);
	}
	delete[] strBuf;
	return words;
}

Lemmatizer::~Lemmatizer()
{
	sol_DeleteLemmatizator(_hEngine);
}
