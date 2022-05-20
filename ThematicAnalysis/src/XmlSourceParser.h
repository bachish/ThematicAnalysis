#pragma once
#include <string>
#include <vector>

class XmlSourceParser
{
public:
	std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> parseTitlesAndContents(std::wstring const & documentText);
};
