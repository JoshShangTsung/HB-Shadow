#pragma once
#include <vector>
#include <string>
#include <sstream>

struct Timestamp {
	int16_t year_;
	int8_t month_;
	int8_t day_;
	int8_t hour_;
	int8_t minute_;
	int8_t second_;
	int16_t millisecond_;
};

static bool operator==(const Timestamp &a, const Timestamp &b) {
	return a.millisecond_ == b.millisecond_ &&
			  a.second_ == b.second_ &&
			  a.minute_ == b.minute_ &&
			  a.hour_ == b.hour_ &&
			  a.day_ == b.day_ &&
			  a.month_ == b.month_ &&
			  a.year_ == b.year_;
}

static bool operator!=(const Timestamp &a, const Timestamp &b) {
	return !(a==b);
}

struct FileData {
	std::string name_;
	uint32_t size_;
	uint32_t checksum_;
	Timestamp timestamp_;
};

struct LightFileData {
	std::string name_;
	Timestamp timestamp_;
};

namespace I {

	static void toStr(std::ostream &) {
	}

	template<typename T, typename... Args> void toStr(std::ostream &os, T &&t, Args&&... args) {
		os << std::forward<T>(t);
		I::toStr(os, std::forward<Args>(args)...);
	}
}

template<typename... Args> std::string toStr(Args&&... args) {
	std::stringstream ss;
	I::toStr(ss, std::forward<Args>(args)...);
	return ss.str();
}

uint32_t calculateChecksum(const std::string &data);
std::string readFile(const char *fileName);
Timestamp getModificationTime(const char *path);
std::vector<FileData> getFileData(const char *path);
std::vector<LightFileData> getLightFileData(const char *path);