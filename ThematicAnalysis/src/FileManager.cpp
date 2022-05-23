#include "FileManager.h"

#include <Windows.h>
#include <sstream>
#include <fstream>


std::string FileManager::readAllFile(const std::ifstream& fin)
{
	std::stringstream wss;
	wss << fin.rdbuf();
	return wss.str();
}

std::string cp1251ToUTF8(const std::string& str)
{
	auto wsize = MultiByteToWideChar(1251, 0, str.c_str(), -1, nullptr, 0);
	auto wres = new WCHAR[wsize];
	MultiByteToWideChar(1251, 0, str.c_str(), -1, wres, wsize);

	auto usize = WideCharToMultiByte(CP_UTF8, 0, wres, -1, nullptr, 0, nullptr, nullptr);
	auto ures = new char[usize];
	WideCharToMultiByte(CP_UTF8, 0, wres, -1, ures, usize, nullptr, nullptr);
	std::string res = ures;

	delete[] ures;
	delete[] wres;

	return res;
}

void FileManager::writeUTF8ToFile(std::string const& filePath, std::string const& text)
{
	std::ofstream fout(filePath);
	fout.imbue(std::locale(""));
	auto wtext = cp1251ToUTF8(text);
	fout << wtext;
	fout.close();
}