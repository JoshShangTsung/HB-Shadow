#include "../updaterlib/updaterlib.h"
#include <iostream>
#include <map>

struct App {

	App() {
		watchedPath_ = "watched";
		fds_ = getFileData(watchedPath_, "cache.dat");
		filedata_ = asStr(fds_);
		clients_ = Net::createPacketedClients();
		clients_->addServer(io_service_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1492));
		std::cout << "Listening..." << std::endl;
		while (io_service_.run_one()) {
			update();
		}
	}
private:
	std::string watchedPath_;
	FileDatas fds_;
	std::string filedata_;
	asio::io_service io_service_;
	Net::ClientsPtr clients_;

	struct Client {

		Client(Net::ClientId id, Net::IClients &clients) : id_(id), clients_(clients) {
		}
		void send(const std::string &msg) {
			clients_.send(id_, msg);
		}
		void close() {
			clients_.close(id_);
		}
		Net::ClientId id_;
		Net::IClients &clients_;
	};
	std::map<Net::ClientId, Client> clis_;

	void update() {
		Net::ServerEvents events = clients_->getEvents();
		for (const Net::ServerEvent &ev : events) {
			switch (ev.type_) {
				case Net::EventType::CONNECTED:
					onConnected(ev.id_);
					break;
				case Net::EventType::DISCONNECTED:
					onDisconnected(ev.id_);
					break;
				case Net::EventType::MSG:
					onMsg(ev.id_, ev.msg_);
					break;
			}
		}
	}

	void onConnected(Net::ClientId id) {
		std::cout << id << " Connected!" << std::endl;
		clis_.insert(std::make_pair(id, Client(id, *clients_)));
	}

	void onDisconnected(Net::ClientId id) {
		std::cout << id << " Disconnected!" << std::endl;
		clis_.erase(id);
	}

	void onMsg(Net::ClientId id, const std::string&msg) {
		Client &client = clis_.at(id);
		BinaryIstream is(msg);
		OpcodeClientToSrv op;
		is>>op;
		switch(op) {
			case OpcodeClientToSrv::REQUEST_FILEDATA:
				onRequestFileData(client);
				break;
			case OpcodeClientToSrv::REQUEST_FILE:
				onRequestFile(is, client);
				break;
		}
	}
	void onRequestFileData(Client &client) {
		BinaryOstream os;
		os << OpcodeSrvToClient::RESPONSE_FILEDATA;
		os << filedata_;
		client.send(os.str());
	}
	void onRequestFile(BinaryIstream &is, Client &client) {
		std::string fileName;
		is >> fileName;
		auto iter = fds_.find(fileName);
		if(iter == fds_.end()) {
			std::cout << "Requesting invalid file " << fileName << std::endl;
			client.close();
			return;
		}
		std::string data = readFile(toStr(watchedPath_, "/", fileName));
		constexpr std::size_t chunk_size = 1024 -  sizeof(uint32_t);
		for(std::size_t i=0;i*chunk_size<data.size();++i) {
			std::size_t sz = std::min(chunk_size, data.size() - i * chunk_size);
			BinaryOstream os;
			os.reserve(sizeof(OpcodeSrvToClient) + sz + sizeof(uint32_t));
			os << OpcodeSrvToClient::RESPONSE_FILE;
			os << data.substr(i * chunk_size, chunk_size);
			client.send(os.str());
		}
		BinaryOstream os;
		os << OpcodeSrvToClient::RESPONSE_FILE_FINISHED;
		client.send(os.str());
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
