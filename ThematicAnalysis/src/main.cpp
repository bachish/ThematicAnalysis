#include <clocale>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <vector>

#include "DocumentReader.h"
#include "Lemmatizer.h"
#include "SemanticGraphBuilder.h"
#include "Utils.h"


int main()
{
	setlocale(LC_ALL, "rus");
	//	auto a = std::filesystem::current_path();
	auto builder = SemanticGraphBuilder();
	auto reader = DocumentReader();
	auto graph = builder.build(reader.read("../ThematicAnalysisTests/resources/AllMath.txt"));
	auto termHash = Utils::calculateNormalizedHashCode("Алгебраическая функция");
	graph = graph.getNeighborhood(termHash, 1);
	Utils::drawGraphImageFromDot(graph.getDotView(), "image.png");

}
