#include "TagsAnalyzer.h"

#include <algorithm>
#include <execution>
#include <iterator>

#include "ArticlesNormalizer.h"
#include "SemanticGraphBuilder.h"
#include "TermsUtils.h"

constexpr double TagsAnalyzer::DISTRIBUTION_COEF = 1.;
constexpr double TagsAnalyzer::ABSORPTION_COEF = 0.5;
constexpr size_t TagsAnalyzer::LINK_RADIUS = 1;


void TagsAnalyzer::analyze(std::string const& text, SemanticGraph const& graph)
{
	TextNormalizer normalizer;
	auto normText = normalizer.normalize(text);
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
	auto termsCounts = TermsUtils::extractTermsCounts(graph, normalizedText);
	std::map<std::string, size_t> termsViewsCounts;
	std::transform(termsCounts.begin(), termsCounts.end(), std::inserter(termsViewsCounts,termsViewsCounts.begin()), [&graph](std::pair<size_t, size_t> const& pair){return std::pair{graph.nodes.at(pair.first).term.view, pair.second};});
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
			node.weight = TermsUtils::calcTfIdf(static_cast<size_t>(node.weight), textTermsCount, node.term.numberOfArticlesThatUseIt, graph.nodes.size() + 1);
		}
}

void TagsAnalyzer::distributeTermWeight(SemanticGraph& graph, size_t centerTermHash, size_t radius, double weight)
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

SemanticGraph TagsAnalyzer::distributeTermsWeights(SemanticGraph const& graph)
{
	auto distrGraph = graph;
	for (auto&& [hash, node] : graph.nodes)
		if (node.weight > FLT_EPSILON) {
			distributeTermWeight(distrGraph, hash, LINK_RADIUS, node.weight * DISTRIBUTION_COEF);
		}
	return distrGraph;
}

void TagsAnalyzer::analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph)
{
	tagsGraph = graph;
	calcFrequency(tagsGraph, normalizedText);
	calcTfIdf(tagsGraph);
	tagsGraph = distributeTermsWeights(tagsGraph);
}


std::vector<Tag> TagsAnalyzer::getRelevantTags(size_t tagsCount)
{
	std::vector<Node> nodes;
	nodes.reserve(tagsGraph.nodes.size());
	std::transform(tagsGraph.nodes.begin(), tagsGraph.nodes.end(), std::back_inserter(nodes), [](auto el) {return el.second; });
	std::sort(nodes.begin(), nodes.end(), [](Node& n1, Node& n2) {return n1.weight > n2.weight; });
	tagsCount = tagsCount <= nodes.size() ? tagsCount : nodes.size();
	
	std::vector<Tag> tags;
	tags.reserve(tagsCount);
	std::transform(nodes.begin(), nodes.begin() + tagsCount, std::back_inserter(tags), [](Node& el) {return Tag{el.term.view, el.weight}; });
	return tags;

}


