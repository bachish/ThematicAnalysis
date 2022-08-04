#pragma once
#include "NormalizedArticle.h"
#include "SemanticGraph.h"

class IArticlesReader;

class SemanticGraphBuilder
{
public:
	explicit SemanticGraphBuilder(std::string const& graph_name = "undefined", size_t nGramLength = 4);

	SemanticGraph build(std::vector<NormalizedArticle> articles);
	SemanticGraph build(std::string const& xmlText);
	SemanticGraph build(std::string const& articlesText, IArticlesReader const& articlesReader);
	SemanticGraph graph;

	const size_t nGramLenghth;
private:
	void addAllTermsToGraph(std::vector<NormalizedArticle> const& articles);
	void countTermsUsedDocuments(std::vector<NormalizedArticle> const& articles);
};
