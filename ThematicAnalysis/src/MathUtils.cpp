#include "MathUtils.h"
#include <cmath>

double MathUtils::calcTfIdf(size_t termFreq, size_t termCount, size_t countOfArticlesUsedTerm, size_t articlesCount)
{
	auto tf = static_cast<double>(termFreq) / static_cast<double>(termCount);
	double idf;
	if (articlesCount == 0 || countOfArticlesUsedTerm == 0)
		idf = std::log10(static_cast<double>(articlesCount + 1) / static_cast<double>(countOfArticlesUsedTerm + 1));
	else
		idf = std::log10(static_cast<double>(articlesCount) / static_cast<double>(countOfArticlesUsedTerm));
	return tf*idf;
}
