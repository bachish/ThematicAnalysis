#include <clocale>
#include <cstdio>
#include <iostream>
#include <vector>
#include "Lemmatizer.h"


int main()
{
	setlocale(LC_ALL, "rus");

	Lemmatizer lemmatizer(Lemmatizer::DefaultPathToRussianDictionary);
	auto words = lemmatizer.LemmatizeText(L"Сижу и буду сидеть");
	for (auto&& word : words)
		std::wcout << word << std::endl;
}
