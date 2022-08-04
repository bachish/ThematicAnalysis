#include "AnalyzeUtils.h"
#include <fstream>
#include <iostream>

void AnalyzeUtils::countLinksOfEachTerm(const SemanticGraph& graph)
{
	auto count = std::map<size_t, size_t>();
	for (int i = 0; i < 5000; i++)
		count[i] = 0;
	for (auto [hash, node] : graph.nodes)
	{
		count[node.neighbors.size()]++;
	}
	std::ofstream out = std::ofstream("temp/" + graph.name + "_links_counts.txt");
	for (auto [_, cnt] : count)
	{
		out << cnt << std::endl;
	}
}
