#pragma once
#include "SemanticGraph.h"

class TextAnalyzer
{
public:
	//why public?
	SemanticGraph tagsGraph;
	void analyze(std::string const& text, SemanticGraph const& graph);

	void analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph);
	std::vector<std::string> getRelevantTags(int tagsCount);
private:
	static void distributeTermWeight(SemanticGraph& graph, size_t centerTermHash, size_t radius, double weight);
	static SemanticGraph distributeWeights(SemanticGraph const& graph);
	static const double DISTRIBUTION_COEF;	// the percent of the central vertex weight, transfer to the neighbors in the distribution
	static const double ABSORPTION_COEF;	// percent of weight absorbed by the vertex in distribution
	static const size_t LINK_RADIUS;
	SemanticGraph _sourceGraph;
};



