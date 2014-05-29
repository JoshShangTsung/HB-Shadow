#include "updaterlib.h"
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <deque>
#include <set>
#include <map>
#include <windows.h>

bool operator==(const Timestamp &a, const Timestamp &b) {
	return a.millisecond_ == b.millisecond_ &&
			  a.second_ == b.second_ &&
			  a.minute_ == b.minute_ &&
			  a.hour_ == b.hour_ &&
			  a.day_ == b.day_ &&
			  a.month_ == b.month_ &&
			  a.year_ == b.year_;
}

bool operator!=(const Timestamp &a, const Timestamp &b) {
	return !(a == b);
}

BinaryOstream& operator<<(BinaryOstream &os, const std::string &str) {
	uint32_t sz = str.size();
	os << sz;
	os.add(str.data(), str.size());
	return os;
}

BinaryIstream& operator>>(BinaryIstream &is, std::string &str) {
	uint32_t sz;
	is >> sz;
	str.assign(sz, 0);
	is.get(&str[0], str.size());
	return is;
}

namespace I {

	void toStr(std::ostream &) {
	}
}

std::string readFile(const std::string &fileName) {
	std::ifstream in(fileName.c_str(), std::ios::in | std::ios::binary);
	if (!in) {
		throw std::runtime_error(toStr("Couldn't find file ", fileName, " for read"));
	}
	std::stringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}

void writeFile(const std::string &fileName, const std::string &data) {
	std::ofstream ofs(fileName, std::ios::out | std::ios::binary);
	if (!ofs) {
		throw std::runtime_error(toStr("Couldn't open file ", fileName, " for writing"));
	}
	ofs << data;
}
namespace {

	FileDatas getFileData(const std::string &path);
	LightFileDatas getLightFileData(const std::string &path);

