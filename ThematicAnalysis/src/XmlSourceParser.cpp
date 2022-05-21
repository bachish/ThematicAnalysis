#include "XmlSourceParser.h"

#include "Utils.h"

/**
 * \brief Extract from text only titles and content
 * \param documentText - source text in xml
 * \return tuple vectors of article titles and it's content
 */
std::tuple<std::vector<std::string>, std::vector<std::string>> XmlSourceParser::parseTitlesAndContents(
	std::string const& documentText)
{
	_text = documentText;
	_size = _text.size();
	_ptr = 0;
	_titles = std::vector<std::string>();
	_contents = std::vector<std::string>();
	parse();
	return std::make_tuple(_titles, _contents);
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
		Utils::clearString(title);
		Utils::clearString(content);
		_titles.push_back(title);
		_contents.push_back(content);
	}
}

void XmlSourceParser::parse()
{
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tag = readTag();
			if (std::get<std::string>(tag) == paperTag &&
				!std::get<bool>(tag))
			{
				parsePaper();
			}
		}
		else ++_ptr;
	}
}

std::string XmlSourceParser::readWord()
{
	skipSpace();
	const auto start = _ptr;
	while (Utils::isLetter(_text[_ptr]))
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
