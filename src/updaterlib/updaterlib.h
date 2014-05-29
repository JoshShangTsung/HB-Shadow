#pragma once
#include <unordered_map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <memory>

#define ASIO_STANDALONE
#include <asio.hpp>

namespace std {

	template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args&&... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
}

namespace Net {
	typedef std::size_t ClientId;

	enum class EventType {
		CONNECTED,
		DISCONNECTED,
		MSG
	};

	struct ServerEvent {
		EventType type_;
		ClientId id_;
		std::string msg_;
	};
	typedef std::vector<ServerEvent> ServerEvents;

	struct IClients {
		virtual void addServer(asio::io_service& io_service,
				  const asio::ip::tcp::endpoint& endpoint) = 0;
		virtual ServerEvents getEvents() = 0;
		virtual void send(ClientId id, const std::string &data) = 0;
		virtual void close(ClientId id) = 0;

		virtual ~IClients() {
		}
	};
	typedef std::unique_ptr<IClients> ClientsPtr;

	struct Event {
		EventType type_;
		std::string msg_;
	};
	typedef std::vector<Event> Events;

	struct IClient {
		virtual Events getEvents() = 0;
		virtual void send(const std::string &data) = 0;
		virtual void close() = 0;
	};
	typedef std::shared_ptr<IClient> ClientPtr;

	ClientsPtr createPacketedClients();

	ClientPtr createPacketedClient(asio::io_service& io_service, const char*address, const char*port);
}

struct Timestamp {
	int16_t year_;
	int8_t month_;
	int8_t day_;
	int8_t hour_;
	int8_t minute_;
	int8_t second_;
	int16_t millisecond_;
};

bool operator==(const Timestamp &a, const Timestamp &b);
bool operator!=(const Timestamp &a, const Timestamp &b);

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

BinaryOstream& operator<<(BinaryOstream &os, const std::string &str);

template<typename T> typename std::enable_if<std::is_enum<T>::value, BinaryOstream&>::type operator<<(BinaryOstream &os, T v) {
	typedef typename std::underlying_type<T>::type ut;
	os << (ut) v;
	return os;
}

struct BinaryIstream {

	BinaryIstream(const std::string &str) : str_(str), size_(str_.size()) {
	}

	template<typename T> void peek(T &v) {
		auto i = index_;
		try {
			*this >> v;
		} catch (std::exception &e) {
			index_ = i;
			throw e;
		}
		index_ = i;
	}

	void get(void *ptr, std::size_t sz) {
		if (remaining() < sz) {
			throw std::runtime_error("Not enough data");
		}
		std::memcpy(ptr, current(), sz);
		advance(sz);
	}

	void advance(std::size_t sz) {
		if (sz > remaining()) {
			throw std::runtime_error("Can't advance that much");
		}
		index_ += sz;
	}

	const char *current() const {
		return str_.data() + index_;
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

BinaryIstream& operator>>(BinaryIstream &is, std::string &str);

template<typename T> typename std::enable_if<std::is_enum<T>::value, BinaryIstream &>::type &operator>>(BinaryIstream &is, T &i) {
	typedef typename std::underlying_type<T>::type ut;
	ut u;
	is >> u;
	i = (T) u;
	return is;
}

template<std::size_t L> struct FixLenStr {
	constexpr static std::size_t LENGTH = L;

	FixLenStr() {
	}

	FixLenStr(const FixLenStr<L> &o) {
		data_ = o.data_;
		length_ = o.length_;
	}

	explicit FixLenStr(const char *ptr) {
		strncpy(data_.data(), ptr, LENGTH);
		length_ = strlen(c_str());
	}

	void operator=(const char *ptr) {
		data_.fill(0);
		strncpy(data_.data(), ptr, LENGTH);
		length_ = strlen(c_str());
	}

	const char *c_str() const {
		return data_.data();
	}

	std::size_t length() const {
		return length_;
	}
private:
	std::array<char, LENGTH + 1 > data_;
	std::size_t length_ = 0;
};

template<std::size_t sz> bool operator==(FixLenStr<sz> &a, FixLenStr<sz> &b) {
	return strcmp(a.c_str(), b.c_str()) == 0;
}

template<std::size_t L> std::ostream &operator<<(std::ostream &os, const FixLenStr<L> &s) {
	os << s.c_str();
	return os;
}

template<std::size_t len> BinaryIstream &operator>>(BinaryIstream &is, FixLenStr<len> &str) {
	if (is.remaining() < len) {
		throw std::out_of_range("Extracting FixLenStr");
	}
	str = is.current();
	is.advance(len);
	return is;
}

struct FileData {
	uint32_t size_;
	uint32_t checksum_;
	Timestamp timestamp_;
};

struct LightFileData {
	Timestamp timestamp_;
};

namespace I {

	void toStr(std::ostream &);

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


std::string readFile(const std::string &fileName);
void writeFile(const std::string &fileName, const std::string &data);
typedef std::unordered_map<std::string, FileData> FileDatas;
typedef std::unordered_map<std::string, LightFileData> LightFileDatas;
void print(const FileDatas &fds);
FileDatas getFileData(const std::string &path, const std::string &fileName);
std::string asStr(const FileDatas &fds);
FileDatas fromStr(const std::string &str);
uint32_t calculateChecksum(const std::string &data);
Timestamp getModificationTime(const std::string &path);
void launchProcess(const std::string &fileName);
void closeThisProcess();

enum class OpcodeClientToSrv : uint32_t {
	REQUEST_FILEDATA,
	REQUEST_FILE
};

enum class OpcodeSrvToClient {
	RESPONSE_FILEDATA,
	RESPONSE_FILE,
	RESPONSE_FILE_FINISHED
};
void saveCache(const FileDatas &fds, const std::string &fileName);
void makeFullPath(const std::string &path);
std::string filePath(const std::string &path);

namespace Updater {

	struct Listener {
		virtual void loadingCache() = 0;
		virtual void connecting(const std::string &address, const std::string &port) = 0;
		virtual void couldntConnect()=0;
		virtual void requestingFileData() = 0;
		virtual void updaterUpdateRequired(std::size_t totalSize) = 0;
		virtual void updatesRequired(std::size_t numFiles, std::size_t totalSize) = 0;
		virtual void noUpdatesRequired() = 0;
		virtual void requestingFile(const std::string &name, std::size_t sz) = 0;
		virtual void gotChunk(const std::size_t sz) = 0;
		virtual void fileFinished() = 0;
		virtual void jobDone() = 0;
	protected:

		~Listener() {
		}
	};

	void run(Listener &listener, const std::string &address, const std::string &port);
}
namespace UpdateServer {

	struct Listener {
		virtual void loadingMetadata() = 0;
		virtual void listening(uint16_t port) = 0;
		virtual void clientConnected(Net::ClientId id, const std::string &address) = 0;
		virtual void clientDisconnected(Net::ClientId id) = 0;
		virtual void clientRequestedBadFile(Net::ClientId id, const std::string &file)=0;
		virtual void clientRequestedFile(Net::ClientId id, const std::string &file)=0;
	protected:

		~Listener() {
		}
	};

	void run(Listener &listener, uint16_t port);
}