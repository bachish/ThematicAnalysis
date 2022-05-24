#include "Lemmatizer.h"
#include <locale>
#include <filesystem>
#include <iostream>
#include <Windows.h>

#include "FileManager.h"
#include "StringUtils.h"

const std::string Lemmatizer::RUS_DICTIONARY_DEFAULT_PATH = "external/lemmatizator/x64/lemmatizer.db";
constexpr size_t Lemmatizer::CHARS_THRESHOLD = 3000L;

std::string runMyStem(std::string const& tempFile)
{
	std::string output;
	auto isExecuted = FileManager::executeExeWithParams("external\\mystem.exe", "-e cp1251 -nl " + tempFile, output);
	if (!isExecuted)
	{
		throw std::runtime_error("Can't run mystem.exe!");
	}
	return output;
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

	FileManager::writeToFile(tempFile, text);

	auto resStr = runMyStem(tempFile);
	std::filesystem::remove(tempFile);
	return resStr;
}


std::vector<std::string> Lemmatizer::lemmatizeText(const std::string& text) const
{
	if (text.size() > CHARS_THRESHOLD)
		return lemmatizeTextWithMyStem(text);
	else
		return lemmatizeTextWithSol(text);
}

std::vector<std::string> Lemmatizer::lemmatizeTextWithMyStem(const std::string& text) const
{
	auto resText = useMyStem(text);
	auto lines = StringUtils::split(resText, "\n", true);
	std::transform(lines.begin(), lines.end(), lines.begin(), [](std::string& line) {return handleLine(line); });
	return lines;
}



Lemmatizer::Lemmatizer() :Lemmatizer(RUS_DICTIONARY_DEFAULT_PATH)
{
}

Lemmatizer::Lemmatizer(const std::string& pathToDictionary)
{
	_hEngine = sol_LoadLemmatizatorA(pathToDictionary.c_str(), LEME_DEFAULT);
	if (_hEngine == nullptr)
		throw std::ios_base::failure("Can not open dictionary");
}

std::vector<std::string> Lemmatizer::lemmatizeTextWithSol(const std::string& text) const
{
	std::vector<std::string> words;
	int bufSize = static_cast<int>((text.length() + 100) * 1.5);
	auto strBuf = std::unique_ptr<char[]>(new char[bufSize]);
	auto hWords = sol_LemmatizePhraseA(_hEngine, text.c_str(), 0, L' ');
	if (hWords != nullptr)
	{
		auto wordsCount = sol_CountLemmas(hWords);

		for (int i = 0; i < wordsCount; ++i)
		{
			sol_GetLemmaStringA(hWords, i, strBuf.get(), bufSize);
			words.emplace_back(strBuf.get());
			auto& word = words.back();

			// word to lower
			std::transform(word.begin(), word.end(), word.begin(), [](wchar_t ch) {return std::tolower(ch); });
		}

		sol_DeleteLemmas(hWords);
	}
	return words;
}

std::string Lemmatizer::lemmatizeWord(const std::string& word) const
{

	int bufSize = static_cast<int>((word.length() + 100) * 1.5);
	auto strBuf = std::unique_ptr<char[]>(new char[bufSize]);
	sol_GetLemmaA(_hEngine, word.c_str(), strBuf.get(), bufSize);
	std::string normWord = strBuf.get();
	return normWord;
}

Lemmatizer::~Lemmatizer()
{
	sol_DeleteLemmatizator(_hEngine);
}

