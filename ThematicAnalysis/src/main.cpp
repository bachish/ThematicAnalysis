#include <clocale>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <regex>
#include <boost/regex.hpp>
#include "Hasher.h"
#include "Normalizer.h"
#include "SemanticGraphBuilder.h"
#include "TextAnalyzer.h"
#include "XmlConverter/MathXmlConverter.h"


void create() {
	auto builder = SemanticGraphBuilder();
	auto graph =  builder.build("resources/math/math.txt", MathXmlConverter());
	graph.exportToFile("temp/coolAllMath.gr");
}

SemanticGraph getMathGraph()
{
	SemanticGraph graph;
	graph.importFromFile("resources/coolAllMath.gr");
	return graph;
}

void draw()
{
	auto graph = getMathGraph();
	Normalizer normalizer;
	auto subgr = graph.getNeighborhood(Hasher::sortAndCalcHash(normalizer.normalize("лагранжиан")), 1, 3);
	subgr.drawToImage("", "image");
}

void tags()
{
	auto graph = getMathGraph();
	
	TextAnalyzer analyzer;
	analyzer.analyze("resources/mathText.txt", graph);
	auto tags = analyzer.getRelevantTags(50);
	for (auto& tag : tags)
	{
		//std::cout << tag << '\n';
	}
}


int main() {
	setlocale(LC_ALL, "rus");
	Lemmatizer lemmatizer;
	std::vector<std::string> sourceWords = { "Сижу", "Работы", "Гречневые", "Забавно", "Интегралов" };
	std::vector<std::string> normWords = { "сидеть", "работа", "гречневый", "забавно", "интеграл" };
	for (size_t i = 0; i < sourceWords.size(); i++)
	{
		auto res = lemmatizer.lemmatizeText(sourceWords[i]);
		std::cout << res[0] << ' ';
	}
	
	return 0;
}
