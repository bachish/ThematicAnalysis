#include <clocale>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <vector>

#include "DocumentReader.h"
#include "Lemmatizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils.h"

void create() {
	auto builder = SemanticGraphBuilder();
	auto reader = DocumentReader();
	auto graph = builder.build(reader.read("resources/AllMath.txt"));
	graph.exportToFile("resources/allMath.gr");
}

void import()
{
	SemanticGraph graph;
	graph.importFromFile("resources/allMath.gr");
	auto subgr = graph.getNeighborhood(Utils::calculateNormalizedHashCode("Первообразная"), 1, 3);
	Utils::drawGraphImageFromDot(subgr.getDotView(), "image");
}
int main()
{
	setlocale(LC_ALL, "rus");
	//	auto a = std::filesystem::current_path();
	//create();
	import();
}
