#include "TextAnalyzer.h"

#include <algorithm>
#include <iostream>
#include <iterator>

#include "DocumentReader.h"
#include "FileManager.h"
#include "SemanticGraphBuilder.h"
#include "Hasher.h"
#include "Normalizer.h"
constexpr double TextAnalyzer::WEIGHT_ADDITION = 3.0;
constexpr size_t TextAnalyzer::LINK_RADIUS = 1;


void TextAnalyzer::analyze(std::string const& textFilePath, SemanticGraph const& graph)
{
	DocumentReader reader;
	auto normText = reader.readAndNormalizeText(textFilePath);
	analyze(normText, graph);
}

void TextAnalyzer::analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph)
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
	if (radius > 0)
	{
		auto node = tagsGraph.nodes.at(centerTermHash);
		const auto weightSum = node.sumLinksWeight();
		for (auto [neighbor_hash, link] : node.neighbors)
		{
			recalculateTermWeight(neighbor_hash, radius - 1, link.weight / weightSum);
		}
	}
}