	std::pair<std::vector<std::string>, std::vector<std::string>> getFiles(const std::string &path) {
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

	void addFileData(FileDatas &ret, const std::string &realPath, const std::string &path) {
		std::vector<std::string> files;
		std::vector<std::string> folders;
		std::tie(files, folders) = getFiles(realPath.c_str());
		for (const std::string &f : files) {
			std::string fileName = toStr(realPath, "/", f);
			std::string fileData = readFile(fileName.c_str());
			std::string name = path.empty() ? f : toStr(path, "/", f);
			FileData fd;
			fd.size_ = fileData.length();
			fd.checksum_ = calculateChecksum(fileData);
			fd.timestamp_ = getModificationTime(fileName.c_str());
			ret.insert(std::make_pair(std::move(name), std::move(fd)));
		}
		for (const std::string &f : folders) {
			std::string newRealPath = toStr(realPath, "/", f);
			std::string newPath = path.empty() ? f : toStr(path, "/", f);
			addFileData(ret, newRealPath, newPath);
		}
	}

	void addLightFileData(LightFileDatas &ret, const std::string &realPath, const std::string &path) {
		std::vector<std::string> files;
		std::vector<std::string> folders;
		std::tie(files, folders) = getFiles(realPath.c_str());
		for (const std::string &f : files) {
			std::string fileName = toStr(realPath, "/", f);
			//std::string fileData = readFile(fileName.c_str());
			LightFileData fd;
			std::string name = path.empty() ? f : toStr(path, "/", f);
			//fd.size_ = fileData.length();
			//fd.checksum_ = std::accumulate(fileData.begin(), fileData.end(), (uint32_t) 0);
			fd.timestamp_ = getModificationTime(fileName.c_str());
			ret.insert(std::make_pair(std::move(name), std::move(fd)));
		}
		for (const std::string &f : folders) {
			std::string newRealPath = toStr(realPath, "/", f);
			std::string newPath = path.empty() ? f : toStr(path, "/", f);
			addLightFileData(ret, newRealPath, newPath);
		}
	}

	FileDatas getFileData(const std::string &path) {
		FileDatas ret;
		addFileData(ret, path,{});
		return ret;
	}

	LightFileDatas getLightFileData(const std::string &path) {
		LightFileDatas ret;
		addLightFileData(ret, path,{});
		return ret;
	}

	void loadCache(FileDatas&fds, const std::string &fileName) {
		fds = fromStr(readFile(fileName));
	}

}

uint32_t calculateChecksum(const std::string &data) {
	return std::accumulate(data.begin(), data.end(), (uint32_t) 0);
}

Timestamp getModificationTime(const std::string &path) {
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
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

void saveCache(const FileDatas &fds, const std::string &fileName) {
	std::string str = asStr(fds);
	std::ofstream ofs(fileName, std::ios::out | std::ios::binary);
	ofs << str;
}

std::string asStr(const FileDatas &fds) {
	BinaryOstream os;
	uint32_t sz = fds.size();
	os << sz;
	for (const std::pair<std::string, FileData> &p : fds) {
		const std::string &name = p.first;
		const FileData &fd = p.second;
		os << name << fd.size_ << fd.checksum_ <<
				  fd.timestamp_.year_ << fd.timestamp_.month_ << fd.timestamp_.day_ <<
				  fd.timestamp_.hour_ << fd.timestamp_.minute_ << fd.timestamp_.second_ << fd.timestamp_.millisecond_;
	}
	return os.str();
}

FileDatas fromStr(const std::string &str) {
	FileDatas fds;
	BinaryIstream is(str);
	uint32_t sz = 0;
	is >> sz;
	fds.reserve(sz);
	for (std::size_t i = 0; i < sz; ++i) {
		std::string name;
		FileData fd;
		is >> name >> fd.size_ >> fd.checksum_ >>
				  fd.timestamp_.year_ >> fd.timestamp_.month_ >> fd.timestamp_.day_ >>
				  fd.timestamp_.hour_ >> fd.timestamp_.minute_ >> fd.timestamp_.second_ >> fd.timestamp_.millisecond_;
		fds.insert(std::make_pair(std::move(name), std::move(fd)));
	}
	return fds;
}

void print(const FileDatas &fds) {
	for (const auto &p : fds) {
		const std::string &name = p.first;
		const FileData &fd = p.second;
		std::cout << name << " " << fd.size_ << " " << fd.checksum_ << " " <<
				  (int) fd.timestamp_.year_ << "/" << (int) fd.timestamp_.month_ << "/" << (int) fd.timestamp_.day_ << " " <<
				  (int) fd.timestamp_.hour_ << ":" << (int) fd.timestamp_.minute_ << ":" << (int) fd.timestamp_.second_ << "." << fd.timestamp_.millisecond_ << std::endl;
	}
}

namespace {

	void updateFd(FileData &fd, const LightFileData &lfd, const std::string &path, const std::string &name) {
		fd.timestamp_ = lfd.timestamp_;
		std::string data = readFile(toStr(path, "/", name).c_str());
		fd.size_ = data.size();
		fd.checksum_ = calculateChecksum(data);
	}
}

FileDatas getFileData(const std::string &path, const std::string &fileName) {
	FileDatas fds;
	try {
		loadCache(fds, fileName);
		LightFileDatas lfds = getLightFileData(path);
		lfds.erase(fileName);
		bool updated = false;
		for (const auto &p : lfds) {
			const std::string &name = p.first;
			const LightFileData &lfd = p.second;
			auto iter = fds.find(name);
			if (iter != fds.end()) {
				FileData &fd = iter->second;
				if (fd.timestamp_ != lfd.timestamp_) {
					// Modified
					updateFd(fd, lfd, path, name);
					updated = true;
				}
			} else {
				// New file
				FileData fd;
				updateFd(fd, lfd, path, name);
				fds.insert(std::make_pair(name, std::move(fd)));
				updated = true;
			}
		}
		std::vector<std::string> toRemove;
		for (const auto &p : fds) {
			const std::string &name = p.first;
			auto iter = lfds.find(name);
			if (iter == lfds.end()) {
				updated = true;
				toRemove.push_back(name);
			}
		}
		std::for_each(toRemove.begin(), toRemove.end(), [&fds](const std::string & name) {
			fds.erase(name);
		});
		if (updated) {
			saveCache(fds, fileName);
		}
	} catch (std::exception &e) {
		std::cout << "Exception loading cache: " << e.what() << std::endl;
		std::cout << "Caching data..." << std::endl;
		fds = getFileData(path);
		fds.erase(fileName);
		saveCache(fds, fileName);
	}
	return fds;
}

void makePath(const std::string &path) {
	auto delays = {0, 0, 0, 100, 500, 1000, 2000};
	int ret = 0;
	for (auto delay : delays) {
		ret = CreateDirectory(path.c_str(), nullptr);
		if (ret == 0 || ret == ERROR_ALREADY_EXISTS) {
			return;
		}
		Sleep(delay);
	}
	throw std::runtime_error(toStr("Couldn't create path ", path, ": error code ", ret));
}

void makeFullPath(const std::string &path) {
	if (path.empty()) {
		return;
	}
	std::size_t i = 0;
	while (true) {
		i = path.find_first_of("/", i);
		std::string partialPath = path.substr(0, i);
		makePath(partialPath);
		if (i == std::string::npos) {
			break;
		}
		++i;
	}
}

std::string filePath(const std::string &path) {
	std::size_t i = path.find_last_of("/");
	if (i == std::string::npos) {
		return "";
	}
	return path.substr(0, i);
}

void launchProcess(const std::string &fileName) {
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof (siStartupInfo));
	memset(&piProcessInfo, 0, sizeof (piProcessInfo));
	siStartupInfo.cb = sizeof (siStartupInfo);

	::CreateProcess(fileName.c_str(), // application name/path
			  NULL, // command line (optional)
			  NULL, // no process attributes (default)
			  NULL, // default security attributes
			  false,
			  CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE,
			  NULL, // default env
			  NULL, // default working dir
			  &siStartupInfo,
			  &piProcessInfo);
}

void closeThisProcess() {
	::TerminateProcess(GetCurrentProcess(), 0);
	::ExitProcess(0); // exit this process
}
namespace Net {
	namespace Packeted {

