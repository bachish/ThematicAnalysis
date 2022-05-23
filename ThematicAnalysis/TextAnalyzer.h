#pragma once
#include "SemanticGraph.h"

class TextAnalyzer
{
public:
	SemanticGraph tagsGraph;
	void analyze(std::string textFilePath, SemanticGraph graph);
	void analyze(std::vector<std::string> normalizedText, SemanticGraph graph);
	std::vector<std::string> getRelevantTags(int tagsCount);
private:
};

