#include "SemanticGraphBuilder.h"

#include <set>

#include "DocumentReader.h"
#include "Hasher.h"

constexpr double SemanticGraphBuilder::WEIGHT_ADDITION = 1.0;
constexpr int SemanticGraphBuilder::N_FOR_NGRAM = 4;

void SemanticGraphBuilder::addAllTermsToGraph(std::vector<NormalizedArticle> const& articles)
{
	for (auto&& article : articles)
		_graph.addTerm(Term(article.titleWords, article.titleView, Hasher::sortAndCalcHash(article.titleWords)));
}

void SemanticGraphBuilder::calculateTermsUsedDocuments(std::vector<NormalizedArticle> const& articles)
{
	for (auto&& article : articles)
	{
		auto terms = std::set<size_t>();
		for (size_t n = 1; n < N_FOR_NGRAM; n++)
		{
			if (article.text.size() < n)
				return;
			for (size_t pos = 0; pos < article.text.size() - n + 1; pos++)
			{
				auto ngramHash = Hasher::sortAndCalcHash(article.text, pos, n);
				if (_graph.isTermExist(ngramHash))
				{
					terms.insert(ngramHash);
				}
			}
		}
		for (auto termsHash : terms)
		{
			_graph.nodes[termsHash].term.numberArticlesThatUseIt += 1;
		}
	}
}

void SemanticGraphBuilder::tryAddNormalizedNgramsToGraph(size_t titleTermHash, std::vector<std::string> const& words, std::map<size_t, size_t>& linkedTerms, size_t n)
{
	if (words.size() < n)
		return;
	for (size_t pos = 0; pos < words.size() - n + 1; pos++)
	{
		auto ngramHash = Hasher::sortAndCalcHash(words, pos, n);
		if (linkedTerms.find(ngramHash) != linkedTerms.end())
		{
			linkedTerms[ngramHash]++;
		}
		else if (_graph.isTermExist(ngramHash))
		{
			linkedTerms[ngramHash] = 1;
		}
	}
}

SemanticGraph SemanticGraphBuilder::build(std::vector<NormalizedArticle> const& articles)
{
	_graph = SemanticGraph();
	addAllTermsToGraph(articles);
	calculateTermsUsedDocuments(articles);
	for (auto&& article : articles)
	{
		auto titleHash = Hasher::sortAndCalcHash(article.titleWords);
		auto& contentWords = article.text;
		auto linkedTerms = std::map<size_t, size_t>();
		for (int n = 1; n <= N_FOR_NGRAM; n++) {
			tryAddNormalizedNgramsToGraph(titleHash, contentWords, linkedTerms, n);
		}
		for (auto [neighborHash, linkCount] : linkedTerms)
		{
			auto& term = _graph.nodes[neighborHash].term;
			auto tf = (double)linkCount / (double)article.text.size();
			auto idf = log((double)articles.size() / (double)term.numberArticlesThatUseIt);
			_graph.createLink(titleHash, neighborHash, tf * idf);
		}
	}
	return _graph;
}

SemanticGraph SemanticGraphBuilder::build(std::string const& xmlFilePath)
{
	DocumentReader reader;
	return build(reader.readAndNormalizeArticles(xmlFilePath));
}

SemanticGraph SemanticGraphBuilder::build(std::string const& articlesFilePath, IXmlConverter const& xmlConverter)
{
	DocumentReader reader;
	return build(reader.readAndNormalizeArticles(articlesFilePath, xmlConverter));
}
