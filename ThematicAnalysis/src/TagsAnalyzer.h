#pragma once
#include "SemanticGraph.h"


struct Tag
{
	std::string termView;
	double weight;
};

class TagsAnalyzer
{
public:
	//why public?
	SemanticGraph tagsGraph;
	void analyze(std::string const& text, SemanticGraph const& graph);
	void analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph);

	std::vector<Tag> getRelevantTags(size_t tagsCount);

	 double distributionCoef = 1.;		// the percent of the central vertex weight, transfer to the neighbors in the distribution
	 double absorptionCoef = 0.5;		// percent of weight absorbed by the vertex in distribution
	 size_t linkRadius = 1;

private:
	void distributeTermWeight(SemanticGraph& graph, size_t centerTermHash, size_t radius, double weight) const;
	 SemanticGraph distributeTermsWeights(SemanticGraph const& graph) const;

	SemanticGraph _sourceGraph;
};



