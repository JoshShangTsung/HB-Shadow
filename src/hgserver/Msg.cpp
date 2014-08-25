#include "Msg.h"

CMsg::CMsg() {
	m_pData = 0;
	m_dwSize = 0;
}

CMsg::~CMsg() {
	if (m_pData != 0) delete m_pData;
}

bool CMsg::bPut(char cFrom, char * pData, DWORD dwSize, int iIndex, char cKey) {
	m_pData = new char [dwSize + 1];
	if (m_pData == 0) return false;
	ZeroMemory(m_pData, dwSize + 1);
	memcpy(m_pData, pData, dwSize);

	m_dwSize = dwSize;
	m_cFrom = cFrom;
	m_iIndex = iIndex;
	m_cKey = cKey;

	return true;
}

void CMsg::Get(char * pFrom, char * pData, DWORD * pSize, int * pIndex, char * pKey) {
	*pFrom = m_cFrom;
	memcpy(pData, m_pData, m_dwSize);
	*pSize = m_dwSize;
	*pIndex = m_iIndex;
	*pKey = m_cKey;
}
