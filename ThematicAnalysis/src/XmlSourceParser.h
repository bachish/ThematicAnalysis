#pragma once
#include <string>
#include <vector>
#include <tuple>

class XmlSourceParser
{
public:
	XmlSourceParser();
	std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> parseTitlesAndContents(std::wstring const & documentText);
private:
	std::vector<std::wstring> _titles;
	std::vector<std::wstring> _contents;
	std::wstring & _text;
	size_t _ptr = 0, _size;
	const std::wstring nameTag = L"name";
	const std::wstring contentTag = L"content";
	const std::wstring paperTag = L"paper";
	const std::wstring definitionTag = L"paperDef";
	std::wstring readToEnd(const std::wstring& tagName);
	void parsePaper();
	void parse();
	std::wstring readWord();
	std::tuple<std::wstring, bool> readTag();
	static bool isLetter(wchar_t c);
	void skipSpace();
	
};



