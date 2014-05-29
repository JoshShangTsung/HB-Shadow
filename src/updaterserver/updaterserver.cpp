#include "../updaterlib/updaterlib.h"
#include <iostream>

struct MyListener : public UpdateServer::Listener {

	void loadingMetadata() override {
		std::cout << "Loading metadata..." << std::endl;
	}

	void listening(uint16_t port) override {
		std::cout << "Listening at port " << port << std::endl;
	}

	void clientConnected(Net::ClientId id, const std::string &address) override {
		std::cout << "Client " << id << " connected from " << address.substr(0, address.find_first_of(":")) << std::endl;
	}

	void clientDisconnected(Net::ClientId id) override {
		std::cout << "Client " << id << " disconnected" << std::endl;
	}

	void clientRequestedBadFile(Net::ClientId id, const std::string &file) override {
		std::cout << "Client " << id << " requested bad file " << file << std::endl;
	}

	void clientRequestedFile(Net::ClientId /*id*/, const std::string &/*file*/) override {
		// nop
	}
};

int main() {
	try {
		MyListener listener;
		UpdateServer::run(listener, 1492);
	} catch (std::exception &e) {
		std::cout << "Exception " << e.what() << std::endl;
	} catch (...) {
		std::cout << "Exception!" << std::endl;
	}
	return 0;
}
