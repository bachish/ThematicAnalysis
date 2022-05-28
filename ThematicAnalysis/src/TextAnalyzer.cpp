#include "TextAnalyzer.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <iterator>

#include "DocumentReader.h"
#include "FileManager.h"
#include "SemanticGraphBuilder.h"
#include "Hasher.h"
#include "MathUtils.h"
#include "Normalizer.h"
constexpr double TextAnalyzer::DISTRIBUTION_COEF = 1.;
constexpr double TextAnalyzer::ABSORPTION_COEF = 0.5;
constexpr size_t TextAnalyzer::LINK_RADIUS = 1;


void TextAnalyzer::analyze(std::string const& text, SemanticGraph const& graph)
{
	DocumentReader reader;
	auto normText = reader.readAndNormalizeText(text);
	analyze(normText, graph);
}

size_t getNotNullWeightNodesCount(SemanticGraph const& graph)
{
	return std::transform_reduce(std::execution::par, graph.nodes.begin(), graph.nodes.end(), 0ull,
		[](size_t a, size_t b) {return a + b; },
		[](std::pair<size_t, Node> const& pair) {return pair.second.weight > FLT_EPSILON ? 1ull : 0ull; });
}



void calcFrequency(SemanticGraph& graph, std::vector<std::string> const& normalizedText)
{
	auto termsCounts = extractAndCalculateTerms(graph, normalizedText);
	for(auto&& [termhash, count]: termsCounts)
	{
		graph.addTermWeight(termhash, count);
	}
	
}

void calcTfIdf(SemanticGraph& graph)
{
	auto textTermsCount = getNotNullWeightNodesCount(graph);
	for (auto&& [hash, node] : graph.nodes)
		if (node.weight > FLT_EPSILON) {
			node.weight = MathUtils::calcTfIdf(node.weight, textTermsCount, node.term.numberOfArticlesThatUseIt, graph.nodes.size() + 1);
		}
}

void TextAnalyzer::distributeTermWeight(SemanticGraph& graph, size_t centerTermHash, size_t radius, double weight)
{
	if (radius > 0)
	{
		auto node = graph.nodes.at(centerTermHash);
		const auto weightSum = node.sumLinksWeight();
		for (auto [neighborHash, link] : node.neighbors)
		{
			auto neighborWeight = weight * (link.weight / weightSum);
			graph.addTermWeight(neighborHash, neighborWeight * ABSORPTION_COEF);
			distributeTermWeight(graph, neighborHash, radius - 1, neighborWeight * (1-ABSORPTION_COEF));
		}
	}
}

SemanticGraph TextAnalyzer::distributeWeights(SemanticGraph const& graph)
{
	auto distrGraph = graph;
	for (auto&& [hash, node] : graph.nodes)
		if (node.weight > FLT_EPSILON) {
			distributeTermWeight(distrGraph, hash, LINK_RADIUS, node.weight * DISTRIBUTION_COEF);
		}
	return distrGraph;
}

void TextAnalyzer::analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph)
{
	tagsGraph = graph;
	calcFrequency(tagsGraph, normalizedText);
	calcTfIdf(tagsGraph);
	tagsGraph = distributeWeights(tagsGraph);
}


std::vector<std::string> TextAnalyzer::getRelevantTags(int tagsCount)
{
	std::vector<Node> nodes;
	nodes.reserve(tagsGraph.nodes.size());
	std::transform(tagsGraph.nodes.begin(), tagsGraph.nodes.end(), std::back_inserter(nodes), [](auto el) {return el.second; });
	std::sort(nodes.begin(), nodes.end(), [](Node& n1, Node& n2) {return n1.weight > n2.weight; });
	tagsCount = tagsCount <= nodes.size() ? tagsCount : nodes.size();
	for (int i = 0; i < tagsCount; i++)
		std::cout << nodes[i].term.view << ' ' << nodes[i].weight << "\n";
	std::vector<std::string> terms;
	terms.reserve(tagsCount);
	std::transform(nodes.begin(), nodes.begin() + tagsCount, std::back_inserter(terms), [](Node& el) {return el.term.view; });
	return terms;

}


