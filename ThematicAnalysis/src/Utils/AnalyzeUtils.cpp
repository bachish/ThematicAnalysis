#include "AnalyzeUtils.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "FileUtils.h"
#include "TagsAnalyzer.h"

void AnalyzeUtils::calcGraphStatistics(const SemanticGraph& graph)
{
	auto linkCount = std::map<size_t, size_t>();
	auto wordsCount = std::map<size_t, size_t>();
	for (int i = 0; i < 5000; i++)
		linkCount[i] = 0;
	for (int i = 0; i < 10; i++)
		wordsCount[i] = 0;
	for (auto [hash, node] : graph.nodes)
	{
		linkCount[node.neighbors.size()]++;
		wordsCount[node.term.normalizedWords.size()]++;
	}
	auto printMap = [](auto&& out, auto const& map)
	{
		for (auto&& [_, cnt] : map)
			out << cnt << std::endl;
	};
	printMap(std::ofstream("temp/" + graph.name + "_links_counts.txt"), linkCount);
	printMap(std::ofstream("temp/" + graph.name + "_words_counts.txt"), wordsCount);
}

void AnalyzeUtils::printArticlesTags(const SemanticGraph& graph, std::string const& articleName, std::string const& articleText, int cnt)
{
	std::ofstream out = std::ofstream("temp/" + articleName + "_key_words.txt");

	TagsAnalyzer analyzer;
	analyzer.distributionCoef = 1;

	analyzer.analyze(articleText, graph);
	auto tags = analyzer.getRelevantTags(cnt);
	for (auto& [termView, weight] : tags)
	{
		out << termView << ' ' << std::fixed << std::setprecision(2) << weight << "\n";
	}
}


