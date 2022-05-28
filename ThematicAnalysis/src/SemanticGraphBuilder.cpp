#include "SemanticGraphBuilder.h"

#include <execution>
#include <set>
#include <cmath>
#include <iterator>
#include <list>

#include "DocumentReader.h"
#include "Hasher.h"
#include "MathUtils.h"

constexpr double SemanticGraphBuilder::WEIGHT_ADDITION = 1.0;
constexpr size_t SemanticGraphBuilder::N_FOR_NGRAM = 4;

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

void SemanticGraphBuilder::calculateTermsUsedDocuments(std::vector<NormalizedArticle> const& articles)
{
	for (auto&& article : articles)
	{
		auto terms = std::set<size_t>();
		for (size_t n = 1; n < N_FOR_NGRAM; n++)
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

std::map<size_t, size_t> SemanticGraphBuilder::calcLinkedTermsCounts(std::vector<std::string> const& words) const
{
	std::map<size_t, size_t> linkedTerms;
	for (int n = 1; n <= N_FOR_NGRAM; n++) {

		if (words.size() < n)
			return linkedTerms;
		for (size_t pos = 0; pos < words.size() - n + 1; pos++)
		{
			auto ngramHash = Hasher::sortAndCalcHash(words, pos, n);
			auto linkedTermIt = linkedTerms.find(ngramHash);
			if (linkedTermIt != linkedTerms.end())
			{
				linkedTermIt->second++;
			}
			else if (_graph.isTermExist(ngramHash))
			{
				linkedTerms.emplace(ngramHash, 1ull);
			}
		}
	}
	return linkedTerms;
}

/**
 * \brief extract and calculate terms in text
 * \return pairs vector of term hash and count term in text
 */
std::map<size_t, size_t> extractAndCalculateTerms(SemanticGraph const& graph, std::vector<std::string> const& allWords)
{
	std::map<size_t, size_t> termsCounts;
	std::list<std::pair<size_t, size_t>> termsPlaces;
	for (size_t n = std::min(SemanticGraphBuilder::N_FOR_NGRAM, allWords.size()); n > 0; n--)
	{
		for (size_t pos = 0; pos < allWords.size() - n + 1; pos++)
		{
			auto ngramHash = Hasher::sortAndCalcHash(allWords, pos, n);
			auto term = termsCounts.find(ngramHash);
			if (term != termsCounts.end())
			{
				term->second++;
				auto it = std::find_if(termsPlaces.begin(), termsPlaces.end(),
					[pos](auto& a) {return a.first > pos; });
				termsPlaces.insert(it, { pos, pos + n - 1 });

			}
			else if (graph.isTermExist(ngramHash))
			{
				auto isIntersect = [pos, n](auto& a)
				{
					auto [start, end] = a;
					auto endPos = pos + n - 1;
					return start <= pos && pos <= end || start <= endPos && endPos <= end;
				};
				auto intersectionIt = std::find_if(termsPlaces.begin(), termsPlaces.end(),isIntersect);
				if (intersectionIt == termsPlaces.end())
				{
					auto it = std::find_if(termsPlaces.begin(), termsPlaces.end(),
						[pos](auto& a) {return a.first > pos; });
					termsPlaces.insert(it, { pos, pos + n - 1 });
					termsCounts.emplace(ngramHash, 1ull);
				}
			}
		}
	}
	return termsCounts;
}

size_t getTermsCount(std::map<size_t, size_t> const& termsCount)
{
	return std::transform_reduce(std::execution::par, termsCount.begin(), termsCount.end(), 0ull, [](size_t a, size_t b) {return a + b; }, [](auto const& pair) {return pair.second; });
}

SemanticGraph SemanticGraphBuilder::build(std::vector<NormalizedArticle> const& sourceArticles)
{
	_graph = SemanticGraph();
	//auto const& articles = sourceArticles;
	auto articles = concatArticlesWithSameName(sourceArticles);
	addAllTermsToGraph(articles);
	calculateTermsUsedDocuments(articles);
	for (auto&& article : articles)
	{
		auto titleHash = Hasher::sortAndCalcHash(article.titleWords);
		auto const& contentWords = article.text;
		auto linkedTermsCounts = calcLinkedTermsCounts(contentWords);
		auto linkedTermsSumCount = getTermsCount(linkedTermsCounts);
		auto articlesCount = articles.size();

		for (auto [termHash, linkCount] : linkedTermsCounts)
			if (titleHash != termHash) {
				auto const& term = _graph.nodes[termHash].term;
				auto tfIdf = MathUtils::calcTfIdf(linkCount, linkedTermsSumCount, term.numberOfArticlesThatUseIt, articlesCount);
				_graph.createLink(titleHash, termHash, tfIdf);
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
