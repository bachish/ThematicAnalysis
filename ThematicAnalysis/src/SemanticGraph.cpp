#include "SemanticGraph.h"

#include <stdexcept>

void SemanticGraph::addTerm(Term const& term)
{
	_graph[term] = std::map<Term, double, TermComparator>();
}

void SemanticGraph::addLink(size_t firstTermHash, size_t secondTermHash)
{
	if (isLinkExist(firstTermHash, secondTermHash))
		return;
	const auto ptr = _graph.find(firstTermHash);
	const auto ptr2 = _graph.find(secondTermHash);
	ptr->second[ptr2->first] = 0;
	ptr2->second[ptr->first] = 0;
}

void SemanticGraph::addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight)
{
	isLinkExist(firstTermHash, secondTermHash);
	const auto ptr = _graph.find(firstTermHash);
	const auto ptr2 = _graph.find(secondTermHash);
	ptr->second[ptr2->first] += weight;
	ptr2->second[ptr->first] += weight;
}

double SemanticGraph::getLinkWeight(size_t firstTermHash, size_t secondTermHash) const
{
	if(isLinkExist(firstTermHash, secondTermHash))
	{
		const auto ptr1= _graph.find(firstTermHash);
		return ptr1->second.find(secondTermHash)->second;
	}
	return 0;
}

bool SemanticGraph::isTermExist(size_t termHash) const
{
	return _graph.find(termHash) != _graph.end();
}

bool SemanticGraph::isLinkExist(size_t firstTermHash, size_t secondTermHash) const
{
	const auto ptr = _graph.find(firstTermHash);
	const auto ptr2 = _graph.find(secondTermHash);
	if (ptr == _graph.end() || ptr2 == _graph.end())
	{
		throw std::invalid_argument("invalid hash: term not exist");
	}
	for (auto [fst, snd] : ptr->second)
	{
		if (fst.getHashCode() == secondTermHash)
			return true;
	}
	return false;
}
