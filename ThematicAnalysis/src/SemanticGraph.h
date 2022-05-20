#pragma once
#include <map>

#include "Term.h"

class SemanticGraph
{
public:
private:
	std::map<Term, std::map<Term, int>> _graph;
};
