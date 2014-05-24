#pragma once

class CCurse {
public:
	void LoadCurse(const char *filename);
	bool IsCurse(const char *str);
	char *ConvertString(char *str, int max_len);
	void filterChar(const char *src, char *dst, const char *chars);
	char* getField(const char *buf, int delim, char *field);
};
