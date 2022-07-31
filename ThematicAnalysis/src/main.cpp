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


void listTerms() {
	auto reader = MathArticlesReader();
	auto text = FileUtils::readAllFile("resources/math/math.txt");

	//FileUtils::writeUTF8ToFile("temp/temp.txt", text);
	auto [titles, _] = reader.read(text);
	FileUtils::writeUTF8ToFile("terms.txt", StringUtils::concat(titles, "\n"));
}

void create() {
	auto builder = SemanticGraphBuilder();
	auto graph = builder.build(FileUtils::readAllFile("resources/math/math.txt"), MathArticlesReader());
	graph.exportToFile("resources/coolAllMath.gr");
}

void createXml() {
	auto text = FileUtils::readAllFile("resources/math/math.txt");
	auto builder = SemanticGraphBuilder();
	auto [terms, content] = MathArticlesReader().read(text);
	auto xmlText = XmlArticlesReader().exportToXml(terms, content);
	std::ofstream out = std::ofstream("xmlMath.txt");
	out << xmlText;
	out.close();
}
SemanticGraph getMathGraph()
{
	SemanticGraph graph;
	graph.importFromFile("resources/coolAllMath.gr");
	return graph;
}

void draw(std::string term)
{
	auto graph = getMathGraph();
	TextNormalizer normalizer;
	auto hash = Hasher::sortAndCalcHash(normalizer.normalize(term));
	auto subgr = graph.getNeighborhood(hash,1, 0.001);
	subgr.drawToImage("temp/", "image2", hash);
}

void tags()
{
	auto graph = getMathGraph();

	TagsAnalyzer analyzer;
	analyzer.DISTRIBUTION_COEF = 1;

	analyzer.analyze(FileUtils::readAllUTF8File("resources/articles/4.txt"), graph);
	auto tags = analyzer.getRelevantTags(30);
	for (auto& tag : tags)
	{
		std::cout << tag.termView << ' ' << std::fixed << std::setprecision(2) << tag.weight << "\n";
	}
	auto hash = Hasher::sortAndCalcHash(TextNormalizer().normalize(tags[0].termView));
	auto gr = SemanticGraph();
	//auto gr = analyzer.tagsGraph.getNeighborhood(hash, 10, 0.09, 0.1);


	for (int i = 0; i < tags.size(); i++) {
		auto gr2 = analyzer.tagsGraph.getNeighborhood(Hasher::sortAndCalcHash(TextNormalizer().normalize(tags[i].termView)), 3, 0.05, 0.06);
		gr.merge(gr2);
	}
	gr.drawToImage("temp/", "temp2", hash);

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
	for (int i = 0; i < 5000; i++)
		count[i] = 0;
	std::ofstream out = std::ofstream("statres.txt");
	std::map<size_t, std::string> top;
	for (auto [hash, node] : graph.nodes)
	{
		count[node.neighbors.size()]++;
		if (node.neighbors.size() > 1000)
			top.insert({ node.neighbors.size() , node.term.view });
	}
	for (auto value : top)
	{
		out << value.first << " " <<value.second <<  std::endl;
	}
	out.close();
}
void calcColTerms() {
	auto graph = getMathGraph();
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 20; i++)
		count[i] = 0;
	std::ofstream out = std::ofstream("statwords.txt");

	for (auto [hash, node] : graph.nodes)
	{
		 count[node.term.normalizedWords.size()]++;
	}
	for (auto [words, term]  : count)
	{
		out << words << " " << term << std::endl;
	}
}

int main() {
	srand(time(0));
	setlocale(LC_ALL, "rus");
	//getMathGraph().drawToImage("temp/", "all");
	//create();
	//calcColTerms();
	//draw("ТОЧКИ");
	//tags();
	draw("ФУНКЦИЯ");
	//listTerms();
	return 0;
}
