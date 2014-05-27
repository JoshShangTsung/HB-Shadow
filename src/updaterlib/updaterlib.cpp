#include "updaterlib.h"
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <windows.h>

std::string readFile(const char *fileName) {
	std::ifstream in(fileName, std::ios::in | std::ios::binary);
	std::stringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}

namespace {

	std::pair<std::vector<std::string>, std::vector<std::string>> getFiles(const char *path) {
		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(toStr(path, "/*").c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE) {
			throw std::runtime_error(toStr("Error retrieving files at ", path));
		}
		std::vector<std::string> files;
		std::vector<std::string> folders;
		do {
			if (strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0) {
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					folders.push_back(ffd.cFileName);
				} else {
					files.push_back(ffd.cFileName);
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);
		auto e = GetLastError();
		if (e != ERROR_NO_MORE_FILES) {
			FindClose(hFind);
			throw std::runtime_error(toStr("Error retrieving files at ", path, ": ", e));
		}
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
		return std::make_pair(std::move(files), std::move(folders));
	}

	void addFileData(std::vector<FileData> &ret, const std::string &realPath, const std::string &path) {
		std::vector<std::string> files;
		std::vector<std::string> folders;
		std::tie(files, folders) = getFiles(realPath.c_str());
		for (const std::string &f : files) {
			std::string fileName = toStr(realPath, "/", f);
			std::string fileData = readFile(fileName.c_str());
			FileData fd;
			fd.name_ = path.empty() ? f : toStr(path, "/", f);
			fd.size_ = fileData.length();
			fd.checksum_ = calculateChecksum(fileData);
			fd.timestamp_ = getModificationTime(fileName.c_str());
			ret.push_back(fd);
		}
		for (const std::string &f : folders) {
			std::string newRealPath = toStr(realPath, "/", f);
			std::string newPath = path.empty() ? f : toStr(path, "/", f);
			addFileData(ret, newRealPath, newPath);
		}
	}
	
	void addLightFileData(std::vector<LightFileData> &ret, const std::string &realPath, const std::string &path) {
		std::vector<std::string> files;
		std::vector<std::string> folders;
		std::tie(files, folders) = getFiles(realPath.c_str());
		for (const std::string &f : files) {
			std::string fileName = toStr(realPath, "/", f);
			//std::string fileData = readFile(fileName.c_str());
			LightFileData fd;
			fd.name_ = path.empty() ? f : toStr(path, "/", f);
			//fd.size_ = fileData.length();
			//fd.checksum_ = std::accumulate(fileData.begin(), fileData.end(), (uint32_t) 0);
			fd.timestamp_ = getModificationTime(fileName.c_str());
			ret.push_back(fd);
		}
		for (const std::string &f : folders) {
			std::string newRealPath = toStr(realPath, "/", f);
			std::string newPath = path.empty() ? f : toStr(path, "/", f);
			addLightFileData(ret, newRealPath, newPath);
		}
	}
}

Timestamp getModificationTime(const char *path) {
		HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			throw std::runtime_error(toStr("Couldn't read file ", path, " for ts"));
		}
		FILETIME ftCreate, ftAccess, ftWrite;
		// Retrieve the file times for the file.
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
			CloseHandle(hFile);
			throw std::runtime_error(toStr("Couldn't get file ", path, " time"));
		}
		CloseHandle(hFile);
		// Convert the last-write time to local time.
		SYSTEMTIME stUTC;
		FileTimeToSystemTime(&ftWrite, &stUTC);
		
		SYSTEMTIME stLocal;
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		Timestamp ret;
		ret.year_ = stLocal.wYear;
		ret.month_ = stLocal.wMonth;
		ret.day_ = stLocal.wDay;
		ret.hour_ = stLocal.wHour;
		ret.minute_ = stLocal.wMinute;
		ret.second_ = stLocal.wSecond;
		ret.millisecond_ = stLocal.wMilliseconds;
		return ret;
	}
uint32_t calculateChecksum(const std::string &data) {
	return std::accumulate(data.begin(), data.end(), (uint32_t) 0);;
}
std::vector<FileData> getFileData(const char *path) {
	std::vector<FileData> ret;
	addFileData(ret, path, "");
	return ret;
}


std::vector<LightFileData> getLightFileData(const char *path) {
	std::vector<LightFileData> ret;
	addLightFileData(ret, path, "");
	return ret;
}