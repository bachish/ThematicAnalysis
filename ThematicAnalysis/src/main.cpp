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
#include "Utils/AnalyzeUtils.h"

SemanticGraph getGraph(const std::string& graph_name)
{
	//todo check on existing
	SemanticGraph graph(graph_name);
	graph.importFromFile("resources/" + graph_name + ".gr");
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


void draw(SemanticGraph const& graph, const std::string& term)
{
	TextNormalizer normalizer;
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize(term));
	auto subgr = graph.getNeighborhood(hash, 1, 0.15);
	subgr.drawToImage("temp/" + graph.name + "_" + term, hash);
}

void termsInArticleCount(SemanticGraph const& graph, std::string const& article_name, std::ostream& out)
{
	for (auto& [termHash, cnt] : TermsUtils::extractTermsCounts(graph, TextNormalizer().normalize(
		FileUtils::readAllUTF8File("resources/integral.txt"))))
	{
		out << graph.nodes.at(termHash).term.view << " " << cnt << '\n';
	}
}

//todo what it do?
void printTermsToFile(std::string const& path) {
	auto reader = MathArticlesReader();
	auto text = FileUtils::readAllFile("resources/math/math.txt");
	//auto reader = XmlArticlesReader();
	//auto text = FileUtils::readAllFile("resources/AllMath.txt");

	auto [titles, _] = reader.read(text);
	FileUtils::writeUTF8ToFile(path, StringUtils::concat(titles, "\n"));
}

void createGraphFromTxt(std::string const& enc_name) {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllFile("resources/" + enc_name + ".txt"), MathArticlesReader());
	graph.exportToFile("resources/" + enc_name + ".gr");
}
void createGraphFromXml(std::string const& enc_name) {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllFile("resources/" + enc_name + ".txt"), XmlArticlesReader());
	graph.exportToFile("resources/" + enc_name + ".gr");
}


void drawTermSubGraph(SemanticGraph const& graph, std::string const& term, int radius, double minEdgeWeight)
{
	TextNormalizer normalizer{};
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize(term));
	const auto subGraph = graph.getNeighborhood(hash, radius, minEdgeWeight);
	subGraph.drawToImage("resources/ " + graph.name + "_" + term, hash);
}

void tags(const SemanticGraph& graph, const std::string& text, int cnt, std::ostream& out)
{
	TagsAnalyzer analyzer;
	analyzer.distributionCoef = 1;

	analyzer.analyze(text, graph);
	auto tags = analyzer.getRelevantTags(cnt);
	for (auto& [termView, weight] : tags)
	{
		out << termView << ' ' << std::fixed << std::setprecision(2) << weight << "\n";
	}
}

void printGraph(SemanticGraph const& gr, std::ostream& out)
{
	out << "Terms count: " << gr.getTermsCount() << "\n";
	std::vector<std::string> terms(gr.getTermsCount());
	std::transform(gr.nodes.begin(), gr.nodes.end(), terms.begin(), [](auto& n) {return n.second.term.view; });
	std::sort(terms.begin(), terms.end());
	for (auto& term : terms)
		out << term << "\n";
	std::vector<std::tuple<std::string, std::string, double>> links;
	for (auto& [_, node] : gr.nodes)
		for (auto& [neighborHash, link] : node.neighbors)
			links.emplace_back(node.term.view, gr.nodes.at(neighborHash).term.view, link.weight);
	std::sort(links.begin(), links.end(), [](auto& a, auto& b) {return std::get<2>(a) > std::get<2>(b); });
	out << "Links count: " << links.size() << "\n";
	for (auto& [term1, term2, w] : links)
		out << term1 << " -> " << term2 << " (" << w << ")\n";
}


int main() {
	srand(time(nullptr));
	setlocale(LC_ALL, "rus");
	auto graph = getGraph("math");
	AnalyzeUtils::calcLinksCounts(graph);

	//create();
	//calcColTerms();
	//draw("ТОЧКИ");
	//tags();
	//draw("ФУНКЦИЯ");
	//listTerms();
	return 0;
}