		struct Message {
		private:
#pragma pack(1)

			struct Header {
				uint8_t key_;
				uint16_t size_;
			};
#pragma pack()
		public:
			constexpr static std::size_t HEADER_SIZE = sizeof (Header);

			Message() {
				clear();
			}

			Message(const std::string &data) {
				clear();
				data_.append(data.begin(), data.end());
				Header &header = getHeader();
				header.key_ = 0;
				header.size_ = data.size() + HEADER_SIZE;
			}

			void decode() {
				const Header &header = getHeader();
				const uint8_t key = header.key_;
				const uint16_t size = header.size_ - 3;
				if (key) {
					char *body = getBody();
					for (int i = 0; i < (int) (size); i++) {
						body[i] = (char) (body[i] ^ (key ^ (size - i)));
						body[i] -= (i ^ key);
					}
				}
			}

			void encode() {
				Header &header = getHeader();
				header.key_ = (rand() % (0xff - 1)) + 1; // 1~255
				const uint8_t key = header.key_;
				const uint16_t size = header.size_ - 3;
				char *body = getBody();
				for (int i = 0; i < size; i++) {
					body[i] += (i ^ key);
					body[i] = (char) (body[ i] ^ (key ^ (size - i)));
				}
			}

			bool decodeHeader() {
				Header &header = getHeader();
				data_.resize(header.size_, 0);
				return true;
			}

			char *getData() {
				return &data_[0];
			}

			const char *getData() const {
				return &data_[0];
			}

			char *getBody() {
				return getData() + HEADER_SIZE;
			}

			const char *getBody() const {
				return getData() + HEADER_SIZE;
			}

			std::size_t getBodySize() const {
				return getSize() - HEADER_SIZE;
			}

			std::size_t getSize() const {
				return data_.size();
			}

			void clear() {
				data_.clear();
				data_.reserve(HEADER_SIZE);
				data_.assign(3, 0);
			}
		private:
			std::string data_;

			Header &getHeader() {
				return *((Header*) getData());
			}
		};

		typedef std::deque<Message> MsgQueue;

		namespace Server {


			struct Client;
			typedef std::shared_ptr<Client> ClientPtr;

			struct Clients;

			class ClientServer {
			public:

				ClientServer(Clients &clients, asio::io_service& io_service,
						  const asio::ip::tcp::endpoint& endpoint);
			private:
				void do_accept();
				asio::io_service &io_service_;
				asio::ip::tcp::acceptor acceptor_;
				asio::ip::tcp::socket socket_;
				Clients &clients_;
			};

			class Clients : public IClients {
			public:

				void addServer(asio::io_service& io_service,
						  const asio::ip::tcp::endpoint& endpoint) override {
					servers_.emplace_back(*this, io_service, endpoint);
				}
				void add(ClientPtr client);
				void remove(ClientPtr client);
				void deliver(ClientPtr client, const Message& msg);
				void send(ClientId id, const std::string &data) override;
				void close(ClientId id) override;

				ServerEvents getEvents() override {
					ServerEvents tmp;
					std::swap(tmp, events_);
					return tmp;
				}
			private:
				ClientId nextId_ = 1;
				std::set<ClientPtr> clients_;
				std::map<ClientId, ClientPtr> clientsById_;
				ServerEvents events_;
				std::vector<ClientServer> servers_;
			};

