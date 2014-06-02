#include "StrTok.h"
#include <cstring>
extern void PutLogFileList(char * cStr);

CStrTok::CStrTok(char * pData, const char * pSeps) {
	m_pData = pData;
	m_pSeps = pSeps;

	m_iDataLength = strlen(pData);
	m_iCurLoc = 0;
}

char * CStrTok::pGet() {
	register int i = 0;
	char cNextData;
	bool bFlag;

	std::memset(m_cToken, 0, sizeof (m_cToken));

	while (m_iCurLoc < m_iDataLength) {

		if (m_iCurLoc <= (m_iDataLength - 2))
			cNextData = m_pData[m_iCurLoc + 1];
		else cNextData = 0;

		if (_bIsSeperator(m_pData[m_iCurLoc], cNextData) == false) {
			// separator�� �ƴϸ� ��ū�� �ۼ�  
			m_cToken[i] = m_pData[m_iCurLoc];
			i++;
			m_iCurLoc++;
		} else {
			// ���� ��ū���� �����͸� �̵���Ų�� ��ū�� ��ȯ�Ѵ�.
			bFlag = false;
			while (bFlag == false) {
				if (m_iCurLoc <= (m_iDataLength - 2))
					cNextData = m_pData[m_iCurLoc + 1];
				else cNextData = 0;
				if (_bIsSeperator(m_pData[m_iCurLoc], cNextData) == true) {
					m_iCurLoc++;
				} else bFlag = true;

				if (m_iCurLoc >= (m_iDataLength - 1)) bFlag = true;
			}

			return (char *) (&m_cToken);
		}
	}

	if (strlen(m_cToken) != 0) return (char *) (&m_cToken);
	return nullptr;
}

bool CStrTok::_bIsSeperator(char cData, char cNextData) {
	int i = 0;

	if (cData == 0) return true;
	if ((cData == 0x0D) && (cNextData == 0x0A)) return true;

	while (m_pSeps[i] != 0) {
		if (m_pSeps[i] == cData) return true;
		i++;
	}

	return false;
}