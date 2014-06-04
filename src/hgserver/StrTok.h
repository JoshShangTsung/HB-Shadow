#pragma once

class CStrTok {
public:
	char * pGet();
	CStrTok(char * pData, const char * pSeps);

	char * m_pData;
	const char * m_pSeps;
	char m_cToken[1024];
	int m_iDataLength;
	int m_iCurLoc;

private:
	bool _bIsSeperator(char cData, char cNextData);
};
