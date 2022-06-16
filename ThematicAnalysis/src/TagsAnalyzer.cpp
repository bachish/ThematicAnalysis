#include "TagsAnalyzer.h"

#include <algorithm>
#include <execution>
#include <iterator>

#include "ArticlesNormalizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils/TermsUtils.h"

void TagsAnalyzer::analyze(std::string const& text, SemanticGraph const& graph)
{
	TextNormalizer normalizer{};
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
	std::transform(termsCounts.begin(), termsCounts.end(), std::inserter(termsViewsCounts, termsViewsCounts.begin()), 
		[&graph](std::pair<size_t, size_t> const& pair) {return std::pair{ graph.nodes.at(pair.first).term.view, pair.second }; });
	for (auto&& [termhash, count] : termsCounts)
	{
		graph.nodes.at(termhash).cnt+=count;
		graph.addTermWeight(termhash, static_cast<double>(count));
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

void TagsAnalyzer::distributeTermWeight(SemanticGraph& graph, size_t centerTermHash, size_t radius, double weight) const
{
	if (radius > 0)
	{
		auto node = graph.nodes.at(centerTermHash);
		const auto weightSum = node.sumLinksWeight();
		for (auto [neighborHash, link] : node.neighbors)
		{
			auto neighborWeight = weight * (link.weight / weightSum);
			graph.addTermWeight(neighborHash, neighborWeight * absorptionCoef);
			distributeTermWeight(graph, neighborHash, radius - 1, neighborWeight * (1 - absorptionCoef));
		}
	}
}

SemanticGraph TagsAnalyzer::distributeTermsWeights(SemanticGraph const& graph) const
{
	auto distrGraph = graph;
	for (auto&& [hash, node] : graph.nodes)
		if (node.weight > FLT_EPSILON) {
			distributeTermWeight(distrGraph, hash, linkRadius, node.weight * distributionCoef);
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
	std::sort(nodes.begin(), nodes.end(), [](const Node& n1, const Node& n2) {return n1.weight > n2.weight; });
	tagsCount = tagsCount <= nodes.size() ? tagsCount : nodes.size();

	std::vector<Tag> tags;
	tags.reserve(tagsCount);
	std::transform(nodes.begin(), std::next(nodes.begin(), static_cast<long long>(tagsCount)), std::back_inserter(tags), [](const Node& el) {return Tag{ el.term.view, el.weight }; });
	return tags;
}