			class Client : public std::enable_shared_from_this<Client> {
			public:

				Client(asio::io_service &io_service, asio::ip::tcp::socket socket, Clients& clients)
				: io_service_(io_service), socket_(std::move(socket)),
				clients_(clients) {
				}

				void start() {
					auto self(shared_from_this());
					clients_.add(self);
					do_read_header();
				}

				void deliver(const Message& msg) {
					io_service_.post(
							  [this, msg]() {
								  bool write_in_progress = !write_msgs_.empty();
								  write_msgs_.push_back(msg);
								  if (!write_in_progress) {
									  do_write();
								  }
							  });
				}

				void setId(ClientId id) {
					id_ = id;
				}

				ClientId getId() const {
					return id_;
				}

				void close() {
					io_service_.post([this]() {
						socket_.close(); });
				}
				std::string address() const {
					return toStr(socket_.remote_endpoint().address().to_string(), ":", socket_.remote_endpoint().port());
				}
			private:
				ClientId id_ = 0;

				void disconnected(ClientPtr self) {
					clients_.remove(self);
				}

				void do_read_header() {
					auto self(shared_from_this());
					asio::async_read(socket_,
							  asio::buffer(read_msg_.getData(), Message::HEADER_SIZE),
							  [this, self](std::error_code ec, std::size_t /*length*/) {
								  if (!ec && read_msg_.decodeHeader()) {
									  do_read_body();
								  } else {
									  disconnected(self);
								  }
							  });
				}

				void do_read_body() {
					auto self(shared_from_this());
					asio::async_read(socket_,
							  asio::buffer(read_msg_.getBody(), read_msg_.getBodySize()),
							  [this, self](std::error_code ec, std::size_t /*length*/) {
								  if (!ec) {
									  read_msg_.decode();
									  clients_.deliver(self, read_msg_);
									  read_msg_.clear();
									  do_read_header();
								  } else {
									  disconnected(self);
								  }
							  });
				}

				void do_write() {
					auto self(shared_from_this());
					asio::async_write(socket_,
							  asio::buffer(write_msgs_.front().getData(),
							  write_msgs_.front().getSize()),
							  [this, self](std::error_code ec, std::size_t /*length*/) {
								  if (!ec) {
									  write_msgs_.pop_front();
									  if (!write_msgs_.empty()) {
										  do_write();
									  }
								  } else {
									  disconnected(self);
								  }
							  });
				}
				asio::io_service &io_service_;
				asio::ip::tcp::socket socket_;
				Clients& clients_;
				Message read_msg_;
				MsgQueue write_msgs_;
			};

			void Clients::add(ClientPtr client) {
				ClientId id = nextId_++;
				client->setId(id);
				clients_.insert(client);
				clientsById_[id] = client;
				events_.push_back({EventType::CONNECTED, id, client->address()});
			}

			void Clients::remove(ClientPtr client) {
				const ClientId id = client->getId();
				clients_.erase(client);
				clientsById_.erase(id);
				events_.push_back({EventType::DISCONNECTED, id,
					{}});
			}

			void Clients::deliver(ClientPtr client, const Message& msg) {
				events_.push_back({EventType::MSG, client->getId(), {
						msg.getBody(), msg.getBodySize()
					}});
			}

			void Clients::send(ClientId id, const std::string &data) {
				Message msg(data);
				clientsById_.at(id)->deliver(msg);
			}

			void Clients::close(ClientId id) {
				clientsById_.at(id)->close();
			}

			ClientServer::ClientServer(Clients &clients, asio::io_service& io_service,
					  const asio::ip::tcp::endpoint& endpoint)
			: io_service_(io_service),
			acceptor_(io_service, endpoint),
			socket_(io_service),
			clients_(clients) {
				do_accept();
			}

			void ClientServer::do_accept() {
				acceptor_.async_accept(socket_,
						  [this](std::error_code ec) {
							  if (!ec) {
								  std::make_shared<Client>(io_service_, std::move(socket_), clients_)->start();
							  }

							  do_accept();
						  });
			}
		}

		namespace Client {

			class PacketedClient : public IClient, public std::enable_shared_from_this<PacketedClient> {
			public:

				PacketedClient(asio::io_service& io_service)
				: io_service_(io_service),
				socket_(io_service) {

				}

				void start(asio::ip::tcp::resolver::iterator endpoint_iterator) {
					do_connect(endpoint_iterator);
				}

