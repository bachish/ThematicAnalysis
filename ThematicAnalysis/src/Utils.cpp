#include "Utils.h"
#include <sstream>
#include <fstream>

size_t Utils::CalculateHashCode(std::wstring const& text)
{
	static std::hash<std::wstring> _hashCalculator;
	return _hashCalculator(text);
}


std::wstring Utils::readAllFile( std::ifstream const& fin)
{
	std::wstringstream wss;
	wss << fin.rdbuf();
	return wss.str();
}