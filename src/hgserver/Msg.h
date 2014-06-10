#pragma once
#include <windows.h>

#define DEF_MSGFROM_CLIENT		1
#define DEF_MSGFROM_LOGSERVER	2
#define DEF_MSGFROM_GATESERVER	3
#define DEF_MSGFROM_BOT			4

class CMsg {
public:
	void Get(char * pFrom, char * pData, DWORD * pSize, int * pIndex, char * pKey);
	BOOL bPut(char cFrom, char * pData, DWORD dwSize, int iIndex, char cKey);
	CMsg();
	virtual ~CMsg();

	char m_cFrom;

	char * m_pData;
	DWORD m_dwSize;

	int m_iIndex;
	char m_cKey; // v1.4
};
