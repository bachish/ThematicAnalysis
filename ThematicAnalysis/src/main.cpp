#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
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
	auto graph = builder.build("resources/math/math.txt", MathXmlConverter());
	graph.exportToFile("resources/coolAllMath.gr");
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
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize("первообразная"));
	auto subgr = graph.getNeighborhood(hash, 3, 15);
	subgr.drawToImage("", "image", hash);
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

	draw();
	return 0;
}
