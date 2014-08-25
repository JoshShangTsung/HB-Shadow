#pragma once
#include <map>
class CCurse {
public:
	void LoadCurse(const char *filename);
	void ConvertString(char *str, std::size_t max_len);
private:
	std::map<std::string, std::string> curses_;
};
