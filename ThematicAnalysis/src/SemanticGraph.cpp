#include <execution>
#include <sstream>
#include <fstream>
#include <numeric>
#include "UGraphviz/UGraphviz.hpp"
#include "SemanticGraph.h"

#include <iomanip>
#include <utility>

#include "Utils/FileUtils.h"
#include "Utils/Hasher.h"
#include "TextNormalizer.h"
#include "Utils/StringUtils.h"

Node::Node(Term term) :
	term(std::move(term)),
	weight(0)
{
}

Node::Node(Term term, double weight) :
	term(std::move(term)),
	weight(weight)
{
}


double Node::sumLinksWeight() const
{
	//if (isSumLinksWeightsChanged) {
	sumLinksWeights = 0;
	for (auto&& [hash, link] : neighbors)
	{
		sumLinksWeights += link.weight;
	}
	isSumLinksWeightsChanged = false;
	//	}
	return sumLinksWeights;

}

Link::Link(double weight)
	:weight(weight)
{
}

Link::Link() : Link(0)
{
}

size_t SemanticGraph::getNGramLength() const
{
	return _nGramLength;
}

SemanticGraph::SemanticGraph(const std::string& name, size_t nForNgrams) : name(name), _nGramLength(nForNgrams)
{
}

size_t SemanticGraph::getTermsCount() const
{
	return nodes.size();
}

size_t SemanticGraph::getLinksCount() const
{
	return std::transform_reduce(std::execution::par, nodes.begin(), nodes.end(), 0ull,
		[](size_t cnt, size_t cnt2) {return cnt + cnt2; },
		[](auto const& pair) {return pair.second.neighbors.size(); });
}

