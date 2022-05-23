#pragma once
#include "NormalizedArticle.h"
#include "SemanticGraph.h"

class SemanticGraphBuilder
{
public:
	SemanticGraph build(std::vector<NormalizedArticle> const& articles);
	SemanticGraph build(std::string const& articlesFilePath);

	static const double WEIGHT_ADDITION;
	static const int N_FOR_NGRAM;

private:
	void tryToAddLinkWeight(size_t titleTermHash, size_t ngramHash);
	void tryAddNormalizedNgramsToGraph(size_t titleTermHash, std::vector<std::string> const& words, size_t n);
	void addAllTermsToGraph(std::vector<NormalizedArticle> const& articles);

	SemanticGraph _graph;
};
