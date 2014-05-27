#include "../updaterlib/updaterlib.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <algorithm>
struct BinaryOstream {

	void add(const void *ptr, std::size_t sz) {
		str_.append(reinterpret_cast<const char*> (ptr), sz);
	}

	std::string str() {
		return str_;
	}

	void reserve(std::size_t sz) {
		str_.reserve(sz);
	}
private:
	std::string str_;
};

template<typename T> typename std::enable_if<std::is_integral<T>::value, BinaryOstream&>::type operator<<(BinaryOstream &os, T v) {
	os.add(&v, sizeof (T));
	return os;
}

BinaryOstream& operator<<(BinaryOstream &os, const std::string &str) {
	os << str.size();
	os.add(str.data(), str.size());
	return os;
}

struct BinaryIstream {

	BinaryIstream(const std::string &str) : str_(str), size_(str_.size()) {
	}

	void get(void *ptr, std::size_t sz) {
		if (remaining() < sz) {
			throw std::runtime_error("Not enough data");
		}
		std::memcpy(ptr, str_.data() + index_, sz);
		index_ += sz;
	}

	std::size_t remaining() const {
		return size_ - index_;
	}
private:
	std::size_t index_ = 0;
	const std::string str_;
	const std::size_t size_;
};

template<typename T> typename std::enable_if<std::is_integral<T>::value, BinaryIstream&>::type operator>>(BinaryIstream &is, T &v) {
	is.get(&v, sizeof (T));
	return is;
}

BinaryIstream& operator>>(BinaryIstream &is, std::string &str) {
	decltype(str.size()) sz;
	is >> sz;
	str.assign(sz, 0);
	is.get(&str[0], str.size());
	return is;
}

void saveCache(const std::vector<FileData> &fds) {
	BinaryOstream os;
	os << fds.size();
	for (const FileData &fd : fds) {
		os << fd.name_ << fd.size_ << fd.checksum_ <<
				  fd.timestamp_.year_ << fd.timestamp_.month_ << fd.timestamp_.day_ <<
				  fd.timestamp_.hour_ << fd.timestamp_.minute_ << fd.timestamp_.second_ << fd.timestamp_.millisecond_;
	}
	std::ofstream ofs("cache.dat", std::ios::out | std::ios::binary);
	ofs << os.str();
}

void loadCache(std::vector<FileData> &fds) {
	BinaryIstream is(readFile("cache.dat"));
	decltype(fds.size()) sz = 0;
	is >> sz;
	fds.reserve(sz);
	for (std::size_t i = 0; i < sz; ++i) {
		FileData fd;
		is >> fd.name_ >> fd.size_ >> fd.checksum_ >>
				  fd.timestamp_.year_ >> fd.timestamp_.month_ >> fd.timestamp_.day_ >>
				  fd.timestamp_.hour_ >> fd.timestamp_.minute_ >> fd.timestamp_.second_ >> fd.timestamp_.millisecond_;
		fds.push_back(fd);
	}
}

void print(const std::vector<FileData> &fds) {
	for (const FileData &fd : fds) {
		std::cout << fd.name_ << " " << fd.size_ << " " << fd.checksum_ << " " <<
				  (int) fd.timestamp_.year_ << "/" << (int) fd.timestamp_.month_ << "/" << (int) fd.timestamp_.day_ << " " <<
				  (int) fd.timestamp_.hour_ << ":" << (int) fd.timestamp_.minute_ << ":" << (int) fd.timestamp_.second_ << "." << fd.timestamp_.millisecond_ << std::endl;
	}
}

int main() {
	try {
#if 0
		std::vector<FileData> fds = getFileData(".");
		saveCache(fds);
#endif
#if 1
		std::vector<FileData> fds;
		try {
			loadCache(fds);
			std::vector<LightFileData> lfds = getLightFileData(".");
			bool updated = false;
			for(const LightFileData &lfd: lfds) {
				if(lfd.name_ == "cache.dat") continue;
				FileData *fdPtr = nullptr;
				for(FileData &fd : fds) {
					if(fd.name_ == lfd.name_) {
						fdPtr = &fd;
					}
				}
				if(fdPtr) {
					if(fdPtr->timestamp_ != lfd.timestamp_) {
						// Modified
						fdPtr->timestamp_ = lfd.timestamp_;
						std::string data = readFile(toStr("./", lfd.name_).c_str());
						fdPtr->size_ = data.size();
						fdPtr->checksum_ = calculateChecksum(data);
						updated = true;
					}
				} else {
					// New file
					FileData fd;						
					fd.name_ = lfd.name_;
					fd.timestamp_ = lfd.timestamp_;
					std::string data = readFile(toStr("./", lfd.name_).c_str());
					fd.size_ = data.size();
					fd.checksum_ = calculateChecksum(data);
					fds.push_back(fd);
					updated = true;
				}
			}
			std::vector<std::string> toRemove;
			for(FileData &fd: fds) {
				if(fd.name_ == "cache.dat") continue;
				bool found = false;
				for(const LightFileData &lfd: lfds) {
					if(lfd.name_ == fd.name_) {
						found = true;
					}
				}
				if(!found) {
					updated = true;
					std::cout << fd.name_ << " Removed" << std::endl;
					toRemove.push_back(fd.name_);
				}
			}
			auto iter = std::remove_if(fds.begin(), fds.end(), [&toRemove](const FileData &fd){return std::find(toRemove.begin(), toRemove.end(), fd.name_) != toRemove.end();});
			fds.erase(iter, fds.end());
			if(updated) {
				std::cout << "Refreshing cache" << std::endl;
				saveCache(fds);
			}
		}catch(std::exception &) {
			std::cout << "Caching data..." << std::endl;
			fds = getFileData(".");
			saveCache(fds);
		}
		//print(fds);
#endif
	} catch (std::exception &e) {
		std::cout << "Exception " << e.what() << std::endl;
	}
	return 0;
}

