#pragma once
#include <string>

class IXmlConverter
{
public:
	virtual std::string convertFileToXml(std::string const& filePath) const = 0;
	virtual ~IXmlConverter() = default;
};
