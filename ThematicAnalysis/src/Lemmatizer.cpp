#include "Lemmatizer.h"
#include <locale>
#include <filesystem>
#include <iostream>
#include <Windows.h>

#include "FileManager.h"
#include "StringUtils.h"

void runMyStem(std::string const& tempFile, std::string const& resFile)
{
	auto isExecuted = FileManager::executeExeWithParams("external\\mystem.exe", "-e cp1251 -nl " + tempFile + " " + resFile);
	if (!isExecuted)
	{
		throw std::runtime_error("Can't run mystem.exe!");
	}
}

std::string handleLine(std::string line)
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
	std::string resFile = "temp/res.txt";

	FileManager::writeToFile(tempFile, text);
	runMyStem(tempFile, resFile);
	auto resStr = FileManager::readAllFile(resFile);
	std::filesystem::remove(tempFile);
	std::filesystem::remove(resFile);
	return resStr;
}


std::vector<std::string> Lemmatizer::lemmatizeText(const std::string& text) const
{
	auto resText = useMyStem(text);
	auto lines = StringUtils::split(resText, "\n", true);
	std::transform(lines.begin(), lines.end(), lines.begin(), [](std::string& line) {return handleLine(line); });
	return lines;
}


