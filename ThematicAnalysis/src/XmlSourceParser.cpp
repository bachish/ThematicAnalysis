#include "XmlSourceParser.h"

/**
 * \brief Extract from text only titles and content
 * \param documentText - source text in xml
 * \return tuple vectors of article titles and it's content
 */
std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> XmlSourceParser::parseTitlesAndContents(
	std::wstring const& documentText)
{
	_text = documentText;
	_size = _text.size();
	_ptr = 0;
	_titles = std::vector<std::wstring>();
	_contents = std::vector<std::wstring>();
	parse();
	return std::make_tuple(_titles, _contents);
}

void XmlSourceParser::parsePaper()
{
	std::wstring title, content;
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tag = readTag();
			std::wstring tag_name = std::get<std::wstring>(tag);
			if (tag_name == nameTag)
			{
				title = readToEnd(nameTag);
			}
			else if (tag_name == contentTag || tag_name == definitionTag)
			{
				content += L" ";
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
			if (std::get<std::wstring>(tag) == paperTag &&
				!std::get<bool>(tag))
			{
				parsePaper();
			}
		}
		else ++_ptr;
	}
}

std::wstring XmlSourceParser::readWord()
{
	skipSpace();
	const auto start = _ptr;
	while (isLetter(_text[_ptr]))
		++_ptr;
	return _text.substr(start, _ptr - start);
}

std::tuple<std::wstring, bool> XmlSourceParser::readTag()
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
	while (_ptr < _size && _ptr == ' ')
		++_ptr;
}

bool XmlSourceParser::isLetter(wchar_t c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

std::wstring XmlSourceParser::readToEnd(const std::wstring& tagName)
{
	const auto start = _ptr;
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tmp = _ptr;
			auto tag = readTag();
			if (std::get<std::wstring>(tag) == tagName)
			{
				if (std::get<bool>(tag))
					return _text.substr(start, tmp - start);
			}
		}
		else ++_ptr;
	}
	return _text.substr(start, _ptr - start);
}
