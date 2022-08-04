#include "AnalyzeUtils.h"
#include <fstream>
#include <iomanip>
#include <iostream>

#include "FileUtils.h"
#include "TagsAnalyzer.h"

void AnalyzeUtils::calcStatistic(const SemanticGraph& graph)
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
	std::ofstream out = std::ofstream("temp/" + graph.name + "_liks_count.txt");
	for (auto [_, cnt] : linkCount)
	{
		out << cnt << std::endl;
	}
	out = std::ofstream("temp/" + graph.name + "_words_count.txt");
	for (auto [_, cnt] : wordsCount)
	{
		out << cnt << std::endl;
	}
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
void AnalyzeUtils::getTags(const SemanticGraph& graph, std::string const& articleName)
{
	std::ofstream out = std::ofstream("resources/articles/" + articleName + "_keyWords.txt");
	tags(graph, FileUtils::readAllUTF8File("resources/articles/" + articleName + ".txt"), 30, out);
}


