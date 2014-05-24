#pragma once
#include <map>
class CCurse {
public:
	void LoadCurse(const char *filename);
	void ConvertString(char *str, int max_len);
private:
	std::map<std::string, std::string> curses_;
};
