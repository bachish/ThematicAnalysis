#include "Lemmatizer.h"
#include <locale>
#include <filesystem>

#include "Utils/FileUtils.h"
#include "Utils/StringUtils.h"

std::string runMyStem(std::string const& tempFile)
{
	std::string output;
	auto isExecuted = FileUtils::executeExeWithParams("external\\mystem.exe", "-e cp1251 -nl " + tempFile, output);
	if (!isExecuted)
	{
		throw std::runtime_error("Can't run mystem.exe!");
	}
	return output;
}

std::string handleMyStemLine(std::string line)
{
	auto words = StringUtils::split(line, "|");
	auto word = words.front();
	while (word.back() == '?') word.pop_back();
	return word;
}


std::string useMyStem(const std::string& text)
{
	std::filesystem::create_directory("temp");
	std::string tempFile = "temp/temp.txt";
	FileUtils::writeToFile(tempFile, text);
	auto resStr = runMyStem(tempFile);
	std::filesystem::remove(tempFile);
	return resStr;
}


std::vector<std::string> Lemmatizer::lemmatizeText(const std::string& text) const
{
	return lemmatizeTextWithMyStem(text);
}

std::vector<std::string> Lemmatizer::lemmatizeTextWithMyStem(const std::string& text) const
{
	auto resText = useMyStem(text);
	auto lines = StringUtils::split(resText, "\n", true);
	std::transform(lines.begin(), lines.end(), lines.begin(), [](std::string& line) {return handleMyStemLine(line); });
	return lines;
}