#pragma once
#include <map>
#include "Term.h"

class SemanticGraph
{
public:
	void addTerm(Term const& term);
	void createLink(size_t firstTermHash, size_t secondTermHash, double weight = 1);
	void addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight);
	double getLinkWeight(size_t firstTermHash, size_t secondTermHash) const;
	bool isTermExist(size_t termHash) const; 
	bool isLinkExist(size_t firstTermHash, size_t secondTermHash) const;

	SemanticGraph getNeighborhood(size_t centerHash, unsigned radius, unsigned minWeight = 1) const;
	std::string getDotView() const;

	void exportToFile(std::string const& filePath);
	void exportToStream(std::ostream& out);
	void importFromFile(std::string const& filePath);
	void importFromStream(std::istream& in);
	 void drawToImage(std::string const& dirPath, std::string const& imageName) const;


private:
	std::map<Term, std::map<Term, double, TermComparator>, TermComparator> _graph;
	void buildNeighborhood(size_t curHash, unsigned radius,unsigned minWeight, SemanticGraph& current) const;
};