#pragma once
#include "NormalizedArticle.h"
#include "SemanticGraph.h"

class IArticlesReader;

class SemanticGraphBuilder
{
public:
	SemanticGraph build(std::vector<NormalizedArticle> const& articles);
	SemanticGraph build(std::string const& xmlText);
	SemanticGraph build(std::string const& articlesText, IArticlesReader const& articlesReader);
	SemanticGraph _graph;
	static const double WEIGHT_ADDITION;

private:
	void addAllTermsToGraph(std::vector<NormalizedArticle> const& articles);
	void countTermsUsedDocuments(std::vector<NormalizedArticle> const& articles);
};
