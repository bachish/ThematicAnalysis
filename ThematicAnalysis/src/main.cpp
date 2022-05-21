#include <clocale>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <vector>
#include "Lemmatizer.h"
#include "XmlSourceParser.h"


int main()
{
	setlocale(LC_ALL, "rus");
	auto a = std::filesystem::current_path();

	Lemmatizer lemmatizer(Lemmatizer::RUS_DICTIONARY_DEFAULT_PATH);
	auto words = lemmatizer.lemmatizeText("Сижу и буду сидеть");
	for (auto&& word : words)
		std::cout << word << std::endl;
}
