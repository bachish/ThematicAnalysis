#include "SemanticGraph.h"

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
	if(isLinkExist(!firstTermHash, secondTermHash))
	{
		//todo : what should it do?
		//maybe "throw std::invalid_argument("invalid hash: link not exist")"?
	}
	const auto ptr = _graph.find(firstTermHash);
	const auto ptr2 = _graph.find(secondTermHash);
	ptr->second[ptr2->first] += weight;
	ptr2->second[ptr->first] += weight;
}

double SemanticGraph::getLinkWeight(size_t firstTermHash, size_t secondTermHash) const
{
	if (isLinkExist(firstTermHash, secondTermHash))
	{
		const auto ptr1 = _graph.find(firstTermHash);
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
	if (ptr == _graph.end() || !isTermExist(secondTermHash))
	{
		return false;
	}
	const auto neighbors = ptr->second;
	return neighbors.find(secondTermHash) != neighbors.end();
}

/**
 * \brief Extract subGraph 
 * \param centerHash subGraph center term
 * \param radius extraction level (from center term)
 * \return result subGraph
 */
SemanticGraph SemanticGraph::getNeighborhood(size_t centerHash, unsigned radius) const
{
	auto neighbors = SemanticGraph();
	buildNeighborhood(centerHash, radius, neighbors);
	return neighbors;
}

void SemanticGraph::buildNeighborhood(size_t centerHash, unsigned radius,
	SemanticGraph& neighbors) const
{
	if (!isTermExist(centerHash)) return;
	const auto termMap = _graph.find(centerHash);
	neighbors.addTerm(termMap->first);
	if (radius == 0) return;
	for (auto neib : termMap->second)
	{
		const auto neighborHash = neib.first.getHashCode();
		if (!neighbors.isTermExist(neighborHash)) {
			buildNeighborhood(neighborHash, radius - 1, neighbors);
			neighbors.addLink(centerHash, neighborHash);
		}
	}

}