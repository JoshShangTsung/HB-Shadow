#pragma once
#include <windows.h>

class CStrTok {
public:
	CStrTok(char * pData, char * pSeps);
	char * pGet();
	bool _bIsSeperator(char cData, char cNextData);

	char * m_pData, * m_pSeps, m_cToken[256];
	int m_iDataLength, m_iCurLoc;
};
