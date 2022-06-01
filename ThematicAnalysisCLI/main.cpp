#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Utils/FileUtils.h"
#include "Hasher.h"
#include "TextNormalizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils/TermsUtils.h"
#include "TagsAnalyzer.h"
#include "ArticlesReader/MathArticlesReader.h"


void create() {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllFile("resources/math/math.txt"), MathArticlesReader());
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
	TextNormalizer normalizer;
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize("бэра классы"));
	auto subgr = graph.getNeighborhood(hash, 1, 0.05);
	subgr.drawToImage("", "image", hash);
}

void tags()
{
	auto graph = getMathGraph();

	TagsAnalyzer analyzer;

	analyzer.analyze(FileUtils::readAllUTF8File("resources/temp.txt"), graph);
	auto tags = analyzer.getRelevantTags(100);
	for (auto& tag : tags)
	{
		std::cout << tag.termView << ' ' << std::fixed << std::setprecision(2) << tag.weight << ", ";
	}
}

void terms()
{
	auto graph = getMathGraph();
	auto koko = std::find_if(graph.nodes.begin(), graph.nodes.end(), [](decltype(graph.nodes)::const_reference node)
		{
			return node.second.term.view == "КО---ПРОСТРАНСТВО";
		});

	for (auto& tag : TermsUtils::extractTermsCounts(graph, TextNormalizer().normalize(
		FileUtils::readAllUTF8File("resources/integral.txt"))))
	{
		std::cout << graph.nodes[tag.first].term.view << " " << tag.second << '\n';
	}
}
void calcTerms()
{
	auto graph = getMathGraph();
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 3500; i++)
		count[i] = 0;
	for (auto [hash, node] : graph.nodes)
	{
		count[node.neighbors.size()]++;
		if (node.neighbors.size() > 200)
			std::cout << node.term.view << std::endl;
	}
	std::ofstream out = std::ofstream("statres.txt");
	for (auto value : count)
	{
		out << value.second << std::endl;

	}
}

int main() {
	setlocale(LC_ALL, "rus");
	//create();
	//calcTerms();
	tags();
	return 0;
}
