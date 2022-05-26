#include "SemanticGraph.h"

#include <execution>

#include "UGraphviz/UGraphviz.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <numeric>

#include "FileManager.h"
#include "Hasher.h"
#include "StringUtils.h"

Node::Node(Term term) :
	term(term),
	weight(0)
{
}

Node::Node()
{
}

double Node::sumLinksWeight()
{
	double sum = 0;
	for (auto [hash, link] : neighbors)
	{
		sum += link.weight;
	}
	return sum;
}

Link::Link(double weight)
	:weight(weight)
{
}

Link::Link() : Link(0)
{
}

void SemanticGraph::addTerm(Term const& term)
{
	auto const& [_, isInserted] = nodes.emplace(term.getHashCode(), Node(term));
	if (!isInserted)
	{
		std::cout << "Term " + term.view + " already exist!\n";
	}
}

void SemanticGraph::createLink(size_t firstTermHash, size_t secondTermHash, double weight)
{
	auto& firstNode = nodes.at(firstTermHash);
	auto const& [_, isInserted] = firstNode.neighbors.emplace(secondTermHash, Link(weight));
	if (!isInserted) {
		if (isTermExist(firstTermHash) && isTermExist(secondTermHash)) {
			auto term1 = nodes.at(firstTermHash).term;
			auto term2 = nodes.at(secondTermHash).term;
			throw std::logic_error("Link " + term1.view + " -> " + term2.view + " already exist!");

		}
		throw std::logic_error("Link already exist!");
	}
}

void SemanticGraph::addTermWeight(size_t termHash, double weight)
{
	nodes.at(termHash).weight += weight;
}

double SemanticGraph::getLinkWeight(size_t firstTermHash, size_t secondTermHash) const
{
	return nodes.at(firstTermHash).neighbors.at(secondTermHash).weight;
}

bool SemanticGraph::isTermExist(size_t termHash) const
{
	return nodes.find(termHash) != nodes.end();
}

bool SemanticGraph::isLinkExist(size_t firstTermHash, size_t secondTermHash) const
{
	const auto& ptr = nodes.find(firstTermHash);
	return ptr != nodes.end() && ptr->second.neighbors.find(secondTermHash) != ptr->second.neighbors.end();
}


/**
 * \brief Extract subGraph
 * \param centerHash subGraph center term
 * \param radius extraction level (from center term)
 * \return result subGraph
 */
SemanticGraph SemanticGraph::getNeighborhood(size_t centerHash, unsigned radius, double minWeight) const
{
	auto neighbors = SemanticGraph();
	buildNeighborhood(centerHash, radius, minWeight, neighbors);
	return neighbors;
}

void SemanticGraph::buildNeighborhood(size_t curHash, unsigned radius, double minWeight,
	SemanticGraph& neighbors) const
{
	if (!isTermExist(curHash)) return;
	const auto& termNode = nodes.find(curHash)->second;
	neighbors.addTerm(termNode.term);
	if (radius == 0) return;
	for (auto&& [hash, link] : termNode.neighbors)
	{
		if (!neighbors.isTermExist(hash) && link.weight >= minWeight) {
			buildNeighborhood(hash, radius - 1, minWeight, neighbors);
			neighbors.createLink(curHash, hash, link.weight);
		}
	}
}

std::string doubleToString(double num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

std::string breakText(std::string const& text, int maxLen)
{
	auto words = StringUtils::split(text);
	std::stringstream ss;
	size_t len = 0;
	for (auto& word : words)
	{
		ss << word << ' ';
		len += word.size();
		if (len > maxLen)
		{
			ss << '\n';
			len = 0;
		}
	}
	return ss.str();
}

std::string SemanticGraph::getDotView() const
{
	Ubpa::UGraphviz::Graph gr("SemanticGraph", true);
	auto& reg = gr.GetRegistry();
	std::map<size_t, size_t> registredNodes;
	std::map<size_t, bool> visited;
	for (auto&& [hash, node] : nodes)
	{
		auto nodeId = reg.RegisterNode(breakText(node.term.view, 6));
		gr.AddNode(nodeId);
		registredNodes.emplace(hash, nodeId);
		visited.emplace(hash, false);
	}

	for (auto&& [hash, node] : nodes) {
		for (auto&& [neighbor_hash, link] : node.neighbors)
			if (!visited[neighbor_hash]) {
				auto edgeId = reg.RegisterEdge(registredNodes[hash], registredNodes[neighbor_hash]);
				reg.RegisterEdgeAttr(edgeId, "label", doubleToString(link.weight));
				gr.AddEdge(edgeId);
			}
		visited.at(hash) = true;
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
	std::map<size_t, bool> visited;
	std::map<size_t, int> indexes;
	out << nodes.size() << std::endl;
	int index = 0;
	for (auto&& [hash, node] : nodes)
	{
		out << node.term.view << '\n' << node.weight << ' ' << node.term.normalizedWords.size() << ' ';
		for (auto&& word : node.term.normalizedWords)
			out << word << ' ';
		out << std::endl;
		indexes[hash] = index++;
		visited[hash] = false;
	}

	auto edgesCount = std::transform_reduce(std::execution::par, nodes.begin(), nodes.end(), 0ull,
		[](size_t cnt, size_t cnt2) {return cnt + cnt2; },
		[](auto const& pair) {return pair.second.neighbors.size(); });

	out << edgesCount << std::endl;
	for (auto&& [hash, node] : nodes)
		for (auto&& [neighborHash, link] : node.neighbors)
			out << indexes[hash] << ' ' << indexes[neighborHash] << ' ' << link.weight << std::endl;
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
	std::string command = std::string("external\\graphviz\\neato.exe  -Tpng temp.dot  -o ") + dirPath + imageName + ".png";
	system(command.c_str());
	std::remove(dotFile.c_str());
}

