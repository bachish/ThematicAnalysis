#pragma once
#include "SemanticGraph.h"

class TextAnalyzer
{
public:
	//why public?
	SemanticGraph tagsGraph;
	void analyze(std::string const& textFilePath, SemanticGraph const& graph);

	void analyze(std::vector<std::string> const& normalizedText, SemanticGraph const& graph);
	std::vector<std::string> getRelevantTags(int tagsCount);
private:
	static void distributeTermWeight(SemanticGraph const& sourceGraph, SemanticGraph& graph, size_t centerTermHash, size_t radius);
	static SemanticGraph distributeWeights(SemanticGraph const& graph);
	static const double DAMP_COEF;
	static const size_t LINK_RADIUS;
	SemanticGraph _sourceGraph;
};



