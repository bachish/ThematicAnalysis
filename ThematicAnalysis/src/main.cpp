#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <boost/regex.hpp>
#include "DocumentReader.h"
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
	create();
	return 0;
}
