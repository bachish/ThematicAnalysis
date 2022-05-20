#include "Utils.h"

size_t Utils::CalculateHashCode(std::wstring const& text)
{
	static std::hash<std::wstring> _hashCalculator;
	return _hashCalculator(text);
}
