#include <clocale>
#include <cstdio>
#include <iostream>
#include <vector>
#include "Lemmatizer.h"
#include "XmlSourceParser.h"


int main()
{
	setlocale(LC_ALL, "rus");

	Lemmatizer lemmatizer(Lemmatizer::RUS_DICTIONARY_DEFAULT_PATH);
	auto words = lemmatizer.lemmatizeText(L"Сижу и буду сидеть");
	for (auto&& word : words)
		std::wcout << word << std::endl;
}
