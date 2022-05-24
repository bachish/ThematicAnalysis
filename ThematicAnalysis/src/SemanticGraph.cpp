#include "SemanticGraph.h"

#include "UGraphviz/UGraphviz.hpp"
#include <sstream>
#include <fstream>
#include <numeric>

#include "FileManager.h"
#include "Hasher.h"

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

Link::Link(size_t hash, double weight)
	:hash(hash),
	weight(weight)
{
}

Link::Link()
{
}

void SemanticGraph::addTerm(Term const& term)
{
	if (nodes.find(term.getHashCode()) == nodes.end())
		nodes.insert({ term.getHashCode() ,  Node(term) });
}

void SemanticGraph::createLink(size_t firstTermHash, size_t secondTermHash, double weight)
{
	auto& node = nodes[firstTermHash];
	auto& node2 = nodes[secondTermHash];
	node.neighbors.insert({ secondTermHash, Link(secondTermHash, weight) });
	node.neighbors.insert({ firstTermHash, Link(firstTermHash, weight) });
}

void SemanticGraph::addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight)
{
	if (!isLinkExist(firstTermHash, secondTermHash))
	{
		//todo : what should it do?
		//maybe "throw std::invalid_argument("invalid hash: link not exist")"?
	}
	nodes[firstTermHash].neighbors[secondTermHash].weight += weight;
	nodes[secondTermHash].neighbors[firstTermHash].weight += weight;
}

void SemanticGraph::addTermWeight(size_t termHash, double weight)
{
	nodes[termHash].weight += weight;
}

double SemanticGraph::getLinkWeight(size_t firstTermHash, size_t secondTermHash)
{
	return nodes[firstTermHash].neighbors[secondTermHash].weight;
}

bool SemanticGraph::isTermExist(size_t termHash) const
{
	return nodes.find(termHash) != nodes.end();
}

bool SemanticGraph::isLinkExist(size_t firstTermHash, size_t secondTermHash) const
{
	const auto& ptr = nodes.find(firstTermHash);
	if (ptr == nodes.end() || !isTermExist(secondTermHash))
	{
		return false;
	}
	return ptr->second.neighbors.find(secondTermHash) != ptr->second.neighbors.end();
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

inline std::vector<std::string> split(std::string const& text)
{
	std::stringstream ss(text);
	std::vector<std::string> words;
	while (!ss.eof())
	{
		std::string str;
		ss >> str;
		words.push_back(str);
	}
	return words;
}

std::string breakText(std::string const& text, int maxLen)
{
	auto words = split(text);
	std::stringstream ss;
	int len = 0;
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
	Ubpa::UGraphviz::Graph gr("SemanticGraph");
	auto& reg = gr.GetRegistry();
	std::map<size_t, size_t> registredNodes;
	std::map<size_t, bool> visited;
	for (auto&& [hash, node] : nodes)
	{
		auto nodeId = reg.RegisterNode(breakText(node.term.view, 6));
		gr.AddNode(nodeId);
		registredNodes[hash] = nodeId;
		visited[hash] = false;
	}

	for (auto&& [hash, node] : nodes) {
		for (auto&& [neighbor_hash, link] : node.neighbors)
			if (!visited[neighbor_hash]) {
				auto edgeId = reg.RegisterEdge(registredNodes[hash], registredNodes[neighbor_hash]);
				reg.RegisterEdgeAttr(edgeId, "label", doubleToString(link.weight));
				gr.AddEdge(edgeId);
			}
		visited[hash] = true;
	}
	gr.RegisterGraphAttr("overlap", "2:false");
	gr.RegisterGraphAttr("splines", "true");
	gr.RegisterGraphAttr("start", "6");
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

	/// ?????????
	auto edgesCount = std::accumulate(nodes.begin(), nodes.end(), static_cast<size_t>(0), [](size_t cnt, auto pair)
		{return cnt + pair.second.neighbors.size(); }) / 2;
	out << edgesCount << std::endl;
	for (auto&& [hash, node] : nodes) {
		for (auto&& [neighborHash, link] : node.neighbors)
			if (!visited[neighborHash]) {
				out << indexes[hash] << ' ' << indexes[neighborHash] << ' ' << link.weight << std::endl;
			}
		visited[hash] = true;
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
	std::string command = std::string("fdp  -Tpng temp.dot  -o ") + dirPath + imageName + ".png";
	system(command.c_str());
	std::remove(dotFile.c_str());
}

