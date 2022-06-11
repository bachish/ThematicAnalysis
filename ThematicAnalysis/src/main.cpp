#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#include "Utils/FileUtils.h"
#include "Hasher.h"
#include "TextNormalizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils/TermsUtils.h"
#include "TagsAnalyzer.h"
#include "ArticlesReader/MathArticlesReader.h"
#include "ArticlesReader/XmlArticlesReader.h"
#include "Utils/StringUtils.h"

SemanticGraph getMathGraph()
{
	SemanticGraph graph;
	graph.importFromFile("resources/coolAllMath.gr");
	return graph;
}

void wrongTerms()
{
	auto reader = MathArticlesReader();
	auto text = FileUtils::readAllFile("resources/math/math.txt");

	auto [titles, _] = reader.read(text);
	for(int i = 1; i < titles.size(); i++)
	{
		if(titles[i] < titles[i-1])
		{
			std::cout << titles[i-1] << " >> " << titles[i] << "\n";
		}
	}
}

void calcTermsLinksCounts()
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

void termsCounts()
{
	auto graph = getMathGraph();

	for (auto& tag : TermsUtils::extractTermsCounts(graph, TextNormalizer().normalize(
		FileUtils::readAllUTF8File("resources/integral.txt"))))
	{
		std::cout << graph.nodes[tag.first].term.view << " " << tag.second << '\n';
	}
}

void printTermsToFile(std::string const& path) {
	auto reader = MathArticlesReader();
	auto text = FileUtils::readAllFile("resources/math/math.txt");
	//auto reader = XmlArticlesReader();
	//auto text = FileUtils::readAllFile("resources/AllMath.txt");

	auto [titles, _] = reader.read(text);
	FileUtils::writeUTF8ToFile(path, StringUtils::concat(titles, "\n"));
}

void createMathGraph() {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllFile("resources/math/math.txt"), MathArticlesReader());
	graph.exportToFile("demo/newAllMath.gr");
}

void drawTermSubGraph(std::string term, std::string imagePngPath, int radius, double minEdgeWeight)
{
	auto graph = getMathGraph();
	TextNormalizer normalizer;
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize(term));
	auto subgr = graph.getNeighborhood(hash, radius, minEdgeWeight);
	subgr.drawToImage(imagePngPath, hash);
}

void tags(std::string text, int cnt)
{
	auto graph = getMathGraph();

	TagsAnalyzer analyzer;
	analyzer.DISTRIBUTION_COEF = 1;

	analyzer.analyze(text, graph);
	auto tags = analyzer.getRelevantTags(cnt);
	for (auto& tag : tags)
	{
		std::cout << tag.termView << ' ' << std::fixed << std::setprecision(2) << tag.weight << "\n";
	}
}



int main() {
	srand(time(0));
	setlocale(LC_ALL, "rus");

	createMathGraph();

	//tags(FileUtils::readAllUTF8File("resources/articles/7.txt"), 30);
	//tags(FileUtils::readAllFile("resources/voevoda.txt"), 30);
	//tags(FileUtils::readAllFile("resources/giperbola.txt"), 30);

	//drawTermSubGraph("функция", "demo/func.png", 1, 0.15);
	//drawTermSubGraph("программа", "demo/prog.png", 1, 0.15);


	return 0;
}