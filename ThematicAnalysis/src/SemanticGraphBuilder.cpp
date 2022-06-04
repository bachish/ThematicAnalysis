#include "SemanticGraphBuilder.h"
#include <execution>
#include <set>
#include <iterator>

#include "ArticlesNormalizer.h"
#include "Hasher.h"
#include "Utils/TermsUtils.h"
#include "ArticlesReader/XmlArticlesReader.h"


void SemanticGraphBuilder::addAllTermsToGraph(std::vector<NormalizedArticle> const& articles)
{
	for (auto&& article : articles)
		_graph.addTerm(Term(article.titleWords, article.titleView, Hasher::sortAndCalcHash(article.titleWords)));
}

std::vector<NormalizedArticle> concatArticlesWithSameName(std::vector<NormalizedArticle> const& articles)
{
	std::map<size_t, std::vector<NormalizedArticle>> hashToArticles;
	for (auto&& article : articles)
	{
		auto articleHash = Hasher::sortAndCalcHash(article.titleWords);
		auto articleIt = hashToArticles.find(articleHash);
		hashToArticles[articleHash].push_back(std::move(article));
	}
	if (articles.size() == hashToArticles.size()) return articles;
	std::vector<NormalizedArticle> newArticles;
	std::transform(hashToArticles.begin(), hashToArticles.end(), std::back_inserter(newArticles),
		[](auto& it)
		{
			std::vector<NormalizedArticle>& articles = it.second;
			if (articles.size() > 1) {
				std::vector<std::string> contentSum;
				std::for_each(articles.begin(), articles.end(), [&contentSum](NormalizedArticle const& art)
					{
						contentSum.insert(contentSum.end(), art.text.begin(), art.text.end());
					});
				articles[0].text = contentSum;
			}
			return articles[0];
		});
	return newArticles;
}

/**
 * \brief for each term, count how many articles use it
 */
void SemanticGraphBuilder::countTermsUsedDocuments(std::vector<NormalizedArticle> const& articles)
{
	for (auto&& article : articles)
	{
		auto terms = std::set<size_t>();
		for (size_t n = 1; n < _graph.getNGramLength(); n++)
		{
			if (article.text.size() < n)
				break;
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
			_graph.nodes.at(termsHash).term.numberOfArticlesThatUseIt += 1;
		}
	}
}

/**
 * \brief total count terms in all articles
 * (count of words in text)
 */
size_t getTermsCountsSum(std::map<size_t, size_t> const& termsCount)
{
	return std::transform_reduce(std::execution::par, termsCount.begin(), termsCount.end(), 0ull, [](size_t a, size_t b) {return a + b; }, [](auto const& pair) {return pair.second; });
}

SemanticGraphBuilder::SemanticGraphBuilder(size_t nGramLenght) : _graph(nGramLenght), nGramLenght(nGramLenght)
{
}

SemanticGraph SemanticGraphBuilder::build(std::vector<NormalizedArticle> const& sourceArticles)
{
	_graph = SemanticGraph(nGramLenght);
	auto articles = concatArticlesWithSameName(sourceArticles);
	addAllTermsToGraph(articles);
	countTermsUsedDocuments(articles);
	for (auto&& article : articles)
	{
		auto titleHash = Hasher::sortAndCalcHash(article.titleWords);
		auto const& contentWords = article.text;
		auto linkedTermsCounts = TermsUtils::extractTermsCounts(_graph, contentWords);
		auto linkedTermsSumCount = getTermsCountsSum(linkedTermsCounts);
		auto articlesCount = articles.size();

		for (auto [termHash, linkCount] : linkedTermsCounts)
			if (titleHash != termHash) {
				auto const& term = _graph.nodes[termHash].term;
				auto tfIdf = TermsUtils::calcTfIdf(linkCount, linkedTermsSumCount, term.numberOfArticlesThatUseIt, articlesCount);
				if (_graph.isLinkExist(titleHash, termHash))
					_graph.addLinkWeight(termHash, titleHash, tfIdf);
				else
					_graph.createLink(titleHash, termHash, tfIdf);
				if (_graph.isLinkExist(termHash, titleHash))
					_graph.addLinkWeight(termHash, titleHash, tfIdf * 0.7);
				else
					_graph.createLink(termHash, titleHash, tfIdf * 0.7);
			}
	}
	return _graph;
}

SemanticGraph SemanticGraphBuilder::build(std::string const& xmlText)
{
	ArticlesNormalizer articlesNormalizer;
	return build(articlesNormalizer.readAndNormalizeArticles(xmlText, XmlArticlesReader()));
}

SemanticGraph SemanticGraphBuilder::build(std::string const& articlesText, IArticlesReader const& articlesReader)
{
	ArticlesNormalizer reader;
	return build(reader.readAndNormalizeArticles(articlesText, articlesReader));
}