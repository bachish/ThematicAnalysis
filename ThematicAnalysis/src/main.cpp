#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#include "ArticlesNormalizer.h"
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

std::string readEncyclopedia(std::string const& encName)
{
	return FileUtils::readAllFile("resources/encyclopedias/" + encName + ".txt");
}

std::string readArticle(std::string const& articleName)
{
	return FileUtils::readAllFile("resources/articles/" + articleName + ".txt");
}

std::string getGraphPath(std::string const& graphName)
{
	return "resources/graphs/" + graphName + ".gr";
}

SemanticGraph getGraph(const std::string& graphName)
{
	//todo check on existing
	SemanticGraph graph(graphName);
	graph.importFromFile(getGraphPath(graphName));
	return graph;
}

void printWrongTerms(std::string const& encName, IArticlesReader const& reader)
{
	auto text = readEncyclopedia(encName);
	auto [titles, _] = reader.read(text);
	for (int i = 1; i < titles.size(); i++)
	{
		if (titles[i] < titles[i - 1])
			std::cout << titles[i - 1] << " >> " << titles[i] << "\n";
	}
}

void calcTopTerms(SemanticGraph const& graph)
{
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 5000; i++)
		count[i] = 0;
	std::ofstream out = std::ofstream("topterms.txt");
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

void calcTermsCountsInArticle(SemanticGraph const& graph, std::string const& articleName)
{
	for (auto& [termHash, cnt] : TermsUtils::extractTermsCounts(graph, TextNormalizer().normalize(readArticle(articleName))))
	{
		std::cout << graph.nodes.at(termHash).term.view << " " << cnt << '\n';
	}
}

void drawTermSubGraph(SemanticGraph const& graph, std::string const& term, int radius, double minEdgeWeight)
{
	TextNormalizer normalizer{};
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize(term));
	const auto subGraph = graph.getNeighborhood(hash, radius, minEdgeWeight);
	subGraph.drawToImage("resources/ " + graph.name + "_" + term, hash);
}

void listEncyclopediaTerms(std::string const& encName, IArticlesReader const& reader)
{
	auto [titles, _] = reader.read(readEncyclopedia(encName));
	std::transform(titles.begin(), titles.end(), titles.begin(), [](std::string const& title) {return TextNormalizer::clearText(title); });
	FileUtils::writeUTF8ToFile("temp/" + encName + "_terms.txt", StringUtils::concat(titles, "\n"));
}

void createAndExportGraph(std::string const& encName, IArticlesReader const& reader)
{
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(readEncyclopedia(encName), reader);
	graph.exportToFile(getGraphPath(encName));
}


void printTextTags(const SemanticGraph& graph, const std::string& text, int cnt, std::ostream& out)
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

void printGraphStructure(SemanticGraph const& gr, std::ostream& out)
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
	//auto graph = getGraph("math");
	/*for(int i = 1; i < 11; i++)
		AnalyzeUtils::printArticlesTags(graph, std::to_string(i), readArticle(std::to_string(i)), 30);*/
		//listEncyclopediaTerms("math", MathArticlesReader());
	createAndExportGraph("med", MathArticlesReader());
//	printWrongTerms("math", MathArticlesReader());
	return 0;
}