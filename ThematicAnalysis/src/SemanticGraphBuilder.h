#pragma once
#include "NormalizedArticle.h"
#include "SemanticGraph.h"
#include "XmlConverter/IXmlConverter.h"

class SemanticGraphBuilder
{
public:
	SemanticGraph build(std::vector<NormalizedArticle> const& articles);
	SemanticGraph build(std::string const& xmlFilePath);
	SemanticGraph build(std::string const& articlesFilePath, IXmlConverter const& xmlConverter);

	static const double WEIGHT_ADDITION;
	static const int N_FOR_NGRAM;

private:
	std::map<size_t, size_t> calcLinkedTermsCounts(std::vector<std::string> const& words) const;
	void addAllTermsToGraph(std::vector<NormalizedArticle> const& articles);
	void calculateTermsUsedDocuments(std::vector<NormalizedArticle> const& articles);

	SemanticGraph _graph;
};
