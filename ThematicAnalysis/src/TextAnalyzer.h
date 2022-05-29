#pragma once
#include "SemanticGraph.h"


struct Tag
{
	std::string termView;
	double weight;
};

class TextAnalyzer
{
public:
	//why public?
	SemanticGraph tagsGraph;
	void analyze(std::string const& text, SemanticGraph const& graph);
	void analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph);

	std::vector<Tag> getRelevantTags(size_t tagsCount);

private:
	static void distributeTermWeight(SemanticGraph& graph, size_t centerTermHash, size_t radius, double weight);
	static SemanticGraph distributeTermsWeights(SemanticGraph const& graph);

	static const double DISTRIBUTION_COEF;	// the percent of the central vertex weight, transfer to the neighbors in the distribution
	static const double ABSORPTION_COEF;	// percent of weight absorbed by the vertex in distribution
	static const size_t LINK_RADIUS;
	SemanticGraph _sourceGraph;
};



