#pragma once
#include <string>

class IXmlConverter
{
public:
	virtual std::string convertTextToXml(std::string const& text) const = 0;
	virtual ~IXmlConverter() = default;
};
