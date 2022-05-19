#include <clocale>
#include <cstdio>

#include "lemmatizator_engine.h"


int main()
{
	setlocale(LC_ALL, "rus");
	HLEM hEngine = sol_LoadLemmatizatorA("external/lemmatizator/x64/lemmatizer.db", LEME_DEFAULT);

	wchar_t utf8[256];
	auto lemmas = sol_LemmatizePhraseW(hEngine, L"Сижу и кушаю гречку, роя землю", 0, L' ');
	if (lemmas != nullptr)
	{
		int nlemma = sol_CountLemmas(lemmas);

		for (int i = 0; i < nlemma; ++i)
		{
			sol_GetLemmaStringW(lemmas, i, utf8, sizeof(utf8));
			printf("%ls\n", utf8);
		}

		sol_DeleteLemmas(lemmas);
	}
	sol_DeleteLemmatizator(hEngine);
}