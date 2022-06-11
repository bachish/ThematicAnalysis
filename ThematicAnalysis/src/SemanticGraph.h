#pragma once
#include <map>
#include "Term.h"

class Link;

class Node
{
public:
	Node(Term term);
	Node(Term term, double weight);
	Node();
	Node(Node const& node) = default;
	Term term;
	double weight;
	int cnt = 0;
	std::map<size_t, Link> neighbors;
	double sumLinksWeight() const;

	mutable bool isSumLinksWeightsChanged = true;
	mutable double sumLinksWeights = true;

};

class Link
{
public:
	Link(double weight);
	Link();
	double weight;
};


namespace Ubpa::UGraphviz { class Graph; }

class SemanticGraph
{
public:
	std::map<size_t, Node> nodes;
	size_t getNGramLength() const;
	SemanticGraph(size_t nForNgrams = 4);

	void addTerm(Term const& term, double weight = 0);
	void createLink(size_t firstTermHash, size_t secondTermHash, double weight = 0);

	void addTermWeight(size_t termHash, double weight);
	void addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight);

	double getLinkWeight(size_t firstTermHash, size_t secondTermHash) const;
	bool isTermExist(size_t termHash) const;
	bool isLinkExist(size_t firstTermHash, size_t secondTermHash) const;
	void merge(SemanticGraph const& src);
	SemanticGraph getNeighborhood(size_t centerHash, unsigned radius, double minEdgeWeight = 0, double minNodeWeight = 0) const;
	std::string getDotView() const;
	std::string getDotView(size_t centerHash) const;

	void exportToFile(std::string const& filePath);
	void exportToStream(std::ostream& out);
	void importFromFile(std::string const& filePath);
	void importFromStream(std::istream& in);
	void drawToImage(std::string const& imagePngPath) const;
	void drawToImage(std::string const& imagePngPath, size_t centerHash) const;

private:
	size_t _nGramLength;
	void buildNeighborhood(size_t curHash, unsigned radius, double minEdgeWeight, double minNodeWeight, SemanticGraph& current) const;
	Ubpa::UGraphviz::Graph createDotView(std::map<size_t, size_t>& registredNodes) const;
};