				void send(const std::string &data) override {
					Message msg(data);
					auto self = shared_from_this();
					io_service_.post(
							  [this, msg, self]() {
								  bool write_in_progress = !write_msgs_.empty();
								  write_msgs_.push_back(msg);
								  if (!write_in_progress) {
									  do_write();
								  }
							  });
				}

				void close() override {
					auto self = shared_from_this();
					io_service_.post([this, self]() {
						socket_.close(); });
				}

				Events getEvents() override {
					Events tmp;
					std::swap(tmp, events_);
					return tmp;
				}

			private:

				void onDisconnect() {
					events_.push_back({EventType::DISCONNECTED,
						{}});
				}

				void do_connect(asio::ip::tcp::resolver::iterator endpoint_iterator) {
					auto self = shared_from_this();
					asio::async_connect(socket_, endpoint_iterator,
							  [this, self](std::error_code ec, asio::ip::tcp::resolver::iterator) {
								  if (!ec) {
									  events_.push_back({EventType::CONNECTED,
										  {}});
									  do_read_header();
								  } else {
									  onDisconnect();
								  }
							  });
				}

				void do_read_header() {
					auto self = shared_from_this();
					asio::async_read(socket_,
							  asio::buffer(read_msg_.getData(), Message::HEADER_SIZE),
							  [this, self](std::error_code ec, std::size_t /*length*/) {
								  if (!ec && read_msg_.decodeHeader()) {
									  do_read_body();
								  } else {
									  socket_.close();
									  onDisconnect();
								  }
							  });
				}

				void do_read_body() {
					auto self = shared_from_this();
					asio::async_read(socket_,
							  asio::buffer(read_msg_.getBody(), read_msg_.getBodySize()),
							  [this, self](std::error_code ec, std::size_t /*length*/) {
								  if (!ec) {
									  read_msg_.decode();
									  events_.push_back({EventType::MSG,
										  {read_msg_.getBody(), read_msg_.getBodySize()}});
									  read_msg_.clear();
									  do_read_header();
								  } else {
									  socket_.close();
									  onDisconnect();
								  }
							  });
				}

				void do_write() {
					auto self = shared_from_this();
					asio::async_write(socket_,
							  asio::buffer(write_msgs_.front().getData(),
							  write_msgs_.front().getSize()),
							  [this, self](std::error_code ec, std::size_t /*length*/) {
								  if (!ec) {
									  write_msgs_.pop_front();
									  if (!write_msgs_.empty()) {
										  do_write();
									  }
								  } else {
									  socket_.close();
									  onDisconnect();
								  }
							  });
				}

			private:
				asio::io_service& io_service_;
				asio::ip::tcp::socket socket_;
				Message read_msg_;
				MsgQueue write_msgs_;
				Events events_;
			};
		}
	}

	ClientsPtr createPacketedClients() {
		return std::make_unique<Packeted::Server::Clients>();
	}

	ClientPtr createPacketedClient(asio::io_service& io_service, const char *host, const char *port) {

		asio::ip::tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({host, port});
		auto ret = std::make_shared<Packeted::Client::PacketedClient>(io_service);
		ret->start(endpoint_iterator);
		return ret;
	}

