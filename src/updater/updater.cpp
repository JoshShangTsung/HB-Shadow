#include "../updaterlib/updaterlib.h"
#include <iostream>
#include <deque>
#include <fstream>
#include <thread>

struct Cfg {
	std::string updaterName_ = "updater.exe";
	std::string oldUpdaterName_ = toStr(updaterName_, ".old");
	std::string path_ = ".";
	std::string cacheFile_ = "cache.dat";
};

struct Cache {

	Cache(Cfg &cfg) : cfg_(cfg) {
	}
	void load() {
		fds_ = getFileData(cfg_.path_, cfg_.cacheFile_);
	}
	FileDatas fds_;

	void commitCache() {
		if (updated_) {
			saveCache(fds_, cfg_.cacheFile_);
		}
	}
	bool updated_ = false;
private:
	Cfg &cfg_;
};

struct Client {

	Client(Cfg &cfg, Cache &fds, Net::ClientPtr &&ptr) : client_(std::move(ptr)), cfg_(cfg), fds_(fds) {
	}

	void update() {
		Net::Events events = client_->getEvents();
		for (const Net::Event &ev : events) {
			switch (ev.type_) {
				case Net::EventType::CONNECTED:
					onConnected();
					break;
				case Net::EventType::DISCONNECTED:
					onDisconnected();
					break;
				case Net::EventType::MSG:
					onMsg(ev.msg_);
					break;
			}
		}
	}
	
	bool isRunning() const {
		return running_;
	}
private:
	bool running_ = true;
	Net::ClientPtr client_;
	Cfg &cfg_;
	Cache &fds_;
	FileDatas serverFds_;
	std::deque<std::string> toUpdate_;

	struct CurrentFile {

		void reset(const FileDatas &fds, const std::string &path, const std::string &fileName) {
			std::string fullPath = toStr(path, "/", fileName);
			std::string fp = filePath(fullPath);
			makeFullPath(fp);
			name_ = fileName;
			path_ = fp;
			fileName_ = fullPath;
			fd_ = fds.at(fileName);
			partialSize_ = 0;
			partialChecksum_ = 0;
			tmpName_ = fileName + ".tmp";
			tmpFile_.close();
			tmpFile_.clear();
			tmpFile_.open(toStr(path, "/", tmpName_), std::ios::out | std::ios::binary);
		}
		std::string name_;
		std::string path_;
		std::string fileName_;
		FileData fd_;
		uint32_t partialSize_;
		uint32_t partialChecksum_;
		std::string tmpName_;
		std::ofstream tmpFile_;
	};
	CurrentFile currentFile_;

	void onConnected() {
		std::cout << "Connected!" << std::endl;
		BinaryOstream os;
		os << OpcodeClientToSrv::REQUEST_FILEDATA;
		client_->send(os.str());
	}

	void onDisconnected() {
		std::cout << "Disconnected!" << std::endl;
		running_ = false;
	}

	void onMsg(const std::string& msg) {
		BinaryIstream is(msg);
		OpcodeSrvToClient op;
		is >> op;
		switch (op) {
			case OpcodeSrvToClient::RESPONSE_FILEDATA:
				onResponseFileData(is);
				break;
			case OpcodeSrvToClient::RESPONSE_FILE:
				onResponseFile(is);
				break;
			case OpcodeSrvToClient::RESPONSE_FILE_FINISHED:
				onResponseFileFinished();
				break;
		}
	}

	void onResponseFileData(BinaryIstream &is) {
		std::string data;
		is >> data;
		serverFds_ = fromStr(data);
		for (const auto &p : serverFds_) {
			const std::string &name = p.first;
			const FileData &fd = p.second;
			auto iter = fds_.fds_.find(name);
			if (iter == fds_.fds_.end()) {
				toUpdate_.push_back(name);
			} else {
				const FileData &lfd = iter->second;
				if (fd.size_ != lfd.size_ || fd.checksum_ != lfd.checksum_) {
					toUpdate_.push_back(name);
				}
			}
		}
		if (std::find(toUpdate_.begin(), toUpdate_.end(), cfg_.updaterName_) != toUpdate_.end()) {
			requestFile(cfg_.updaterName_);
		} else {
			requestNextFile();
		}
	}

