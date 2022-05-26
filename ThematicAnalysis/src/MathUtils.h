#pragma once

class MathUtils
{
public:
	static double calcTfIdf(size_t termFreq, size_t termCount, size_t countOfArticlesUsedTerm, size_t articlesCount);
};
