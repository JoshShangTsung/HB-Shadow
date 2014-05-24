#pragma once
#include <windows.h>

class CStrTok {
public:
	char * pGet();
	CStrTok(char * pData, char * pSeps);

	char * m_pData, * m_pSeps, m_cToken[1024];
	int m_iDataLength, m_iCurLoc;

private:
	BOOL _bIsSeperator(char cData, char cNextData);
};
