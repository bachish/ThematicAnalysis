#pragma once
#include <string>

class Utils
{
public:
	static size_t CalculateHashCode(std::wstring const& text);
	static std::wstring readAllFile( std::ifstream const& fin);
private:
	
};
