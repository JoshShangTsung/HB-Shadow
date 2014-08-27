#include "Msg.h"
CMsg::CMsg(char cType, const char * pMsg, uint32_t dwTime) {
	m_cType = cType;

	m_pMsg = 0;
	m_pMsg = new char [strlen(pMsg) + 1];
	ZeroMemory(m_pMsg, strlen(pMsg) + 1);
	strcpy(m_pMsg, pMsg);
	m_dwTime = dwTime;
	m_iObjectID = -1;
}

CMsg::~CMsg() {
	if (m_pMsg != 0) delete m_pMsg;
}
