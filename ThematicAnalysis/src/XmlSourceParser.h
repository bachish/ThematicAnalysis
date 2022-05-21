#pragma once
#include <string>
#include <vector>
#include <tuple>

class XmlSourceParser
{
public:
	std::tuple<std::vector<std::string>, std::vector<std::string>> parseTitlesAndContents(std::string const & documentText);
private:
	std::vector<std::string> _titles;
	std::vector<std::string> _contents;
	std::string _text;
	size_t _ptr = 0, _size = 0;
	const std::string nameTag = "name";
	const std::string contentTag = "content";
	const std::string paperTag = "paper";
	const std::string definitionTag = "paperDef";
	std::string readToEnd(const std::string& tagName);
	void parsePaper();
	void parse();
	std::string readWord();
	std::tuple<std::string, bool> readTag();
	void skipSpace();
	
};