	/*

typedef FixLenStr<10> AccName;
typedef FixLenStr<10> AccPassword;
typedef FixLenStr<50> AccMail;
typedef FixLenStr<45> AccQuiz;
typedef FixLenStr<20> AccQuizAnswer;
typedef FixLenStr<30> WorldServerName;

enum class Opcode : uint32_t {
	REQUEST_LOGIN = 0x0FC94201,
	REQUEST_CREATENEWACCOUNT = 0x0FC94202
};

namespace std {

	template<> struct hash<Opcode> {

		std::size_t operator()(const Opcode &op) const {
			typedef std::underlying_type<Opcode>::type ut;
			return std::hash<ut>()((ut) op);
		}
	};
}

enum class SOpcode : uint16_t {
};

struct Client {

	Client(Net::IClients &clients, Net::ClientId id) : clients_(clients), id_(id) {
	}
	void send(std::string &msg) {
		clients_.send(id_, msg);
	}
	Net::IClients &clients_;
	Net::ClientId id_;
};

struct BaseAction {
	virtual void execute(Client &client, BinaryIstream &is) = 0;

	virtual ~BaseAction() {
	}
};

struct ActionRegister : BaseAction {

	template<typename T, Opcode op> void registerAction() {
		actions_[op] = std::make_unique<T>();
	}

	void execute(Client &client, BinaryIstream &is) override {
		Opcode op;
		is.peek(op);
		auto iter = actions_.find(op);
		if (iter == actions_.end()) {
			std::cout << "Unknown opcode: " << std::hex << (uint32_t) op << std::endl;
		} else {
			iter->second->execute(client, is);
		}
	}
private:
	std::unordered_map<Opcode, std::unique_ptr<BaseAction>> actions_;
};

template<typename T, Opcode op> struct Action : public BaseAction {

	static void doRegister(ActionRegister &ar) {
		ar.registerAction<T, op>();
	}

	virtual ~Action() {
	}
};

struct RequestNewAccountAction : public Action<RequestNewAccountAction, Opcode::REQUEST_CREATENEWACCOUNT> {

	void execute(Client &client, BinaryIstream &is) override {
		NewAccountMsg nam;
		is >> nam.op_ >> nam.sop_ >> nam.accName_ >> nam.accPwd_ >> nam.accMail_;
		FixLenStr<10> gender;
		is >> gender;
		FixLenStr<10> age;
		is >> age;
		uint32_t d32;
		uint16_t d16;
		FixLenStr<17> country;
		FixLenStr<28> ssn;
		FixLenStr<50> cmdLine;
		is >> d32 >> d16 >> d16 >> country >> ssn >> nam.quiz_ >> nam.quizAns_ >> cmdLine;
		onNewAccount(nam);
	}
private:

	struct NewAccountMsg {
		Opcode op_;
		SOpcode sop_;
		AccName accName_;
		AccPassword accPwd_;
		AccMail accMail_;
		AccQuiz quiz_;
		AccQuizAnswer quizAns_;
	};

	void onNewAccount(NewAccountMsg &msg) {
		std::cout << "New account " << msg.accName_ << ":" << msg.accPwd_ << " " << msg.accMail_ << " - " << msg.quiz_ << "? " << msg.quizAns_ << std::endl;
	}
};

struct RequestLoginAction : public Action<RequestLoginAction, Opcode::REQUEST_LOGIN> {

	void execute(Client &client, BinaryIstream &is) override {

		struct LoginMsg {
			Opcode op_;
			SOpcode sop_;
			AccName accName_;
			AccPassword accPwd_;
			WorldServerName worldServerName_;
		};
		LoginMsg lm;
		is >> lm.op_ >> lm.sop_ >> lm.accName_ >> lm.accPwd_ >> lm.worldServerName_;
		std::cout << "Logging in with " << lm.accName_ << ":" << lm.accPwd_ << " @ " << lm.worldServerName_ << std::endl;
	}
};

struct ClientsListener {

	ClientsListener(Net::IClients &netClients, ActionRegister &reg) : netClients_(netClients), reg_(reg) {
	}

	void onConnect(Net::ClientId id) {
		std::cout << id << " connected" << std::endl;
		clients_.insert(std::make_pair(id, Client(netClients_, id)));
	}

	void onDisconnect(Net::ClientId id) {
		std::cout << id << " disconnected" << std::endl;
		clients_.erase(id);
	}

	void onMsg(Net::ClientId id, const std::string &msg) {
		try {
			Client &client = clients_.at(id);
			BinaryIstream is(msg);
			reg_.execute(client, is);
		} catch (std::exception &e) {

		}
	}
private:
	Net::IClients &netClients_;
	ActionRegister &reg_;
	std::map<Net::ClientId, Client> clients_;
};
#include <thread>

int main2(int) {
	try {
		asio::io_service io_service;
		ActionRegister ar;
		RequestLoginAction::doRegister(ar);
		RequestNewAccountAction::doRegister(ar);
		Net::ClientsPtr clients = Net::createPacketedClients();
		ClientsListener listener(*clients, ar);
		auto ports = {2848};
		for (int i : ports) {
			asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), i);
			clients->addServer(io_service, endpoint);
		}
		std::thread t([&io_service]() {
			io_service.run();
		});
		while (true) {
			auto events = clients->getEvents();
			for (const Net::ServerEvent &ev : events) {
				switch (ev.type_) {
					case Net::EventType::CONNECTED:
						listener.onConnect(ev.id_);
						break;
					case Net::EventType::DISCONNECTED:
						listener.onDisconnect(ev.id_);
						break;
					case Net::EventType::MSG:
						listener.onMsg(ev.id_, ev.msg_);
						break;
				}
			}
		}
	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
	 */
}