#include "XmlArticlesReader.h"

#include <algorithm>
#include <locale>

/**
 * \brief Extract from text only titles and content
 * \param text - source text in xml
 * \return tuple vectors of article titles and it's content
 */
std::tuple<std::vector<std::string>, std::vector<std::string>> XmlArticlesReader::read(std::string const& text) const
{
	_text = text;
	_size = _text.size();
	_ptr = 0;
	_titles = std::vector<std::string>();
	_contents = std::vector<std::string>();
	parse();
	return std::make_tuple(_titles, _contents);
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

void XmlArticlesReader::parsePaper() const
{
	std::string title, content;
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto [tag_name, isClosed] = readTag();
			if (tag_name == nameTag)
			{
				title = readToEnd(nameTag);
			}
			else if (tag_name == contentTag || tag_name == definitionTag)
			{
				content += readToEnd(tag_name);
			}
			else if (tag_name == paperTag)
			{
				if (isClosed) break;
				parsePaper();
			}
		}
		else ++_ptr;
	}
	if (!title.empty())
	{
		//	clearString(title);
		//	clearString(content);
		_titles.push_back(title);
		_contents.push_back(content);
	}
}

void XmlArticlesReader::parse() const
{
	int i = 0;

	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto [tag, isClosed] = readTag();
			if (tag == paperTag && !isClosed)
			{
				i++;
				parsePaper();
			}
		}
		else ++_ptr;
	}
	;
}

std::string XmlArticlesReader::readWord() const
{
	skipSpace();
	const auto start = _ptr;
	while (isLetter(_text[_ptr]))
		++_ptr;
	return _text.substr(start, _ptr - start);
}

std::tuple<std::string, bool> XmlArticlesReader::readTag() const
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
	return { tag, isClosed };
}

void XmlArticlesReader::skipSpace() const
{
	while (_ptr < _size && (_text[_ptr] == ' ' || _text[_ptr] == '\t' || _text[_ptr] == '\n'))
		++_ptr;
}




std::string XmlArticlesReader::readToEnd(const std::string& tagName) const
{
	const auto start = _ptr;
	while (_ptr < _size)
	{
		if (_text[_ptr] == '<')
		{
			auto tmp = _ptr;
			auto [tag, isClosed] = readTag();
			if (tag == tagName)
			{
				if (isClosed)
					return _text.substr(start, tmp - start);
			}
		}
		else ++_ptr;
	}
	return _text.substr(start, _ptr - start);
}
