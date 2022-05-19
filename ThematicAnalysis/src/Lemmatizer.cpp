#include "Lemmatizer.h"
#include <lemmatizator_engine.h>
#include <memory>

const std::wstring Lemmatizer::DefaultPathToRussianDictionary = L"external/lemmatizator/x64/lemmatizer.db";

Lemmatizer::Lemmatizer(const std::wstring& pathToDictionary)
{
	_hEngine = sol_LoadLemmatizatorW(pathToDictionary.c_str(), LEME_DEFAULT);
}

std::vector<std::wstring> Lemmatizer::LemmatizeText(const std::wstring& text) const
{
	std::vector<std::wstring> words;
	auto strBuf = std::unique_ptr<wchar_t[]>(new wchar_t[text.length() + 1]);
	auto hWords = sol_LemmatizePhraseW(_hEngine, text.c_str(), 0, L' ');
	if (hWords != nullptr)
	{
		auto wordsCount = sol_CountLemmas(hWords);

		for (int i = 0; i < wordsCount; ++i)
		{
			sol_GetLemmaStringW(hWords, i, strBuf.get(), sizeof(strBuf));
			words.emplace_back(strBuf.get());
		}

		sol_DeleteLemmas(hWords);
	}
	return words;
}

Lemmatizer::~Lemmatizer()
{
	sol_DeleteLemmatizator(_hEngine);
}
