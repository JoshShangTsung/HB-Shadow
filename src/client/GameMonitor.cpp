#include "GameMonitor.h"
#include <cstring>

CGameMonitor::CGameMonitor() {
	int i;

	for (i = 0; i < DEF_MAXBADWORD; i++)
		m_pWordList[i] = 0;
}

CGameMonitor::~CGameMonitor() {
	int i;

	for (i = 0; i < DEF_MAXBADWORD; i++)
		delete m_pWordList[i];
}

int CGameMonitor::iReadBadWordFileList(const char *pFn) {
	char * pContents, * token;
	char seps[] = "/,\t\n";
	int iIndex = 0;
	class CStrTok * pStrTok;
	HANDLE hFile;
	FILE * pFile;
	uint32_t dwFileSize;

	hFile = CreateFile(pFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	dwFileSize = GetFileSize(hFile, 0);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(pFn, "rt");
	if (pFile == 0) return 0;
	else {
		pContents = new char[dwFileSize + 1];
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != 0) {
		m_pWordList[iIndex] = new class CMsg(0, token, 0);
		iIndex++;
		if (iIndex >= DEF_MAXBADWORD) break;
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete[] pContents;
	return iIndex;
}

bool CGameMonitor::bCheckBadWord(char *pWord) {
	int i;
	char cBuffer[500];
	std::memset(cBuffer, 0, sizeof(cBuffer));
	strcpy(cBuffer, pWord);
	i = 0;
	while ((m_pWordList[i] != 0) && (strlen(m_pWordList[i]->m_pMsg) != 0)) {
		if (memcmp(cBuffer, m_pWordList[i]->m_pMsg, strlen(m_pWordList[i]->m_pMsg)) == 0) {
			return true;
		}
		i++;
	}
	return false;
}
