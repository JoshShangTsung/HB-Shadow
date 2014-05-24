#include "Curse.h"
#include <sstream>
#include <fstream>
#include <cstring>

void CCurse::LoadCurse(const char *filename) {
	std::ifstream is(filename);
	std::string str;
	while (std::getline(is, str)) {
		std::stringstream ss(str);
		std::string name;
		std::string value;
		ss >> name >> value;
		if (ss) {
			curses_[name] = value;
		}
	}
}
namespace {

	bool isIgnorable(char c) {
		static std::string ignore("\t _-.:^");
		for (auto t : ignore) {
			if (t == c) {
				return true;
			}
		}
		return false;
	}

	bool replace(std::string &s, std::size_t &i, const std::string &from, const std::string &to) {

		std::size_t j = i;
		std::size_t t = 0;
		while (char c = s[j]) {
			if (isIgnorable(c)) {
				if(j == i) return false;
				++j;
				continue;
			}
			char f = from[t];
			if (c != f) {
				return false;
			}
			++t;
			++j;
			if (t == from.length()) {
				s.replace(s.begin() + i, s.begin() + j, to);
				i += to.length();
				return true;
			}
		}
		return false;
	}
}

void CCurse::ConvertString(char * str, int max_len) {
	if (!str || !max_len) return;
	std::string s(str);
	for (std::size_t i = 0; i < s.length(); ++i) {
		for (auto iter : curses_) {
			if (replace(s, i, iter.first, iter.second)) {
				--i;
				break;
			}
		}
	}
	if(s.length() <= max_len) {
		strcpy(str, s.data());
	}
}
