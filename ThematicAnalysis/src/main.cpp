#include <clocale>
#include <cstdio>
#include <filesystem>

#include "DocumentReader.h"
#include "Hasher.h"
#include "Normalizer.h"
#include "SemanticGraphBuilder.h"

void create() {
	auto builder = SemanticGraphBuilder();
	auto reader = DocumentReader();
	auto graph = builder.build(reader.readAndNormalizeArticles("resources/AllMath.txt"));
	graph.exportToFile("resources/allMath.gr");
}

void import()
{
	SemanticGraph graph;
	graph.importFromFile("resources/allMath.gr");
	Normalizer normalizer;
	auto subgr = graph.getNeighborhood(Hasher::sortAndCalcHash(normalizer.normalize("Первообразная")), 1, 3);
	subgr.drawToImage("", "image");
}
int main()
{
	setlocale(LC_ALL, "rus");
	//	auto a = std::filesystem::current_path();
	//create();
	import();
}
