#pragma once
#include <string>
#include <vector>
#include <tuple>

#include "IArticlesReader.h"


class XmlArticlesReader: public IArticlesReader
{
public:
	std::tuple<std::vector<std::string>, std::vector<std::string>> read(std::string const & text) const override;
private:
	mutable std::vector<std::string> _titles;
	mutable std::vector<std::string> _contents;
	mutable std::string _text;
	mutable size_t _ptr = 0, _size = 0;
	const std::string nameTag = "name";
	const std::string contentTag = "content";
	const std::string paperTag = "paper";
	const std::string definitionTag = "paperDef";
	std::string readToEnd(const std::string& tagName) const;
	void parsePaper() const;
	void parse() const;
	std::string readWord() const;
	std::tuple<std::string, bool> readTag() const;
	void skipSpace() const;
	
};



