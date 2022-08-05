#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "SemanticGraph.h"
#include "TagsAnalyzer.h"
#include "TextNormalizer.h"
#include "ArticlesReader/IArticlesReader.h"
#include "Utils/FileUtils.h"
#include "Utils/Hasher.h"
#include "Utils/StringUtils.h"
#include "Utils/TermsUtils.h"

namespace AnalyzeUtils
{
	std::string readEncyclopedia(std::string const& encName);
	std::string readArticle(std::string const& articleName);
	std::string getGraphPath(std::string const& graphName);

	SemanticGraph getGraph(const std::string& graphName);

	void printWrongTerms(std::string const& encName, IArticlesReader const& reader);
	void calcGraphStatistics(const SemanticGraph& graph);
	void printArticlesTags(const SemanticGraph& graph, std::string const& articleName, std::string const& articleText, int cnt);
	void calcTopTerms(SemanticGraph const& graph);
	void calcTermsCountsInArticle(SemanticGraph const& graph, std::string const& articleName);
	void drawTermSubGraph(SemanticGraph const& graph, std::string const& term, int radius, double minEdgeWeight);
	void listEncyclopediaTerms(std::string const& encName, IArticlesReader const& reader);
	void createAndExportGraph(std::string const& encName, IArticlesReader const& reader);
	void printTextTags(const SemanticGraph& graph, const std::string& text, int cnt, std::ostream& out);
	void printGraphStructure(SemanticGraph const& gr, std::ostream& out);
};


