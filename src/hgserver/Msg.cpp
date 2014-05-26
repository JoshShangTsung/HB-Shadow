#include "Msg.h"
#include <cstring>

CMsg::CMsg() {
	m_pData = nullptr;
	m_dwSize = 0;
}

CMsg::~CMsg() {
	if (m_pData != nullptr) delete m_pData;
}

bool CMsg::bPut(char cFrom, char * pData, uint32_t dwSize, int iIndex, char cKey) {
	m_pData = new char [dwSize + 1];
	if (m_pData == nullptr) return false;
	memcpy(m_pData, pData, dwSize);
	m_pData[dwSize] = 0;

	m_dwSize = dwSize;
	m_cFrom = cFrom;
	m_iIndex = iIndex;
	m_cKey = cKey;

	return true;
}

void CMsg::Get(char * pFrom, char * pData, uint32_t * pSize, int * pIndex, char * pKey) {
	*pFrom = m_cFrom;
	memcpy(pData, m_pData, m_dwSize);
	*pSize = m_dwSize;
	*pIndex = m_iIndex;
	*pKey = m_cKey;
}
