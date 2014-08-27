#include "Client.h"
#include <cstring>

CClient::CClient(HWND hWnd) {
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);

	for (int i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
		m_sItemEquipmentStatus[i] = -1;

	for (int i = 0; i < DEF_MAXITEMS; i++) {
		m_ItemPosList[i].x = 40;
		m_ItemPosList[i].y = 30;
	}

	for (int i = 0; i < 4; i++) {
		m_cExchangeItemIndex[i] = -1;
	}
	std::strcpy(m_cLocation, "NONE");
	std::strcpy(m_cProfile, "__________");
	std::strcpy(m_cLockedMapName, "NONE");
}

CClient::~CClient() {
	if (m_pXSock != 0) delete m_pXSock;
	for (int i = 0; i < DEF_MAXITEMS; i++) {
		if (m_pItemList[i] != 0) {
			delete m_pItemList[i];
			m_pItemList[i] = 0;
		}
	}
	for (int i = 0; i < DEF_MAXBANKITEMS; i++) {
		if (m_pItemInBankList[i] != 0) {
			delete m_pItemInBankList[i];
			m_pItemInBankList[i] = 0;
		}
	}
}

bool CClient::bCreateNewParty() {
	if (m_iPartyRank != -1) return false;
	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();
	for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		std::memset(m_stPartyMemberName[i].cName, 0, sizeof (m_stPartyMemberName[i].cName));
	}
	return true;
}

