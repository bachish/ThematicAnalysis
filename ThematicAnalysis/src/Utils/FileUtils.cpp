#include "FileUtils.h"
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <array>

std::string FileUtils::readAllFile(const std::ifstream& fin)
{
	std::stringstream wss;
	wss << fin.rdbuf();
	return wss.str();
}

std::string FileUtils::readAllFile(std::string const& filePath)
{
	std::ifstream fin(filePath);
	auto text = readAllFile(fin);
	fin.close();
	return text;
}

std::string convertEncodings(std::string const& str, UINT encodingFrom, UINT encodingTo)
{
	auto wsize = MultiByteToWideChar(encodingFrom, 0, str.c_str(), -1, nullptr, 0);
	auto wres = new WCHAR[wsize];
	MultiByteToWideChar(encodingFrom, 0, str.c_str(), -1, wres, wsize);

	auto usize = WideCharToMultiByte(encodingTo, 0, wres, -1, nullptr, 0, nullptr, nullptr);
	auto ures = new char[usize];
	WideCharToMultiByte(encodingTo, 0, wres, -1, ures, usize, nullptr, nullptr);
	std::string res = ures;

	delete[] ures;
	delete[] wres;

	return res;
}

std::string convertUTF8ToCp1251(const std::string& str)
{
	return convertEncodings(str, CP_UTF8, 1251);
}

std::string convertCp1251ToUTF8(const std::string& str)
{
	return convertEncodings(str, 1251, CP_UTF8);
}

std::string FileUtils::readAllUTF8File(std::string const& filePath)
{
	auto text = readAllFile(filePath);
	text = convertUTF8ToCp1251(text);
	return text;
}

void FileUtils::writeUTF8ToFile(std::string const& filePath, std::string const& text)
{
	std::ofstream fout(filePath);
	fout.imbue(std::locale(""));
	auto wtext = convertCp1251ToUTF8(text);
	fout << wtext;
	fout.close();
}

void FileUtils::writeToFile(std::string const& filePath, std::string const& text)
{
	std::ofstream fout(filePath);
	fout.imbue(std::locale(""));
	fout << text;
	fout.close();
}

bool FileUtils::executeExeWithParams(const std::string& exePath, const std::string& params, std::string& output)
{
	if (!std::filesystem::exists(exePath)) return false;
	auto cmd = exePath + " " + params;
	std::array<char, 128> buffer{};
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
	if (pipe == nullptr) {
		return false;
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		output += buffer.data();
	}

	return true;
}

bool FileUtils::executeExeWithParams(const std::string& exePath, const std::string& params)
{
	std::string _;
	return executeExeWithParams(exePath, params, _);
}