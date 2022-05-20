#pragma once
#include "NormalizedArticle.h"
#include "SemanticGraph.h"

class SemanticGraphBuilder
{
public:
	SemanticGraph build(std::vector<NormalizedArticle> const& articles);
};
