#pragma once
#include <cstdint>

#define DEF_MSGFROM_CLIENT		1
#define DEF_MSGFROM_LOGSERVER	2
#define DEF_MSGFROM_GATESERVER	3
#define DEF_MSGFROM_BOT			4

class CMsg {
public:
	void Get(char * pFrom, char * pData, uint32_t * pSize, int * pIndex, char * pKey);
	bool bPut(char cFrom, char * pData, uint32_t dwSize, int iIndex, char cKey);
	virtual ~CMsg();

	char m_cFrom = 0;

	char * m_pData = nullptr;
	uint32_t m_dwSize = 0;

	int m_iIndex = 0;
	char m_cKey = 0; // v1.4
};
