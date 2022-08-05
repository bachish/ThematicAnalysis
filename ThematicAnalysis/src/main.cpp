#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#include "Utils/FileUtils.h"
#include "TextNormalizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils/TermsUtils.h"
#include "TagsAnalyzer.h"
#include "ArticlesReader/MathArticlesReader.h"
#include "ArticlesReader/XmlArticlesReader.h"
#include "Utils/Hasher.h"
#include "Utils/StringUtils.h"
#include "AnalyzeUtils.h"



int main() {
	using namespace AnalyzeUtils;
	srand(time(nullptr));
	setlocale(LC_ALL, "rus");
	//auto graph = getGraph("math");
	/*for(int i = 1; i < 11; i++)
		AnalyzeUtils::printArticlesTags(graph, std::to_string(i), readArticle(std::to_string(i)), 30);*/
		//listEncyclopediaTerms("math", MathArticlesReader());
	createAndExportGraph("med", MathArticlesReader());
//	printWrongTerms("math", MathArticlesReader());
	return 0;
}