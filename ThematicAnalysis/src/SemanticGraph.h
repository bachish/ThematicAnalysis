#pragma once
#include <map>
#include "Term.h"

class SemanticGraph
{
public:
	void addTerm(Term const& term);
	void addLink(size_t firstTermHash, size_t secondTermHash);
	void addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight);
	double getLinkWeight(size_t firstTermHash, size_t secondTermHash) const;
	bool isTermExist(size_t termHash) const; 
	bool isLinkExist(size_t firstTermHash, size_t secondTermHash) const;

	SemanticGraph getNeighborhood(size_t centerHash, unsigned radius) const;
	std::string getDotView() const;
private:
	std::map<Term, std::map<Term, double, TermComparator>, TermComparator> _graph;
	void buildNeighborhood(size_t centerHash, unsigned radius, SemanticGraph& current) const;
};