#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <boost/regex.hpp>

#include "Utils/FileUtils.h"
#include "TextNormalizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils/TermsUtils.h"
#include "TagsAnalyzer.h"
#include "ArticlesReader/MathArticlesReader.h"
#include "ArticlesReader/XmlArticlesReader.h"
#include "Utils/Hasher.h"
#include "Utils/StringUtils.h"

SemanticGraph getMathGraph()
{
	SemanticGraph graph;
	graph.importFromFile("resources/coolAllMath.gr");
	return graph;
}

SemanticGraph getBiologyGraph()
{
	SemanticGraph graph;
	graph.importFromFile("resources/biology.gr");
	return graph;
}

SemanticGraph getBadGraph()
{
	SemanticGraph graph;
	graph.importFromFile("resources/breakingBad.gr");
	return graph;
}

void wrongTerms()
{
	auto reader = MathArticlesReader();
	auto text = FileUtils::readAllFile("resources/math/math.txt");

	auto [titles, _] = reader.read(text);
	for (int i = 1; i < titles.size(); i++)
	{
		if (titles[i] < titles[i - 1])
		{
			std::cout << titles[i - 1] << " >> " << titles[i] << "\n";
		}
	}
}

void calcTermsLinksCounts()
{
	auto graph = getBadGraph();
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 5000; i++)
		count[i] = 0;
	for (auto [hash, node] : graph.nodes)
	{
		count[node.neighbors.size()]++;
	}
	std::ofstream out = std::ofstream("statneighbors.txt");
	for (auto [_, cnt] : count)
	{
		out << cnt << std::endl;
	}
}

void calcTermsWords() {
	auto graph = getBadGraph();
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 20; i++)
		count[i] = 0;
	std::ofstream out = std::ofstream("statwords.txt");

	for (auto [hash, node] : graph.nodes)
	{
		count[node.term.normalizedWords.size()]++;
	}
	for (auto [words, term] : count)
	{
		out << words << " " << term << std::endl;
	}
}

void calcTopTerms()
{
	auto graph = getBadGraph();
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 5000; i++)
		count[i] = 0;
	std::ofstream out = std::ofstream("statres.txt");
	std::map<size_t, std::string> top;
	for (auto [hash, node] : graph.nodes)
	{
		count[node.neighbors.size()]++;
		if (node.neighbors.size() > 1000)
			top.insert({ node.neighbors.size(), node.term.view });
	}
	for (auto& [fst, snd] : top)
	{
		out << fst << " " << snd << std::endl;
	}
	out.close();
}

void termsCounts()
{
	auto graph = getMathGraph();

	for (auto& [termHash, cnt] : TermsUtils::extractTermsCounts(graph, TextNormalizer().normalize(
		FileUtils::readAllUTF8File("resources/integral.txt"))))
	{
		std::cout << graph.nodes.at(termHash).term.view << " " << cnt << '\n';
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

void createBiologyGraph() {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllFile("resources/biology.txt"), MathArticlesReader());
	graph.exportToFile("resources/biology.gr");
}

void createBadGraph() {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllUTF8File("resources/breakingBad.txt"), XmlArticlesReader());
	graph.exportToFile("resources/breakingBad.gr");
}

void drawTermSubGraph(std::string const& term, std::string const& imagePngPath, int radius, double minEdgeWeight)
{
	auto graph = getBadGraph();
	TextNormalizer normalizer{};
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize(term));
	auto subgr = graph.getNeighborhood(hash, radius, minEdgeWeight);
	subgr.drawToImage(imagePngPath, hash);
}

void tags(const std::string& text, int cnt)
{
	auto graph = getMathGraph();

	TagsAnalyzer analyzer;
	analyzer.distributionCoef = 1;

	analyzer.analyze(text, graph);
	auto tags = analyzer.getRelevantTags(cnt);
	for (auto& [termView, weight] : tags)
	{
		std::cout << termView << ' ' << std::fixed << std::setprecision(2) << weight << "\n";
	}
}

void printGraph(SemanticGraph const& gr, std::ostream& out)
{
	out << "Terms count: " << gr.getTermsCount() << "\n";
	std::vector<std::string> terms(gr.getTermsCount());
	std::transform(gr.nodes.begin(), gr.nodes.end(), terms.begin(), [](auto& n){return n.second.term.view;});
	std::sort(terms.begin(), terms.end());
	for (auto& term: terms)
		out << term << "\n";
	std::vector<std::tuple<std::string, std::string, double>> links;
	for (auto& [_, node] : gr.nodes)
		for (auto& [neighborHash, link] : node.neighbors)
			links.emplace_back(node.term.view, gr.nodes.at(neighborHash).term.view, link.weight);
	std::sort(links.begin(), links.end(), [](auto& a, auto& b) {return std::get<2>(a) > std::get<2>(b);});
	out << "Links count: " << links.size() << "\n";
	for(auto& [term1, term2, w]: links)
		out << term1 << " -> " << term2 << " (" << w << ")\n";
}


int main() {
	srand(time(nullptr));
	setlocale(LC_ALL, "rus");

	//createMathGraph();

	std::ofstream fout("temp/graphView.txt");
	printGraph(getMathGraph(), fout);

	//tags(FileUtils::readAllUTF8File("resources/articles/7.txt"), 30);
	//tags(FileUtils::readAllFile("resources/voevoda.txt"), 30);
	//tags(FileUtils::readAllFile("resources/giperbola.txt"), 30);

	//drawTermSubGraph("корень", "demo/root.png", 1, 0.1);
	//drawTermSubGraph("вода", "demo/water.png", 1, 0.05);
	//calcTopTerms();
	//calcTermsWords();
	//calcTermsLinksCounts();
	return 0;
}