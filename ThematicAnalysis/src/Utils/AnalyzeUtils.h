#pragma once
#include "SemanticGraph.h"

class AnalyzeUtils
{
public:
	static void calcStatistic(const SemanticGraph& graph);
	static void getTags(const SemanticGraph& graph, std::string const& articleName);
};


