#include "SemanticGraph.h"

#include "UGraphviz/UGraphviz.hpp"
#include <sstream>
#include <fstream>
#include <numeric>

#include "FileManager.h"
#include "Hasher.h"

void SemanticGraph::addTerm(Term const& term)
{
	if (_graph.find(term.getHashCode()) == _graph.end())
		_graph[term] = std::map<Term, double, TermComparator>();
}

void SemanticGraph::createLink(size_t firstTermHash, size_t secondTermHash, double weight)
{
	const auto ptr = _graph.find(firstTermHash);
	const auto ptr2 = _graph.find(secondTermHash);
	ptr->second[ptr2->first] = weight;
	ptr2->second[ptr->first] = weight;
}

void SemanticGraph::addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight)
{
	if (!isLinkExist(firstTermHash, secondTermHash))
	{
		//todo : what should it do?
		//maybe "throw std::invalid_argument("invalid hash: link not exist")"?
	}
	const auto ptr = _graph.find(firstTermHash);
	const auto ptr2 = _graph.find(secondTermHash);
	ptr->second[ptr2->first] += weight;
	ptr2->second[ptr->first] += weight;
}

void SemanticGraph::addTermWeight(size_t termHash, double weight)
{
	auto& term = _graph.find(termHash)->first;
	term.weight += weight;
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

std::map<size_t, Term> SemanticGraph::getAllTerms()
{
	auto terms = std::map<size_t, Term>();
	for (auto && [term, links] : _graph)
	{
		terms[term.getHashCode()] = term;
	}
	return terms;
}

std::map<Term, double, TermComparator> SemanticGraph::getAllNeighbors(size_t centralTermHash)
{
	return _graph.find(centralTermHash)->second;

}

/**
 * \brief Extract subGraph
 * \param centerHash subGraph center term
 * \param radius extraction level (from center term)
 * \return result subGraph
 */
SemanticGraph SemanticGraph::getNeighborhood(size_t centerHash, unsigned radius, unsigned minWeight) const
{
	auto neighbors = SemanticGraph();
	buildNeighborhood(centerHash, radius, minWeight, neighbors);
	return neighbors;
}

void SemanticGraph::buildNeighborhood(size_t curHash, unsigned radius, unsigned minWeight,
	SemanticGraph& neighbors) const
{
	if (!isTermExist(curHash)) return;
	const auto termMap = _graph.find(curHash);
	neighbors.addTerm(termMap->first);
	if (radius == 0) return;
	for (auto&& [neib, weight] : termMap->second)
	{
		const auto neighborHash = neib.getHashCode();
		if (!neighbors.isTermExist(neighborHash) && weight >= minWeight) {
			buildNeighborhood(neighborHash, radius - 1, minWeight, neighbors);
			neighbors.createLink(curHash, neighborHash);
			neighbors.addLinkWeight(curHash, neighborHash, getLinkWeight(curHash, neighborHash));
		}
	}
}


std::string doubleToString(double num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

std::string SemanticGraph::getDotView() const
{
	Ubpa::UGraphviz::Graph gr("SemanticGraph");
	auto& reg = gr.GetRegistry();
	std::map<Term, size_t, TermComparator> nodes;
	std::map<Term, bool, TermComparator> visited;
	for (auto&& [term, neighbors] : _graph)
	{
		auto nodeId = reg.RegisterNode(term.view);
		gr.AddNode(nodeId);
		nodes[term] = nodeId;
		visited[term] = false;
	}

	for (auto&& [term, neighbors] : _graph) {
		for (auto&& [neighbor, weight] : neighbors)
			if (!visited[neighbor]) {
				auto edgeId = reg.RegisterEdge(nodes[term], nodes[neighbor]);
				reg.RegisterEdgeAttr(edgeId, "label", doubleToString(weight));
				gr.AddEdge(edgeId);
			}
		visited[term] = true;
	}
	gr.RegisterGraphAttr("overlap", "false");
	return gr.Dump();
}


///	EXPORT/IMPORT FORMAT
/// <vertexes count>
/// <view>
/// <weight> <normalized words count> <word 1> ...
/// ...
/// <edges count>
///	<first term index> <second term index> <weight>
///	...
void SemanticGraph::exportToFile(std::string const& filePath)
{
	std::ofstream fout(filePath);
	exportToStream(fout);
	fout.close();
}

void SemanticGraph::exportToStream(std::ostream& out)
{
	std::map<Term, bool, TermComparator> visited;
	std::map<Term, int, TermComparator> indexes;
	out << _graph.size() << std::endl;
	int index = 0;
	for (auto&& [term, neighbors] : _graph)
	{
		out << term.view << '\n' << term.weight << ' ' << term.normalizedWords.size() << ' ';
		for (auto&& word : term.normalizedWords)
			out << word << ' ';
		out << std::endl;
		indexes[term] = index++;
		visited[term] = false;
	}

	auto edgesCount = std::accumulate(_graph.begin(), _graph.end(), static_cast<size_t>(0), [](size_t cnt, auto pair) {return cnt + pair.second.size(); }) / 2;
	out << edgesCount << std::endl;
	for (auto&& [term, neighbors] : _graph) {
		for (auto&& [neighbor, weight] : neighbors)
			if (!visited[neighbor]) {
				out << indexes[term] << ' ' << indexes[neighbor] << ' ' << weight << std::endl;
			}
		visited[term] = true;
	}
}

void SemanticGraph::importFromFile(std::string const& filePath)
{
	std::ifstream fin(filePath);
	std::stringstream ss(FileManager::readAllFile(fin));
	importFromStream(ss);
	fin.close();
}

Term readTerm(std::istream& in)
{
	std::string view;
	double weight;
	size_t wordsCount;
	std::ws(in);
	std::getline(in, view);
	in >> weight >> wordsCount;
	std::vector<std::string> words(wordsCount);
	for (size_t i = 0; i < wordsCount; i++)
		in >> words[i];
	return { words, view, Hasher::sortAndCalcHash(words) };
}

void SemanticGraph::importFromStream(std::istream& in)
{
	int termsCount, linksCount;
	in >> termsCount;
	std::vector<Term> terms;
	terms.reserve(termsCount);
	for (int i = 0; i < termsCount; i++) {
		auto term = readTerm(in);
		terms.push_back(term);
		addTerm(term);
	}
	in >> linksCount;
	while (linksCount--) {
		size_t firstTermIndex, secondTermIndex;
		double weight;
		in >> firstTermIndex >> secondTermIndex >> weight;

		createLink(terms[firstTermIndex].getHashCode(), terms[secondTermIndex].getHashCode(), weight);
	}
}

void SemanticGraph::drawToImage(std::string const& dirPath, std::string const& imageName) const
{
	std::string dotFile = "temp.dot";
	FileManager::writeUTF8ToFile(dotFile, getDotView());
	std::string command = std::string("neato -Tpng temp.dot  -o ") + dirPath + imageName + ".png";
	system(command.c_str());
	std::remove(dotFile.c_str());
}

