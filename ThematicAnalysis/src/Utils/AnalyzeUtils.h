#pragma once
#include "SemanticGraph.h"

class AnalyzeUtils
{
public:
	static void calcGraphStatistics(const SemanticGraph& graph);
	static void printArticlesTags(const SemanticGraph& graph, std::string const& articleName, std::string const& articleText, int cnt);
};


