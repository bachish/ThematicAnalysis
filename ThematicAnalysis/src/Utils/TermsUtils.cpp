#include "TermsUtils.h"
#include <cmath>
#include <list>

#include "Hasher.h"

double TermsUtils::calcTfIdf(size_t termFreq, size_t termCount, size_t countOfArticlesUsedTerm, size_t articlesCount)
{
	auto tf = static_cast<double>(termFreq) / static_cast<double>(termCount);
	double idf;
	if (articlesCount == 0 || countOfArticlesUsedTerm == 0)
		idf = std::log10(static_cast<double>(articlesCount + 1) / static_cast<double>(countOfArticlesUsedTerm + 1));
	else
		idf = std::log10(static_cast<double>(articlesCount) / static_cast<double>(countOfArticlesUsedTerm));
	return tf*idf;
}
/**
 * \brief extract and calculate terms in text
 * \return pairs vector of term hash and count term in text
 */
std::map<size_t, size_t> TermsUtils::extractTermsCounts(SemanticGraph const& graph, std::vector<std::string> const& allWords)
{
	std::map<size_t, size_t> termsCounts;
	std::list<std::pair<size_t, size_t>> termsPlaces;
	for (size_t n = std::min(graph.getNForNgram(), allWords.size()); n > 0; n--)
	{
		for (size_t pos = 0; pos < allWords.size() - n + 1; pos++)
		{
			auto ngramHash = Hasher::sortAndCalcHash(allWords, pos, n);
			auto term = termsCounts.find(ngramHash);
			if (term != termsCounts.end() || graph.isTermExist(ngramHash))
			{
				auto isIntersect = [pos, n](auto& a)
				{
					auto [start, end] = a;
					auto endPos = pos + n - 1;
					return start <= pos && pos <= end || start <= endPos && endPos <= end;
				};
				auto intersectionIt = std::find_if(termsPlaces.begin(), termsPlaces.end(), isIntersect);
				if (intersectionIt == termsPlaces.end())
				{
					auto it = std::find_if(termsPlaces.begin(), termsPlaces.end(),
						[pos](auto& a) {return a.first > pos; });
					termsPlaces.insert(it, { pos, pos + n - 1 });
					if (term != termsCounts.end())
					{
						term->second++;
					}
					else
					{
						termsCounts.emplace(ngramHash, 1ull);
					}
				}
			}
		}
	}
	return termsCounts;
}
