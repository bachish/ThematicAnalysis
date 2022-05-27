#include "XmlSourceParser.h"

#include <algorithm>
#include <locale>

/**
 * \brief Extract from text only titles and content
 * \param xmlText - source text in xml
 * \return tuple vectors of article titles and it's content
 */
std::tuple<std::vector<std::string>, std::vector<std::string>> XmlSourceParser::parseTitlesAndContentsFromXml(
	std::string const& xmlText)
{
	_text = xmlText;
	_size = _text.size();
	_ptr = 0;
	_titles = std::vector<std::string>();
	_contents = std::vector<std::string>();
	parse();
	return std::make_tuple(_titles, _contents);
}

std::tuple<std::vector<std::string>, std::vector<std::string>> XmlSourceParser::parseTitlesAndContents(
	std::string const& text, IXmlConverter const& xmlConverter)
{
	auto xmlText = xmlConverter.convertTextToXml(text);
	return parseTitlesAndContentsFromXml(xmlText);
}


void clearString(std::string& s)
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


bool isLetter(char c)
{
	static std::locale loc("ru-RU");
	return std::isalpha(c, loc);
}

void XmlSourceParser::parsePaper()
{
	std::string title, content;
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tag = readTag();
			std::string tag_name = std::get<std::string>(tag);
			if (tag_name == nameTag)
			{
				title = readToEnd(nameTag);
			}
			else if (tag_name == contentTag || tag_name == definitionTag)
			{
				content += " ";
				content += readToEnd(tag_name);
			}
			else if (tag_name == paperTag)
			{
				if (std::get<bool>(tag)) break;
				parsePaper();
			}
		}
		else ++_ptr;
	}
	if (!title.empty())
	{
		clearString(title);
		clearString(content);
		_titles.push_back(title);
		_contents.push_back(content);
	}
}

void XmlSourceParser::parse()
{
	int i = 0;

	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tag = readTag();
			if (std::get<std::string>(tag) == paperTag &&
				!std::get<bool>(tag))
			{
				i++;
				parsePaper();
			}
		}
		else ++_ptr;
	}
	;
}

std::string XmlSourceParser::readWord()
{
	skipSpace();
	const auto start = _ptr;
	while (isLetter(_text[_ptr]))
		++_ptr;
	return _text.substr(start, _ptr - start);
}

std::tuple<std::string, bool> XmlSourceParser::readTag()
{
	bool isClosed = false;
	while (_ptr < _size && _text[_ptr++] != '<') {}
	if (_ptr < _size && _text[_ptr] == '/')
	{
		++_ptr;
		isClosed = true;
	}
	auto tag = readWord();
	while (_ptr < _size && _text[_ptr++] != '>') {}
	return std::make_tuple(tag, isClosed);
}

void XmlSourceParser::skipSpace()
{
	while (_ptr < _size && (_text[_ptr] == ' ' || _text[_ptr] == '\t' || _text[_ptr] == '\n'))
		++_ptr;
}



std::string XmlSourceParser::readToEnd(const std::string& tagName)
{
	const auto start = _ptr;
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tmp = _ptr;
			auto tag = readTag();
			if (std::get<std::string>(tag) == tagName)
			{
				if (std::get<bool>(tag))
					return _text.substr(start, tmp - start);
			}
		}
		else ++_ptr;
	}
	return _text.substr(start, _ptr - start);
}
