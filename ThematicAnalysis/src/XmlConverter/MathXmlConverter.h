#pragma once
#include "IXmlConverter.h"

class MathXmlConverter: public IXmlConverter
{
public:
	std::string convertTextToXml(std::string const& text) const override;
};
