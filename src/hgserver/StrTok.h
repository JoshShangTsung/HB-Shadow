#pragma once

class CStrTok {
public:
	char * pGet();
	CStrTok(char * pData, const char * pSeps);

	char * m_pData = nullptr;
	const char* m_pSeps = nullptr;
	char m_cToken[1024]{};
	int m_iDataLength = 0;
	int m_iCurLoc = 0;

private:
	bool _bIsSeperator(char cData, char cNextData);
};