	void requestNextFile() {
		if (toUpdate_.empty()) {
			client_->close();
			return;
		}
		std::string fileName = toUpdate_.front();
		toUpdate_.pop_front();
		requestFile(fileName);
	}

	void requestFile(const std::string &fileName) {
		std::cout << "Requesting file " << fileName << ": ";
		std::cout.flush();

		currentFile_.reset(serverFds_, cfg_.path_, fileName);

		BinaryOstream os;
		os << OpcodeClientToSrv::REQUEST_FILE;
		os << fileName;
		client_->send(os.str());
	}

	void onResponseFile(BinaryIstream &is) {
		std::string data;
		is >> data;
		currentFile_.partialSize_ += data.size();
		currentFile_.partialChecksum_ = std::accumulate(data.begin(), data.end(), currentFile_.partialChecksum_);
		currentFile_.tmpFile_.write(data.data(), data.size());
		if (currentFile_.fd_.size_ > 0) {
			auto sz = currentFile_.partialSize_;
			auto totalsz = currentFile_.fd_.size_;
			std::size_t lastProgress = ((sz - data.size())*100) / totalsz;
			std::size_t currentProgress = (sz * 100) / totalsz;
			const int d = currentProgress - lastProgress;
			if (d) {
				for (int i = 0; i < d; ++i) {
					std::cout << "*";
				}
				std::cout.flush();
			}
		}
	}

	void onResponseFileFinished() {
		currentFile_.tmpFile_.close();
		if (currentFile_.name_ == cfg_.updaterName_) {
			onUpdaterFileFinished();
		} else {
			onCommonFileFinished();
		}
	}

	void onUpdaterFileFinished() {
		validateFile();
		std::cout << " Got new updater!" << std::endl;
		std::rename(cfg_.updaterName_.c_str(), cfg_.oldUpdaterName_.c_str());
		std::rename(currentFile_.tmpName_.c_str(), currentFile_.fileName_.c_str());
		//updateFds();
		//commitCache();
		launchProcess(currentFile_.fileName_);
		closeThisProcess();
	}

	void onCommonFileFinished() {
		validateFile();
		std::cout << " Got file " << currentFile_.name_ << std::endl;
		std::remove(currentFile_.fileName_.c_str()); // Hope that this happens atomically.
		std::rename(currentFile_.tmpName_.c_str(), currentFile_.fileName_.c_str());
		updateFds();
		requestNextFile();
	}

	void updateFds() {
		FileData fd;
		fd.size_ = currentFile_.partialSize_;
		fd.checksum_ = currentFile_.partialChecksum_;
		fd.timestamp_ = getModificationTime(currentFile_.fileName_);
		fds_.fds_[currentFile_.name_] = fd;
		fds_.updated_ = true;
	}

	void validateFile() {
		if (currentFile_.partialSize_ != currentFile_.fd_.size_) {
			std::remove(currentFile_.tmpName_.c_str());
			throw std::runtime_error(toStr("Retrieved file ", currentFile_.name_, " size mismatch! (Got ", currentFile_.partialSize_, ", expected ", currentFile_.fd_.size_, ")"));
		}
		if (currentFile_.partialChecksum_ != currentFile_.fd_.checksum_) {
			std::remove(currentFile_.tmpName_.c_str());
			throw std::runtime_error(toStr("Retrieved file ", currentFile_.name_, " checksum mismatch! (Got ", currentFile_.partialChecksum_, ", expected ", currentFile_.fd_.checksum_, ")"));
		}
	}
};

struct App {

	App() : fds_(cfg_) {
		std::remove(cfg_.oldUpdaterName_.c_str());
		fds_.load();
		client_ = std::make_unique<Client>(cfg_, fds_, Net::createPacketedClient(io_service_, "localhost", "1492"));
		while (client_->isRunning()) {
			if(io_service_.poll()) {
				client_->update();
			} else {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		fds_.commitCache();
		std::cout << "Everything is up to date" << std::endl;
		std::system("pause");
	}
private:
	Cfg cfg_;
	Cache fds_;
	asio::io_service io_service_;
	std::unique_ptr<Client> client_;
};

int main() {
	try {
		App app;
	} catch (std::exception &e) {
		std::cout << "Exception " << e.what() << std::endl;
	} catch (...) {
		std::cout << "Exception!" << std::endl;
	}
	return 0;
}
