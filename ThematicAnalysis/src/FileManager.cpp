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

std::string FileManager::readAllFile(std::string const& filePath)
{
	return readAllFile(std::ifstream(filePath));
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

void FileManager::writeToFile(std::string const& filePath, std::string const& text)
{
	std::ofstream fout(filePath);
	fout.imbue(std::locale(""));
	fout << text;
	fout.close();
}

bool FileManager::executeExeWithParams(std::string exe, std::string params, std::string workingDir)
{
	DWORD exitCode = 0;
	SHELLEXECUTEINFOA ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = exe.c_str();
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = workingDir.c_str();
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteExA(&ShExecInfo);

	auto res = WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	if (res == 0)
		GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);
	return res == 0 && exitCode == 0;

}