void SemanticGraph::addTerm(Term const& term, double weight)
{
	nodes.emplace(term.getHashCode(), Node(term, weight));
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

void SemanticGraph::addLinkWeight(size_t firstTermHash, size_t secondTermHash, double weight)
{
	auto& firstNode = nodes.at(firstTermHash);
	firstNode.neighbors.at(secondTermHash).weight += weight;
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

void SemanticGraph::merge(SemanticGraph const& src)
{
	for (auto [hash, node] : src.nodes)
	{
		addTerm(node.term, node.weight);
		nodes.at(node.term.getHashCode()).cnt = node.cnt;
	}
	for (auto [hash, node] : src.nodes)
	{
		for (auto [hash2, link] : node.neighbors)
		{
			if (isTermExist(hash2) && !isLinkExist(hash, hash2))
				createLink(hash, hash2, link.weight);
		}
	}
}

/**
 * \brief Extract subGraph
 * \param centerHash subGraph center term
 * \param radius extraction level (from center term)
 * \param minEdgeWeight min edge weight
 * \param minNodeWeight min node weight
 * \return result subGraph
 */
SemanticGraph SemanticGraph::getNeighborhood(size_t centerHash, unsigned radius, double minEdgeWeight, double minNodeWeight) const
{
	auto neighbors = SemanticGraph(name + "_" + nodes.at(centerHash).term.view, _nGramLength);
	buildNeighborhood(centerHash, radius, minEdgeWeight, minNodeWeight, neighbors);
	return neighbors;
}

void SemanticGraph::buildNeighborhood(size_t curHash, unsigned radius, double minEdgeWeight,
	double minNodeWeight, SemanticGraph& neighbors) const
{
	if (!isTermExist(curHash)) return;
	const auto& termNode = nodes.find(curHash)->second;
	if (termNode.weight < minNodeWeight) return;
	neighbors.addTerm(termNode.term, termNode.weight);
	neighbors.nodes.at(termNode.term.getHashCode()).cnt = termNode.cnt;
	if (radius == 0) return;
	for (auto&& [hash, link] : termNode.neighbors)
	{
		if (link.weight >= minEdgeWeight) {
			if (!neighbors.isTermExist(hash))
				buildNeighborhood(hash, radius - 1, minEdgeWeight, minNodeWeight, neighbors);

			if (neighbors.isTermExist(hash) && !neighbors.isLinkExist(curHash, hash))
				neighbors.createLink(curHash, hash, link.weight);
		}
	}
}

std::string doubleToString(double num, int precision = 3)
{
	std::stringstream ss;
	ss << std::setprecision(precision) << num;
	return ss.str();
}

std::string breakText(std::string const& text, size_t maxLen)
{
	auto words = StringUtils::split(text);
	std::stringstream ss;
	size_t len = 0;
	for (auto& word : words)
	{
		if (len > maxLen)
		{
			ss << '\n';
			len = 0;
		}
		ss << word << ' ';
		len += word.size();

	}
	return ss.str();
}

Ubpa::UGraphviz::Graph SemanticGraph::createDotView(std::map<size_t, size_t>& registeredNodes) const
{
	Ubpa::UGraphviz::Graph dotGraph("SemanticGraph", true);
	auto& reg = dotGraph.GetRegistry();
	for (auto&& [hash, node] : nodes)
	{
		auto nodeId = reg.RegisterNode(breakText(node.term.view, 6) + (node.cnt < 1 ? "" : "cnt=" + std::to_string(node.cnt)) + (node.weight < 1e-5 ? " " : "\n" + doubleToString(node.weight)));
		dotGraph.AddNode(nodeId);
		registeredNodes.emplace(hash, nodeId);
	}
	auto tempHash = Hasher::sortAndCalcHash(TextNormalizer().normalize("Координаты"));
	auto tempHash2 = Hasher::sortAndCalcHash(TextNormalizer().normalize("Декартова прямоугольная система координат"));

	for (auto&& [hash, node] : nodes) {
		for (auto&& [neighbor_hash, link] : node.neighbors)
			if (hash != tempHash || neighbor_hash != tempHash2) {
				auto edgeId = reg.RegisterEdge(registeredNodes[hash], registeredNodes[neighbor_hash]);
				reg.RegisterEdgeAttr(edgeId, "label", doubleToString(link.weight, 2));
				reg.RegisterEdgeAttr(edgeId, "weight", std::to_string(static_cast<int>(link.weight * 1000)));
				dotGraph.AddEdge(edgeId);
			}
	}
	dotGraph.RegisterGraphAttr("overlap", "false");
	dotGraph.RegisterGraphAttr("start", "14");
	//dotGraph.RegisterGraphAttr("start", std::to_string(rand()));
	return dotGraph;
}

std::string SemanticGraph::getDotView() const
{
	std::map<size_t, size_t> registredNodes;
	return createDotView(registredNodes).Dump();
}

std::string SemanticGraph::getDotView(size_t centerHash) const
{
	std::map<size_t, size_t> registredNodes;
	auto gr = createDotView(registredNodes);
	if (nodes.find(centerHash) != nodes.end())
	{
		gr.GetRegistry().RegisterNodeAttr(registredNodes[centerHash], "color", "red");
		gr.GetRegistry().RegisterNodeAttr(registredNodes[centerHash], "fontname", "times-bold");
	}
	return gr.Dump();
}

///	EXPORT/IMPORT FORMAT
///	<ngram length>
/// <vertexes count>
/// <view>
/// <weight> <numberOfArticles> <normalized words count> <word 1> ...
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
	std::unordered_map<size_t, int> indexes;
	out << _nGramLength << std::endl;
	out << getTermsCount() << std::endl;
	int index = 0;

	for (auto&& [hash, node] : nodes)
	{
		out << node.term.view << '\n' << node.weight << ' ' << node.term.numberOfArticlesThatUseIt << ' ' << node.term.normalizedWords.size() << ' ';
		for (auto&& word : node.term.normalizedWords)
			out << word << ' ';
		out << std::endl;
		indexes[hash] = index++;
	}


	out << getLinksCount() << std::endl;
	for (auto&& [hash, node] : nodes)
		for (auto&& [neighborHash, link] : node.neighbors)
			out << indexes[hash] << ' ' << indexes[neighborHash] << ' ' << link.weight << std::endl;
}

void SemanticGraph::importFromFile(std::string const& filePath)
{
	std::ifstream fin(filePath);
	std::stringstream ss(FileUtils::readAllFile(fin));
	importFromStream(ss);
	fin.close();
}

Node readNode(std::istream& in)
{
	std::string view;
	double weight;
	size_t wordsCount, numberOfArticlesThatUseIt;
	std::getline(in, view);
	std::getline(in, view);
	in >> weight >> numberOfArticlesThatUseIt >> wordsCount;
	std::vector<std::string> words(wordsCount);
	for (size_t i = 0; i < wordsCount; i++)
		in >> words[i];

	Node node = { {words, view, Hasher::sortAndCalcHash(words)} , weight };
	node.term.numberOfArticlesThatUseIt = numberOfArticlesThatUseIt;
	return node;
}

void SemanticGraph::importFromStream(std::istream& in)
{
	int termsCount, linksCount;
	in >> _nGramLength >> termsCount;
	std::vector<Term> terms;
	terms.reserve(termsCount);
	for (int i = 0; i < termsCount; i++) {
		auto node = readNode(in);
		terms.push_back(node.term);
		addTerm(node.term, node.weight);
	}
	in >> linksCount;
	while (linksCount--) {
		size_t firstTermIndex, secondTermIndex;
		double weight;
		in >> firstTermIndex >> secondTermIndex >> weight;

		createLink(terms[firstTermIndex].getHashCode(), terms[secondTermIndex].getHashCode(), weight);
	}
}

void drawDotToImage(std::string const& dotView, std::string const& imagePngPath)
{
	std::string dotFile = "temp.dot";
	FileUtils::writeUTF8ToFile(dotFile, dotView);
	FileUtils::executeExeWithParams("external\\graphviz\\neato.exe", std::string(" -Tpng temp.dot  -o ") + imagePngPath);
	std::remove(dotFile.c_str());
}

void SemanticGraph::drawToImage(std::string const& imagePngPath) const
{
	drawDotToImage(getDotView(), imagePngPath);
}

void SemanticGraph::drawToImage(std::string const& imagePngPath, size_t centerHash) const
{
	auto a = getDotView(centerHash);
	drawDotToImage(a, imagePngPath);
}