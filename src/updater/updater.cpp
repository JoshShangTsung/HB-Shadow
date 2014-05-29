#include "../updaterlib/updaterlib.h"
#include <iostream>

struct MyListener : public Updater::Listener {

	void loadingCache() override {
		std::cout << "Loading cache..." << std::endl;
	}

	void connecting(const std::string &/*address*/, const std::string&/*port*/) override {
		std::cout << "Connecting to updating site..." << std::endl;
	}

	void couldntConnect() override {
		std::cout << "Couldn't connect to updating site!" << std::endl;
	}
	void requestingFileData() override {
		std::cout << "Requesting list of updates..." << std::endl;
	}

	void updaterUpdateRequired(std::size_t totalSize) override {
		std::cout << "A new updater was found." << std::endl;
		currentFileIndex_ = 0;
		totalFiles_ = 1;
		totalSize_ = totalSize;
		accumulatedSize_ = 0;
	}

	void updatesRequired(std::size_t numFiles, std::size_t totalSize) override {
		currentFileIndex_ = 0;
		totalFiles_ = numFiles;
		totalSize_ = totalSize;
		accumulatedSize_ = 0;
	}

	void noUpdatesRequired() override {
		std::cout << "No updates found" << std::endl;
	}

	void requestingFile(const std::string &name, std::size_t sz) override {
		std::cout << (currentFileIndex_ + 1) << " of " << totalFiles_ << '\t' << name << '\t';
		std::cout.flush();
		currentFileSize_ = 0;
		currentFileTotalSize_ = sz;
	}

	void gotChunk(const std::size_t sz) override {
		const int lastPct = currentFileSize_ / (currentFileTotalSize_ / double(100));
		currentFileSize_ += sz;
		accumulatedSize_ += sz;
		const int currentPct = currentFileSize_ / (currentFileTotalSize_ / double(100));
		const int diff = currentPct - lastPct;
		if (diff) {
			for (int i = 0; i < diff; ++i) {
				std::cout << '*';
			}
			std::cout.flush();
		}
	}

	void fileFinished() override {
		const int accumulatedPct = accumulatedSize_ / (totalSize_ / double(100));
		std::cout << '\t' << accumulatedPct << "%\t" << accumulatedSize_ << "\t" << totalSize_ << std::endl;
		++currentFileIndex_;
	}

	void jobDone() override {
		std::cout << "Everything is up to date" << std::endl;
	}
private:
	std::size_t totalFiles_;
	std::size_t totalSize_;
	std::size_t currentFileTotalSize_;
	std::size_t currentFileSize_;
	std::size_t currentFileIndex_;
	std::size_t accumulatedSize_;
};

int main() {
	try {
		MyListener listener;
		Updater::run(listener, "localhost", "1492");
	} catch (std::exception &e) {
		std::cout << "Exception " << e.what() << std::endl;
	} catch (...) {
		std::cout << "Exception!" << std::endl;
	}
	return 0;
}
