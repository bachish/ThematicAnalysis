#pragma once
#include "IXmlConverter.h"

class MathXmlConverter: public IXmlConverter
{
public:
	std::string convertFileToXml(std::string const& filePath) const override;
};
