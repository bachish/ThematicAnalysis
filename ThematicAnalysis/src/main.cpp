#include <clocale>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <boost/regex.hpp>
#include "DocumentReader.h"
#include "Hasher.h"
#include "Normalizer.h"
#include "SemanticGraphBuilder.h"
#include "XmlConverter/MathXmlConverter.h"
void create() {
	auto builder = SemanticGraphBuilder();
	auto reader = DocumentReader();
	auto graph = builder.build(reader.readAndNormalizeArticles("resources/AllMath.txt"));
	graph.exportToFile("resources/allMath.gr");
}

void draw()
{
	SemanticGraph graph;
	
	graph.importFromFile("resources/coolAllMath.gr");
	Normalizer normalizer;
	auto subgr = graph.getNeighborhood(Hasher::sortAndCalcHash(normalizer.normalize("первообразная")), 1, 3);
	subgr.drawToImage("", "image");
}


void regexTest()
{
	std::string str("софтваре");
	boost::regex str_expr("(софт)(.*)");

	boost::smatch sm;
	boost::regex_match(str.cbegin(), str.cend(), sm, str_expr);
	std::cout << "String:range, size:" << sm.size() << " matches\n";



	std::cout << "the matches are: ";
	for (unsigned i = 0; i < sm.size(); ++i) {
		std::cout << "[" << sm[i] << "] ";
	}

	std::cout << std::endl;
}

int main() {
	setlocale(LC_ALL, "rus");
	/*SemanticGraphBuilder builder;
	auto graph = builder.build("resources/math/math.txt", MathXmlConverter());
	graph.exportToFile("resources/coolAllMath.gr");*/
	draw();
	return 0;
}
