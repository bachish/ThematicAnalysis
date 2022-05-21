#include <clocale>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <vector>
#include "UGraphviz/UGraphviz.hpp"
#include "DocumentReader.h"
#include "Lemmatizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils.h"


int main()
{
	setlocale(LC_ALL, "rus");
	auto graph = SemanticGraph();
	auto term = Term({ "АБАК" }, "АБАК");
	auto term2 = Term({ "АБЕЛЕВА", "ГРУППА" }, "АБЕЛЕВЫ ГРУППЫ");
	auto term3 = Term({ "СТЕПЕНЬ" }, "СТЕПЕНИ");
	graph.addTerm(term);
	graph.addTerm(term2);
	graph.addTerm(term3);
	graph.addLink(term.getHashCode(), term2.getHashCode());
	graph.addLinkWeight(term.getHashCode(), term2.getHashCode(), 12);
	graph.addLink(term3.getHashCode(), term2.getHashCode());
	Utils::drawGraphImageFromDot(graph.getDotView(), "image.png");
}
