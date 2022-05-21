#include "Utils.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <string>
#include <locale>
#include <Windows.h>


size_t Utils::calculateHashCode(std::string const& text)
{
	static std::hash<std::string> _hashCalculator;
	return _hashCalculator(text);
}
size_t Utils::calculateTermHashCode(std::vector<std::string> words)
{
	return calculateHashCode(sortAndConcatWords(words));
}

std::string Utils::readAllFile(std::ifstream& fin)
{
	std::stringstream wss;
	wss << fin.rdbuf();
	return wss.str();
}

std::string cp1251ToUTF8(std::string str)
{
	auto size = str.size() + 1;
	std::string res;

	auto wsize = MultiByteToWideChar(1251, 0, str.c_str(), -1, nullptr, 0);
	auto wres = new WCHAR[wsize];
	MultiByteToWideChar(1251, 0, str.c_str(), -1, wres, wsize);

	auto usize = WideCharToMultiByte(CP_UTF8, 0, wres, -1, nullptr, 0, 0, 0);
	auto ures = new char[usize];
	WideCharToMultiByte(CP_UTF8, 0, wres, -1, ures, usize, 0, 0);
	res = ures;

	delete[] ures;
	delete[] wres;

	return res;
}

void Utils::writeToFile(std::string const& filePath, std::string const& text)
{
	std::ofstream fout(filePath);
	fout.imbue(std::locale(""));
	auto wtext = cp1251ToUTF8(text);
	fout << wtext;
	fout.close();
}

std::string Utils::sortAndConcatWords(std::vector<std::string>  words)
{
	std::sort(words.begin(), words.end());
	return std::accumulate(words.begin(), words.end(), std::string(""));
}
bool isStopChar(wchar_t c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}
void Utils::clearString(std::string& s)
{
	for (auto& i : s)
	{
		if (i == '\t' || i == '\n')
			i = ' ';
	}

	auto isTwoSpaces = [](wchar_t a, wchar_t b) {return a == b && a == ' '; };
	s.erase(std::unique(s.begin(), s.end(), isTwoSpaces), s.end());

	if (!s.empty() && s.front() == ' ')
		s.erase(s.begin(), s.begin() + 1);
	if (!s.empty() && s.back() == ' ')
		s.erase(s.end() - 1, s.end());
}


bool Utils::isLetter(char c)
{
	return std::isalpha(c, std::locale("ru-RU"));
}



void Utils::drawGraphImageFromDot(std::string dot, std::string imagePath)
{
	std::string dotFile = "temp.dot";
	writeToFile(dotFile, dot);
	std::string command = std::string("dot -Tpng temp.dot  -o ") + imagePath;
	system(command.c_str());
	//std::remove(dotFile.c_str());
}
