#include <clocale>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <vector>

#include "DocumentReader.h"
#include "Lemmatizer.h"
#include "SemanticGraphBuilder.h"


int main()
{
	setlocale(LC_ALL, "rus");
	//	auto a = std::filesystem::current_path();
	auto builder = SemanticGraphBuilder();
	auto reader = DocumentReader();
	auto graph = builder.build(reader.read("D:/Source/ThematicAnalysis/ThematicAnalysisTests/resources/AllMath.txt"));

	Lemmatizer lemmatizer(Lemmatizer::RUS_DICTIONARY_DEFAULT_PATH);
	auto words = lemmatizer.lemmatizeText("Сижу и буду сидеть");
	for (auto&& word : words)
		std::cout << word << std::endl;
}
