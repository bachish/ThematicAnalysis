#include "SemanticGraphBuilder.h"

#include "Utils.h"

constexpr double SemanticGraphBuilder::WEIGHT_ADDITION = 1.0;
constexpr int SemanticGraphBuilder::N_FOR_NGRAM = 4;

void SemanticGraphBuilder::addAllTermsToGraph(std::vector<NormalizedArticle> const& articles)
{
	for (auto&& article : articles)
		_graph.addTerm(article.title);
}

void SemanticGraphBuilder::tryToAddLinkWeight(size_t titleTermHash, size_t ngramHash)
{
	if (_graph.isTermExist(ngramHash) && titleTermHash != ngramHash)
	{
		if (!_graph.isLinkExist(titleTermHash, ngramHash))
			_graph.createLink(titleTermHash, ngramHash);
		_graph.addLinkWeight(titleTermHash, ngramHash, SemanticGraphBuilder::WEIGHT_ADDITION);
	}
}

std::string getNormalizedNgram(std::vector<std::string> const& words, size_t pos, size_t n)
{
	return Utils::sortAndConcatWords(std::vector<std::string>(words.begin() + pos, words.begin() + pos + n));
}

void SemanticGraphBuilder::tryAddNormalizedNgramsToGraph(size_t titleTermHash, std::vector<std::string> const& words, size_t n)
{
	if (words.size() < n)
		return;
	for (size_t pos = 0; pos < words.size() - n + 1; pos++)
	{
		auto ngram = getNormalizedNgram(words, pos, n);
		auto ngramHash = Utils::calculateHashCode(ngram);
		tryToAddLinkWeight(titleTermHash, ngramHash);
	}
}

SemanticGraph SemanticGraphBuilder::build(std::vector<NormalizedArticle> const& articles)
{
	_graph = SemanticGraph();
	addAllTermsToGraph(articles);

	for (auto&& article : articles)
		for (int n = 1; n <= N_FOR_NGRAM; n++) {
			auto& contentWords = article.text;
			auto titleHash = article.title.getHashCode();
			tryAddNormalizedNgramsToGraph(titleHash, contentWords, n);
		}
	return _graph;
}
