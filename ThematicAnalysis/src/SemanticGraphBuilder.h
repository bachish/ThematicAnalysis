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
	static const size_t N_FOR_NGRAM;
	std::map<size_t, size_t> calcLinkedTermsCounts(std::vector<std::string> const& words) const;

private:
	void addAllTermsToGraph(std::vector<NormalizedArticle> const& articles);
	void calculateTermsUsedDocuments(std::vector<NormalizedArticle> const& articles);

};
std::map<size_t, size_t> extractAndCalculateTerms(SemanticGraph const& graph, std::vector<std::string> const& allWords);
