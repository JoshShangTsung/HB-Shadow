#include "../updaterlib/updaterlib.h"
#include <iostream>
#include <deque>

struct App {

	App() {
		path_ = ".";
		fds_ = getFileData(path_, "cache.dat");
		client_ = Net::createPacketedClient(io_service_, "localhost", "1492");
		while (io_service_.run_one()) {
			update();
		}
		if (updated_) {
			saveCache(fds_, "cache.dat");
		}
	}
private:
	std::string path_;
	FileDatas fds_;
	asio::io_service io_service_;
	Net::ClientPtr client_;
	FileDatas serverFds_;
	std::deque<std::string> toUpdate_;
	std::string currentFile_;
	FileData currentFileDesc_;
	std::string currentFileData_;
	bool updated_ = false;

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

	void onConnected() {
		std::cout << "Connected!" << std::endl;
		BinaryOstream os;
		os << OpcodeClientToSrv::REQUEST_FILEDATA;
		client_->send(os.str());
	}

	void onDisconnected() {
		std::cout << "Disconnected!" << std::endl;
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
			auto iter = fds_.find(name);
			if (iter == fds_.end()) {
				toUpdate_.push_back(name);
			} else {
				const FileData &lfd = iter->second;
				if (fd.size_ != lfd.size_ || fd.checksum_ != lfd.checksum_) {
					toUpdate_.push_back(name);
				}
			}
		}
		requestNextFile();
	}

	void requestNextFile() {
		if (toUpdate_.empty()) {
			client_->close();
			return;
		}
		currentFile_ = toUpdate_.front();
		toUpdate_.pop_front();
		std::cout << "Requesting file " << currentFile_ << " " << std::endl;
		currentFileDesc_ = serverFds_.at(currentFile_);
		currentFileData_.clear();
		currentFileData_.reserve(currentFileDesc_.size_);
		BinaryOstream os;
		os << OpcodeClientToSrv::REQUEST_FILE;
		os << currentFile_;
		client_->send(os.str());
	}

	void onResponseFile(BinaryIstream &is) {
		std::string data;
		is >> data;
		currentFileData_.append(data);
		if (currentFileDesc_.size_ > 0) {
			std::size_t lastProgress = ((currentFileData_.size() - data.size())*100) / currentFileDesc_.size_;
			std::size_t currentProgress = (currentFileData_.size()*100) / currentFileDesc_.size_;
			for (int i = 0; i < currentProgress - lastProgress; ++i) {
				std::cout << "*";
			}
		}
	}

	void onResponseFileFinished() {
		std::cout << " Got file " << currentFile_ << std::endl;
		std::string fullPath = toStr(path_, "/", currentFile_);
		makeFullPath(filePath(fullPath));
		writeFile(fullPath, currentFileData_);
		updated_ = true;
		FileData fd;
		fd.size_ = currentFileData_.size();
		fd.checksum_ = calculateChecksum(currentFileData_);
		fd.timestamp_ = getModificationTime(fullPath);
		fds_[currentFile_] = fd;
		requestNextFile();
	}
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
