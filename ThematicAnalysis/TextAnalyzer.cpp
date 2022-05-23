#include "TextAnalyzer.h"
#include "SemanticGraphBuilder.h"
#include "Hasher.h"
constexpr double TextAnalyzer::WEIGHT_ADDITION = 3.0;
constexpr size_t TextAnalyzer::LINK_RADIUS = 2;


void TextAnalyzer::analyze(std::vector<std::string> normalizedText, SemanticGraph graph)
{
	tagsGraph = SemanticGraph();
	_sourceGraph = graph;
	for (int n = 1; n <= SemanticGraphBuilder::N_FOR_NGRAM; n++) {
		for (size_t i = 0; i < normalizedText.size() - n; i++)
		{
			auto termHash = Hasher::sortAndCalcHash(normalizedText, i, n);
			if (graph.isTermExist(termHash))
			{
				rebuildTagsGraph(termHash);
			}
		}
	}
}

void TextAnalyzer::rebuildTagsGraph(size_t metTermHash)
{
	auto local = _sourceGraph.getNeighborhood(metTermHash, LINK_RADIUS);
	mergeGraphs(local);
	recalculateTermWeight(metTermHash, LINK_RADIUS);
}
void TextAnalyzer::mergeGraphs(SemanticGraph src)
{
	for (auto [hash, node] : src.nodes)
	{
		tagsGraph.addTerm(node.term);
	}
	for (auto [hash, node] : src.nodes)
	{
		for (auto [hash2, link] : node.neighbors)
		{
			if (tagsGraph.isTermExist(hash2))
				tagsGraph.createLink(hash, hash2, link.weight);
		}
	}
}
void TextAnalyzer::recalculateTermWeight(size_t centerTermHash, size_t radius, double delta)
{
	tagsGraph.addTermWeight(centerTermHash, delta);
	if(radius > 0)
	{
		auto node = tagsGraph.nodes.at(centerTermHash);
		const auto weightSum = node.sumLinksWeight();
		for (auto [neighbor_hash, link] : node.neighbors)
		{
			recalculateTermWeight(neighbor_hash, radius - 1, link.weight / weightSum);
		}
	}
}
