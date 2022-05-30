#pragma once
#include <map>

#include "SemanticGraph.h"

class TermsUtils
{
public:
	static double calcTfIdf(size_t termFreq, size_t termCount, size_t countOfArticlesUsedTerm, size_t articlesCount);
	static std::map<size_t, size_t> extractTermsCounts(SemanticGraph const& graph, std::vector<std::string> const& allWords);

};
