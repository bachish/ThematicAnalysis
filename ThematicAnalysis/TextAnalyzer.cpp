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
	for (auto [hash, term] : src.getAllTerms())
	{
		tagsGraph.addTerm(term);
	}
	for (auto [hash, term] : src.getAllTerms())
	{
		for (auto [term2, weight] : src.getAllNeighbors(hash))
		{
			if (tagsGraph.isTermExist(term2.getHashCode()))
				tagsGraph.createLink(hash, term2.getHashCode(), weight);
		}
	}
}
void TextAnalyzer::recalculateTermWeight(size_t centerTermHash, size_t radius, double delta)
{
	tagsGraph.addTermWeight(centerTermHash, delta);
	if(radius > 0)
	{
		auto neighbors = tagsGraph.getAllNeighbors(centerTermHash);
		//todo calculate it and store in term
		double sumLinkWeightForNode = 10.;

		for (auto [terms, weight] : neighbors)
		{
			recalculateTermWeight(terms.getHashCode(), radius - 1, weight / sumLinkWeightForNode);
		}
	}
}
