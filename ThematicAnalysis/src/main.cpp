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
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize("бэра классы"));
	auto subgr = graph.getNeighborhood(hash, 1, 0.05);
	subgr.drawToImage("", "image", hash);
}

void tags()
{
	auto graph = getMathGraph();

	TextAnalyzer analyzer;
	analyzer.analyze("resources/giperbola.txt", graph);
	auto tags = analyzer.getRelevantTags(150);
	for (auto& tag : tags)
	{
		//std::cout << tag << '\n';
	}
}


int main() {
	setlocale(LC_ALL, "rus");
	//create();
	tags();
	return 0;
}
