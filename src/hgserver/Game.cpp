#include "Game.h"
#include <cstring>
#include <algorithm>
class CDebugWindow *DbgWnd;
extern void PutLogList(const char * cMsg);
extern char G_cTxt[512];
extern char G_cData50000[50000];
extern void PutLogFileList(const char * cStr);
extern void PutAdminLogFileList(const char * cStr);
extern void PutItemLogFileList(const char * cStr);
extern void PutLogEventFileList(const char * cStr);
extern void PutHackLogFileList(const char * cStr);
extern void PutPvPLogFileList(const char * cStr);

// extern void PutDebugMsg(char * cStr);	// 2002-09-09 #2

extern FILE * pLogFile;
extern HWND G_hWnd;


constexpr int _tmp_iMoveLocX[9][37] = {
	// 0
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	// 1
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, -1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//2
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, -1},
	//3
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, -1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//4
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 19, 18, 17, 16,
		15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
		5, 4, 3, 2, 1, 0, -1},
	//5
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, -1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//6
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
		5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, -1},
	//7
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, -1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//8
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
		5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, -1}
};

constexpr int _tmp_iMoveLocY[9][37] = {
	// 0
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//1
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//2
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, -1},
	//3
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, -1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//4
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, -1},
	//5
	{15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, -1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	//6
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, -1},
	//7
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, -1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0},
	// 8
	{15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
		5, 4, 3, 2, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, -1}
};

extern bool G_bIsThread;
extern void ThreadProc(void *ch);

CGame::CGame(HWND hWnd) {
	int i, x;


	m_bIsGameStarted = false;
	m_hWnd = hWnd;
	m_pMainLogSock = nullptr;
	m_bIsLogSockAvailable = false;
	m_bIsItemAvailable = false;
	m_bIsBuildItemAvailable = false;
	m_bIsNpcAvailable = false;
	m_bIsMagicAvailable = false;
	m_bIsSkillAvailable = false;
	m_bIsQuestAvailable = false;
	m_bIsPortionAvailable = false;


	std::memset(m_cServerName, 0, sizeof (m_cServerName));

	m_iPlayerMaxLevel = DEF_PLAYERMAXLEVEL;

	for (i = 0; i < DEF_MAXCLIENTS; i++)
		m_pClientList[i] = nullptr;

	for (i = 0; i < DEF_MAXMAPS; i++)
		m_pMapList[i] = nullptr;

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		m_pItemConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		m_pNpcConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXNPCS; i++)
		m_pNpcList[i] = nullptr;

	for (i = 0; i < DEF_MSGQUENESIZE; i++)
		m_pMsgQuene[i] = nullptr;

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_pMagicConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_pSkillConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		m_pQuestConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		m_pDynamicObjectList[i] = nullptr;

	delayEvents_.reset();

	for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
		m_pNoticeMsgList[i] = nullptr;

	for (i = 0; i < DEF_MAXFISHS; i++)
		m_pFish[i] = nullptr;

	for (i = 0; i < DEF_MAXMINERALS; i++)
		m_pMineral[i] = nullptr;

	for (i = 0; i < DEF_MAXPORTIONTYPES; i++) {
		m_pPortionConfigList[i] = nullptr;
		m_pCraftingConfigList[i] = nullptr; // Crafting
	}

	for (i = 0; i < DEF_MAXSUBLOGSOCK; i++) {
		m_pSubLogSock[i] = nullptr;
		m_bIsSubLogSockAvailable[i] = false;
	}

	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		m_pBuildItemList[i] = nullptr;

	for (i = 0; i < DEF_MAXDUPITEMID; i++)
		m_pDupItemIDList[i] = nullptr;


	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		m_stPartyInfo[i].iTotalMembers = 0;
		for (x = 0; x < DEF_MAXPARTYMEMBERS; x++)
			m_stPartyInfo[i].iIndex[x] = 0;
	}

	m_iQueneHead = 0;
	m_iQueneTail = 0;



	m_iTotalClients = 0;
	m_iMaxClients = 0;
	m_iTotalMaps = 0;

	m_iTotalGameServerClients = 0;
	m_iTotalGameServerMaxClients = 0;

	m_MaxUserSysTime.wHour = 0;
	m_MaxUserSysTime.wMinute = 0;

	m_bIsServerShutdowned = false;
	m_cShutDownCode = 0;

	m_iMiddlelandMapIndex = -1;
	m_iAresdenOccupyTiles = 0;
	m_iElvineOccupyTiles = 0;

	m_iCurMsgs = 0;
	m_iMaxMsgs = 0;


	m_stCityStatus[1].iCrimes = 0;
	m_stCityStatus[1].iFunds = 0;
	m_stCityStatus[1].iWins = 0;

	m_stCityStatus[2].iCrimes = 0;
	m_stCityStatus[2].iFunds = 0;
	m_stCityStatus[2].iWins = 0;

	m_iAutoRebootingCount = 0;
	m_bEnemyKillMode = false;
	m_iEnemyKillAdjust = 1;
	m_bAdminSecurity = true;
	m_sRaidTimeMonday = 0;
	m_sRaidTimeTuesday = 0;
	m_sRaidTimeWednesday = 0;
	m_sRaidTimeThursday = 0;
	m_sRaidTimeFriday = 0;
	m_sRaidTimeSaturday = 0;
	m_sRaidTimeSunday = 0;
	m_sCharPointLimit = 0;
	m_sSlateSuccessRate = 0;

	//Show Debug Window
	//DbgWnd = new CDebugWindow();
	//DbgWnd->Startup();
	//DbgWnd->AddEventMsg("CGame Startup");
	m_bReceivedItemList = false;

}

CGame::~CGame() {
	//DbgWnd->Shutdown();
	//delete DbgWnd;
}

bool CGame::bAccept(class XSocket * pXSock) {
	if ((m_bIsLogSockAvailable == false) ||
			  (m_bIsItemAvailable == false) || (m_bIsNpcAvailable == false) ||
			  (m_bIsMagicAvailable == false) || (m_bIsSkillAvailable == false) ||
			  (m_bIsPortionAvailable == false) || (m_bOnExitProcess == true) ||
			  (m_bIsQuestAvailable == false) || (m_bIsBuildItemAvailable == false) ||
			  (m_bIsGameStarted == false))
		goto CLOSE_ANYWAY;


	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] == nullptr) {
			std::unique_ptr<XSocket> socket(new class XSocket(m_hWnd, DEF_CLIENTSOCKETBLOCKLIMIT));
			socket->bInitBufferSize(DEF_MSGBUFFERSIZE);
			pXSock->bAccept(socket.get(), WM_ONCLIENTSOCKETEVENT + i);

			m_pClientList[i] = new class CClient(i, std::move(socket));
			CClient &client = *m_pClientList[i];
			bAddClientShortCut(i);
			client.m_dwSPTime = client.m_dwMPTime =
					  client.m_dwHPTime = client.m_dwAutoSaveTime =
					  client.m_dwTime = client.m_dwHungerTime = client.m_dwExpStockTime =
					  client.m_dwRecentAttackTime = client.m_dwAutoExpTime = client.m_dwSpeedHackCheckTime = timeGetTime();

			std::memset(client.m_cIPaddress, 0, sizeof (client.m_cIPaddress));
			client.m_pXSock->iGetPeerAddress(client.m_cIPaddress);

			wsprintf(G_cTxt, "<%d> Client Connected: (%s)", i, client.m_cIPaddress);
			PutLogList(G_cTxt);
			m_iTotalClients++;

			if (m_iTotalClients > m_iMaxClients) {

				m_iMaxClients = m_iTotalClients;
				//GetLocalTime(&m_MaxUserSysTime);
				//wsprintf(cTxt, "Maximum Players: %d", m_iMaxClients);
				//PutLogFileList(cTxt);
			}
			return true;
		}
	}
CLOSE_ANYWAY:
	;

	//std::unique_ptr<XSocket> socket = std::make_unique<XSocket>(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT)
	std::unique_ptr<XSocket> socket(new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT));
	pXSock->bAccept(socket.get(), 0);
	return false;
}

void CGame::OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	UINT iTmp;
	register int iClientH, iRet;
	uint32_t dwTime = timeGetTime();

	iTmp = WM_ONCLIENTSOCKETEVENT;
	iClientH = message - iTmp;
	CClient *clientPtr = m_pClientList[iClientH];
	if (clientPtr == nullptr) return;
	CClient &client = *clientPtr;
	iRet = client.m_pXSock->iOnSocketEvent(wParam, lParam);

	switch (iRet) {
		case DEF_XSOCKEVENT_READCOMPLETE:
			OnClientRead(client);
			client.m_dwTime = timeGetTime();
			break;

		case DEF_XSOCKEVENT_BLOCK:
			PutLogList("Socket BLOCKED!");
			break;

		case DEF_XSOCKEVENT_CONFIRMCODENOTMATCH:
			wsprintf(G_cTxt, "<%d> Confirmcode notmatch!", iClientH);
			PutLogList(G_cTxt);
			DeleteClient(iClientH, false, true);
			break;

		case DEF_XSOCKEVENT_MSGSIZETOOLARGE:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			wsprintf(G_cTxt, "<%d> Client Disconnected! (%s)", iClientH, client.m_cIPaddress);
			PutLogList(G_cTxt);
			if ((dwTime - client.m_dwLogoutHackCheck) < 1000) {
				wsprintf(G_cTxt, "Logout Hack: (%s) Player: (%s) - disconnected within 10 seconds of most recent damage. Hack? Lag?", client.m_cIPaddress, client.m_cCharName);
				PutHackLogFileList(G_cTxt);
			}

			DeleteClient(iClientH, true, true);
			break;
	}
}

bool CGame::bInit() {
	char cTxt[120];
	int i;
	SYSTEMTIME SysTime;
	uint32_t dwTime = timeGetTime();

	//m_Misc.Temp();

	PutLogList("(!) INITIALIZING GAME SERVER...");
	//

	for (i = 0; i < DEF_MAXCLIENTS + 1; i++)
		m_iClientShortCut[i] = 0;

	if (m_pMainLogSock != nullptr) delete m_pMainLogSock;

	for (i = 0; i < DEF_MAXSUBLOGSOCK; i++)
		if (m_pSubLogSock[i] != nullptr) delete m_pSubLogSock[i];

	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) delete m_pClientList[i];

	for (i = 0; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) delete m_pNpcList[i];

	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) delete m_pMapList[i];

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != nullptr) delete m_pItemConfigList[i];

	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		if (m_pNpcConfigList[i] != nullptr) delete m_pNpcConfigList[i];

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		if (m_pMagicConfigList[i] != nullptr) delete m_pMagicConfigList[i];

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		if (m_pSkillConfigList[i] != nullptr) delete m_pSkillConfigList[i];

	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != nullptr) delete m_pQuestConfigList[i];

	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] != nullptr) delete m_pDynamicObjectList[i];

	delayEvents_.reset();

	for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
		if (m_pNoticeMsgList[i] != nullptr) delete m_pNoticeMsgList[i];

	for (i = 0; i < DEF_MAXFISHS; i++)
		if (m_pFish[i] != nullptr) delete m_pFish[i];

	for (i = 0; i < DEF_MAXMINERALS; i++)
		if (m_pMineral[i] != nullptr) delete m_pMineral[i];

	for (i = 0; i < DEF_MAXPORTIONTYPES; i++) {
		if (m_pPortionConfigList[i] != nullptr) delete m_pPortionConfigList[i];
		if (m_pCraftingConfigList[i] != nullptr) delete m_pCraftingConfigList[i]; // Crafting
	}

	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		if (m_pBuildItemList[i] != nullptr) delete m_pBuildItemList[i];

	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		m_iNpcConstructionPoint[i] = 0;


	for (i = 0; i < DEF_MAXSCHEDULE; i++) {
		m_stCrusadeWarSchedule[i].iDay = -1;
		m_stCrusadeWarSchedule[i].iHour = -1;
		m_stCrusadeWarSchedule[i].iMinute = -1;
	}

	for (i = 0; i < DEF_MAXHELDENIAN; i++) {
		m_stHeldenianSchedule[i].iDay = -1;
		m_stHeldenianSchedule[i].StartiHour = -1;
		m_stHeldenianSchedule[i].StartiMinute = -1;
		m_stHeldenianSchedule[i].EndiHour = -1;
		m_stHeldenianSchedule[i].EndiMinute = -1;
	}


	m_iNpcConstructionPoint[1] = 100; // MS
	m_iNpcConstructionPoint[2] = 100; // MS
	m_iNpcConstructionPoint[3] = 100; // MS
	m_iNpcConstructionPoint[4] = 100; // MS
	m_iNpcConstructionPoint[5] = 100; // MS
	m_iNpcConstructionPoint[6] = 100; // MS

	m_iNpcConstructionPoint[43] = 1000; // LWB
	m_iNpcConstructionPoint[44] = 2000; // GHK
	m_iNpcConstructionPoint[45] = 3000; // GHKABS
	m_iNpcConstructionPoint[46] = 2000; // TK
	m_iNpcConstructionPoint[47] = 3000; // BG
	m_iNpcConstructionPoint[51] = 1500; // Catapult
	//

	// Majestic Code By Diuuude
	ReadMajesticSettings();

	m_bIsGameStarted = false;

	m_pMainLogSock = nullptr;
	m_bIsLogSockAvailable = false;
	m_bIsItemAvailable = false;
	m_bIsBuildItemAvailable = false;
	m_bIsNpcAvailable = false;
	m_bIsMagicAvailable = false;
	m_bIsSkillAvailable = false;
	m_bIsQuestAvailable = false;
	m_bIsPortionAvailable = false;

	std::memset(m_cServerName, 0, sizeof (m_cServerName));

	for (i = 0; i < DEF_MAXCLIENTS; i++)
		m_pClientList[i] = nullptr;

	for (i = 0; i < DEF_MAXMAPS; i++)
		m_pMapList[i] = nullptr;

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		m_pItemConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		m_pNpcConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXNPCS; i++)
		m_pNpcList[i] = nullptr;

	for (i = 0; i < DEF_MSGQUENESIZE; i++)
		m_pMsgQuene[i] = nullptr;

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_pMagicConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_pSkillConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		m_pQuestConfigList[i] = nullptr;

	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		m_pDynamicObjectList[i] = nullptr;

	delayEvents_.reset();

	for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
		m_pNoticeMsgList[i] = nullptr;

	for (i = 0; i < DEF_MAXFISHS; i++)
		m_pFish[i] = nullptr;

	for (i = 0; i < DEF_MAXMINERALS; i++)
		m_pMineral[i] = nullptr;

	for (i = 0; i < DEF_MAXPORTIONTYPES; i++) {
		m_pPortionConfigList[i] = nullptr;
		m_pCraftingConfigList[i] = nullptr; // Crafting
	}

	for (i = 0; i < DEF_MAXSUBLOGSOCK; i++) {
		m_pSubLogSock[i] = nullptr;
		m_bIsSubLogSockAvailable[i] = false;
	}

	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		m_pBuildItemList[i] = nullptr;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		std::memset(m_stCrusadeStructures[i].cMapName, 0, sizeof (m_stCrusadeStructures[i].cMapName));
		m_stCrusadeStructures[i].cType = 0;
		m_stCrusadeStructures[i].dX = 0;
		m_stCrusadeStructures[i].dY = 0;
	}

	for (i = 0; i < DEF_MAXADMINS; i++) {
		std::memset(m_stAdminList[i].m_cGMName, 0, sizeof (m_stAdminList[i].m_cGMName));
	}

	for (i = 0; i < DEF_MAXBANNED; i++) {
		std::memset(m_stBannedList[i].m_cBannedIPaddress, 0, sizeof (m_stBannedList[i].m_cBannedIPaddress));
	}

	for (i = 0; i < DEF_MAXGUILDS; i++)
		m_pGuildTeleportLoc[i].m_iV1 = 0;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stMiddleCrusadeStructureInfo[i].cType = 0;
		m_stMiddleCrusadeStructureInfo[i].cSide = 0;
		m_stMiddleCrusadeStructureInfo[i].sX = 0;
		m_stMiddleCrusadeStructureInfo[i].sY = 0;
	}
	m_iTotalMiddleCrusadeStructures = 0;

	m_pNoticementData = nullptr;

	m_iQueneHead = 0;
	m_iQueneTail = 0;

	m_iTotalClients = 0;
	m_iMaxClients = 0;
	m_iTotalMaps = 0;

	m_iTotalGameServerClients = 0;
	m_iTotalGameServerMaxClients = 0;

	m_MaxUserSysTime.wHour = 0;
	m_MaxUserSysTime.wMinute = 0;

	m_bIsServerShutdowned = false;
	m_cShutDownCode = 0;

	m_iMiddlelandMapIndex = -1;
	m_iAresdenMapIndex = -1;
	m_iElvineMapIndex = -1;
	m_iBTFieldMapIndex = -1;
	m_iGodHMapIndex = -1;

	m_iAresdenOccupyTiles = 0;
	m_iElvineOccupyTiles = 0;

	m_iSubLogSockInitIndex = 0;

	m_iCurMsgs = 0;
	m_iMaxMsgs = 0;

	m_stCityStatus[1].iCrimes = 0;
	m_stCityStatus[1].iFunds = 0;
	m_stCityStatus[1].iWins = 0;

	m_stCityStatus[2].iCrimes = 0;
	m_stCityStatus[2].iFunds = 0;
	m_stCityStatus[2].iWins = 0;

	m_iStrategicStatus = 0;

	m_iCollectedMana[0] = 0;
	m_iCollectedMana[1] = 0;
	m_iCollectedMana[2] = 0;

	m_iAresdenMana = 0;
	m_iElvineMana = 0;

	m_dwSpecialEventTime = m_dwWhetherTime = m_dwGameTime1 =
			  m_dwGameTime2 = m_dwGameTime3 = m_dwGameTime4 = m_dwGameTime5 = m_dwGameTime6 = m_dwFishTime = dwTime;

	m_bIsSpecialEventTime = false;

	GetLocalTime(&SysTime);

	m_dwCanFightzoneReserveTime = dwTime - ((SysTime.wHour % 2)*60 * 60 + SysTime.wMinute * 60) *1000;


	for (i = 0; i < DEF_MAXFIGHTZONE; i++)
		m_iFightZoneReserve[i] = 0;


	m_iFightzoneNoForceRecall = 0;


	for (i = 1; i < 3500; i++) {
		m_iLevelExpTable[i] = iGetLevelExp(i);
		//testcode
		//wsprintf(G_cTxt, "Level:%d --- Exp:%d", i, m_iLevelExpTable[i]);
		//PutLogFileList(G_cTxt);
	}

	m_iLimitedUserExp = m_iLevelExpTable[DEF_LEVELLIMIT + 1];
	m_iLevelExp20 = m_iLevelExpTable[20];

	m_iGameServerMode = 0;
	if (bReadProgramConfigFile("GServer.cfg") == false) {
		PutLogList(" ");
		PutLogList("(!!!) CRITICAL ERROR! Cannot execute server! GServer.cfg file contents error!");
		return false;
	}
	if (bReadSettingsConfigFile("..\\GameConfigs\\Settings.cfg") == false) {
		;
		PutLogList(" ");
		PutLogList("(!!!) CRITICAL ERROR! Cannot execute server! Settings.cfg file contents error!");
		return false;
	}

	if (bReadBallSystemConfigFile("..\\GameConfigs\\BallItems.cfg") == false) {
		PutLogList(" ");
		PutLogList("(!!!) CRITICAL ERROR! Cannot execute server! AdminSettings.cfg file contents error!");
		return false;
	}

	if (bReadAdminListConfigFile("..\\GameConfigs\\AdminList.cfg") == false) {
		;
		PutLogList(" ");
		PutLogList("(!!!) CRITICAL ERROR! Cannot execute server! AdminList.cfg file contents error!");
		return false;
	}
	if (bReadBannedListConfigFile("..\\GameConfigs\\BannedList.cfg") == false) {
		;
		PutLogList(" ");
		PutLogList("(!!!) CRITICAL ERROR! Cannot execute server! BannedList.cfg file contents error!");
		return false;
	}
	if (bReadAdminSetConfigFile("..\\GameConfigs\\AdminSettings.cfg") == false) {
		;
		PutLogList(" ");
		PutLogList("(!!!) CRITICAL ERROR! Cannot execute server! AdminSettings.cfg file contents error!");
		return false;
	}
	srand((unsigned) time(nullptr));

	m_pMainLogSock = new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	m_pMainLogSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_ONLOGSOCKETEVENT);
	m_pMainLogSock->bInitBufferSize(DEF_MSGBUFFERSIZE);

	wsprintf(cTxt, "(!) Try to Connect main-log-socket... Addr:%s  Port:%d", m_cLogServerAddr, m_iLogServerPort);
	PutLogList(cTxt);
	//Sleep(100);

	m_bF1pressed = m_bF4pressed = m_bF12pressed = m_bF5pressed = false;


	m_bOnExitProcess = false;


	for (i = 0; i <= 100; i++) {
		m_iSkillSSNpoint[i] = _iCalcSkillSSNpoint(i);
	}


	GetLocalTime(&SysTime);
	if (SysTime.wMinute >= DEF_NIGHTTIME)
		m_cDayOrNight = 2;
	else m_cDayOrNight = 1;


	bReadNotifyMsgListFile("notice.txt");
	m_dwNoticeTime = dwTime;

	m_iCurSubLogSockIndex = 0;
	m_iSubLogSockFailCount = 0;
	m_iSubLogSockActiveCount = 0;

	m_pNoticementData = nullptr;
	m_dwNoticementDataSize = 0;

	m_dwMapSectorInfoTime = dwTime;
	m_iMapSectorInfoUpdateCount = 0;

	m_iCrusadeCount = 0;
	m_bIsCrusadeMode = false;
	m_bIsApocalypseMode = false;

	m_dwCrusadeGUID = 0;
	m_iCrusadeWinnerSide = 0;
	m_iLastCrusadeWinner = 0;
	m_sLastCrusadeDate = -1;
	m_iFinalShutdownCount = 0;
	m_bIsCrusadeWarStarter = false;
	m_iLatestCrusadeDayOfWeek = -1;

	//Heldenian 3.00 Sources
	m_dwHeldenianGUID = 0;
	m_bHeldinianDuty = 0; // new
	m_bHeldenianDutyMultiplyer = 1; // new
	m_cHeldenianModeType = 0;
	m_bIsHeldenianMode = false;
	m_bHeldenianRunning = false;
	m_bHeldenianInitiated = false;
	m_iHeldenianAresdenLeftTower = 0;
	m_iHeldenianElvineLeftTower = 0;
	m_iHeldenianAresdenDead = 0;
	m_iHeldenianElvineDead = 0;
	m_bIsHeldenianReady = false; // new
	m_sLastHeldenianWinner = 0; // new
	std::memset(m_cHeldenianMapName, 0, sizeof (m_cHeldenianMapName)); // new

	//50Cent - Capture The Flag
	m_sFlagCountWin = 0;
	m_bIsCTFMode = false;
	m_bIsElvineFlagCaptured = false;
	m_bIsAresdenFlagCaptured = false;
	m_iFlagCarrierIndex = -1;

	return true;
}

void CGame::OnClientRead(CClient &client) {
	char * pData, cKey;
	uint32_t dwMsgSize;
	pData = client.m_pXSock->pGetRcvDataPointer(&dwMsgSize, &cKey); // v1.4

	if (bPutMsgQuene(DEF_MSGFROM_CLIENT, pData, dwMsgSize, client.id_, cKey) == false) {

		PutLogList("@@@@@@ CRITICAL ERROR in MsgQuene!!! @@@@@@");
	}
}

void CGame::DisplayInfo(HDC hdc) {
	char cTxt[350];
	int i, iLine;


	wsprintf(cTxt, "Server-Name(%s) Max.Level(%d) Players(%d/%d - %d/%d) Crusade(%d:%d) SLSock(%d:%d) RBT(%d)", m_cServerName, m_iPlayerMaxLevel, m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients, m_dwCrusadeGUID, (int) m_bIsCrusadeMode, m_iSubLogSockActiveCount, m_iSubLogSockFailCount, m_iAutoRebootingCount);
	TextOut(hdc, 5, 10, cTxt, strlen(cTxt));

	//#ifdef DEF_TESTSERVER
	//	std::memset(cTxt, 0, sizeof(cTxt));
	//	strcpy(cTxt, "인접한 클라이언트들에게 방향전환 이벤트를 알린다. ********** TEST SERVER MODE **********");
	//	TextOut(hdc, 5, 25, cTxt, strlen(cTxt));
	//#endif


	iLine = 0;
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) {

			std::memset(G_cTxt, 0, sizeof (G_cTxt));
			wsprintf(G_cTxt, "Map(%s)    Object(%d)    P(%d, %d)    N(%d, %d)    A(%d, %d)    E(%d, %d)    M(%d, %d)",
					  m_pMapList[i]->m_cName, m_pMapList[i]->m_iTotalActiveObject, m_pMapList[i]->m_iMaxPx * 20 + 10, m_pMapList[i]->m_iMaxPy * 20 + 10,
					  m_pMapList[i]->m_iMaxNx * 20 + 10, m_pMapList[i]->m_iMaxNy * 20 + 10, m_pMapList[i]->m_iMaxAx * 20 + 10, m_pMapList[i]->m_iMaxAy * 20 + 10,
					  m_pMapList[i]->m_iMaxEx * 20 + 10, m_pMapList[i]->m_iMaxEy * 20 + 10, m_pMapList[i]->m_iMaxMx * 20 + 10, m_pMapList[i]->m_iMaxMy * 20 + 10);

			TextOut(hdc, 5, 400 + iLine * 15, G_cTxt, strlen(G_cTxt));
			iLine++;
		}
}

void CGame::ClientMotionHandler(CClient &client, char * pData) {
	int iRet;

	if (client.m_bIsInitComplete == false) return;
	if (client.m_bIsKilled == true) return;
	client.m_dwLastActionTime = client.m_dwAFKCheckTime = timeGetTime();
	/*client.m_cConnectionCheck++;
	if (client.m_cConnectionCheck > 50) {
		wsprintf(G_cTxt, "Hex: (%s) Player: (%s) - removed 03203203h, vital to hack detection.", client.m_cIPaddress, client.m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, true, true);
		return;
	}*/
	uint16_t *wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	uint16_t wCommand = *wp;

	char *cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	short *sp = (short *) cp;
	short sX = *sp;
	cp += 2;

	sp = (short *) cp;
	short sY = *sp;
	cp += 2;

	char cDir = *cp;
	cp++;

	sp = (short *) cp;
	short dX = *sp;
	cp += 2;

	sp = (short *) cp;
	short dY = *sp;
	cp += 2;

	sp = (short *) cp;
	short wType = *sp;
	cp += 2;


	uint16_t wTargetObjectID = 0;
	if ((wCommand == DEF_OBJECTATTACK) || (wCommand == DEF_OBJECTATTACKMOVE)) {
		wp = (uint16_t *) cp;
		wTargetObjectID = *wp;
		cp += 2;
	}

	uint32_t *dwp = (uint32_t *) cp;
	uint32_t dwClientTime = *dwp;
	cp += 4;
	CheckDenialServiceAttack(client.id_, dwClientTime);
	int iClientH = client.id_;
	switch (wCommand) {
		case DEF_OBJECTSTOP:
			iRet = iClientMotion_Stop_Handler(client.id_, sX, sY, cDir);
			if (iRet == 1) {
				SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTSTOP, 0, 0, 0);
			} else if (iRet == 2) {
				SendObjectMotionRejectMsg(client.id_);
			}
			break;

		case DEF_OBJECTRUN:
			iRet = iClientMotion_Move_Handler(client.id_, sX, sY, cDir, 1);
			if (iRet == 1) {
				SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTRUN, 0, 0, 0);
				if (m_pMapList[client.m_cMapIndex]->bGetIsTeleport(client.m_sX, client.m_sY)) {
					RequestTeleportHandler(client.id_, "3");
				}
			}
			if ((m_pClientList[iClientH] != nullptr) && (client.m_iHP <= 0)) {
				ClientKilledHandler(iClientH, 0, 0, 1);
			}
			bCheckClientMoveFrequency(client.id_, dwClientTime);
			break;

		case DEF_OBJECTMOVE:
			iRet = iClientMotion_Move_Handler(client.id_, sX, sY, cDir, 2);
			if (iRet == 1) {
				SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
				if (m_pMapList[client.m_cMapIndex]->bGetIsTeleport(client.m_sX, client.m_sY)) {
					RequestTeleportHandler(client.id_, "3");
				}
			}
			if ((m_pClientList[iClientH] != nullptr) && (client.m_iHP <= 0)) {
				ClientKilledHandler(client.id_, 0, 0, 1);
			}
			bCheckClientMoveFrequency(client.id_, dwClientTime);
			break;

		case DEF_OBJECTDAMAGEMOVE:
			iRet = iClientMotion_Move_Handler(client.id_, sX, sY, cDir, 0);
			if (iRet == 1) {
				SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGEMOVE, client.m_iLastDamage, 0, 0);
				if (m_pMapList[client.m_cMapIndex]->bGetIsTeleport(client.m_sX, client.m_sY)) {
					RequestTeleportHandler(client.id_, "3");
				}
			}
			if ((m_pClientList[iClientH] != nullptr) && (client.m_iHP <= 0)) {
				ClientKilledHandler(iClientH, 0, 0, 1);
			}
			break;

		case DEF_OBJECTATTACKMOVE:
			iRet = iClientMotion_Move_Handler(client.id_, sX, sY, cDir, 0);
			if ((iRet == 1) && (m_pClientList[iClientH] != nullptr)) {
				SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTATTACKMOVE, 0, 0, 0);
				iClientMotion_Attack_Handler(client.id_, client.m_sX, client.m_sY, dX, dY, wType, cDir, wTargetObjectID, false, true);
				if (m_pMapList[client.m_cMapIndex]->bGetIsTeleport(client.m_sX, client.m_sY)) {
					RequestTeleportHandler(client.id_, "3");
				}
			}
			if ((m_pClientList[iClientH] != nullptr) && (client.m_iHP <= 0)) {
				ClientKilledHandler(client.id_, 0, 0, 1);
			}
			bCheckClientAttackFrequency(client.id_, dwClientTime);
			break;

		case DEF_OBJECTATTACK:
			_CheckAttackType(client.id_, &wType);
			iRet = iClientMotion_Attack_Handler(client.id_, sX, sY, dX, dY, wType, cDir, wTargetObjectID);
			if (iRet == 1) {
				if (wType >= 20) {
					client.m_iSuperAttackLeft--;
					if (client.m_iSuperAttackLeft < 0) client.m_iSuperAttackLeft = 0;
				}

				SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, wType);
			} else if (iRet == 2) SendObjectMotionRejectMsg(client.id_);
			bCheckClientAttackFrequency(client.id_, dwClientTime);
			break;

		case DEF_OBJECTGETITEM:
			iRet = iClientMotion_GetItem_Handler(client.id_, sX, sY, cDir);
			if (iRet == 1) {
				SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTGETITEM, 0, 0, 0);
			} else if (iRet == 2) SendObjectMotionRejectMsg(client.id_);
			break;

		case DEF_OBJECTMAGIC:
			iRet = iClientMotion_Magic_Handler(client.id_, sX, sY, cDir);
			//client hp recorded here ONLY if its less than
			if (iRet == 1) {
				if (client.m_bMagicPauseTime == false) {
					client.m_bMagicPauseTime = true;
					SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTMAGIC, dX, 10, 0);
					client.m_iSpellCount++;
					bCheckClientMagicFrequency(client.id_, dwClientTime);
				} else if (client.m_bMagicPauseTime == true) {
					wsprintf(G_cTxt, "Cast Delay Hack: (%s) Player: (%s) - player casting too fast.", client.m_cIPaddress, client.m_cCharName);
					PutHackLogFileList(G_cTxt);
					DeleteClient(client.id_, true, true);
				}
			} else if (iRet == 2) SendObjectMotionRejectMsg(client.id_);
			break;

		default:
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType)
//  description			:: Handles how player or npc run, walk, attack, or get flown by attack
//  last updated		:: October 30, 2004; 1:52 AM; Hypnotoad
//	return value		:: int
//  commentary			:: - contains speed hack detection previously unavailable
//						   - changed variable 5 to char, prior bool.
//								1 = object run
//								2 = object malk
//								0 = object damage move, object attack move
//						   - fixed bump bug removing aura
/////////////////////////////////////////////////////////////////////////////////////
// Missing 3.51:
//			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->3CA18h
// 			bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->sub_4C0F20(dX, dY, cTemp, wV1, wV2);
//
/////////////////////////////////////////////////////////////////////////////////////

int CGame::iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType) {
	char * cp, cData[3000];
	class CTile * pTile;
	uint32_t * dwp;
	uint16_t * wp, wObjectID;
	short * sp, dX, dY, sTemp, sTemp2, sDOtype, pTopItem;
	int * ip, iRet, iSize, iDamage;
	bool bRet, bIsBlocked = false;

	if (m_pClientList[iClientH] == nullptr) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	/*dwTime = timeGetTime();
	m_pClientList[iClientH]->m_dwLastActionTime = dwTime;
	if (cMoveType == 2) {
		if (m_pClientList[iClientH]->m_iRecentWalkTime > dwTime) {
			m_pClientList[iClientH]->m_iRecentWalkTime = dwTime;
			if (m_pClientList[iClientH]->m_sV1 < 1) {
				if (m_pClientList[iClientH]->m_iRecentWalkTime < dwTime) {
					m_pClientList[iClientH]->m_sV1++;
				}
				else {
					bIsBlocked = true;
					m_pClientList[iClientH]->m_sV1 = 0;
				}
			}
		m_pClientList[iClientH]->m_iRecentWalkTime = dwTime;
		}
		if (bIsBlocked == false) m_pClientList[iClientH]->m_iMoveMsgRecvCount++;
		if (m_pClientList[iClientH]->m_iMoveMsgRecvCount >= 3) {
			if (m_pClientList[iClientH]->m_dwMoveLAT != 0) {
				if ((dwTime - m_pClientList[iClientH]->m_dwMoveLAT) < (590)) {
					//wsprintf(G_cTxt, "3.51 Walk Speeder: (%s) Player: (%s) walk difference: %d. Speed Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, dwTime - m_pClientList[iClientH]->m_dwMoveLAT);
					//PutHackLogFileList(G_cTxt);
					bIsBlocked = true;
				}
			}
			m_pClientList[iClientH]->m_dwMoveLAT = dwTime;
			m_pClientList[iClientH]->m_iMoveMsgRecvCount = 0;
		}
	}
	else if (cMoveType == 1) {
		if (m_pClientList[iClientH]->m_iRecentRunTime > dwTime) {
			m_pClientList[iClientH]->m_iRecentRunTime = dwTime;
			if (m_pClientList[iClientH]->m_sV1 < 1) {
				if (m_pClientList[iClientH]->m_iRecentRunTime < dwTime) {
					m_pClientList[iClientH]->m_sV1++;
				}
				else {
					bIsBlocked = true;
					m_pClientList[iClientH]->m_sV1 = 0;
				}
			}
		m_pClientList[iClientH]->m_iRecentRunTime = dwTime;
		}
		if (bIsBlocked == false) m_pClientList[iClientH]->m_iRunMsgRecvCount++;
		if (m_pClientList[iClientH]->m_iRunMsgRecvCount >= 3) {
			if (m_pClientList[iClientH]->m_dwRunLAT != 0) {
				if ((dwTime - m_pClientList[iClientH]->m_dwRunLAT) < (290)) {
					//wsprintf(G_cTxt, "3.51 Run Speeder: (%s) Player: (%s) run difference: %d. Speed Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, dwTime - m_pClientList[iClientH]->m_dwRunLAT);
					//PutHackLogFileList(G_cTxt);
					bIsBlocked = true;
				}
			}
			m_pClientList[iClientH]->m_dwRunLAT	= dwTime;
			m_pClientList[iClientH]->m_iRunMsgRecvCount = 0;
		}
	}*/

	int iStX, iStY;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != nullptr) {
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (m_pClientList[iClientH]->m_cSide) {
			case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}

	ClearSkillUsingStatus(iClientH);

	dX = m_pClientList[iClientH]->m_sX;
	dY = m_pClientList[iClientH]->m_sY;

	switch (cDir) {
		case 1: dY--;
			break;
		case 2: dX++;
			dY--;
			break;
		case 3: dX++;
			break;
		case 4: dX++;
			dY++;
			break;
		case 5: dY++;
			break;
		case 6: dX--;
			dY++;
			break;
		case 7: dX--;
			break;
		case 8: dX--;
			dY--;
			break;
	}

	pTopItem = 0;
	bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(dX, dY, &sDOtype, &pTopItem);

	if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
		bRet = false;

	if ((bRet == true) && (bIsBlocked == false)) {
		if (m_pClientList[iClientH]->m_iQuest) _bCheckIsQuestCompleted(iClientH);

		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(1, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);

		m_pClientList[iClientH]->m_sX = dX;
		m_pClientList[iClientH]->m_sY = dY;
		m_pClientList[iClientH]->m_cDir = cDir;

		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner((short) iClientH,
				  DEF_OWNERTYPE_PLAYER,
				  dX, dY);

		if (sDOtype == DEF_DYNAMICOBJECT_SPIKE) {
			if ((m_pClientList[iClientH]->m_bIsNeutral == true) && ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) == 0)) {

			} else {
				iDamage = iDice(2, 4);

				if (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
					m_pClientList[iClientH]->m_iHP -= iDamage;
			}
		}

		if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 0;

		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_MOTION;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_OBJECTMOVE_CONFIRM;

		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

		sp = (short *) cp;
		*sp = (short) (dX - 10);
		cp += 2;

		sp = (short *) cp;
		*sp = (short) (dY - 7);
		cp += 2;

		*cp = cDir;
		cp++;

		if (cMoveType == 1) {
			if (m_pClientList[iClientH]->m_iSP > 0) {
				*cp = 0;
				if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0) {
					m_pClientList[iClientH]->m_iSP--;
					*cp = 1;
				}
			} else {
				*cp = 0;
				if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0) {
					m_pClientList[iClientH]->m_iSP--;
					*cp = 1;
				}
				if (m_pClientList[iClientH]->m_iSP < -10) {
					m_pClientList[iClientH]->m_iSP = 0;
					DeleteClient(iClientH, true, true);
					return 0;
				}
			}
		} else *cp = 0;
		cp++;

		pTile = (class CTile *)(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_pTile + dX + dY * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);
		*cp = (char) pTile->m_iOccupyStatus;
		cp++;

		ip = (int *) cp;
		*ip = m_pClientList[iClientH]->m_iHP;
		cp += 4;

		iSize = iComposeMoveMapData((short) (dX - 10), (short) (dY - 7), iClientH, cDir, cp);
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, iSize + 12 + 1 + 4);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, true, true);
				return 0;
		}
		/*if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->3CA18h == true) {

			.text:00406037                 mov     [ebp+var_C1C], 0
			.text:0040603E                 xor     edx, edx
			.text:00406040                 mov     [ebp+var_C1B], edx
			.text:00406046                 mov     [ebp+var_C17], edx
			.text:0040604C                 mov     [ebp+var_C13], dx

			bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->sub_4C0F20(dX, dY, cTemp, wV1, wV2);
			if (bRet == 1) {
				RequestTeleportHandler(iClientH, "2   ", cTemp, wV1, wV2);
			}
		}*/
	} else {
		m_pClientList[iClientH]->m_bIsMoveBlocked = true;

		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_MOTION;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_OBJECTMOVE_REJECT;
		if (bIsBlocked == true) {
			m_pClientList[iClientH]->m_dwAttackLAT = 1050;
		}
		m_pClientList[iClientH]->m_dwAttackLAT = 1010;

		wObjectID = (uint16_t) iClientH;

		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

		wp = (uint16_t *) cp;
		*wp = wObjectID;
		cp += 2;

		sp = (short *) cp;
		sX = m_pClientList[wObjectID]->m_sX;
		*sp = sX;
		cp += 2;

		sp = (short *) cp;
		sY = m_pClientList[wObjectID]->m_sY;
		*sp = sY;
		cp += 2;

		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sType;
		cp += 2;

		*cp = m_pClientList[wObjectID]->m_cDir;
		cp++;

		memcpy(cp, m_pClientList[wObjectID]->m_cCharName, 10);
		cp += 10;

		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr1;
		cp += 2;

		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr2;
		cp += 2;

		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr3;
		cp += 2;

		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr4;
		cp += 2;

		ip = (int *) cp;
		*ip = m_pClientList[wObjectID]->m_iApprColor;
		cp += 4;

		ip = (int *) cp;
		sTemp = m_pClientList[wObjectID]->m_iStatus;
		sTemp = 0x0FFFFFFF & sTemp;
		sTemp2 = iGetPlayerABSStatus(wObjectID, iClientH);
		sTemp = (sTemp | (sTemp2 << 28));
		*ip = sTemp;
		cp += 4;

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 42);

		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, true, true);
				return 0;
		}
		return 0;
	}
	//SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	return 1;
}

void CGame::RequestInitPlayerHandler(CClient &client, char * pData, char cKey) {
	register int i;
	char * cp, cCharName[11], cAccountName[11], cAccountPassword[11], cTxt[120];
	bool bIsObserverMode;

	if (client.m_bIsInitComplete == true) return;
	std::memset(cCharName, 0, sizeof (cCharName));
	std::memset(cAccountName, 0, sizeof (cAccountName));
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));

	std::memset(client.m_cCharName, 0, sizeof (client.m_cCharName));
	std::memset(client.m_cAccountName, 0, sizeof (client.m_cAccountName));
	std::memset(client.m_cAccountPassword, 0, sizeof (client.m_cAccountPassword));

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	memcpy(cCharName, cp, 10);
	cp += 10;

	std::memset(cTxt, 0, sizeof (cTxt));
	memcpy(cTxt, cCharName, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cCharName, 0, sizeof (cCharName));
	memcpy(cCharName, cTxt, 10);

	if (strlen(cTxt) == 0) PutLogList("RIPH - cTxt: Char nullptr!");

	memcpy(cAccountName, cp, 10);
	cp += 10;

	std::memset(cTxt, 0, sizeof (cTxt));
	memcpy(cTxt, cAccountName, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cAccountName, 0, sizeof (cAccountName));
	memcpy(cAccountName, cTxt, 10);

	memcpy(cAccountPassword, cp, 10);
	cp += 10;

	std::memset(cTxt, 0, sizeof (cTxt));
	memcpy(cTxt, cAccountPassword, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));
	memcpy(cAccountPassword, cTxt, 10);

	bIsObserverMode = (bool) * cp;
	cp++;

	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (client.id_ != i) && (memcmp(m_pClientList[i]->m_cAccountName, cAccountName, 10) == 0)) {
			if (memcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword, 10) == 0) {
				wsprintf(G_cTxt, "<%d> Duplicate account player! Deleted with data save : CharName(%s) AccntName(%s) IP(%s)", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cAccountName, m_pClientList[i]->m_cIPaddress);
				PutLogList(G_cTxt);
				DeleteClient(i, true, true, false);
			} else {
				memcpy(client.m_cCharName, cCharName, 10);
				memcpy(client.m_cAccountName, cAccountName, 10);
				memcpy(client.m_cAccountPassword, cAccountPassword, 10);
				DeleteClient(client.id_, false, false, false);
				return;
			}
		}
	}

	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (client.id_ != i) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)) {
			if (memcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword, 10) == 0) {
				wsprintf(G_cTxt, "<%d> Duplicate player! Deleted with data save : CharName(%s) IP(%s)", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cIPaddress);
				PutLogList(G_cTxt);
				//PutLogFileList(G_cTxt);
				DeleteClient(i, true, true, false);
			} else {
				memcpy(client.m_cCharName, cCharName, 10);
				memcpy(client.m_cAccountName, cAccountName, 10);
				memcpy(client.m_cAccountPassword, cAccountPassword, 10);
				DeleteClient(client.id_, false, false);
				return;
			}
		}
	}

	memcpy(client.m_cCharName, cCharName, 10);
	memcpy(client.m_cAccountName, cAccountName, 10);
	memcpy(client.m_cAccountPassword, cAccountPassword, 10);

	client.m_bIsObserverMode = bIsObserverMode;
	bSendMsgToLS(MSGID_REQUEST_PLAYERDATA, client.id_);
}

// 05/22/2004 - Hypnotoad - sends client to proper location after dieing

void CGame::RequestInitDataHandler(int iClientH, char * pData, char cKey) {
	bool bFlag;
	char * pBuffer = nullptr;
	short * sp;
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cPlayerName[11], cTxt[120];
	int sSummonPoints;
	int * ip, i, iTotalItemA, iTotalItemB, iSize, iRet, iStats;
	SYSTEMTIME SysTime;

	if (m_pClientList[iClientH] == nullptr) return;

	pBuffer = new char [DEF_MSGBUFFERSIZE + 1];
	ZeroMemory(pBuffer, DEF_MSGBUFFERSIZE + 1);


	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cPlayerName, 0, sizeof (cPlayerName));
	memcpy(cPlayerName, cp, 10);

	std::memset(cTxt, 0, sizeof (cTxt)); // v1.4
	memcpy(cTxt, cPlayerName, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cPlayerName, 0, sizeof (cPlayerName));
	memcpy(cPlayerName, cTxt, 10);


	if (memcmp(m_pClientList[iClientH]->m_cCharName, cPlayerName, 10) != 0) {
		DeleteClient(iClientH, false, true);
		return;
	}


	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_PLAYERCHARACTERCONTENTS; // 0x0Fm_cHeldenianVictoryType000 = 262406144
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;

	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iHP;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iMP;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iSP;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iDefenseRatio;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iHitRatio;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iLevel;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iStr;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iInt;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iVit;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iDex;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iMag;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iCharisma;
	cp += 4;

	iStats = (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iVit +
			  m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma);

	m_pClientList[iClientH]->m_iLU_Pool = m_pClientList[iClientH]->m_iLevel * 3 - (iStats - 70);
	wp = (uint16_t *) cp;
	//*wp = m_pClientList[iClientH]->m_iLevel*3 - (iStats - 70);
	*wp = m_pClientList[iClientH]->m_iLU_Pool;
	cp += 2;

	*cp = m_pClientList[iClientH]->m_cVar;
	cp++;

	*cp = 0;
	cp++;

	*cp = 0;
	cp++;

	*cp = 0;
	cp++;

	*cp = 0;
	cp++;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iExp;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iEnemyKillCount;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iPKCount;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iRewardGold;
	cp += 4;

	memcpy(cp, m_pClientList[iClientH]->m_cLocation, 10);
	cp += 10;

	memcpy(cp, m_pClientList[iClientH]->m_cGuildName, 20);
	cp += 20;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iGuildRank;
	cp += 4;

	// v1.4311
	*cp = (char) m_pClientList[iClientH]->m_iSuperAttackLeft;
	cp++;


	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iFightzoneNumber;
	cp += 4;

	//Syntax : ======HP==MP==SP==DRatHRatLVL=STR=INT=VIT=DEX=MAG=CHR=LUstatEXP=EK==PK==RewaLocation==GuildName=RankAF
	//Syntax : 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
	//Syntax : ......145212521152........376.200=200=200=200=200=195=......big.8...17......aresden...NONE......NONE30
	// 0x0Fm_cHeldenianVictoryType000 = 262406144


	//Debug Event
	//DbgWnd->AddEventMsg(MSG_SEND,pBuffer,180,0);

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 118); // Original : 115
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			DeleteClient(iClientH, true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}


	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_PLAYERITEMLISTCONTENTS;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;



	bFlag = false; // Kaozures - Adaptado darmart123.
	while (bFlag == false) {
		bFlag = true;
		for (i = 0; i < DEF_MAXITEMS - 1; i++)
			if ((m_pClientList[iClientH]->m_pItemList[i] == nullptr) && (m_pClientList[iClientH]->m_pItemList[i + 1] != nullptr)) {
				std::swap(m_pClientList[iClientH]->m_pItemList[i], m_pClientList[iClientH]->m_pItemList[i + 1]);
				m_pClientList[iClientH]->m_ItemPosList[i].x = m_pClientList[iClientH]->m_ItemPosList[i + 1].x;
				m_pClientList[iClientH]->m_ItemPosList[i].y = m_pClientList[iClientH]->m_ItemPosList[i + 1].y;
				bFlag = false;
			}
	}
	iTotalItemA = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr)
			iTotalItemA++;

	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);

	*cp = iTotalItemA;
	cp++;

	for (i = 0; i < iTotalItemA; i++) {
		// ### ERROR POINT!!!
		if (m_pClientList[iClientH]->m_pItemList[i] == nullptr) {

			wsprintf(G_cTxt, "RequestInitDataHandler error: Client(%s) Item(%d)", m_pClientList[iClientH]->m_cCharName, i);
			PutLogFileList(G_cTxt);

			DeleteClient(iClientH, false, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
		}
		memcpy(cp, m_pClientList[iClientH]->m_pItemList[i]->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = m_pClientList[iClientH]->m_pItemList[i]->m_dwCount;
		cp += 4;
		*cp = m_pClientList[iClientH]->m_pItemList[i]->m_cItemType;
		cp++;
		*cp = m_pClientList[iClientH]->m_pItemList[i]->m_cEquipPos;
		cp++;
		*cp = (char) m_pClientList[iClientH]->m_bIsItemEquipped[i];
		cp++;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_pItemList[i]->m_sLevelLimit;
		cp += 2;
		*cp = m_pClientList[iClientH]->m_pItemList[i]->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = m_pClientList[iClientH]->m_pItemList[i]->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_pItemList[i]->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_pItemList[i]->m_sSpriteFrame;
		cp += 2;
		*cp = m_pClientList[iClientH]->m_pItemList[i]->m_cItemColor; // v1.4
		cp++;
		*cp = (char) m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = m_pClientList[iClientH]->m_pItemList[i]->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(m_pClientList[iClientH]->m_pItemList[i]->m_dwAttribute & 0x00000001); 
		cp++;
		 */
	}



	iTotalItemB = 0;
	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] != nullptr)
			iTotalItemB++;

	*cp = iTotalItemB;
	cp++;

	for (i = 0; i < iTotalItemB; i++) {

		if (m_pClientList[iClientH]->m_pItemInBankList[i] == nullptr) {

			wsprintf(G_cTxt, "RequestInitDataHandler error: Client(%s) Bank-Item(%d)", m_pClientList[iClientH]->m_cCharName, i);
			PutLogFileList(G_cTxt);

			DeleteClient(iClientH, false, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
		}
		memcpy(cp, m_pClientList[iClientH]->m_pItemInBankList[i]->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwCount;
		cp += 4;
		*cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cItemType;
		cp++;
		*cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cEquipPos;
		cp++;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_sLevelLimit;
		cp += 2;
		*cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_sSpriteFrame;
		cp += 2;
		*cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cItemColor; // v1.4
		cp++;
		*cp = (char) m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwAttribute & 0x00000001); 
		cp++;
		 */
	}

	for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
		*cp = m_pClientList[iClientH]->m_cMagicMastery[i];
		cp++;
	}

	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		*cp = m_pClientList[iClientH]->m_cSkillMastery[i];
		cp++;
	}


	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 6 + 1 + iTotalItemA * 44 + iTotalItemB * 43 + DEF_MAXMAGICTYPE + DEF_MAXSKILLTYPE);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			DeleteClient(iClientH, true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}


	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITDATA;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;

	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);


	if (m_pClientList[iClientH]->m_bIsObserverMode == false)
		bGetEmptyPosition(&m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cMapIndex);
	else GetMapInitialPoint(m_pClientList[iClientH]->m_cMapIndex, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY);

	// ObjectID
	wp = (uint16_t *) cp;
	*wp = iClientH;
	cp += 2;


	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sX - 14 - 5;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sY - 12 - 5;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sType;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr1;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr2;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr3;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr4;
	cp += 2;

	ip = (int *) cp; // v1.4
	*ip = m_pClientList[iClientH]->m_iApprColor;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iStatus;
	cp += 4; // Original : 2


	memcpy(cp, m_pClientList[iClientH]->m_cMapName, 10);
	cp += 10;


	memcpy(cp, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10);
	cp += 10;


	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
		*cp = 1;
	else *cp = m_cDayOrNight;
	cp++;


	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
		*cp = 0;
	else *cp = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus;
	cp++;

	// v1.4 Contribution
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iContribution;
	cp += 4;




	if (m_pClientList[iClientH]->m_bIsObserverMode == false) {
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH,
				  DEF_OWNERTYPE_PLAYER,
				  m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY);
	}

	// v1.41
	*cp = (char) m_pClientList[iClientH]->m_bIsObserverMode;
	cp++;
	// catches debug on player load up

	// v1.41
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iRating;
	cp += 4;

	// v1.44
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iHP;
	cp += 4;

	//Unknown variable
	*cp = 0;
	cp++;


	iSize = iComposeInitMapData(m_pClientList[iClientH]->m_sX - 10, m_pClientList[iClientH]->m_sY - 7, iClientH, cp);


	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 46 + iSize + 4 + 4 + 1 + 4 + 4 + 3); // Zabuza fix
	//iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 59 + iSize +4 +4 +1 +4 +4); // v1.41
	//	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 46 + iSize +4 +4 +1 +4 +4); // v1.41
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			DeleteClient(iClientH, true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}

	if (pBuffer != nullptr) delete pBuffer;


	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);

	// �÷��̾ �ٸ� �ʿ� ����. ���� ���̶�� �ð��� �����Ѵ�.

	if ((memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) &&
			  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)
			  && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {

		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
		m_pClientList[iClientH]->m_bIsWarLocation = true;
		// v2.17 2002-7-15 ���� �ð��� ���� �� �� �ְ� �����Ѵ�.
		SetForceRecallTime(iClientH);
	} else if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0) &&
			  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)
			  && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {

		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
		m_pClientList[iClientH]->m_bIsWarLocation = true;

		// v2.17 2002-7-15 ���� �ð��� ���� �� �� �ְ� �����Ѵ�.
		SetForceRecallTime(iClientH);
	} else if (((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "arejail", 7) == 0) ||
			  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvjail", 7) == 0))
			  && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		m_pClientList[iClientH]->m_bIsWarLocation = true;
		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();

		// v2.17 2002-7-15
		if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0) {
			m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * 5;
			// ���ݽð��� 5�� ���� ���
		} else if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 20 * 5) {
			m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * 5; // 5��
		}

	} else if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true) &&
			  (m_iFightzoneNoForceRecall == 0) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {


		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
		m_pClientList[iClientH]->m_bIsWarLocation = true;

		// �������� �� �ִ� ���� �ð��� ������ �ش�.

		// �������� �ð� ���̸� ����Ͽ� �����ð� 2�� �ִ�. �� 2�ð� �Ǳ� 2������� ���ݵȴ�.
		GetLocalTime(&SysTime);
		m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
	} else {
		m_pClientList[iClientH]->m_bIsWarLocation = false;
		// v1.42
		m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 0;
		// 06/11/2004
		SetForceRecallTime(iClientH);
	}

	// v2.17 2002-7-15 ���� �ð��� ����ڿ��� �����ش�.
	if ((m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0) && (m_pClientList[iClientH]->m_bIsWarLocation == true)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_FORCERECALLTIME, m_pClientList[iClientH]->m_iTimeLeft_ForceRecall, 0, 0, nullptr);
		wsprintf(G_cTxt, "(!) Game Server Force Recall Time  %d (%d)min", m_pClientList[iClientH]->m_iTimeLeft_ForceRecall, m_pClientList[iClientH]->m_iTimeLeft_ForceRecall / 20);
		PutLogList(G_cTxt);
	}


	// No entering enemy shops
	int iMapside, iMapside2;

	iMapside = iGetMapLocationSide(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName);
	if (iMapside > 3) iMapside2 = iMapside - 2;
	else iMapside2 = iMapside;
	m_pClientList[iClientH]->m_bIsInsideOwnTown = false;
	if ((m_pClientList[iClientH]->m_cSide != iMapside2) && (iMapside != 0)) {
		if ((iMapside <= 2) && (m_pClientList[iClientH]->m_iAdminUserLevel < 1)) {
			if (m_pClientList[iClientH]->m_cSide != 0) {
				m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
				m_pClientList[iClientH]->m_bIsWarLocation = true;
				m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 1;
				m_pClientList[iClientH]->m_bIsInsideOwnTown = true;
			}
		}
	} else {
		if (m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_bIsFightZone == true &&
				  m_iFightzoneNoForceRecall == false &&
				  m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
			m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
			m_pClientList[iClientH]->m_bIsWarLocation = true;
			GetLocalTime(&SysTime);
			m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
		} else {
			if (memcmp(m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_cLocationName, "arejail", 7) == 0 ||
					  memcmp(m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_cLocationName, "elvjail", 7) == 0) {
				if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
					m_pClientList[iClientH]->m_bIsWarLocation = true;
					m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
					if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
						m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
					else if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 100)
						m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
				}
			}
		}
	}

	/*if ((m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0) &&
		(m_pClientList[iClientH]->m_bIsWarLocation == true)) {
		SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_FORCERECALLTIME, m_pClientList[iClientH]->m_iTimeLeft_ForceRecall, nullptr, nullptr, nullptr);
	}*/

	if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft < 0) {
		m_pClientList[iClientH]->m_iGizonItemUpgradeLeft = 0;
	}

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, nullptr);
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, 0, 0, nullptr);
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMPOSLIST, 0, 0, 0, nullptr);

	_SendQuestContents(iClientH);
	_CheckQuestEnvironment(iClientH);

	// v1.432
	if (m_pClientList[iClientH]->m_iSpecialAbilityTime == 0) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, nullptr);
	}


	if (m_bIsCrusadeMode == true) {
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == 0) {

			m_pClientList[iClientH]->m_iCrusadeDuty = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
		} else if (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwCrusadeGUID) {



			m_pClientList[iClientH]->m_iCrusadeDuty = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			m_pClientList[iClientH]->m_iWarContribution = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;

			SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
		}
		m_pClientList[iClientH]->m_cVar = 1;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, 0, nullptr);
	} else if (m_bIsHeldenianMode == true) {
		sSummonPoints = m_pClientList[iClientH]->m_iCharisma * 300;
		if (sSummonPoints > 12000) sSummonPoints = 12000;
		if (m_pClientList[iClientH]->m_dwHeldenianGUID == 0) {
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwHeldenianGUID;
			m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
		} else if (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwHeldenianGUID) {
			m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
			m_pClientList[iClientH]->m_iWarContribution = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwHeldenianGUID;
		}
		m_pClientList[iClientH]->m_cWarType = 2;
		if (m_bIsHeldenianMode == true) {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
			if (m_bHeldenianInitiated == false) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
			}
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, nullptr);
			UpdateHeldenianStatus();
		}
	} else if ((m_pClientList[iClientH]->m_cVar == 1) && (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID)) {
		m_pClientList[iClientH]->m_iCrusadeDuty = 0;
		m_pClientList[iClientH]->m_iConstructionPoint = 0;
	} else {
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (m_pClientList[iClientH]->m_cVar == 1) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
			}
		} else {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
			m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
			m_pClientList[iClientH]->m_iWarContribution = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
		}
	}

	// v1.42
	if (memcmp(m_pClientList[iClientH]->m_cMapName, "fightzone", 9) == 0) {
		wsprintf(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_bIsObserverMode);
		PutLogEventFileList(G_cTxt);
	}

	if (m_bIsHeldenianMode == true) SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr, 0);
	if (m_bHeldenianInitiated == true) SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr, 0);

	// Crusade
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 1, nullptr);

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
	//Critical Count Fix.
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
}

int CGame::iComposeInitMapData(short sX, short sY, int iClientH, char * pData) {
	register int * ip, ix, iy, iSize, iTileExists;
	class CTile * pTileSrc, * pTile;
	unsigned char ucHeader;
	short * sp, * pTotal;
	int sTemp, sTemp2;
	uint16_t * wp;
	char * cp;

	if (m_pClientList[iClientH] == nullptr) return 0;

	pTotal = (short *) pData;
	cp = (char *) (pData + 2);

	iSize = 2;
	iTileExists = 0;
	pTileSrc = (class CTile *)(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_pTile +
			  (sX) + (sY) * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

	for (iy = 0; iy < 16; iy++)
		for (ix = 0; ix < 21; ix++) {

			if (((sX + ix) == 100) && ((sY + iy) == 100))
				sX = sX;

			pTile = (class CTile *)(pTileSrc + ix + iy * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

			//If player not same side and is invied (Beholder Hack)
			if ((m_pClientList[pTile->m_sOwner] != nullptr) && (pTile->m_sOwner != iClientH))
				if ((m_pClientList[pTile->m_sOwner]->m_cSide != 0) &&
						  (m_pClientList[pTile->m_sOwner]->m_cSide != m_pClientList[iClientH]->m_cSide) &&
						  ((m_pClientList[pTile->m_sOwner]->m_iStatus & 0x00000010) != 0)) {
					continue;
				}


			if ((pTile->m_sOwner != 0) || (pTile->m_sDeadOwner != 0) ||
					  (pTile->m_pItem[0] != nullptr) || (pTile->m_sDynamicObjectType != 0)) {
				iTileExists++;

				sp = (short *) cp;
				*sp = ix;
				cp += 2;
				sp = (short *) cp;
				*sp = iy;
				cp += 2;
				iSize += 4;


				ucHeader = 0;
				if (pTile->m_sOwner != 0) {


					if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER) {
						if (m_pClientList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
						else {
							// ###debugcode
							wsprintf(G_cTxt, "Empty player handle: %d", pTile->m_sOwner);
							//PutLogFileList(G_cTxt);
							//
							pTile->m_sOwner = 0;
						}
					}

					if (pTile->m_cOwnerClass == DEF_OWNERTYPE_NPC) {
						if (m_pNpcList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
						else pTile->m_sOwner = 0;
					}
				}
				if (pTile->m_sDeadOwner != 0) {
					if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_PLAYER) {
						if (m_pClientList[pTile->m_sDeadOwner] != nullptr) ucHeader = ucHeader | 0x02;
						else pTile->m_sDeadOwner = 0;
					}
					if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_NPC) {
						if (m_pNpcList[pTile->m_sDeadOwner] != nullptr) ucHeader = ucHeader | 0x02;
						else pTile->m_sDeadOwner = 0;
					}
				}
				if (pTile->m_pItem[0] != nullptr) ucHeader = ucHeader | 0x04;
				if (pTile->m_sDynamicObjectType != 0) ucHeader = ucHeader | 0x08;
				//
				*cp = ucHeader;
				cp++;
				iSize++;

				if ((ucHeader & 0x01) != 0) {
					switch (pTile->m_cOwnerClass) {
						case DEF_OWNERTYPE_PLAYER:
							// Object ID number(Player) : 1~10000
							sp = (short *) cp;
							*sp = pTile->m_sOwner;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = m_pClientList[pTile->m_sOwner]->m_cDir;
							cp++;
							iSize++;
							// Appearance1
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sOwner]->m_sAppr1;
							cp += 2;
							iSize += 2;
							// Appearance2
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Appearance3
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sOwner]->m_sAppr3;
							cp += 2;
							iSize += 2;
							// Appearance4
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sOwner]->m_sAppr4;
							cp += 2;
							iSize += 2;
							// v1.4 ApprColor
							ip = (int *) cp;
							*ip = m_pClientList[pTile->m_sOwner]->m_iApprColor;
							cp += 4;
							iSize += 4;

							// Status
							ip = (int *) cp;


							sTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = iGetPlayerABSStatus(pTile->m_sOwner, iClientH); //(short)iGetPlayerRelationship(iClientH, pTile->m_sOwner);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, m_pClientList[pTile->m_sOwner]->m_cCharName, 10);
							cp += 10;
							iSize += 10;
							break;

						case DEF_OWNERTYPE_NPC:
							// Object ID number(NPC) : 10000~
							sp = (short *) cp;
							*sp = pTile->m_sOwner + 10000;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = m_pNpcList[pTile->m_sOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = m_pNpcList[pTile->m_sOwner]->m_cDir;
							cp++;
							iSize++;
							// Appr2
							sp = (short *) cp;
							*sp = m_pNpcList[pTile->m_sOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Status
							ip = (int *) cp;

							sTemp = m_pNpcList[pTile->m_sOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = iGetNpcRelationship(pTile->m_sOwner, iClientH);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, m_pNpcList[pTile->m_sOwner]->m_cName, 5);
							cp += 5;
							iSize += 5;
							break;
					}
				}

				if ((ucHeader & 0x02) != 0) {

					switch (pTile->m_cDeadOwnerClass) {
						case DEF_OWNERTYPE_PLAYER:
							// Object ID number : 1~10000
							sp = (short *) cp;
							*sp = pTile->m_sDeadOwner;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sDeadOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = m_pClientList[pTile->m_sDeadOwner]->m_cDir;
							cp++;
							iSize++;
							// Appearance1
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr1;
							cp += 2;
							iSize += 2;
							// Appearance2
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Appearance3
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr3;
							cp += 2;
							iSize += 2;
							// Appearance4
							sp = (short *) cp;
							*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr4;
							cp += 2;
							iSize += 2;
							// v1.4 ApprColor
							ip = (int *) cp;
							*ip = m_pClientList[pTile->m_sDeadOwner]->m_iApprColor;
							cp += 4;
							iSize += 4;

							// Status
							ip = (int *) cp;


							sTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = iGetPlayerABSStatus(pTile->m_sDeadOwner, iClientH); //(short)iGetPlayerRelationship(iClientH, pTile->m_sDeadOwner);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, m_pClientList[pTile->m_sDeadOwner]->m_cCharName, 10);
							cp += 10;
							iSize += 10;
							break;

						case DEF_OWNERTYPE_NPC:
							// Object ID number : 10000	~
							sp = (short *) cp;
							*sp = pTile->m_sDeadOwner + 10000;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = m_pNpcList[pTile->m_sDeadOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = m_pNpcList[pTile->m_sDeadOwner]->m_cDir;
							cp++;
							iSize++;
							// Appr2
							sp = (short *) cp;
							*sp = m_pNpcList[pTile->m_sDeadOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Status
							ip = (int *) cp;

							sTemp = m_pNpcList[pTile->m_sDeadOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = iGetNpcRelationship(pTile->m_sDeadOwner, iClientH);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, m_pNpcList[pTile->m_sDeadOwner]->m_cName, 5);
							cp += 5;
							iSize += 5;
							break;
					}
				}

				if (pTile->m_pItem[0] != nullptr) {


					sp = (short *) cp;
					*sp = pTile->m_pItem[0]->m_sSprite;
					cp += 2;
					iSize += 2;

					sp = (short *) cp;
					*sp = pTile->m_pItem[0]->m_sSpriteFrame;
					cp += 2;
					iSize += 2;

					*cp = pTile->m_pItem[0]->m_cItemColor;
					cp++;
					iSize++;
				}

				if (pTile->m_sDynamicObjectType != 0) {

					wp = (uint16_t *) cp;
					*wp = pTile->m_wDynamicObjectID;
					cp += 2;
					iSize += 2;

					sp = (short *) cp;
					*sp = pTile->m_sDynamicObjectType;
					cp += 2;
					iSize += 2;
				}
			} // Big if
		} // while(1)

	*pTotal = iTileExists;
	return iSize;
}

void CGame::DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout, bool bForceCloseConn) {
	int i, iExH;
	char cTmpMap[30];

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == true) {
		if (memcmp(m_pClientList[iClientH]->m_cMapName, "fight", 5) == 0) {
			wsprintf(G_cTxt, "Char(%s)-Exit(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cMapName);
			PutLogEventFileList(G_cTxt);
		}

		if (m_pClientList[iClientH]->m_bIsExchangeMode == true) {
			iExH = m_pClientList[iClientH]->m_iExchangeH;
			_ClearExchangeStatus(iExH);
			_ClearExchangeStatus(iClientH);
		}

		if ((m_pClientList[iClientH]->m_iAllocatedFish != 0) && (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] != nullptr))
			m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;

		if (bNotify == true)
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);

		RemoveFromTarget(iClientH, DEF_OWNERTYPE_PLAYER);

		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iWhisperPlayerIndex == iClientH)) {
				m_pClientList[i]->m_iWhisperPlayerIndex = -1;
				SendNotifyMsg(0, i, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, m_pClientList[iClientH]->m_cCharName);
			}

		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(2, iClientH, DEF_OWNERTYPE_PLAYER,
				  m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY);

		delayEvents_.bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, 0);
	}

	if ((bSave == true) && (m_pClientList[iClientH]->m_bIsOnServerChange == false)) {

		if (m_pClientList[iClientH]->m_bIsKilled == true) {
			m_pClientList[iClientH]->m_sX = -1;
			m_pClientList[iClientH]->m_sY = -1;

			strcpy(cTmpMap, m_pClientList[iClientH]->m_cMapName);

			std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));

			if (m_pClientList[iClientH]->m_cSide == 0) {
				strcpy(m_pClientList[iClientH]->m_cMapName, "default");
			} else {
				if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) {
					if (m_bIsCrusadeMode == true) {
						if (m_pClientList[iClientH]->m_iDeadPenaltyTime > 0) {
							std::memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof (m_pClientList[iClientH]->m_cLockedMapName));
							strcpy(m_pClientList[iClientH]->m_cLockedMapName, "aresden");
							m_pClientList[iClientH]->m_iLockedMapTime = 60 * 5;
							m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
						} else {
							m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
						}
					}

					if (strcmp(cTmpMap, "elvine") == 0) {
						strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvjail");
						m_pClientList[iClientH]->m_iLockedMapTime = 10 * 2;
						memcpy(m_pClientList[iClientH]->m_cMapName, "elvjail", 7);
					} else if (m_pClientList[iClientH]->m_iLevel > 80)
						memcpy(m_pClientList[iClientH]->m_cMapName, "resurr1", 7);
					else memcpy(m_pClientList[iClientH]->m_cMapName, "resurr1", 7);
				} else {
					if (m_bIsCrusadeMode == true) {
						if (m_pClientList[iClientH]->m_iDeadPenaltyTime > 0) {
							std::memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof (m_pClientList[iClientH]->m_cLockedMapName));
							strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvine");
							m_pClientList[iClientH]->m_iLockedMapTime = 60 * 5;
							m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
						} else {
							m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
						}
					}
					if (strcmp(cTmpMap, "aresden") == 0) {
						strcpy(m_pClientList[iClientH]->m_cLockedMapName, "arejail");
						m_pClientList[iClientH]->m_iLockedMapTime = 10 * 2;
						memcpy(m_pClientList[iClientH]->m_cMapName, "arejail", 7);

					} else if (m_pClientList[iClientH]->m_iLevel > 80)
						memcpy(m_pClientList[iClientH]->m_cMapName, "resurr2", 7);
					else memcpy(m_pClientList[iClientH]->m_cMapName, "resurr2", 7);
				}
			}
		} else if (bForceCloseConn == true) {
			std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
			memcpy(m_pClientList[iClientH]->m_cMapName, "bisle", 5);
			m_pClientList[iClientH]->m_sX = -1;
			m_pClientList[iClientH]->m_sY = -1;

			std::memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof (m_pClientList[iClientH]->m_cLockedMapName));
			strcpy(m_pClientList[iClientH]->m_cLockedMapName, "bisle");
			m_pClientList[iClientH]->m_iLockedMapTime = 10 * 60;
		}

		if (m_pClientList[iClientH]->m_bIsObserverMode == true) {
			std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
			if (m_pClientList[iClientH]->m_cSide == 0) {
				switch (iDice(1, 2)) {
					case 1:
						memcpy(m_pClientList[iClientH]->m_cMapName, "aresden", 7);
						break;
					case 2:
						memcpy(m_pClientList[iClientH]->m_cMapName, "elvine", 6);
						break;
				}
			} else {
				memcpy(m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_cLocation, 10);
			}
			m_pClientList[iClientH]->m_sX = -1;
			m_pClientList[iClientH]->m_sY = -1;
		}

		if (memcmp(m_pClientList[iClientH]->m_cMapName, "fight", 5) == 0) {
			std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
			if (m_pClientList[iClientH]->m_cSide == 0) {
				switch (iDice(1, 2)) {
					case 1:
						memcpy(m_pClientList[iClientH]->m_cMapName, "aresden", 7);
						break;
					case 2:
						memcpy(m_pClientList[iClientH]->m_cMapName, "elvine", 6);
						break;
				}
			} else {
				memcpy(m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_cLocation, 10);
			}
			m_pClientList[iClientH]->m_sX = -1;
			m_pClientList[iClientH]->m_sY = -1;
		}

		if (m_pClientList[iClientH]->m_bIsInitComplete == true) {
			if (bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, iClientH, bCountLogout) == false) LocalSavePlayerData(iClientH);
		} else bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, iClientH, bCountLogout);
	} else {
		if (m_pClientList[iClientH]->m_bIsOnServerChange == false) {

			bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, iClientH, bCountLogout);
		} else {
			bSendMsgToLS(MSGID_REQUEST_SETACCOUNTWAITSTATUS, iClientH, false);
		}
	}

	if (m_pClientList[iClientH]->m_iPartyID != 0) {
		for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH) {
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = 0;
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers--;
				wsprintf(G_cTxt, "PartyID:%d member:%d Out(Delete) Total:%d", m_pClientList[iClientH]->m_iPartyID, iClientH, m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers);
				PutLogList(G_cTxt);
				goto DC_LOOPBREAK1;
			}
DC_LOOPBREAK1:
		;
		for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
			if ((m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] != 0)) {
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1];
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] = 0;
			}
	}

	m_iTotalClients--;

	//50Cent - Capture The Flag
	if (bCheckIfIsFlagCarrier(iClientH)) {
		SetFlagCarrierFlag(iClientH, false);
		SetIceFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
	}

	delete m_pClientList[iClientH];
	m_pClientList[iClientH] = nullptr;

	RemoveClientShortCut(iClientH);
}

void CGame::SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3) {
	int * ip, i, iShortCutIndex;
	char * cp_a, * cp_s, * cp_sv, cData_All[200], cData_Srt[200], cData_Srt_Av[200];
	uint32_t * dwp;
	uint16_t * wp;
	int * ipStatus, iDumm;
	short * sp, sRange, sX, sY;
	bool bFlag, cOwnerSend;
	char cKey;
	int iTemp3, iTemp, iTemp2;

	std::memset(cData_All, 0, sizeof (cData_All));
	std::memset(cData_Srt, 0, sizeof (cData_Srt));
	std::memset(cData_Srt_Av, 0, sizeof (cData_Srt_Av));
	ipStatus = (int *) &iDumm;
	cKey = (rand() % 255) + 1;

	dwp = (uint32_t *) (cData_All + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData_All + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;

	dwp = (uint32_t *) (cData_Srt + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData_Srt + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;

	dwp = (uint32_t *) (cData_Srt_Av + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData_Srt_Av + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;

	cp_a = (char *) (cData_All + DEF_INDEX2_MSGTYPE + 2);
	cp_s = (char *) (cData_Srt + DEF_INDEX2_MSGTYPE + 2);
	cp_sv = (char *) (cData_Srt_Av + DEF_INDEX2_MSGTYPE + 2);


	if ((dwMsgID == MSGID_EVENT_LOG) || (wMsgType == DEF_OBJECTMOVE) || (wMsgType == DEF_OBJECTRUN) ||
			  (wMsgType == DEF_OBJECTATTACKMOVE) || (wMsgType == DEF_OBJECTDAMAGEMOVE) || (wMsgType == DEF_OBJECTDYING))
		sRange = 1;
	else sRange = 0;


	if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
		if (m_pClientList[sOwnerH] == nullptr) return;

		switch (wMsgType) {
			case DEF_OBJECTNULLACTION:
			case DEF_OBJECTDAMAGE:
			case DEF_OBJECTDYING:
			case DEF_MSGTYPE_CONFIRM:
				cOwnerSend = true;
				break;
			default:
				cOwnerSend = false;
				break;
		}

		wp = (uint16_t *) cp_a;
		*wp = sOwnerH;
		cp_a += 2;

		sp = (short *) cp_a;
		sX = m_pClientList[sOwnerH]->m_sX;
		*sp = sX;
		cp_a += 2;

		sp = (short *) cp_a;
		sY = m_pClientList[sOwnerH]->m_sY;
		*sp = sY;
		cp_a += 2;

		sp = (short *) cp_a;
		*sp = m_pClientList[sOwnerH]->m_sType;
		cp_a += 2;

		*cp_a = m_pClientList[sOwnerH]->m_cDir;
		cp_a++;

		memcpy(cp_a, m_pClientList[sOwnerH]->m_cCharName, 10);
		cp_a += 10;

		sp = (short *) cp_a;
		*sp = m_pClientList[sOwnerH]->m_sAppr1;
		cp_a += 2;

		sp = (short *) cp_a;
		*sp = m_pClientList[sOwnerH]->m_sAppr2;
		cp_a += 2;

		sp = (short *) cp_a;
		*sp = m_pClientList[sOwnerH]->m_sAppr3;
		cp_a += 2;

		sp = (short *) cp_a;
		*sp = m_pClientList[sOwnerH]->m_sAppr4;
		cp_a += 2;

		ip = (int *) cp_a;
		*ip = m_pClientList[sOwnerH]->m_iApprColor;
		cp_a += 4;

		ip = (int *) cp_a;
		ipStatus = ip;
		*ip = m_pClientList[sOwnerH]->m_iStatus;
		cp_a += 4;

		//iTemp += m_pClientList[sOwnerH]->m_iStatus & 0x0F0;
		iTemp3 = m_pClientList[sOwnerH]->m_iStatus & 0x0F0FFFF7F; //0F0FFFF7Fh

		if (wMsgType == DEF_OBJECTNULLACTION) {
			if (m_pClientList[sOwnerH]->m_bIsKilled == true)
				*cp_a = 1;
			else *cp_a = 0;
		} else *cp_a = 0;
		cp_a++;

		wp = (uint16_t *) cp_s;
		*wp = sOwnerH + 30000;
		cp_s += 2;

		*cp_s = m_pClientList[sOwnerH]->m_cDir;
		cp_s++;

		*cp_s = (unsigned char) sV1;
		cp_s++;

		*cp_s = (unsigned char) sV2;
		cp_s++;

		sp = (short *) cp_s;
		sX = m_pClientList[sOwnerH]->m_sX;
		*sp = sX;
		cp_s += 2;

		sp = (short *) cp_s;
		sY = m_pClientList[sOwnerH]->m_sY;
		*sp = sY;
		cp_s += 2;

		wp = (uint16_t *) cp_sv;
		*wp = sOwnerH + 30000;
		cp_sv += 2;

		*cp_sv = m_pClientList[sOwnerH]->m_cDir;
		cp_sv++;

		*cp_sv = sV1 - sX;
		cp_sv++;

		*cp_sv = sV2 - sY;
		cp_sv++;

		sp = (short *) cp_sv;
		*sp = sV3;
		cp_sv += 2;

		bFlag = true;
		iShortCutIndex = 0;

		while (bFlag) {
			i = m_iClientShortCut[iShortCutIndex];
			iShortCutIndex++;
			if (i == 0) bFlag = false;

			if ((bFlag == true) && (m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true))

				if ((m_pClientList[i]->m_cMapIndex == m_pClientList[sOwnerH]->m_cMapIndex) &&
						  (m_pClientList[i]->m_sX >= m_pClientList[sOwnerH]->m_sX - 10 - sRange) &&
						  (m_pClientList[i]->m_sX <= m_pClientList[sOwnerH]->m_sX + 10 + sRange) &&
						  (m_pClientList[i]->m_sY >= m_pClientList[sOwnerH]->m_sY - 8 - sRange) &&
						  (m_pClientList[i]->m_sY <= m_pClientList[sOwnerH]->m_sY + 8 + sRange)) {

					/*//If player not same side and is invied (Beholder Hack)
					if (m_pClientList[sOwnerH] != nullptr && i != sOwnerH)
						if ((m_pClientList[i]->m_cSide != m_pClientList[sOwnerH]->m_cSide) &&
							((m_pClientList[sOwnerH]->m_iStatus & 0x00000010) != 0)) {
							if(wMsgType == DEF_OBJECTRUN || wMsgType == DEF_OBJECTMOVE || wMsgType == DEF_OBJECTSTOP || wMsgType == DEF_OBJECTGETITEM)
								if we filter attacking
								[KLKS] - http://xtremehb.com -> Play Abaddon (PRO ARESDEN) says:
								the poor guy who's being attacked wont know what hit him
						continue;
					}*/

					/*iTemp = *ipStatus;
					iTemp = 0x0FFFFFFF & iTemp; // ���� 4��Ʈ Ŭ����
					iTemp2 = (short)iGetPlayerABSStatus(sOwnerH, i); //(short)iGetPlayerRelationship_SendEvent(sOwnerH, i);
					iTemp  = (iTemp | (iTemp2 << 28));
					 *ipStatus = iTemp;*/

					if (m_pClientList[sOwnerH]->m_cSide != m_pClientList[i]->m_cSide) {
						if (m_pClientList[i]->m_iAdminUserLevel > 0) {
							iTemp = m_pClientList[sOwnerH]->m_iStatus;
						} else if (i != sOwnerH) {
							iTemp = iTemp3;
						} else {
							iTemp = m_pClientList[sOwnerH]->m_iStatus;
						}
					} else {
						iTemp = m_pClientList[sOwnerH]->m_iStatus;
						//iTemp = 0x0FFFFFFF;
					}

					//iTemp = m_pClientList[sOwnerH]->m_iStatus;
					iTemp = 0x0FFFFFFF & iTemp;
					iTemp2 = iGetPlayerABSStatus(sOwnerH, i);
					iTemp = (iTemp | (iTemp2 << 28));
					*ipStatus = iTemp;

					if ((m_pClientList[i]->m_sX >= m_pClientList[sOwnerH]->m_sX - 9) &&
							  (m_pClientList[i]->m_sX <= m_pClientList[sOwnerH]->m_sX + 9) &&
							  (m_pClientList[i]->m_sY >= m_pClientList[sOwnerH]->m_sY - 7) &&
							  (m_pClientList[i]->m_sY <= m_pClientList[sOwnerH]->m_sY + 7)) {

						switch (wMsgType) {
							case DEF_MSGTYPE_CONFIRM:
							case DEF_MSGTYPE_REJECT:
							case DEF_OBJECTNULLACTION:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 43, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 43, cKey);
								break;

							case DEF_OBJECTATTACK:
							case DEF_OBJECTATTACKMOVE:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
								break;

							case DEF_OBJECTMAGIC:
							case DEF_OBJECTDAMAGE:
							case DEF_OBJECTDAMAGEMOVE:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 11, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 11, cKey);
								break;

							case DEF_OBJECTDYING:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 15, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 15, cKey);
								break;

							default:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 9, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 9, cKey);
								break;
						} //Switch
					}// If 2
					else {
						switch (wMsgType) {
							case DEF_MSGTYPE_CONFIRM:
							case DEF_MSGTYPE_REJECT:
							case DEF_OBJECTNULLACTION:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 43, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 43, cKey);
								break;

							case DEF_OBJECTATTACK:
							case DEF_OBJECTATTACKMOVE:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
								break;

							case DEF_OBJECTMAGIC:
							case DEF_OBJECTDAMAGE:
							case DEF_OBJECTDAMAGEMOVE:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 11, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 11, cKey);
								break;

							case DEF_OBJECTDYING:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 15, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 15, cKey);
								break;

							default:
								if (cOwnerSend == true)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 43, cKey);
								else
									if (i != sOwnerH)
									m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 43, cKey);
								break;
						} //Switch
					} //else
				} // If 1
		} //While finish
	}//Finish Player
	else {

		if (m_pNpcList[sOwnerH] == nullptr) return;

		wp = (uint16_t *) cp_a;
		*wp = sOwnerH + 10000;
		cp_a += 2;

		sp = (short *) cp_a;
		sX = m_pNpcList[sOwnerH]->m_sX;
		*sp = sX;
		cp_a += 2;

		sp = (short *) cp_a;
		sY = m_pNpcList[sOwnerH]->m_sY;
		*sp = sY;
		cp_a += 2;

		sp = (short *) cp_a;
		*sp = m_pNpcList[sOwnerH]->m_sType;
		cp_a += 2;

		*cp_a = m_pNpcList[sOwnerH]->m_cDir;
		cp_a++;

		memcpy(cp_a, m_pNpcList[sOwnerH]->m_cName, 5);
		cp_a += 5;

		sp = (short *) cp_a;
		*sp = m_pNpcList[sOwnerH]->m_sAppr2;
		cp_a += 2;

		ip = (int *) cp_a;
		ipStatus = ip;
		*ip = m_pNpcList[sOwnerH]->m_iStatus;
		cp_a += 4;

		if (wMsgType == DEF_OBJECTNULLACTION) {
			if (m_pNpcList[sOwnerH]->m_bIsKilled == true)
				*cp_a = 1;
			else *cp_a = 0;
		} else *cp_a = 0;
		cp_a++;

		wp = (uint16_t *) cp_s;
		*wp = sOwnerH + 40000;
		cp_s += 2;

		*cp_s = m_pNpcList[sOwnerH]->m_cDir;
		cp_s++;

		*cp_s = (unsigned char) sV1;
		cp_s++;
		*cp_s = (unsigned char) sV2;
		cp_s++;

		sp = (short *) cp_s;
		sX = m_pNpcList[sOwnerH]->m_sX;
		*sp = sX;
		cp_s += 2;
		sp = (short *) cp_s;
		sY = m_pNpcList[sOwnerH]->m_sY;
		*sp = sY;
		cp_s += 2;

		wp = (uint16_t *) cp_sv;
		*wp = sOwnerH + 40000;
		cp_sv += 2;
		*cp_sv = m_pNpcList[sOwnerH]->m_cDir;
		cp_sv++;
		*cp_sv = sV1 - sX;
		cp_sv++;
		*cp_sv = sV2 - sY;
		cp_sv++;
		sp = (short *) cp_sv;
		*sp = sV3;
		cp_sv += 2;

		bFlag = true;
		iShortCutIndex = 0;

		while (bFlag) {

			i = m_iClientShortCut[iShortCutIndex];
			iShortCutIndex++;
			if (i == 0) bFlag = false;

			if ((bFlag == true) && (m_pClientList[i] != nullptr))

				if ((m_pClientList[i]->m_cMapIndex == m_pNpcList[sOwnerH]->m_cMapIndex) &&
						  (m_pClientList[i]->m_sX >= m_pNpcList[sOwnerH]->m_sX - 10 - sRange) &&
						  (m_pClientList[i]->m_sX <= m_pNpcList[sOwnerH]->m_sX + 10 + sRange) &&
						  (m_pClientList[i]->m_sY >= m_pNpcList[sOwnerH]->m_sY - 8 - sRange) &&
						  (m_pClientList[i]->m_sY <= m_pNpcList[sOwnerH]->m_sY + 8 + sRange)) {

					iTemp = *ipStatus;
					iTemp = 0x0FFFFFFF & iTemp;
					iTemp2 = iGetNpcRelationship(sOwnerH, i);
					iTemp = (iTemp | (iTemp2 << 28));
					*ipStatus = iTemp;

					if ((m_pClientList[i]->m_sX >= m_pNpcList[sOwnerH]->m_sX - 9) &&
							  (m_pClientList[i]->m_sX <= m_pNpcList[sOwnerH]->m_sX + 9) &&
							  (m_pClientList[i]->m_sY >= m_pNpcList[sOwnerH]->m_sY - 7) &&
							  (m_pClientList[i]->m_sY <= m_pNpcList[sOwnerH]->m_sY + 7)) {
						switch (wMsgType) {
							case DEF_MSGTYPE_CONFIRM:
							case DEF_MSGTYPE_REJECT:
							case DEF_OBJECTNULLACTION:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 27, cKey);
								break;

							case DEF_OBJECTDYING:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 15, cKey);
								break;

							case DEF_OBJECTDAMAGE:
							case DEF_OBJECTDAMAGEMOVE:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 11, cKey);
								break;

							case DEF_OBJECTATTACK:
							case DEF_OBJECTATTACKMOVE:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
								break;

							default:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 9, cKey);
								break;

						} //Switch
					} else {
						switch (wMsgType) {
							case DEF_MSGTYPE_CONFIRM:
							case DEF_MSGTYPE_REJECT:
							case DEF_OBJECTNULLACTION:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 27, cKey);
								break;

							case DEF_OBJECTDYING:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 15, cKey);
								break;

							case DEF_OBJECTDAMAGE:
							case DEF_OBJECTDAMAGEMOVE:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt, 11, cKey);
								break;

							case DEF_OBJECTATTACK:
							case DEF_OBJECTATTACKMOVE:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
								break;

							default:
								m_pClientList[i]->m_pXSock->iSendMsg(cData_All, 27, cKey);
								break;

						} //Switch
					}
				}
		}
	} // else - NPC
}

int CGame::iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char * pData) {
	register int * ip, ix, iy, iSize, iTileExists, iIndex;
	class CTile * pTileSrc, * pTile;
	unsigned char ucHeader;
	short * sp, * pTotal;
	int iTemp, iTemp2;
	uint16_t * wp;
	char * cp;

	if (m_pClientList[iClientH] == nullptr) return 0;

	pTotal = (short *) pData;
	cp = (char *) (pData + 2);

	iSize = 2;
	iTileExists = 0;

	pTileSrc = (class CTile *)(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_pTile +
			  (sX) + (sY) * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

	iIndex = 0;

	while (1) {
		ix = _tmp_iMoveLocX[cDir][iIndex];
		iy = _tmp_iMoveLocY[cDir][iIndex];
		if ((ix == -1) || (iy == -1)) break;

		iIndex++;

		pTile = (class CTile *)(pTileSrc + ix + iy * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

		//If player not same side and is invied (Beholder Hack)
		// there is another person on the tiles, and the owner is not the player
		//xxxxxx
		if ((m_pClientList[pTile->m_sOwner] != nullptr) && (pTile->m_sOwner != iClientH))
			if ((m_pClientList[pTile->m_sOwner]->m_cSide != 0) &&
					  (m_pClientList[pTile->m_sOwner]->m_cSide != m_pClientList[iClientH]->m_cSide) &&
					  ((m_pClientList[pTile->m_sOwner]->m_iStatus & 0x00000010) != 0)) {
				continue;
			}

		if ((pTile->m_sOwner != 0) || (pTile->m_sDeadOwner != 0) ||
				  (pTile->m_pItem[0] != nullptr) || (pTile->m_sDynamicObjectType != 0)) {

			iTileExists++;

			sp = (short *) cp;
			*sp = ix;
			cp += 2;
			sp = (short *) cp;
			*sp = iy;
			cp += 2;
			iSize += 4;

			ucHeader = 0;

			if (pTile->m_sOwner != 0) {
				if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER) {
					if (m_pClientList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
					else pTile->m_sOwner = 0;
				}
				if (pTile->m_cOwnerClass == DEF_OWNERTYPE_NPC) {
					if (m_pNpcList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
					else pTile->m_sOwner = 0;
				}
			}
			if (pTile->m_sDeadOwner != 0) {
				if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_PLAYER) {
					if (m_pClientList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
					else pTile->m_sDeadOwner = 0;
				}
				if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_NPC) {
					if (m_pNpcList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
					else pTile->m_sDeadOwner = 0;
				}
			}

			if (pTile->m_pItem[0] != nullptr) ucHeader = ucHeader | 0x04;
			if (pTile->m_sDynamicObjectType != 0) ucHeader = ucHeader | 0x08;

			*cp = ucHeader;
			cp++;
			iSize++;

			if ((ucHeader & 0x01) != 0) {
				switch (pTile->m_cOwnerClass) {
					case DEF_OWNERTYPE_PLAYER:
						sp = (short *) cp;
						*sp = pTile->m_sOwner;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sOwner]->m_sType;
						cp += 2;
						iSize += 2;

						*cp = m_pClientList[pTile->m_sOwner]->m_cDir;
						cp++;
						iSize++;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sOwner]->m_sAppr1;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sOwner]->m_sAppr3;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sOwner]->m_sAppr4;
						cp += 2;
						iSize += 2;

						ip = (int *) cp;
						*ip = m_pClientList[pTile->m_sOwner]->m_iApprColor;
						cp += 4;
						iSize += 4;

						ip = (int *) cp;

						if (m_pClientList[iClientH]->m_cSide != m_pClientList[pTile->m_sOwner]->m_cSide) {
							if (iClientH != pTile->m_sOwner) {
								iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus & 0x0F0FFFF7F;
							} else {
								iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
							}
						} else {
							iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
						}

						//iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = iGetPlayerABSStatus(pTile->m_sOwner, iClientH);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;

						memcpy(cp, m_pClientList[pTile->m_sOwner]->m_cCharName, 10);
						cp += 10;
						iSize += 10;
						break;

					case DEF_OWNERTYPE_NPC:
						sp = (short *) cp;
						*sp = pTile->m_sOwner + 10000;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pNpcList[pTile->m_sOwner]->m_sType;
						cp += 2;
						iSize += 2;

						*cp = m_pNpcList[pTile->m_sOwner]->m_cDir;
						cp++;
						iSize++;

						sp = (short *) cp;
						*sp = m_pNpcList[pTile->m_sOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;

						ip = (int *) cp;
						iTemp = m_pNpcList[pTile->m_sOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = iGetNpcRelationship(pTile->m_sOwner, iClientH);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;

						memcpy(cp, m_pNpcList[pTile->m_sOwner]->m_cName, 5);
						cp += 5;
						iSize += 5;
				}//end switch
			}// if ((ucHeader & 0x01) != 0)

			if ((ucHeader & 0x02) != 0) {
				switch (pTile->m_cDeadOwnerClass) {
					case DEF_OWNERTYPE_PLAYER:

						sp = (short *) cp;
						*sp = pTile->m_sDeadOwner;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sDeadOwner]->m_sType;
						cp += 2;
						iSize += 2;

						*cp = m_pClientList[pTile->m_sDeadOwner]->m_cDir;
						cp++;
						iSize++;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr1;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr3;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr4;
						cp += 2;
						iSize += 2;

						ip = (int *) cp;
						*ip = m_pClientList[pTile->m_sDeadOwner]->m_iApprColor;
						cp += 4;
						iSize += 4;

						ip = (int *) cp;

						if (m_pClientList[iClientH]->m_cSide != m_pClientList[pTile->m_sDeadOwner]->m_cSide) {
							if (iClientH != pTile->m_sDeadOwner) {
								iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus & 0x0F0FFFF7F;
							} else {
								iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
							}
						} else {
							iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
						}

						//iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;

						iTemp2 = iGetPlayerABSStatus(pTile->m_sDeadOwner, iClientH);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;

						memcpy(cp, m_pClientList[pTile->m_sDeadOwner]->m_cCharName, 10);
						cp += 10;
						iSize += 10;
						break;

					case DEF_OWNERTYPE_NPC:
						sp = (short *) cp;
						*sp = pTile->m_sDeadOwner + 10000;
						cp += 2;
						iSize += 2;

						sp = (short *) cp;
						*sp = m_pNpcList[pTile->m_sDeadOwner]->m_sType;
						cp += 2;
						iSize += 2;

						*cp = m_pNpcList[pTile->m_sDeadOwner]->m_cDir;
						cp++;
						iSize++;

						sp = (short *) cp;
						*sp = m_pNpcList[pTile->m_sDeadOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;

						ip = (int *) cp;

						iTemp = m_pNpcList[pTile->m_sDeadOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = iGetNpcRelationship(pTile->m_sDeadOwner, iClientH);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;

						cp += 4;
						iSize += 4;

						memcpy(cp, m_pNpcList[pTile->m_sDeadOwner]->m_cName, 5);
						cp += 5;
						iSize += 5;
						break;
				}//End Switch
			}// if ((ucHeader & 0x02) != 0)

			if (pTile->m_pItem[0] != nullptr) {
				sp = (short *) cp;
				*sp = pTile->m_pItem[0]->m_sSprite;
				cp += 2;
				iSize += 2;

				sp = (short *) cp;
				*sp = pTile->m_pItem[0]->m_sSpriteFrame;
				cp += 2;
				iSize += 2;

				*cp = pTile->m_pItem[0]->m_cItemColor;
				cp++;
				iSize++;
			}

			if (pTile->m_sDynamicObjectType != 0) {

				wp = (uint16_t *) cp;
				*wp = pTile->m_wDynamicObjectID;
				cp += 2;
				iSize += 2;

				sp = (short *) cp;
				*sp = pTile->m_sDynamicObjectType;
				cp += 2;
				iSize += 2;
			} //(pTile->m_sDynamicObjectType != nullptr)

		} //(pTile->m_sOwner != nullptr)
	} // end While(1)
	*pTotal = iTileExists;
	return iSize;
}

void CGame::CheckClientResponseTime() {
	register int i, iPlusTime, iMaxSuperAttack, iValue;
	short sTemp;
	uint32_t dwTime;
	short sItemIndex;
	int iMapside, iMapside2;
	//SYSTEMTIME SysTime;

	/*
	
	GetLocalTime(&SysTime);
	switch (SysTime.wDayOfWeek) {
	case 1:	iWarPeriod = 30; break; 
	case 2:	iWarPeriod = 30; break; 
	case 3:	iWarPeriod = 60; break; 
	case 4:	iWarPeriod = 60*2;  break; 
	case 5:	iWarPeriod = 60*5;  break; 
	case 6:	iWarPeriod = 60*10; break; 
	case 0:	iWarPeriod = 60*20; break; 
	}
	 */

	dwTime = timeGetTime();


	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {

			if ((dwTime - m_pClientList[i]->m_dwTime) > DEF_CLIENTTIMEOUT) {
				if (m_pClientList[i]->m_bIsInitComplete == true) {

					//Testcode
					wsprintf(G_cTxt, "Client Timeout: %s", m_pClientList[i]->m_cIPaddress);
					PutLogList(G_cTxt);

					DeleteClient(i, true, true);
				} else if ((dwTime - m_pClientList[i]->m_dwTime) > DEF_CLIENTTIMEOUT) {

					DeleteClient(i, false, false);
				}
			} else if (m_pClientList[i]->m_bIsInitComplete == true) {

				m_pClientList[i]->m_iTimeLeft_ShutUp--;
				if (m_pClientList[i]->m_iTimeLeft_ShutUp < 0) m_pClientList[i]->m_iTimeLeft_ShutUp = 0;


				m_pClientList[i]->m_iTimeLeft_Rating--;
				if (m_pClientList[i]->m_iTimeLeft_Rating < 0) m_pClientList[i]->m_iTimeLeft_Rating = 0;


				if (((dwTime - m_pClientList[i]->m_dwHungerTime) > DEF_HUNGERTIME) && (m_pClientList[i]->m_bIsKilled == false)) {


					if ((m_pClientList[i]->m_iLevel < DEF_LEVELLIMIT) || (m_pClientList[i]->m_iAdminUserLevel >= 1)) {

					} else m_pClientList[i]->m_iHungerStatus--;
					if (m_pClientList[i]->m_iHungerStatus <= 0) m_pClientList[i]->m_iHungerStatus = 0;
					m_pClientList[i]->m_dwHungerTime = dwTime;
					if ((m_pClientList[i]->m_iHP > 0) && (m_pClientList[i]->m_iHungerStatus < 40)) {//50Cent - Hunger Fix

						SendNotifyMsg(0, i, DEF_NOTIFY_HUNGER, m_pClientList[i]->m_iHungerStatus, 0, 0, nullptr);
					}
				}

				if (_bCheckCharacterData(i) == false) {
					DeleteClient(i, true, true);
					break;
				}


				if ((m_pClientList[i]->m_iHungerStatus <= 30) && (m_pClientList[i]->m_iHungerStatus >= 0))
					iPlusTime = (30 - m_pClientList[i]->m_iHungerStatus)*1000;
				else iPlusTime = 0;

				iPlusTime = abs(iPlusTime);


				if ((dwTime - m_pClientList[i]->m_dwHPTime) > (uint32_t) (DEF_HPUPTIME + iPlusTime)) {
					TimeHitPointsUp(i);
					m_pClientList[i]->m_dwHPTime = dwTime;
				}


				if ((dwTime - m_pClientList[i]->m_dwMPTime) > (uint32_t) (DEF_MPUPTIME + iPlusTime)) {
					TimeManaPointsUp(i);
					m_pClientList[i]->m_dwMPTime = dwTime;
				}


				if ((dwTime - m_pClientList[i]->m_dwSPTime) > (uint32_t) (DEF_SPUPTIME + iPlusTime)) {
					TimeStaminarPointsUp(i);
					m_pClientList[i]->m_dwSPTime = dwTime;
				}


				if ((m_pClientList[i]->m_bIsPoisoned == true) && ((dwTime - m_pClientList[i]->m_dwPoisonTime) > DEF_POISONTIME)) {
					PoisonEffect(i, 0);
					m_pClientList[i]->m_dwPoisonTime = dwTime;
				}

				if ((m_pMapList[m_pClientList[i]->m_cMapIndex]->m_bIsFightZone == false) &&
						  ((dwTime - m_pClientList[i]->m_dwAutoSaveTime) > (uint32_t) DEF_AUTOSAVETIME)) {
					bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, i);
					m_pClientList[i]->m_dwAutoSaveTime = dwTime;
				}


				if ((dwTime - m_pClientList[i]->m_dwExpStockTime) > (uint32_t) DEF_EXPSTOCKTIME) {
					m_pClientList[i]->m_dwExpStockTime = dwTime;
					CalcExpStock(i);
					CheckUniqueItemEquipment(i);
					CheckCrusadeResultCalculation(i);
					CheckHeldenianResultCalculation(i); // new
				}


				if ((dwTime - m_pClientList[i]->m_dwAutoExpTime) > (uint32_t) DEF_AUTOEXPTIME) {
					iValue = (m_pClientList[i]->m_iLevel / 2);
					if (iValue <= 0) iValue = 1;
					if (m_pClientList[i]->m_iAutoExpAmount < iValue) {

						if ((m_pClientList[i]->m_iExp + iValue) < m_iLevelExpTable[m_pClientList[i]->m_iLevel + 1]) {
							//m_pClientList[i]->m_iExpStock += iValue;
							GetExp(i, iValue, false);
							CalcExpStock(i);
						}
					}

					m_pClientList[i]->m_iAutoExpAmount = 0;
					m_pClientList[i]->m_dwAutoExpTime = dwTime;
				}


				if (m_pClientList[i]->m_iSpecialAbilityTime == 3) {

					SendNotifyMsg(0, i, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, nullptr);
					// New 25/05/2004
					// After the time up, add magic back
					sItemIndex = m_pClientList[i]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
					if (sItemIndex != -1) {
						if ((m_pClientList[i]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[i]->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
							if ((m_pClientList[i]->m_iInt + m_pClientList[i]->m_iAngelicInt) > 99 && (m_pClientList[i]->m_iMag + m_pClientList[i]->m_iAngelicMag) > 99) {
								m_pClientList[i]->m_cMagicMastery[94] = true;
								SendNotifyMsg(0, i, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
							}
						}
					}
				}
				m_pClientList[i]->m_iSpecialAbilityTime -= 3;
				if (m_pClientList[i]->m_iSpecialAbilityTime < 0) m_pClientList[i]->m_iSpecialAbilityTime = 0;


				if (m_pClientList[i]->m_bIsSpecialAbilityEnabled == true) {
					if (((dwTime - m_pClientList[i]->m_dwSpecialAbilityStartTime) / 1000) > m_pClientList[i]->m_iSpecialAbilityLastSec) {

						SendNotifyMsg(0, i, DEF_NOTIFY_SPECIALABILITYSTATUS, 3, 0, 0, nullptr);
						m_pClientList[i]->m_bIsSpecialAbilityEnabled = false;

						m_pClientList[i]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;

						sTemp = m_pClientList[i]->m_sAppr4;
						sTemp = sTemp & 0xFF0F;
						m_pClientList[i]->m_sAppr4 = sTemp;

						SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}
				}

				//Crusade
				m_pClientList[i]->m_iLockedMapTime -= 3;
				if (m_pClientList[i]->m_iLockedMapTime < 0) {

					m_pClientList[i]->m_iLockedMapTime = 0;
					std::memset(m_pClientList[i]->m_cLockedMapName, 0, sizeof (m_pClientList[i]->m_cLockedMapName));
					strcpy(m_pClientList[i]->m_cLockedMapName, "NONE");
				}

				// v2.04 �׾����� ���Ƽ �ð�
				m_pClientList[i]->m_iDeadPenaltyTime -= 3;
				if (m_pClientList[i]->m_iDeadPenaltyTime < 0) m_pClientList[i]->m_iDeadPenaltyTime = 0;

				if ((m_pClientList[i]->m_bIsWarLocation == true)) {
					// Crusade
					if (m_bIsCrusadeMode == false)
						if (m_pClientList[i]->m_bIsInsideOwnTown == false)
							m_pClientList[i]->m_iTimeLeft_ForceRecall--;

					if (m_pClientList[i]->m_iTimeLeft_ForceRecall <= 0) {
						// �ð��� �� �Ǿ��. ���� �����Ѵ�.
						m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
						m_pClientList[i]->m_dwWarBeginTime = dwTime;
						m_pClientList[i]->m_bIsWarLocation = false;

						// ��ȯ�ȴٴ� �޽����� ������.
						SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
						RequestTeleportHandler(i, "1   ");
					}
				}

				if ((m_bIsHeldenianMode == true) && (m_pMapList[m_pClientList[i]->m_cMapIndex] != 0)) {
					if (bCheckHeldenianMap(i, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1) {
						SetHeroFlag(i, DEF_OWNERTYPE_PLAYER, true);
					} else {
						SetHeroFlag(i, DEF_OWNERTYPE_PLAYER, false);
					}
				}

				if (m_pClientList[i] == nullptr) break;
				if (m_pClientList[i]->m_iSkillMsgRecvCount >= 2) {


					//PutLogFileList(G_cTxt);
					DeleteClient(i, true, true);
				} else {
					m_pClientList[i]->m_iSkillMsgRecvCount = 0;
				}


				if (m_pClientList[i] == nullptr) break;

				//if (m_pClientList[i]->m_iLevel < m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iLevelLimit) {
				if ((m_pClientList[i]->m_iLevel < m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iLevelLimit) && (m_pClientList[i]->m_iAdminUserLevel < 2)) {

					SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					RequestTeleportHandler(i, "0   ");
				}

				if (m_pClientList[i] == nullptr) break;

				//if ( (m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit != 0) &&
				//	 (m_pClientList[i]->m_iLevel > m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit) ) {
				if ((m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit != 0) &&
						  (m_pClientList[i]->m_iLevel > m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit) && (m_pClientList[i]->m_iAdminUserLevel < 2)) {

					SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					if ((m_pClientList[i]->m_cSide == 1) &&
							  (m_pClientList[i]->m_iAdminUserLevel == 0)) {
						RequestTeleportHandler(i, "2   ", "aresden", -1, -1);
					} else if ((m_pClientList[i]->m_cSide == 2) &&
							  (m_pClientList[i]->m_iAdminUserLevel == 0)) {
						RequestTeleportHandler(i, "2   ", "elvine", -1, -1);
					}
				}

				if (m_pClientList[i] == nullptr) break;
				if ((strcmp(m_pClientList[i]->m_cLocation, "elvine") != 0) &&
						  (strcmp(m_pClientList[i]->m_cLocation, "elvhunter") != 0) &&
						  (strcmp(m_pClientList[i]->m_cLocation, "arehunter") != 0) &&
						  (strcmp(m_pClientList[i]->m_cLocation, "aresden") != 0) &&
						  (m_pClientList[i]->m_iLevel >= 20) &&
						  (m_pClientList[i]->m_iAdminUserLevel == 0)) {
					wsprintf(G_cTxt, "Traveller Hack: (%s) Player: (%s) is a traveller and is greater than level 19.", m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName);
					PutHackLogFileList(G_cTxt);
					DeleteClient(i, true, true);
				}

				if (m_pClientList[i] == nullptr) break;
				if (((memcmp(m_pClientList[i]->m_cLocation, "aresden", 7) == 0) || (memcmp(m_pClientList[i]->m_cLocation, "elvine", 6) == 0)) &&
						  (m_bIsCrusadeMode == true)) {
					iMapside = iGetMapLocationSide(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName);
					if (iMapside > 3) iMapside2 = iMapside - 2;
					else iMapside2 = iMapside;

					if ((m_pClientList[i]->m_cSide != iMapside2) && (iMapside != 0)) {
						if ((iMapside <= 2) && (m_pClientList[i]->m_iAdminUserLevel < 1)) {
							if (m_pClientList[i]->m_cSide != 0) {
								m_pClientList[i]->m_dwWarBeginTime = timeGetTime();
								m_pClientList[i]->m_bIsWarLocation = true;
								m_pClientList[i]->m_iTimeLeft_ForceRecall = 1;

								RequestTeleportHandler(i, "1   ");
								SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
							}
						}
					}
				}

				if (m_pClientList[i] == nullptr) break;
				if (((memcmp(m_pClientList[i]->m_cLocation, "arehunter", 9) == 0) || (memcmp(m_pClientList[i]->m_cLocation, "elvhunter", 9) == 0)) &&
						  ((strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "2ndmiddle") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "middleland") == 0))) {
					SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					RequestTeleportHandler(i, "1   ");
				}

				if (m_bIsApocalypseMode == true) {
					if (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "abaddon", 7) == 0) {
						SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATEOPEN, 167, 169, 0, m_pClientList[i]->m_cMapName);
					} else if (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "icebound", 8) == 0) {
						SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATEOPEN, 89, 31, 0, m_pClientList[i]->m_cMapName);
					}
				}

				//50Cent - Capture The Flag
				if (m_bIsCTFMode) {
					RequestCheckFlag(i);
					if (bCheckIfIsFlagCarrier(i)) {
						if (m_pClientList[i]->m_iHP >= 1) {
							SetInvisibilityFlag(i, DEF_OWNERTYPE_PLAYER, false);
							SetIceFlag(i, DEF_OWNERTYPE_PLAYER, true);
						}
					}
				}


				if (m_pClientList[i] == nullptr) break;
				if ((m_bIsApocalypseMode == true) &&
						  (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "icebound", 8) == 0) &&
						  ((m_pClientList[i]->m_sX == 89 && m_pClientList[i]->m_sY == 31) ||
						  (m_pClientList[i]->m_sX == 89 && m_pClientList[i]->m_sY == 32) ||
						  (m_pClientList[i]->m_sX == 90 && m_pClientList[i]->m_sY == 31) ||
						  (m_pClientList[i]->m_sX == 90 && m_pClientList[i]->m_sY == 32))) {
					RequestTeleportHandler(i, "2   ", "druncncity", -1, -1);
				}

				if (m_pClientList[i] == nullptr) break;

				if ((memcmp(m_pClientList[i]->m_cLocation, "are", 3) == 0) &&
						  (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "elvfarm") == 0)) {

					SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					RequestTeleportHandler(i, "0   ");
				}

				if (m_pClientList[i] == nullptr) break;

				if ((memcmp(m_pClientList[i]->m_cLocation, "elv", 3) == 0) &&
						  (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "arefarm") == 0)) {

					SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					RequestTeleportHandler(i, "0   ");
				}

				if (m_pClientList[i] == nullptr) break;

				if ((strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "middleland") == 0)
						  && (strcmp(m_pClientList[i]->m_cLocation, "NONE") == 0) &&
						  (m_pClientList[i]->m_iAdminUserLevel < 1)) {
					// ��ȯ�ȴٴ� �޽����� ������ ���� ����.
					SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					RequestTeleportHandler(i, "0   ");
				}

				if (m_pClientList[i]->m_bInRecallImpossibleMap == true) {
					m_pClientList[i]->m_iTimeLeft_ForceRecall--;
					if (m_pClientList[i]->m_iTimeLeft_ForceRecall <= 0) {
						m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
						m_pClientList[i]->m_bInRecallImpossibleMap = false;
						SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
						RequestTeleportHandler(i, "0   ");
					}
				}

				if (m_pClientList[i] == nullptr) break;

				m_pClientList[i]->m_iSuperAttackCount++;
				if (m_pClientList[i]->m_iSuperAttackCount > 12) {

					m_pClientList[i]->m_iSuperAttackCount = 0;

					iMaxSuperAttack = (m_pClientList[i]->m_iLevel / 10);
					if (m_pClientList[i]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[i]->m_iSuperAttackLeft++;


					SendNotifyMsg(0, i, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
				}


				m_pClientList[i]->m_iTimeLeft_FirmStaminar--;
				if (m_pClientList[i]->m_iTimeLeft_FirmStaminar < 0) m_pClientList[i]->m_iTimeLeft_FirmStaminar = 0;


				if (m_pClientList[i] == nullptr) break;
				if (m_pClientList[i]->m_bIsSendingMapStatus == true) _SendMapStatus(i);

				if (m_pClientList[i]->m_iConstructionPoint > 0) {

					CheckCommanderConstructionPoint(i);
				}
			}
		}
	}
}

void CGame::OnMainLogSocketEvent(UINT /*message*/, WPARAM wParam, LPARAM lParam) {
	int iRet;

	if (m_pMainLogSock == nullptr) return;

	iRet = m_pMainLogSock->iOnSocketEvent(wParam, lParam);

	switch (iRet) {
		case DEF_XSOCKEVENT_CONNECTIONESTABLISH:
			// �α׼������� ������ �̷�� �����Ƿ� ���Ӽ��� ��� �޽����� ����Ѵ�.
			PutLogList("(!!!) Main-log-socket connected!");
			bSendMsgToLS(MSGID_REQUEST_REGISTERGAMESERVER, 0);
			break;

		case DEF_XSOCKEVENT_READCOMPLETE:
			// �޽����� ���ŵǾ��.
			OnMainLogRead();
			break;

		case DEF_XSOCKEVENT_BLOCK:
			PutLogList("(!!!) Main-log-socket BLOCKED!");
			break;

		case DEF_XSOCKEVENT_CONFIRMCODENOTMATCH:

		case DEF_XSOCKEVENT_MSGSIZETOOLARGE:
			// �����ؾ� �� �޽��� ũ�Ⱑ ���ۺ��� ũ��.	�����ؾ߸� �Ѵ�.
		case DEF_XSOCKEVENT_SOCKETERROR:
			// ���Ͽ� ������ ����.
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			// ������ �����.
			delete m_pMainLogSock;
			m_pMainLogSock = nullptr;
			PutLogList("(!!!) Main-log-socket connection lost!");
			m_bIsLogSockAvailable = false;

			// �α� �������� ������ �������Ƿ� ������ �� ���. �ڵ����� ������ �˴ٿ��Ѵ�.
			if (m_bOnExitProcess == false) {
				m_cShutDownCode = 3;
				m_bOnExitProcess = true;
				m_dwExitProcessTime = timeGetTime();
				// ���� ���� �˴ٿ��� ���۵Ǹ� ���̻��� Ŭ���̾�Ʈ ������ �źεȴ�.
				PutLogList("(!!!) GAME SERVER SHUTDOWN PROCESS BEGIN(by main-log-socket connection Lost)!!!");
			}
			break;
	}
}

void CGame::OnMainLogRead() {
	uint32_t dwMsgSize;
	char * pData, cKey;

	pData = m_pMainLogSock->pGetRcvDataPointer(&dwMsgSize, &cKey);

	if (bPutMsgQuene(DEF_MSGFROM_LOGSERVER, pData, dwMsgSize, 0, cKey) == false) {

		PutLogList("@@@@@@ CRITICAL ERROR in MsgQuene!!! @@@@@@");
	}
}

bool CGame::bSendMsgToLS(uint32_t dwMsg, int iClientH, bool bFlag, char* pData) {
	uint32_t * dwp;
	uint16_t * wp;
	int iRet, i, iSize;
	char cCharName[11], cAccountName[11], cAccountPassword[11], cAddress[16], cGuildName[21], cTxt[120], * cp;
	char cGuildLoc[11], cTemp[120];
	int * ip, iSendSize;


	/*
	
	m_iCurSubLogSockIndex++;
	if (m_iCurSubLogSockIndex >= DEF_MAXSUBLOGSOCK) m_iCurSubLogSockIndex = 0;


	
	iCnt = 0;
	bLoopFlag = false;
	while (bLoopFlag == false) {
		if (m_pSubLogSock[m_iCurSubLogSockIndex] != nullptr)
			 bLoopFlag = true;
		else m_iCurSubLogSockIndex++;

		iCnt++;
		if (iCnt >= DEF_MAXSUBLOGSOCK) {
			
			if (m_bOnExitProcess == false) {
				m_cShutDownCode      = 3;
				m_bOnExitProcess     = true;
				m_dwExitProcessTime  = timeGetTime();
				
				PutLogList("(!) GAME SERVER SHUTDOWN PROCESS STARTED(by Log-server connection Lost)!!!");
			}
			return false;
		}
	}
	 */


	// v1.41
	std::memset(G_cData50000, 0, sizeof (G_cData50000));
	std::memset(cCharName, 0, sizeof (cCharName));
	std::memset(cAccountName, 0, sizeof (cAccountName));
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));
	std::memset(cAddress, 0, sizeof (cAddress));
	std::memset(cGuildName, 0, sizeof (cGuildName));
	std::memset(cGuildLoc, 0, sizeof (cGuildLoc));

	switch (dwMsg) {
			// New 07/05/2004
		case MSGID_GAMEMASTERLOG:

			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;
			if (pData == nullptr) return false;


			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMEMASTERLOG;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			iSize = strlen(pData);
			memcpy((char *) cp, pData, iSize);

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 6 + iSize);
			iSendSize = 6 + iSize;
			break;
			// v2.15

		case MSGID_GAMEITEMLOG:

			if (_bCheckSubLogSocketIndex() == false) return false;

			//		if (m_pClientList[iClientH] == nullptr) return false;
			if (pData == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMEITEMLOG;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			iSize = strlen(pData);
			memcpy((char *) cp, pData, iSize);

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 6 + iSize);
			iSendSize = 6 + iSize;
			break;

		case MSGID_SENDSERVERSHUTDOWNMSG:
			if (m_pMainLogSock == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_SENDSERVERSHUTDOWNMSG;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;


			iRet = m_pMainLogSock->iSendMsg(G_cData50000, 6);
			break;

		case MSGID_GAMESERVERSHUTDOWNED:
			if (m_pMainLogSock == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMESERVERSHUTDOWNED;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;


			iRet = m_pMainLogSock->iSendMsg(G_cData50000, 6);
			return true;

		case MSGID_REQUEST_SETACCOUNTWAITSTATUS:
		case MSGID_REQUEST_SETACCOUNTINITSTATUS:

			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = dwMsg;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cp, m_pClientList[iClientH]->m_cAccountName, 10);
			cp += 10;

			ip = (int *) cp;
			*ip = m_pClientList[iClientH]->m_iLevel;
			cp += 4;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 20);
			iSendSize = 16;
			break;

		case MSGID_ENTERGAMECONFIRM:


			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_ENTERGAMECONFIRM;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cp, m_pClientList[iClientH]->m_cAccountName, 10);
			cp += 10;

			memcpy(cp, m_pClientList[iClientH]->m_cAccountPassword, 10);
			cp += 10;

			memcpy(cp, m_cServerName, 10);
			cp += 10;

			std::memset(cTxt, 0, sizeof (cTxt));
			m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTxt);
			memcpy(cp, cTxt, 16);
			cp += 16;

			ip = (int *) cp;
			*ip = m_pClientList[iClientH]->m_iLevel;
			cp += 4;

			//testcode
			wsprintf(G_cTxt, "Confirmed. Account: (%s) Name: (%s) Level: (%d)", m_pClientList[iClientH]->m_cAccountName, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iLevel);
			PutLogList(G_cTxt);

			if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID)) {
				break;
			} else
				m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 56);
			iSendSize = 56;
			break;

		case MSGID_REQUEST_REGISTERGAMESERVER:

			if (m_pMainLogSock == nullptr) return false;

			wsprintf(cTxt, "(!) Try to register game server(%s)", m_cServerName);
			PutLogList(cTxt);

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_REGISTERGAMESERVER;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cAccountName, m_cServerName, 10);
			if (m_iGameServerMode == 1) {
				memcpy(cAddress, m_cGameServerAddrExternal, strlen(m_cGameServerAddrExternal));
			}
			if (m_iGameServerMode == 2) {
				memcpy(cAddress, m_cGameServerAddr, strlen(m_cGameServerAddr));
			}
			memcpy(cp, cAccountName, 10);
			cp += 10;

			memcpy(cp, cAddress, 16);
			cp += 16;

			wp = (uint16_t *) cp;
			*wp = m_iGameServerPort;
			cp += 2;

			*cp = m_iTotalMaps;
			cp++;

			for (i = 0; i < m_iTotalMaps; i++) {
				memcpy(cp, m_pMapList[i]->m_cName, 11);
				cp += 11;
			}


			iRet = m_pMainLogSock->iSendMsg(G_cData50000, 35 + m_iTotalMaps * 11);

			return true;

		case MSGID_REQUEST_PLAYERDATA:


			if (_bCheckSubLogSocketIndex() == false) return false;


			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_PLAYERDATA;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			//testcode
			if (strlen(m_pClientList[iClientH]->m_cCharName) == 0) PutLogList("(X) CharName nullptr!");

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
			memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;

			std::memset(cTemp, 0, sizeof (cTemp));
			iRet = m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTemp);
			memcpy((char *) cp, cTemp, 15);
			cp += 15;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 52);
			iSendSize = 52;
			break;

		case MSGID_REQUEST_SAVEPLAYERDATA_REPLY:
		case MSGID_REQUEST_SAVEPLAYERDATA:
		case MSGID_REQUEST_SAVEPLAYERDATALOGOUT:


			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = dwMsg;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
			memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;

			*cp = (char) bFlag;
			cp++;

			iSize = _iComposePlayerDataFileContents(iClientH, cp);

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 37 + iSize);
			iSendSize = 37 + iSize;
			break;

		case MSGID_REQUEST_NOSAVELOGOUT:


			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_NOSAVELOGOUT;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
			memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;

			*cp = (char) bFlag;
			cp++;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 37);
			iSendSize = 37;
			break;

		case MSGID_REQUEST_CREATENEWGUILD:


			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_CREATENEWGUILD;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
			memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);
			memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);
			memcpy(cGuildLoc, m_pClientList[iClientH]->m_cLocation, 10);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;

			memcpy((char *) cp, cGuildName, 20);
			cp += 20;
			memcpy((char *) cp, cGuildLoc, 10);
			cp += 10;

			ip = (int *) cp;
			*ip = m_pClientList[iClientH]->m_iGuildGUID;
			cp += 4;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 75);
			iSendSize = 75;
			break;

		case MSGID_REQUEST_DISBANDGUILD:

			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_DISBANDGUILD;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
			memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);
			memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountName, 10);
			cp += 10;

			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;

			memcpy((char *) cp, cGuildName, 20);
			cp += 20;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 56);
			iSendSize = 56;
			break;

		case MSGID_REQUEST_HELDENIAN_WINNER:
			if (_bCheckSubLogSocketIndex() == false) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_HELDENIAN_WINNER;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			if (m_cHeldenianVictoryType == 1)
				memcpy(cp, "aresden", 7);
			else if (m_cHeldenianVictoryType == 2)
				memcpy(cp, "elvine", 6);
			else
				memcpy(cp, "draw", 4);

			cp += 7;

			ip = (int *) cp;
			*ip = m_dwHeldenianStartTime;
			cp += 4;

			ip = (int *) cp;
			*ip = m_dwHeldenianFinishTime;
			cp += 4;

			ip = (int *) cp;
			*ip = m_cHeldenianModeType;
			cp += 4;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 21);
			iSendSize = 21;
			break;

		case MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN:


			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cGuildName, 20);
			cp += 20;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 36);
			iSendSize = 36;
			break;

		case MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN:


			if (_bCheckSubLogSocketIndex() == false) return false;

			if (m_pClientList[iClientH] == nullptr) return false;

			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;

			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

			memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
			memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

			memcpy((char *) cp, cCharName, 10);
			cp += 10;

			memcpy((char *) cp, cGuildName, 20);
			cp += 20;

			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 36);
			iSendSize = 36;
			break;
	}

	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			wsprintf(G_cTxt, "(!!!) Sub-log-socket(%d) send error!", m_iCurSubLogSockIndex);
			PutLogList(G_cTxt);
			PutLogFileList(G_cTxt);

			delete m_pSubLogSock[m_iCurSubLogSockIndex];
			m_pSubLogSock[m_iCurSubLogSockIndex] = nullptr;
			m_bIsSubLogSockAvailable[m_iCurSubLogSockIndex] = false;
			m_iSubLogSockActiveCount--;


			m_pSubLogSock[m_iCurSubLogSockIndex] = new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
			m_pSubLogSock[m_iCurSubLogSockIndex]->bConnect(m_cLogServerAddr, m_iLogServerPort, (WM_ONLOGSOCKETEVENT + m_iCurSubLogSockIndex + 1));
			m_pSubLogSock[m_iCurSubLogSockIndex]->bInitBufferSize(DEF_MSGBUFFERSIZE);

			wsprintf(G_cTxt, "(!) Try to reconnect sub-log-socket(%d)... Addr:%s  Port:%d", m_iCurSubLogSockIndex, m_cLogServerAddr, m_iLogServerPort);
			PutLogList(G_cTxt);


			m_iSubLogSockFailCount++;



			if (_bCheckSubLogSocketIndex() == false) return false;
			m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, iSendSize);

			return false;
	}

	return true;
}

void CGame::ResponsePlayerDataHandler(char * pData, uint32_t dwSize) {
	uint16_t * wp;
	char * cp, cCharName[11], cTxt[120];
	int i;


	std::memset(cCharName, 0, sizeof (cCharName));
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	memcpy(cCharName, cp, 10);
	cp += 10;


	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) {
			if (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0) {
				wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
				switch (*wp) {
					case DEF_LOGRESMSGTYPE_CONFIRM:

						InitPlayerData(i, pData, dwSize);
						break;

					case DEF_LOGRESMSGTYPE_REJECT:

						wsprintf(G_cTxt, "(HACK?) Not existing character(%s) data request! Rejected!", m_pClientList[i]->m_cCharName);
						PutLogList(G_cTxt);
						//PutLogFileList(G_cTxt); // v1.4

						DeleteClient(i, false, false);
						break;

					default:
						break;
				}

				return;
			}
		}


	wsprintf(cTxt, "(!)Non-existing player data received from Log server: CharName(%s)", cCharName);
	PutLogList(cTxt);
}

void CGame::InitPlayerData(int iClientH, char * pData, uint32_t dwSize) {

	char * cp, cName[11], cData[256], cTxt[256], cGuildStatus, cQuestRemain;
	uint32_t * dwp;
	uint16_t * wp;
	int iRet, i, iTotalPoints;
	bool bRet;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == true) return;




	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	std::memset(cName, 0, sizeof (cName));
	memcpy(cName, cp, 10);
	cp += 10;


	//m_pClientList[iClientH]->m_cAccountStatus = *cp;
	cp++;


	cGuildStatus = *cp;
	cp++;


	m_pClientList[iClientH]->m_iHitRatio = 0;
	m_pClientList[iClientH]->m_iDefenseRatio = 0;
	m_pClientList[iClientH]->m_cSide = 0;

	bRet = _bDecodePlayerDatafileContents(iClientH, cp, dwSize - 19);

	if (bRet == false) {
		wsprintf(G_cTxt, "(HACK?) Character(%s) data error!", m_pClientList[iClientH]->m_cCharName);
		DeleteClient(iClientH, false, true); //!!!
		return;
	}
	if (m_bHappyHour == true) {
		StarHappyHour();
	}

	//***************************************************************************


	___RestorePlayerCharacteristics(iClientH);


	___RestorePlayerRating(iClientH);

	if ((m_pClientList[iClientH]->m_sX == -1) && (m_pClientList[iClientH]->m_sY == -1)) {

		GetMapInitialPoint(m_pClientList[iClientH]->m_cMapIndex, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cLocation);
	}

	// New 17/05/2004
	SetPlayingStatus(iClientH);
	int iTemp, iTemp2;
	iTemp = m_pClientList[iClientH]->m_iStatus;
	iTemp = 0x0FFFFFFF & iTemp;
	iTemp2 = iGetPlayerABSStatus(iClientH);
	iTemp = iTemp | (iTemp2 << 28);
	m_pClientList[iClientH]->m_iStatus = iTemp;

	if (m_pClientList[iClientH]->m_iLevel > 100)
		if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
			ForceChangePlayMode(iClientH, false);


	m_pClientList[iClientH]->m_iNextLevelExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel + 1]; //iGetLevelExp(m_pClientList[iClientH]->m_iLevel + 1);


	CalcTotalItemEffect(iClientH, -1, true); //false

	iCalcTotalWeight(iClientH);

	// New 23/05/2004
	//GM Invi comes here
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) {
		SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
	}

	if ((m_pClientList[iClientH]->m_iLevel > 2) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0) &&
			  (m_pClientList[iClientH]->m_iExp < iGetLevelExp(m_pClientList[iClientH]->m_iLevel - 1) - 3000)) {
		m_pClientList[iClientH]->m_iExp = iGetLevelExp(m_pClientList[iClientH]->m_iLevel - 1) - 3000;
		wsprintf(G_cTxt, "Problema, posible editaci�n: (%s) Jugador: (%s) tiene la experiencia bajo 0 - (Exp:%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iExp);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, true, true);
		return;
	}
	iTotalPoints = 0;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		iTotalPoints += m_pClientList[iClientH]->m_cSkillMastery[i];
	if ((iTotalPoints - 21 > m_sCharSkillLimit) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) - has more than allowed skill points (%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, iTotalPoints);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, true, true);
		return;
	}


	CheckSpecialEvent(iClientH);
	bCheckMagicInt(iClientH);


	if ((cGuildStatus == 0) && (memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0)) {

		std::memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof (m_pClientList[iClientH]->m_cGuildName));
		strcpy(m_pClientList[iClientH]->m_cGuildName, "NONE");
		m_pClientList[iClientH]->m_iGuildRank = -1;
		m_pClientList[iClientH]->m_iGuildGUID = -1;

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_GUILDDISBANDED, 0, 0, 0, m_pClientList[iClientH]->m_cGuildName);
	}

	if (m_pClientList[iClientH]->m_iQuest != 0) {
		cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount);
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, nullptr);
		_bCheckIsQuestCompleted(iClientH);
	}



	if (m_pClientList[iClientH] == nullptr) {
		wsprintf(cTxt, "<%d> InitPlayerData error - Socket error! Disconnected.", iClientH);
		PutLogList(cTxt);
		return;
	}

	std::memset(cData, 0, sizeof (cData));
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITPLAYER;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;


	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:


			wsprintf(cTxt, "<%d> InitPlayerData - Socket error! Disconnected.", iClientH);
			PutLogList(cTxt);

			DeleteClient(iClientH, false, true); //!!!!!
			return;
	}


	m_pClientList[iClientH]->m_bIsInitComplete = true;


	bSendMsgToLS(MSGID_ENTERGAMECONFIRM, iClientH);


	m_pClientList[iClientH]->m_dwLastActionTime = m_pClientList[iClientH]->m_dwAFKCheckTime = timeGetTime();
	if (m_iTotalClients > DEF_MAXONESERVERUSERS) {
		switch (iDice(1, 2)) {
			case 1:
				RequestTeleportHandler(iClientH, "2   ", "bisle", -1, -1);
				break;
			case 2:
				switch (m_pClientList[iClientH]->m_cSide) {
					case 0: RequestTeleportHandler(iClientH, "2   ", "resurr1", -1, -1);
						break;
					case 1: RequestTeleportHandler(iClientH, "2   ", "resurr1", -1, -1);
						break;
					case 2: RequestTeleportHandler(iClientH, "2   ", "resurr2", -1, -1);
						break;
				}
				break;
		}
	}

	PlayerOrder_GetSkills(iClientH);

	return;
}

void CGame::GameProcess() {
	NpcProcess();
	MsgProcess();
}

bool CGame::bReadProgramConfigFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode, cTxt[120], cGSMode[16] = "";
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	bool bLogDNS = false; // bLogDNS added by snaipperi

	cReadMode = 0;

	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {

		//	PutLogList("(!) Cannot open configuration file.");
		return false;
	} else {
		PutLogList("(!) Reading configuration file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);

		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		//token = strtok( cp, seps );
		while (token != nullptr) {

			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						std::memset(m_cServerName, 0, sizeof (m_cServerName));
						if (strlen(token) > 10) {

							wsprintf(cTxt, "(!!!) Game server name(%s) must within 10 chars!", token);
							PutLogList(cTxt);
							return false;
						}
						strcpy(m_cServerName, token);
						wsprintf(cTxt, "(*) Game server name : %s", m_cServerName);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 2:

						std::memset(m_cGameServerAddr, 0, sizeof (m_cGameServerAddr));
						char ServerAddr[50];
						::gethostname(ServerAddr, 50);
						struct hostent *pHostEnt;
						pHostEnt = ::gethostbyname(ServerAddr);
						if (pHostEnt != nullptr) {
							wsprintf(ServerAddr, "%d.%d.%d.%d",
									  (pHostEnt->h_addr_list[0][0] & 0x00ff),
									  (pHostEnt->h_addr_list[0][1] & 0x00ff),
									  (pHostEnt->h_addr_list[0][2] & 0x00ff),
									  (pHostEnt->h_addr_list[0][3] & 0x00ff));
						}
						strcpy(m_cGameServerAddr, ServerAddr);

						wsprintf(cTxt, "(*) Game server address : %s", m_cGameServerAddr);
						PutLogList(cTxt);

						m_iGameServerPort = atoi(token);
						wsprintf(cTxt, "(*) Game server port : %d", m_iGameServerPort);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 3:
						std::memset(m_cLogServerAddr, 0, sizeof (m_cLogServerAddr));

						if (bLogDNS == true) { //bLogDNS by Snaipperi
							PutLogList(cTxt);
							char *cAddress = token;
							char cDnsResult[40];
							struct hostent *host_entry;

							host_entry = gethostbyname(cAddress);
							if (host_entry == nullptr) {
								wsprintf(cTxt, "(!)DNS (%s) failed", token);
								PutLogList(cTxt);
								return false;
							}

							wsprintf(cDnsResult, "%d.%d.%d.%d",
									  (pHostEnt->h_addr_list[0][0] & 0x00ff),
									  (pHostEnt->h_addr_list[0][1] & 0x00ff),
									  (pHostEnt->h_addr_list[0][2] & 0x00ff),
									  (pHostEnt->h_addr_list[0][3] & 0x00ff));

							wsprintf(cTxt, "(!)DNS from (%s) to (%s) success!", token, cDnsResult);
							PutLogList(cTxt);
							strcpy(m_cLogServerAddr, cDnsResult);
							wsprintf(cTxt, "(*) Log server address : %s", m_cLogServerAddr);
							PutLogList(cTxt);
							cReadMode = 0;
							break;
						}

						if (strlen(token) > 20) {
							// �ּҰ� 15����Ʈ �̻�. �����.
							wsprintf(cTxt, "(!!!) Log server address(%s) must within 20 chars!", token);
							PutLogList(cTxt);
							return false;
						}
						strcpy(m_cLogServerAddr, token);
						wsprintf(cTxt, "(*) Log server address : %s", m_cLogServerAddr);
						PutLogList(cTxt);
						cReadMode = 0;
						break;


					case 4:
						m_iLogServerPort = atoi(token);
						wsprintf(cTxt, "(*) Log server port : %d", m_iLogServerPort);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 5:

						if (strlen(token) > 10) {

							wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map name(%s) must within 10 chars!", token);
							PutLogList(cTxt);
							return false;
						}
						if (_bRegisterMap(token) == false) {
							return false;
						}
						cReadMode = 0;
						break;

					case 8:
						std::memset(m_cGameServerAddrInternal, 0, sizeof (m_cGameServerAddrInternal));
						if (strlen(token) > 15) {

							wsprintf(cTxt, "(!!!) Internal (LAN) Game server address(%s) must within 15 chars!", token);
							PutLogList(cTxt);
							return false;
						}
						strcpy(m_cGameServerAddrInternal, token);
						wsprintf(cTxt, "(*) Internal (LAN) Game server address : %s", m_cGameServerAddrInternal);
						PutLogList(cTxt);
						cReadMode = 0;
						break;


					case 9:
						std::memset(m_cGameServerAddrExternal, 0, sizeof (m_cGameServerAddrExternal));
						if (strlen(token) > 15) {

							wsprintf(cTxt, "(!!!) External (Internet) Game server address(%s) must within 15 chars!", token);
							PutLogList(cTxt);
							return false;
						}
						strcpy(m_cGameServerAddrExternal, token);
						wsprintf(cTxt, "(*) External (Internet) Game server address : %s", m_cGameServerAddrExternal);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 10:
						std::memset(m_cGameServerAddr, 0, sizeof (m_cGameServerAddr));
						if (strlen(token) > 15) {

							wsprintf(cTxt, "(!!!) Game server address(%s) must within 15 chars!", token);
							PutLogList(cTxt);
							return false;
						}
						strcpy(m_cGameServerAddr, token);
						wsprintf(cTxt, "(*) Game server address : %s", m_cGameServerAddr);
						PutLogList(cTxt);
						cReadMode = 0;
						break;
					case 11:
						if ((memcmp(token, "lan", 3) == 0) || (memcmp(token, "LAN", 3) == 0)) {
							m_iGameServerMode = 1;
							memcpy(cGSMode, "LAN", 3);
						}
						if ((memcmp(token, "internet", 3) == 0) || (memcmp(token, "INTERNET", 3) == 0)) {
							m_iGameServerMode = 2;
							memcpy(cGSMode, "INTERNET", 8);
						}
						if (m_iGameServerMode == 0) {
							wsprintf(cTxt, "(!!!) Game server mode(%s) must be either LAN/lan/INTERNET/internet", token);
							PutLogList(cTxt);
							return false;
						}
						wsprintf(cTxt, "(*) Game server mode : %s", cGSMode);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 13: //Log DNS by Snaipperi
						if (memcmp(token, "true", 4) == 0) {
							bLogDNS = true;
						} else bLogDNS = false;
						wsprintf(cTxt, "Log DNS is set to %i", bLogDNS);
						PutLogList(cTxt);
						cReadMode = 0;
						break;
				}
			} else {
				if (memcmp(token, "game-server-name", 16) == 0) cReadMode = 1;
				if (memcmp(token, "game-server-port", 16) == 0) cReadMode = 2;
				if (memcmp(token, "log-server-address", 18) == 0) cReadMode = 3;
				if (memcmp(token, "internal-log-server-port", 24) == 0) cReadMode = 4;
				if (memcmp(token, "game-server-map", 15) == 0) cReadMode = 5;
				if (memcmp(token, "game-server-internal-address", 28) == 0) cReadMode = 8;
				if (memcmp(token, "game-server-external-address", 28) == 0) cReadMode = 9;
				if (memcmp(token, "game-server-address", 19) == 0) cReadMode = 10;
				if (memcmp(token, "game-server-mode", 16) == 0) cReadMode = 11;
				if (memcmp(token, "log-server-dns", 14) == 0) cReadMode = 13;

			}
			token = pStrTok->pGet();
			//token = strtok( nullptr, seps );
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);

	if (m_iGameServerMode == 0) {
		wsprintf(cTxt, "(!!!) Game server mode cannot be empty. It must be either LAN/lan/INTERNET/internet", token);
		PutLogList(cTxt);
		return false;
	}

	return true;
}

bool CGame::bReadSettingsConfigFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode, cTxt[120], len;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;

	cReadMode = 0;

	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {

		//   PutLogList("(!) Cannot open configuration file.");
		return false;
	} else {
		PutLogList("(!) Reading settings file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);

		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		//token = strtok( cp, seps );
		while (token != nullptr) {

			if (cReadMode != 0) {
				switch (cReadMode) {

					case 1:
						if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1)) {
							m_iPrimaryDropRate = atoi(token);
						} else {
							wsprintf(cTxt, "(!!!) Primary drop rate(%s) invalid must be between 1 to 10000", token);
							PutLogList(cTxt);
						}
						wsprintf(cTxt, "(*) Primary drop rate: (%d)", m_iPrimaryDropRate);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 2:
						if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1)) {
							m_iSecondaryDropRate = atoi(token);
						} else {
							wsprintf(cTxt, "(!!!) Secondary drop rate(%s) invalid must be between 1 to 10000", token);
							PutLogList(cTxt);
						}
						wsprintf(cTxt, "(*) Secondary drop rate: (%d)", m_iSecondaryDropRate);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 3:
						if ((memcmp(token, "classic", 7) == 0) || (memcmp(token, "CLASSIC", 7) == 0)) {
							m_bEnemyKillMode = false;
							wsprintf(cTxt, "(*) Enemy Kill Mode: CLASSIC");
							PutLogList(cTxt);
						} else if ((memcmp(token, "deathmatch", 10) == 0) || (memcmp(token, "DEATHMATCH", 10) == 0)) {
							m_bEnemyKillMode = true;
							wsprintf(cTxt, "(*) Enemy Kill Mode: DEATHMATCH");
							PutLogList(cTxt);
						} else {
							wsprintf(cTxt, "(!!!) ENEMY-KILL-MODE: (%s) must be either DEATHMATCH or CLASSIC", token);
							PutLogList(cTxt);
							return false;
						}
						cReadMode = 0;
						break;

					case 4:
						if ((strlen(token) != 0) && (strlen(token) <= 100) && (strlen(token) >= 1)) {
							m_iEnemyKillAdjust = atoi(token);
						} else {
							wsprintf(cTxt, "(!!!) ENEMY-KILL-ADJUST: (%s) invalid must be between 1 to 100", token);
							PutLogList(cTxt);
						}
						wsprintf(cTxt, "(*) Player gains (%d) EK per enemy kill.", m_iEnemyKillAdjust);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 5:
						if ((memcmp(token, "on", 2) == 0) || (memcmp(token, "ON", 2) == 0)) {
							m_bAdminSecurity = true;
							wsprintf(cTxt, "(*) Administator actions limited through security.");
							PutLogList(cTxt);
						}
						if ((memcmp(token, "off", 3) == 0) || (memcmp(token, "OFF", 3) == 0)) {
							m_bAdminSecurity = false;
							wsprintf(cTxt, "(*) Administator vulnerability ignored.");
							PutLogList(cTxt);
						}

						cReadMode = 0;
						break;

					case 6:
						m_sRaidTimeMonday = atoi(token);
						if (m_sRaidTimeMonday == 0) m_sRaidTimeMonday = 3;
						wsprintf(cTxt, "(*) Monday Raid Time : (%d) minutes", m_sRaidTimeMonday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 7:
						m_sRaidTimeTuesday = atoi(token);
						if (m_sRaidTimeTuesday == 0) m_sRaidTimeTuesday = 3;
						wsprintf(cTxt, "(*) Tuesday Raid Time : (%d) minutes", m_sRaidTimeTuesday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 8:
						m_sRaidTimeWednesday = atoi(token);
						if (m_sRaidTimeWednesday == 0) m_sRaidTimeWednesday = 3;
						wsprintf(cTxt, "(*) Wednesday Raid Time : (%d) minutes", m_sRaidTimeWednesday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 9:
						m_sRaidTimeThursday = atoi(token);
						if (m_sRaidTimeThursday == 0) m_sRaidTimeThursday = 3;
						wsprintf(cTxt, "(*) Thursday Raid Time : (%d) minutes", m_sRaidTimeThursday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 10:
						m_sRaidTimeFriday = atoi(token);
						if (m_sRaidTimeFriday == 0) m_sRaidTimeFriday = 10;
						wsprintf(cTxt, "(*) Friday Raid Time : (%d) minutes", m_sRaidTimeFriday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 11:
						m_sRaidTimeSaturday = atoi(token);
						if (m_sRaidTimeSaturday == 0) m_sRaidTimeSaturday = 45;
						wsprintf(cTxt, "(*) Saturday Raid Time : (%d) minutes", m_sRaidTimeSaturday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 12:
						m_sRaidTimeSunday = atoi(token);
						if (m_sRaidTimeSunday == 0) m_sRaidTimeSunday = 60;
						wsprintf(cTxt, "(*) Sunday Raid Time : (%d) minutes", m_sRaidTimeSunday);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 13:
						if ((memcmp(token, "player", 6) == 0) || (memcmp(token, "PLAYER", 6) == 0)) {
							m_iLogChatOption = 1;
							wsprintf(cTxt, "(*) Logging Player Chat.");
							PutLogList(cTxt);
						}
						if ((memcmp(token, "gm", 2) == 0) || (memcmp(token, "GM", 2) == 0)) {
							m_iLogChatOption = 2;
							wsprintf(cTxt, "(*) Logging GM Chat.");
							PutLogList(cTxt);
						}
						if ((memcmp(token, "all", 3) == 0) || (memcmp(token, "ALL", 3) == 0)) {
							m_iLogChatOption = 3;
							wsprintf(cTxt, "(*) Logging All Chat.");
							PutLogList(cTxt);
						}
						if ((memcmp(token, "none", 4) == 0) || (memcmp(token, "NONE", 4) == 0)) {
							m_iLogChatOption = 4;
							wsprintf(cTxt, "(*) Not Logging Any Chat.");
							PutLogList(cTxt);
						}
						cReadMode = 0;
						break;

					case 14:
						m_iSummonGuildCost = atoi(token);
						wsprintf(cTxt, "(*) Summoning guild costs (%d) gold", m_iSummonGuildCost);
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 15:
						m_sSlateSuccessRate = atoi(token);
						wsprintf(cTxt, "(*) Slate Success Rate (%d) percent", m_sSlateSuccessRate);
						PutLogList(cTxt);
						if (m_sSlateSuccessRate == 0) m_sSlateSuccessRate = 14;
						cReadMode = 0;
						break;

					case 16:
						m_sCharStatLimit = atoi(token);
						if (m_sCharStatLimit == 0) m_sCharStatLimit = 200;
						cReadMode = 0;
						break;

					case 17:
						m_sCharSkillLimit = atoi(token);
						if (m_sCharSkillLimit == 0) m_sCharSkillLimit = 700;
						cReadMode = 0;
						break;

					case 18:
						m_cRepDropModifier = atoi(token);
						wsprintf(cTxt, "(*) Rep<->Drop modifier: (%d)", m_cRepDropModifier);
						PutLogList(cTxt);
						if (m_cRepDropModifier < 0) m_cRepDropModifier = 0;
						cReadMode = 0;
						break;

					case 19:
						std::memset(m_cSecurityNumber, 0, sizeof (m_cSecurityNumber));
						len = strlen(token);
						if (len > 10) len = 10;
						memcpy(m_cSecurityNumber, token, len);
						wsprintf(cTxt, "(*) Security number memorized!");
						PutLogList(cTxt);
						cReadMode = 0;
						break;

					case 20:
						m_sMaxPlayerLevel = atoi(token);
						if (m_sMaxPlayerLevel == 0) m_sMaxPlayerLevel = 180;
						cReadMode = 0;
						break;

				}
			} else {
				if (memcmp(token, "primary-drop-rate", 17) == 0) cReadMode = 1;
				if (memcmp(token, "secondary-drop-rate", 19) == 0) cReadMode = 2;
				if (memcmp(token, "enemy-kill-mode", 15) == 0) cReadMode = 3;
				if (memcmp(token, "enemy-kill-adjust", 17) == 0) cReadMode = 4;
				if (memcmp(token, "admin-security", 14) == 0) cReadMode = 5;
				if (memcmp(token, "monday-raid-time", 16) == 0) cReadMode = 6;
				if (memcmp(token, "tuesday-raid-time", 17) == 0) cReadMode = 7;
				if (memcmp(token, "wednesday-raid-time", 19) == 0) cReadMode = 8;
				if (memcmp(token, "thursday-raid-time", 18) == 0) cReadMode = 9;
				if (memcmp(token, "friday-raid-time", 16) == 0) cReadMode = 10;
				if (memcmp(token, "saturday-raid-time", 18) == 0) cReadMode = 11;
				if (memcmp(token, "sunday-raid-time", 16) == 0) cReadMode = 12;
				if (memcmp(token, "log-chat-settings", 17) == 0) cReadMode = 13;
				if (memcmp(token, "summonguild-cost", 16) == 0) cReadMode = 14;
				if (memcmp(token, "slate-success-rate", 18) == 0) cReadMode = 15;
				if (memcmp(token, "character-stat-limit", 20) == 0) cReadMode = 16;
				if (memcmp(token, "character-skill-limit", 21) == 0) cReadMode = 17;
				if (memcmp(token, "rep-drop-modifier", 17) == 0) cReadMode = 18;
				if (memcmp(token, "admin-security-code", 19) == 0) cReadMode = 19;
				if (memcmp(token, "max-player-level", 16) == 0) cReadMode = 20;
			}

			token = pStrTok->pGet();
			//token = strtok( nullptr, seps );
		}

		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);

	return true;
}

bool CGame::bReadCrusadeStructureConfigFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadModeA, cReadModeB;
	char seps[] = "= \t\n";
	int iIndex;
	class CStrTok * pStrTok;

	cReadModeA = 0;
	cReadModeB = 0;

	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {

		PutLogList("(!) Cannot open Crusade configuration file.");
		return false;
	} else {
		PutLogList("(!) Reading Crusade configuration file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);

		iIndex = 0;
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadModeA != 0) {
				switch (cReadModeA) {
					case 1:
						switch (cReadModeB) {
							case 1:

								if (_bGetIsStringIsNumber(token) == false) {
									PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format(1).");
									delete cp;
									delete pStrTok;
									return false;
								}
								iIndex = atoi(token);

								if (m_stCrusadeStructures[iIndex].cType != 0) {

									PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Duplicate portion number.");
									delete cp;
									delete pStrTok;
									return false;
								}

								cReadModeB = 2;
								break;

							case 2:

								std::memset(m_stCrusadeStructures[iIndex].cMapName, 0, sizeof (m_stCrusadeStructures[iIndex].cMapName));
								memcpy(m_stCrusadeStructures[iIndex].cMapName, token, strlen(token));
								cReadModeB = 3;
								break;


							case 3:

								if (_bGetIsStringIsNumber(token) == false) {
									PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
									delete cp;
									delete pStrTok;
									return false;
								}
								m_stCrusadeStructures[iIndex].cType = atoi(token);
								cReadModeB = 4;
								break;

							case 4:

								if (_bGetIsStringIsNumber(token) == false) {
									PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
									delete cp;
									delete pStrTok;
									return false;
								}
								m_stCrusadeStructures[iIndex].dX = atoi(token);
								cReadModeB = 5;
								break;

							case 5:

								if (_bGetIsStringIsNumber(token) == false) {
									PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
									delete cp;
									delete pStrTok;
									return false;
								}
								m_stCrusadeStructures[iIndex].dY = atoi(token);
								cReadModeA = 0;
								cReadModeB = 0;
								break;
						}
						break;

					default:
						break;
				}
			} else {
				if (memcmp(token, "crusade-structure", 17) == 0) {
					cReadModeA = 1;
					cReadModeB = 1;
				}

			}
			token = pStrTok->pGet();
		}

		delete pStrTok;
		delete cp;

		if ((cReadModeA != 0) || (cReadModeB != 0)) {
			PutLogList("(!!!) CRITICAL ERROR! Crusade Structure configuration file contents error!");
			return false;
		}
	}

	if (pFile != nullptr) fclose(pFile);
	return true;
}

bool CGame::_bDecodePlayerDatafileContents(int iClientH, char * pData, uint32_t dwSize) {
	char * pContents, * token, * pOriginContents, cTmpName[11], cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA, cReadModeB;
	int i, iItemIndex, iItemInBankIndex, iTotalGold, iNotUsedItemPrice;
	class CStrTok * pStrTok;
	short sTmpType, sTmpAppr1;
	bool bRet;
	int iTemp;
	SYSTEMTIME SysTime;
	__int64 iDateSum1, iDateSum2;
	bool bIsNotUsedItemFound = false;

	if (m_pClientList[iClientH] == nullptr) return false;


	iTotalGold = 0;
	iItemIndex = 0;
	iItemInBankIndex = 0;
	iNotUsedItemPrice = 0;

	cReadModeA = 0;
	cReadModeB = 0;

	pContents = new char[dwSize + 2];
	ZeroMemory(pContents, dwSize + 2);
	memcpy(pContents, pData, dwSize);

	pOriginContents = pContents;

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();

	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:

					std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
					strcpy(m_pClientList[iClientH]->m_cMapName, token);

					std::memset(cTmpName, 0, sizeof (cTmpName));
					strcpy(cTmpName, token);
					for (i = 0; i < DEF_MAXMAPS; i++)
						if ((m_pMapList[i] != nullptr) && (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)) {

							m_pClientList[iClientH]->m_cMapIndex = (char) i;
						}

					if (m_pClientList[iClientH]->m_cMapIndex == -1) {

						wsprintf(cTxt, "(!) Player(%s) tries to enter unknown map : %s", m_pClientList[iClientH]->m_cCharName, cTmpName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					cReadModeA = 0;
					break;

				case 2:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_sX = atoi(token);
					cReadModeA = 0;
					break;

				case 3:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_sY = atoi(token);
					cReadModeA = 0;
					break;

				case 4:

					/*
					if (_bGetIsStringIsNumber(token) == false) {
					wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
					PutLogList(cTxt);
					delete pContents;
					delete pStrTok;
					return false;
					}
					m_pClientList[iClientH]->m_cAccountStatus = atoi(token);
					 */
					cReadModeA = 0;
					break;

				case 5:

					switch (cReadModeB) {
						case 1:
							// New 07/05/2004
							// v2.12
							if (iItemIndex >= DEF_MAXITEMS) {
								delete pContents;
								delete pStrTok;
								return false;
							}


							if (_bInitItemAttr(*m_pClientList[iClientH]->m_pItemList[iItemIndex], token) == false) {

								wsprintf(cTxt, "(!!!) Client(%s)-Item(%s) is not existing Item! Conection closed.", m_pClientList[iClientH]->m_cCharName, token);
								PutLogList(cTxt);

								// Debug code @@@@@@@@@@@@@@@
								HANDLE hFile;
								DWORD nWrite;
								hFile = CreateFile("Error.Log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
								WriteFile(hFile, (char *) pContents, dwSize + 2, &nWrite, nullptr);
								CloseHandle(hFile);
								///////////// @@@@@@@@@@@@@@@

								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;

						case 2:
							// m_dwCount
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}


							iTemp = atoi(token);
							if (iTemp < 0) iTemp = 1;

							if (iGetItemWeight(*m_pClientList[iClientH]->m_pItemList[iItemIndex], iTemp) > _iCalcMaxLoad(iClientH)) {
								iTemp = 1;
								wsprintf(G_cTxt, "(!) Player�(%s) Item (%s) too heavy for player to carry", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName);
								PutLogFileList(G_cTxt);
								PutLogList(G_cTxt);
							}

							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount = (uint32_t) iTemp;
							cReadModeB = 3;


							if (memcmp(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, "Gold", 4) == 0)
								iTotalGold += iTemp;
							break;

						case 3:
							// m_sTouchEffectType
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = atoi(token);
							cReadModeB = 4;
							break;

						case 4:
							// m_sTouchEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = atoi(token);
							cReadModeB = 5;
							break;

						case 5:
							// m_sTouchEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = atoi(token);
							cReadModeB = 6;
							break;

						case 6:
							// m_sTouchEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = atoi(token);
							cReadModeB = 7;
							break;

						case 7:
							// m_cItemColor
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor = atoi(token);
							cReadModeB = 8;
							break;

						case 8:
							// m_sItemSpecEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = atoi(token);
							cReadModeB = 9;
							break;

						case 9:
							// m_sItemSpecEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 = atoi(token);
							cReadModeB = 10;
							break;

						case 10:
							// m_sItemSpecEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue3 = atoi(token);
							cReadModeB = 11;
							break;

						case 11:
							// m_wCurLifeSpan
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan = atoi(token);

							cReadModeB = 12;
							break;

						case 12:
							// m_dwAttribute
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = atoi(token);


							if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER) {
								if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
										  (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
										  (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3)) {

									wsprintf(cTxt, "(!) ´Ù¸¥ »ç¶÷ÀÇ ¾ÆÀÌÅÛ ¼ÒÁö: Player(%s) Item(%s) %d %d %d - %d %d %d", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
											  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1,
											  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2,
											  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3,
											  m_pClientList[iClientH]->m_sCharIDnum1,
											  m_pClientList[iClientH]->m_sCharIDnum2,
											  m_pClientList[iClientH]->m_sCharIDnum3);
									PutLogList(cTxt);
									//PutLogFileList(cTxt);
								}
							}

							cReadModeA = 0;
							cReadModeB = 0;


							if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0) {

								m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;

							}


							_AdjustRareItemValue(*m_pClientList[iClientH]->m_pItemList[iItemIndex]);


							if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan > m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan)
								m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;

							// v1.433
							if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan == 0) &&
									  (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {

								wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¼ö¸í 0Â¥¸® Èñ»ý¼® ¼ÒÁö!", m_pClientList[iClientH]->m_cCharName);
								PutLogFileList(G_cTxt);

								m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan = 1;
							}


							bCheckAndConvertPlusWeaponItem(iClientH, iItemIndex);


							if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType == DEF_ITEMTYPE_NOTUSED) {
								iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wPrice;
								m_pClientList[iClientH]->m_pItemList[iItemIndex].reset();

								// v1.41
								bIsNotUsedItemFound = true;
							} else
								if (_bCheckDupItemID(&*m_pClientList[iClientH]->m_pItemList[iItemIndex]) == true) {

								_bItemLog(DEF_ITEMLOG_DUPITEMID, iClientH, 0, &*m_pClientList[iClientH]->m_pItemList[iItemIndex]);

								iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wPrice;
								m_pClientList[iClientH]->m_pItemList[iItemIndex].reset();
							} else iItemIndex++;
							break;
					}
					break;

				case 6:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_cSex = atoi(token);
					cReadModeA = 0;
					break;

				case 7:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_cSkin = atoi(token);
					cReadModeA = 0;
					break;

				case 8:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_cHairStyle = atoi(token);
					cReadModeA = 0;
					break;

				case 9:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_cHairColor = atoi(token);
					cReadModeA = 0;
					break;

				case 10:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_cUnderwear = atoi(token);
					cReadModeA = 0;
					break;

				case 11:



					for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
						m_pClientList[iClientH]->m_sItemEquipmentStatus[i] = -1;


					for (i = 0; i < DEF_MAXITEMS; i++)
						m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;


					if (bIsNotUsedItemFound == false) {
						for (i = 0; i < DEF_MAXITEMS; i++) {
							wsprintf(cTxt, "%d", token[i]);
							if ((token[i] == '1') && (m_pClientList[iClientH]->m_pItemList[i] != nullptr)) {

								if (m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_EQUIP)
									m_pClientList[iClientH]->m_bIsItemEquipped[i] = true;
								else m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;
							} else m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;

							if ((m_pClientList[iClientH] != nullptr) && (m_pClientList[iClientH]->m_bIsItemEquipped[i] == true)) {

								if (bEquipItemHandler(iClientH, i) == false) // false
									m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;
							}
						}
					}

					cReadModeA = 0;
					break;

				case 12:


					std::memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof (m_pClientList[iClientH]->m_cGuildName));
					strcpy(m_pClientList[iClientH]->m_cGuildName, token);
					cReadModeA = 0;
					break;

				case 13:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iGuildRank = atoi(token);
					cReadModeA = 0;
					break;

				case 14:
					// HP
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iHP = atoi(token);
					cReadModeA = 0;
					break;

				case 15:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					//m_pClientList[iClientH]->m_iOriginDefenseRatio = atoi(token);
					//m_pClientList[iClientH]->m_iCurDefenseRatio = m_pClientList[iClientH]->m_iOriginDefenseRatio;
					cReadModeA = 0;
					break;

				case 16:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					//m_pClientList[iClientH]->m_iOriginHitRatio = atoi(token);
					//m_pClientList[iClientH]->m_iCurHitRatio = m_pClientList[iClientH]->m_iOriginHitRatio;
					cReadModeA = 0;
					break;

				case 17:
					// Level
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iLevel = atoi(token);
					cReadModeA = 0;
					break;

				case 18:
					// Str
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iStr = atoi(token);
					cReadModeA = 0;
					break;

				case 19:
					// Int
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iInt = atoi(token);
					cReadModeA = 0;
					break;

				case 20:
					// Vit
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iVit = atoi(token);
					cReadModeA = 0;
					break;

				case 21:
					// Dex
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iDex = atoi(token);
					cReadModeA = 0;
					break;

				case 22:
					// Mag
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iMag = atoi(token);
					cReadModeA = 0;
					break;

				case 23:
					// Charisma
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iCharisma = atoi(token);
					cReadModeA = 0;
					break;

				case 24:
					// Luck
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iLuck = atoi(token);
					cReadModeA = 0;
					break;

				case 25:
					// Exp
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iExp = atoi(token);
					cReadModeA = 0;
					break;

				case 26:

					for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
						m_pClientList[iClientH]->m_cMagicMastery[i] = token[i] - 48;
					}
					cReadModeA = 0;
					break;

				case 27:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_cSkillMastery[cReadModeB] = atoi(token);
					cReadModeB++;

					if (cReadModeB >= DEF_MAXSKILLTYPE) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;
					//
				case 28:

					switch (cReadModeB) {
						case 1:
							if (iItemInBankIndex >= DEF_MAXBANKITEMS) {
								delete pContents;
								delete pStrTok;
								return false;
							}

							if (_bInitItemAttr(*m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex], token) == false) {

								wsprintf(cTxt, "(!!!) Client(%s)-Bank Item(%s) is not existing Item! Conection closed.", m_pClientList[iClientH]->m_cCharName, token);
								PutLogList(cTxt);

								// Debug code @@@@@@@@@@@@@@@
								HANDLE hFile;
								DWORD nWrite;
								hFile = CreateFile("Error.Log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
								WriteFile(hFile, (char *) pOriginContents, dwSize + 2, &nWrite, nullptr);
								CloseHandle(hFile);
								///////////// @@@@@@@@@@@@@@@

								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;

						case 2:
							// m_dwCount
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}


							iTemp = atoi(token);
							if (iTemp < 0) iTemp = 1;


							if (iGetItemWeight(*m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex], iTemp) > _iCalcMaxLoad(iClientH)) {
								iTemp = 1;
								wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¾ÆÀÌÅÛ(%s) °³¼ö ¿À¹öÇÃ·Î¿ì", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cName);
								PutLogFileList(G_cTxt);
								PutLogList(G_cTxt);
							}

							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwCount = (uint32_t) iTemp;
							cReadModeB = 3;


							if (memcmp(m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cName, "Gold", 4) == 0)
								iTotalGold += iTemp;
							break;

						case 3:
							// m_sTouchEffectType
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectType = atoi(token);
							cReadModeB = 4;
							break;

						case 4:
							// m_sTouchEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue1 = atoi(token);
							cReadModeB = 5;
							break;

						case 5:
							// m_sTouchEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue2 = atoi(token);
							cReadModeB = 6;
							break;

						case 6:
							// m_sTouchEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue3 = atoi(token);
							cReadModeB = 7;
							break;

						case 7:
							// m_cItemColor
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cItemColor = atoi(token);
							cReadModeB = 8;
							break;

						case 8:
							// m_sItemSpecEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1 = atoi(token);
							cReadModeB = 9;
							break;

						case 9:
							// m_sItemSpecEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue2 = atoi(token);
							cReadModeB = 10;
							break;

						case 10:
							// m_sItemSpecEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue3 = atoi(token);
							cReadModeB = 11;
							break;

						case 11:
							// m_wCurLifeSpan
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = atoi(token);

							cReadModeB = 12;
							break;


						case 12:
							// m_dwAttribute
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;


							if ((m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute & 0x00000001) != 0) {

								m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1;


							}


							int iValue = (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute & 0xF0000000) >> 28;
							if (iValue > 0) {

								switch (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cCategory) {
									case 5: // ¹æÆÐ
									case 6: // ¹æ¾î±¸
										m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1;
										break;
								}
							}


							_AdjustRareItemValue(*m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]);


							if (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan > m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan)
								m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan;


							// v1.433
							if ((m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan == 0) &&
									  (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {

								wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¼ö¸í 0Â¥¸® Èñ»ý¼® ¼ÒÁö!", m_pClientList[iClientH]->m_cCharName);
								PutLogFileList(G_cTxt);

								m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = 1;
							}


							if (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cItemType == DEF_ITEMTYPE_NOTUSED) {
								iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wPrice;
								m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex].reset();
							} else
								if (_bCheckDupItemID(&*m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]) == true) {

								_bItemLog(DEF_ITEMLOG_DUPITEMID, iClientH, 0, &*m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]);

								iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wPrice;
								m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex].reset();
							} else iItemInBankIndex++;
							break;
					}
					break;
					//

				case 29:

					std::memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof (m_pClientList[iClientH]->m_cLocation));
					strcpy(m_pClientList[iClientH]->m_cLocation, token);
					if (memcmp(m_pClientList[iClientH]->m_cLocation + 3, "hunter", 6) == 0)
						m_pClientList[iClientH]->m_bIsPlayerCivil = true;
					cReadModeA = 0;
					break;

					/* 2.03 Code - Fixed by KLKS
								case 29:
									
									std::memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof(m_pClientList[iClientH]->m_cLocation));
									strcpy(m_pClientList[iClientH]->m_cLocation, token);
									cReadModeA = 0;
									break;
					 */
				case 30:
					// m_iMP
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iMP = atoi(token);
					cReadModeA = 0;
					break;

				case 31:
					// m_iSP
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iSP = atoi(token);
					cReadModeA = 0;
					break;

				case 32:
					// m_cLU_Pool
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iLU_Pool = atoi(token);
					cReadModeA = 0;
					break;
					/*
					case 33:
						// m_cLU_Vit
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						m_pClientList[iClientH]->m_cLU_Vit = atoi(token);
						cReadModeA = 0;
						break;

					case 34:
						// m_cLU_Dex
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						m_pClientList[iClientH]->m_cLU_Dex = atoi(token);
						cReadModeA = 0;
						break;

					case 35:
						// m_cLU_Int
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						m_pClientList[iClientH]->m_cLU_Int = atoi(token);
						cReadModeA = 0;
						break;

					case 36:
						// m_cLU_Mag
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						m_pClientList[iClientH]->m_cLU_Mag = atoi(token);
						cReadModeA = 0;
						break;

					case 37:
						// m_cLU_Char
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						m_pClientList[iClientH]->m_cLU_Char = atoi(token);
						cReadModeA = 0;
						break;
					 */
				case 38:
					// m_iEnemyKillCount
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iEnemyKillCount = atoi(token);
					cReadModeA = 0;
					break;

				case 39:
					// m_iPKCount
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iPKCount = atoi(token);
					cReadModeA = 0;
					break;

				case 40:
					// m_iRewardGold
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iRewardGold = atoi(token);
					cReadModeA = 0;
					break;

				case 41:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iSkillSSN[cReadModeB] = atoi(token);
					cReadModeB++;

					if (cReadModeB >= DEF_MAXSKILLTYPE) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;

				case 42:
					if (token != nullptr) {
						std::memset(m_pClientList[iClientH]->m_cProfile, 0, sizeof (m_pClientList[iClientH]->m_cProfile));
						strcpy(m_pClientList[iClientH]->m_cProfile, token);
					}
					cReadModeA = 0;
					break;

				case 43:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iHungerStatus = atoi(token);
					cReadModeA = 0;
					break;

				case 44:
					// AdminUserLevel
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iAdminUserLevel = 0; //Default it to 0
					if (atoi(token) > 0) {
						for (i = 0; i < DEF_MAXADMINS; i++) {
							if (strlen(m_stAdminList[i].m_cGMName) == 0) break; //No more GM's on list
							if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(m_pClientList[iClientH]->m_cCharName))) {
								if (memcmp(m_stAdminList[i].m_cGMName, m_pClientList[iClientH]->m_cCharName, strlen(m_pClientList[iClientH]->m_cCharName)) == 0) {
									m_pClientList[iClientH]->m_iAdminUserLevel = atoi(token);
									break; //Break goes to cReadModeA = 0;, so no need to do it again
								}
							}
						}
					}
					cReadModeA = 0;
					break;

					/*case 44:
					// AdminUserLevel
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					for (i = 0; i < DEF_MAXADMINS; i++) {
						if (atoi(token) > 0) {
							if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(m_pClientList[iClientH]->m_cCharName))) {
								m_pClientList[iClientH]->m_iAdminUserLevel = atoi(token);
								cReadModeA = 0;
								break;
							}
							else {
								m_pClientList[iClientH]->m_iAdminUserLevel = 0;
							}
						else m_pClientList[iClientH]->m_iAdminUserLevel = 0;
						}
					}
					cReadModeA = 0;
					break;*/

				case 45:
					// TimeLeft_ShutUp
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iTimeLeft_ShutUp = atoi(token);
					cReadModeA = 0;
					break;

				case 46:
					// TimeLeft_Rating
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iTimeLeft_Rating = atoi(token);
					cReadModeA = 0;
					break;

				case 47:
					// Rating
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iRating = atoi(token);
					cReadModeA = 0;
					break;

				case 48:

					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iGuildGUID = atoi(token);
					cReadModeA = 0;
					break;

				case 49:
					// Down Skill Index
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pClientList[iClientH]->m_iDownSkillIndex = atoi(token);
					cReadModeA = 0;
					break;

				case 50:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].x = atoi(token);
					if (m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].x < -10) m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].x = -10; //KaoZureS adaptado darmart123
					cReadModeB++;
					if (cReadModeB > 50) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;

				case 51:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].y = atoi(token);

					if (m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].y < -10) m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].y = -10;
					cReadModeB++;
					if (cReadModeB > 50) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;

				case 52:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_sCharIDnum1 = atoi(token);
					cReadModeA = 0;
					break;

				case 53:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_sCharIDnum2 = atoi(token);
					cReadModeA = 0;
					break;

				case 54:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_sCharIDnum3 = atoi(token);
					cReadModeA = 0;
					break;

				case 55:
					switch (cReadModeB) {
						case 1:
							// Penalty Block Year
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_iPenaltyBlockYear = atoi(token);

							cReadModeB = 2;
							break;

						case 2:
							// Penalty Block Month
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_iPenaltyBlockMonth = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							// Penalty Block day
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_iPenaltyBlockDay = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 56:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iQuest = atoi(token);
					cReadModeA = 0;
					break;

				case 57:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iCurQuestCount = atoi(token);
					cReadModeA = 0;
					break;

				case 58:
					cReadModeA = 0;
					break;

				case 59:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iQuestRewardType = atoi(token);
					cReadModeA = 0;
					break;

				case 60:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iQuestRewardAmount = atoi(token);
					cReadModeA = 0;
					break;

				case 61:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iContribution = atoi(token);
					cReadModeA = 0;
					break;

				case 62:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iQuestID = atoi(token);
					cReadModeA = 0;
					break;

				case 63:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_bIsQuestCompleted = (bool) atoi(token);
					cReadModeA = 0;
					break;

				case 64:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = (bool) atoi(token);
					cReadModeA = 0;
					break;

				case 65:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar = (bool) atoi(token);
					cReadModeA = 0;
					break;

				case 66:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iSpecialEventID = (bool) atoi(token);
					cReadModeA = 0;
					break;


				case 67:
					switch (cReadModeB) {
						case 1:
							// FightZone Number
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_iFightzoneNumber = atoi(token);

							cReadModeB = 2;
							break;

						case 2:

							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_iReserveTime = atoi(token);


							cReadModeB = 3;
							break;
						case 3:

							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pClientList[iClientH]->m_iFightZoneTicketNumber = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;

					}
					break;

				case 70:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iSuperAttackLeft = (bool) atoi(token);
					cReadModeA = 0;
					break;

				case 71:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iSpecialAbilityTime = atoi(token);
					cReadModeA = 0;
					break;

				case 72:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iWarContribution = atoi(token);
					cReadModeA = 0;
					break;

				case 73:
					if (strlen(token) > 10) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					std::memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof (m_pClientList[iClientH]->m_cLockedMapName));
					strcpy(m_pClientList[iClientH]->m_cLockedMapName, token);
					cReadModeA = 0;
					break;

				case 74:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iLockedMapTime = atoi(token);
					cReadModeA = 0;
					break;

				case 75:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iCrusadeDuty = atoi(token);
					cReadModeA = 0;
					break;

				case 76:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iConstructionPoint = atoi(token);
					cReadModeA = 0;
					break;

				case 77:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_dwCrusadeGUID = atoi(token);
					cReadModeA = 0;
					break;

				case 78:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iDeadPenaltyTime = atoi(token);
					cReadModeA = 0;
					break;

				case 79: // v2.06 12-4
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iPartyID = atoi(token);
					if (m_pClientList[iClientH]->m_iPartyID != 0) m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
					cReadModeA = 0;
					break;

				case 80:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iGizonItemUpgradeLeft = atoi(token);
					cReadModeA = 0;
					break;
				case 81:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! (BallPoints) - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pClientList[iClientH]->m_iBallPoints = atoi(token);
					cReadModeA = 0;
					break;
			}
		} else {
			if (memcmp(token, "character-loc-map", 17) == 0) cReadModeA = 1;
			if (memcmp(token, "character-loc-x", 15) == 0) cReadModeA = 2;
			if (memcmp(token, "character-loc-y", 15) == 0) cReadModeA = 3;
			if (memcmp(token, "character-account-status", 21) == 0) cReadModeA = 4;
			if (memcmp(token, "character-item", 14) == 0) {
				cReadModeA = 5;
				cReadModeB = 1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex].reset(new CItem);
			}

			if (memcmp(token, "character-bank-item", 18) == 0) {
				cReadModeA = 28;
				cReadModeB = 1;
				m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex].reset(new class CItem);
			}

			if (memcmp(token, "sex-status", 10) == 0) cReadModeA = 6;
			if (memcmp(token, "skin-status", 11) == 0) cReadModeA = 7;
			if (memcmp(token, "hairstyle-status", 16) == 0) cReadModeA = 8;
			if (memcmp(token, "haircolor-status", 16) == 0) cReadModeA = 9;
			if (memcmp(token, "underwear-status", 16) == 0) cReadModeA = 10;

			if (memcmp(token, "item-equip-status", 17) == 0) cReadModeA = 11;
			if (memcmp(token, "character-guild-name", 20) == 0) cReadModeA = 12;
			if (memcmp(token, "character-guild-rank", 20) == 0) cReadModeA = 13;
			if (memcmp(token, "character-HP", 12) == 0) cReadModeA = 14;
			if (memcmp(token, "character-DefenseRatio", 22) == 0) cReadModeA = 15;
			if (memcmp(token, "character-HitRatio", 18) == 0) cReadModeA = 16;
			if (memcmp(token, "character-LEVEL", 15) == 0) cReadModeA = 17;
			if (memcmp(token, "character-STR", 13) == 0) cReadModeA = 18;
			if (memcmp(token, "character-INT", 13) == 0) cReadModeA = 19;
			if (memcmp(token, "character-VIT", 13) == 0) cReadModeA = 20;
			if (memcmp(token, "character-DEX", 13) == 0) cReadModeA = 21;
			if (memcmp(token, "character-MAG", 13) == 0) cReadModeA = 22;
			if (memcmp(token, "character-CHARISMA", 18) == 0) cReadModeA = 23;
			if (memcmp(token, "character-LUCK", 14) == 0) cReadModeA = 24;
			if (memcmp(token, "character-EXP", 13) == 0) cReadModeA = 25;
			if (memcmp(token, "magic-mastery", 13) == 0) cReadModeA = 26;

			if (memcmp(token, "skill-mastery", 13) == 0) {
				cReadModeA = 27;
				cReadModeB = 0;
			}

			if (memcmp(token, "character-location", 18) == 0) cReadModeA = 29;

			if (memcmp(token, "character-MP", 12) == 0) cReadModeA = 30;
			if (memcmp(token, "character-SP", 12) == 0) cReadModeA = 31;

			if (memcmp(token, "character-LU_Pool", 17) == 0) cReadModeA = 32;

			/*
			if (memcmp(token, "character-LU_Str", 16) == 0)     cReadModeA = 32;
			if (memcmp(token, "character-LU_Vit", 16) == 0)     cReadModeA = 33;
			if (memcmp(token, "character-LU_Dex", 16) == 0)     cReadModeA = 34;
			if (memcmp(token, "character-LU_Int", 16) == 0)     cReadModeA = 35;
			if (memcmp(token, "character-LU_Mag", 16) == 0)     cReadModeA = 36;
			if (memcmp(token, "character-LU_Char",17) == 0)     cReadModeA = 37;
			 */

			if (memcmp(token, "character-EK-Count", 18) == 0) cReadModeA = 38;
			if (memcmp(token, "character-PK-Count", 18) == 0) cReadModeA = 39;

			if (memcmp(token, "character-reward-gold", 21) == 0) cReadModeA = 40;
			if (memcmp(token, "skill-SSN", 9) == 0) cReadModeA = 41;
			if (memcmp(token, "character-profile", 17) == 0) cReadModeA = 42;

			if (memcmp(token, "hunger-status", 13) == 0) cReadModeA = 43;
			if (memcmp(token, "admin-user-level", 16) == 0) cReadModeA = 44;
			if (memcmp(token, "timeleft-shutup", 15) == 0) cReadModeA = 45;
			if (memcmp(token, "timeleft-rating", 15) == 0) cReadModeA = 46;
			if (memcmp(token, "character-RATING", 16) == 0) cReadModeA = 47;

			if (memcmp(token, "character-guild-GUID", 20) == 0) cReadModeA = 48;

			if (memcmp(token, "character-downskillindex", 24) == 0) cReadModeA = 49;
			if (memcmp(token, "item-position-x", 16) == 0) {
				cReadModeA = 50;
				cReadModeB = 1;
			}
			if (memcmp(token, "item-position-y", 16) == 0) {
				cReadModeA = 51;
				cReadModeB = 1;
			}
			if (memcmp(token, "character-IDnum1", 16) == 0) cReadModeA = 52;
			if (memcmp(token, "character-IDnum2", 16) == 0) cReadModeA = 53;
			if (memcmp(token, "character-IDnum3", 16) == 0) cReadModeA = 54;
			if (memcmp(token, "penalty-block-date", 18) == 0) {
				cReadModeA = 55;
				cReadModeB = 1;
			}

			if (memcmp(token, "character-quest-number", 22) == 0) cReadModeA = 56;
			if (memcmp(token, "current-quest-count", 19) == 0) cReadModeA = 57;

			if (memcmp(token, "quest-reward-type", 17) == 0) cReadModeA = 59;
			if (memcmp(token, "quest-reward-amount", 19) == 0) cReadModeA = 60;

			if (memcmp(token, "character-contribution", 22) == 0) cReadModeA = 61;
			if (memcmp(token, "character-quest-ID", 18) == 0) cReadModeA = 62;
			if (memcmp(token, "character-quest-completed", 25) == 0) cReadModeA = 63;

			if (memcmp(token, "timeleft-force-recall", 21) == 0) cReadModeA = 64;
			if (memcmp(token, "timeleft-firm-staminar", 22) == 0) cReadModeA = 65;
			if (memcmp(token, "special-event-id", 16) == 0) cReadModeA = 66;
			if (memcmp(token, "super-attack-left", 17) == 0) cReadModeA = 70;


			if (memcmp(token, "reserved-fightzone-id", 21) == 0) {
				cReadModeA = 67;
				cReadModeB = 1;
			}

			if (memcmp(token, "special-ability-time", 20) == 0) cReadModeA = 71;
			if (memcmp(token, "character-war-contribution", 26) == 0) cReadModeA = 72;

			if (memcmp(token, "locked-map-name", 15) == 0) cReadModeA = 73;
			if (memcmp(token, "locked-map-time", 15) == 0) cReadModeA = 74;
			if (memcmp(token, "crusade-job", 11) == 0) cReadModeA = 75;
			if (memcmp(token, "construct-point", 15) == 0) cReadModeA = 76;
			if (memcmp(token, "crusade-GUID", 12) == 0) cReadModeA = 77;

			if (memcmp(token, "dead-penalty-time", 17) == 0) cReadModeA = 78;
			if (memcmp(token, "party-id", 8) == 0) cReadModeA = 79; // v2.06 12-4
			if (memcmp(token, "gizon-item-upgade-left", 22) == 0) cReadModeA = 80;
			if (memcmp(token, "character-ball-points", 21) == 0) cReadModeA = 81;
			if (memcmp(token, "[EOF]", 5) == 0) goto DPDC_STOP_DECODING;
		}

		token = pStrTok->pGet();
		//token = strtok( nullptr, seps );
	}

DPDC_STOP_DECODING:
	;

	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		wsprintf(cTxt, "(!!!) Player(%s) data file contents error(%d %d)! Connection closed.", m_pClientList[iClientH]->m_cCharName, cReadModeA, cReadModeB);
		PutLogList(cTxt);

		// Debug code @@@@@@@@@@@@@@@
		HANDLE hFile2;
		DWORD nWrite2;
		hFile2 = CreateFile("Error.Log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		WriteFile(hFile2, (char *) pOriginContents, dwSize + 2, &nWrite2, nullptr);
		CloseHandle(hFile2);
		///////////// @@@@@@@@@@@@@@@

		return false;
	}


	bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bIsValidLoc(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
	if (bRet == false) {
		if ((m_pClientList[iClientH]->m_sX != -1) || (m_pClientList[iClientH]->m_sY != -1)) {
			wsprintf(cTxt, "Invalid location error! %s (%d, %d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
			PutLogList(cTxt);
			return false;
		}
	}


	/*	if ((m_pClientList[iClientH]->m_cLU_Str > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Str < 0))
	return false;

	if ((m_pClientList[iClientH]->m_cLU_Vit > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Vit < 0))
	return false;

	if ((m_pClientList[iClientH]->m_cLU_Dex > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Dex < 0))
	return false;

	if ((m_pClientList[iClientH]->m_cLU_Int > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Int < 0))
	return false;

	if ((m_pClientList[iClientH]->m_cLU_Mag > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Mag < 0))
	return false;

	if ((m_pClientList[iClientH]->m_cLU_Char > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Char < 0))
	return false;
	 */
	//	if ( (m_pClientList[iClientH]->m_cLU_Str + m_pClientList[iClientH]->m_cLU_Vit + m_pClientList[iClientH]->m_cLU_Dex +
	//  		  m_pClientList[iClientH]->m_cLU_Int + m_pClientList[iClientH]->m_cLU_Mag + m_pClientList[iClientH]->m_cLU_Char) > DEF_TOTALLEVELUPPOINT)
	if ((m_pClientList[iClientH]->m_iLU_Pool < 0) || (m_pClientList[iClientH]->m_iLU_Pool > DEF_CHARPOINTLIMIT))
		return false;

	if ((m_pClientList[iClientH]->m_iStr < 10) || (m_pClientList[iClientH]->m_iStr > DEF_CHARPOINTLIMIT))
		return false;

	if ((m_pClientList[iClientH]->m_iDex < 10) || (m_pClientList[iClientH]->m_iDex > DEF_CHARPOINTLIMIT))
		return false;

	if ((m_pClientList[iClientH]->m_iVit < 10) || (m_pClientList[iClientH]->m_iVit > DEF_CHARPOINTLIMIT))
		return false;

	if ((m_pClientList[iClientH]->m_iInt < 10) || (m_pClientList[iClientH]->m_iInt > DEF_CHARPOINTLIMIT))
		return false;

	if ((m_pClientList[iClientH]->m_iMag < 10) || (m_pClientList[iClientH]->m_iMag > DEF_CHARPOINTLIMIT))
		return false;

	if ((m_pClientList[iClientH]->m_iCharisma < 10) || (m_pClientList[iClientH]->m_iCharisma > DEF_CHARPOINTLIMIT))
		return false;


	//	if ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iVit +
	//		 m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma)
	//		 !=	((m_pClientList[iClientH]->m_iLevel-1)*3 + 70)) return false;



	//if ((m_pClientList[iClientH]->m_cAccountStatus != 2) && (m_pClientList[iClientH]->m_iLevel > DEF_LEVELLIMIT))
	//	return false;


	if ((m_Misc.bCheckValidName(m_pClientList[iClientH]->m_cCharName) == false) || (m_Misc.bCheckValidName(m_pClientList[iClientH]->m_cAccountName) == false))
		return false;


	if (m_pClientList[iClientH]->m_iPenaltyBlockYear != 0) {
		GetLocalTime(&SysTime);
		iDateSum1 = (__int64) (m_pClientList[iClientH]->m_iPenaltyBlockYear * 10000 + m_pClientList[iClientH]->m_iPenaltyBlockMonth * 100 + m_pClientList[iClientH]->m_iPenaltyBlockDay);
		iDateSum2 = (__int64) (SysTime.wYear * 10000 + SysTime.wMonth * 100 + SysTime.wDay);
		if (iDateSum1 >= iDateSum2) return false;
	}


	if (m_pClientList[iClientH]->m_iReserveTime != 0) {
		GetLocalTime(&SysTime);
		iDateSum1 = (__int64) m_pClientList[iClientH]->m_iReserveTime;
		iDateSum2 = (__int64) (SysTime.wMonth * 10000 + SysTime.wDay * 100 + SysTime.wHour);
		if (iDateSum2 >= iDateSum1) {

			SendNotifyMsg(0, i, DEF_NOTIFY_FIGHTZONERESERVE, -2, 0, 0, nullptr);
			m_pClientList[iClientH]->m_iFightzoneNumber = 0;
			m_pClientList[iClientH]->m_iReserveTime = 0;
			m_pClientList[iClientH]->m_iFightZoneTicketNumber = 0;
		}
	}


	if (m_pClientList[iClientH]->m_iAdminUserLevel < 0) m_pClientList[iClientH]->m_iAdminUserLevel = 0;

	// ============================================================================================================



	if (m_pClientList[iClientH]->m_cSex == 1) {

		sTmpType = 1;
	} else if (m_pClientList[iClientH]->m_cSex == 2) {

		sTmpType = 4;
	}

	switch (m_pClientList[iClientH]->m_cSkin) {
		case 1:

			break;
		case 2:
			sTmpType += 1;
			break;
		case 3:
			sTmpType += 2;
			break;
	}


	if (m_pClientList[iClientH]->m_iAdminUserLevel >= 10)
		sTmpType = m_pClientList[iClientH]->m_iAdminUserLevel;

	sTmpAppr1 = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);

	m_pClientList[iClientH]->m_sType = sTmpType;
	m_pClientList[iClientH]->m_sAppr1 = sTmpAppr1;


	iCalcTotalWeight(iClientH);


	//if (iTotalGold > 800000) {

	//PutLogFileList(G_cTxt);
	//}

	// v.135
	/*
	if ((m_pClientList[iClientH]->m_cSkillMastery[0] >= 70) || (m_pClientList[iClientH]->m_cSkillMastery[1] >= 70) ||
	(m_pClientList[iClientH]->m_cSkillMastery[12] >= 70)) {
	wsprintf(G_cTxt, "(!) ÇØÅ· ¿ëÀÇÀÚ(%s) ³ôÀº ½ºÅ³ (%d %d %d)", m_pClientList[iClientH]->m_cCharName,
	m_pClientList[iClientH]->m_cSkillMastery[0], m_pClientList[iClientH]->m_cSkillMastery[1],
	m_pClientList[iClientH]->m_cSkillMastery[12]);
	PutLogFileList(G_cTxt);
	}
	 */

	if (m_pClientList[iClientH]->m_sCharIDnum1 == 0) {

		int _i, _iTemp1, _iTemp2;
		short _sID1, _sID2, _sID3;

		_iTemp1 = 1;
		_iTemp2 = 1;
		for (_i = 0; _i < 10; _i++) {
			_iTemp1 += m_pClientList[iClientH]->m_cCharName[_i];
			_iTemp2 += abs(m_pClientList[iClientH]->m_cCharName[_i] ^ m_pClientList[iClientH]->m_cCharName[_i]);
		}

		_sID1 = (short) timeGetTime();
		_sID2 = (short) _iTemp1;
		_sID3 = (short) _iTemp2;

		m_pClientList[iClientH]->m_sCharIDnum1 = _sID1;
		m_pClientList[iClientH]->m_sCharIDnum2 = _sID2;
		m_pClientList[iClientH]->m_sCharIDnum3 = _sID3;
	}


	m_pClientList[iClientH]->m_iRewardGold += iNotUsedItemPrice;

	//
	m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;

	// v1.41
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) m_pClientList[iClientH]->m_bIsNeutral = true;

	return true;
}

int CGame::_iComposePlayerDataFileContents(int iClientH, char * pData) {
	SYSTEMTIME SysTime;
	char cTxt[120], cTmp[21];
	POINT TempItemPosList[DEF_MAXITEMS];
	int i, iPos;

	if (m_pClientList[iClientH] == nullptr) return 0;

	GetLocalTime(&SysTime);
	strcat(pData, "[FILE-DATE]\n\n");

	wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	strcat(pData, "[NAME-ACCOUNT]\n\n");
	strcat(pData, "character-name     = ");
	strcat(pData, m_pClientList[iClientH]->m_cCharName);
	strcat(pData, "\n");
	strcat(pData, "account-name       = ");
	strcat(pData, m_pClientList[iClientH]->m_cAccountName);
	strcat(pData, "\n\n");

	strcat(pData, "[STATUS]\n\n");

	strcat(pData, "character-profile 	=");
	if (strlen(m_pClientList[iClientH]->m_cProfile) == 0) {

		strcat(pData, "__________");
	} else strcat(pData, m_pClientList[iClientH]->m_cProfile);
	strcat(pData, "\n");


	strcat(pData, "character-location   = ");
	strcat(pData, m_pClientList[iClientH]->m_cLocation);
	strcat(pData, "\n");


	strcat(pData, "character-guild-name = ");
	if (m_pClientList[iClientH]->m_iGuildRank != -1) {

		strcat(pData, m_pClientList[iClientH]->m_cGuildName);
	} else strcat(pData, "NONE");
	strcat(pData, "\n");


	strcat(pData, "character-guild-GUID = ");
	if (m_pClientList[iClientH]->m_iGuildRank != -1) {

		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d", m_pClientList[iClientH]->m_iGuildGUID);
		strcat(pData, cTxt);
	} else strcat(pData, "-1");
	strcat(pData, "\n");


	strcat(pData, "character-guild-rank = ");
	itoa(m_pClientList[iClientH]->m_iGuildRank, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	strcat(pData, "character-loc-map = ");
	strcat(pData, m_pClientList[iClientH]->m_cMapName);
	strcat(pData, "\n");

	strcat(pData, "character-loc-x   = ");
	itoa(m_pClientList[iClientH]->m_sX, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	strcat(pData, "character-loc-y   = ");
	itoa(m_pClientList[iClientH]->m_sY, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");
	//
	if (m_pClientList[iClientH]->m_iHP <= 0)
		m_pClientList[iClientH]->m_iHP = iGetMaxHP(iClientH); //50Cent - 30HP CL Fix

	wsprintf(cTxt, "character-HP       = %d", m_pClientList[iClientH]->m_iHP);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-MP       = %d", m_pClientList[iClientH]->m_iMP);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	if (m_pClientList[iClientH]->m_iSP < 0) m_pClientList[iClientH]->m_iSP = 0; // v1.1
	wsprintf(cTxt, "character-SP       = %d", m_pClientList[iClientH]->m_iSP);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-LEVEL    = %d", m_pClientList[iClientH]->m_iLevel);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-RATING   = %d", m_pClientList[iClientH]->m_iRating);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-STR      = %d", m_pClientList[iClientH]->m_iStr);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-INT      = %d", m_pClientList[iClientH]->m_iInt);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-VIT      = %d", m_pClientList[iClientH]->m_iVit);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-DEX      = %d", m_pClientList[iClientH]->m_iDex);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-MAG      = %d", m_pClientList[iClientH]->m_iMag);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-CHARISMA = %d", m_pClientList[iClientH]->m_iCharisma);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-LUCK     = %d", m_pClientList[iClientH]->m_iLuck);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-EXP      = %d", m_pClientList[iClientH]->m_iExp);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-LU_Pool  = %d", m_pClientList[iClientH]->m_iLU_Pool);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	/*
	wsprintf(cTxt, "character-LU_Str   = %d", m_pClientList[iClientH]->m_cLU_Str);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "character-LU_Vit   = %d", m_pClientList[iClientH]->m_cLU_Vit);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "character-LU_Dex   = %d", m_pClientList[iClientH]->m_cLU_Dex);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "character-LU_Int   = %d", m_pClientList[iClientH]->m_cLU_Int);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "character-LU_Mag   = %d", m_pClientList[iClientH]->m_cLU_Mag);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "character-LU_Char  = %d", m_pClientList[iClientH]->m_cLU_Char);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	 */
	wsprintf(cTxt, "character-EK-Count = %d", m_pClientList[iClientH]->m_iEnemyKillCount);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-PK-Count = %d", m_pClientList[iClientH]->m_iPKCount);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-reward-gold = %d", m_pClientList[iClientH]->m_iRewardGold);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-downskillindex = %d", m_pClientList[iClientH]->m_iDownSkillIndex);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-IDnum1 = %d", m_pClientList[iClientH]->m_sCharIDnum1);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-IDnum2 = %d", m_pClientList[iClientH]->m_sCharIDnum2);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-IDnum3 = %d", m_pClientList[iClientH]->m_sCharIDnum3);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	/*
	wsprintf(cTxt, "party-rank = %d", m_pClientList[iClientH]->m_iPartyRank);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "party-IDnum1 = %d", m_pClientList[iClientH]->m_sPartyIDnum1);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "party-IDnum2 = %d", m_pClientList[iClientH]->m_sPartyIDnum2);
	strcat(pData, cTxt);
	strcat(pData,"\n");

	wsprintf(cTxt, "party-IDnum3 = %d", m_pClientList[iClientH]->m_sPartyIDnum3);
	strcat(pData, cTxt);
	strcat(pData,"\n\n");
	 */

	//

	strcat(pData, "sex-status       = ");
	itoa(m_pClientList[iClientH]->m_cSex, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "skin-status      = ");
	itoa(m_pClientList[iClientH]->m_cSkin, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "hairstyle-status = ");
	itoa(m_pClientList[iClientH]->m_cHairStyle, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "haircolor-status = ");
	itoa(m_pClientList[iClientH]->m_cHairColor, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "underwear-status = ");
	itoa(m_pClientList[iClientH]->m_cUnderwear, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "hunger-status    = %d", m_pClientList[iClientH]->m_iHungerStatus);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "timeleft-shutup  = %d", m_pClientList[iClientH]->m_iTimeLeft_ShutUp);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "timeleft-rating  = %d", m_pClientList[iClientH]->m_iTimeLeft_Rating);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "timeleft-force-recall  = %d", m_pClientList[iClientH]->m_iTimeLeft_ForceRecall);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "timeleft-firm-staminar = %d", m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "admin-user-level = %d", m_pClientList[iClientH]->m_iAdminUserLevel);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "penalty-block-date = %d %d %d", m_pClientList[iClientH]->m_iPenaltyBlockYear, m_pClientList[iClientH]->m_iPenaltyBlockMonth, m_pClientList[iClientH]->m_iPenaltyBlockDay);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-quest-number = %d", m_pClientList[iClientH]->m_iQuest);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-quest-ID     = %d", m_pClientList[iClientH]->m_iQuestID);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "current-quest-count    = %d", m_pClientList[iClientH]->m_iCurQuestCount);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "quest-reward-type      = %d", m_pClientList[iClientH]->m_iQuestRewardType);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "quest-reward-amount    = %d", m_pClientList[iClientH]->m_iQuestRewardAmount);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-contribution = %d", m_pClientList[iClientH]->m_iContribution);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-war-contribution = %d", m_pClientList[iClientH]->m_iWarContribution);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-quest-completed = %d", (int) m_pClientList[iClientH]->m_bIsQuestCompleted);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "special-event-id = %d", (int) m_pClientList[iClientH]->m_iSpecialEventID);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "super-attack-left = %d", (int) m_pClientList[iClientH]->m_iSuperAttackLeft);
	strcat(pData, cTxt);
	strcat(pData, "\n");



	wsprintf(cTxt, "reserved-fightzone-id = %d %d %d", m_pClientList[iClientH]->m_iFightzoneNumber, m_pClientList[iClientH]->m_iReserveTime, m_pClientList[iClientH]->m_iFightZoneTicketNumber);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "special-ability-time = %d", m_pClientList[iClientH]->m_iSpecialAbilityTime);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "locked-map-name = %s", m_pClientList[iClientH]->m_cLockedMapName);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "locked-map-time = %d", m_pClientList[iClientH]->m_iLockedMapTime);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "crusade-job = %d", m_pClientList[iClientH]->m_iCrusadeDuty);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "crusade-GUID = %d", m_pClientList[iClientH]->m_dwCrusadeGUID);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "construct-point = %d", m_pClientList[iClientH]->m_iConstructionPoint);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "dead-penalty-time = %d", m_pClientList[iClientH]->m_iDeadPenaltyTime);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "party-id = 0", m_pClientList[iClientH]->m_iPartyID);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	wsprintf(cTxt, "character-ball-points = %d", m_pClientList[iClientH]->m_iBallPoints);
	strcat(pData, cTxt);
	strcat(pData, "\n");


	wsprintf(cTxt, "gizon-item-upgade-left = %d", m_pClientList[iClientH]->m_iGizonItemUpgradeLeft);
	strcat(pData, cTxt);
	strcat(pData, "\n");

	/*wsprintf(cTxt, "gizon-item-upgade-left = %d", m_pClientList[iClientH]->m_iGizonItemUpgradeLeft);
	strcat(pData, cTxt);
	strcat(pData,"\n");*/

	strcat(pData, "\n\n");


	strcat(pData, "appr1 = ");
	itoa(m_pClientList[iClientH]->m_sAppr1, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "appr2 = ");

	//m_pClientList[iClientH]->m_sAppr2 = m_pClientList[iClientH]->m_sAppr2 & 0x0FFF;
	itoa(m_pClientList[iClientH]->m_sAppr2, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "appr3 = ");
	itoa(m_pClientList[iClientH]->m_sAppr3, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "appr4 = ");
	itoa(m_pClientList[iClientH]->m_sAppr4, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	// v1.4 ApprColor
	strcat(pData, "appr-color = ");
	itoa(m_pClientList[iClientH]->m_iApprColor, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");


	strcat(pData, "[ITEMLIST]\n\n");

	for (i = 0; i < DEF_MAXITEMS; i++) { // v1.4
		TempItemPosList[i].x = 40;
		TempItemPosList[i].y = 30;
	}
	iPos = 0;


	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {

			TempItemPosList[iPos].x = m_pClientList[iClientH]->m_ItemPosList[i].x;
			TempItemPosList[iPos].y = m_pClientList[iClientH]->m_ItemPosList[i].y;
			iPos++;

			strcat(pData, "character-item = ");
			memset(cTmp, ' ', 21);
			strcpy(cTmp, m_pClientList[iClientH]->m_pItemList[i]->m_cName);
			cTmp[strlen(m_pClientList[iClientH]->m_pItemList[i]->m_cName)] = (char) ' ';
			cTmp[20] = 0;
			strcat(pData, cTmp);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_dwCount, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectType, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_cItemColor, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemList[i]->m_dwAttribute, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}
	strcat(pData, "\n\n");


	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pClientList[iClientH]->m_ItemPosList[i].x = TempItemPosList[i].x;
		m_pClientList[iClientH]->m_ItemPosList[i].y = TempItemPosList[i].y;
	}

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] != nullptr) {
			strcat(pData, "character-bank-item = ");
			memset(cTmp, ' ', 21);
			strcpy(cTmp, m_pClientList[iClientH]->m_pItemInBankList[i]->m_cName);
			cTmp[strlen(m_pClientList[iClientH]->m_pItemInBankList[i]->m_cName)] = (char) ' ';
			cTmp[20] = 0;
			strcat(pData, cTmp);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwCount, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectType, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_cItemColor, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_wCurLifeSpan, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwAttribute, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}
	strcat(pData, "\n\n");


	strcat(pData, "[MAGIC-SKILL-MASTERY]\n\n");

	strcat(pData, "//------------------012345678901234567890123456789012345678901234567890");
	strcat(pData, "\n");

	strcat(pData, "magic-mastery     = ");
	for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
		wsprintf(cTxt, "%d", (int) m_pClientList[iClientH]->m_cMagicMastery[i]);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n");

	strcat(pData, "skill-mastery     = ");


	for (i = 0; i < 60; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_cSkillMastery[i]);

		strcat(pData, cTxt);
	}
	strcat(pData, "\n");

	strcat(pData, "skill-SSN     = ");
	for (i = 0; i < 60; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_iSkillSSN[i]);

		strcat(pData, cTxt);
	}
	strcat(pData, "\n");


	strcat(pData, "[ITEM-EQUIP-STATUS]\n\n");
	strcat(pData, "item-equip-status = ");

	std::memset(cTxt, 0, sizeof (cTxt));
	strcpy(cTxt, "00000000000000000000000000000000000000000000000000");

	int iEP = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {
			if ((m_pClientList[iClientH]->m_bIsItemEquipped[i] == true) &&
					  (m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_EQUIP)) {
				cTxt[iEP] = '1';
			}
			iEP++;
		}
	strcat(pData, cTxt);
	strcat(pData, "\n");


	strcat(pData, "item-position-x = ");
	for (i = 0; i < DEF_MAXITEMS; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_ItemPosList[i].x);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n");

	strcat(pData, "item-position-y = ");
	for (i = 0; i < DEF_MAXITEMS; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_ItemPosList[i].y);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n\n");

	strcat(pData, "[EOF]");
	strcat(pData, "\n\n\n\n");

	return strlen(pData);
}

bool CGame::_bDecodeItemConfigFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iItemConfigListIndex, iTemp;
	class CStrTok * pStrTok;

	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	//token = strtok( pContents, seps );
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemIDnumber");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iItemConfigListIndex = atoi(token);

							//testcode
							if (iItemConfigListIndex == 490)
								iItemConfigListIndex = atoi(token);


							if (m_pItemConfigList[iItemConfigListIndex] != nullptr) {

								wsprintf(cTxt, "(!!!) CRITICAL ERROR! Duplicate ItemIDnum(%d)", iItemConfigListIndex);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex] = new class CItem;
							m_pItemConfigList[iItemConfigListIndex]->m_sIDnum = iItemConfigListIndex;
							cReadModeB = 2;
							break;
						case 2:
							// m_cName
							std::memset(m_pItemConfigList[iItemConfigListIndex]->m_cName, 0, sizeof (m_pItemConfigList[iItemConfigListIndex]->m_cName));
							memcpy(m_pItemConfigList[iItemConfigListIndex]->m_cName, token, strlen(token));
							cReadModeB = 3;
							break;
						case 3:
							// m_cItemType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemType");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cItemType = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// m_cEquipPos
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - EquipPos");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cEquipPos = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// m_sItemEffectType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectType");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectType = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							// m_sItemEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue1");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue1 = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							// m_sItemEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue2");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue2 = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							// m_sItemEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue3");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue3 = atoi(token);
							cReadModeB = 9;
							break;
						case 9:
							// m_sItemEffectValue4
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue4");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue4 = atoi(token);
							cReadModeB = 10;
							break;
						case 10:
							// m_sItemEffectValue5
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue5");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue5 = atoi(token);
							cReadModeB = 11;
							break;
						case 11:
							// m_sItemEffectValue6
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue6");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue6 = atoi(token);
							cReadModeB = 12;
							break;
						case 12:
							// m_wMaxLifeSpan
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - MaxLifeSpan");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_wMaxLifeSpan = (uint16_t) atoi(token);
							cReadModeB = 13;
							break;
						case 13:
							// m_sSpecialEffect
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - MaxFixCount");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffect = atoi(token);
							cReadModeB = 14;
							break;
						case 14:
							// m_sSprite
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Sprite");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sSprite = atoi(token);
							cReadModeB = 15;
							break;
						case 15:
							// m_sSpriteFrame
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - SpriteFrame");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sSpriteFrame = atoi(token);
							cReadModeB = 16;
							break;
						case 16:
							// m_wPrice
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Price");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iTemp = atoi(token);
							if (iTemp < 0)
								m_pItemConfigList[iItemConfigListIndex]->m_bIsForSale = false;
							else m_pItemConfigList[iItemConfigListIndex]->m_bIsForSale = true;

							m_pItemConfigList[iItemConfigListIndex]->m_wPrice = abs(iTemp);
							cReadModeB = 17;
							break;
						case 17:
							// m_wWeight
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Weight");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_wWeight = atoi(token);
							cReadModeB = 18;
							break;
						case 18:
							// Appr Value
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ApprValue");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cApprValue = atoi(token);
							cReadModeB = 19;
							break;
						case 19:
							// m_cSpeed
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Speed");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cSpeed = atoi(token);
							cReadModeB = 20;
							break;

						case 20:
							// m_sLevelLimit
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - LevelLimit");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sLevelLimit = atoi(token);
							cReadModeB = 21;
							break;

						case 21:
							// m_cGederLimit
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - GenderLimit");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cGenderLimit = atoi(token);
							cReadModeB = 22;
							break;

						case 22:
							// m_sSpecialEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - SM_HitRatio");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffectValue1 = atoi(token);
							cReadModeB = 23;
							break;

						case 23:
							// m_sSpecialEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - L_HitRatio");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffectValue2 = atoi(token);
							cReadModeB = 24;
							break;

						case 24:
							// m_sRelatedSkill
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - RelatedSkill");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_sRelatedSkill = atoi(token);
							cReadModeB = 25;
							break;

						case 25:
							// m_cCategory
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Category");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cCategory = atoi(token);
							cReadModeB = 26;
							break;

						case 26:
							// m_cItemColor
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Category");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pItemConfigList[iItemConfigListIndex]->m_cItemColor = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				default:
					break;
			}
		} else {
			if (memcmp(token, "Item", 4) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

			if (memcmp(token, "[ENDITEMLIST]", 13) == 0) {
				cReadModeA = 0;
				cReadModeB = 0;
				goto DICFC_STOPDECODING;
			}
		}
		token = pStrTok->pGet();
		//token = strtok( nullptr, seps );
	}

DICFC_STOPDECODING:
	;

	delete pStrTok;
	delete pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) ITEM(Total:%d) configuration - success!", iItemConfigListIndex);
	PutLogList(cTxt);

	return true;
}

bool CGame::_bInitItemAttr(class CItem &item, const char * pItemName) {
	register int i;
	char cTmpName[21];

	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pItemName);

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != nullptr) {
			if (memcmp(cTmpName, m_pItemConfigList[i]->m_cName, 20) == 0) {

				std::memset(item.m_cName, 0, sizeof (item.m_cName));
				strcpy(item.m_cName, m_pItemConfigList[i]->m_cName);
				item.m_cItemType = m_pItemConfigList[i]->m_cItemType;
				item.m_cEquipPos = m_pItemConfigList[i]->m_cEquipPos;
				item.m_sItemEffectType = m_pItemConfigList[i]->m_sItemEffectType;
				item.m_sItemEffectValue1 = m_pItemConfigList[i]->m_sItemEffectValue1;
				item.m_sItemEffectValue2 = m_pItemConfigList[i]->m_sItemEffectValue2;
				item.m_sItemEffectValue3 = m_pItemConfigList[i]->m_sItemEffectValue3;
				item.m_sItemEffectValue4 = m_pItemConfigList[i]->m_sItemEffectValue4;
				item.m_sItemEffectValue5 = m_pItemConfigList[i]->m_sItemEffectValue5;
				item.m_sItemEffectValue6 = m_pItemConfigList[i]->m_sItemEffectValue6;
				item.m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
				item.m_wCurLifeSpan = item.m_wMaxLifeSpan;
				item.m_sSpecialEffect = m_pItemConfigList[i]->m_sSpecialEffect;

				item.m_sSprite = m_pItemConfigList[i]->m_sSprite;
				item.m_sSpriteFrame = m_pItemConfigList[i]->m_sSpriteFrame;
				item.m_wPrice = m_pItemConfigList[i]->m_wPrice;
				item.m_wWeight = m_pItemConfigList[i]->m_wWeight;
				item.m_cApprValue = m_pItemConfigList[i]->m_cApprValue;
				item.m_cSpeed = m_pItemConfigList[i]->m_cSpeed;
				item.m_sLevelLimit = m_pItemConfigList[i]->m_sLevelLimit;
				item.m_cGenderLimit = m_pItemConfigList[i]->m_cGenderLimit;

				item.m_sSpecialEffectValue1 = m_pItemConfigList[i]->m_sSpecialEffectValue1;
				item.m_sSpecialEffectValue2 = m_pItemConfigList[i]->m_sSpecialEffectValue2;

				item.m_sRelatedSkill = m_pItemConfigList[i]->m_sRelatedSkill;
				item.m_cCategory = m_pItemConfigList[i]->m_cCategory;
				item.m_sIDnum = m_pItemConfigList[i]->m_sIDnum;

				item.m_bIsForSale = m_pItemConfigList[i]->m_bIsForSale;
				item.m_cItemColor = m_pItemConfigList[i]->m_cItemColor;
				return true;
			}
		}


	return false;
}

bool CGame::_bGetIsStringIsNumber(char * pStr) {
	int i;
	for (i = 0; i < (int) strlen(pStr); i++)
		if ((pStr[i] != '-') && ((pStr[i] < (char) '0') || (pStr[i] > (char) '9'))) return false;

	return true;
}

bool CGame::_bReadMapInfoFiles(int iMapIndex) {
	if (__bReadMapInfo(iMapIndex) == false) {
		return false;
	}

	return true;
}

bool CGame::bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType) {
	register int i, iIndex, iMapIndex, iFollowIndex;
	char cTmpName[11], cFollowSide;

	std::memset(cTmpName, 0, sizeof (cTmpName));
	iMapIndex = -1;
	iFollowIndex = -1;

	for (i = 1; i < DEF_MAXNPCS; i++)
		if ((m_pNpcList[i] != nullptr) && (memcmp(m_pNpcList[i]->m_cName, pName, 5) == 0)) {
			iIndex = i;
			iMapIndex = m_pNpcList[i]->m_cMapIndex;
			goto NEXT_STEP_SNFM1;
		}

NEXT_STEP_SNFM1:
	;

	switch (cFollowOwnerType) {
		case DEF_OWNERTYPE_NPC:
			for (i = 1; i < DEF_MAXNPCS; i++)
				if ((m_pNpcList[i] != nullptr) && (memcmp(m_pNpcList[i]->m_cName, pFollowName, 5) == 0)) {


					if (m_pNpcList[i]->m_cMapIndex != iMapIndex) return false;
					iFollowIndex = i;
					cFollowSide = m_pNpcList[i]->m_cSide;
					goto NEXT_STEP_SNFM2;
				}
			break;

		case DEF_OWNERTYPE_PLAYER:
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pFollowName, 10) == 0)) {


					if (m_pClientList[i]->m_cMapIndex != iMapIndex) return false;
					iFollowIndex = i;
					cFollowSide = m_pClientList[i]->m_cSide;
					goto NEXT_STEP_SNFM2;
				}
			break;
	}

NEXT_STEP_SNFM2:
	;

	if ((iIndex == -1) || (iFollowIndex == -1)) return false;

	m_pNpcList[iIndex]->m_cMoveType = DEF_MOVETYPE_FOLLOW;
	m_pNpcList[iIndex]->m_cFollowOwnerType = cFollowOwnerType;
	m_pNpcList[iIndex]->m_iFollowOwnerIndex = iFollowIndex;
	m_pNpcList[iIndex]->m_cSide = cFollowSide;


	return true;
}

int CGame::bCreateNewNpc(const char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned, bool bFirmBerserk, bool bIsMaster, int iGuildGUID) {
	register int i, t, j, k, iMapIndex;
	char cTmpName[11], cTxt[120];
	short sX, sY, sRange;
	bool bFlag;
	SYSTEMTIME SysTime;

	if (strlen(pName) == 0) return false;
	if (strlen(pNpcName) == 0) return false;
	GetLocalTime(&SysTime);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pMapName);
	iMapIndex = -1;

	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) {
			if (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)
				iMapIndex = i;
		}

	if (iMapIndex == -1) return false;

	for (i = 1; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] == nullptr) {
			m_pNpcList[i] = new class CNpc(pName);
			if (_bInitNpcAttr(m_pNpcList[i], pNpcName, sClass, cSA) == false) {
				wsprintf(cTxt, "(!) Not existing NPC creation request! (%s) Ignored.", pNpcName);
				PutLogList(cTxt);

				delete m_pNpcList[i];
				m_pNpcList[i] = nullptr;
				return false;
			}

			if (m_pNpcList[i]->m_cDayOfWeekLimit < 10) {
				if (m_pNpcList[i]->m_cDayOfWeekLimit != SysTime.wDayOfWeek) {
					delete m_pNpcList[i];
					m_pNpcList[i] = nullptr;
					return false;
				}
			}

			switch (cMoveType) {
				case DEF_MOVETYPE_GUARD:
				case DEF_MOVETYPE_RANDOM:
					if ((poX != nullptr) && (poY != nullptr) && (*poX != 0) && (*poY != 0)) {
						sX = *poX;
						sY = *poY;
					} else {
						for (j = 0; j <= 30; j++) {

							sX = (rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15;
							sY = (rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15;

							bFlag = true;
							for (k = 0; k < DEF_MAXMGAR; k++)
								if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].left != -1) {

									if ((sX >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].left) &&
											  (sX <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].right) &&
											  (sY >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].top) &&
											  (sY <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].bottom)) {

										bFlag = false;
									}
								}
							if (bFlag == true) goto GET_VALIDLOC_SUCCESS;

						}

						delete m_pNpcList[i];
						m_pNpcList[i] = nullptr;
						return false;

GET_VALIDLOC_SUCCESS:
						;

					}
					break;

				case DEF_MOVETYPE_RANDOMAREA:

					sRange = (short) (pArea->right - pArea->left);
					sX = (short) ((rand() % sRange) + pArea->left);
					sRange = (short) (pArea->bottom - pArea->top);
					sY = (short) ((rand() % sRange) + pArea->top);
					break;

				case DEF_MOVETYPE_RANDOMWAYPOINT:

					sX = (short) m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].x;
					sY = (short) m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].y;
					break;

				default:

					if ((poX != nullptr) && (poY != nullptr) && (*poX != 0) && (*poY != 0)) {

						sX = *poX;
						sY = *poY;
					} else {
						sX = (short) m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[0]].x;
						sY = (short) m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[0]].y;
					}
					break;
			}


			if (bGetEmptyPosition(&sX, &sY, iMapIndex) == false) {

				delete m_pNpcList[i];
				m_pNpcList[i] = nullptr;
				return false;
			}

			if ((bHideGenMode == true) && (_iGetPlayerNumberOnSpot(sX, sY, iMapIndex, 7) != 0)) {

				delete m_pNpcList[i];
				m_pNpcList[i] = nullptr;
				return false;
			}

			if ((poX != nullptr) && (poY != nullptr)) {

				*poX = sX;
				*poY = sY;
			}

			m_pNpcList[i]->m_sX = sX;
			m_pNpcList[i]->m_sY = sY;


			m_pNpcList[i]->m_vX = sX;
			m_pNpcList[i]->m_vY = sY;


			for (t = 0; t < 10; t++)
				m_pNpcList[i]->m_iWayPointIndex[t] = pWaypointList[t];


			m_pNpcList[i]->m_cTotalWaypoint = 0;
			for (t = 0; t < 10; t++)
				if (m_pNpcList[i]->m_iWayPointIndex[t] != -1) m_pNpcList[i]->m_cTotalWaypoint++;

			if (pArea != nullptr) {
				// RANDOMAREA Copy
				SetRect(&m_pNpcList[i]->m_rcRandomArea, pArea->left, pArea->top, pArea->right, pArea->bottom);
			}

			switch (cMoveType) {
				case DEF_MOVETYPE_GUARD:
					m_pNpcList[i]->m_dX = m_pNpcList[i]->m_sX;
					m_pNpcList[i]->m_dY = m_pNpcList[i]->m_sY;
					break;

				case DEF_MOVETYPE_SEQWAYPOINT:
					m_pNpcList[i]->m_cCurWaypoint = 1;

					m_pNpcList[i]->m_dX = (short) m_pMapList[iMapIndex]->m_WaypointList[ m_pNpcList[i]->m_iWayPointIndex[ m_pNpcList[i]->m_cCurWaypoint ] ].x;
					m_pNpcList[i]->m_dY = (short) m_pMapList[iMapIndex]->m_WaypointList[ m_pNpcList[i]->m_iWayPointIndex[ m_pNpcList[i]->m_cCurWaypoint ] ].y;
					break;

				case DEF_MOVETYPE_RANDOMWAYPOINT:
					m_pNpcList[i]->m_cCurWaypoint = (rand() % (m_pNpcList[i]->m_cTotalWaypoint - 1)) + 1;

					m_pNpcList[i]->m_dX = (short) m_pMapList[iMapIndex]->m_WaypointList[ m_pNpcList[i]->m_iWayPointIndex[ m_pNpcList[i]->m_cCurWaypoint ] ].x;
					m_pNpcList[i]->m_dY = (short) m_pMapList[iMapIndex]->m_WaypointList[ m_pNpcList[i]->m_iWayPointIndex[ m_pNpcList[i]->m_cCurWaypoint ] ].y;
					break;

				case DEF_MOVETYPE_RANDOMAREA:
					m_pNpcList[i]->m_cCurWaypoint = 0;

					sRange = (short) (m_pNpcList[i]->m_rcRandomArea.right - m_pNpcList[i]->m_rcRandomArea.left);
					m_pNpcList[i]->m_dX = (short) ((rand() % sRange) + m_pNpcList[i]->m_rcRandomArea.left);
					sRange = (short) (m_pNpcList[i]->m_rcRandomArea.bottom - m_pNpcList[i]->m_rcRandomArea.top);
					m_pNpcList[i]->m_dY = (short) ((rand() % sRange) + m_pNpcList[i]->m_rcRandomArea.top);
					break;

				case DEF_MOVETYPE_RANDOM:
					m_pNpcList[i]->m_dX = (short) ((rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15);
					m_pNpcList[i]->m_dY = (short) ((rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15);
					break;
			}

			m_pNpcList[i]->m_tmp_iError = 0;

			m_pNpcList[i]->m_cMoveType = cMoveType;

			switch (m_pNpcList[i]->m_cActionLimit) {
				case 2:
				case 3:
				case 5:

					m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_STOP;

					switch (m_pNpcList[i]->m_sType) {
						case 15: // ShopKeeper-W
						case 19: // Gandlf
						case 20: // Howard
						case 24: // Tom
						case 25: // William
						case 26: // Kennedy
							m_pNpcList[i]->m_cDir = 4 + iDice(1, 3) - 1;
							break;

						default:
							m_pNpcList[i]->m_cDir = iDice(1, 8);
							break;
					}
					break;

				default:
					m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
					m_pNpcList[i]->m_cDir = 5;
					break;
			}

			m_pNpcList[i]->m_iFollowOwnerIndex = 0;
			m_pNpcList[i]->m_iTargetIndex = 0;
			m_pNpcList[i]->m_cTurn = (rand() % 2);

			switch (m_pNpcList[i]->m_sType) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:

					m_pNpcList[i]->m_sAppr2 = (short) 0xF000;
					m_pNpcList[i]->m_sAppr2 = m_pNpcList[i]->m_sAppr2 | ((rand() % 13) << 4); // ¹«±â
					m_pNpcList[i]->m_sAppr2 = m_pNpcList[i]->m_sAppr2 | (rand() % 9); // ¹æÆÐ
					break;

				case 36: // AGT-Aresden/AGT-Elvine
				case 37: // CGT-Aresden/CGT-Elvine
				case 38: // MS-Aresden/MS-Elvine
				case 39: // DT-Aresden/DT-Elvine
					m_pNpcList[i]->m_sAppr2 = 3;
					break;

					//case 64: // Crop

					//	m_pNpcList[i]->m_sAppr2 = 1;
					//	break;
					// appr2 = 2 seems to be enemy detection for crusade

					/*case 91: // gate
						
						m_pNpcList[i]->m_sAppr2 = 0xF000; // 10 aura no sphere 13 no name movable with magic (crash) test to 29
						break;*/

				case 64: // Crop
					m_pNpcList[i]->m_sAppr2 = 1; // 1 bud; 2 grown; 3 large
					break;

					// case 66: // Wyvern
					//	m_pNpcList[i]->m_iStatus = 0x00000010;
					//	// m_pNpcList[i]->m_cSide = 2;
					//	break;

				default:
					m_pNpcList[i]->m_sAppr2 = 0;
					break;
			}

			m_pNpcList[i]->m_cMapIndex = (char) iMapIndex;
			m_pNpcList[i]->m_dwTime = timeGetTime() + (rand() % 10000);
			m_pNpcList[i]->m_dwActionTime += (rand() % 300);
			m_pNpcList[i]->m_dwMPupTime = timeGetTime();
			m_pNpcList[i]->m_dwHPupTime = m_pNpcList[i]->m_dwMPupTime;
			m_pNpcList[i]->m_sBehaviorTurnCount = 0;
			m_pNpcList[i]->m_bIsSummoned = bIsSummoned;
			m_pNpcList[i]->m_bIsMaster = bIsMaster;

			if (bIsSummoned == true)
				m_pNpcList[i]->m_dwSummonedTime = timeGetTime();


			if (bFirmBerserk == true) {
				m_pNpcList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = 1;
				m_pNpcList[i]->m_iStatus = m_pNpcList[i]->m_iStatus | 0x20;

				//iExpRoll = iDice(m_pNpcList[i]->m_iExpDiceMin, m_pNpcList[i]->m_iExpDiceMax);
				//iExpRoll *= 2;
			}

			// !!!
			if (cChangeSide != -1) m_pNpcList[i]->m_cSide = cChangeSide;

			m_pNpcList[i]->m_cBravery = (rand() % 3) + m_pNpcList[i]->m_iMinBravery;
			m_pNpcList[i]->m_iSpotMobIndex = iSpotMobIndex;
			m_pNpcList[i]->m_iGuildGUID = iGuildGUID;
			//testcode
			if (iGuildGUID != 0) {
				wsprintf(G_cTxt, "Summon War Unit(%d) GUID(%d)", m_pNpcList[i]->m_sType, iGuildGUID);
				PutLogList(G_cTxt);
			}

			m_pMapList[iMapIndex]->SetOwner(i, DEF_OWNERTYPE_NPC, sX, sY);
			m_pMapList[iMapIndex]->m_iTotalActiveObject++;
			m_pMapList[iMapIndex]->m_iTotalAliveObject++;


			switch (m_pNpcList[i]->m_sType) {
				case 36: // AGT-Aresden/AGT-Elvine
				case 37: // CGT-Aresden/CGT-Elvine
				case 38: // MS-Aresden/MS-Elvine
				case 39: // DT-Aresden/DT-Elvine
				case 42: // ManaStone
					m_pMapList[iMapIndex]->bAddCrusadeStructureInfo(m_pNpcList[i]->m_sType, sX, sY, m_pNpcList[i]->m_cSide);
					break;

				case 64:
					m_pMapList[iMapIndex]->bAddCropsTotalSum();
					break;
			}


			SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
			return true;
		}

	return false;
}

void CGame::NpcProcess() {
	register int i, iMaxHP;
	uint32_t dwTime, dwActionTime;

	dwTime = timeGetTime();

	for (i = 1; i < DEF_MAXNPCS; i++) {

		if (m_pNpcList[i] != nullptr) {

			if (m_pNpcList[i]->m_cBehavior == DEF_BEHAVIOR_ATTACK) {
				switch (iDice(1, 7)) {
					case 1: dwActionTime = m_pNpcList[i]->m_dwActionTime;
						break;
					case 2: dwActionTime = m_pNpcList[i]->m_dwActionTime - 100;
						break;
					case 3: dwActionTime = m_pNpcList[i]->m_dwActionTime - 200;
						break;
					case 4: dwActionTime = m_pNpcList[i]->m_dwActionTime - 300;
						break;
					case 5: dwActionTime = m_pNpcList[i]->m_dwActionTime - 400;
						break;
					case 6: dwActionTime = m_pNpcList[i]->m_dwActionTime - 600;
						break;
					case 7: dwActionTime = m_pNpcList[i]->m_dwActionTime - 700;
						break;
				}
				if (dwActionTime < 600) dwActionTime = 600;
			} else dwActionTime = m_pNpcList[i]->m_dwActionTime;


			if (m_pNpcList[i]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] != 0)
				dwActionTime += (dwActionTime / 2);
		}

		if ((m_pNpcList[i] != nullptr) && ((dwTime - m_pNpcList[i]->m_dwTime) > dwActionTime)) {

			m_pNpcList[i]->m_dwTime = dwTime;

			if (abs(m_pNpcList[i]->m_cMagicLevel) > 0) {

				if ((dwTime - m_pNpcList[i]->m_dwMPupTime) > DEF_MPUPTIME) {
					m_pNpcList[i]->m_dwMPupTime = dwTime;


					m_pNpcList[i]->m_iMana += iDice(1, (m_pNpcList[i]->m_iMaxMana / 5));

					if (m_pNpcList[i]->m_iMana > m_pNpcList[i]->m_iMaxMana)
						m_pNpcList[i]->m_iMana = m_pNpcList[i]->m_iMaxMana;
				}
			}


			if (((dwTime - m_pNpcList[i]->m_dwHPupTime) > DEF_HPUPTIME) && (m_pNpcList[i]->m_bIsKilled == false)) {

				m_pNpcList[i]->m_dwHPupTime = dwTime;

				iMaxHP = iDice(m_pNpcList[i]->m_iHitDice, 8) + m_pNpcList[i]->m_iHitDice;
				if (m_pNpcList[i]->m_iHP < iMaxHP) {

					if (m_pNpcList[i]->m_bIsSummoned == false)
						m_pNpcList[i]->m_iHP += iDice(1, m_pNpcList[i]->m_iHitDice);

					if (m_pNpcList[i]->m_iHP > iMaxHP) m_pNpcList[i]->m_iHP = iMaxHP;
					if (m_pNpcList[i]->m_iHP <= 0) m_pNpcList[i]->m_iHP = 1;
				}
			}

			switch (m_pNpcList[i]->m_cBehavior) {
				case DEF_BEHAVIOR_DEAD:
					NpcBehavior_Dead(i);
					break;
				case DEF_BEHAVIOR_STOP:
					NpcBehavior_Stop(i);
					break;
				case DEF_BEHAVIOR_MOVE:
					NpcBehavior_Move(i);
					break;
				case DEF_BEHAVIOR_ATTACK:
					NpcBehavior_Attack(i);
					break;
				case DEF_BEHAVIOR_FLEE:
					NpcBehavior_Flee(i);
					break;
			}


			if ((m_pNpcList[i] != nullptr) && (m_pNpcList[i]->m_iHP != 0) && (m_pNpcList[i]->m_bIsSummoned == true)) {

				switch (m_pNpcList[i]->m_sType) {
					case 29:
						if ((dwTime - m_pNpcList[i]->m_dwSummonedTime) > 1000 * 90)
							NpcKilledHandler(0, 0, i, 0);
						break;

					default:
						if ((dwTime - m_pNpcList[i]->m_dwSummonedTime) > DEF_SUMMONTIME)
							NpcKilledHandler(0, 0, i, 0);
						break;
				}
			}
		}
	}
}

// 05/29/2004 - Hypnotoad - GM chat tweak

void CGame::ChatMsgHandler(CClient &client, char * pData, uint32_t dwMsgSize) {
	register int i, iRet;
	uint16_t * wp;
	char * cp;
	char cBuffer[256], cTemp[256], cSendMode = 0;
	int iClientH = client.id_;
	client.m_dwAFKCheckTime = timeGetTime();
	if (client.m_bIsInitComplete == false) return;
	if (dwMsgSize > 83 + 30) return;
	{
	}

	if (client.m_iTimeLeft_ShutUp > 0) return;


	if (memcmp((pData + 10), client.m_cCharName, strlen(client.m_cCharName)) != 0) return;


	if ((client.m_bIsObserverMode == true) && (client.m_iAdminUserLevel == 0)) return;


	int iStX, iStY;
	if (m_pMapList[client.m_cMapIndex] != nullptr) {
		iStX = client.m_sX / 20;
		iStY = client.m_sY / 20;
		m_pMapList[client.m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (client.m_cSide) {
			case 0: m_pMapList[client.m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: m_pMapList[client.m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: m_pMapList[client.m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}

	cp = (char *) (pData + 21);

	switch (m_iLogChatOption) {
			// Chat Logs of only players
		case 1:
			if (client.m_iAdminUserLevel == 0) {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) PC(%s):\"%s\"", client.m_cMapName, client.m_cIPaddress, client.m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			}
			break;
			// Chat Logs of only GM
		case 2:
			if (client.m_iAdminUserLevel > 0) {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) GM(%s):\"%s\"", client.m_cMapName, client.m_cIPaddress, client.m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			}
			break;
			// Chat logs of all
		case 3:
			if (client.m_iAdminUserLevel > 0) {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) GM(%s):\"%s\"", client.m_cMapName, client.m_cIPaddress, client.m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			} else {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) PC(%s):\"%s\"", client.m_cMapName, client.m_cIPaddress, client.m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			}
			break;
			// Chat logs of none
		case 4:

			break;
	}

	/*	// New 08/05/2004
	// Log GM Chats
	if (client.m_iAdminUserLevel > 0){
		std::memset(cTemp, 0, sizeof(cTemp));
		pData[dwMsgSize-1] = 0;
		wsprintf(cTemp,"GM Chat(%s):\"%s\"",client.m_cCharName,cp);
		bSendMsgToLS(MSGID_GAMEMASTERLOG,iClientH, false,cTemp);
	}*/


	switch (*cp) {
		case '@':

			*cp = 32;
			if ((client.m_iTimeLeft_ShutUp == 0) && (client.m_iLevel > 1) &&
					  (client.m_iSP >= 3)) {

				//v1.42
				if (client.m_iTimeLeft_FirmStaminar == 0) {
					client.m_iSP -= 3;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 1;
			} else cSendMode = 0;


			if (client.m_iTimeLeft_ShutUp > 0) cSendMode = 0;
			break;

			// New 08/05/2004
			// Party chat
		case '$':
			*cp = 32;

			if ((client.m_iTimeLeft_ShutUp == 0) && (client.m_iSP >= 3)) {
				if (client.m_iTimeLeft_FirmStaminar == 0) {
					client.m_iSP -= 3;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 4;
			} else {
				cSendMode = 0;
			}

			if (client.m_iTimeLeft_ShutUp > 0) {
				cSendMode = 0;
			}
			break;

		case '^':


			*cp = 32;
			if ((client.m_iTimeLeft_ShutUp == 0) && (client.m_iLevel > 10) &&
					  (client.m_iSP > 5) && client.m_iGuildRank != -1) {
				if (client.m_iTimeLeft_FirmStaminar == 0) {
					client.m_iSP -= 3;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 1;
			} else cSendMode = 0;


			if (client.m_iTimeLeft_ShutUp > 0) cSendMode = 0;


			if (client.m_iHP < 0) cSendMode = 0;


			if (client.m_iAdminUserLevel > 0) cSendMode = 10;
			break;

		case '!':


			*cp = 32;
			if ((client.m_iTimeLeft_ShutUp == 0) && (client.m_iLevel > 10) &&
					  (client.m_iSP >= 5)) {

				//v1.42
				if (client.m_iTimeLeft_FirmStaminar == 0) {
					client.m_iSP -= 5;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 2;
			} else cSendMode = 0;


			if (client.m_iTimeLeft_ShutUp > 0) cSendMode = 0;


			if (client.m_iHP <= 0) cSendMode = 0;


			if (client.m_iAdminUserLevel > 0) cSendMode = 35; // GM Chat By Carlos
			break;

		case '~':

			*cp = 32;
			if ((client.m_iTimeLeft_ShutUp == 0) && (client.m_iLevel > 1) &&
					  (client.m_iSP >= 3)) {

				//v1.42
				if (client.m_iTimeLeft_FirmStaminar == 0) {
					client.m_iSP -= 3;
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 3;
			} else cSendMode = 0;


			if (client.m_iTimeLeft_ShutUp > 0) cSendMode = 0;

			if (client.m_iHP <= 0) cSendMode = 0;
			break;

		case '/':

			std::memset(cBuffer, 0, sizeof (cBuffer));
			memcpy(cBuffer, cp, dwMsgSize - 21);
			cp = (char *) (cBuffer);

			// New 19/05/2004
			if (memcmp(cp, "/version", 8) == 0) {
				ShowVersion(iClientH);
				return;
			}

			// New 10/05/2004
			if (memcmp(cp, "/begincrusadetotalwar", 21) == 0) {
				if (client.m_iAdminUserLevel > 3) {
					GlobalStartCrusadeMode();
					wsprintf(cTemp, "(%s) GM Order(%s): begincrusadetotalwar", client.m_cIPaddress, client.m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}

			if (memcmp(cp, "/endcrusadetotalwar", 19) == 0) {
				if (client.m_iAdminUserLevel > 3) {
					ManualEndCrusadeMode(0);
					wsprintf(cTemp, "(%s) GM Order(%s): endcrusadetotalwar", client.m_cIPaddress, client.m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}

			// New 08/05/2004
			if (memcmp(cp, "/goto ", 6) == 0) {
				AdminOrder_GoTo(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/unsummonboss", 13) == 0) {
				AdminOrder_UnsummonBoss(iClientH);
				return;
			}

			//Showdmg
			if (memcmp(cp, "/showdmg", 8) == 0) {
				PlayerOrder_ShowDamage(iClientH);
				return;
			}

			if (memcmp(cp, "/clearnpc", 9) == 0) {
				AdminOrder_ClearNpc(iClientH);
				return;
			}

			if (memcmp(cp, "/clearmap", 9) == 0) {
				AdminOrder_CleanMap(iClientH, cp, dwMsgSize);
				return;
			}

			if (memcmp(cp, "/setforcerecalltime ", 20) == 0) {
				AdminOrder_SetForceRecallTime(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/enableadmincommand ", 20) == 0) {
				AdminOrder_EnableAdminCommand(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/monstercount", 13) == 0) {
				AdminOrder_MonsterCount(iClientH, cp, dwMsgSize - 21);
				return;
			}

			// New 07/05/2004
			if (memcmp(cp, "/createparty", 12) == 0) {
				RequestCreatePartyHandler(iClientH);
				return;
			}

			if (memcmp(cp, "/joinparty ", 11) == 0) {
				RequestJoinPartyHandler(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/dismissparty", 13) == 0) {
				RequestDismissPartyHandler(iClientH);
				return;
			}

			if (memcmp(cp, "/getpartyinfo", 13) == 0) {
				GetPartyInfoHandler(iClientH);
				return;
			}

			if (memcmp(cp, "/deleteparty", 12) == 0) {
				RequestDeletePartyHandler(iClientH);
				return;
			}

			if (memcmp(cp, "/who", 4) == 0) {
				if (client.m_iAdminUserLevel >= m_iAdminLevelWho) {
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_TOTALUSERS, 0, 0, 0, nullptr);
				}
				return;
			}

			//50Cent - Capture The Flag
			if (memcmp(cp, "/startctf", 9) == 0) {
				if (client.m_iAdminUserLevel >= 4) StartCaptureTheFlag();
				return;
			}
			if (memcmp(cp, "/endctf", 7) == 0) {
				if (client.m_iAdminUserLevel >= 4) EndCaptureTheFlag(0);
				return;
			}


			if (memcmp(cp, "/happyhour ", 11) == 0) { // MORLA 2.3 - Comando Happy Hour
				if (client.m_iAdminUserLevel == 0) return;
				ManualStartHappyHour(iClientH, cp, dwMsgSize - 21);
				return;
			}


			if (memcmp(cp, "/porpatas", 3) == 0) {
				if (client.m_iAdminUserLevel > 0)
					DeleteClient(iClientH, true, true);
				return;
			}

			if (memcmp(cp, "/desbug ", 8) == 0) {
				DesbugCharacter(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/dkset", 6) == 0) {
				GetDkSet(iClientH);
				return;
			}

			//criticals by MihD
			if (memcmp(cp, "/criticals", 10) == 0) {
				PlayerOrder_GetCrits(client);
				return;
			}

			if (memcmp(cp, "/addmaster ", 11) == 0) {
				PlayerCommandAddMaster(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/deletemaster ", 13) == 0) {
				PlayerOrder_DeleteGuildMaster(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/balltrade ", 7) == 0) {
				_TradeItem(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/ballpoints", 10) == 0) {
				char cInfoString[50];
				wsprintf(cInfoString, " You have got:(%d) Ball Points. ", client.m_iBallPoints);
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
				return;
			}

			if (memcmp(cp, "/setcrit", 6) == 0) {
				AdminOrder_SetCrit(iClientH, cp, dwMsgSize - 21);
				;
				return;
			}

			if (memcmp(cp, "/setek", 6) == 0) {
				AdminOrder_SetEK(iClientH, cp, dwMsgSize - 21);
				;
				return;
			}

			if (memcmp(cp, "/setmaj", 7) == 0) {
				AdminOrder_SetMAJ(iClientH, cp, dwMsgSize - 21);
				;
				return;
			}

			if (memcmp(cp, "/setrep", 7) == 0) {
				AdminOrder_SetRep(iClientH, cp, dwMsgSize - 21);
				;
				return;
			}

			if (memcmp(cp, "/setpk", 6) == 0) {
				AdminOrder_SetPK(iClientH, cp, dwMsgSize - 21);
				;
				return;
			}

			if (memcmp(cp, "/fi ", 4) == 0) {

				CheckAndNotifyPlayerConnection(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/to", 3) == 0) {

				ToggleWhisperPlayer(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/setpf ", 7) == 0) {

				SetPlayerProfile(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/weather", 8) == 0) {
				AdminOrder_Weather(iClientH, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/pf ", 4) == 0) {

				GetPlayerProfile(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/shutup ", 8) == 0) {

				ShutUpPlayer(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/rep+ ", 6) == 0) {

				SetPlayerReputation(iClientH, cp, 1, dwMsgSize - 21);
				return;
			}


			if (memcmp(cp, "/time ", 6) == 0) {
				AdminOrder_Time(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/checkrep", 9) == 0) {
				AdminOrder_CheckRep(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/checkstatus ", 13) == 0) {
				AdminOrder_CheckStats(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/send ", 5) == 0) {
				AdminOrder_Pushplayer(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/rep- ", 6) == 0) {

				SetPlayerReputation(iClientH, cp, 0, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/hold", 5) == 0) {
				SetSummonMobAction(iClientH, 1, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/tgt ", 5) == 0) {
				SetSummonMobAction(iClientH, 2, dwMsgSize - 21, cp);
				return;
			}

			if (memcmp(cp, "/free", 5) == 0) {
				SetSummonMobAction(iClientH, 0, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/summonall ", 11) == 0) {
				AdminOrder_SummonAll(iClientH, cp, dwMsgSize - 21);
				return;
			}


			/*		if (memcmp(cp, "/summonguild ", 13) == 0) {
						AdminOrder_SummonGuild(iClientH, cp, dwMsgSize - 21);
						return;
					}*/

			if (memcmp(cp, "/summonplayer ", 14) == 0) {
				AdminOrder_SummonPlayer(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/storm ", 7) == 0) {
				AdminOrder_SummonStorm(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/summondeath ", 13) == 0) {
				AdminOrder_SummonDeath(iClientH);
				return;
			}

			if (memcmp(cp, "/kill ", 6) == 0) {
				AdminOrder_Kill(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/revive ", 8) == 0) {
				AdminOrder_Revive(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/closeconn ", 11) == 0) {

				AdminOrder_CloseConn(iClientH, cp, dwMsgSize - 21);
				return;
			}


			if (memcmp(cp, "/ban", 4) == 0) {
				UserCommand_BanGuildsman(iClientH, cp, dwMsgSize - 21);
				return;
			}


			if (memcmp(cp, "/reservefightzone", 17) == 0) {
				AdminOrder_ReserveFightzone(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/dissmiss ", 9) == 0) {
				UserCommand_DissmissGuild(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/attack ", 8) == 0) {

				AdminOrder_CallGuard(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/createfish ", 12) == 0) {
				AdminOrder_CreateFish(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/teleport ", 10) == 0 || memcmp(cp, "/tp ", 4) == 0) {
				AdminOrder_Teleport(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/summondemon ", 13) == 0) {
				AdminOrder_SummonDemon(iClientH);
				return;
			}

			if (memcmp(cp, "/unsummonall ", 13) == 0) {
				AdminOrder_UnsummonAll(iClientH);
				return;
			}

			if (memcmp(cp, "/unsummondemon ", 15) == 0) {
				AdminOrder_UnsummonDemon(iClientH);
				return;
			}

			if (memcmp(cp, "/checkip ", 9) == 0) {
				AdminOrder_CheckIP(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/polymorph ", 11) == 0) {
				AdminOrder_Polymorph(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/setinvi ", 9) == 0) {
				AdminOrder_SetInvi(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/gns ", 4) == 0) {
				AdminOrder_GetNpcStatus(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/setattackmode ", 15) == 0) {
				AdminOrder_SetAttackMode(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/summon ", 8) == 0) {
				AdminOrder_Summon(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/setzerk ", 9) == 0) {
				AdminOrder_SetZerk(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/setfreeze ", 11) == 0) {
				AdminOrder_SetFreeze(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/setstatus ", 11) == 0) {
				AdminOrder_SetStatus(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/disconnectall ", 15) == 0) {
				AdminOrder_DisconnectAll(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/createitem ", 12) == 0) {
				AdminOrder_CreateItem(iClientH, cp, dwMsgSize - 21);
				return;
			}

			if (memcmp(cp, "/energysphere ", 14) == 0) {
				if (client.m_iAdminUserLevel >= m_iAdminLevelEnergySphere) EnergySphereProcessor(true, iClientH);
				return;
			}
			if (memcmp(cp, "/banip ", 7) == 0) {
				AdminOrder_BanIP(iClientH, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/roll", 5) == 0) {


				CStrTok * pStrTok = new class CStrTok(cp, " ");
				char * token;
				token = pStrTok->pGet();
				token = pStrTok->pGet();

				if (token == nullptr) {
					delete pStrTok;
					return;
				}

				delete pStrTok;
				return;
			}

			if ((memcmp(cp, "/shutdownthisserverrightnow ", 28) == 0) && (client.m_iAdminUserLevel >= m_iAdminLevelShutdown)) {

				m_cShutDownCode = 2;
				m_bOnExitProcess = true;
				m_dwExitProcessTime = timeGetTime();

				PutLogList("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Admin-Command)!!!");

				bSendMsgToLS(MSGID_GAMESERVERSHUTDOWNED, 0);

				if (m_iMiddlelandMapIndex > 0) {
					// Crusade
					SaveOccupyFlagData();
					//bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, nullptr, nullptr);
					//bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, nullptr, nullptr);
				}
				return;
			}

			if ((memcmp(cp, "/setobservermode ", 17) == 0) && (client.m_iAdminUserLevel >= m_iAdminLevelObserver)) {

				AdminOrder_SetObserverMode(iClientH);
				return;
			}

			if ((memcmp(cp, "/getticket ", 11) == 0) && (client.m_iAdminUserLevel >= 2)) {
				AdminOrder_GetFightzoneTicket(iClientH);
				return;
			}

			if (memcmp(cp, "/beginapocalypse", 16) == 0) {
				if (client.m_iAdminUserLevel > 3) {
					if (client.m_bIsAdminCommandEnabled == false) return;
					GlobalStartApocalypseMode();
					wsprintf(cTemp, "GM Order(%s): beginapocalypse", client.m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}

			if (memcmp(cp, "/endapocalypse", 14) == 0) {
				if (client.m_iAdminUserLevel > 3) {
					if (client.m_bIsAdminCommandEnabled == false) return;
					GlobalEndApocalypseMode();
					wsprintf(cTemp, "GM Order(%s): endapocalypse", client.m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}

			if (memcmp(cp, "/beginheldenian ", 16) == 0) {
				if (client.m_iAdminUserLevel > 2) {
					ManualStartHeldenianMode(iClientH, cp, dwMsgSize - 21);
				}
				return;
			}

			if (memcmp(cp, "/endheldenian ", 14) == 0) {
				if (client.m_iAdminUserLevel > 2) {
					ManualEndHeldenianMode(iClientH, cp, dwMsgSize - 21);
				}
				return;
			}

			if (memcmp(cp, "/beginheldenianfight", 20) == 0) {
				if ((client.m_iAdminUserLevel > 2) && (m_bIsHeldenianMode == 1)) {
					NotifyStartHeldenianFightMode();
				}
				return;

			}

			return;
	}


	pData[dwMsgSize - 1] = 0;

	if ((client.m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] == 1) && (iDice(1, 3) != 2)) {

		cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 17);

		while (*cp != 0) {
			if ((cp[0] != 0) && (cp[0] != ' ') && (cp[1] != 0) && (cp[1] != ' ')) {
				switch (iDice(1, 3)) {
					case 1: memcpy(cp, "¿ö", 2);
						break;
					case 2: memcpy(cp, "¿ì", 2);
						break;
					case 3: memcpy(cp, "¿ù", 2);
						break;
				}
				cp += 2;
			} else cp++;
		}
	}

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 17);

	if ((cSendMode == 0) && (client.m_iWhisperPlayerIndex != -1)) {

		cSendMode = 20;


		if (*cp == '#') cSendMode = 0;


		if (client.m_iTimeLeft_ShutUp > 0) cSendMode = 0;
	}



	wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	*wp = (uint16_t) iClientH;

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 16);
	*cp = cSendMode;

	if (cSendMode != 20) {
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if (m_pClientList[i] != nullptr) {
				switch (cSendMode) {
					case 0:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;

						if ((m_pClientList[i]->m_cMapIndex == client.m_cMapIndex) &&
								  (m_pClientList[i]->m_sX > client.m_sX - 10) &&
								  (m_pClientList[i]->m_sX < client.m_sX + 10) &&
								  (m_pClientList[i]->m_sY > client.m_sY - 7) &&
								  (m_pClientList[i]->m_sY < client.m_sY + 7)) {

							// Crusade
							if (m_bIsCrusadeMode == true) {
								if ((client.m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
										  (m_pClientList[i]->m_cSide != client.m_cSide)) {

								} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
							} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						}
						break;

					case 1:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;

						if ((memcmp(m_pClientList[i]->m_cGuildName, client.m_cGuildName, 20) == 0) &&
								  (memcmp(m_pClientList[i]->m_cGuildName, "NONE", 4) != 0)) {

							// Crusade
							if (m_bIsCrusadeMode == true) {
								if ((client.m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
										  (m_pClientList[i]->m_cSide != client.m_cSide)) {

								} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
							} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						}
						break;

					case 2:
					case 10:
					case 35: // GM Chat By Carlos
						// Crusade
						if (m_bIsCrusadeMode == true) {
							if ((client.m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
									  (m_pClientList[i]->m_cSide != client.m_cSide)) {

							} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						break;

					case 3:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;

						if ((m_pClientList[i]->m_cSide == client.m_cSide))
							iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						break;

					case 4:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;
						if ((m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == client.m_iPartyID))
							iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						break;
				}

				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:


						//DeleteClient(i, true, true);
						break;
				}
			}
	} else {
		// New 16/05/2004
		iRet = client.m_pXSock->iSendMsg(pData, dwMsgSize);

		if (m_pClientList[ client.m_iWhisperPlayerIndex ] != nullptr &&
				  strcmp(client.m_cWhisperPlayerName, m_pClientList[ client.m_iWhisperPlayerIndex ]->m_cCharName) == 0) {
			iRet = m_pClientList[client.m_iWhisperPlayerIndex]->m_pXSock->iSendMsg(pData, dwMsgSize);
			switch (m_iLogChatOption) {

				case 1:
					if (m_pClientList[client.m_iWhisperPlayerIndex]->m_iAdminUserLevel == 0) {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "GM Whisper   (%s):\"%s\"\tto Player(%s)", client.m_cCharName, pData + 21, client.m_cWhisperPlayerName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					}
					break;

				case 2:
					if (m_pClientList[client.m_iWhisperPlayerIndex]->m_iAdminUserLevel > 0) {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "GM Whisper   (%s):\"%s\"\tto GM(%s)", client.m_cCharName, pData + 21, client.m_cWhisperPlayerName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					}
					break;

				case 3:
					if (m_pClientList[client.m_iWhisperPlayerIndex]->m_iAdminUserLevel > 0) {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "GM Whisper   (%s):\"%s\"\tto GM(%s)", client.m_cCharName, pData + 21, client.m_cWhisperPlayerName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					} else {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "Player Whisper   (%s):\"%s\"\tto Player(%s)", client.m_cCharName, pData + 21, client.m_cWhisperPlayerName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					}
					break;

				case 4:
					break;
			}
		}

		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:


				//DeleteClient(i, true, true);
				break;
		}
	}
}

void CGame::ChatMsgHandlerGSM(int iMsgType, int iV1, char * pName, char * pData, uint32_t dwMsgSize) {
	register int i;
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	char * cp, cTemp[256];

	std::memset(cTemp, 0, sizeof (cTemp));

	dwp = (uint32_t *) cTemp;
	*dwp = MSGID_COMMAND_CHATMSG;

	wp = (uint16_t *) (cTemp + DEF_INDEX2_MSGTYPE);
	*wp = 0;

	cp = (char *) (cTemp + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	*sp = 0;
	cp += 2;

	sp = (short *) cp;
	*sp = 0;
	cp += 2;

	memcpy(cp, pName, 10);
	cp += 10;

	*cp = (char) iMsgType;
	cp++;

	memcpy(cp, pData, dwMsgSize);
	cp += dwMsgSize;

	switch (iMsgType) {
		case 1:
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if (m_pClientList[i] != nullptr) {
					if (m_pClientList[i]->m_bIsInitComplete == false) break;
					if ((m_pClientList[i]->m_iGuildGUID == iV1) && (m_pClientList[i]->m_iGuildGUID != 0)) {

						m_pClientList[i]->m_pXSock->iSendMsg(cTemp, dwMsgSize + 22);
					}
				}
			break;

		case 2:
		case 10:
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if (m_pClientList[i] != nullptr) {
					m_pClientList[i]->m_pXSock->iSendMsg(cTemp, dwMsgSize + 22);
				}
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse, bool bIsDash)
//  description			:: controls player attack
//	return value		:: int
//  last updated		:: October 29, 2004; 8:06 PM; Hypnotoad
//  commentary			:: - contains attack hack detection
//						   - added checks for Firebow and Directionbow to see if player is m_bIsInsideOwnTown
//						   - added ability to attack moving object
//						   - fixed attack unmoving object
/////////////////////////////////////////////////////////////////////////////////////
// Incomplete:
//			- Direction Bow damage disabled
//
/////////////////////////////////////////////////////////////////////////////////////

int CGame::iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse, bool bIsDash) {
	char cData[100];
	uint32_t * dwp, dwTime;
	uint16_t * wp;
	int iRet, iExp, tdX = 0, tdY = 0, i;
	short sOwner, sAbsX, sAbsY;
	char cOwnerType;
	bool bNearAttack = false, var_AC = false;
	short sItemIndex;
	int tX, tY, iErr, iStX, iStY;

	if (m_pClientList[iClientH] == nullptr) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;

	dwTime = timeGetTime();
	m_pClientList[iClientH]->m_dwLastActionTime = dwTime;
	m_pClientList[iClientH]->m_iAttackMsgRecvCount++;
	if (m_pClientList[iClientH]->m_iAttackMsgRecvCount >= 7) {
		if (m_pClientList[iClientH]->m_dwAttackLAT != 0) {
			if ((dwTime - m_pClientList[iClientH]->m_dwAttackLAT) < (3500)) {
				DeleteClient(iClientH, true, true, true);
				return 0;
			}
		}
		m_pClientList[iClientH]->m_dwAttackLAT = dwTime;
		m_pClientList[iClientH]->m_iAttackMsgRecvCount = 0;
	}

	if ((wTargetObjectID != 0) && (wType != 2)) {
		if (wTargetObjectID < DEF_MAXCLIENTS) {
			if (m_pClientList[wTargetObjectID] != nullptr) {
				tdX = m_pClientList[wTargetObjectID]->m_sX;
				tdY = m_pClientList[wTargetObjectID]->m_sY;
			}
		} else if ((wTargetObjectID > 10000) && (wTargetObjectID < (10000 + DEF_MAXNPCS))) {
			if (m_pNpcList[wTargetObjectID - 10000] != nullptr) {
				tdX = m_pNpcList[wTargetObjectID - 10000]->m_sX;
				tdY = m_pNpcList[wTargetObjectID - 10000]->m_sY;
			}
		}

		m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->GetOwner(&sOwner, &cOwnerType, dX, dY);
		if (sOwner == (wTargetObjectID - 10000)) {
			tdX = m_pNpcList[sOwner]->m_sX;
			dX = tdX;
			tdY = m_pNpcList[sOwner]->m_sY;
			dY = tdY;
			bNearAttack = false;
			var_AC = true;
		}
		if (var_AC != true) {
			if ((tdX == dX) && (tdY == dY)) {
				bNearAttack = false;
			} else if ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1)) {
				dX = tdX;
				dY = tdY;
				bNearAttack = true;
			}
		}
	}

	if ((dX < 0) || (dX >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeX) ||
			  (dY < 0) || (dY >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY)) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != nullptr) {
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (m_pClientList[iClientH]->m_cSide) {
			case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}

	sAbsX = abs(sX - dX);
	sAbsY = abs(sY - dY);
	if ((wType != 2) && (wType < 20)) {
		if (var_AC == false) {
			sItemIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
			if (sItemIndex != -1) {
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return 0;
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 845) {
					if ((sAbsX > 4) || (sAbsY > 4)) wType = 0;
				} else {
					if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
				}
			} else {
				if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
			}
		} else if (var_AC != false) {
			sItemIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
			if (sItemIndex != -1) {
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return 0;
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 845) {
					cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
					if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bCheckFlySpaceAvailable(sX, sY, cDir, sOwner)) != false) {
						wType = 0;
					}
				} else {
					if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
				}
			} else {
				if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
			}
		} else {
			if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
		}
	}


	ClearSkillUsingStatus(iClientH);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

	m_pClientList[iClientH]->m_cDir = cDir;

	iExp = 0;
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, dX, dY);

	if (sOwner != 0) {
		if ((wType != 0) && ((dwTime - m_pClientList[iClientH]->m_dwRecentAttackTime) > 100)) {
			if ((m_pClientList[iClientH]->m_pIsProcessingAllowed == false) && (m_pClientList[iClientH]->m_bIsInsideOwnTown == false)) {
				sItemIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
				if (sItemIndex != -1 && m_pClientList[iClientH]->m_pItemList[sItemIndex] != nullptr) {
					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 874) { // Directional bow
						iErr = 0;
						for (i = 1; i < 2; i++) {
							m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
							m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, tX, tY);
							iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, tX, tY, wType, bNearAttack, bIsDash, true); // 1
							if (((abs(tdX - dX) >= 1) && (abs(tdY - dY) >= 1)) || ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1))) {
								m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, dX, dY);
								iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
							}
						}
					} else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 873) { // Firebow
						if ((strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresden") == 0) ||
								  (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvine") == 0) ||
								  (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "arefarm") == 0) ||
								  (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvfarm") == 0)) {
							iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false);
						} else {

							if ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) != 0) {
								if (m_bHeldenianInitiated != 1) {
									iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, DEF_DYNAMICOBJECT_FIRE3, m_pClientList[iClientH]->m_cMapIndex, dX, dY, (iDice(1, 7) + 3)*1000, 8);
								}
								iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
							}
						}
					} else {
						iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
					}
				} else {
					iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
				}
			} else {
				iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
			}
			if (m_pClientList[iClientH] == nullptr) return 0;
			m_pClientList[iClientH]->m_dwRecentAttackTime = dwTime;
		}
	} else _CheckMiningAction(iClientH, dX, dY);

	if (iExp != 0) {
		GetExp(iClientH, iExp, true);
	}

	if (bResponse == true) {
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_MOTION;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_OBJECTMOTION_ATTACK_CONFIRM;

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, true, true);
				return 0;
		}
	}

	return 1;
}

char CGame::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int * pError) {
	char cDir, cTmpDir;
	int aX, aY, dX, dY;
	int i, iResX, iResY;

	if ((sX == dstX) && (sY == dstY)) return 0;

	dX = sX;
	dY = sY;

	if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1)) {
		iResX = dstX;
		iResY = dstY;
	} else m_Misc.GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);

	cDir = m_Misc.cGetNextMoveDir(dX, dY, iResX, iResY);

	if (cTurn == 0)
		for (i = cDir; i <= cDir + 7; i++) {
			cTmpDir = i;
			if (cTmpDir > 8) cTmpDir -= 8;
			aX = _tmp_cTmpDirX[cTmpDir];
			aY = _tmp_cTmpDirY[cTmpDir];
			if (m_pMapList[cMapIndex]->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
		}

	if (cTurn == 1)
		for (i = cDir; i >= cDir - 7; i--) {
			cTmpDir = i;
			if (cTmpDir < 1) cTmpDir += 8;
			aX = _tmp_cTmpDirX[cTmpDir];
			aY = _tmp_cTmpDirY[cTmpDir];
			if (m_pMapList[cMapIndex]->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
		}

	return 0;
}


char _tmp_cEmptyPosX[] = {0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2};
char _tmp_cEmptyPosY[] = {0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2};

bool CGame::bGetEmptyPosition(short * pX, short * pY, char cMapIndex) {
	register int i;
	short sX, sY;


	for (i = 0; i < 25; i++)
		if ((m_pMapList[cMapIndex]->bGetMoveable(*pX + _tmp_cEmptyPosX[i], *pY + _tmp_cEmptyPosY[i]) == true) &&
				  (m_pMapList[cMapIndex]->bGetIsTeleport(*pX + _tmp_cEmptyPosX[i], *pY + _tmp_cEmptyPosY[i]) == false)) {
			sX = *pX + _tmp_cEmptyPosX[i];
			sY = *pY + _tmp_cEmptyPosY[i];
			*pX = sX;
			*pY = sY;
			return true;
		}



	GetMapInitialPoint(cMapIndex, &sX, &sY);
	*pX = sX;
	*pY = sY;

	return false;
}

void CGame::NpcBehavior_Move(int iNpcH) {
	char cDir;
	short sX, sY, dX, dY, absX, absY;
	short sTarget, sDistance;
	char cTargetType;

	if (m_pNpcList[iNpcH] == nullptr) return;
	if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;

	if ((m_pNpcList[iNpcH]->m_bIsSummoned == true) &&
			  (m_pNpcList[iNpcH]->m_iSummonControlMode == 1)) return;
	if (m_pNpcList[iNpcH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) return;

	// �̵��� �� ��� NPC�� ����
	switch (m_pNpcList[iNpcH]->m_cActionLimit) {
		case 2:
		case 3:
		case 5:
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_STOP;
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			return;
	}

	// v1.432-2 �� ���� ������ �����Ѵ�.
	int iStX, iStY;
	if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex] != nullptr) {
		iStX = m_pNpcList[iNpcH]->m_sX / 20;
		iStY = m_pNpcList[iNpcH]->m_sY / 20;
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iMonsterActivity++;
	}

	// �� ī��Ʈ ����.
	m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;
	if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 5) {
		// ���� �� ī��Ʈ Ŭ����.
		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;

		// ���ڸ����� ��� �ɵ��� �ִ��� �Ǵ��Ѵ�.
		absX = abs(m_pNpcList[iNpcH]->m_vX - m_pNpcList[iNpcH]->m_sX);
		absY = abs(m_pNpcList[iNpcH]->m_vY - m_pNpcList[iNpcH]->m_sY);

		if ((absX <= 2) && (absY <= 2)) {
			// ���ڸ��� �ɵ��� �ִ�. ���� WayPoint�� �������� �ٲ۴�.
			CalcNextWayPointDestination(iNpcH);
		}

		m_pNpcList[iNpcH]->m_vX = m_pNpcList[iNpcH]->m_sX;
		m_pNpcList[iNpcH]->m_vY = m_pNpcList[iNpcH]->m_sY;
	}

	// ����� ����� �˻��Ѵ�.
	TargetSearch(iNpcH, &sTarget, &cTargetType);
	if (sTarget != 0) {
		// ��ݸ�ǥ �߰�.
		if (m_pNpcList[iNpcH]->m_dwActionTime < 1000) {
			// �׼� ���ǵ� 1000ms �̸��� NPC�� 33% Ȯ��� ��ǥ�� ȹ���Ѵ�.
			if (iDice(1, 3) == 3) {
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
				m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
				// ���⼭ ǥȿ ���۰������� ���� �޽��� �߼�.
				return;
			}
		} else {
			// �׼� ���ǵ� 1000ms �̻��� NPC�� ��ǥ���� �߰ߵǸ� ������ �Ҵ��Ѵ�.
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
			m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
			// ���⼭ ǥȿ ���۰������� ���� �޽��� �߼�.
			return;
		}
	}

	// ������ ���� �̵� �ӵ��� ������.
	if ((m_pNpcList[iNpcH]->m_bIsMaster == true) && (iDice(1, 3) == 2)) return;

	if (m_pNpcList[iNpcH]->m_cMoveType == DEF_MOVETYPE_FOLLOW) {

		sX = m_pNpcList[iNpcH]->m_sX;
		sY = m_pNpcList[iNpcH]->m_sY;
		switch (m_pNpcList[iNpcH]->m_cFollowOwnerType) {
			case DEF_OWNERTYPE_PLAYER:

				if (m_pClientList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex] == nullptr) {
					m_pNpcList[iNpcH]->m_cMoveType = DEF_MOVETYPE_RANDOM;
					return;
				}

				dX = m_pClientList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sX;
				dY = m_pClientList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sY;
				break;
			case DEF_OWNERTYPE_NPC:

				if (m_pNpcList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex] == nullptr) {
					m_pNpcList[iNpcH]->m_cMoveType = DEF_MOVETYPE_RANDOM;
					m_pNpcList[iNpcH]->m_iFollowOwnerIndex = 0;
					//bSerchMaster(iNpcH);
					return;
				}

				dX = m_pNpcList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sX;
				dY = m_pNpcList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sY;
				break;
		}

		if (abs(sX - dX) >= abs(sY - dY))
			sDistance = abs(sX - dX);
		else sDistance = abs(sY - dY);

		if (sDistance >= 3) {
			// ��� ����� �Ӱ�ġ �̻� ������ ������ �̵��Ѵ�.
			cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
			if (cDir == 0) {
				// �̵��� �� ���.
			} else {
				dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
				dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
				// ���� ��ġ���� �����.
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(3, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
				// �� ��ġ�� ǥ���Ѵ�.
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
				m_pNpcList[iNpcH]->m_sX = dX;
				m_pNpcList[iNpcH]->m_sY = dY;
				m_pNpcList[iNpcH]->m_cDir = cDir;
				SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
			}
		}
	} else {
		// ���� �̵� ������ ���Ѵ�.
		cDir = cGetNextMoveDir(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY,
				  m_pNpcList[iNpcH]->m_dX, m_pNpcList[iNpcH]->m_dY,
				  m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);

		if (cDir == 0) {
			// ���� �̵����� ���Ѵ�. ������ �� ���� ���ڸ����� ��� �ӹ��� ����
			if (iDice(1, 10) == 3) CalcNextWayPointDestination(iNpcH);
		} else {
			dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
			dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
			// ���� ��ġ���� �����.
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(4, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
			// �� ��ġ�� ǥ���Ѵ�.
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
			m_pNpcList[iNpcH]->m_sX = dX;
			m_pNpcList[iNpcH]->m_sY = dY;
			m_pNpcList[iNpcH]->m_cDir = cDir;
			SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
		}
	}
}

void CGame::TargetSearch(int iNpcH, short * pTarget, char * pTargetType) {
	register int ix, iy, iPKCount;
	register short sX, sY, rX, rY, dX, dY;
	short sOwner, sTargetOwner, sDistance, sTempDistance;
	char cOwnerType, cTargetType, cTargetSide;
	int iInv;

	sTargetOwner = 0;
	cTargetType = 0;
	sDistance = 100;

	sX = m_pNpcList[iNpcH]->m_sX;
	sY = m_pNpcList[iNpcH]->m_sY;


	rX = m_pNpcList[iNpcH]->m_sX - m_pNpcList[iNpcH]->m_cTargetSearchRange;
	rY = m_pNpcList[iNpcH]->m_sY - m_pNpcList[iNpcH]->m_cTargetSearchRange;

	for (ix = rX; ix < rX + m_pNpcList[iNpcH]->m_cTargetSearchRange * 2 + 1; ix++)
		for (iy = rY; iy < rY + m_pNpcList[iNpcH]->m_cTargetSearchRange * 2 + 1; iy++) {

			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, ix, iy);
			if (sOwner != 0) {

				if ((sOwner == iNpcH) && (cOwnerType == DEF_OWNERTYPE_NPC)) break;



				iPKCount = 0;
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwner] == nullptr) {

							m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(5, sOwner, DEF_OWNERTYPE_PLAYER, ix, iy);
						} else {
							if (m_pClientList[sOwner]->m_iAdminUserLevel > 0) goto SKIP_SEARCH;
							if (m_pClientList[sOwner]->m_cSide == 0) goto SKIP_SEARCH; // v2.24 ��ǥ��� NETURAL ����* �����
							dX = m_pClientList[sOwner]->m_sX;
							dY = m_pClientList[sOwner]->m_sY;
							cTargetSide = m_pClientList[sOwner]->m_cSide;
							iPKCount = m_pClientList[sOwner]->m_iPKCount;
							iInv = m_pClientList[sOwner]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ];
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwner] == nullptr) {

							m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(6, sOwner, DEF_OWNERTYPE_NPC, ix, iy);
						} else {
							dX = m_pNpcList[sOwner]->m_sX;
							dY = m_pNpcList[sOwner]->m_sY;
							cTargetSide = m_pNpcList[sOwner]->m_cSide;
							iPKCount = 0;
							iInv = m_pNpcList[sOwner]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ];

							if (m_pNpcList[iNpcH]->m_sType == 21) {

								if (_iCalcPlayerNum(m_pNpcList[sOwner]->m_cMapIndex, dX, dY, 2) != 0) {
									sOwner = 0;
									cOwnerType = 0;
								}
							}
						}
						break;
				}

				if (m_pNpcList[iNpcH]->m_cSide < 10) {

					if (cTargetSide == 0) {

						if (iPKCount == 0) goto SKIP_SEARCH;
					} else {

						if ((iPKCount == 0) && (cTargetSide == m_pNpcList[iNpcH]->m_cSide)) goto SKIP_SEARCH;

						if (m_pNpcList[iNpcH]->m_cSide == 0) goto SKIP_SEARCH;
					}
				} else {

					if ((cOwnerType == DEF_OWNERTYPE_NPC) && (cTargetSide == 0)) goto SKIP_SEARCH;
					if (cTargetSide == m_pNpcList[iNpcH]->m_cSide) goto SKIP_SEARCH;
				}


				if ((iInv != 0) && (m_pNpcList[iNpcH]->m_cSpecialAbility != 1)) goto SKIP_SEARCH;

				if (abs(sX - dX) >= abs(sY - dY))
					sTempDistance = abs(sX - dX);
				else sTempDistance = abs(sY - dY);

				if (sTempDistance < sDistance) {
					sDistance = sTempDistance;
					sTargetOwner = sOwner;
					cTargetType = cOwnerType;
				}
SKIP_SEARCH:
				;
			}
		}

	*pTarget = sTargetOwner;
	*pTargetType = cTargetType;
	return;
}

void CGame::NpcBehavior_Attack(int iNpcH) {
	int iMagicType;
	short sX, sY, dX, dY;
	char cDir;
	uint32_t dwTime = timeGetTime();

	if (m_pNpcList[iNpcH] == nullptr) return;
	if (m_pNpcList[iNpcH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) return;
	if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;

	switch (m_pNpcList[iNpcH]->m_cActionLimit) {
		case 1:
		case 2:
		case 3:
		case 4:
			return;

		case 5:
			if (m_pNpcList[iNpcH]->m_iBuildCount > 0) return;
	}

	// v1.432-2 �� ���� ������ �����Ѵ�.
	int iStX, iStY;
	if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex] != nullptr) {
		iStX = m_pNpcList[iNpcH]->m_sX / 20;
		iStY = m_pNpcList[iNpcH]->m_sY / 20;
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iMonsterActivity++;
	}

	// ���� ��� ī��Ʈ �ʱ�ȭ
	if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount == 0)
		m_pNpcList[iNpcH]->m_iAttackCount = 0;

	// �� ī��Ʈ ����.
	m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;
	if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 20) {
		// 20ȸ �̻� ��ݵ����� �����ϸ� �ٽ� �̵����� ����. �� ���� ��� ��尡 �ƴ϶��
		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;

		if ((m_pNpcList[iNpcH]->m_bIsPermAttackMode == false))
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;

		// ���⼭ ǥȿ�����̳� ���������� ���ϱ� ���� �޽����� �߼��� ���� �ִ�.
		return;
	}

	// ��ġ�� ���Ѵ�.
	sX = m_pNpcList[iNpcH]->m_sX;
	sY = m_pNpcList[iNpcH]->m_sY;

	switch (m_pNpcList[iNpcH]->m_cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex] == nullptr) {
				// ��ǥ���� �������.
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
				return;
			}
			dX = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;

		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex] == nullptr) {
				// ��ǥ���� �������.
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
				return;
			}
			dX = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;
	}

	// ��ǥ�� ������ ���� ������ ����ģ��. �� ���๰(5)�� ����
	if ((iGetDangerValue(iNpcH, dX, dY) > m_pNpcList[iNpcH]->m_cBravery) &&
			  (m_pNpcList[iNpcH]->m_bIsPermAttackMode == false) &&
			  (m_pNpcList[iNpcH]->m_cActionLimit != 5)) {

		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
		m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
		return;
	}

	// �������� ���� Bravery�� ������ ����ģ��. �� ���๰(5)�� ����
	if ((m_pNpcList[iNpcH]->m_iHP <= 2) && (iDice(1, m_pNpcList[iNpcH]->m_cBravery) <= 3) &&
			  (m_pNpcList[iNpcH]->m_bIsPermAttackMode == false) &&
			  (m_pNpcList[iNpcH]->m_cActionLimit != 5)) {

		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
		m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
		return;
	}


	if ((abs(sX - dX) <= 1) && (abs(sY - dY) <= 1)) {
		// ������ �ִ�.	�ٰŸ� ���⸦ �����ϰ� �ִٸ� ��ݰ���.

		cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
		if (cDir == 0) return;
		// ������ �ٲٰ� ��ݵ���
		m_pNpcList[iNpcH]->m_cDir = cDir;

		if (m_pNpcList[iNpcH]->m_cActionLimit == 5) {
			// ����� �ǹ��� ���
			switch (m_pNpcList[iNpcH]->m_sType) {
				case 89:
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
					m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
					NpcMagicHandler(iNpcH, dX, dY, 61);
					break;

				case 87:
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2);
					iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2);
					break;

				case 36: // Crossbow Guard Tower: Ȱ ���
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 2); // Ȱ
					iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2, false, false, false);
					break;

				case 37: // Cannon Guard Tower:
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
					m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
					NpcMagicHandler(iNpcH, dX, dY, 61);
					break;
			}
		} else {
			// �Ϲ� NPC�� ���
			SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 1); // 1 : Į���� ��������� ����ϴ� �ǹ�
			iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 1, false, false); // ��ݿ� ���� ȿ�� ����Ѵ�.
		}
		// ���� ��� ī��Ʈ ����.
		m_pNpcList[iNpcH]->m_iAttackCount++;


		if ((m_pNpcList[iNpcH]->m_bIsPermAttackMode == false) && (m_pNpcList[iNpcH]->m_cActionLimit == 0)) {
			switch (m_pNpcList[iNpcH]->m_iAttackStrategy) {
				case DEF_ATTACKAI_EXCHANGEATTACK:
					// ��� -> ���� -> ��� -> ���� -> ...
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
					break;

				case DEF_ATTACKAI_TWOBYONEATTACK:
					// ��� -> ��� -> ���� -> ��� -> ��� -> ���� -> ...
					if (m_pNpcList[iNpcH]->m_iAttackCount >= 2) {
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
					}
					break;
			}
		}
	} else {
		// ������ �ִ�.
		cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
		if (cDir == 0) return;
		m_pNpcList[iNpcH]->m_cDir = cDir;

		// ���� �ɷ��� �־�� �ϰ� �����Ÿ� ������ �Ѵ�.
		if ((m_pNpcList[iNpcH]->m_cMagicLevel > 0) && (iDice(1, 2) == 1) &&
				  (abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
			iMagicType = -1;
			switch (m_pNpcList[iNpcH]->m_cMagicLevel) { // �� ��Ŭ���� ���� ����� �������� �Ǵ��� ó��.
				case 1:
					if (m_pMagicConfigList[0]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 0;
					break;

				case 2:
					if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 10;
					else if (m_pMagicConfigList[0]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 0;
					break;

				case 3: // Orc-Mage
					if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 20;
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 10;
					break;

				case 4:
					if (m_pMagicConfigList[30]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 30;
					else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 37;
					else if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 20;
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 10;
					break;

				case 5: // Rudolph, Cannibal-Plant, Cyclops
					if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 43;
					else if (m_pMagicConfigList[30]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 30;
					else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 37;
					else if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 20;
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 10;
					break;

				case 6: // Tentocle, Liche
					if (m_pMagicConfigList[51]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 51;
					else if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 43;
					else if (m_pMagicConfigList[30]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 30;
					else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 37;
					else if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 20;
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 10;
					break;

				case 7: // Barlog, Fire-Wyvern, MasterMage-Orc , LightWarBeatle, GHK, GHKABS, TK, BG
					// Sor, Gagoyle, Demon
					if ((m_pMagicConfigList[70]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 5) == 3))
						iMagicType = 70;
					else if (m_pMagicConfigList[61]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 61;
					else if (m_pMagicConfigList[60]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 60;
					else if (m_pMagicConfigList[51]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 51;
					else if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 43;
					break;

				case 8: // Unicorn, Centaurus
					if ((m_pMagicConfigList[35]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 35;
					else if (m_pMagicConfigList[60]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 60;
					else if (m_pMagicConfigList[51]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 51;
					else if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 43;
					break;

				case 9: // Tigerworm
					if ((m_pMagicConfigList[74]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 74; // Lightning-Strike
					break;

				case 10: // Frost, Nizie
					break;

				case 11: // Ice-Golem
					break;

				case 12: // Wyvern
					if ((m_pMagicConfigList[91]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 91; // Blizzard
					else if (m_pMagicConfigList[63]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 63; // Mass-Chill-Wind
					break;

				case 13: // Abaddon
					if ((m_pMagicConfigList[96]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 92; // Earth Shock Wave
					else if (m_pMagicConfigList[81]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
						iMagicType = 96; // Metoer Strike
					break;

			}

			if (iMagicType != -1) {


				if (m_pNpcList[iNpcH]->m_iAILevel >= 2) {
					switch (m_pNpcList[iNpcH]->m_cTargetType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] == 2) {
								// ���� ��ȣ�� �ɷ����� ������� ������ �߰��ϰų� �̵����� ��ȯ, ���ο� ��ǥ���� �˻�
								if ((abs(sX - dX) > m_pNpcList[iNpcH]->m_iAttackRange) || (abs(sY - dY) > m_pNpcList[iNpcH]->m_iAttackRange)) {
									m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
									m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
									return;
								} else goto NBA_CHASE;
							}
							// �з������ε� ��ǥ���� �̹� �з����¶�� ���� ��� ����.
							if ((iMagicType == 35) && (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0)) goto NBA_CHASE;
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] == 2) {
								// ���� ��ȣ�� �ɷ����� ������� ������ �߰����� �ʰ� �̵����� ��ȯ, ���ο� ��ǥ���� �˻�
								if ((abs(sX - dX) > m_pNpcList[iNpcH]->m_iAttackRange) || (abs(sY - dY) > m_pNpcList[iNpcH]->m_iAttackRange)) {
									m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
									m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
									return;
								} else goto NBA_CHASE;
							}
							// �з������ε� ��ǥ���� �̹� �з����¶�� ���� ��� ����.
							if ((iMagicType == 35) && (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0)) goto NBA_CHASE;
							break;
					}
				}

				SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 1); // 1 : Į���� ��������� ����ϴ� �ǹ�
				NpcMagicHandler(iNpcH, dX, dY, iMagicType);
				m_pNpcList[iNpcH]->m_dwTime = dwTime + 2000;
				return;
			}
		}

		// ������ ���� ����� ������ ��� (MagicLevel�� ������ ���)
		if ((m_pNpcList[iNpcH]->m_cMagicLevel < 0) && (iDice(1, 2) == 1) &&
				  (abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
			iMagicType = -1;
			if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
				iMagicType = 43;
			else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
				iMagicType = 37;
			else if (m_pMagicConfigList[0]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana)
				iMagicType = 0;

			if (iMagicType != -1) {
				SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 1); // 1 : Į���� ��������� ����ϴ� �ǹ�
				NpcMagicHandler(iNpcH, dX, dY, iMagicType);
				m_pNpcList[iNpcH]->m_dwTime = dwTime + 2000;
				return;
			}
		}

		// v1.41 ���� �ɷ��� ������ ��Ÿ� ����� �����ϴٸ�
		if ((m_pNpcList[iNpcH]->m_iAttackRange > 1) &&
				  (abs(sX - dX) <= m_pNpcList[iNpcH]->m_iAttackRange) && (abs(sY - dY) <= m_pNpcList[iNpcH]->m_iAttackRange)) {

			cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
			if (cDir == 0) return;
			// ������ �ٲٰ� ��ݵ���
			m_pNpcList[iNpcH]->m_cDir = cDir;

			if (m_pNpcList[iNpcH]->m_cActionLimit == 5) {
				switch (m_pNpcList[iNpcH]->m_sType) {
					case 36: // Crossbow Guard Tower
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2);
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2);
						break;

					case 37: // Cannon Guard Tower: �Ž� ���̾� ��Ʈ����ũ ���
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
						m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
						NpcMagicHandler(iNpcH, dX, dY, 61);
						break;
				}
			} else {
				switch (m_pNpcList[iNpcH]->m_sType) {
					case 51: // v2.05 Catapult: �Ž� ���̾� ��Ʈ����ũ ���
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
						m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
						NpcMagicHandler(iNpcH, dX, dY, 61);
						break;

					case 54: // Dark Elf: Ȱ ����� �Ѵ�.
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2); // 2: Ȱ���
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2);
						break;

					case 63: // Frost
					case 79: // Nizie
						switch (m_pNpcList[iNpcH]->m_cTargetType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((m_pMagicConfigList[57]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2))
									NpcMagicHandler(iNpcH, dX, dY, 57);

								if ((m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) &&
										  (bCheckResistingIceSuccess(m_pNpcList[iNpcH]->m_cDir, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, m_pNpcList[iNpcH]->m_iMagicHitRatio) == false)) {
									if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, true);
										// ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
										// ����� �÷��̾��� ��� �˷��ش�.
										SendNotifyMsg(0, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((m_pMagicConfigList[57]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2))
									NpcMagicHandler(iNpcH, dX, dY, 57);
								if ((m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) &&
										  (bCheckResistingIceSuccess(m_pNpcList[iNpcH]->m_cDir, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_iMagicHitRatio) == false)) {
									if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, true);
										// ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
					case 53: //Beholder: �õ� ����� �Ѵ�.
						switch (m_pNpcList[iNpcH]->m_cTargetType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex] == nullptr) goto NBA_BREAK1;

								if ((m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) &&
										  (bCheckResistingIceSuccess(m_pNpcList[iNpcH]->m_cDir, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, m_pNpcList[iNpcH]->m_iMagicHitRatio) == false)) {
									if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, true);
										// ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
										// ����� �÷��̾��� ��� �˷��ش�.
										SendNotifyMsg(0, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) &&
										  (bCheckResistingIceSuccess(m_pNpcList[iNpcH]->m_cDir, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_iMagicHitRatio) == false)) {
									if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, true);
										// ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
NBA_BREAK1:
						;
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 20); // 20: �ʻ��
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 20);
						break;

					default:
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 20); // 20: �ʻ��
						// ��ݿ� ���� ȿ�� ����Ѵ�.
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 20);
						break;
				}
			}
			// ���� ��� ī��Ʈ ����.
			m_pNpcList[iNpcH]->m_iAttackCount++;


			if ((m_pNpcList[iNpcH]->m_bIsPermAttackMode == false) && (m_pNpcList[iNpcH]->m_cActionLimit == 0)) {
				switch (m_pNpcList[iNpcH]->m_iAttackStrategy) {
					case DEF_ATTACKAI_EXCHANGEATTACK:
						// ��� -> ���� -> ��� -> ���� -> ...
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
						break;

					case DEF_ATTACKAI_TWOBYONEATTACK:
						// ��� -> ��� -> ���� -> ��� -> ��� -> ���� -> ...
						if (m_pNpcList[iNpcH]->m_iAttackCount >= 2) {
							m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
							m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
						}
						break;
				}
			}
			return;
		}

		// ������ ������� �ʾҴٸ� �߰��Ѵ�.
NBA_CHASE:
		;

		if (m_pNpcList[iNpcH]->m_cActionLimit != 0) return;

		// ���� ��� ī��Ʈ �ʱ�ȭ
		m_pNpcList[iNpcH]->m_iAttackCount = 0;

		{
			// �̵���尡 Follow�� �ƴ϶�� ��ݸ�ǥ�� ������ �߰��Ѵ�.	�����ӿ� ������ ���.
			cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
			if (cDir == 0) {
				return;
			}
			dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
			dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
			// ���� ��ġ���� �����.
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(9, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
			// �� ��ġ�� ǥ���Ѵ�.
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
			m_pNpcList[iNpcH]->m_sX = dX;
			m_pNpcList[iNpcH]->m_sY = dY;
			m_pNpcList[iNpcH]->m_cDir = cDir;
			SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
		}
	}
}

void CGame::RemoveFromTarget(short sTargetH, char cTargetType, int iCode) {
	register int i;
	uint32_t dwTime = timeGetTime();

	for (i = 0; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {


			if ((m_pNpcList[i]->m_iGuildGUID != 0) && (cTargetType == DEF_OWNERTYPE_PLAYER) &&
					  (m_pClientList[sTargetH]->m_iGuildGUID == m_pNpcList[i]->m_iGuildGUID)) {

				if (m_pNpcList[i]->m_cActionLimit == 0) {

					m_pNpcList[i]->m_bIsSummoned = true;
					m_pNpcList[i]->m_dwSummonedTime = dwTime;
				}
			}

			if ((m_pNpcList[i]->m_iTargetIndex == sTargetH) &&
					  (m_pNpcList[i]->m_cTargetType == cTargetType)) {

				switch (iCode) {
					case DEF_MAGICTYPE_INVISIBILITY:
						if (m_pNpcList[i]->m_cSpecialAbility == 1) {

						} else {
							m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
							m_pNpcList[i]->m_iTargetIndex = 0;
							m_pNpcList[i]->m_cTargetType = 0;
						}
						break;

					default:
						m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
						m_pNpcList[i]->m_iTargetIndex = 0;
						m_pNpcList[i]->m_cTargetType = 0;
						break;
				}
			}
		}
}

void CGame::NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage) {
	short sAttackerWeapon;
	int i, iQuestIndex, iExp, iConstructionPoint, iWarContribution;
	double dTmp1, dTmp2, dTmp3;
	char cQuestRemain;

	if (m_pNpcList[iNpcH] == nullptr) return;
	if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;

	m_pNpcList[iNpcH]->m_bIsKilled = true;
	m_pNpcList[iNpcH]->m_iHP = 0;
	m_pNpcList[iNpcH]->m_iLastDamage = sDamage;


	m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalAliveObject--;


	RemoveFromTarget(iNpcH, DEF_OWNERTYPE_NPC);


	ReleaseFollowMode(iNpcH, DEF_OWNERTYPE_NPC);


	m_pNpcList[iNpcH]->m_iTargetIndex = 0;
	m_pNpcList[iNpcH]->m_cTargetType = 0;

	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		sAttackerWeapon = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
	} else sAttackerWeapon = 1;
	SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);

	m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(10, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

	m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetDeadOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

	m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_DEAD;


	m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;

	m_pNpcList[iNpcH]->m_dwDeadTime = timeGetTime();


	if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;


	NpcDeadItemGenerator(iNpcH, sAttackerH, cAttackerType);



	if ((m_pNpcList[iNpcH]->m_bIsSummoned != true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) &&
			  (m_pClientList[sAttackerH] != nullptr)) {


		//Happy Hour para vos 
		if (m_bHappyHour == true) {
			iExp = (m_pNpcList[iNpcH]->m_iExp);
		} else {
			iExp = (m_pNpcList[iNpcH]->m_iExp / 3);
		}
		if (m_pNpcList[iNpcH]->m_iNoDieRemainExp > 0)
			iExp += m_pNpcList[iNpcH]->m_iNoDieRemainExp;


		if (m_pClientList[sAttackerH]->m_iAddExp != 0) {
			dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
			dTmp2 = (double) iExp;
			dTmp3 = (dTmp1 / 100.0f) * dTmp2;
			iExp += (int) dTmp3;
		}

		if (m_pNpcList[iNpcH]->m_sType == 81) {
			for (i = 1; i < DEF_MAXCLIENTS; i++) {
				if (m_pClientList[i] != nullptr) {
					SendNotifyMsg(sAttackerH, i, DEF_NOTIFY_ABADDONKILLED, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
				}
			}
		}


		if (m_bIsCrusadeMode == true) {
			if (iExp > 10) iExp = iExp / 3;
		}

		if (m_pNpcList[iNpcH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] == 1) {
			iExp *= 2;
		}

		// New 09/05/2004
		//m_pClientList[sAttackerH]->m_iExpStock += iExp;
		GetExp(sAttackerH, iExp, true);


		iQuestIndex = m_pClientList[sAttackerH]->m_iQuest;
		if (iQuestIndex != 0) {
			if (m_pQuestConfigList[iQuestIndex] != nullptr) {
				switch (m_pQuestConfigList[iQuestIndex]->m_iType) {
					case DEF_QUESTTYPE_MONSTERHUNT:
						if ((m_pClientList[sAttackerH]->m_bQuestMatchFlag_Loc == true) &&
								  (m_pQuestConfigList[iQuestIndex]->m_iTargetType == m_pNpcList[iNpcH]->m_sType)) {

							m_pClientList[sAttackerH]->m_iCurQuestCount++;
							cQuestRemain = (m_pQuestConfigList[m_pClientList[sAttackerH]->m_iQuest]->m_iMaxCount - m_pClientList[sAttackerH]->m_iCurQuestCount);
							SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, nullptr);
							_bCheckIsQuestCompleted(sAttackerH);
						}
						break;
				}
			}
		}
	}


	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		switch (m_pNpcList[iNpcH]->m_sType) {
			case 32:

				m_pClientList[sAttackerH]->m_iRating -= 0;
				// v1.41 Rating �϶� 
				m_pClientList[sAttackerH]->m_iRating -= 0;
				if (m_pClientList[sAttackerH]->m_iRating > 10000) m_pClientList[sAttackerH]->m_iRating = 10000;
				if (m_pClientList[sAttackerH]->m_iRating < -10000) m_pClientList[sAttackerH]->m_iRating = -10000;
				break;

			case 33:
				break;
		}
	}

	// Crusade
	iConstructionPoint = 0;
	switch (m_pNpcList[iNpcH]->m_sType) {

		case 1: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 2: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 3: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 4: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 5: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 6: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 36: iConstructionPoint = 700;
			iWarContribution = 4000;
			break;
		case 37: iConstructionPoint = 700;
			iWarContribution = 4000;
			break;
		case 38: iConstructionPoint = 500;
			iWarContribution = 2000;
			break;
		case 39: iConstructionPoint = 500;
			iWarContribution = 2000;
			break;
		case 40: iConstructionPoint = 1500;
			iWarContribution = 5000;
			break;
		case 41: iConstructionPoint = 5000;
			iWarContribution = 10000;
			break;
		case 43: iConstructionPoint = 500;
			iWarContribution = 1000;
			break;
		case 44: iConstructionPoint = 1000;
			iWarContribution = 2000;
			break;
		case 45: iConstructionPoint = 1500;
			iWarContribution = 3000;
			break;
		case 46: iConstructionPoint = 1000;
			iWarContribution = 2000;
			break;
		case 47: iConstructionPoint = 1500;
			iWarContribution = 3000;
			break;
		case 64: m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCropsTotalSum();
			break;

	}

	// Crusade
	if (iConstructionPoint != 0) {
		switch (cAttackerType) {
			case DEF_OWNERTYPE_PLAYER:
				if (m_pClientList[sAttackerH]->m_cSide != m_pNpcList[iNpcH]->m_cSide) {

					m_pClientList[sAttackerH]->m_iConstructionPoint += iConstructionPoint;

					if (m_pClientList[sAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
						m_pClientList[sAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

					m_pClientList[sAttackerH]->m_iWarContribution += iWarContribution;
					if (m_pClientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
						m_pClientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

					//testcode
					wsprintf(G_cTxt, "Enemy NULLNpc Killed by player! Construction: +%d WarContribution: +%d", iConstructionPoint, iWarContribution);
					PutLogList(G_cTxt);

					SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 0, nullptr);
				} else {

					m_pClientList[sAttackerH]->m_iWarContribution -= (iWarContribution * 2);
					if (m_pClientList[sAttackerH]->m_iWarContribution < 0)
						m_pClientList[sAttackerH]->m_iWarContribution = 0;

					//testcode
					wsprintf(G_cTxt, "Friendly Npc Killed by player! WarContribution: -%d", iWarContribution);
					PutLogList(G_cTxt);

					SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 0, nullptr);
				}
				break;

			case DEF_OWNERTYPE_NPC:
				if (m_pNpcList[sAttackerH]->m_iGuildGUID != 0) {
					if (m_pNpcList[sAttackerH]->m_cSide != m_pNpcList[iNpcH]->m_cSide) {


						for (i = 1; i < DEF_MAXCLIENTS; i++)
							if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iGuildGUID == m_pNpcList[sAttackerH]->m_iGuildGUID) &&
									  (m_pClientList[i]->m_iCrusadeDuty == 3)) {

								m_pClientList[i]->m_iConstructionPoint += iConstructionPoint;
								if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
									m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

								//testcode
								wsprintf(G_cTxt, "Enemy Npc Killed by Npc! Construct point +%d", iConstructionPoint);
								PutLogList(G_cTxt);

								SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, nullptr);
								goto NKH_GOTOPOINT1;
							}
					}
				}
				break;
		}
	}

	// new - heldenian
	if ((m_bIsHeldenianMode == true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_cSide != m_pNpcList[iNpcH]->m_cSide)) {
		switch (m_pNpcList[iNpcH]->m_sType) {
			case 82: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 83: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 84: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 85: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 86: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 87: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 88: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 89: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
		}
		if (iWarContribution > 0) m_pClientList[sAttackerH]->m_iWarContribution += iWarContribution;
		if (m_pClientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION) m_pClientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
		SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 1, 0, 0, 0);
	}

NKH_GOTOPOINT1:
	;


	if (m_pNpcList[iNpcH]->m_cSpecialAbility == 7) {
		m_pNpcList[iNpcH]->m_iMana = 100;
		m_pNpcList[iNpcH]->m_iMagicHitRatio = 100;
		NpcMagicHandler(iNpcH, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 30);
	} else if (m_pNpcList[iNpcH]->m_cSpecialAbility == 8) {
		m_pNpcList[iNpcH]->m_iMana = 100;
		m_pNpcList[iNpcH]->m_iMagicHitRatio = 100;
		NpcMagicHandler(iNpcH, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 61);
	}

	if ((m_bIsHeldenianMode == true) && (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_bIsHeldenianMap == true) && (m_cHeldenianModeType == 1)) {
		if ((m_pNpcList[iNpcH]->m_sType == 87) || (m_pNpcList[iNpcH]->m_sType == 89)) {
			if (m_pNpcList[iNpcH]->m_cSide == 1) {
				m_iHeldenianAresdenLeftTower--;
				wsprintf(G_cTxt, "Aresden Tower Broken, Left TOWER %d", m_iHeldenianAresdenLeftTower);
			} else if (m_pNpcList[iNpcH]->m_cSide == 2) {
				m_iHeldenianElvineLeftTower--;
				wsprintf(G_cTxt, "Elvine Tower Broken, Left TOWER %d", m_iHeldenianElvineLeftTower);
			}
			PutLogList(G_cTxt);
			UpdateHeldenianStatus();
		}
		if ((m_iHeldenianElvineLeftTower == 0) || (m_iHeldenianAresdenLeftTower == 0)) {
			GlobalEndHeldenianMode();
		}
	}
}

int CGame::iGetDangerValue(int iNpcH, short dX, short dY) {
	register int ix, iy, iDangerValue;
	short sOwner, sDOType;
	char cOwnerType;
	uint32_t dwRegisterTime;

	if (m_pNpcList[iNpcH] == nullptr) return false;

	iDangerValue = 0;

	for (ix = dX - 2; ix <= dX + 2; ix++)
		for (iy = dY - 2; iy <= dY + 2; iy++) {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, ix, iy);
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bGetDynamicObject(ix, iy, &sDOType, &dwRegisterTime);

			if (sDOType == 1) iDangerValue++;

			switch (cOwnerType) {
				case 0:
					break;
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwner] == nullptr) break;
					if (m_pNpcList[iNpcH]->m_cSide != m_pClientList[sOwner]->m_cSide)
						iDangerValue++;
					else iDangerValue--;
					break;
				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwner] == nullptr) break;
					if (m_pNpcList[iNpcH]->m_cSide != m_pNpcList[sOwner]->m_cSide)
						iDangerValue++;
					else iDangerValue--;
					break;
			}
		}

	return iDangerValue;
}

void CGame::NpcBehavior_Flee(int iNpcH) {
	char cDir;
	short sX, sY, dX, dY;
	short sTarget;
	char cTargetType;

	if (m_pNpcList[iNpcH] == nullptr) return;
	if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;

	// �� ī��Ʈ ����.
	m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;

	// ��� �ΰ����ɿ� ���� ȿ����.
	switch (m_pNpcList[iNpcH]->m_iAttackStrategy) {
		case DEF_ATTACKAI_EXCHANGEATTACK: // ��� -> ���� -> ...
		case DEF_ATTACKAI_TWOBYONEATTACK: // ��� -> ��� -> ���� -> ...
			if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 2) {
				// �ٽ� ��� ���� ��ȯ.
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				return;
			}
			break;

		default:
			if (iDice(1, 2) == 1) NpcRequestAssistance(iNpcH);
			break;
	}

	if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 10) {
		// 10���� �ϸ� ������ �� �ٽ� �̵����� ��ȯ.
		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
		m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
		m_pNpcList[iNpcH]->m_tmp_iError = 0;
		if (m_pNpcList[iNpcH]->m_iHP <= 3) {
			m_pNpcList[iNpcH]->m_iHP += iDice(1, m_pNpcList[iNpcH]->m_iHitDice); // ! ������ �������� ���������� ���������� ���������Ƿ� ������ ����.
			if (m_pNpcList[iNpcH]->m_iHP <= 0) m_pNpcList[iNpcH]->m_iHP = 1;
		}
		return;
	}


	TargetSearch(iNpcH, &sTarget, &cTargetType);
	if (sTarget != 0) {
		m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
		m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
	}

	// ��ǥ���� �ڽ��� ��ġ�� ���Ѵ�. �ݴ�������� ����ġ�� ����.
	sX = m_pNpcList[iNpcH]->m_sX;
	sY = m_pNpcList[iNpcH]->m_sY;
	switch (m_pNpcList[iNpcH]->m_cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			dX = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;
		case DEF_OWNERTYPE_NPC:
			dX = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;
	}
	dX = sX - (dX - sX);
	dY = sY - (dY - sY);

	cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
	if (cDir == 0) {
		// �̵��� �� ���.
	} else {
		dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
		dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
		// ���� ��ġ���� �����.
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(11, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
		// �� ��ġ�� ǥ���Ѵ�.
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
		m_pNpcList[iNpcH]->m_sX = dX;
		m_pNpcList[iNpcH]->m_sY = dY;
		m_pNpcList[iNpcH]->m_cDir = cDir;
		SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
	}
}

void CGame::processClientMsg(CClient &client, uint32_t msgId, char *pData, uint32_t dwMsgSize, char cKey) {
	int iClientH = client.id_;
	switch (msgId) { // 84148741

		case DEF_REQUEST_RESURRECTPLAYER_YES:
			RequestResurrectPlayer(client, true);
			break;

		case DEF_REQUEST_RESURRECTPLAYER_NO:
			RequestResurrectPlayer(client, false);
			break;

			//Shorcut para Criticals by 
		case DEF_REQUEST_CRITICALSADD:
			PlayerOrder_GetCrits(client);
			break;

			//Shorcut para Criticals by 
		case DEF_REQUEST_SHOWBALLPOINTS:
			SendNotifyMsg(0, client.id_, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, nullptr);
			break;

		case DEF_REQUEST_ANGEL: // Angels by Snoopy...
			GetAngelHandler(client, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_SELLITEMLIST:
			RequestSellItemListHandler(client, pData);
			break;

		case MSGID_REQUEST_RESTART:
			RequestRestartHandler(client);
			SendNotifyMsg(0, client.id_, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
			break;

		case MSGID_REQUEST_PANNING:
			iRequestPanningMapDataRequest(client, pData);
			break;

		case MSGID_REQUEST_NOTICEMENT:
			RequestNoticementHandler(client, pData);
			break;

		case MSGID_BWM_COMMAND_SHUTUP:
			_BWM_Command_Shutup(pData);
			break;

		case MSGID_BWM_INIT:
			_BWM_Init(client, pData);
			break;

		case MSGID_REQUEST_SETITEMPOS:
			_SetItemPos(client, pData);
			break;

		case MSGID_REQUEST_FULLOBJECTDATA:
			RequestFullObjectData(client, pData);
			break;

		case MSGID_REQUEST_RETRIEVEITEM:
			RequestRetrieveItemHandler(client, pData);
			break;

		case MSGID_REQUEST_CIVILRIGHT:
			RequestCivilRightHandler(client, pData);
			break;

		case MSGID_REQUEST_TELEPORT:
			RequestTeleportHandler(iClientH, "1");
			break;

		case MSGID_REQUEST_INITPLAYER:
			RequestInitPlayerHandler(client, pData, cKey);
			SendNotifyMsg(0, client.id_, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
			break;

		case MSGID_REQUEST_INITDATA:
		{
			uint32_t dwTime = timeGetTime();
			// Anti Bump
			if (client.m_bIsClientConnected == true) {
				if (m_pClientList[iClientH] == nullptr) break;
				wsprintf(G_cTxt, "(!!!) Client (%s) connection closed!. Sniffer suspect!.", client.m_cCharName);
				PutLogList(G_cTxt);
				m_pMapList[client.m_cMapIndex]->ClearOwner(2, iClientH, DEF_OWNERTYPE_PLAYER, client.m_sX, client.m_sY);
				delayEvents_.bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, 0);

				bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, iClientH, false);
				if ((dwTime - m_dwGameTime2) > 3000) { // 3 segs
					client.m_bIsClientConnected = false;
					m_iTotalClients--;
					delete m_pClientList[iClientH];
					m_pClientList[iClientH] = nullptr;
				}
				break;
			} else {
				client.m_bIsClientConnected = true;
				RequestInitDataHandler(iClientH, pData, cKey);
			}
		}
			break;

		case MSGID_COMMAND_COMMON:
			ClientCommonHandler(iClientH, pData);
			break;

		case MSGID_COMMAND_MOTION:
			ClientMotionHandler(client, pData);
			break;

		case MSGID_COMMAND_CHECKCONNECTION:
			CheckConnectionHandler(iClientH, pData);
			break;

		case MSGID_COMMAND_CHATMSG:
			ChatMsgHandler(client, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_CREATENEWGUILD:
			RequestCreateNewGuildHandler(iClientH, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_DISBANDGUILD:
			RequestDisbandGuildHandler(iClientH, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_FIGHTZONE_RESERVE:
			FightzoneReserveHandler(iClientH, pData, dwMsgSize);
			break;

		case MSGID_LEVELUPSETTINGS:
			LevelUpSettingsHandler(iClientH, pData, dwMsgSize);
			break;

		case MSGID_STATECHANGEPOINT:
			StateChangeHandler(iClientH, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_HELDENIANTELEPORT:
			RequestHeldenianTeleport(iClientH, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_NPCSPEAKTELEPORT:
			RequestNpcSpeakTeleport(iClientH, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_HELDENIAN_SCROLL:
			RequestHeldenianScroll(iClientH, pData, dwMsgSize);
			break;

		case MSGID_REQUEST_CITYHALLTELEPORT:
			if (memcmp(client.m_cLocation, "aresden", 7) == 0) {
				RequestTeleportHandler(iClientH, "2   ", "elvine", 205, 63);
			} else if (memcmp(client.m_cLocation, "elvine", 6) == 0) {
				RequestTeleportHandler(client.id_, "2   ", "aresden", 49, 49);
			}
			break;

		default:
		{
			char m_msgBuff[1000];
			wsprintf(m_msgBuff, "Unknown message received! (0x%.8X) Delete Client", msgId);
			PutLogList(m_msgBuff);
			DeleteClient(client.id_, true, true); // v1.4
		}
			break;
	}
}

void CGame::MsgProcess() {
	char * pData, cFrom, cKey;
	uint32_t dwMsgSize, * dwpMsgID;
	uint16_t * wpMsgType;
	int i, iClientH;

	if ((m_bF5pressed == true) && (m_bF1pressed == true)) {
		PutLogList("(XXX) RELOADING CONFIGS MANUALY...");
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, i);
			}
		bInit();
	}

	if ((m_bF1pressed == true) && (m_bF4pressed == true) && (m_bOnExitProcess == false)) {

		m_cShutDownCode = 2;
		m_bOnExitProcess = true;
		m_dwExitProcessTime = timeGetTime();

		PutLogList("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Local command)!!!");

		bSendMsgToLS(MSGID_GAMESERVERSHUTDOWNED, 0);

		if (m_iMiddlelandMapIndex > 0) {
			// Crusade
			SaveOccupyFlagData();
			//bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, nullptr, nullptr);
			//bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, nullptr, nullptr);
		}

		return;
	}

	ZeroMemory(m_pMsgBuffer, DEF_MSGBUFFERSIZE + 1);
	pData = (char *) m_pMsgBuffer;

	m_iCurMsgs = 0;
	while (bGetMsgQuene(&cFrom, pData, &dwMsgSize, &iClientH, &cKey) == true) {


		m_iCurMsgs++;
		if (m_iCurMsgs > m_iMaxMsgs) m_iMaxMsgs = m_iCurMsgs;

		switch (cFrom) {
			case DEF_MSGFROM_CLIENT:
			{
				/*client.m_cConnectionCheck++;
				if (client.m_cConnectionCheck > 3) {
					wsprintf(G_cTxt, "Client Hex Edit: (%s) Player: (%s) - has removed 3203203 (check connection handler).", client.m_cIPaddress, client.m_cCharName);
					PutHackLogFileList(G_cTxt);
					DeleteClient(iClientH, true, true);
					break;
				}*/
				CClient *clientPtr = m_pClientList[iClientH];
				if (!clientPtr) {
					continue;
				}
				CClient &client = *clientPtr;
				dwpMsgID = (uint32_t *) (pData + DEF_INDEX4_MSGID);
				processClientMsg(client, *dwpMsgID, pData, dwMsgSize, cKey);
			}
				break;

			case DEF_MSGFROM_LOGSERVER:
				dwpMsgID = (uint32_t *) (pData + DEF_INDEX4_MSGID);
				wpMsgType = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);

				switch (*dwpMsgID) {
					case MSGID_REQUEST_CHECKACCOUNTPASSWORD:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_REQUEST_CHECKACCOUNTPASSWORD");

						RequestCheckAccountPasswordHandler(pData, dwMsgSize);
						break;

					case MSGID_REQUEST_FORCEDISCONECTACCOUNT:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_REQUEST_FORCEDISCONECTACCOUNT");

						wpMsgType = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
						ForceDisconnectAccount((char *) (pData + DEF_INDEX2_MSGTYPE + 2), *wpMsgType);
						break;

					case MSGID_RESPONSE_SAVEPLAYERDATA_REPLY:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_SAVEPLAYERDATA_REPLY");

						ResponseSavePlayerDataReplyHandler(pData, dwMsgSize);
						break;

					case MSGID_GUILDNOTIFY:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_GUILDNOTIFY");

						GuildNotifyHandler(pData, dwMsgSize);
						break;

					case MSGID_RESPONSE_DISBANDGUILD:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_DISBANDGUILD");

						ResponseDisbandGuildHandler(pData, dwMsgSize);
						break;

					case MSGID_RESPONSE_CREATENEWGUILD:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_CREATENEWGUILD");

						ResponseCreateNewGuildHandler(pData, dwMsgSize);
						break;

					case MSGID_RESPONSE_REGISTERGAMESERVER:
						switch (*wpMsgType) {
							case DEF_MSGTYPE_CONFIRM:
								//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_REGISTERGAMESERVER -> DEF_MSGTYPE_CONFIRM");
								PutLogList("(!) Game Server registration to Log Server - Success!");
								m_bIsLogSockAvailable = true;
								break;

							case DEF_MSGTYPE_REJECT:
								//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_REGISTERGAMESERVER -> DEF_MSGTYPE_REJECT");
								PutLogList("(!) Game Server registration to Log Server - Fail!");
								break;
						}

						if (m_bIsBuildItemAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! Build-Item configuration error.");
						}

						if (m_bIsItemAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! Item configuration error.");
						}

						if (m_bIsNpcAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! Npc configuration error.");
						}

						if (m_bIsMagicAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! MAGIC configuration error.");
						}

						if (m_bIsSkillAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! SKILL configuration error.");
						}

						if (m_bIsQuestAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! QUEST configuration error.");
						}

						if (m_bIsPortionAvailable == false) {

							PutLogList(" ");
							PutLogList("(!!!) STOPPED! PORTION configuration error.");
						}

						//wsprintf(G_cTxt, "%d %d %d %d %d %d", (int)m_bIsGameStarted, (int)m_bIsItemAvailable, m_bIsNpcAvailable, , (int)m_bIsLogSockAvailable, (int)m_bIsMagicAvailable, (int)m_bIsSkillAvailable);
						//PutLogList(G_cTxt);

						/*
						if ((m_bIsGameStarted == false) && (m_bIsItemAvailable == true) &&
							(m_bIsNpcAvailable == true) && (m_bIsLogSockAvailable == true) && 
							(m_bIsMagicAvailable == true) &&
							(m_bIsSkillAvailable == true) && (m_bIsPortionAvailable == true) &&
							(m_bIsQuestAvailable == true) ) {
							
							SendMessage(m_hWnd, WM_USER_STARTGAMESIGNAL, nullptr, nullptr);
							m_bIsGameStarted = true;
						}
						 */
						break;

					case MSGID_RESPONSE_PLAYERDATA:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_PLAYERDATA");

						ResponsePlayerDataHandler(pData, dwMsgSize);
						break;

					case MSGID_BUILDITEMCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_BUILDITEMCONFIGURATIONCONTENTS");
						// Build Item contents
						PutLogList("(!) BUILD-ITEM configuration contents received. Now decoding...");
						m_bIsBuildItemAvailable = _bDecodeBuildItemConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_ITEMCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_ITEMCONFIGURATIONCONTENTS");

						PutLogList("(!) ITEM configuration contents received. Now decoding...");
						m_bIsItemAvailable = _bDecodeItemConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_NPCCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_NPCCONFIGURATIONCONTENTS");

						PutLogList("(!) NPC configuration contents received. Now decoding...");
						m_bIsNpcAvailable = _bDecodeNpcConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_MAGICCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_MAGICCONFIGURATIONCONTENTS");
						PutLogList("(!) MAGIC configuration contents received. Now decoding...");
						m_bIsMagicAvailable = _bDecodeMagicConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_SKILLCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_SKILLCONFIGURATIONCONTENTS");
						PutLogList("(!) SKILL configuration contents received. Now decoding...");
						m_bIsSkillAvailable = _bDecodeSkillConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_QUESTCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_QUESTCONFIGURATIONCONTENTS");
						PutLogList("(!) QUEST configuration contents received. Now decoding...");
						m_bIsQuestAvailable = _bDecodeQuestConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_PORTIONCONFIGURATIONCONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_PORTIONCONFIGURATIONCONTENTS");
						PutLogList("(!) PORTION configuration contents received. Now decoding...");
						m_bIsPortionAvailable = _bDecodePortionConfigFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

						// Crusade
						//case MSGID_ARESDENOCCUPYFLAGSAVEFILECONTENTS:
						//PutLogList("(!) Aresden OccupyFlag save file contents received. Now decoding...");
						//_bDecodeOccupyFlagSaveFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize, 1);
						//break;

						// Crusade
						//case MSGID_ELVINEOCCUPYFLAGSAVEFILECONTENTS:
						//PutLogList("(!) Elvine OccupyFlag save file contents received. Now decoding...");
						//_bDecodeOccupyFlagSaveFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize, 2);
						//break;

					case MSGID_DUPITEMIDFILECONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_DUPITEMIDFILECONTENTS");
						PutLogList("(!) DupItemID file contents received. Now decoding...");
						_bDecodeDupItemIDFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

					case MSGID_NOTICEMENTFILECONTENTS:
						//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_NOTICEMENTFILECONTENTS");
						PutLogList("(!) Noticement file contents received. Now decoding...");
						_bDecodeNoticementFileContents((char *) (pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
						break;

						// v2.17 2002-8-7 // 2002-09-06 #1
						/*case MSGID_NPCITEMCONFIGCONTENTS:
							PutLogList("(!) NpcItemConfig file contents received. Now decoding...");
							_bDecodeNpcItemConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
							break;*/

				}
				break;
		}
	}
}

bool CGame::bPutMsgQuene(char cFrom, char * pData, uint32_t dwMsgSize, int iIndex, char cKey) {
	/*
	HANDLE hMutex;

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, m_cServerName);
	if (hMutex != nullptr) return false; 

	hMutex = CreateMutex(nullptr, false, m_cServerName);
	 */

	if (m_pMsgQuene[m_iQueneTail] != nullptr) return false;


	m_pMsgQuene[m_iQueneTail] = new class CMsg;
	if (m_pMsgQuene[m_iQueneTail] == nullptr) return false;

	if (m_pMsgQuene[m_iQueneTail]->bPut(cFrom, pData, dwMsgSize, iIndex, cKey) == false) return false;

	m_iQueneTail++;
	if (m_iQueneTail >= DEF_MSGQUENESIZE) m_iQueneTail = 0;


	//ReleaseMutex(hMutex);

	return true;
}

bool CGame::bGetMsgQuene(char * pFrom, char * pData, uint32_t * pMsgSize, int * pIndex, char * pKey) {
	/*
	HANDLE hMutex;

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, m_cServerName);
	if (hMutex != nullptr) return false; 

	hMutex = CreateMutex(nullptr, false, m_cServerName);
	 */

	if (m_pMsgQuene[m_iQueneHead] == nullptr) return false;

	m_pMsgQuene[m_iQueneHead]->Get(pFrom, pData, pMsgSize, pIndex, pKey);

	delete m_pMsgQuene[m_iQueneHead];
	m_pMsgQuene[m_iQueneHead] = nullptr;

	m_iQueneHead++;
	if (m_iQueneHead >= DEF_MSGQUENESIZE) m_iQueneHead = 0;


	//ReleaseMutex(hMutex);

	return true;
}

void CGame::ClientCommonHandler(int iClientH, char * pData) {
	uint16_t * wp, wCommand;
	short * sp, sX, sY;
	int * ip, iV1, iV2, iV3, iV4;
	char * cp, cDir, * pString;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;

	wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	wCommand = *wp;

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	sp = (short *) cp;
	sX = *sp;
	cp += 2;

	sp = (short *) cp;
	sY = *sp;
	cp += 2;

	cDir = *cp;
	cp++;

	ip = (int *) cp;
	iV1 = *ip;
	cp += 4;

	ip = (int *) cp;
	iV2 = *ip;
	cp += 4;

	ip = (int *) cp;
	iV3 = *ip;
	cp += 4;

	pString = cp;
	cp += 30;

	ip = (int *) cp;
	iV4 = *ip;
	cp += 4;

	switch (wCommand) {



			// New 15/05/2004
		case DEF_COMMONTYPE_REQ_CREATESLATE:
			ReqCreateSlateHandler(iClientH, pData);
			break;

			// 2.06 - by KLKS
		case DEF_COMMONTYPE_REQ_CHANGEPLAYMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> MSGID_REQUEST_CIVILRIGHT");
			RequestChangePlayMode(iClientH);
			break;
			//

		case DEF_COMMONTYPE_SETGUILDTELEPORTLOC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDTELEPORTLOC");
			RequestSetGuildTeleportLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, "middleland");
			break;

		case DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC");
			RequestSetGuildConstructLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, pString);
			break;

		case DEF_COMMONTYPE_GUILDTELEPORT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GUILDTELEPORT");
			RequestGuildTeleportHandler(iClientH);
			break;

		case DEF_COMMONTYPE_SUMMONWARUNIT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SUMMONWARUNIT");
			RequestSummonWarUnitHandler(iClientH, sX, sY, iV1, iV2, iV3);
			break;

		case DEF_COMMONTYPE_REQUEST_HELP:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_HELP");
			RequestHelpHandler(iClientH);
			break;

		case DEF_COMMONTYPE_REQUEST_MAPSTATUS:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_MAPSTATUS");
			MapStatusHandler(iClientH, iV1, pString);
			break;

		case DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY");
			SelectCrusadeDutyHandler(iClientH, iV1);
			break;

		case DEF_COMMONTYPE_REQUEST_CANCELQUEST:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_CANCELQUEST");
			CancelQuestHandler(iClientH);
			break;

		case DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY");
			ActivateSpecialAbilityHandler(iClientH);
			break;

		case DEF_COMMONTYPE_REQUEST_JOINPARTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_JOINPARTY");
			JoinPartyHandler(iClientH, iV1, pString);
			break;

		case DEF_COMMONTYPE_GETMAGICABILITY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GETMAGICABILITY");
			GetMagicAbilityHandler(iClientH);
			break;

		case DEF_COMMONTYPE_BUILDITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_BUILDITEM");
			BuildItemHandler(iClientH, pData);
			break;

		case DEF_COMMONTYPE_QUESTACCEPTED:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_QUESTACCEPTED");
			QuestAcceptedHandler(iClientH);
			break;

		case DEF_COMMONTYPE_CANCELEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CANCELEXCHANGEITEM");
			CancelExchangeItem(iClientH);
			break;

		case DEF_COMMONTYPE_CONFIRMEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CONFIRMEXCHANGEITEM");
			ConfirmExchangeItem(iClientH);
			break;

		case DEF_COMMONTYPE_SETEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETEXCHANGEITEM");
			SetExchangeItem(iClientH, iV1, iV2);
			break;

		case DEF_COMMONTYPE_REQ_GETHEROMANTLE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETHEROMANTLE");
			GetHeroMantleHandler(iClientH, iV1, pString);
			break;

		case DEF_COMMONTYPE_REQ_GETTRADEEK: // By Luqah
			GetTradeEKMantleHandler(iClientH, iV1, pString);
			break;

		case DEF_COMMONTYPE_REQ_GETOCCUPYFLAG:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFLAG");
			GetOccupyFlagHandler(iClientH);
			break;

		case DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX");
			SetDownSkillIndexHandler(iClientH, iV1);
			break;

		case DEF_COMMONTYPE_TALKTONPC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TALKTONPC");
			// works for client, but for npc it returns middleland
			// if ((m_pMapList[m_pNpcList[iV1]->m_cMapIndex]->m_cName) != (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName)) break;
			NpcTalkHandler(iClientH, iV1);
			break;

		case DEF_COMMONTYPE_REQ_CREATEPORTION:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_CREATEPORTION");
			ReqCreatePortionHandler(iClientH, pData);
			break;

		case DEF_COMMONTYPE_REQ_GETFISHTHISTIME:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETFISHTHISTIME");
			ReqGetFishThisTimeHandler(iClientH);
			break;

		case DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM");
			ReqRepairItemCofirmHandler(iClientH, iV1, pString);
			break;

		case DEF_COMMONTYPE_REQ_REPAIRITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEM");
			ReqRepairItemHandler(iClientH, iV1, iV2, pString);
			break;

		case DEF_COMMONTYPE_REQ_SELLITEMCONFIRM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEMCONFIRM");
			ReqSellItemConfirmHandler(iClientH, iV1, iV2, pString);
			break;

		case DEF_COMMONTYPE_REQ_SELLITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEM");
			ReqSellItemHandler(iClientH, iV1, iV2, iV3, pString);
			break;

		case DEF_COMMONTYPE_REQ_USESKILL:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USESKILL");
			UseSkillHandler(iClientH, iV1, iV2, iV3);
			break;

		case DEF_COMMONTYPE_REQ_USEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USEITEM");
			UseItemHandler(iClientH, iV1, iV2, iV3, iV4);
			break;

		case DEF_COMMONTYPE_REQ_GETREWARDMONEY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETREWARDMONEY");
			GetRewardMoneyHandler(iClientH);
			break;

		case DEF_COMMONTYPE_ITEMDROP:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_ITEMDROP");
			DropItemHandler(iClientH, iV1, iV2, pString, true);
			break;

		case DEF_COMMONTYPE_EQUIPITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EQUIPITEM");

			bEquipItemHandler(iClientH, iV1);

			// .....
			break;

		case DEF_COMMONTYPE_REQ_PURCHASEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_PURCHASEITEM");

			RequestPurchaseItemHandler(iClientH, pString, iV1);
			break;

		case DEF_COMMONTYPE_REQ_STUDYMAGIC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_STUDYMAGIC");

			RequestStudyMagicHandler(iClientH, pString);
			break;

		case DEF_COMMONTYPE_REQ_TRAINSKILL:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_TRAINSKILL");
			//RequestTrainSkillHandler(iClientH, pString);
			break;

		case DEF_COMMONTYPE_GIVEITEMTOCHAR:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GIVEITEMTOCHAR");

			GiveItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
			break;

		case DEF_COMMONTYPE_EXCHANGEITEMTOCHAR:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EXCHANGEITEMTOCHAR");
			ExchangeItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
			break;

		case DEF_COMMONTYPE_JOINGUILDAPPROVE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDAPPROVE");
			JoinGuildApproveHandler(iClientH, pString);
			break;

		case DEF_COMMONTYPE_JOINGUILDREJECT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDREJECT");
			JoinGuildRejectHandler(iClientH, pString);
			break;

		case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDAPPROVE");
			DismissGuildApproveHandler(iClientH, pString);
			break;

		case DEF_COMMONTYPE_DISMISSGUILDREJECT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDREJECT");
			DismissGuildRejectHandler(iClientH, pString);
			break;

		case DEF_COMMONTYPE_RELEASEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_RELEASEITEM");

			ReleaseItemHandler(iClientH, iV1, true);
			break;

		case DEF_COMMONTYPE_TOGGLECOMBATMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLECOMBATMODE");
			ToggleCombatModeHandler(iClientH);
			break;

		case DEF_COMMONTYPE_MAGIC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_MAGIC");
			PlayerMagicHandler(iClientH, iV1, iV2, (iV3 - 100));
			break;

		case DEF_COMMONTYPE_TOGGLESAFEATTACKMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLESAFEATTACKMODE");
			ToggleSafeAttackModeHandler(iClientH);
			break;


		case DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET");
			GetFightzoneTicketHandler(iClientH);
			break;

			// Upgrade Item
		case DEF_COMMONTYPE_UPGRADEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_UPGRADEITEM");
			RequestItemUpgradeHandler(iClientH, iV1);
			break;

		case DEF_COMMONTYPE_REQGUILDNAME:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
			RequestGuildNameHandler(iClientH, iV1, iV2);
			break;

		case DEF_COMMONTYPE_REQRANGO: // MORLA 2.2 - Llama al rango del pj
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
			RequestRango(iClientH, iV1);
			break;

		case DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY");
			RequestAcceptJoinPartyHandler(iClientH, iV1);
			break;

			// Crafting
		case DEF_COMMONTYPE_CRAFTITEM:
			ReqCreateCraftingHandler(iClientH, pData);
			break;

		default:
			wsprintf(G_cTxt, "Unknown message received! (0x%.8X)", wCommand);
			PutLogList(G_cTxt);
			break;
	}
}

// New 07/05/2004

void CGame::DropItemHandler(int iClientH, short sItemIndex, int iAmount, char * pItemName, bool bByPlayer) {
	class CItem * pItem;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return;
	if ((iAmount != -1) && (iAmount < 0)) return;


	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
			  (iAmount == -1))
		iAmount = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount;



	if (memcmp(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) return;

	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
			  (((int) m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount - iAmount) > 0)) {

		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName) == false) {

			delete pItem;
			return;
		} else {
			if (iAmount <= 0) {

				delete pItem;
				return;
			}
			pItem->m_dwCount = (uint32_t) iAmount;
		}




		if ((uint32_t) iAmount > m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount) {
			delete pItem;
			return;
		}

		m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount -= iAmount;


		// v1.41 !!!
		SetItemCount(iClientH, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount);


		m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY, pItem);



		if (bByPlayer == true)
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, pItem);
		else
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, pItem, true);


		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0, nullptr);
	} else {




		ReleaseItemHandler(iClientH, sItemIndex, true);


		if (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == true)
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

		// v1.432
		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) &&
				  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan == 0)) {

			m_pClientList[iClientH]->m_pItemList[sItemIndex].reset();
		} else {

			m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
					  m_pClientList[iClientH]->m_sY,
					  &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);



			if (bByPlayer == true)
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			else
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, &*m_pClientList[iClientH]->m_pItemList[sItemIndex], true);


			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); //v1.4 color
		}


		m_pClientList[iClientH]->m_pItemList[sItemIndex] = nullptr;
		m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;


		SendNotifyMsg(0, iClientH, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, nullptr);


		m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	}


	iCalcTotalWeight(iClientH);
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: check if player is dropping item or picking up item
//  last updated		:: October 29, 2004; 7:12 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////

int CGame::iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir) {
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	short * sp, sRemainItemSprite, sRemainItemSpriteFrame;
	char cRemainItemColor, cData[100];
	int iRet, iEraseReq;
	class CItem * pItem;

	if (m_pClientList[iClientH] == nullptr) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	int iStX, iStY;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != nullptr) {
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (m_pClientList[iClientH]->m_cSide) {
			case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}

	ClearSkillUsingStatus(iClientH);

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

	pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(sX, sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
	if (pItem != nullptr) {
		if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {

			_bItemLog(DEF_ITEMLOG_GET, iClientH, 0, pItem);

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMOBTAINED;

			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

			*cp = 1;
			cp++;

			memcpy(cp, pItem->m_cName, 20);
			cp += 20;

			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;

			*cp = pItem->m_cItemType;
			cp++;

			*cp = pItem->m_cEquipPos;
			cp++;

			*cp = (char) 0;
			cp++;

			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;

			*cp = pItem->m_cGenderLimit;
			cp++;

			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;

			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;

			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;

			*cp = pItem->m_cItemColor;
			cp++;

			*cp = (char) pItem->m_sItemSpecEffectValue2;
			cp++;

			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;

			if (iEraseReq == 1) delete pItem;

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
					  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					  sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					DeleteClient(iClientH, true, true);
					return 0;
			}
		} else {
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(sX, sY, pItem);

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					DeleteClient(iClientH, true, true);
					return 0;
			}
		}
	}

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(iClientH, true, true);
			return 0;
	}

	return 1;
}

bool CGame::_bAddClientItemList(int iClientH, class CItem * pItem, int * pDelReq) {
	register int i;

	if (m_pClientList[iClientH] == nullptr) return false;
	if (pItem == nullptr) return false;


	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {

		if ((m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(*pItem, pItem->m_dwCount)) > _iCalcMaxLoad(iClientH))
			return false;
	} else {

		if ((m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(*pItem, 1)) > _iCalcMaxLoad(iClientH))
			return false;
	}


	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {
		for (i = 0; i < DEF_MAXITEMS; i++)
			if ((m_pClientList[iClientH]->m_pItemList[i] != nullptr) &&
					  (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, pItem->m_cName, 20) == 0)) {

				m_pClientList[iClientH]->m_pItemList[i]->m_dwCount += pItem->m_dwCount;
				//delete pItem;
				*pDelReq = 1;


				iCalcTotalWeight(iClientH);

				return true;
			}
	}

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] == nullptr) {

			m_pClientList[iClientH]->m_pItemList[i].reset(pItem);

			m_pClientList[iClientH]->m_ItemPosList[i].x = 40;
			m_pClientList[iClientH]->m_ItemPosList[i].y = 30;

			*pDelReq = 0;


			if (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)
				m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);


			iCalcTotalWeight(iClientH);

			return true;
		}


	return false;
}

bool CGame::bEquipItemHandler(int iClientH, short sItemIndex, bool bNotify) {
	char cEquipPos, cHeroArmorType;
	short sSpeed;
	short sTemp;
	int iTemp;

	if (m_pClientList[iClientH] == nullptr) return false;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return false;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return false;

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan == 0) return false;

	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) == 0) &&
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sLevelLimit > m_pClientList[iClientH]->m_iLevel)) return false;


	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 0) {
		switch (m_pClientList[iClientH]->m_sType) {
			case 1:
			case 2:
			case 3:

				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 1) return false;
				break;
			case 4:
			case 5:
			case 6:

				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 2) return false;
				break;
		}
	}

	if (iGetItemWeight(*m_pClientList[iClientH]->m_pItemList[sItemIndex], 1) > (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr)*100) return false;

	cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;

	if ((cEquipPos == DEF_EQUIPPOS_BODY) || (cEquipPos == DEF_EQUIPPOS_LEGGINGS) ||
			  (cEquipPos == DEF_EQUIPPOS_ARMS) || (cEquipPos == DEF_EQUIPPOS_HEAD)) {
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4) {
			case 10:
				if ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 11: // Dex
				if ((m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iAngelicDex) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 12: // Vit
				if (m_pClientList[iClientH]->m_iVit < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 13: // Int
				if ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 14: // Mag
				if ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 15: // Chr
				if (m_pClientList[iClientH]->m_iCharisma < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {

					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);

					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
		}
	}

	if (cEquipPos == DEF_EQUIPPOS_TWOHAND) {
		// Stormbringer
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 845) {
			if ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iInt) < 65) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, sItemIndex, 0, nullptr);
				ReleaseItemHandler(iClientH, sItemIndex, true);
				return false;
			}
		}
	}

	if (cEquipPos == DEF_EQUIPPOS_RHAND) {
		// Resurrection wand(MS.10) or Resurrection wand(MS.20)
		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
			if ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) > 99 && (m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) > 99 && m_pClientList[iClientH]->m_iSpecialAbilityTime < 1) {
				m_pClientList[iClientH]->m_cMagicMastery[94] = true;
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
			}
		}
	}

	if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) {

		if ((m_pClientList[iClientH]->m_iSpecialAbilityType != 0)) {


			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos != m_pClientList[iClientH]->m_iSpecialAbilityEquipPos) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, m_pClientList[iClientH]->m_sItemEquipmentStatus[ m_pClientList[iClientH]->m_iSpecialAbilityEquipPos ], 0, nullptr);

				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ m_pClientList[iClientH]->m_iSpecialAbilityEquipPos ], true);
			}
		}
	}


	if (cEquipPos == DEF_EQUIPPOS_NONE) return false;

	if (cEquipPos == DEF_EQUIPPOS_TWOHAND) {

		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ] != -1)
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], false);
		else {

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1)
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ], false);
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LHAND ] != -1)
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LHAND ], false);
		}
	} else {

		if ((cEquipPos == DEF_EQUIPPOS_LHAND) || (cEquipPos == DEF_EQUIPPOS_RHAND)) {

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1)
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ], false);
		}


		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ] != -1)
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], false);
	}


	if (cEquipPos == DEF_EQUIPPOS_RELEASEALL) {
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], false);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_HEAD ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_HEAD ], false);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BODY ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BODY ], false);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_ARMS ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_ARMS ], false);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LEGGINGS ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LEGGINGS ], false);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_PANTS ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_PANTS ], false);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BACK ] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BACK ], false);
		}
	} else {
		if (cEquipPos == DEF_EQUIPPOS_HEAD || cEquipPos == DEF_EQUIPPOS_BODY || cEquipPos == DEF_EQUIPPOS_ARMS ||
				  cEquipPos == DEF_EQUIPPOS_LEGGINGS || cEquipPos == DEF_EQUIPPOS_PANTS || cEquipPos == DEF_EQUIPPOS_BACK) {
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RELEASEALL ] != -1) {
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RELEASEALL ], false);
			}
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ] != -1)
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ cEquipPos ], false);
	}


	m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] = sItemIndex;
	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = true;

	switch (cEquipPos) {

		case DEF_EQUIPPOS_HEAD:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0xFF0F;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4);
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF0;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor));
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_PANTS:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0xF0FF;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 8);
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFF0FF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 8);
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_LEGGINGS:
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0x0FFF;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12);
			m_pClientList[iClientH]->m_sAppr4 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFF0F;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 4);
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_BODY:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0x0FFF;

			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue < 100) {

				sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12);
				m_pClientList[iClientH]->m_sAppr3 = sTemp;
			} else {

				sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue - 100) << 12);
				m_pClientList[iClientH]->m_sAppr3 = sTemp;

				sTemp = m_pClientList[iClientH]->m_sAppr4;
				sTemp = sTemp | 0x080;
				m_pClientList[iClientH]->m_sAppr4 = sTemp;
			}

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFF0FFFFF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 20);
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_ARMS:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0xFFF0;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue));
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFFF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 12);
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_LHAND:

			sTemp = m_pClientList[iClientH]->m_sAppr2;
			sTemp = sTemp & 0xFFF0;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue));
			m_pClientList[iClientH]->m_sAppr2 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xF0FFFFFF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 24);
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_RHAND:

			sTemp = m_pClientList[iClientH]->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4);
			m_pClientList[iClientH]->m_sAppr2 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 28);
			m_pClientList[iClientH]->m_iApprColor = iTemp;


			iTemp = m_pClientList[iClientH]->m_iStatus;
			iTemp = iTemp & 0xFFFFFFF0;
			sSpeed = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cSpeed);

			sSpeed -= ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) / 13);
			if (sSpeed < 0) sSpeed = 0;
			iTemp = iTemp | (int) sSpeed;
			m_pClientList[iClientH]->m_iStatus = iTemp;
			m_pClientList[iClientH]->m_iComboAttackCount = 0;
			break;

		case DEF_EQUIPPOS_TWOHAND:

			sTemp = m_pClientList[iClientH]->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4);
			m_pClientList[iClientH]->m_sAppr2 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 28);
			m_pClientList[iClientH]->m_iApprColor = iTemp;


			iTemp = m_pClientList[iClientH]->m_iStatus;
			iTemp = iTemp & 0xFFFFFFF0;
			sSpeed = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cSpeed);

			sSpeed -= ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) / 13);
			if (sSpeed < 0) sSpeed = 0;
			iTemp = iTemp | (int) sSpeed;
			m_pClientList[iClientH]->m_iStatus = iTemp;
			m_pClientList[iClientH]->m_iComboAttackCount = 0;
			break;

		case DEF_EQUIPPOS_BACK:
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0xF0FF;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 8);
			m_pClientList[iClientH]->m_sAppr4 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 16);
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_RELEASEALL:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12);
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;
	}

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) {

		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFF3;
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect) {
			case 0: break;
			case 1:
				m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0004;
				break;

			case 2:
				m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x000C;
				break;

			case 3:
				m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0008;
				break;
		}
	}

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY) {
		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFFC;
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect) {
			case 0:
				break;
			case 50:
			case 51:
			case 52:
				m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0002;
				break;
			default:
				if (m_pClientList[iClientH]->m_iAdminUserLevel > 0)
					m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0001;
				// m_sAppr4
				// 0x0001 GM
				// 0x0002 Green
				// 0x0003 ice element
				// 0x0004 sparkle
				// 0x0005 sparkle green gm
				// 0x0006 sparkle green
				break;
		}
	}

	cHeroArmorType = _cCheckHeroItemEquipped(iClientH);
	if (cHeroArmorType != 0x0FFFFFFFF) m_pClientList[iClientH]->m_cHeroArmourBonus = cHeroArmorType;

	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	CalcTotalItemEffect(iClientH, sItemIndex, bNotify);
	return true;

}

void CGame::SendEventToNearClient_TypeB(uint32_t dwMsgID, uint16_t wMsgType, char cMapIndex, short sX, short sY, short sV1, short sV2, short sV3, short sV4) {
	int i, iShortCutIndex;
	char * cp, cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	bool bFlag;

	std::memset(cData, 0, sizeof (cData));

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;

	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

	sp = (short *) cp;
	*sp = sX;
	cp += 2;

	sp = (short *) cp;
	*sp = sY;
	cp += 2;

	sp = (short *) cp;
	*sp = sV1;
	cp += 2;

	sp = (short *) cp;
	*sp = sV2;
	cp += 2;

	sp = (short *) cp;
	*sp = sV3;
	cp += 2;

	sp = (short *) cp;
	*sp = sV4;
	cp += 2;

	//for (i = 1; i < DEF_MAXCLIENTS; i++)
	bFlag = true;
	iShortCutIndex = 0;
	while (bFlag == true) {

		i = m_iClientShortCut[iShortCutIndex];
		iShortCutIndex++;
		if (i == 0) bFlag = false;

		if ((bFlag == true) && (m_pClientList[i] != nullptr)) {
			if ((m_pClientList[i]->m_cMapIndex == cMapIndex) &&
					  (m_pClientList[i]->m_sX >= sX - 10) &&
					  (m_pClientList[i]->m_sX <= sX + 10) &&
					  (m_pClientList[i]->m_sY >= sY - 8) &&
					  (m_pClientList[i]->m_sY <= sY + 8)) {

				m_pClientList[i]->m_pXSock->iSendMsg(cData, 18);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: checks if player is stopped
//  last updated		:: October 29, 2004; 6:46 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////

int CGame::iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir) {
	char cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	short sOwnerH;
	char cOwnerType;

	if (m_pClientList[iClientH] == nullptr) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) {
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, sX, sY);
		if (sOwnerH != 0) {
			DeleteClient(iClientH, true, true);
			return 0;
		}
	}

	ClearSkillUsingStatus(iClientH);

	int iStX, iStY;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != nullptr) {
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (m_pClientList[iClientH]->m_cSide) {
			case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}

	m_pClientList[iClientH]->m_cDir = cDir;

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(iClientH, true, true);
			return 0;
	}

	return 1;
}

void CGame::ResponseCreateNewGuildHandler(char * pData, uint32_t /*dwMsgSize*/) {
	register int i;
	uint16_t * wp, wResult;
	uint32_t * dwp;
	char * cp, cCharName[11], cData[100], cTxt[120];
	int iRet;


	std::memset(cCharName, 0, sizeof (cCharName));
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	cp += 10;


	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0) &&
				  (m_pClientList[i]->m_iLevel >= 20) && (m_pClientList[i]->m_iCharisma >= 20)) {

			wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
			switch (*wp) {
				case DEF_LOGRESMSGTYPE_CONFIRM:

					wResult = DEF_MSGTYPE_CONFIRM;

					m_pClientList[i]->m_iGuildRank = 0;
					wsprintf(cTxt, "(!) New guild(%s) creation success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
					PutLogList(cTxt);
					break;

				case DEF_LOGRESMSGTYPE_REJECT:


					wResult = DEF_MSGTYPE_REJECT;
					std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
					memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
					m_pClientList[i]->m_iGuildRank = -1;
					m_pClientList[i]->m_iGuildGUID = -1;
					wsprintf(cTxt, "(!) New guild(%s) creation Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
					PutLogList(cTxt);
					break;
			}

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_CREATENEWGUILD;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = wResult;


			iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					DeleteClient(i, true, true);
					return;
			}

			return;
		}


	wsprintf(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
	PutLogList(cTxt);
}

void CGame::RequestCreateNewGuildHandler(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cGuildName[21], cTxt[120], cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	SYSTEMTIME SysTime;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_bIsCrusadeMode == true) return;

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	cp += 10;
	cp += 10;
	cp += 10;

	std::memset(cGuildName, 0, sizeof (cGuildName));
	memcpy(cGuildName, cp, 20);
	cp += 20;

	if (m_pClientList[iClientH]->m_iGuildRank != -1) {

		wsprintf(cTxt, "(!)Cannot create guild! Already guild member.: CharName(%s)", m_pClientList[iClientH]->m_cCharName);
		PutLogList(cTxt);
	} else {
		if ((m_pClientList[iClientH]->m_iLevel < 20) || (m_pClientList[iClientH]->m_iCharisma < 20) ||
				  (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) ||
				  (memcmp(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10) != 0)) { // v1.4

			std::memset(cData, 0, sizeof (cData));

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_CREATENEWGUILD;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_REJECT;


			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					DeleteClient(iClientH, true, true);
					return;
			}
		} else {

			std::memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof (m_pClientList[iClientH]->m_cGuildName));
			strcpy(m_pClientList[iClientH]->m_cGuildName, cGuildName);

			std::memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof (m_pClientList[iClientH]->m_cLocation));
			strcpy(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName);

			GetLocalTime(&SysTime);
			m_pClientList[iClientH]->m_iGuildGUID = (int) (SysTime.wYear + SysTime.wMonth + SysTime.wDay + SysTime.wHour + SysTime.wMinute + timeGetTime());


			bSendMsgToLS(MSGID_REQUEST_CREATENEWGUILD, iClientH);
		}
	}
}

void CGame::RequestDisbandGuildHandler(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cGuildName[21], cTxt[120];

	if (m_bIsCrusadeMode == true) return;

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cGuildName, 0, sizeof (cGuildName));

	cp += 10;
	cp += 10;
	cp += 10;

	memcpy(cGuildName, cp, 20);
	cp += 20;

	if ((m_pClientList[iClientH]->m_iGuildRank != 0) || (memcmp(m_pClientList[iClientH]->m_cGuildName, cGuildName, 20) != 0)) {

		wsprintf(cTxt, "(!)Cannot Disband guild! Not guildmaster.: CharName(%s)", m_pClientList[iClientH]->m_cCharName);
		PutLogList(cTxt);
	} else {

		bSendMsgToLS(MSGID_REQUEST_DISBANDGUILD, iClientH);
	}
}

void CGame::ResponseDisbandGuildHandler(char * pData, uint32_t /*dwMsgSize*/) {
	register int i;
	uint16_t * wp, wResult;
	uint32_t * dwp;
	char * cp, cCharName[11], cData[100], cTxt[120];
	int iRet;

	// �α� �����κ��� ��� �ػ� ��û�� ���� ���䵥���Ͱ� �����ߴ�.
	std::memset(cCharName, 0, sizeof (cCharName));
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	cp += 10;


	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)) {

			wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
			switch (*wp) {
				case DEF_LOGRESMSGTYPE_CONFIRM:
					// Ŭ���̾�Ʈ�� ��� �ػ� �䱸�� �����Ͽ���.
					wResult = DEF_MSGTYPE_CONFIRM;
					wsprintf(cTxt, "(!) Disband guild(%s) success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
					PutLogList(cTxt);

					// �������� ����鿡�� ��尡 �ػ�Ǿ����� �˸��� �޽����� ����Ѵ�.
					SendGuildMsg(i, DEF_NOTIFY_GUILDDISBANDED, 0, 0, nullptr);

					// ����̸� Ŭ����
					std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
					memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
					m_pClientList[i]->m_iGuildRank = -1; // ��� ��ũ�� -1. ������ �ƴϴ�.
					m_pClientList[i]->m_iGuildGUID = -1;
					break;

				case DEF_LOGRESMSGTYPE_REJECT:
					// Ŭ���̾�Ʈ�� ��� �ػ� �䱸�� �����Ͽ���.
					wResult = DEF_MSGTYPE_REJECT;
					wsprintf(cTxt, "(!) Disband guild(%s) Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
					PutLogList(cTxt);
					break;
			}

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_DISBANDGUILD;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = wResult;

			// ��� �ػ� �䱸 ���� �޽����� Ŭ���̾�Ʈ���� ���
			iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
					DeleteClient(i, true, true);
					return;
			}
			return;
		}


	wsprintf(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
	PutLogList(cTxt);
}

// 05/29/2004 - Hypnotoad - Purchase Dicount updated to take charisma into consideration

void CGame::RequestPurchaseItemHandler(int iClientH, char * pItemName, int iNum) {
	class CItem * pItem;
	char * cp, cItemName[21], cData[100];
	short * sp;
	uint32_t * dwp, dwGoldCount, dwItemCount;
	uint16_t * wp, wTempPrice;
	int i, iRet, iEraseReq;
	int iCost2;
	int iCost, iDiscountRatio, iDiscountCost;
	double dTmp1, dTmp2, dTmp3;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	//if ( (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) &&
	//	 (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, m_pClientList[iClientH]->m_cLocation, 10) != 0) ) return;

	if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) {
		if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) {
			if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0) ||
					  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "arefarm", 7) == 0)) {

			} else return;
		}

		if (memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0) {
			if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0) ||
					  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvfarm", 7) == 0)) {

			} else return;
		}
	}



	std::memset(cData, 0, sizeof (cData));
	std::memset(cItemName, 0, sizeof (cItemName));

	// New 18/05/2004
	if (m_pClientList[iClientH]->m_pIsProcessingAllowed == false) return;


	if (memcmp(pItemName, "10Arrows", 8) == 0) {
		strcpy(cItemName, "Arrow");
		dwItemCount = 10;
	} else if (memcmp(pItemName, "100Arrows", 9) == 0) {
		strcpy(cItemName, "Arrow");
		dwItemCount = 100;
	} else {
		memcpy(cItemName, pItemName, 20);
		dwItemCount = 1;
	}

	for (i = 1; i <= iNum; i++) {

		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {

			delete pItem;
		} else {

			if (pItem->m_bIsForSale == false) {

				delete pItem;
				return;
			}

			pItem->m_dwCount = dwItemCount;
			if (m_pClientList[iClientH]->m_cSide == m_iLastCrusadeWinner) {
				iCost = (int) ((float) (pItem->m_wPrice) * 0.9f + 0.5f);
				iCost = iCost * pItem->m_dwCount;
				iCost2 = pItem->m_wPrice * pItem->m_dwCount;
			} else {
				iCost2 = iCost = ((pItem->m_wPrice * pItem->m_dwCount)*2);
			}


			iCost = pItem->m_wPrice * pItem->m_dwCount;

			/* v1.3 <- ÀÌ°Ç _bAddClientItemList¿¡¼­ Ã¼Å©ÇÏ´Â ±â´ÉÀÌ¹Ç·Î µÎ¹øÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
			if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW))
				iItemWeight = (pItem->m_wWeight * pItem->m_dwCount);
			else iItemWeight = pItem->m_wWeight;

			if ( (iItemWeight + m_pClientList[iClientH]->m_iCurWeightLoad) > _iCalcMaxLoad(iClientH)) {
				

				
				dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
			 *dwp = MSGID_NOTIFY;
				wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
			 *wp  = DEF_NOTIFY_CANNOTCARRYMOREITEM;

				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					
					DeleteClient(iClientH, true, true);
					break;
				}

				delete pItem;
				return;
			}
			 */


			dwGoldCount = dwGetItemCount(iClientH, "Gold");



			iDiscountRatio = ((m_pClientList[iClientH]->m_iCharisma - 10) / 4);

			// 2.03 Discount Method

			// iDiscountRatio = (m_pClientList[iClientH]->m_iCharisma / 4) -1;
			// if (iDiscountRatio == 0) iDiscountRatio = 1;

			dTmp1 = (double) (iDiscountRatio);
			dTmp2 = dTmp1 / 100.0f;
			dTmp1 = (double) iCost;
			dTmp3 = dTmp1 * dTmp2;
			iDiscountCost = (int) dTmp3;

			if (iDiscountCost >= (iCost / 2)) iDiscountCost = (iCost / 2) - 1;
			if ((iCost - iDiscountCost) <= (iCost2 / 2)) {
				iDiscountCost = iCost - (iCost2 / 2) + 1;
			}
			if (dwGoldCount < (uint32_t) (iCost - iDiscountCost)) {

				delete pItem;

				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_NOTENOUGHGOLD;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = -1;
				cp++;

				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 7);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:

						DeleteClient(iClientH, true, true);
						return;
				}
				return;
			}

			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {

				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;


				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMPURCHASED;

				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

				*cp = 1;
				cp++;

				memcpy(cp, pItem->m_cName, 20);
				cp += 20;

				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;

				*cp = pItem->m_cItemType;
				cp++;

				*cp = pItem->m_cEquipPos;
				cp++;

				*cp = (char) 0;
				cp++;

				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;

				*cp = pItem->m_cGenderLimit;
				cp++;

				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;

				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;

				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;

				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;

				*cp = pItem->m_cItemColor;
				cp++;

				wp = (uint16_t *) cp;
				*wp = (iCost - iDiscountCost);
				wTempPrice = (iCost - iDiscountCost);
				cp += 2;

				if (iEraseReq == 1) delete pItem;


				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 48);


				SetItemCount(iClientH, "Gold", dwGoldCount - wTempPrice);

				iCalcTotalWeight(iClientH);


				m_stCityStatus[m_pClientList[iClientH]->m_cSide].iFunds += wTempPrice;

				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:

						DeleteClient(iClientH, true, true);
						return;
				}
			} else {

				delete pItem;


				iCalcTotalWeight(iClientH);

				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:

						DeleteClient(iClientH, true, true);
						return;
				}
			}
		}
	}
}

void CGame::GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName) {
	int iEraseReq;
	short * sp, sOwnerH;
	char * cp, cOwnerType, cData[100], cCharName[21];
	uint32_t * dwp;
	uint16_t * wp;
	class CItem * pItem;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (iAmount <= 0) return;


	if (memcmp(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) {
		PutLogList("GiveItemHandler - Not matching Item name");
		return;
	}

	std::memset(cCharName, 0, sizeof (cCharName));

	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount > (uint32_t) iAmount)) {


		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName) == false) {

			delete pItem;
			return;
		} else {
			pItem->m_dwCount = iAmount;
		}


		m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount -= iAmount;



		SetItemCount(iClientH, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount);


		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);


		if (wObjectID != 0) {
			if (wObjectID < 10000) {

				if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS)) {
					if (m_pClientList[wObjectID] != nullptr) {
						if ((uint16_t) sOwnerH != wObjectID) sOwnerH = 0;
					}
				}
			} else {
				// NPC
				if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS)) {
					if (m_pNpcList[wObjectID - 10000] != nullptr) {
						if ((uint16_t) sOwnerH != (wObjectID - 10000)) sOwnerH = 0;
					}
				}
			}
		}

		if (sOwnerH == 0) {


			m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

			// v1.411
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);


			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		} else {

			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {

				memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);

				if (sOwnerH == iClientH) {

					delete pItem;
					return;
				}

				if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == true) {

					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_ITEMOBTAINED;

					cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


					*cp = 1;
					cp++;

					memcpy(cp, pItem->m_cName, 20);
					cp += 20;

					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwCount;
					cp += 4;

					*cp = pItem->m_cItemType;
					cp++;

					*cp = pItem->m_cEquipPos;
					cp++;

					*cp = (char) 0;
					cp++;

					sp = (short *) cp;
					*sp = pItem->m_sLevelLimit;
					cp += 2;

					*cp = pItem->m_cGenderLimit;
					cp++;

					wp = (uint16_t *) cp;
					*wp = pItem->m_wCurLifeSpan;
					cp += 2;

					wp = (uint16_t *) cp;
					*wp = pItem->m_wWeight;
					cp += 2;

					sp = (short *) cp;
					*sp = pItem->m_sSprite;
					cp += 2;

					sp = (short *) cp;
					*sp = pItem->m_sSpriteFrame;
					cp += 2;

					*cp = pItem->m_cItemColor;
					cp++;

					*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
					cp++;

					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwAttribute;
					cp += 4;
					/*
					 *cp = (char)(pItem->m_dwAttribute & 0x00000001); 
					cp++;
					 */

					if (iEraseReq == 1) delete pItem;


					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 53);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:

							DeleteClient(sOwnerH, true, true);
							break;
					}


					SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0, cCharName);
				} else {


					m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
							  m_pClientList[iClientH]->m_sY,
							  pItem);

					// v1.411
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);


					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color


					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:

							DeleteClient(sOwnerH, true, true);
							break;
					}


					SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTGIVEITEM, sItemIndex, iAmount, 0, cCharName);
				}

			} else {

				memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);

				if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0) {

					if (bSetItemToBankItem(iClientH, pItem) == false) {

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTITEMTOBANK, 0, 0, 0, nullptr);


						m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

						// v1.411
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);


						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
								  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
								  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
					}
				} else {

					m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

					// v1.411
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);


					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
				}
			}
		}
	} else {



		ReleaseItemHandler(iClientH, sItemIndex, true);


		if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)
			m_pClientList[iClientH]->m_cArrowIndex = -1;


		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);


		if (wObjectID != 0) {
			if (wObjectID < 10000) {

				if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS)) {
					if (m_pClientList[wObjectID] != nullptr) {
						if ((uint16_t) sOwnerH != wObjectID) sOwnerH = 0;
					}
				}
			} else {
				// NPC
				if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS)) {
					if (m_pNpcList[wObjectID - 10000] != nullptr) {
						if ((uint16_t) sOwnerH != (wObjectID - 10000)) sOwnerH = 0;
					}
				}
			}
		}

		if (sOwnerH == 0) {


			m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
					  m_pClientList[iClientH]->m_sY,
					  &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			// v1.411
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color


			SendNotifyMsg(0, iClientH, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, nullptr);
		} else {


			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
				// ĳ���Ϳ��� �������� �־��.
				memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);
				pItem = &*m_pClientList[iClientH]->m_pItemList[sItemIndex];

				// v2.03 ũ�缼�̵� ����ΰ�쵵 ��� ������ �����ϴ�.

				// v2.17 2002-7-31 �������� ������ ������ȣ�� ���� �� �ְ� �Ѵ�.
				if (pItem->m_sIDnum == 88) {

					// �÷��̾� iClientH �� sOwnerH���� ��尡�� ��û���� �־��. ����
					// sOwnerH�� ��帶���Ͷ�� �������� �޴� ���� �ƴ϶� Ȯ���� �� �־�� �Ѵ�.
					// v2.17 2002-7-31 �������� ������ ������ȣ�� ���� �� �ְ� �Ѵ�.
					if ((m_pClientList[iClientH]->m_iGuildRank == -1) &&
							  (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) &&
							  (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[sOwnerH]->m_cLocation, 10) == 0) &&
							  (m_pClientList[sOwnerH]->m_iGuildRank == 0)) {
						// ��� �������̴�.	��� �����Ϳ��Դ� ����Ȯ�� ��û �޽����� ����Ѵ�.
						SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION, 0, 0, 0, nullptr);
						// ���� �������� �� ���ο��� �������� �־����Ƿ� ����Ʈ���� �����Ұ��� �뺸�Ѵ�.
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, 0, cCharName);

						_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int) - 1, pItem);

						goto REMOVE_ITEM_PROCEDURE;
					}
				}

				// v2.17 2002-7-31 �������� ������ ������ȣ�� ���� �� �ְ� �Ѵ�.
				if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 89)) {

					// �÷��̾� iClientH �� sOwnerH���� ���Ż�� ��û���� �־��. ����
					// sOwnerH�� ��帶�����̰� iClientH�� ��� �̸��� ���� iClientH�� �����̶��
					// �������� �޴� ���� �ƴ϶� Ȯ���� �� �־�� �Ѵ�.
					if ((memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
							  (m_pClientList[iClientH]->m_iGuildRank != -1) &&
							  (m_pClientList[sOwnerH]->m_iGuildRank == 0)) {
						// ��� �������̴�.	��� �����Ϳ��Դ� Ż��Ȯ�� ��û �޽����� ����Ѵ�.
						SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION, 0, 0, 0, nullptr);
						// ���� �������� �� ���ο��� �������� �־����Ƿ� ����Ʈ���� �����Ұ��� �뺸�Ѵ�.
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, 0, cCharName);

						_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int) - 1, pItem);

						goto REMOVE_ITEM_PROCEDURE;
					}
				}


				if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == true) {


					_bItemLog(DEF_ITEMLOG_GIVE, iClientH, sOwnerH, pItem);


					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_ITEMOBTAINED;

					cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


					*cp = 1;
					cp++;

					memcpy(cp, pItem->m_cName, 20);
					cp += 20;

					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwCount;
					cp += 4;

					*cp = pItem->m_cItemType;
					cp++;

					*cp = pItem->m_cEquipPos;
					cp++;

					*cp = (char) 0;
					cp++;

					sp = (short *) cp;
					*sp = pItem->m_sLevelLimit;
					cp += 2;

					*cp = pItem->m_cGenderLimit;
					cp++;

					wp = (uint16_t *) cp;
					*wp = pItem->m_wCurLifeSpan;
					cp += 2;

					wp = (uint16_t *) cp;
					*wp = pItem->m_wWeight;
					cp += 2;

					sp = (short *) cp;
					*sp = pItem->m_sSprite;
					cp += 2;

					sp = (short *) cp;
					*sp = pItem->m_sSpriteFrame;
					cp += 2;

					*cp = pItem->m_cItemColor;
					cp++;

					*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
					cp++;

					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwAttribute;
					cp += 4;
					/*
					 *cp = (char)(pItem->m_dwAttribute & 0x00000001); 
					cp++;
					 */

					if (iEraseReq == 1) delete pItem;


					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 53);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:

							DeleteClient(sOwnerH, true, true);
							break;
					}
				} else {


					m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
							  m_pClientList[iClientH]->m_sY,
							  &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);

					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
							  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
							  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color


					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:

							DeleteClient(sOwnerH, true, true);
							break;
					}


					std::memset(cCharName, 0, sizeof (cCharName));
				}
			} else {

				memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);

				if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0) {

					if (bSetItemToBankItem(iClientH, sItemIndex) == false) {

						SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTITEMTOBANK, 0, 0, 0, nullptr);


						m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
								  m_pClientList[iClientH]->m_sY,
								  &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


						_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
								  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
					}
				} else if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Kennedy", 7) == 0) {

					if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 89)) {

						if ((m_pClientList[iClientH]->m_iGuildRank != 0) && (m_pClientList[iClientH]->m_iGuildRank != -1)) {

							SendNotifyMsg(iClientH, iClientH, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, nullptr);


							std::memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof (m_pClientList[iClientH]->m_cGuildName));
							memcpy(m_pClientList[iClientH]->m_cGuildName, "NONE", 4);
							m_pClientList[iClientH]->m_iGuildRank = -1;
							m_pClientList[iClientH]->m_iGuildGUID = -1;


							SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);


							m_pClientList[iClientH]->m_iExp -= 300;
							if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
						}


						m_pClientList[iClientH]->m_pItemList[sItemIndex].reset();
					} else {

						m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
								  m_pClientList[iClientH]->m_sY,
								  &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


						_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
								  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color


						std::memset(cCharName, 0, sizeof (cCharName));

					}
				} else {


					m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->bSetItem(m_pClientList[iClientH]->m_sX,
							  m_pClientList[iClientH]->m_sY,
							  &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);


					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
							  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
							  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color


					std::memset(cCharName, 0, sizeof (cCharName));
				}
			}


			SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, cCharName);
		}

REMOVE_ITEM_PROCEDURE:
		;


		if (m_pClientList[iClientH] == nullptr) return;


		m_pClientList[iClientH]->m_pItemList[sItemIndex] = nullptr;
		m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;


		m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	}


	iCalcTotalWeight(iClientH);
}

void CGame::SendNotifyMsg(int iFromH, int iToH, uint16_t wMsgType, uint32_t sV1, uint32_t sV2, uint32_t sV3, const char * pString, uint32_t sV4, uint32_t sV5, uint32_t sV6, uint32_t sV7, uint32_t sV8, uint32_t sV9, char * pString2) {
	char cData[1000];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	short * sp;
	int * ip, iRet, i;

	if (m_pClientList[iToH] == nullptr) return;

	std::memset(cData, 0, sizeof (cData));

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_NOTIFY;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;

	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


	switch (wMsgType) {
		case DEF_NOTIFY_HELDENIANCOUNT:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV3;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV4;
			cp += 2;

			cp += 14;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 14);

			break;

		case DEF_NOTIFY_NOMOREAGRICULTURE:
		case DEF_NOTIFY_AGRICULTURESKILLLIMIT:
		case DEF_NOTIFY_AGRICULTURENOAREA:
		case DEF_NOTIFY_HAPPYHOURSTAR:
		case DEF_NOTIFY_HAPPYHOUREND:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

			// Crafting
		case DEF_NOTIFY_CRAFTING_FAIL: //reversed by Snoopy: 0x0BF1:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

			// New 18/05/2004
		case DEF_NOTIFY_SPAWNEVENT:
			*cp = (char) sV3;
			cp++;

			sp = (short *) cp;
			*cp = sV1;
			cp += 2;

			sp = (short *) cp;
			*cp = sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 11);
			break;

		case DEF_NOTIFY_QUESTCOUNTER:
			ip = (int *) cp;
			*ip = sV1;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_REQRANGO: // Morla2.2 - Notify Rango
			ip = (int *) cp;
			*ip = sV1;
			cp += 4;

			ip = (int *) cp;
			*ip = sV2;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_APOCGATECLOSE:
		case DEF_NOTIFY_APOCGATEOPEN:
			ip = (int *) cp;
			*ip = sV1;
			cp += 4;

			ip = (int *) cp;
			*ip = sV2;
			cp += 4;

			memcpy(cp, pString, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_ABADDONKILLED:
			memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_APOCFORCERECALLPLAYERS:
		case DEF_NOTIFY_APOCGATESTARTMSG:
		case DEF_NOTIFY_APOCGATEENDMSG:
		case DEF_NOTIFY_NORECALL:
			//50Cent - Capture The Flag
		case DEF_NOTIFY_CAPTURETHEFLAGSTART:
		case DEF_NOTIFY_ARESDENCAPTUREDELVINEFLAG:
		case DEF_NOTIFY_ELVINECAPTUREDARESDENFLAG:
		case DEF_NOTIFY_ELVINEFLAGBACKTOCH:
		case DEF_NOTIFY_ARESDENFLAGBACKTOCH:
		case DEF_NOTIFY_ELVINEWINSROUND:
		case DEF_NOTIFY_ARESDENWINSROUND:
		case DEF_NOTIFY_ELVINEWINCTF:
		case DEF_NOTIFY_ARESDENWINCTF:
		case DEF_NOTIFY_TIECTF:

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_FORCERECALLTIME:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

			// New 16/05/2004
			//0xB4E2, 0xBEB
		case DEF_NOTIFY_MONSTERCOUNT:
		case DEF_NOTIFY_SLATE_STATUS:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

			//0x0BE5, 0x0BE7, 0x0BE8, 0x0BEA
		case DEF_NOTIFY_0BE8:
		case DEF_NOTIFY_HELDENIANTELEPORT:
		case DEF_NOTIFY_HELDENIANEND:
		case DEF_NOTIFY_RESURRECTPLAYER:
		case DEF_NOTIFY_SLATE_EXP:
		case DEF_NOTIFY_SLATE_MANA:
		case DEF_NOTIFY_SLATE_INVINCIBLE:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SLATE_CREATEFAIL:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SLATE_CREATESUCCESS:
			dwp = (uint32_t *) cp;
			*dwp = sV1;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

			// New 07/05/2004
			// Party Notify Msg's
		case DEF_NOTIFY_PARTY:

			switch (sV1) {
				case 4:
				case 6:
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV1;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV2;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV3;
					cp += 2;
					memcpy(cp, pString, 10);
					cp += 10;
					iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 12 + 10);
					SendNotifyMsg(0, iToH, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
					break;

				case 5:
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV1;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV2;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV3;
					cp += 2;
					memcpy(cp, pString, sV3 * 11);
					cp += sV3 * 11;
					iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 12 + sV3 * 11);
					SendNotifyMsg(0, iToH, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
					break;

				default:
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV1;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV2;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV3;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV4;
					cp += 2;
					iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 14);
					SendNotifyMsg(0, iToH, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
					break;
			}
			break;

		case DEF_NOTIFY_REQGUILDNAMEANSWER:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 30);
			break;

			// New 06/05/2004
			// Upgrade Notify Msg's
		case DEF_NOTIFY_ITEMUPGRADEFAIL:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_ITEMATTRIBUTECHANGE:
		case DEF_NOTIFY_GIZONITEMUPGRADELEFT:
			sp = (short *) cp;
			*sp = sV1;
			cp += 2;

			dwp = (uint32_t *) cp;
			*dwp = sV2;
			cp += 4;

			dwp = (uint32_t *) cp;
			*dwp = sV3;
			cp += 4;

			dwp = (uint32_t *) cp;
			*dwp = sV4;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 20);
			break;

		case DEF_NOTIFY_UPGRADEHEROCAPE:
		case DEF_NOTIFY_GIZONITEMCANGE:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			*cp = (char) sV2;
			cp++;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV4;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV5;
			cp += 2;

			*cp = (char) sV6;
			cp++;

			*cp = (char) sV7;
			cp++;

			dwp = (uint32_t *) cp;
			*dwp = sV8;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 41);
			break;

			// 2.06 - by KLKS
		case DEF_NOTIFY_CHANGEPLAYMODE:
			memcpy(cp, pString, 10);
			cp += 10;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 16);
			break;
			//

		case DEF_NOTIFY_TCLOC:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV4;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV5;
			cp += 2;

			memcpy(cp, pString2, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 34);
			break;

			//New 11/05/2004
		case DEF_NOTIFY_GRANDMAGICRESULT:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV3;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV4;
			cp += 2;

			if (sV9 > 0) {
				memcpy(cp, pString2, (sV9 * 2) + 2);
				cp += (sV9 * 2) + 2;
			} else {
				sp = (short *) cp;
				*sp = 0;
				cp += 2;
			}
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, (sV9 * 2) + 26);
			break;

		case DEF_NOTIFY_MAPSTATUSNEXT:
			memcpy(cp, pString, sV1);
			cp += sV1;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6 + sV1);
			break;

		case DEF_NOTIFY_MAPSTATUSLAST:
			memcpy(cp, pString, sV1);
			cp += sV1;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6 + sV1);
			break;

		case DEF_NOTIFY_LOCKEDMAP:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 18);
			break;

		case DEF_NOTIFY_BUILDITEMSUCCESS:
		case DEF_NOTIFY_BUILDITEMFAIL:

			if (sV1 >= 0) {
				sp = (short *) cp;
				*sp = (short) sV1;
				cp += 2;
			} else {
				sp = (short *) cp;
				*sp = (short) sV1 + 10000;
				cp += 2;
			}

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_HELP:
		case DEF_NOTIFY_QUESTREWARD:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV3;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 38);
			break;

		case DEF_NOTIFY_CANNOTCONSTRUCT:
		case DEF_NOTIFY_METEORSTRIKECOMING:
		case DEF_NOTIFY_METEORSTRIKEHIT:
		case DEF_NOTIFY_HELPFAILED:
		case DEF_NOTIFY_SPECIALABILITYENABLED:
		case DEF_NOTIFY_FORCEDISCONN:
		case DEF_NOTIFY_OBSERVERMODE:
		case DEF_NOTIFY_QUESTCOMPLETED:
		case DEF_NOTIFY_QUESTABORTED:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_QUESTCONTENTS:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV4;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV5;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV9;
			cp += 2;

			if (pString2 != nullptr) memcpy(cp, pString2, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 44);
			break;

		case DEF_NOTIFY_ENERGYSPHERECREATED:
		case DEF_NOTIFY_ITEMCOLORCHANGE:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_NOMORECRUSADESTRUCTURE:
		case DEF_NOTIFY_EXCHANGEITEMCOMPLETE:
		case DEF_NOTIFY_CANCELEXCHANGEITEM:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SETEXCHANGEITEM:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			*cp = (char) sV5;
			cp++;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			// v1.42
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV9;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 57);
			break;

		case DEF_NOTIFY_OPENEXCHANGEWINDOW:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			*cp = (char) sV5;
			cp++;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			// v1.42
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV9;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 57);
			break;

		case DEF_NOTIFY_NOTFLAGSPOT:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_ITEMPOSLIST:
			for (i = 0; i < DEF_MAXITEMS; i++) {
				sp = (short *) cp;
				*sp = (short) m_pClientList[iToH]->m_ItemPosList[i].x;
				cp += 2;
				sp = (short *) cp;
				*sp = (short) m_pClientList[iToH]->m_ItemPosList[i].y;
				cp += 2;
			}
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6 + DEF_MAXITEMS * 4);
			break;

		case DEF_NOTIFY_ENEMYKILLS:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_CRUSADE:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			ip = (int *) cp;
			*ip = (int) sV3;
			cp += 4;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 22);
			break;

		case DEF_NOTIFY_CONSTRUCTIONPOINT:
		case DEF_NOTIFY_SPECIALABILITYSTATUS:
		case DEF_NOTIFY_DAMAGEMOVE:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 12);
			break;

		case DEF_NOTIFY_DOWNSKILLINDEXSET:
		case DEF_NOTIFY_RESPONSE_CREATENEWPARTY:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_ADMINIFO:
			switch (sV1) {
				case 1:

					ip = (int *) cp;
					*ip = m_pNpcList[sV2]->m_iHP;
					cp += 4;

					ip = (int *) cp;
					*ip = m_pNpcList[sV2]->m_iDefenseRatio;
					cp += 4;

					ip = (int *) cp;
					*ip = m_pNpcList[sV2]->m_bIsSummoned;
					cp += 4;

					ip = (int *) cp;
					*ip = m_pNpcList[sV2]->m_cActionLimit;
					cp += 4;

					ip = (int *) cp;
					*ip = m_pNpcList[sV2]->m_iHitDice;
					cp += 4;

					dwp = (uint32_t *) cp; // v1.4
					*dwp = m_pNpcList[sV2]->m_dwDeadTime;
					cp += 4;

					dwp = (uint32_t *) cp;
					*dwp = m_pNpcList[sV2]->m_dwRegenTime;
					cp += 4;

					ip = (int *) cp;
					*ip = (int) m_pNpcList[sV2]->m_bIsKilled;
					cp += 4;

					iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 26 + 12);
					break;
			}
			break;

		case DEF_NOTIFY_HELDENIANSTART:
		case DEF_NOTIFY_NPCTALK:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV4;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV5;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV9;
			cp += 2;

			if (pString != nullptr) memcpy(cp, pString, 20);
			cp += 20;

			if (pString2 != nullptr) memcpy(cp, pString2, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 64);
			break;

		case DEF_NOTIFY_CRAFTING_SUCCESS: //reversed by Snoopy: 0x0BF0
		case DEF_NOTIFY_PORTIONSUCCESS:
		case DEF_NOTIFY_LOWPORTIONSKILL:
		case DEF_NOTIFY_PORTIONFAIL:
		case DEF_NOTIFY_NOMATCHINGPORTION:

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SUPERATTACKLEFT:
			sp = (short *) cp;
			*sp = m_pClientList[iToH]->m_iSuperAttackLeft;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_SAFEATTACKMODE:
			*cp = m_pClientList[iToH]->m_bIsSafeAttackMode;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_QUERY_JOINPARTY:
		case DEF_NOTIFY_IPACCOUNTINFO:
			strcpy(cp, pString);
			cp += strlen(pString);

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6 + strlen(pString) + 1);
			break;

		case DEF_NOTIFY_REWARDGOLD:
			dwp = (uint32_t *) cp;
			*dwp = m_pClientList[iToH]->m_iRewardGold;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_SERVERSHUTDOWN:
			*cp = (char) sV1;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_GLOBALATTACKMODE:
		case DEF_NOTIFY_WHETHERCHANGE:
			*cp = (char) sV1;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_FISHCANCELED:
		case DEF_NOTIFY_FISHSUCCESS:
		case DEF_NOTIFY_FISHFAIL:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_DEBUGMSG:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_FISHCHANCE:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_ENERGYSPHEREGOALIN:
		case DEF_NOTIFY_EVENTFISHMODE:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV3;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 32);
			break;

		case DEF_NOTIFY_NOTICEMSG:
			memcpy(cp, pString, strlen(pString));
			cp += strlen(pString);

			*cp = 0;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, strlen(pString) + 7);
			break;

		case DEF_NOTIFY_CANNOTRATING:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_RATINGPLAYER:
			*cp = (char) sV1;
			cp++;

			memcpy(cp, pString, 10);
			cp += 10;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iRating;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 22);
			break;

		case DEF_NOTIFY_ADMINUSERLEVELLOW:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_PLAYERSHUTUP:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 19);
			break;

		case DEF_NOTIFY_TIMECHANGE:
			*cp = (char) sV1;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_TOBERECALLED:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_HUNGER:
			*cp = (char) sV1;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_PLAYERPROFILE:
			if (strlen(pString) > 100) {
				memcpy(cp, pString, 100);
				cp += 100;
			} else {
				memcpy(cp, pString, strlen(pString));
				cp += strlen(pString);
			}
			*cp = 0;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 7 + strlen(pString));
			break;

			// New 10/05/2004 Changed
		case DEF_NOTIFY_WHISPERMODEON:
		case DEF_NOTIFY_WHISPERMODEOFF:
		case DEF_NOTIFY_PLAYERNOTONGAME:
			memcpy(cp, pString, 10);
			cp += 10;

			memcpy(cp, "          ", 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 27);
			break;

			// New 15/05/2004 Changed
		case DEF_NOTIFY_PLAYERONGAME:
			memcpy(cp, pString, 10);
			cp += 10;

			if (pString[0] != 0) {
				memcpy(cp, pString2, 14);
				cp += 14;
			}

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 31);
			break;

			// New 06/05/2004
		case DEF_NOTIFY_ITEMSOLD:
		case DEF_NOTIFY_ITEMREPAIRED:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV1;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 14);
			break;

			// New 06/05/2004
		case DEF_NOTIFY_REPAIRITEMPRICE:
		case DEF_NOTIFY_SELLITEMPRICE:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV1;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV3;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV4;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 42);
			break;

		case DEF_NOTIFY_CANNOTREPAIRITEM:
		case DEF_NOTIFY_CANNOTSELLITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 30);

			break;

		case DEF_NOTIFY_SHOWMAP:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_SKILLUSINGEND:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_TOTALUSERS:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) m_iTotalGameServerClients; //_iGetTotalClients();
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_MAGICEFFECTOFF:
		case DEF_NOTIFY_MAGICEFFECTON:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV3;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 16);
			break;

		case DEF_NOTIFY_CANNOTITEMTOBANK:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SERVERCHANGE:
			memcpy(cp, m_pClientList[iToH]->m_cMapName, 10);
			cp += 10;
			if (m_iGameServerMode == 1)
				if (memcmp(m_cLogServerAddr, m_cGameServerAddr, 15) == 0)
					memcpy(cp, m_cGameServerAddrExternal, 15);
				else
					memcpy(cp, m_cLogServerAddr, 15);
			else
				memcpy(cp, m_cLogServerAddr, 15);
			cp += 15;
			ip = (int *) cp;
			*ip = m_iLogServerPort;
			cp += 4;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 16 + 19);
			break;

		case DEF_NOTIFY_SKILL:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_SETITEMCOUNT:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;

			*cp = (char) sV3;
			cp++;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 13);
			break;

		case DEF_NOTIFY_ITEMDEPLETED_ERASEITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 12);
			break;

		case DEF_NOTIFY_DROPITEMFIN_ERASEITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 12);
			break;

		case DEF_NOTIFY_CANNOTGIVEITEM:
		case DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 32);
			break;

		case DEF_NOTIFY_GIVEITEMFIN_ERASEITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 32);
			break;

		case DEF_NOTIFY_ENEMYKILLREWARD:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iExp;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iEnemyKillCount;
			cp += 4;
			memcpy(cp, m_pClientList[sV1]->m_cCharName, 10);
			cp += 10;
			memcpy(cp, m_pClientList[sV1]->m_cGuildName, 20);
			cp += 20;
			sp = (short *) cp;
			*sp = (short) m_pClientList[sV1]->m_iGuildRank;
			cp += 2;
			sp = (short *) cp;
			*sp = (short) m_pClientList[iToH]->m_iWarContribution;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 48);
			break;

		case DEF_NOTIFY_PKCAPTURED:


			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;
			memcpy(cp, pString, 10);
			cp += 10;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iRewardGold;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = m_pClientList[iToH]->m_iExp;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 28);
			break;

		case DEF_NOTIFY_PKPENALTY:

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iExp;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iStr;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iVit;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iDex;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iInt;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iMag;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iCharisma;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iPKCount;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 38);
			break;

		case DEF_NOTIFY_TRAVELERLIMITEDLEVEL:
		case DEF_NOTIFY_LIMITEDLEVEL:

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iExp;
			cp += 4;
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_ITEMRELEASED:
		case DEF_NOTIFY_ITEMLIFESPANEND:

			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;
			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_KILLED:

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 26);
			break;

		case DEF_NOTIFY_EXP:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iExp;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iRating;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 14);
			break;

		case DEF_NOTIFY_HP: // Fixed Party Info by xXx// 50cent
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iHP;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iMP;
			cp += 4;

			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				iH = m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i];
				if (m_pClientList[iH] != nullptr) {
					SendNotifyMsg(0, iH, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 14);
			break;

		case DEF_NOTIFY_PARTYMEMBERSTATUS:// 50 cent
			for (i = 0; i < m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iTotalMembers; i++) {
				ip = (int *) cp;
				*ip = (uint32_t) m_pClientList[m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i]]->m_iHP;
				cp += 4;

				ip = (int *) cp;
				*ip = (uint32_t) iGetMaxHP(m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i]);
				cp += 4;

				ip = (int *) cp;
				*ip = (uint32_t) m_pClientList[m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i]]->m_iMP;
				cp += 4;

				ip = (int *) cp;
				*ip = (uint32_t) iGetMaxMP(m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i]);
				cp += 4;
				//screemin fix/update :D -  Poison mode
				ip = (int *) cp;
				*ip = (uint32_t) m_pClientList[m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i]]->m_bIsPoisoned;
				cp += 2;

			}
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6 + (m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iTotalMembers * 18));
			break;

		case DEF_NOTIFY_MP: // Fixed Party Info by xXx
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iMP;
			cp += 4;

			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				iH = m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i];
				if (m_pClientList[iH] != nullptr) {
					SendNotifyMsg(0, iH, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;



		case DEF_NOTIFY_SP: // Fixed Party Info by xXx
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iSP;
			cp += 4;

			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				iH = m_stPartyInfo[m_pClientList[iToH]->m_iPartyID].iIndex[i];
				if (m_pClientList[iH] != nullptr) {
					SendNotifyMsg(0, iH, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_CHARISMA:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) m_pClientList[iToH]->m_iCharisma;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;

			//MOG Fixes
		case DEF_NOTIFY_STATECHANGE_FAILED:
		case DEF_NOTIFY_SETTING_FAILED:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_STATECHANGE_SUCCESS:
		{
			int i;

			for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
				*cp = m_pClientList[iToH]->m_cMagicMastery[i];
				cp++;
			}

			for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
				*cp = m_pClientList[iToH]->m_cSkillMastery[i];
				cp++;
			}

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6 + DEF_MAXMAGICTYPE + DEF_MAXSKILLTYPE);
		}
			break;

		case DEF_NOTIFY_SETTING_SUCCESS:
		case DEF_NOTIFY_LEVELUP:
			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iLevel;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iStr;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iVit;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iDex;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iInt;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iMag;
			cp += 4;

			ip = (int *) cp;
			*ip = m_pClientList[iToH]->m_iCharisma;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 34);
			break;

			/*
		case DEF_NOTIFY_LEVELUP:
			ip  = (int *)cp;
			 *ip = m_pClientList[iToH]->m_iLevel;
			cp += 4;

			ip   = (int *)cp;
			 *ip  = m_pClientList[iToH]->m_iStr;
			cp  += 4;


			ip   = (int *)cp;
			 *ip  = m_pClientList[iToH]->m_iDex;
			cp  += 4;

			ip   = (int *)cp;
			 *ip  = m_pClientList[iToH]->m_iInt;
			cp  += 4;

			ip   = (int *)cp;
			 *ip  = m_pClientList[iToH]->m_iMag;
			cp  += 4;

			ip   = (int *)cp;
			 *ip  = m_pClientList[iToH]->m_iCharisma;
			cp  += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 34);
			break;
			 */
		case DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
		case DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION:
		case DEF_NOTIFY_CANNOTJOINMOREGUILDSMAN:

			memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 16);
			break;

		case DEF_COMMONTYPE_JOINGUILDAPPROVE:
			if (m_pClientList[iFromH] != nullptr)
				memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
			else memcpy(cp, "?", 1);
			cp += 20;

			sp = (short *) cp;
			*sp = DEF_GUILDSTARTRANK;
			cp += 2;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 28);
			break;

		case DEF_COMMONTYPE_JOINGUILDREJECT:
		case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
		case DEF_COMMONTYPE_DISMISSGUILDREJECT:
			if (m_pClientList[iFromH] != nullptr)
				memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
			else memcpy(cp, "?", 1);
			cp += 20;

			sp = (short *) cp;
			*sp = DEF_GUILDSTARTRANK;
			cp += 2;

			memcpy(cp, m_pClientList[iToH]->m_cLocation, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 38);
			break;

		case DEF_NOTIFY_GUILDDISBANDED:

			memcpy(cp, pString, 20);
			cp += 20;

			memcpy(cp, m_pClientList[iToH]->m_cLocation, 10);
			cp += 10;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 36);
			break;


		case DEF_NOTIFY_FIGHTZONERESERVE:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;

			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 10);
			break;


		case DEF_NOTIFY_NOGUILDMASTERLEVEL:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;


		case DEF_NOTIFY_CANNOTBANGUILDMAN:
			iRet = m_pClientList[iToH]->m_pXSock->iSendMsg(cData, 6);
			break;
	}

	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:


			//DeleteClient(iToH, true, true);
			return;
	}
}

void CGame::JoinGuildApproveHandler(int iClientH, char * pName) {
	register int i;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;




	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {

			if (memcmp(m_pClientList[i]->m_cLocation, m_pClientList[iClientH]->m_cLocation, 10) != 0) return;


			std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
			strcpy(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName);


			m_pClientList[i]->m_iGuildGUID = m_pClientList[iClientH]->m_iGuildGUID;


			std::memset(m_pClientList[i]->m_cLocation, 0, sizeof (m_pClientList[i]->m_cLocation));
			strcpy(m_pClientList[i]->m_cLocation, m_pClientList[iClientH]->m_cLocation);

			m_pClientList[i]->m_iGuildRank = DEF_GUILDSTARTRANK;


			SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, nullptr);


			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);


			SendGuildMsg(i, DEF_NOTIFY_NEWGUILDSMAN, 0, 0, nullptr);


			bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN, i);
			return;
		}


}

void CGame::JoinGuildRejectHandler(int iClientH, char * pName) {
	register int i;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;




	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {


			SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, nullptr);
			return;
		}


}

void CGame::DismissGuildApproveHandler(int iClientH, char * pName) {
	register int i;


	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {

			bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);
			//_bItemLog(DEF_ITEMLOG_BANGUILD,i,(char *)nullptr,nullptr) ;
			SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, nullptr);

			std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
			strcpy(m_pClientList[i]->m_cGuildName, "NONE");
			m_pClientList[i]->m_iGuildRank = -1;
			m_pClientList[i]->m_iGuildGUID = -1;

			SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, nullptr);

			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			return;
		}

}

void CGame::DismissGuildRejectHandler(int iClientH, char * pName) {
	register int i;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;




	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {


			SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, nullptr);
			return;
		}


}

uint32_t CGame::dwGetItemCount(int iClientH, const char * pName) {
	register int i;
	char cTmpName[21];

	if (m_pClientList[iClientH] == nullptr) return 0;

	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pName);

	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((m_pClientList[iClientH]->m_pItemList[i] != nullptr) && (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, cTmpName, 20) == 0)) {

			return m_pClientList[iClientH]->m_pItemList[i]->m_dwCount;
		}

	return 0;
}

int CGame::SetItemCount(int iClientH, const char * pItemName, uint32_t dwCount) {
	register int i;
	char cTmpName[21];
	uint16_t wWeight;

	if (m_pClientList[iClientH] == nullptr) return -1;

	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pItemName);

	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((m_pClientList[iClientH]->m_pItemList[i] != nullptr) && (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, cTmpName, 20) == 0)) {

			wWeight = iGetItemWeight(*m_pClientList[iClientH]->m_pItemList[i], 1); // m_pClientList[iClientH]->m_pItemList[i]->m_wWeight;


			if (dwCount == 0) {
				ItemDepleteHandler(iClientH, i, false, true);
			} else {

				m_pClientList[iClientH]->m_pItemList[i]->m_dwCount = dwCount;
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETITEMCOUNT, i, dwCount, (char) true, nullptr);
			}

			return wWeight;
		}

	return -1;
}

int CGame::SetItemCount(int iClientH, int iItemIndex, uint32_t dwCount) {
	uint16_t wWeight;

	if (m_pClientList[iClientH] == nullptr) return -1;
	if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == nullptr) return -1;

	wWeight = iGetItemWeight(*m_pClientList[iClientH]->m_pItemList[iItemIndex], 1); //m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wWeight;


	if (dwCount == 0) {
		ItemDepleteHandler(iClientH, iItemIndex, false, true);
	} else {

		m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount = dwCount;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETITEMCOUNT, iItemIndex, dwCount, (char) true, nullptr);
	}

	return wWeight;
}

void CGame::ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage) {
	char cAttackerName[21], cEKMsg[1000];
	short sAttackerWeapon;
	int i, iExH;
	bool bIsSAattacked = false;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;

	// �������� ��� �ð��� �Է��Ѵ�.
	// 2002-7-4 �������� ������ �ø� �� �ֵ���
	if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "fight", 5) == 0) {
		m_pClientList[iClientH]->m_dwFightzoneDeadTime = timeGetTime();
		wsprintf(G_cTxt, "Fightzone Dead Time: %d", m_pClientList[iClientH]->m_dwFightzoneDeadTime);
		PutLogList(G_cTxt);
	}

	m_pClientList[iClientH]->m_bIsKilled = true;
	// HP�� 0�̴�.
	m_pClientList[iClientH]->m_iHP = 0;

	// ���� ��ȯ ����� ��ȯ�� ����Ѵ�.
	if (m_pClientList[iClientH]->m_bIsExchangeMode == true) {
		iExH = m_pClientList[iClientH]->m_iExchangeH;
		_ClearExchangeStatus(iExH);
		_ClearExchangeStatus(iClientH);
	}


	RemoveFromTarget(iClientH, DEF_OWNERTYPE_PLAYER);

	std::memset(cAttackerName, 0, sizeof (cAttackerName));
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER_INDIRECT:
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[iAttackerH] != nullptr)
				memcpy(cAttackerName, m_pClientList[iAttackerH]->m_cCharName, 10);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[iAttackerH] != nullptr)
#ifdef DEF_LOCALNPCNAME     // v2.14 NPC �̸� �߹�ȭ�� ���� ����
				wsprintf(cAttackerName, "NPCNPCNPC@%d", m_pNpcList[iAttackerH]->m_sType);
#else
				memcpy(cAttackerName, m_pNpcList[iAttackerH]->m_cNpcName, 20);
#endif
			break;
		default:
			break;
	}

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_KILLED, 0, 0, 0, cAttackerName);
	// �ٸ� Ŭ���̾�Ʈ���� �״� ���� ���.
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		sAttackerWeapon = ((m_pClientList[iAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
	} else sAttackerWeapon = 1;
	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(12, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetDeadOwner(iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;
	if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != nullptr) &&
			  (m_bIsHeldenianMode == true) &&
			  (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == true)) {
		if (m_pClientList[iClientH]->m_cSide == 1) {
			m_iHeldenianAresdenDead++;
		} else if (m_pClientList[iClientH]->m_cSide == 2) {
			m_iHeldenianElvineDead++;
		}
		UpdateHeldenianStatus();
	}


	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		// v1.432
		// Ư�� �ɷ��� �ִ� ����� ����� ���ߴ�.
		switch (m_pClientList[iAttackerH]->m_iSpecialAbilityType) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				bIsSAattacked = true;
				break;
		}

		if (iAttackerH == iClientH) return; // �����̴�.

		if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) {
			// ����ڰ� �������̴�.
			if (m_pClientList[iClientH]->m_iPKCount == 0) {



				ApplyPKpenalty(iAttackerH, iClientH);
			} else {


				PK_KillRewardHandler(iAttackerH, iClientH);
			}
		} else {
			// ����ڰ� �����ڰ� �ƴ϶� �� ���� �Ҽ��̴�.
			if (m_pClientList[iClientH]->m_iGuildRank == -1) {


				if (memcmp(m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {
					// ����ڰ� �������̴�.
					if (m_pClientList[iClientH]->m_iPKCount == 0) {

						ApplyPKpenalty(iAttackerH, iClientH);
					} else {
						// �����ڴ� PK�� ��Ƶ� ������ ���� ���Ѵ�.

					}
				} else {

					if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iAttackerH]->m_cLocation, 10) == 0) {

						if (m_pClientList[iClientH]->m_iPKCount == 0) {
							// ����ڰ� ������ ����. PK�̴�.
							ApplyPKpenalty(iAttackerH, iClientH);
						} else {
							// �����ڸ� ��Ҵ�.
							PK_KillRewardHandler(iAttackerH, iClientH);
						}
					} else {
						// ����ڰ� �ٸ� ���� �Ҽ�. ������ �������
						EnemyKillRewardHandler(iAttackerH, iClientH);
					}
				}
			} else {
				// ����ڴ� �����̴�.

				if (memcmp(m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {
					// ����ڰ� �������̴�.
					if (m_pClientList[iClientH]->m_iPKCount == 0) {
						// ������ ������ �����ڰ� �׿���. �����ڴ� PK�� �ȴ�.
						ApplyPKpenalty(iAttackerH, iClientH);
					} else {
						// �����ڴ� PK�� ��Ƶ� ������ ���� ���Ѵ�.

					}
				} else {

					if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iAttackerH]->m_cLocation, 10) == 0) {

						if (m_pClientList[iClientH]->m_iPKCount == 0) {
							// ����ڰ� ������ ����. PK�̴�.
							ApplyPKpenalty(iAttackerH, iClientH);
						} else {
							// �����ڸ� ��Ҵ�.
							PK_KillRewardHandler(iAttackerH, iClientH);
						}
					} else {
						// ����ڰ� �ٸ� ���� �Ҽ�. ������ �������
						EnemyKillRewardHandler(iAttackerH, iClientH);
					}
				}
			}
		}


		if (m_pClientList[iClientH]->m_iPKCount == 0) {
			// Innocent
			if (memcmp(m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {
				//�����ڿ��� ��ݹ޾� �׾��.

				//m_pClientList[iClientH]->m_iExp -= iDice(1, 100);
				//if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;
				//SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
			} else {
				if (memcmp(m_pClientList[iAttackerH]->m_cLocation, m_pClientList[iClientH]->m_cLocation, 10) == 0) {


					//m_pClientList[iClientH]->m_iExp -= iDice(1, 100);
					//if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;
					//SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
				} else {

					ApplyCombatKilledPenalty(iClientH, 2, bIsSAattacked);
				}
			}
		} else if ((m_pClientList[iClientH]->m_iPKCount >= 1) && (m_pClientList[iClientH]->m_iPKCount <= 3)) {
			// Criminal
			ApplyCombatKilledPenalty(iClientH, 3, bIsSAattacked);
		} else if ((m_pClientList[iClientH]->m_iPKCount >= 4) && (m_pClientList[iClientH]->m_iPKCount <= 11)) {
			// Murderer
			ApplyCombatKilledPenalty(iClientH, 6, bIsSAattacked);
		} else if ((m_pClientList[iClientH]->m_iPKCount >= 12)) {
			// Slaughterer
			ApplyCombatKilledPenalty(iClientH, 12, bIsSAattacked);
		}
	} else if (cAttackerType == DEF_OWNERTYPE_NPC) {

		_bPKLog(DEF_PKLOG_BYNPC, iClientH, 0, cAttackerName);


		if (m_pClientList[iClientH]->m_iPKCount == 0) {
			// Innocent
			ApplyCombatKilledPenalty(iClientH, 1, bIsSAattacked);
		} else if ((m_pClientList[iClientH]->m_iPKCount >= 1) && (m_pClientList[iClientH]->m_iPKCount <= 3)) {
			// Criminal
			ApplyCombatKilledPenalty(iClientH, 3, bIsSAattacked);
		} else if ((m_pClientList[iClientH]->m_iPKCount >= 4) && (m_pClientList[iClientH]->m_iPKCount <= 11)) {
			// Murderer
			ApplyCombatKilledPenalty(iClientH, 6, bIsSAattacked);
		} else if ((m_pClientList[iClientH]->m_iPKCount >= 12)) {
			// Slaughterer
			ApplyCombatKilledPenalty(iClientH, 12, bIsSAattacked);
		}
		// ���� ����� NPC�� ����� ����Ʈ��� ���ְ�� �Ǽ� ����Ʈ �ΰ�
		if (m_pNpcList[iAttackerH]->m_iGuildGUID != 0) {

			if (m_pNpcList[iAttackerH]->m_cSide != m_pClientList[iClientH]->m_cSide) {
				// ����� ������ Ȥ�� ����Ʈ�� �� �÷��̾ �׿���. �ٷ� �뺸�Ѵ�.
				// ���� ������ ���ְ��� �ִٸ� ��ٷ� �뺸. ������ �ٸ� ������ �˷���.
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iGuildGUID == m_pNpcList[iAttackerH]->m_iGuildGUID) &&
							  (m_pClientList[i]->m_iCrusadeDuty == 3)) {
						m_pClientList[i]->m_iConstructionPoint += (m_pClientList[iClientH]->m_iLevel / 2);

						if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
							m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

						//testcode
						wsprintf(G_cTxt, "Enemy Player Killed by Npc! Construction +%d", (m_pClientList[iClientH]->m_iLevel / 2));
						PutLogList(G_cTxt);
						// ���ְ�� �ٷ� �뺸.
						SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, nullptr);
						return;
					}
			}
		}
	} else if (cAttackerType == DEF_OWNERTYPE_PLAYER_INDIRECT) {
		_bPKLog(DEF_PKLOG_BYOTHER, iClientH, 0, nullptr);
		// �÷��̾ �׾����� ����ڰ� �������̴�. �ƹ��� ������ ���.
		// m_pClientList[iClientH]->m_iExp -= iDice(1, 50);
		// if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;

		// SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
	}
	//----------------------------EK Announcer-------------------------
	//---- Function: CGame::ClientKilledHandler                    ----
	//---- Description: Announces a message to alert all connected ----
	//---- clients an EK has taken place                           ----
	//---- Version: HBX 2.03 Build                                 ----
	//---- Date: November 07 2005                                  ----
	//---- By: Daxation                                            ----
	//---- Notes: Add char cEKMsg[1000]                            ----
	//-----------------------------------------------------------------
	std::memset(cEKMsg, 0, sizeof (cEKMsg));
	//Multiple EK Messages
	//Note - Remove section '01' and replace with alternative code for a single message
	//Alternative code: wsprintf(cEKMsg, "%s killed %s", cAttackerName, m_pClientList[iClientH]->m_cCharName);
	// 01
	switch (iDice(1, 10))
		//You can add extra messages by creating a new case.
		//Remember to increase iDice
	{
		case 1: // To reverse the order the names appear in the message reverse the last 2 parameters
			wsprintf(cEKMsg, "%s whooped %s's ass!", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
		case 2:
			wsprintf(cEKMsg, "%s smashed %s's face into the ground!", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
		case 3:
			wsprintf(cEKMsg, "%s was sliced to pieces by %s", m_pClientList[iClientH]->m_cCharName, cAttackerName);
			break;
		case 4:
			wsprintf(cEKMsg, "%s says LAG LAG!! but gets PWNED by %s", m_pClientList[iClientH]->m_cCharName, cAttackerName);
			break;
		case 5:
			wsprintf(cEKMsg, "%s sent %s off too pie heaven!", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
		case 6:
			wsprintf(cEKMsg, "%s got beat by %s's ugly stick!", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
		case 7:
			wsprintf(cEKMsg, "%s OwneD! %s", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
		case 8:
			wsprintf(cEKMsg, "%s Sended %s To Revival Zone! Too Bad ;(", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
		case 9:
			wsprintf(cEKMsg, "%s says: I CAN OWN YOU! But gets OWNED by %s", m_pClientList[iClientH]->m_cCharName, cAttackerName);
			break;
		case 10:
			wsprintf(cEKMsg, "%s KilleD %s", cAttackerName, m_pClientList[iClientH]->m_cCharName);
			break;
	}
	// 01
	for (i = 1; i < DEF_MAXCLIENTS; i++) // Check all clients
	{
		if ((m_pClientList[i] != nullptr)) // Check if client is avtice
		{
			SendNotifyMsg(0, i, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cEKMsg); // Send message to client
			// Log EK
			wsprintf(G_cTxt, "%s killed %s", cAttackerName, m_pClientList[iClientH]->m_cCharName); // Log message
			PutLogFileList(G_cTxt); // Enter into logs
		}
	}
	//-----------------------------------------------------------------
	//----                   End EK Announcer Code                 ----
	//-----------------------------------------------------------------
}

void CGame::ReleaseItemHandler(int iClientH, short sItemIndex, bool bNotice) {
	char cEquipPos, cHeroArmorType;
	short sTemp;
	int iTemp;

	if (m_pClientList[iClientH] == nullptr) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return;

	if (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == false) return;

	cHeroArmorType = _cCheckHeroItemEquipped(iClientH);
	if (cHeroArmorType != 0x0FFFFFFFF) m_pClientList[iClientH]->m_cHeroArmourBonus = 0;

	cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;
	if (cEquipPos == DEF_EQUIPPOS_RHAND) {
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != nullptr) {
			if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
				m_pClientList[iClientH]->m_cMagicMastery[94] = false;
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
			}
		}
	}


	switch (cEquipPos) {
		case DEF_EQUIPPOS_RHAND:

			sTemp = m_pClientList[iClientH]->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			m_pClientList[iClientH]->m_sAppr2 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0x0FFFFFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;


			iTemp = m_pClientList[iClientH]->m_iStatus;
			iTemp = iTemp & 0xFFFFFFF0;
			m_pClientList[iClientH]->m_iStatus = iTemp;
			break;

		case DEF_EQUIPPOS_LHAND:

			sTemp = m_pClientList[iClientH]->m_sAppr2;
			sTemp = sTemp & 0xFFF0;
			m_pClientList[iClientH]->m_sAppr2 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xF0FFFFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_TWOHAND:

			sTemp = m_pClientList[iClientH]->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			m_pClientList[iClientH]->m_sAppr2 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0x0FFFFFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_BODY:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			m_pClientList[iClientH]->m_sAppr3 = sTemp;


			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0xFF7F;
			m_pClientList[iClientH]->m_sAppr4 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFF0FFFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_BACK:
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0xF0FF;
			m_pClientList[iClientH]->m_sAppr4 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_ARMS:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0xFFF0;
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFF0FFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_PANTS:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0xF0FF;
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFF0FF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_LEGGINGS:
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0x0FFF;
			m_pClientList[iClientH]->m_sAppr4 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFF0F;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_HEAD:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0xFF0F;
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF0;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;

		case DEF_EQUIPPOS_RELEASEALL:
			sTemp = m_pClientList[iClientH]->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			m_pClientList[iClientH]->m_sAppr3 = sTemp;

			iTemp = m_pClientList[iClientH]->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			m_pClientList[iClientH]->m_iApprColor = iTemp;
			break;
	}


	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) {

		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFF3;
	}

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY) {

		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFFC;
	}

	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;
	m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] = -1;


	if (bNotice == true)
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);


	CalcTotalItemEffect(iClientH, sItemIndex, true);
}

bool CGame::_bDecodeNpcConfigFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iNpcConfigListIndex = 0;
	class CStrTok * pStrTok;

	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	//token = strtok(pContents, seps);
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							// NPC
							if (strlen(token) > 20) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Too long Npc name.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							std::memset(m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName, 0, sizeof (m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName));
							memcpy(m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName, token, strlen(token));
							cReadModeB = 2;
							break;
						case 2:
							// m_sType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_sType = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							// m_iHitDice
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iHitDice = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// m_iDefenseRatio
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iDefenseRatio = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// m_iHitRatio
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iHitRatio = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							// m_iMinBravery
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iMinBravery = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							// m_iExpDiceMin
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iExpDiceMin = atoi(token);
							cReadModeB = 8;
							break;

						case 8:
							// m_iExpDiceMax
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iExpDiceMax = atoi(token);
							cReadModeB = 9;
							break;

						case 9:
							// m_iGoldDiceMin
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iGoldDiceMin = atoi(token);
							cReadModeB = 10;
							break;

						case 10:
							// m_iGoldDiceMax
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iGoldDiceMax = atoi(token);
							cReadModeB = 11;
							break;

						case 11:
							// m_cAttackDiceThrow
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cAttackDiceThrow = atoi(token);
							cReadModeB = 12;
							break;

						case 12:
							// m_cAttackDiceRange
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cAttackDiceRange = atoi(token);
							cReadModeB = 13;
							break;

						case 13:
							// m_cSize
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cSize = atoi(token);
							cReadModeB = 14;
							break;

						case 14:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cSide = atoi(token);
							cReadModeB = 15;
							break;

						case 15:
							// ActionLimit
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cActionLimit = atoi(token);
							cReadModeB = 16;
							break;

						case 16:
							// Action Time
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_dwActionTime = atoi(token);
							cReadModeB = 17;
							break;

						case 17:
							// ResistMagic
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cResistMagic = atoi(token);
							cReadModeB = 18;
							break;

						case 18:
							// cMagicLevel
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cMagicLevel = atoi(token);
							cReadModeB = 19;
							break;

						case 19:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cDayOfWeekLimit = atoi(token);
							cReadModeB = 20;
							break;

						case 20:
							// cChatMsgPresence
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cChatMsgPresence = atoi(token);

							if (m_pNpcConfigList[iNpcConfigListIndex]->m_cChatMsgPresence == 1) {



							}
							cReadModeB = 21;
							break;

						case 21:
							// m_cTargetSearchRange
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cTargetSearchRange = atoi(token);

							cReadModeB = 22;
							break;

						case 22:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_dwRegenTime = atoi(token);

							cReadModeB = 23;
							break;

						case 23:
							// Attribute
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_cAttribute = atoi(token);

							cReadModeB = 24;
							break;

						case 24:
							// Absorb Magic Damage
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iAbsDamage = atoi(token);

							cReadModeB = 25;
							break;

						case 25:
							// Maximum Mana Point
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iMaxMana = atoi(token);

							cReadModeB = 26;
							break;

						case 26:
							// MagicHitRatio
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iMagicHitRatio = atoi(token);

							cReadModeB = 27;
							break;

						case 27:
							// AttackRange
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcConfigList[iNpcConfigListIndex]->m_iAttackRange = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							iNpcConfigListIndex++;
							break;
					}
					break;

				case 2:
					m_iPlayerMaxLevel = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				default:
					break;
			}
		} else {
			if (memcmp(token, "Npc", 3) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
				m_pNpcConfigList[iNpcConfigListIndex] = new class CNpc(" ");
			}

			if (memcmp(token, "world-server-max-level", 22) == 0) {
				cReadModeA = 2;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
		//token = strtok(nullptr, seps);
	}

	delete pStrTok;
	delete pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! NPC configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) NPC(Total:%d) configuration - success!", iNpcConfigListIndex);
	PutLogList(cTxt);

	return true;
}

bool CGame::_bInitNpcAttr(class CNpc * pNpc, const char * pNpcName, short sClass, char cSA) {
	register int i, iTemp;
	char cTmpName[21];
	int sTemp;
	double dV1, dV2, dV3;

	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pNpcName);

	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		if (m_pNpcConfigList[i] != nullptr) {
			if (memcmp(cTmpName, m_pNpcConfigList[i]->m_cNpcName, 20) == 0) {

				std::memset(pNpc->m_cNpcName, 0, sizeof (pNpc->m_cNpcName));
				memcpy(pNpc->m_cNpcName, m_pNpcConfigList[i]->m_cNpcName, 20);

				pNpc->m_sType = m_pNpcConfigList[i]->m_sType;


				if (m_pNpcConfigList[i]->m_iHitDice <= 5)
					pNpc->m_iHP = abs(iDice(m_pNpcConfigList[i]->m_iHitDice, 4) + m_pNpcConfigList[i]->m_iHitDice);
				else pNpc->m_iHP = abs((m_pNpcConfigList[i]->m_iHitDice * 4) + m_pNpcConfigList[i]->m_iHitDice + iDice(1, m_pNpcConfigList[i]->m_iHitDice));

				if (pNpc->m_iHP == 0) pNpc->m_iHP = 1;

				pNpc->m_iExpDiceMin = m_pNpcConfigList[i]->m_iExpDiceMin;
				pNpc->m_iExpDiceMax = m_pNpcConfigList[i]->m_iExpDiceMax;
				pNpc->m_iGoldDiceMin = m_pNpcConfigList[i]->m_iGoldDiceMin * 5;
				pNpc->m_iGoldDiceMax = m_pNpcConfigList[i]->m_iGoldDiceMax * 5;
				pNpc->m_iExp = (iDice(1, (m_pNpcConfigList[i]->m_iExpDiceMax - m_pNpcConfigList[i]->m_iExpDiceMin)) + m_pNpcConfigList[i]->m_iExpDiceMin) * 500;
				pNpc->m_iHitDice = m_pNpcConfigList[i]->m_iHitDice;
				pNpc->m_iDefenseRatio = m_pNpcConfigList[i]->m_iDefenseRatio;
				pNpc->m_iHitRatio = m_pNpcConfigList[i]->m_iHitRatio;
				pNpc->m_iMinBravery = m_pNpcConfigList[i]->m_iMinBravery;
				pNpc->m_cAttackDiceThrow = m_pNpcConfigList[i]->m_cAttackDiceThrow;
				pNpc->m_cAttackDiceRange = m_pNpcConfigList[i]->m_cAttackDiceRange;
				pNpc->m_cSize = m_pNpcConfigList[i]->m_cSize;
				pNpc->m_cSide = m_pNpcConfigList[i]->m_cSide;
				pNpc->m_cActionLimit = m_pNpcConfigList[i]->m_cActionLimit;
				pNpc->m_dwActionTime = m_pNpcConfigList[i]->m_dwActionTime;
				pNpc->m_dwRegenTime = m_pNpcConfigList[i]->m_dwRegenTime;
				pNpc->m_cResistMagic = m_pNpcConfigList[i]->m_cResistMagic;
				pNpc->m_cMagicLevel = m_pNpcConfigList[i]->m_cMagicLevel;
				pNpc->m_iMaxMana = m_pNpcConfigList[i]->m_iMaxMana; // v1.4
				pNpc->m_iMana = m_pNpcConfigList[i]->m_iMaxMana;
				pNpc->m_cChatMsgPresence = m_pNpcConfigList[i]->m_cChatMsgPresence;
				pNpc->m_cDayOfWeekLimit = m_pNpcConfigList[i]->m_cDayOfWeekLimit;
				pNpc->m_cTargetSearchRange = m_pNpcConfigList[i]->m_cTargetSearchRange;

				switch (sClass) {
					case 43:
					case 44:
					case 45:
					case 46:
					case 47:
						pNpc->m_iAttackStrategy = DEF_ATTACKAI_NORMAL;
						break;

					default:
						pNpc->m_iAttackStrategy = iDice(1, 10);
						break;
				}

				pNpc->m_iAILevel = iDice(1, 3);
				pNpc->m_iAbsDamage = m_pNpcConfigList[i]->m_iAbsDamage;
				pNpc->m_iMagicHitRatio = m_pNpcConfigList[i]->m_iMagicHitRatio;
				pNpc->m_iAttackRange = m_pNpcConfigList[i]->m_iAttackRange;
				pNpc->m_cSpecialAbility = cSA;
				pNpc->m_iBuildCount = m_pNpcConfigList[i]->m_iMinBravery;


				switch (pNpc->m_cSpecialAbility) {
					case 1:
						dV2 = (double) pNpc->m_iExp;
						dV3 = 25.0f / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						}// >>>Happy Hour MrKrhiz <<<
						else {
							pNpc->m_iExp += (int) dV1;
						}
						break;
					case 2:
						dV2 = (double) pNpc->m_iExp;
						dV3 = 30.0f / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						} else {
							pNpc->m_iExp += (int) dV1;
						}
						break;
					case 3: // Absorbing Physical Damage
						if (pNpc->m_iAbsDamage > 0) {
							pNpc->m_cSpecialAbility = 0;
							cSA = 0;
						} else {
							iTemp = 20 + iDice(1, 60);
							pNpc->m_iAbsDamage -= iTemp;
							if (pNpc->m_iAbsDamage < -90) pNpc->m_iAbsDamage = -90;
						}
						dV2 = (double) pNpc->m_iExp;
						dV3 = (double) abs(pNpc->m_iAbsDamage) / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						} else {
							pNpc->m_iExp += (int) dV1;
						}
						break;

					case 4: // Absorbing Magical Damage
						if (pNpc->m_iAbsDamage < 0) {

							pNpc->m_cSpecialAbility = 0;
							cSA = 0;
						} else {
							iTemp = 20 + iDice(1, 60);
							pNpc->m_iAbsDamage += iTemp;
							if (pNpc->m_iAbsDamage > 90) pNpc->m_iAbsDamage = 90;
						}
						dV2 = (double) pNpc->m_iExp;
						dV3 = (double) (pNpc->m_iAbsDamage) / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						} else {
							pNpc->m_iExp += (int) dV1;
						}
						break;

					case 5:
						dV2 = (double) pNpc->m_iExp;
						dV3 = 15.0f / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						} else {
							pNpc->m_iExp += (int) dV1;
						}
						break;

					case 6:
					case 7:
						dV2 = (double) pNpc->m_iExp;
						dV3 = 20.0f / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						} else {
							pNpc->m_iExp += (int) dV1;
						}
						break;

					case 8:
						dV2 = (double) pNpc->m_iExp;
						dV3 = 25.0f / 100.0f;
						dV1 = dV2 * dV3;
						if (m_bHappyHour == true) {
							pNpc->m_iExp += (int) dV1 * 2;
						} else {
							pNpc->m_iExp += (int) dV1;
						}
						break;
				}


				pNpc->m_iNoDieRemainExp = (pNpc->m_iExp) - (pNpc->m_iExp / 3);


				pNpc->m_iStatus = pNpc->m_iStatus & 0xFFFFF0FF;
				sTemp = cSA;
				sTemp = sTemp << 8;
				pNpc->m_iStatus = pNpc->m_iStatus | sTemp;


				pNpc->m_iStatus = pNpc->m_iStatus & 0xFFFFFFF0;
				pNpc->m_iStatus = pNpc->m_iStatus | (sClass);

				return true;
			}
		}


	return false;
}

/*********************************************************************************************************************
 **  int CGame::iDice(int iThrow, int iRange)																		**
 **  description			:: produces a random number between the throw and range										**
 **  last updated		:: November 20, 2004; 10:24 PM; Hypnotoad													**
 **	return value		:: int																						**
 **********************************************************************************************************************/
int CGame::iDice(int iThrow, int iRange) {
	register int i, iRet;

	if (iRange <= 0) return 0;
	iRet = 0;
	for (i = 1; i <= iThrow; i++) {
		iRet += (rand() % iRange) + 1;
	}
	return iRet;
}

/*
void CGame::OnStartGameSignal()
{
 int i;
 char * cp;
 HANDLE hFile;
 FILE * pFile;
 uint32_t  dwFileSize;

	
	for (i = 0; i < DEF_MAXMAPS; i++)
	if (m_pMapList[i] != nullptr)
		_bReadMapInfoFiles(i);

	// Crusade

	
	hFile = CreateFile("GameData\\OccupyFlag.txt", GENERIC_READ, nullptr, nullptr, OPEN_EXISTING, nullptr, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen("GameData\\OccupyFlag.txt", "rt");
	if (pFile == nullptr) {
		
		PutLogList("(!) Cannot open file: GameData\\OccupyFlag.txt");
	}
	else {
		PutLogList("(!) Occupy Flag save file contents decoding...");

		cp = new char[dwFileSize+2];
		ZeroMemory(cp, dwFileSize+2);
		fread(cp, dwFileSize, 1, pFile);
		_bDecodeOccupyFlagSaveFileContents(cp, dwFileSize);
		delete cp;
	}

	
	bReadCrusadeStructureConfigFile("Crusade.cfg");
	
	_LinkStrikePointMapIndex();

	bReadCrusadeGUIDFile("GameData\\CrusadeGUID.txt");

	PutLogList("");
	PutLogList("(!) Game Server Activated.");
}*/

void CGame::CalculateGuildEffect(int iVictimH, char cVictimType, short sAttackerH) {
	register int ix, iy, iExp;
	short sOwnerH;
	char cOwnerType;


	return;

	if (m_pClientList[sAttackerH] == nullptr) return;

	switch (cVictimType) {

		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[iVictimH] == nullptr) return;

			for (ix = m_pNpcList[iVictimH]->m_sX - 2; ix <= m_pNpcList[iVictimH]->m_sX + 2; ix++)
				for (iy = m_pNpcList[iVictimH]->m_sY - 2; iy <= m_pNpcList[iVictimH]->m_sY + 2; iy++) {

					m_pMapList[m_pNpcList[iVictimH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);

					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH != 0) && (sOwnerH != sAttackerH) &&
							  (memcmp(m_pClientList[sAttackerH]->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
							  (m_pClientList[sAttackerH]->m_iLevel >= m_pClientList[sOwnerH]->m_iLevel)) {


						if (iDice(1, 3) == 2) {
							iExp = (m_pNpcList[iVictimH]->m_iExp / 3);
							if (m_pClientList[sAttackerH]->m_iLevel >= m_iPlayerMaxLevel) iExp = 0;

							if (iExp > 0) {

								m_pClientList[sOwnerH]->m_iExp += iExp;
								if (bCheckLimitedUser(sOwnerH) == false) {

									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
								}

								bCheckLevelUp(sOwnerH);
							}
						}
					}
				}
			break;
	}
}

void CGame::TimeManaPointsUp(int iClientH) {
	register int iMaxMP, iTotal;
	double dV1, dV2, dV3;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iHungerStatus <= 0) return;
	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;

	iMaxMP = (2 * (m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag)) + (2 * m_pClientList[iClientH]->m_iLevel) + ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) / 2); // v1.4
	if (m_pClientList[iClientH]->m_iMP < iMaxMP) {
		iTotal = iDice(1, ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag)));
		if (m_pClientList[iClientH]->m_iAddMP != 0) {
			dV2 = (double) iTotal;
			dV3 = (double) m_pClientList[iClientH]->m_iAddMP;
			dV1 = (dV3 / 100.0f) * dV2;
			iTotal += (int) dV1;
		}

		m_pClientList[iClientH]->m_iMP += iTotal;

		if (m_pClientList[iClientH]->m_iMP > iMaxMP)
			m_pClientList[iClientH]->m_iMP = iMaxMP;

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
	}
}

// 05/29/2004 - Hypnotoad - fixed infinite sp bug

void CGame::TimeStaminarPointsUp(int iClientH) {
	register int iMaxSP, iTotal = 0;
	double dV1, dV2, dV3;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iHungerStatus <= 0) return;
	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;

	iMaxSP = (2 * (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr)) + (2 * m_pClientList[iClientH]->m_iLevel);
	if (m_pClientList[iClientH]->m_iSP < iMaxSP) {

		iTotal = iDice(1, (m_pClientList[iClientH]->m_iVit / 3));
		if (m_pClientList[iClientH]->m_iAddSP != 0) {
			dV2 = (double) iTotal;
			dV3 = (double) m_pClientList[iClientH]->m_iAddSP;
			dV1 = (dV3 / 100.0f) * dV2;
			iTotal += (int) dV1;
		}


		if (m_pClientList[iClientH]->m_iLevel <= 20) {
			iTotal += 15;
		} else if (m_pClientList[iClientH]->m_iLevel <= 40) {
			iTotal += 10;
		} else if (m_pClientList[iClientH]->m_iLevel <= 60) {
			iTotal += 5;
		}

		m_pClientList[iClientH]->m_iSP += iTotal;
		if (m_pClientList[iClientH]->m_iSP > iMaxSP)
			m_pClientList[iClientH]->m_iSP = iMaxSP;

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
	}
}

void CGame::SendGuildMsg(int iClientH, uint16_t wNotifyMsgType, short /*sV1*/, short /*sV2*/, char * pString) {
	char cData[500];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	register int i, iRet;


	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) &&
				  (memcmp(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName, 20) == 0)) {


			std::memset(cData, 0, sizeof (cData));

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = wNotifyMsgType;

			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


			switch (wNotifyMsgType) {
				case DEF_NOTIFY_GUILDDISBANDED:
					if (i == iClientH) break;

					memcpy(cp, m_pClientList[iClientH]->m_cGuildName, 20);
					cp += 20;

					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 26);

					std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
					strcpy(m_pClientList[i]->m_cGuildName, "NONE");
					m_pClientList[i]->m_iGuildRank = -1;
					m_pClientList[i]->m_iGuildGUID = -1;
					break;

				case DEF_NOTIFY_EVENTMSGSTRING:

					strcpy(cp, pString);
					cp += strlen(pString);

					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + strlen(pString) + 1);
					break;

				case DEF_NOTIFY_NEWGUILDSMAN:
					memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
					cp += 10;

					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + 10 + 1);
					break;

				case DEF_NOTIFY_DISMISSGUILDSMAN:
					memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
					cp += 10;

					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + 10 + 1);
					break;
			}

			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					DeleteClient(i, true, true);
					return;
			}
		}

}

void CGame::GuildNotifyHandler(char * pData, uint32_t /*dwMsgSize*/) {

	char * cp, cCharName[11], cGuildName[21];

	std::memset(cCharName, 0, sizeof (cCharName));
	std::memset(cGuildName, 0, sizeof (cGuildName));

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	memcpy(cCharName, cp, 10);
	cp += 10;

	memcpy(cGuildName, cp, 20);
	cp += 20;


}

void CGame::ToggleCombatModeHandler(int iClientH) {
	short sAppr2;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;

	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;

	sAppr2 = (short) ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) >> 12);

	m_pClientList[iClientH]->m_bIsAttackModeChange = true; // v2.172


	if (sAppr2 == 0) {

		m_pClientList[iClientH]->m_sAppr2 = (0xF000 | m_pClientList[iClientH]->m_sAppr2);
	} else {

		m_pClientList[iClientH]->m_sAppr2 = (0x0FFF & m_pClientList[iClientH]->m_sAppr2);
	}


	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: checks if player is casting magic
//  last updated		:: October 29, 2004; 6:51 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////

int CGame::iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir) {
	char cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;

	if (m_pClientList[iClientH] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	int iStX, iStY;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != nullptr) {
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (m_pClientList[iClientH]->m_cSide) {
			case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}

	ClearSkillUsingStatus(iClientH);

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

	if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0) {
		SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
		delayEvents_.bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		m_pClientList[ iClientH ]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
	}

	m_pClientList[iClientH]->m_cDir = cDir;

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(iClientH, true, true);
			return 0;
	}

	return 1;
}

/*********************************************************************************************************************
 **  void CGame::PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1)			**
 **  description			:: handles all magic related items/spells													**
 **  last updated		:: November 22, 2004; 5:45 PM; Hypnotoad													**
 **	return value		:: void																						**
 **  commentary			::	-	added 3.51 casting detection														**
 **							-	updated it so civilians can only cast certain spells on players and vice versa		**
 **							-	fixed bug causing spell to be cast when mana is below required amount				**
 **********************************************************************************************************************/
int _tmp_iMCProb[] = {0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40};
int _tmp_iMLevelPenalty[] = {0, 5, 5, 8, 8, 10, 14, 28, 32, 36, 40};

void CGame::PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1) {
	short * sp, sX, sY, sOwnerH, sMagicCircle, rx, ry, sRemainItemSprite, sRemainItemSpriteFrame, sLevelMagic, sTemp;
	char * cp, cData[120], cDir, cOwnerType, cName[11], cItemName[21], cNpcWaypoint[11], cName_Master[11], cNpcName[21], cRemainItemColor, cScanMessage[256];
	double dV1, dV2, dV3, dV4;
	int i, iErr, iRet, ix, iy, iResult, iDiceRes, iNamingValue, iFollowersNum, iEraseReq, iWhetherBonus;
	int tX, tY, iManaCost, iMagicAttr;
	class CItem * pItem;
	uint32_t * dwp, dwTime;
	uint16_t * wp, wWeaponType;
	short sEqStatus;

	dwTime = timeGetTime();
	m_pClientList[iClientH]->m_bMagicConfirm = true;

	m_pClientList[iClientH]->m_dwLastActionTime = m_pClientList[iClientH]->m_dwAFKCheckTime = timeGetTime();
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	if ((dX < 0) || (dX >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeX) ||
			  (dY < 0) || (dY >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY)) return;

	if (((dwTime - m_pClientList[iClientH]->m_dwRecentAttackTime) < 1000) && (bItemEffect == 0)) {
		wsprintf(G_cTxt, "3.51 Detection: (%s) Player: (%s) - Magic casting speed is too fast! Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, true, true);
		return;
	}
	m_pClientList[iClientH]->m_dwRecentAttackTime = dwTime;
	m_pClientList[iClientH]->m_dwLastActionTime = dwTime;

	if (m_pClientList[iClientH]->m_cMapIndex < 0) return;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] == nullptr) return;

	if ((sType < 0) || (sType >= 100)) return;
	if (m_pMagicConfigList[sType] == nullptr) return;

	if ((bItemEffect == false) && (m_pClientList[iClientH]->m_cMagicMastery[sType] != 1)) return;

	if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsAttackEnabled == false) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) return;
	//if ((var_874 == true) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == true) && (m_pMagicConfigList[sType]->m_sType != 8)) return;

	if (((m_pClientList[iClientH]->m_iStatus & 0x100000) != 0) && (bItemEffect != true)) {
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}

	if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
		wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
		if ((wWeaponType >= 34) && (wWeaponType <= 39)) {
		} else return;
	}

	if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1) ||
			  (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)) return;

	if ((m_pClientList[iClientH]->m_iSpellCount > 1) && (bItemEffect == false)) {
		wsprintf(G_cTxt, "TSearch Spell Hack: (%s) Player: (%s) - casting magic without precasting.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, true, true);
		return;
	}

	if (m_pClientList[iClientH]->m_bInhibition == true) {
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}

	/*if (((m_pClientList[iClientH]->m_iUninteruptibleCheck - (iGetMaxHP(iClientH)/10)) > (m_pClientList[iClientH]->m_iHP)) && (m_pClientList[iClientH]->m_bMagicItem == false)) {
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, nullptr,
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
		return;
	}*/

	if (m_pMagicConfigList[sType]->m_sType == 32) { // Invisiblity
		sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
		if ((sEqStatus != -1) && (m_pClientList[iClientH]->m_pItemList[sEqStatus] != nullptr)) {
			if ((m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_sIDnum == 866)) {
				bItemEffect = true;
			}
		}
	}

	sX = m_pClientList[iClientH]->m_sX;
	sY = m_pClientList[iClientH]->m_sY;

	sMagicCircle = (sType / 10) + 1;
	if (m_pClientList[iClientH]->m_cSkillMastery[4] == 0)
		dV1 = 1.0f;
	else dV1 = (double) m_pClientList[iClientH]->m_cSkillMastery[4];

	if (bItemEffect == true) dV1 = (double) 100.0f;
	dV2 = (double) (dV1 / 100.0f);
	dV3 = (double) _tmp_iMCProb[sMagicCircle];
	dV1 = dV2 * dV3;
	iResult = (int) dV1;

	if ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) > 50)
		iResult += ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) - 50) / 2;

	sLevelMagic = (m_pClientList[iClientH]->m_iLevel / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double) (m_pClientList[iClientH]->m_iLevel - sLevelMagic * 10);
			dV2 = (double) abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double) abs(sMagicCircle - sLevelMagic)*10;
			dV4 = (dV1 / dV3) * dV2;
			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int) dV4);
		} else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}

	switch (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus) {
		case 0: break;
		case 1: iResult = iResult - (iResult / 24);
			break;
		case 2: iResult = iResult - (iResult / 12);
			break;
		case 3: iResult = iResult - (iResult / 5);
			break;
	}

	if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == 10) {
		dV1 = (double) iResult;
		dV2 = (double) (m_pClientList[iClientH]->m_iSpecialWeaponEffectValue * 3);
		dV3 = dV1 + dV2;
		iResult = (int) dV3;
	}

	if (iResult <= 0) iResult = 1;

	iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus);

	iManaCost = m_pMagicConfigList[sType]->m_sValue1;
	if ((m_pClientList[iClientH]->m_bIsSafeAttackMode == true) &&
			  (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == false)) {
		iManaCost += (iManaCost / 2) - (iManaCost / 10);
	}

	if (m_pClientList[iClientH]->m_iManaSaveRatio > 0) {
		dV1 = (double) m_pClientList[iClientH]->m_iManaSaveRatio;
		dV2 = (double) (dV1 / 100.0f);
		dV3 = (double) iManaCost;
		dV1 = dV2 * dV3;
		dV2 = dV3 - dV1;
		iManaCost = (int) dV2;

		if (iManaCost <= 0) iManaCost = 1;
	}

	wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
	if (wWeaponType == 34) {
		iManaCost += 20;
	}

	if (iResult < 100) {
		iDiceRes = iDice(1, 100);
		if (iResult < iDiceRes) {
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
			return;
		}
	}

	if (((m_pClientList[iClientH]->m_iHungerStatus <= 10) || (m_pClientList[iClientH]->m_iSP <= 0)) && (iDice(1, 1000) <= 100)) {
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}

	if (m_pClientList[iClientH]->m_iMP < iManaCost) {
		return;
	}

	iResult = m_pClientList[iClientH]->m_cSkillMastery[4];
	if ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) > 50) iResult += ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) - 50);

	sLevelMagic = (m_pClientList[iClientH]->m_iLevel / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double) (m_pClientList[iClientH]->m_iLevel - sLevelMagic * 10);
			dV2 = (double) abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double) abs(sMagicCircle - sLevelMagic)*10;
			dV4 = (dV1 / dV3) * dV2;

			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int) dV4);
		} else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}

	iResult += m_pClientList[iClientH]->m_iAddAR;
	if (iResult <= 0) iResult = 1;

	if (sType >= 80) iResult += 10000;

	if (m_pMagicConfigList[sType]->m_sType == 28) {
		iResult += 10000;
	}

	if (m_pMagicConfigList[sType]->m_cCategory == 1) {
		if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000005) != 0) return;
	}

	iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;
	if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0) {
		SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
		delayEvents_.bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
	}

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((m_bIsCrusadeMode == false) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
		if ((m_pClientList[iClientH]->m_bIsPlayerCivil != true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true)) {
			if (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide) return;
		} else if ((m_pClientList[iClientH]->m_bIsPlayerCivil == true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == false)) {
			switch (m_pMagicConfigList[sType]->m_sType) {
				case 1: // DEF_MAGICTYPE_DAMAGE_SPOT
				case 4: // DEF_MAGICTYPE_SPDOWN_SPOT 4
				case 8: // DEF_MAGICTYPE_TELEPORT 8
				case 10: // DEF_MAGICTYPE_CREATE 10
				case 11: // DEF_MAGICTYPE_PROTECT 11
				case 12: // DEF_MAGICTYPE_HOLDOBJECT 12
				case 16: // DEF_MAGICTYPE_CONFUSE
				case 17: // DEF_MAGICTYPE_POISON
				case 24: // DEF_MAGICTYPE_RESURRECTION
					return;
			}
		}
	}

	if (m_pMagicConfigList[sType]->m_dwDelayTime == 0) {
		switch (m_pMagicConfigList[sType]->m_sType) {

			case DEF_MAGICTYPE_DAMAGE_SPOT:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}
				break;

			case DEF_MAGICTYPE_HPUP_SPOT:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				Effect_HpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				break;

			case DEF_MAGICTYPE_PURITYLIFE_SPOT:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				PURITYLIFE(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				break;

			case DEF_MAGICTYPE_DAMAGE_AREA:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}

				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;

			case DEF_MAGICTYPE_SPDOWN_SPOT:
				break;

			case DEF_MAGICTYPE_SPDOWN_AREA:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
				break;

			case DEF_MAGICTYPE_POLYMORPH:

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (1) { // bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] != 0) goto MAGIC_NOEFFECT;
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] = (char) m_pMagicConfigList[sType]->m_sValue4;

							m_pClientList[sOwnerH]->m_sOriginalType = m_pClientList[sOwnerH]->m_sType;

							m_pClientList[sOwnerH]->m_sType = 18;
							SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] != 0) goto MAGIC_NOEFFECT;
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] = (char) m_pMagicConfigList[sType]->m_sValue4;

							m_pNpcList[sOwnerH]->m_sOriginalType = m_pNpcList[sOwnerH]->m_sType;

							m_pNpcList[sOwnerH]->m_sType = 18;
							SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							break;
					}


					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_POLYMORPH, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);


					if (cOwnerType == DEF_OWNERTYPE_PLAYER)
						SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POLYMORPH, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				}
				break;

				// 05/20/2004 - Hypnotoad - Cancellation
			case DEF_MAGICTYPE_CANCELLATION:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)) {

					// Removes Invisibility Flag 0x0010
					SetInvisibilityFlag(sOwnerH, cOwnerType, false);

					// Removes Illusion Flag 0x01000000
					SetIllusionFlag(sOwnerH, cOwnerType, false);

					// Removes Defense Shield Flag 0x02000000
					// Removes Great Defense Shield Flag 0x02000000
					SetDefenseShieldFlag(sOwnerH, cOwnerType, false);

					// Removes Absolute Magic Protection Flag 0x04000000
					// Removes Protection From Magic Flag 0x04000000
					SetMagicProtectionFlag(sOwnerH, cOwnerType, false);

					// Removes Protection From Arrow Flag 0x08000000
					SetProtectionFromArrowFlag(sOwnerH, cOwnerType, false);

					// Removes Illusion Movement Flag 0x00200000
					SetIllusionMovementFlag(sOwnerH, cOwnerType, false);

					// Removes Berserk Flag 0x0020
					SetBerserkFlag(sOwnerH, cOwnerType, false);

					if (m_pClientList[iClientH]->m_bInhibition == true)
						delayEvents_.bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INHIBITION);
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

					if (m_pClientList[iClientH]->m_iStatus & 0x0010)
						delayEvents_.bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

					if (m_pClientList[iClientH]->m_iStatus & 0x0020)
						delayEvents_.bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_BERSERK);
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

					if ((m_pClientList[iClientH]->m_iStatus & 0x08000000) || (m_pClientList[iClientH]->m_iStatus & 0x04000000) || (m_pClientList[iClientH]->m_iStatus & 0x02000000))
						delayEvents_.bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

					if ((m_pClientList[iClientH]->m_iStatus & 0x01000000) || (m_pClientList[iClientH]->m_iStatus & 0x00200000))
						delayEvents_.bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

					// Update Client
					SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
				}
				break;

			case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:

				// �ֺ� ��� ȿ��
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// �ڽŵ� ����� �� ������ ����.
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
						}
					}
				break;

			case DEF_MAGICTYPE_DAMAGE_LINEAR:

				sX = m_pClientList[iClientH]->m_sX;
				sY = m_pClientList[iClientH]->m_sY;

				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

					// tx, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx-1, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx+1, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx, ty-1
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx, ty+1
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}


				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}

				// dX, dY
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				}
				break;


			case DEF_MAGICTYPE_ICE_LINEAR:

				sX = m_pClientList[iClientH]->m_sX;
				sY = m_pClientList[iClientH]->m_sY;

				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

					// tx, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_iHP < 0) goto MAGIC_NOEFFECT;

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}

					// tx-1, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}

					// tx+1, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}

					// tx, ty-1
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}

					// tx, ty+1
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}

					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}


				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

								switch (cOwnerType) {
									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

										if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
											if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
												m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
												SetIceFlag(sOwnerH, cOwnerType, true);

												delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

												SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
											}
										}
										break;

									case DEF_OWNERTYPE_NPC:
										if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
										if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
											if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
												m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
												SetIceFlag(sOwnerH, cOwnerType, true);

												delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											}
										}
										break;
								}
								//
							}
						}
					}

				// dX, dY
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);

									delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);

									delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}
				}
				break;


			case DEF_MAGICTYPE_INHIBITION:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if ((m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) || (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)) goto MAGIC_NOEFFECT; // juan - ic fix para que no pegue si tiene AMP o PFM .
						if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INHIBITION ] != 0) goto MAGIC_NOEFFECT;
						if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
						//if (m_pClientList[sOwnerH]->m_iAdminUserLevel != 0) goto MAGIC_NOEFFECT;
						m_pClientList[sOwnerH]->m_bInhibition = true;
						delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
						break;
				}
				break;
			case DEF_MAGICTYPE_TREMOR:
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}


				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;

			case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:


				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;

			case DEF_MAGICTYPE_SPUP_AREA:

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

				Effect_SpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);

				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);

						Effect_SpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
				break;


			case DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN:

				sX = m_pClientList[iClientH]->m_sX;
				sY = m_pClientList[iClientH]->m_sY;

				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

					// tx, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}

					// tx-1, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}

					// tx+1, ty
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}

					// tx, ty-1
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}

					// tx, ty+1
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}

					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}


				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

								switch (cOwnerType) {
									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

										if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
											Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
											Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
										}
										break;

									case DEF_OWNERTYPE_NPC:
										if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
										if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
											Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
											Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

										}
										break;
								}
								//
							}
						}
					}

				// dX, dY
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
					}
					//
				}

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);

								}
								break;
						}
						//
					}
				}
				break;

			case DEF_MAGICTYPE_TELEPORT:
				//50Cent - Capture The Flag
				if (bCheckIfIsFlagCarrier(iClientH)) {
					ShowClientMsg(iClientH, "You can not use that magic being a flag carrier.");
					goto MAGIC_NOEFFECT;
				}
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH == iClientH)) {
							if ((dwTime - m_pClientList[iClientH]->m_dwLogoutHackCheck) > 10000) {
								RequestTeleportHandler(iClientH, "1 ");
							} else {
								char cCannotRecall[256];
								wsprintf(cCannotRecall, "Wait 10 seconds after get DMG to Recall.");
								ShowClientMsg(iClientH, cCannotRecall);
								goto MAGIC_NOEFFECT;
							}
						}
						break;
				}
				break;

			case DEF_MAGICTYPE_SUMMON:



				if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true) return;

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

				if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {

					iFollowersNum = iGetFollowerNumber(sOwnerH, cOwnerType);


					if (iFollowersNum >= (m_pClientList[iClientH]->m_cSkillMastery[4] / 20)) break;

					iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
					if (iNamingValue == -1) {

					} else {

						std::memset(cName, 0, sizeof (cName));
						wsprintf(cName, "XX%d", iNamingValue);
						cName[0] = '_';
						cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;


						std::memset(cNpcName, 0, sizeof (cNpcName));

						switch (iV1) {
							case 0:
								iResult = iDice(1, m_pClientList[iClientH]->m_cSkillMastery[4] / 10);


								if (iResult < m_pClientList[iClientH]->m_cSkillMastery[4] / 20)
									iResult = m_pClientList[iClientH]->m_cSkillMastery[4] / 20;

								switch (iResult) {
									case 1: strcpy(cNpcName, "Slime");
										break;
									case 2: strcpy(cNpcName, "Giant-Ant");
										break;
									case 3: strcpy(cNpcName, "Amphis");
										break;
									case 4: strcpy(cNpcName, "Orc");
										break;
									case 5: strcpy(cNpcName, "Skeleton");
										break;
									case 6: strcpy(cNpcName, "Clay-Golem");
										break;
									case 7: strcpy(cNpcName, "Stone-Golem");
										break;
									case 8: strcpy(cNpcName, "Orc-Mage");
										break;
									case 9: strcpy(cNpcName, "Hellbound");
										break;
									case 10:strcpy(cNpcName, "Cyclops");
										break;
								}
								break;

							case 1: strcpy(cNpcName, "Orc");
								break;
							case 2: strcpy(cNpcName, "Skeleton");
								break;
							case 3: strcpy(cNpcName, "Clay-Golem");
								break;
							case 4: strcpy(cNpcName, "Stone-Golem");
								break;
							case 5: strcpy(cNpcName, "Hellbound");
								break;
							case 6: strcpy(cNpcName, "Cyclops");
								break;
							case 7: strcpy(cNpcName, "Troll");
								break;
							case 8: strcpy(cNpcName, "Orge");
								break;

							case 9: strcpy(cNpcName, "Sor-Aresden");
								break;
							case 10: strcpy(cNpcName, "ATK-Elvine");
								break;
							case 11: strcpy(cNpcName, "Elf-Aresden");
								break;
							case 12: strcpy(cNpcName, "DSK-Elvine");
								break;
							case 13: strcpy(cNpcName, "HBT-Aresden");
								break;
							case 14: strcpy(cNpcName, "Bar-Elvine");
								break;

						}

						if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypoint, nullptr, 0, m_pClientList[iClientH]->m_cSide, false, true) == false) {

							m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
						} else {
							std::memset(cName_Master, 0, sizeof (cName_Master));
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									memcpy(cName_Master, m_pClientList[sOwnerH]->m_cCharName, 10);
									break;
								case DEF_OWNERTYPE_NPC:
									memcpy(cName_Master, m_pNpcList[sOwnerH]->m_cName, 5);
									break;
							}
							bSetNpcFollowMode(cName, cName_Master, cOwnerType);
						}
					}
				}
				break;

			case DEF_MAGICTYPE_CREATE:



				if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetIsMoveAllowedTile(dX, dY) == false)
					goto MAGIC_NOEFFECT;

				pItem = new class CItem;

				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						// Food
						if (iDice(1, 2) == 1)
							wsprintf(cItemName, "Meat");
						else wsprintf(cItemName, "Baguette");
						break;
				}

				_bInitItemAttr(*pItem, cItemName);


				pItem->m_sTouchEffectType = DEF_ITET_ID;
				pItem->m_sTouchEffectValue1 = iDice(1, 100000);
				pItem->m_sTouchEffectValue2 = iDice(1, 100000);
				pItem->m_sTouchEffectValue3 = (short) timeGetTime();


				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);


				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, pItem);


				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
						  dX, dY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
				break;

			case DEF_MAGICTYPE_PROTECT:

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);


				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] != 0) goto MAGIC_NOEFFECT;

						if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

						m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
						switch (m_pMagicConfigList[sType]->m_sValue4) {
							case 1:
								SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
								break;
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
						if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] != 0) goto MAGIC_NOEFFECT;

						if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
						m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;

						switch (m_pMagicConfigList[sType]->m_sValue4) {
							case 1:
								SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
								break;
						}
						break;
				}


				delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
						  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);


				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
					SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_PROTECT, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				break;

				/*					// Magictype:	Protection from Magic, Protection from Arrow, Defense Shield
						//				Absolue Magic Protection, Great Defense Shield
						case DEF_MAGICTYPE_PROTECT:
							// iClientH (the caster) gets the spell targets (sOwnerH) type (npc/pc) and coords (x,y)
							m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
							// switch cases for the owner type (DEF_OWNERTYPE_PLAYER, DEF_OWNERTYPE_NPC)
							switch (cOwnerType) {
							// if the owners is a player
							case DEF_OWNERTYPE_PLAYER:
								// DISABLED: if the caster side isnt the same as the targets side, no effect occurs
								// DISABLED: if (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;
								// if the player target doesnt exist no magic effect
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								// if the player target already has a protect magic on, no magic effect occurs
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != false) goto MAGIC_NOEFFECT;
								// if the caster is neutral go to no magic effect
								if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
								// switch cases for the effect4 of the spell
								switch (m_pMagicConfigList[sType]->m_sValue4){
									// Protection From Arrow
									case 1:
										// triggers the spell target's protection from arrow flag on
										SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
										break; // end case 1
									// Protection From Magic
									// Absolue Magic Protection
									case 2:
									case 5:
										// triggers the spell target's protection from magic flag on
										SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
										break; // end case 2, case 5
									// Defense Shield
									// Great Defense Shield
									case 3:
									case 4:
										// triggers the spells target's defense shield flag on
										SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
										break; // end case 3, case 4
								} // end switch (m_pMagicConfigList[sType]->m_sValue4){
								// the target's magic effect is set to the effect4 value of the spell cast on him
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char)m_pMagicConfigList[sType]->m_sValue4;
								break; // break; DEF_OWNERTYPE_PLAYER

							case DEF_OWNERTYPE_NPC:
								// if the npc target doesnt exist no magic effect
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								// if the npc target already has a protect magic on, no magic effect occurs
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] != 0) goto MAGIC_NOEFFECT;
								// if the npc's action limit is not 0, no magic effect occurs
								if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
								// switch cases for the effect4 of the spell
								switch (m_pMagicConfigList[sType]->m_sValue4){
									// Protection From Arrow
									case 1:
										// triggers the spell target's protection from arrow flag on
										SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
										break; // end case 1
									// Protection From Magic
									// Absolue Magic Protection
									case 2:
									case 5:
										// triggers the spell target's protection from magic flag on
										SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
										break; // end case 2, case 5
									// Defense Shield
									// Great Defense Shield
									case 3:
									case 4:
										// triggers the spells target's defense shield flag on
										SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
										break; // end case 3, case 4
								} // end switch (m_pMagicConfigList[sType]->m_sValue4){
								// the npc's magic effect is set to the effect4 value of the spell cast on it
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char)m_pMagicConfigList[sType]->m_sValue4;
								break;
							} // end switch (cOwnerType) {
							// the DEF_MAGICTYPE_PROTECT effect leaves after the magic effects "Last" time (in seconds)
							delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime*1000),
													 sOwnerH, cOwnerType, nullptr, nullptr, nullptr, m_pMagicConfigList[sType]->m_sValue4, nullptr, nullptr);
							// if the owner is a player send a message
							if (cOwnerType == DEF_OWNERTYPE_PLAYER)
								SendNotifyMsg(nullptr, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_PROTECT, m_pMagicConfigList[sType]->m_sValue4, nullptr, nullptr);
							break; // end case DEF_MAGICTYPE_PROTECT:*/

			case DEF_MAGICTYPE_SCAN:
				std::memset(cScanMessage, 0, sizeof (cScanMessage));
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							wsprintf(cScanMessage, " Player: %s || Health: %d || Mana: %d || Stamina: %d || Criticals: %d", m_pClientList[sOwnerH]->m_cCharName, m_pClientList[sOwnerH]->m_iHP, m_pClientList[sOwnerH]->m_iMP, m_pClientList[sOwnerH]->m_iSP, m_pClientList[sOwnerH]->m_iSuperAttackLeft);
							//ShowClientMsg(iClientH, cScanMessage);
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cScanMessage);
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							wsprintf(cScanMessage, " Monster: %s || Health: %d || Mana: %d", m_pNpcList[sOwnerH]->m_cNpcName, m_pNpcList[sOwnerH]->m_iHP, m_pNpcList[sOwnerH]->m_iMana);
							//   ShowClientMsg(iClientH, cScanMessage);
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cScanMessage);
							break;
					}
					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, 10, 10);
				}
				break;
			case DEF_MAGICTYPE_HOLDOBJECT:

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto MAGIC_NOEFFECT;

							if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

							if (cOwnerType == DEF_OWNERTYPE_PLAYER) {

								if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
								if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(dX, dY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
							}


							if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "middleland") != 0 &&
									  m_bIsCrusadeMode == false &&
									  m_pClientList[iClientH]->m_cSide == m_pClientList[sOwnerH]->m_cSide)
								goto MAGIC_NOEFFECT;

							m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto MAGIC_NOEFFECT;
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							break;
					}


					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);


					if (cOwnerType == DEF_OWNERTYPE_PLAYER)
						SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				}
				break;

			case DEF_MAGICTYPE_INVISIBILITY:
				//50Cent - Capture The flag
				if (bCheckIfIsFlagCarrier(iClientH)) {
					ShowClientMsg(iClientH, "You can not use that magic being a flag carrier.");
					goto MAGIC_NOEFFECT;
				}
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto MAGIC_NOEFFECT;
								if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetInvisibilityFlag(sOwnerH, cOwnerType, true);
								RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto MAGIC_NOEFFECT;

								if (m_pNpcList[sOwnerH]->m_cActionLimit == 0) {

									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) m_pMagicConfigList[sType]->m_sValue4;
									SetInvisibilityFlag(sOwnerH, cOwnerType, true);

									RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
								}
								break;
						}


						delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

						if (cOwnerType == DEF_OWNERTYPE_PLAYER)
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
						break;

					case 2:

						if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;


						for (ix = dX - 8; ix <= dX + 8; ix++)
							for (iy = dY - 8; iy <= dY + 8; iy++) {
								m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (sOwnerH != 0) {
									switch (cOwnerType) {
										case DEF_OWNERTYPE_PLAYER:
											if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
											if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
												SetInvisibilityFlag(sOwnerH, cOwnerType, false);
												delayEvents_.bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
											}
											break;

										case DEF_OWNERTYPE_NPC:
											if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
											if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
												SetInvisibilityFlag(sOwnerH, cOwnerType, false);
												delayEvents_.bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
											}
											break;
									}
								}
							}
						break;
				}
				break;

			case DEF_MAGICTYPE_CREATE_DYNAMIC:



				if (m_bIsCrusadeMode == false) {
					if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresden") == 0) return;
					if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvine") == 0) return;
					// v2.14
					if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresden") == 0) return;
					if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvine") == 0) return;
				}

				switch (m_pMagicConfigList[sType]->m_sValue10) {
					case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:

					case DEF_DYNAMICOBJECT_FIRE:
					case DEF_DYNAMICOBJECT_SPIKE: // Spike

#ifdef DEF_TAIWANLOG
						short sTemp_X, sTemp_Y;

						sTemp_X = m_pClientList[iClientH]->m_sX;
						sTemp_Y = m_pClientList[iClientH]->m_sY;

						m_pClientList[iClientH]->m_sX = dX;
						m_pClientList[iClientH]->m_sY = dY;

						_bItemLog(DEF_ITEMLOG_SPELLFIELD, iClientH, m_pMagicConfigList[sType]->m_cName, nullptr);

						m_pClientList[iClientH]->m_sX = sTemp_X;
						m_pClientList[iClientH]->m_sY = sTemp_Y;
#endif

						switch (m_pMagicConfigList[sType]->m_sValue11) {
							case 1:
								// wall - type
								cDir = m_Misc.cGetNextMoveDir(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY);
								switch (cDir) {
									case 1: rx = 1;
										ry = 0;
										break;
									case 2: rx = 1;
										ry = 1;
										break;
									case 3: rx = 0;
										ry = 1;
										break;
									case 4: rx = -1;
										ry = 1;
										break;
									case 5: rx = 1;
										ry = 0;
										break;
									case 6: rx = -1;
										ry = -1;
										break;
									case 7: rx = 0;
										ry = -1;
										break;
									case 8: rx = 1;
										ry = -1;
										break;
								}

								iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
										  dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000);

								bAnalyzeCriminalAction(iClientH, dX, dY);

								for (i = 1; i <= m_pMagicConfigList[sType]->m_sValue12; i++) {
									iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
											  dX + i*rx, dY + i*ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
									bAnalyzeCriminalAction(iClientH, dX + i*rx, dY + i * ry);

									iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
											  dX - i*rx, dY - i*ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
									bAnalyzeCriminalAction(iClientH, dX - i*rx, dY - i * ry);
								}
								break;

							case 2:
								// Field - Type
								bool bFlag = false;
								int cx, cy;
								for (ix = dX - m_pMagicConfigList[sType]->m_sValue12; ix <= dX + m_pMagicConfigList[sType]->m_sValue12; ix++)
									for (iy = dY - m_pMagicConfigList[sType]->m_sValue12; iy <= dY + m_pMagicConfigList[sType]->m_sValue12; iy++) {
										iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
												  ix, iy, m_pMagicConfigList[sType]->m_dwLastTime * 1000, m_pMagicConfigList[sType]->m_sValue5);


										if (bAnalyzeCriminalAction(iClientH, ix, iy, true) == true) {
											bFlag = true;
											cx = ix;
											cy = iy;
										}
									}

								if (bFlag == true) bAnalyzeCriminalAction(iClientH, cx, cy);
								break;
						}
						//
						break;

					case DEF_DYNAMICOBJECT_ICESTORM:
						// Ice-Storm Dynamic Object
						iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
								  dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000,
								  m_pClientList[iClientH]->m_cSkillMastery[4]);
						break;

					default:
						break;
				}
				break;

			case DEF_MAGICTYPE_POSSESSION:


				if (m_pClientList[iClientH]->m_cSide == 0) goto MAGIC_NOEFFECT;

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (sOwnerH != 0) break;

				pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(dX, dY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
				if (pItem != nullptr) {

					if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {



						_bItemLog(DEF_ITEMLOG_GET, iClientH, (int) - 1, pItem);

						dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
						*dwp = MSGID_NOTIFY;
						wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
						*wp = DEF_NOTIFY_ITEMOBTAINED;

						cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


						*cp = 1;
						cp++;

						memcpy(cp, pItem->m_cName, 20);
						cp += 20;

						dwp = (uint32_t *) cp;
						*dwp = pItem->m_dwCount;
						cp += 4;

						*cp = pItem->m_cItemType;
						cp++;

						*cp = pItem->m_cEquipPos;
						cp++;

						*cp = (char) 0;
						cp++;

						sp = (short *) cp;
						*sp = pItem->m_sLevelLimit;
						cp += 2;

						*cp = pItem->m_cGenderLimit;
						cp++;

						wp = (uint16_t *) cp;
						*wp = pItem->m_wCurLifeSpan;
						cp += 2;

						wp = (uint16_t *) cp;
						*wp = pItem->m_wWeight;
						cp += 2;

						sp = (short *) cp;
						*sp = pItem->m_sSprite;
						cp += 2;

						sp = (short *) cp;
						*sp = pItem->m_sSpriteFrame;
						cp += 2;

						*cp = pItem->m_cItemColor;
						cp++;

						*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
						cp++;

						dwp = (uint32_t *) cp;
						*dwp = pItem->m_dwAttribute;
						cp += 4;
						/*
						 *cp = (char)(pItem->m_dwAttribute & 0x00000001); 
						cp++;
						 */

						if (iEraseReq == 1) delete pItem;


						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
								  dX, dY, sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor); // v1.4


						iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);

						switch (iRet) {
							case DEF_XSOCKEVENT_QUENEFULL:
							case DEF_XSOCKEVENT_SOCKETERROR:
							case DEF_XSOCKEVENT_CRITICALERROR:
							case DEF_XSOCKEVENT_SOCKETCLOSED:

								DeleteClient(iClientH, true, true);
								return;
						}
					} else {



						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);

						dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
						*dwp = MSGID_NOTIFY;
						wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
						*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

						iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
						switch (iRet) {
							case DEF_XSOCKEVENT_QUENEFULL:
							case DEF_XSOCKEVENT_SOCKETERROR:
							case DEF_XSOCKEVENT_CRITICALERROR:
							case DEF_XSOCKEVENT_SOCKETCLOSED:

								DeleteClient(iClientH, true, true);
								return;
						}
					}
				}
				//
				break;

			case DEF_MAGICTYPE_CONFUSE:
				// if the caster side is the same as the targets side, no effect occurs
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
					case 2: // Confusion, Mass Confusion
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
							for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

								m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (cOwnerType == DEF_OWNERTYPE_PLAYER) {

									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] != 0) break;
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] = (char) m_pMagicConfigList[sType]->m_sValue4;


										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);


										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
									}
								}
							}
						break;

					case 3: // Ilusion, Mass-Ilusion
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
							for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

								m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (cOwnerType == DEF_OWNERTYPE_PLAYER) {

									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] != 0) break;
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] = (char) m_pMagicConfigList[sType]->m_sValue4;

										switch (m_pMagicConfigList[sType]->m_sValue4) {
											case 3:
												SetIllusionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
												break;
										}


										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);


										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, nullptr);
									}
								}
							}
						break;

					case 4: // Ilusion Movement
						if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) break;
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
							for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
								m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] != 0) break;
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] = (char) m_pMagicConfigList[sType]->m_sValue4;

										switch (m_pMagicConfigList[sType]->m_sValue4) {
											case 4:
												//para que los del mismo pueblo no se den con illusion movement by KahBur
												if ((strcmp(m_pClientList[sOwnerH]->m_cLocation, m_pClientList[iClientH]->m_cLocation)) == 0) goto MAGIC_NOEFFECT;
												if ((m_pClientList[iClientH]->m_bIsPlayerCivil == false) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true)) goto MAGIC_NOEFFECT;
												SetIllusionMovementFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
												break;
										}

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, nullptr);
									}
								}
							}
						break;
				}
				break;

			case DEF_MAGICTYPE_POISON:

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

				if (m_pMagicConfigList[sType]->m_sValue4 == 1) {

					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

							if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;


							bAnalyzeCriminalAction(iClientH, dX, dY);

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

								if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false) {

									m_pClientList[sOwnerH]->m_bIsPoisoned = true;
									m_pClientList[sOwnerH]->m_iPoisonLevel = m_pMagicConfigList[sType]->m_sValue5;
									m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;
									// 05/06/2004 - Hypnotoad - poison aura appears when cast Poison
									SetPoisonFlag(sOwnerH, cOwnerType, true);

									SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pMagicConfigList[sType]->m_sValue5, 0, nullptr);
#ifdef DEF_TAIWANLOG
									_bItemLog(DEF_ITEMLOG_POISONED, sOwnerH, (char *) nullptr, nullptr);
#endif
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_iHP > 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

								if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false) {


								}
							}
							break;
					}
				} else if (m_pMagicConfigList[sType]->m_sValue4 == 0) {

					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

							if (m_pClientList[sOwnerH]->m_bIsPoisoned == true) {

								m_pClientList[sOwnerH]->m_bIsPoisoned = false;
								// 05/06/2004 - Hypnotoad - poison aura removed when cure cast
								SetPoisonFlag(sOwnerH, cOwnerType, false);

								SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, nullptr);
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							break;
					}
				}
				break;

			case DEF_MAGICTYPE_BERSERK:
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] != 0) goto MAGIC_NOEFFECT;
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetBerserkFlag(sOwnerH, cOwnerType, true);
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] != 0) goto MAGIC_NOEFFECT;

								if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;

								if (m_pClientList[iClientH]->m_cSide != m_pNpcList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;

								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetBerserkFlag(sOwnerH, cOwnerType, true);
								break;
						}


						delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

						if (cOwnerType == DEF_OWNERTYPE_PLAYER)
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
						break;
				}
				break;


			case DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK:
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
						}

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
								ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
							}
						}
					}
				break;
				/*						// Resurrection Magic.
								case DEF_MAGICTYPE_RESURRECTION:
									// 10 Mins once
									if (m_pClientList[iClientH]->m_iSpecialAbilityTime != 0) goto MAGIC_NOEFFECT;
									m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC/2;
									// Get the ID of the dead Player/NPC on coords dX, dY.
									m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
									switch (cOwnerType) {
												// For Player.
										case DEF_OWNERTYPE_PLAYER:
											// The Player has to exist.
											if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
											// Resurrection is not for alive Players.
											if (m_pClientList[sOwnerH]->m_bIsKilled == false) goto MAGIC_NOEFFECT;
											// Set Deadflag to Alive.
											m_pClientList[sOwnerH]->m_bIsKilled = false;
											// Player's HP becomes half of the Max HP.
											m_pClientList[sOwnerH]->m_iHP = ((m_pClientList[sOwnerH]->m_iLevel * 2) + (m_pClientList[sOwnerH]->m_iVit * 3) + (m_pClientList[sOwnerH]->m_iStr / 2)) / 2;
											// Send new HP to Player.
											SendNotifyMsg(nullptr, sOwnerH, DEF_NOTIFY_HP, nullptr, nullptr, nullptr, nullptr);
											// Make Player stand up. (Currently, by a fake damage).
											m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->ClearDeadOwner(dX, dY);
											m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->SetOwner(sOwnerH, DEF_OWNERTYPE_PLAYER, dX, dY);
											SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, nullptr, nullptr, nullptr);
											SendNotifyMsg(nullptr, sOwnerH, DEF_NOTIFY_HP, nullptr, nullptr, nullptr, nullptr);
											break;
											// Resurrection is not for NPC's.
										case DEF_OWNERTYPE_NPC:
											goto MAGIC_NOEFFECT;
											break;
									}
									break;*/

			case DEF_MAGICTYPE_ICE:
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

							//Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;

									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

											SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;

								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);

											delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}

						}

						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

								//Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);

										delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

										SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
							}
						}
					}
				break;

			default:
				break;
		}
	} else {

		// Resurrection wand(MS.10) or Resurrection wand(MS.20)

		if (m_pMagicConfigList[sType]->m_sType == DEF_MAGICTYPE_RESURRECTION) {
			//Check if player has resurrection wand
			if (m_pClientList[iClientH] != nullptr && m_pClientList[iClientH]->m_iSpecialAbilityTime == 0 &&
					  m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == false) {
				m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (m_pClientList[sOwnerH] != nullptr) {
					// GM's can ressurect ne1, and players must be on same side to ressurect
					/*if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]]->m_sIDnum != 865) ||
						(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]]->m_sIDnum != 866)) {
						wsprintf(G_cTxt, "(!!!) Hack: Player(%s) cast resurrect without wand!", m_pClientList[iClientH]->m_cCharName);
						PutLogList(G_cTxt);
						DeleteClient(iClientH, true, true, true, true);
						return;
					}*/


					if ((m_pClientList[iClientH]->m_iAdminUserLevel < 1) &&
							  (m_pClientList[sOwnerH]->m_cSide != m_pClientList[iClientH]->m_cSide)) {
						return;
					}
					if (cOwnerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sOwnerH] != nullptr &&
							  m_pClientList[sOwnerH]->m_iHP <= 0) {
						m_pClientList[sOwnerH]->m_bIsBeingResurrected = true;
						SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_RESURRECTPLAYER, 0, 0, 0, nullptr);
						if (m_pClientList[iClientH]->m_iAdminUserLevel < 2) {
							m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = true;
							m_pClientList[iClientH]->m_dwSpecialAbilityStartTime = dwTime;
							m_pClientList[iClientH]->m_iSpecialAbilityLastSec = 0;
							m_pClientList[iClientH]->m_iSpecialAbilityTime = m_pMagicConfigList[sType]->m_dwDelayTime;

							sTemp = m_pClientList[iClientH]->m_sAppr4;
							sTemp = 0xFF0F & sTemp;
							sTemp = sTemp | 0x40;
							m_pClientList[iClientH]->m_sAppr4 = sTemp;
						}
						SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityLastSec, nullptr);
						SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}
				}
			}
		}
	}

MAGIC_NOEFFECT:
	;

	if (m_pClientList[iClientH] == nullptr) return;

	//Mana Slate
	if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0) {
		iManaCost = 0;
	}


	m_pClientList[iClientH]->m_iMP -= iManaCost;
	if (m_pClientList[iClientH]->m_iMP < 0)
		m_pClientList[iClientH]->m_iMP = 0;

	CalculateSSN_SkillIndex(iClientH, 4, 1);

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, nullptr);


	SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
			  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, (sType + 100), m_pClientList[iClientH]->m_sType);

}

void CGame::NpcMagicHandler(int iNpcH, short dX, short dY, short sType) {
	short sOwnerH;
	char cOwnerType;
	register int i;
	int iErr, ix, iy, sX, sY, tX, tY, iResult, iWhetherBonus, iMagicAttr;
	uint32_t dwTime = timeGetTime();

	if (m_pNpcList[iNpcH] == nullptr) return;
	if ((dX < 0) || (dX >= m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeX) ||
			  (dY < 0) || (dY >= m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeY)) return;

	if ((sType < 0) || (sType >= 100)) return;
	if (m_pMagicConfigList[sType] == nullptr) return;

	// �����ġ�� ��� �Ұ��� ���̶�� ��� �Ұ���
	if (m_pMapList[ m_pNpcList[iNpcH]->m_cMapIndex ]->m_bIsAttackEnabled == false) return;

	// ���� �� ���� ���߷� �Է�
	iResult = m_pNpcList[iNpcH]->m_iMagicHitRatio;

	// ������ ���� ���� ��ݷ� ����
	iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cWhetherStatus);

	// v1.41 ���� �Ӽ�
	iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;

	if (m_pMagicConfigList[sType]->m_dwDelayTime == 0) {
		// ��� ȿ�� ���� ����
		switch (m_pMagicConfigList[sType]->m_sType) {
			case DEF_MAGICTYPE_INVISIBILITY:
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						// ������ �ʴ� ���·� �����.
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto NMH_NOEFFECT;
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetInvisibilityFlag(sOwnerH, cOwnerType, true);
								// �� ĳ���͸� �����ϰ� �ִ� ���͸� ������Ų��.
								RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER);
								break;

							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto NMH_NOEFFECT;
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetInvisibilityFlag(sOwnerH, cOwnerType, true);
								// �� NPC�� �����ϰ� �ִ� ���͸� ������Ų��.
								RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC);
								break;
						}

						// ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
						delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

						if (cOwnerType == DEF_OWNERTYPE_PLAYER)
							SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
						break;

					case 2:
						// dX, dY �ݰ� 8 �ֺ��� Invisibility ������ Object�� ������ ���� ��Ų��.
						for (ix = dX - 8; ix <= dX + 8; ix++)
							for (iy = dY - 8; iy <= dY + 8; iy++) {
								m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (sOwnerH != 0) {
									switch (cOwnerType) {
										case DEF_OWNERTYPE_PLAYER:
											if (m_pClientList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
											if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												if (m_pClientList[sOwnerH]->m_sType != 66) {
													m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
													SetInvisibilityFlag(sOwnerH, cOwnerType, false);
													delayEvents_.bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
												}
											}
											break;

										case DEF_OWNERTYPE_NPC:
											if (m_pNpcList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
											if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												if (m_pClientList[sOwnerH]->m_sType != 66) {
													m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
													SetInvisibilityFlag(sOwnerH, cOwnerType, false);
													delayEvents_.bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
												}
											}
											break;
									}
								}
							}
						break;
				}
				break;

			case DEF_MAGICTYPE_HOLDOBJECT:
				// ������Ʈ�� �������� �����Ѵ�.
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {

					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto NMH_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto NMH_NOEFFECT;
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto NMH_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto NMH_NOEFFECT;
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							break;
					}

					// ���� ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

					// ȿ�� ������� �˷��ش�.
					if (cOwnerType == DEF_OWNERTYPE_PLAYER)
						SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				}
				break;


			case DEF_MAGICTYPE_DAMAGE_LINEAR:
				// ������ �� �ִ� ��ǥ�� ��� ����Ѵ�.
				sX = m_pNpcList[iNpcH]->m_sX;
				sY = m_pNpcList[iNpcH]->m_sY;

				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

					// tx, ty
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx-1, ty
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx+1, ty
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx, ty-1
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					// tx, ty+1
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {

						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}

					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}

				// �ֺ� ��� ȿ��
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// �ڽŵ� ����� �� ������ ����.
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}

				// dX, dY
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
				}
				break;

			case DEF_MAGICTYPE_DAMAGE_SPOT:
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}
				break;

			case DEF_MAGICTYPE_HPUP_SPOT:
				// �� ������ ���߷��� ����� ���.
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				Effect_HpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				break;

			case DEF_MAGICTYPE_PURITYLIFE_SPOT:
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				PURITYLIFE(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				break;

			case DEF_MAGICTYPE_DAMAGE_AREA:
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {

					if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}

				// �ֺ� ��� ȿ��
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// �ڽŵ� ����� �� ������ ����.
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;

			case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:

				// �ֺ� ��� ȿ��
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// �ڽŵ� ����� �� ������ ����.
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {

							if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;

			case DEF_MAGICTYPE_SPDOWN_AREA:
				// Sp�� �پ���.
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				// �ֺ� ��� ȿ��
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// �ڽŵ� ����� �� ������ ����.
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
				break;

			case DEF_MAGICTYPE_SPUP_AREA:
				// Sp�� ����Ѵ�.
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				// ���� ������ �ʿ���.
				Effect_SpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				// �ֺ� ��� ȿ��
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						// �ڽŵ� ����� �� ������ ����.
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						// ���������� �ʿ� ���.
						Effect_SpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
				break;

		}
	} else {
		// Casting �� �����̰� �ɸ��� ����

	}

NMH_NOEFFECT:
	;

	// Mana�� ���ҽ�Ų��.
	m_pNpcList[iNpcH]->m_iMana -= m_pMagicConfigList[sType]->m_sValue1; // sValue1�� Mana Cost
	if (m_pNpcList[iNpcH]->m_iMana < 0)
		m_pNpcList[iNpcH]->m_iMana = 0;

	// ���� ȿ�� �ٸ� Ŭ���̾�Ʈ���� ����Ѵ�. �����ȣ + 100�� ����Ʈ ��ȣ
	SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pNpcList[iNpcH]->m_cMapIndex,
			  m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, dX, dY, (sType + 100), m_pNpcList[iNpcH]->m_sType);

}

enum class TeleportType {
	TYPE_0, // Forced
	TYPE_1, // Recall
	TYPE_2 // ??
};

void CGame::RequestTeleportHandler(int iClientH, const char * pData, const char * cMapName, int dX, int dY) {
	char * pBuffer, cTempMapName[21];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cDestMapName[11], cDir, cMapIndex, cQuestRemain;
	short * sp, sX, sY, sSummonPoints;
	int * ip, i, iRet, iSize, iDestX, iDestY, iExH, iMapSide;
	bool bRet, bIsLockedMapNotify;
	SYSTEMTIME SysTime;

	m_pClientList[iClientH]->m_dwLastActionTime = m_pClientList[iClientH]->m_dwAFKCheckTime = timeGetTime();
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsOnWaitingProcess == true) return;
	if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsRecallImpossible == true) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0) &&
			  (m_pClientList[iClientH]->m_bIsKilled == false) && (m_pClientList[iClientH]->m_iHP > 0)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_NORECALL, 0, 0, 0, nullptr);
		return;
	}

	if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elvine", 6) == 0)
			  && (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0)
			  && (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)
			  && ((pData[0] == '1'))
			  && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
			  && (m_bIsCrusadeMode == false)) return;


	if ((memcmp(m_pClientList[iClientH]->m_cLocation, "aresden", 7) == 0)
			  && (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0)
			  && (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)
			  && ((pData[0] == '1'))
			  && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
			  && (m_bIsCrusadeMode == false)) return;

	bIsLockedMapNotify = false;


	if (m_pClientList[iClientH]->m_bIsExchangeMode == true) {
		iExH = m_pClientList[iClientH]->m_iExchangeH;
		_ClearExchangeStatus(iExH);
		_ClearExchangeStatus(iClientH);
	}



	if ((memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) && (pData[0] == '1'))
		return;


	RemoveFromTarget(iClientH, DEF_OWNERTYPE_PLAYER);


	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(13, iClientH, DEF_OWNERTYPE_PLAYER,
			  m_pClientList[iClientH]->m_sX,
			  m_pClientList[iClientH]->m_sY);


	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);







	sX = m_pClientList[iClientH]->m_sX;
	sY = m_pClientList[iClientH]->m_sY;

	std::memset(cDestMapName, 0, sizeof (cDestMapName));
	bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSearchTeleportDest(sX, sY, cDestMapName, &iDestX, &iDestY, &cDir);

	// Crusade
	if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0)) {


		iMapSide = iGetMapLocationSide(cDestMapName);
		if (iMapSide > 3) iMapSide -= 2; // New 18/05/2004
		if ((iMapSide != 0) && (m_pClientList[iClientH]->m_cSide == iMapSide)) {

		} else {
			iDestX = -1;
			iDestY = -1;
			bIsLockedMapNotify = true;
			std::memset(cDestMapName, 0, sizeof (cDestMapName));
			strcpy(cDestMapName, m_pClientList[iClientH]->m_cLockedMapName);
		}
	}

	if ((bRet == true) && (cMapName == nullptr)) {

		for (i = 0; i < DEF_MAXMAPS; i++)
			if (m_pMapList[i] != nullptr) {
				if (memcmp(m_pMapList[i]->m_cName, cDestMapName, 10) == 0) {
					if (pData[0] == '3' && m_pClientList[iClientH]->m_iAdminUserLevel == 0 && (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0)) {
						std::vector<std::string> whitelist;
						if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elvine", 6) == 0)
								  && (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)) {
							// Elv in ares
							whitelist = {"middleland", "huntzone2", "aresdend1", "arefarm"};
						}
						if ((memcmp(m_pClientList[iClientH]->m_cLocation, "aresden", 7) == 0)
								  && (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)) {
							// Ares in elv
							whitelist = {"middleland", "huntzone1", "elvined1", "elvfarm"};
						}
						if (!whitelist.empty()) {
							std::string target = cDestMapName;
							if (std::find(whitelist.begin(), whitelist.end(), target) == whitelist.end()) {
								return;
							}
						}
					}

					m_pClientList[iClientH]->m_sX = iDestX;
					m_pClientList[iClientH]->m_sY = iDestY;
					m_pClientList[iClientH]->m_cDir = cDir;
					m_pClientList[iClientH]->m_cMapIndex = i;
					std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
					memcpy(m_pClientList[iClientH]->m_cMapName, m_pMapList[i]->m_cName, 10);
					goto RTH_NEXTSTEP;
				}
			}


		m_pClientList[iClientH]->m_sX = iDestX;
		m_pClientList[iClientH]->m_sY = iDestY;
		m_pClientList[iClientH]->m_cDir = cDir;
		std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
		memcpy(m_pClientList[iClientH]->m_cMapName, cDestMapName, 10);

		// New 18/05/2004
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
				  m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
		SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);


		bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);
		// !!!!
		m_pClientList[iClientH]->m_bIsOnServerChange = true;
		m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
		return;
	} else {

		switch (pData[0]) {
			case '0':
				// Forced Recall.

				std::memset(cTempMapName, 0, sizeof (cTempMapName));
				if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) {
					strcpy(cTempMapName, "default");
				} else if (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0) {
					strcpy(cTempMapName, "arefarm");
				} else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0) {
					strcpy(cTempMapName, "elvfarm");
				} else strcpy(cTempMapName, m_pClientList[iClientH]->m_cLocation);

				// Crusade
				if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0)) {

					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, m_pClientList[iClientH]->m_cLockedMapName);
				}

				for (i = 0; i < DEF_MAXMAPS; i++)
					if (m_pMapList[i] != nullptr) {
						if (memcmp(m_pMapList[i]->m_cName, cTempMapName, 10) == 0) {

							GetMapInitialPoint(i, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cLocation);

							m_pClientList[iClientH]->m_cMapIndex = i;
							std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
							memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);
							goto RTH_NEXTSTEP;
						}
					}


				m_pClientList[iClientH]->m_sX = -1;
				m_pClientList[iClientH]->m_sY = -1;

				std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
				memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);

				// New 18/05/2004
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
						  m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
				SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);


				bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);

				m_pClientList[iClientH]->m_bIsOnServerChange = true;
				m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
				return;

			case '1':


				// if (memcmp(m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_cName, "resurr", 6) == 0) return;

				std::memset(cTempMapName, 0, sizeof (cTempMapName));
				if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) {
					strcpy(cTempMapName, "default");
				} else {

					if (m_pClientList[iClientH]->m_iLevel > 80)
						if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
							strcpy(cTempMapName, "aresden");
						else strcpy(cTempMapName, "elvine");
					else {
						if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
							strcpy(cTempMapName, "aresden");
						else strcpy(cTempMapName, "elvine");
					}
				}
				// Crusade
				if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0)) {

					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, m_pClientList[iClientH]->m_cLockedMapName);
				}

				for (i = 0; i < DEF_MAXMAPS; i++)
					if (m_pMapList[i] != nullptr) {
						if (memcmp(m_pMapList[i]->m_cName, cTempMapName, 10) == 0) {


							GetMapInitialPoint(i, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cLocation);

							m_pClientList[iClientH]->m_cMapIndex = i;
							std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
							memcpy(m_pClientList[iClientH]->m_cMapName, m_pMapList[i]->m_cName, 10);
							goto RTH_NEXTSTEP;
						}
					}


				m_pClientList[iClientH]->m_sX = -1;
				m_pClientList[iClientH]->m_sY = -1;

				std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
				memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);

				// New 18/05/2004
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
						  m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
				SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);


				bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);
				// !!!
				m_pClientList[iClientH]->m_bIsOnServerChange = true;
				m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
				return;

			case '2':


				// Crusade
				if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0)) {

					dX = -1;
					dY = -1;
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, m_pClientList[iClientH]->m_cLockedMapName);
				} else {
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, cMapName);
				}

				cMapIndex = iGetMapIndex(cTempMapName);
				if (cMapIndex == -1) {


					m_pClientList[iClientH]->m_sX = dX;
					m_pClientList[iClientH]->m_sY = dY;

					std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
					memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);


					// New 18/05/2004
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
							  m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
					SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);

					bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);
					// !!!
					m_pClientList[iClientH]->m_bIsOnServerChange = true;
					m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
					return;
				}

				m_pClientList[iClientH]->m_sX = dX;
				m_pClientList[iClientH]->m_sY = dY;
				m_pClientList[iClientH]->m_cMapIndex = cMapIndex;

				std::memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof (m_pClientList[iClientH]->m_cMapName));
				memcpy(m_pClientList[iClientH]->m_cMapName, m_pMapList[cMapIndex]->m_cName, 10);
				break;
		}
	}

RTH_NEXTSTEP:
	;

	// New 17/05/2004
	SetPlayingStatus(iClientH);
	int iTemp, iTemp2;
	iTemp = m_pClientList[iClientH]->m_iStatus;
	iTemp = 0x0FFFFFFF & iTemp;
	iTemp2 = iGetPlayerABSStatus(iClientH);
	iTemp = iTemp | (iTemp2 << 28);
	m_pClientList[iClientH]->m_iStatus = iTemp;

	// Crusade
	if (bIsLockedMapNotify == true) SendNotifyMsg(0, iClientH, DEF_NOTIFY_LOCKEDMAP, m_pClientList[iClientH]->m_iLockedMapTime, 0, 0, m_pClientList[iClientH]->m_cLockedMapName);

	pBuffer = new char [DEF_MSGBUFFERSIZE + 1];
	ZeroMemory(pBuffer, DEF_MSGBUFFERSIZE + 1);


	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITDATA;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;

	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);


	if (m_pClientList[iClientH]->m_bIsObserverMode == false)
		bGetEmptyPosition(&m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cMapIndex);
	else GetMapInitialPoint(m_pClientList[iClientH]->m_cMapIndex, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY);


	sp = (short *) cp;
	*sp = iClientH; // Player ObjectID
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sX - 14 - 5;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sY - 12 - 5;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sType;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr1;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr2;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr3;
	cp += 2;

	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sAppr4;
	cp += 2;
	// v1.4 ApprColor
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iApprColor;
	cp += 4;

	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iStatus;
	cp += 4; //Original 2


	memcpy(cp, m_pClientList[iClientH]->m_cMapName, 10);
	cp += 10;


	memcpy(cp, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10);
	cp += 10;


	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
		*cp = 1;
	else *cp = m_cDayOrNight;
	cp++;


	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
		*cp = 0;
	else *cp = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus;
	cp++;

	// v1.4 Contribution
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iContribution;
	cp += 4;


	if (m_pClientList[iClientH]->m_bIsObserverMode == false) {
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH,
				  DEF_OWNERTYPE_PLAYER,
				  m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY);
	}

	// v1.41
	*cp = (char) m_pClientList[iClientH]->m_bIsObserverMode;
	cp++;

	// v1.41
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iRating;
	cp += 4;

	// v1.44
	ip = (int *) cp;
	*ip = m_pClientList[iClientH]->m_iHP;
	cp += 4;
	//Unknown variable
	*cp = 0;
	cp++;


	iSize = iComposeInitMapData(m_pClientList[iClientH]->m_sX - 10, m_pClientList[iClientH]->m_sY - 7, iClientH, cp);

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 46 + iSize + 4 + 4 + 1 + 4 + 4 + 3); //Zabuza fix
	//iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(pBuffer, 46 + iSize +4 +4 +1 +4 +4); // v1.41
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			DeleteClient(iClientH, true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}

	if (pBuffer != nullptr) delete pBuffer;


	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);

	if ((memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) &&
			  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0) &&
			  (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {

		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
		m_pClientList[iClientH]->m_bIsWarLocation = true;
		// New 17/05/2004
		CheckForceRecallTime(iClientH);
	} else if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0) &&
			  (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0) &&
			  (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {

		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
		m_pClientList[iClientH]->m_bIsWarLocation = true;

		// New 17/05/2004
		CheckForceRecallTime(iClientH);
	} else if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true) {
		m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
		m_pClientList[iClientH]->m_bIsWarLocation = true;
		SetForceRecallTime(iClientH);

		GetLocalTime(&SysTime);
		m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 20 * 60 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;

	} else {
		m_pClientList[iClientH]->m_bIsWarLocation = false;
		m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 0;
		SetForceRecallTime(iClientH);
	}

	// No entering enemy shops
	int iMapside, iMapside2;

	iMapside = iGetMapLocationSide(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName);
	if (iMapside > 3) iMapside2 = iMapside - 2;
	else iMapside2 = iMapside;
	m_pClientList[iClientH]->m_bIsInsideOwnTown = false;
	if ((m_pClientList[iClientH]->m_cSide != iMapside2) && (iMapside != 0)) {
		if ((iMapside <= 2) && (m_pClientList[iClientH]->m_iAdminUserLevel < 1)) {
			if (m_pClientList[iClientH]->m_cSide != 0) {
				m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
				m_pClientList[iClientH]->m_bIsWarLocation = true;
				m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 1;
				m_pClientList[iClientH]->m_bIsInsideOwnTown = true;
			}
		}
	} else {
		if (m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_bIsFightZone == true &&
				  m_iFightzoneNoForceRecall == false &&
				  m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
			m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
			m_pClientList[iClientH]->m_bIsWarLocation = true;
			GetLocalTime(&SysTime);
			m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
		} else {
			if (memcmp(m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_cLocationName, "arejail", 7) == 0 ||
					  memcmp(m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_cLocationName, "elvjail", 7) == 0) {
				if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
					m_pClientList[iClientH]->m_bIsWarLocation = true;
					m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
					if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
						m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
					else if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 100)
						m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
				}
			}
		}
	}


	SendNotifyMsg(0, iClientH, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, nullptr);
	// v1.3
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, 0, 0, nullptr);
	// V1.3
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMPOSLIST, 0, 0, 0, nullptr);
	// v1.4
	_SendQuestContents(iClientH);
	_CheckQuestEnvironment(iClientH);

	// v1.432
	if (m_pClientList[iClientH]->m_iSpecialAbilityTime == 0)
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, nullptr);


	if (m_bIsCrusadeMode == true) {
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == 0) {

			m_pClientList[iClientH]->m_iCrusadeDuty = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
		} else if (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwCrusadeGUID) {



			m_pClientList[iClientH]->m_iCrusadeDuty = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			m_pClientList[iClientH]->m_iWarContribution = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;

			SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
		}
		m_pClientList[iClientH]->m_cVar = 1;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, 0, nullptr);
	} else if (m_bIsHeldenianMode == true) {
		sSummonPoints = m_pClientList[iClientH]->m_iCharisma * 300;
		if (sSummonPoints > DEF_MAXSUMMONPOINTS) sSummonPoints = DEF_MAXSUMMONPOINTS;
		if (m_pClientList[iClientH]->m_dwHeldenianGUID == 0) {
			m_pClientList[iClientH]->m_dwHeldenianGUID = m_dwHeldenianGUID;
			m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
		} else if (m_pClientList[iClientH]->m_dwHeldenianGUID != m_dwHeldenianGUID) {
			m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
			m_pClientList[iClientH]->m_iWarContribution = 0;
			m_pClientList[iClientH]->m_dwHeldenianGUID = m_dwHeldenianGUID;
		}
		m_pClientList[iClientH]->m_cVar = 2;
		if (m_bIsHeldenianMode == true) {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
		}
		if (m_bHeldenianInitiated == true) {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
		}
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, nullptr);
		UpdateHeldenianStatus();
	} else if ((m_pClientList[iClientH]->m_cVar == 1) && (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID)) {
		m_pClientList[iClientH]->m_iCrusadeDuty = 0;
		m_pClientList[iClientH]->m_iConstructionPoint = 0;
	} else {
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (m_pClientList[iClientH]->m_cVar == 1) {
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
			}
		} else {
			m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
			m_pClientList[iClientH]->m_iWarContribution = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
		}
	}
	/*
	if (m_bIsCrusadeMode == true) {
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == 0) {
			
			m_pClientList[iClientH]->m_iCrusadeDuty = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
		}
		else if (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwCrusadeGUID) {
			
			
			
			m_pClientList[iClientH]->m_iCrusadeDuty       = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			m_pClientList[iClientH]->m_iWarContribution   = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
			
			SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, nullptr, 0, nullptr, -1);
		}
		SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, nullptr, nullptr);
	}
	else {
		
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			m_pClientList[iClientH]->m_iCrusadeDuty = 0;
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
		}
		else if ((m_pClientList[iClientH]->m_dwCrusadeGUID != nullptr) && (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwCrusadeGUID)) {
			
			SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, nullptr, 0, nullptr, -1);
			m_pClientList[iClientH]->m_iWarContribution   = 0;
			m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
		}
	}*/

	// v1.42

	if (memcmp(m_pClientList[iClientH]->m_cMapName, "fight", 5) == 0) {
		wsprintf(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_bIsObserverMode);
		PutLogEventFileList(G_cTxt);
	}

	// Crusade
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 1, nullptr);

	// v2.15
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, nullptr);

	if (m_bIsHeldenianMode == true) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
		if (m_bHeldenianInitiated == true) {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
		} else {
			UpdateHeldenianStatus();
		}
	}

	if (m_pClientList[iClientH]->m_iQuest != 0) {
		cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount);
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, nullptr);
		_bCheckIsQuestCompleted(iClientH);
	}

}

void CGame::ReleaseFollowMode(short sOwnerH, char cOwnerType) {
	register int i;

	for (i = 0; i < DEF_MAXNPCS; i++)
		if ((i != sOwnerH) && (m_pNpcList[i] != nullptr)) {
			if ((m_pNpcList[i]->m_cMoveType == DEF_MOVETYPE_FOLLOW) &&
					  (m_pNpcList[i]->m_iFollowOwnerIndex == sOwnerH) &&
					  (m_pNpcList[i]->m_cFollowOwnerType == cOwnerType)) {

				m_pNpcList[i]->m_cMoveType = DEF_MOVETYPE_RANDOMWAYPOINT;
			}
		}
}

bool CGame::_bDecodeMagicConfigFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iMagicConfigListIndex = 0;
	class CStrTok * pStrTok;

	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	//token = strtok(pContents, seps);
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							if (m_pMagicConfigList[atoi(token)] != nullptr) {

								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Duplicate magic number.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[atoi(token)] = new class CMagic;
							iMagicConfigListIndex = atoi(token);

							cReadModeB = 2;
							break;

						case 2:

							std::memset(m_pMagicConfigList[iMagicConfigListIndex]->m_cName, 0, sizeof (m_pMagicConfigList[iMagicConfigListIndex]->m_cName));
							memcpy(m_pMagicConfigList[iMagicConfigListIndex]->m_cName, token, strlen(token));
							cReadModeB = 3;
							break;

						case 3:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sType = atoi(token);
							cReadModeB = 4;
							break;

						case 4:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_dwDelayTime = atoi(token);
							cReadModeB = 5;
							break;

						case 5:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_dwLastTime = atoi(token);
							cReadModeB = 6;
							break;

						case 6:
							// m_sValue1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue1 = atoi(token);
							cReadModeB = 7;
							break;

						case 7:
							// m_sValue2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue2 = atoi(token);
							cReadModeB = 8;
							break;

						case 8:
							// m_sValue3
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue3 = atoi(token);
							cReadModeB = 9;
							break;

						case 9:
							// m_sValue4
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue4 = atoi(token);
							cReadModeB = 10;
							break;

						case 10:
							// m_sValue5
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue5 = atoi(token);
							cReadModeB = 11;
							break;

						case 11:
							// m_sValue6
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue6 = atoi(token);
							cReadModeB = 12;
							break;

						case 12:
							// m_sValue7
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue7 = atoi(token);
							cReadModeB = 13;
							break;

						case 13:
							// m_sValue8
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue8 = atoi(token);
							cReadModeB = 14;
							break;

						case 14:
							// m_sValue9
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue9 = atoi(token);
							cReadModeB = 15;
							break;

						case 15:
							// m_sValue10
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue10 = atoi(token);
							cReadModeB = 16;
							break;

						case 16:
							// m_sValue11
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue11 = atoi(token);
							cReadModeB = 17;
							break;

						case 17:
							// m_sValue12
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sValue12 = atoi(token);
							cReadModeB = 18;
							break;

						case 18:
							// m_sIntLimit
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_sIntLimit = atoi(token);
							cReadModeB = 19;
							break;

						case 19:
							// m_iGoldCost
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_iGoldCost = atoi(token);

							cReadModeB = 20;
							break;

						case 20:
							// m_cCategory
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_cCategory = atoi(token);
							cReadModeB = 21;
							break;

						case 21:
							// m_iAttribute
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMagicConfigList[iMagicConfigListIndex]->m_iAttribute = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				default:
					break;
			}
		} else {
			if (memcmp(token, "magic", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

		}
		token = pStrTok->pGet();
		//token = strtok(nullptr, seps);
	}

	delete pStrTok;
	delete pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) MAGIC(Total:%d) configuration - success!", iMagicConfigListIndex);
	PutLogList(cTxt);

	return true;

}

bool CGame::_bDecodeSkillConfigFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iSkillConfigListIndex = 0;
	class CStrTok * pStrTok;

	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	//token = strtok(pContents, seps);
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							if (m_pSkillConfigList[atoi(token)] != nullptr) {

								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Duplicate magic number.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[atoi(token)] = new class CSkill;
							iSkillConfigListIndex = atoi(token);

							cReadModeB = 2;
							break;

						case 2:

							std::memset(m_pSkillConfigList[iSkillConfigListIndex]->m_cName, 0, sizeof (m_pSkillConfigList[iSkillConfigListIndex]->m_cName));
							memcpy(m_pSkillConfigList[iSkillConfigListIndex]->m_cName, token, strlen(token));
							cReadModeB = 3;
							break;

						case 3:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sType = atoi(token);
							cReadModeB = 4;
							break;

						case 4:
							// m_sValue1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sValue1 = atoi(token);
							cReadModeB = 5;
							break;

						case 5:
							// m_sValue2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sValue2 = atoi(token);
							cReadModeB = 6;
							break;

						case 6:
							// m_sValue3
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sValue3 = atoi(token);
							cReadModeB = 7;
							break;

						case 7:
							// m_sValue4
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sValue4 = atoi(token);
							cReadModeB = 8;
							break;

						case 8:
							// m_sValue5
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sValue5 = atoi(token);
							cReadModeB = 9;
							break;

						case 9:
							// m_sValue6
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pSkillConfigList[iSkillConfigListIndex]->m_sValue6 = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				default:
					break;
			}
		} else {
			if (memcmp(token, "skill", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

		}
		token = pStrTok->pGet();
		//token = strtok(nullptr, seps);
	}

	delete pStrTok;
	delete pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) SKILL(Total:%d) configuration - success!", iSkillConfigListIndex);
	PutLogList(cTxt);

	return true;
}

// 12-22 ���Ĵ� ����  ������⿡ ������ ������ Ŭ���̾�Ʈ�� �����ش�.

void CGame::RequestStudyMagicHandler(int iClientH, char * pName, bool bIsPurchase) {
	char * cp, cMagicName[31], cData[100];
	uint32_t * dwp, dwGoldCount;
	uint16_t * wp;
	int * ip, iReqInt, iCost, iRet;
	bool bMagic = true;
	int iTempInt;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	// ������ ����.
	std::memset(cData, 0, sizeof (cData));

	std::memset(cMagicName, 0, sizeof (cMagicName));
	memcpy(cMagicName, pName, 30);

	iRet = _iGetMagicNumber(cMagicName, &iReqInt, &iCost);
	if (iRet == -1) {


	} else {
		if (bIsPurchase == true) {
			if (m_pMagicConfigList[iRet]->m_iGoldCost < 0) bMagic = false; // �Ϲ������� ���� ��� �����̶��(������ ����) ��� �� ���.
			dwGoldCount = dwGetItemCount(iClientH, "Gold");
			if ((uint32_t) iCost > dwGoldCount) bMagic = false; // ���� �����ص� ��� �� ���.
		}

		if (m_pClientList[iClientH]->m_cMagicMastery[iRet] != 0) return;
		iTempInt = m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt;
		if ((iReqInt <= iTempInt) && (bMagic == true)) {

			// ���� ��������� �˸���.
			if (bIsPurchase == true) SetItemCount(iClientH, "Gold", dwGoldCount - iCost);

			// ����ǰ �� �߷� �� ���
			iCalcTotalWeight(iClientH);

			// ���� ��� �ɷ� ǥ�� .
			m_pClientList[iClientH]->m_cMagicMastery[iRet] = 1;

			// ������ ���ٴ� �޽����� ����Ѵ�.
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_MAGICSTUDYSUCCESS;

			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

			// ���� ��ȣ
			*cp = iRet;
			cp++;

			memcpy(cp, cMagicName, 30);
			cp += 30;

			// ���� ���
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 37);

			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
					DeleteClient(iClientH, true, true);
					return;
			}
		} else {
			// ������ �������� �ʾ� ������ ��� �� ���.
			// ������ ���� ���ٴ� �޽����� ����Ѵ�.
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_MAGICSTUDYFAIL;

			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			// ��������.
			*cp = 1;
			cp++;

			// ���� ��ȣ
			*cp = iRet;
			cp++;

			memcpy(cp, cMagicName, 30);
			cp += 30;

			ip = (int *) cp;
			*ip = iCost;
			cp += 4;

			ip = (int *) cp;
			*ip = iReqInt;
			cp += 4;

			// ���� ���
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 46);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
					DeleteClient(iClientH, true, true);
					return;
			}
		}
	}
}

int CGame::_iGetMagicNumber(char * pMagicName, int * pReqInt, int * pCost) {
	register int i;
	char cTmpName[31];

	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pMagicName);

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		if (m_pMagicConfigList[i] != nullptr) {
			if (memcmp(cTmpName, m_pMagicConfigList[i]->m_cName, 30) == 0) {

				*pReqInt = (int) m_pMagicConfigList[i]->m_sIntLimit;
				*pCost = (int) m_pMagicConfigList[i]->m_iGoldCost;

				return i;
			}
		}

	return -1;
}

void CGame::TrainSkillResponse(bool bSuccess, int iClientH, int iSkillNum, int iSkillLevel) {
	char * cp, cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((iSkillNum < 0) || (iSkillNum > 100)) return;
	if ((iSkillLevel < 0) || (iSkillLevel > 100)) return;

	if (bSuccess == true) {
		// �̹� ����� ���� ���¶�� �ҿ��� ���.
		if (m_pClientList[iClientH]->m_cSkillMastery[iSkillNum] != 0) return;

		m_pClientList[iClientH]->m_cSkillMastery[iSkillNum] = iSkillLevel;
		// ��� �� ���� �� ����Ѵ�.
		bCheckTotalSkillMasteryPoints(iClientH, iSkillNum);

		// ����� ���ٴ� �޽����� ����Ѵ�.
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_SKILLTRAINSUCCESS;

		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

		// ����ȣ
		*cp = iSkillNum;
		cp++;

		// ��� ���� .
		*cp = iSkillLevel;
		cp++;

		//wsprintf(G_cTxt, "Skill Improve: (%s) increased (%s) at %s (%d,%d).", m_pClientList[iClientH]->m_cCharName, m_pSkillConfigList[iSkillNum]->m_cName, m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
		//PutLogFileList(G_cTxt);

		if (m_pSkillConfigList[iSkillNum]->m_cName != nullptr)
			_bItemLog(DEF_ITEMLOG_SKILLLEARN, iClientH, m_pSkillConfigList[iSkillNum]->m_cName, nullptr);

		// ���� ���
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 8);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
				DeleteClient(iClientH, true, true);
				return;
		}
	} else {


	}

	/*
	// ����� ����.
	std::memset(cData, 0, sizeof(cData));

	std::memset(cSkillName, 0, sizeof(cSkillName));
	memcpy(cSkillName, pName, 20);

	iRet = _iGetSkillNumber(cSkillName);
	if (iRet == 0) {
		

	}
	else {
		

		if (1) {
			// @@ Test������ �ϴ� ������ ����� �����ϰ� �����.
			m_pClientList[iClientH]->m_cSkillMastery[iRet]++;

			// ����� ���ٴ� �޽����� ����Ѵ�.
			dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
	 *dwp = MSGID_NOTIFY;
			wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
	 *wp  = DEF_NOTIFY_SKILLTRAINSUCCESS;

			cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
			// ��� ���� .
	 *cp = m_pClientList[iClientH]->m_cSkillMastery[iRet];
			cp++;

			// ����ȣ
	 *cp = iRet;
			cp++;

			memcpy(cp, cSkillName, 20);
			cp += 20;

			// ���� ���
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 28);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
				DeleteClient(iClientH, true, true);
				return;
			}
		}
		else {
			// ������ �������� �ʾ� ������ ��� �� ���.
			// ������ ���� ���ٴ� �޽����� ����Ѵ�.
			dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
	 *dwp = MSGID_NOTIFY;
			wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
	 *wp  = DEF_NOTIFY_SKILLTRAINFAIL;

			cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
			// ��������.
	 *cp = 1;
			cp++;

			// ����ȣ
	 *cp = iRet;
			cp++;

			memcpy(cp, cSkillName, 20);
			cp += 20;

			// ���� ���
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 28);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
				DeleteClient(iClientH, true, true);
				return;
			}
		}
	}
	 */
}

int CGame::_iGetSkillNumber(char * pSkillName) {
	register int i;
	char cTmpName[21];

	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pSkillName);

	for (i = 1; i < DEF_MAXSKILLTYPE; i++)
		if (m_pSkillConfigList[i] != nullptr) {
			if (memcmp(cTmpName, m_pSkillConfigList[i]->m_cName, 20) == 0) {

				return i;
			}
		}

	return 0;
}

bool CGame::bPlayerItemToBank(int iClientH, short sItemIndex) {
	register int i, iIndex;

	if (m_pClientList[iClientH] == nullptr) return false;

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] == nullptr) {
			iIndex = i;


			ReleaseItemHandler(iClientH, sItemIndex, true);


			std::swap(m_pClientList[iClientH]->m_pItemInBankList[iIndex], m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;


			for (i = 1; i < DEF_MAXITEMS; i++)
				if ((m_pClientList[iClientH]->m_pItemList[i - 1] == nullptr) && (m_pClientList[iClientH]->m_pItemList[i] != nullptr)) {
					std::swap(m_pClientList[iClientH]->m_pItemList[i - 1], m_pClientList[iClientH]->m_pItemList[i]);
					m_pClientList[iClientH]->m_bIsItemEquipped[i - 1] = m_pClientList[iClientH]->m_bIsItemEquipped[i];
					m_pClientList[iClientH]->m_pItemList[i] = nullptr;
					m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;
				}

			return true;
		}

	return false;
}

bool CGame::bBankItemToPlayer(int iClientH, short sItemIndex) {
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_pItemInBankList[sItemIndex] == nullptr) return false;
	for (int i = 0; i < DEF_MAXITEMS; i++) {
		if (m_pClientList[iClientH]->m_pItemList[i] == nullptr) {
			std::swap(m_pClientList[iClientH]->m_pItemList[i], m_pClientList[iClientH]->m_pItemInBankList[sItemIndex]);
			return true;
		}
	}
	return false;
}

bool CGame::__bReadMapInfo(int iMapIndex) {
	char * pContents, * token, * pTile, cTxt[250], cFn[255];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iTeleportLocIndex = 0;
	int iWayPointCfgIndex = 0;
	int iTotalNpcSetting = 0;
	int iMGARCfgIndex = 0;
	int iSMGRCfgIndex = 0;
	int iNMRCfgIndex = 0;
	int iFishPointIndex = 0;
	int iMineralPointIndex = 0;
	int iStrategicPointIndex = 0;
	int iIndex = 0;

	int iNamingValue;
	class CStrTok * pStrTok;
	HANDLE hFile;
	uint32_t dwFileSize;
	FILE * pFile;

	char cName[6], cNpcName[21], cNpcMoveType, cNpcWaypointIndex[10], cNamePrefix;
	short sIPindex, dX, dY;


	if (memcmp(m_pMapList[iMapIndex]->m_cName, "fightzone", 9) == 0)
		m_pMapList[iMapIndex]->m_bIsFightZone = true;

	if (memcmp(m_pMapList[iMapIndex]->m_cName, "icebound", 8) == 0)
		m_pMapList[iMapIndex]->m_bIsSnowEnabled = true;

	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, "mapdata\\");
	strcat(cFn, m_pMapList[iMapIndex]->m_cName);
	strcat(cFn, ".txt");

	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) return false;
	dwFileSize = GetFileSize(hFile, nullptr);
	CloseHandle(hFile);


	pContents = new char[dwFileSize + 1];
	ZeroMemory(pContents, dwFileSize + 1);

	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {

		wsprintf(cTxt, "(!) Cannot open file : %s", cFn);
		PutLogList(cTxt);
		return false;
	} else {
		wsprintf(cTxt, "(!) Reading Map info file : %s", cFn);
		PutLogList(cTxt);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	//token = strtok(pContents, seps);
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 1 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sSrcX = atoi(token);
							cReadModeB = 2;
							break;

						case 2:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 2 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sSrcY = atoi(token);
							cReadModeB = 3;
							break;

						case 3:

							ZeroMemory(m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName,
									  sizeof (m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName));
							strcpy(m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName, token);
							cReadModeB = 4;
							break;

						case 4:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 3 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sDestX = atoi(token);
							cReadModeB = 5;
							break;

						case 5:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 4 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sDestY = atoi(token);
							cReadModeB = 6;
							break;

						case 6:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 5 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDir = atoi(token);
							iTeleportLocIndex++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 2:
					switch (cReadModeB) {
						case 1:
							// waypoint ¹øÈ£
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 6 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iWayPointCfgIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_WaypointList[iWayPointCfgIndex].x != -1) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 7 - Duplicated waypoint");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;

						case 2:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 8 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_WaypointList[iWayPointCfgIndex].x = atoi(token);
							cReadModeB = 3;
							break;

						case 3:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 9 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_WaypointList[iWayPointCfgIndex].y = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 3:

					switch (cReadModeB) {
						case 1:

							std::memset(cNpcName, 0, sizeof (cNpcName));
							strcpy(cNpcName, token);
							cReadModeB = 2;
							break;
						case 2:
							// NpcMoveType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 10 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cNpcMoveType = atoi(token);
							cReadModeB = 3;
							break;
						default:
							// WayPoint0~waypoint9
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 11 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cNpcWaypointIndex[cReadModeB - 3] = atoi(token);
							cReadModeB++;
							break;
						case 13:
							// cNamePrefix
							cNamePrefix = token[0];


							iNamingValue = m_pMapList[iMapIndex]->iGetEmptyNamingValue();
							if (iNamingValue == -1) {

							} else {

								std::memset(cName, 0, sizeof (cName));
								wsprintf(cName, "XX%d", iNamingValue);
								cName[0] = cNamePrefix;
								cName[1] = iMapIndex + 65;

								if (bCreateNewNpc(cNpcName, cName, m_pMapList[iMapIndex]->m_cName, 0, 0, cNpcMoveType, nullptr, nullptr, cNpcWaypointIndex, nullptr, 0, -1, false) == false) {

									m_pMapList[iMapIndex]->SetNamingValueEmpty(iNamingValue);
								}
							}
							cReadModeA = 0;
							cReadModeB = 0;
							iTotalNpcSetting++;
							break;
					}
					break;

				case 4:
					switch (cReadModeB) {
						case 1:

							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 12 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_bRandomMobGenerator = (bool) atoi(token);
							cReadModeB = 2;
							break;

						case 2:
							// Mob- Level
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 13 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_cRandomMobGeneratorLevel = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 5:
					// Maximum object
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 14 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}

					m_pMapList[iMapIndex]->m_iMaximumObject = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 6:

					switch (cReadModeB) {
						case 1:
							// Rect ¹øÈ£
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 15 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iMGARCfgIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].left != -1) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 16 - Duplicated Mob Gen Rect Number!");
								delete pContents;
								delete pStrTok;
								return false;
							}

							cReadModeB = 2;
							break;

						case 2:
							// left
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 17 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].left = atoi(token);
							cReadModeB = 3;
							break;

						case 3:
							// top
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 18 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].top = atoi(token);
							cReadModeB = 4;
							break;

						case 4:
							// right
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 19 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].right = atoi(token);
							cReadModeB = 5;
							break;

						case 5:
							// bottom
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 20 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].bottom = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 7:

					switch (cReadModeB) {
						case 1:
							// Rect ¹øÈ£ m_stSpotMobGenerator[]
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 21 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iSMGRCfgIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].bDefined == true) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error - ");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].bDefined = true;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 22 - Wrong Data format(SMGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cType = atoi(token);

							if (m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cType == 1)
								cReadModeB = 3;
							else if (m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cType == 2)
								cReadModeB = 9;
							break;

						case 3:
							// left
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 23 - Wrong Data format(SMGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.left = atoi(token);
							cReadModeB = 4;
							break;

						case 4:
							// top
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 24 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.top = atoi(token);
							cReadModeB = 5;
							break;

						case 5:
							// right
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 25 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.right = atoi(token);
							cReadModeB = 6;
							break;

						case 6:
							// bottom
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 26 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.bottom = atoi(token);
							cReadModeB = 7;
							break;

						case 7:
							// spot mob type
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 27 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMobType = atoi(token);
							cReadModeB = 8;
							break;

						case 8:
							// Max Mobs
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 28 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMaxMobs = atoi(token);
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iCurMobs = 0;
							cReadModeA = 0;
							cReadModeB = 0;
							break;

						default:
							// WayPoint0~waypoint9
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 29 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cWaypoint[cReadModeB - 9] = atoi(token);
							cReadModeB++;
							break;

						case 19:
							// spot mob type
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 30 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMobType = atoi(token);
							cReadModeB = 20;
							break;

						case 20:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 31 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMaxMobs = atoi(token);
							m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iCurMobs = 0;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 8:

					std::memset(m_pMapList[iMapIndex]->m_cLocationName, 0, sizeof (m_pMapList[iMapIndex]->m_cLocationName));
					memcpy(m_pMapList[iMapIndex]->m_cLocationName, token, 10);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 9:
					switch (cReadModeB) {
						case 1:
							// Initial-Point Index
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 32:1 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							sIPindex = atoi(token);
							if (m_pMapList[iMapIndex]->m_pInitialPoint[sIPindex].x != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Duplicate Initial Point Index!");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;

						case 2:
							// Initial-Point X
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 32 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_pInitialPoint[sIPindex].x = atoi(token);
							cReadModeB = 3;
							break;

						case 3:
							// Initial-Point Y
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 33 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_pInitialPoint[sIPindex].y = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 10:

					switch (cReadModeB) {
						case 1:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 34 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iNMRCfgIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].top != -1) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 35 - Duplicate No-Magic-Rect number");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 36 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].left = atoi(token);
							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 37 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].top = atoi(token);
							cReadModeB = 4;
							break;

						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 38 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].right = atoi(token);
							cReadModeB = 5;
							break;

						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 39 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].bottom = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 11:
					m_pMapList[iMapIndex]->m_bIsFixedDayMode = (bool) atoi(token);
					if (m_pMapList[iMapIndex]->m_bIsFixedDayMode == true)
						m_pMapList[iMapIndex]->m_bIsSnowEnabled = false;
					cReadModeA = 0;
					break;

				case 12:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 40 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iFishPointIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_FishPointList[iFishPointIndex].x != -1) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 41 - Duplicate FishPoint number");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_iTotalFishPoint++;
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 42 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_FishPointList[iFishPointIndex].x = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 43 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_FishPointList[iFishPointIndex].y = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 13:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 44 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_iMaxFish = atoi(token);

					cReadModeA = 0;
					break;

				case 14:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 45 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_cType = atoi(token);

					cReadModeA = 0;
					break;

				case 15:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 46 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_iLevelLimit = atoi(token);

					cReadModeA = 0;
					break;

				case 16:

					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 47 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_bMineralGenerator = (bool) atoi(token);
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 48 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_cMineralGeneratorLevel = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 17:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 49 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iMineralPointIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_MineralPointList[iMineralPointIndex].x != -1) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 50 - Duplicate MineralPoint number");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_iTotalMineralPoint++;
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 51 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_MineralPointList[iMineralPointIndex].x = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 52 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_MineralPointList[iMineralPointIndex].y = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 18:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 53 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_iMaxMineral = atoi(token);

					cReadModeA = 0;
					break;

				case 19:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 54 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_iUpperLevelLimit = atoi(token);

					cReadModeA = 0;
					break;

				case 20:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 55 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iStrategicPointIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex] != nullptr) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 56 - Duplicate Strategic Point number");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex] = new class CStrategicPoint;
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 57 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iSide = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 58 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iValue = atoi(token);

							cReadModeB = 4;
							break;

						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 59 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iX = atoi(token);

							cReadModeB = 5;
							break;

						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 60 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iY = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 21:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 61 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].cType != 0) {

								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 62 - Duplicate EnergySphereCreation number");
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_iTotalEnergySphereCreationPoint++;
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 63 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].cType = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 64 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].sX = atoi(token);

							cReadModeB = 4;
							break;

						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 65 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].sY = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 22:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 66 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);

							if (m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].cResult != 0) {

								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 67 - Duplicate EnergySphereGoal number(%d:%d)", iIndex, m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].cResult);
								PutLogList(G_cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_iTotalEnergySphereGoalPoint++;
							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 68 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].cResult = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 69 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].aresdenX = atoi(token);

							cReadModeB = 4;
							break;

						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 70 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].aresdenY = atoi(token);

							cReadModeB = 5;
							break;

						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 71 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].elvineX = atoi(token);

							cReadModeB = 6;
							break;

						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 72 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].elvineY = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 23:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 73 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);

							if (strlen(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName) != 0) {

								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 74 - Duplicate Strike Point number(%d)", iIndex);
								PutLogList(G_cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}

							cReadModeB = 2;
							break;

						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 75 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].dX = atoi(token);

							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 76 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].dY = atoi(token);

							cReadModeB = 4;
							break;

						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iHP = atoi(token);
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iInitHP = atoi(token);
							cReadModeB = 5;
							break;


						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[0] = atoi(token);

							cReadModeB = 6;
							break;

						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[0] = atoi(token);

							cReadModeB = 7;
							break;

						case 7:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[1] = atoi(token);

							cReadModeB = 8;
							break;

						case 8:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[1] = atoi(token);

							cReadModeB = 9;
							break;

						case 9:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[2] = atoi(token);

							cReadModeB = 10;
							break;

						case 10:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[2] = atoi(token);

							cReadModeB = 11;
							break;

						case 11:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[3] = atoi(token);

							cReadModeB = 12;
							break;

						case 12:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[3] = atoi(token);

							cReadModeB = 13;
							break;

						case 13:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[4] = atoi(token);

							cReadModeB = 14;
							break;

						case 14:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[4] = atoi(token);

							cReadModeB = 15;
							break;

						case 15:
							std::memset(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName, 0, sizeof (m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName));
							strcpy(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName, token);

							m_pMapList[iMapIndex]->m_iTotalStrikePoints++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;

					}
					break; // end 23

				case 24:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);

							if (strlen(m_pMapList[iMapIndex]->m_stItemEventList[iIndex].cItemName) != 0) {
								// �̹� �Ҵ�Ǿ��ִ� Item-Event ��ȣ�̴�.
								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 79 - Duplicate Item-Event number(%d:%s)", iIndex, m_pMapList[iMapIndex]->m_stItemEventList[iIndex].cItemName);
								PutLogList(G_cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}

							m_pMapList[iMapIndex]->m_iTotalItemEvents++;
							cReadModeB = 2;
							break;

						case 2:
							strcpy(m_pMapList[iMapIndex]->m_stItemEventList[iIndex].cItemName, token);
							cReadModeB = 3;
							break;

						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 81 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iAmount = atoi(token);

							cReadModeB = 4;
							break;

						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 82 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iTotalNum = atoi(token);

							cReadModeB = 5;
							break;

						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iMonth = atoi(token);

							cReadModeB = 6;
							break;

						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iDay = atoi(token);

							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 25: //mobevent-amount
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->sMobEventAmount = atoi(token);
							cReadModeB = 0;
							break;
					}
					break;

				case 26: //ApocalypseMobGenType
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMobGenType - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_iApocalypseMobGenType = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 27: // ApocalypseBossMob
					switch (cReadModeB) {
						case 1: // 3CB6Ch m_pMapList[]->m_ApocalypseBossMobNpcID
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_iApocalypseBossMobNpcID = atoi(token);
							cReadModeB = 2;
							break;
						case 2: // 3CB70h m_pMapList[]->ApocalypseBossMobRectX1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sApocalypseBossMobRectX1 = atoi(token);
							cReadModeB = 3;
							break;
						case 3: // 3CB74h m_pMapList[]->ApocalypseBossMobRectY1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sApocalypseBossMobRectY1 = atoi(token);
							cReadModeB = 4;
							break;
						case 4: // 3CB78h m_pMapList[]->ApocalypseBossMobRectX2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sApocalypseBossMobRectX2 = atoi(token);
							cReadModeB = 5;
							break;
						case 5: // 3CB7Ch m_pMapList[]->ApocalypseBossMobRectY2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sApocalypseBossMobRectY2 = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 28: //DynamicGateType // 28
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error DynamicGateType - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_cDynamicGateType = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 29: //DynamicGateCoord // 29
					// DynamicGateCoord	= 59 196 60 197        abaddon	   -1  -1
					switch (cReadModeB) {
						case 1: // 3CA20h
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 = atoi(token);
							cReadModeB = 2;
							break;

						case 2: // 3CA24h
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 = atoi(token);
							cReadModeB = 3;
							break;

						case 3: // 3CA28h
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2 = atoi(token);
							cReadModeB = 4;
							break;

						case 4: // 3CA2Ch
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2 = atoi(token);
							cReadModeB = 5;
							break;

						case 5: // 3CA30h
							memcpy(m_pMapList[iMapIndex]->m_cDynamicGateCoordDestMap, token, strlen(token));
							cReadModeB = 6;
							break;

						case 6: // 3CA3Ch
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sDynamicGateCoordTgtX = atoi(token);
							cReadModeB = 7;
							break;

						case 7: // (ty = 3CB60h) unknown (3CA3Eh)
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_sDynamicGateCoordTgtY = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 30: // RecallImpossible // 30
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error RecallImpossible -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_bIsRecallImpossible = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 31: // ApocalypseMap // 31
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMap -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_bIsApocalypseMap = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 32: // CitizenLimit // 32
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error CitizenLimit -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_bIsCitizenLimit = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 33: // HeldenianMap
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error CitizenLimit -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_bIsHeldenianMap = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 34: // HeldenianTower
					switch (cReadModeB) {
						case 1: // NpcID
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian tower type id - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stHeldenianTower[iIndex].sTypeID = atoi(token);
							cReadModeB = 2;
							break;
						case 2: // side
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Side - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stHeldenianTower[iIndex].cSide = atoi(token);
							cReadModeB = 3;
							break;
						case 3: // sX
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower X pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stHeldenianTower[iIndex].dX = atoi(token);
							cReadModeB = 4;
							break;
						case 4: // sY
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Y pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 35: // HeldenianModeMap
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pMapList[iMapIndex]->m_cHeldenianModeMap = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;

				case 36: // HeldenianWinningZone
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							dX = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							dY = atoi(token);
							pTile = 0;
							pTile = (char *) (m_pMapList[iMapIndex]->m_pTile + dX + dY * m_pMapList[iMapIndex]->m_sSizeY);
							if (pTile == 0) {
								wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map Info file error HeldenianWinningZone - pTile is Null dx(%d), dy(%d).", dX, dY);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							//pTile->m_iAttribute = 1;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				case 37: // HeldenianGateDoor // 37
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Direction - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stHeldenianGateDoor[iIndex].cDir = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door X pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stHeldenianGateDoor[iIndex].dX = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Y pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pMapList[iMapIndex]->m_stHeldenianGateDoor[iIndex].dY = atoi(token);
							iIndex++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;

				default:
					break;
			}
		} else {
			if (memcmp(token, "teleport-loc", 12) == 0) {
				m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex] = new class CTeleportLoc;
				cReadModeA = 1;
				cReadModeB = 1;
			}

			if (memcmp(token, "waypoint", 8) == 0) {
				cReadModeA = 2;
				cReadModeB = 1;
			}

			if (memcmp(token, "npc", 3) == 0) {
				cReadModeA = 3;
				cReadModeB = 1;
			}

			if (memcmp(token, "random-mob-generator", 20) == 0) {
				cReadModeA = 4;
				cReadModeB = 1;
			}

			if (memcmp(token, "maximum-object", 14) == 0)
				cReadModeA = 5;

			if (memcmp(token, "npc-avoidrect", 13) == 0) {
				cReadModeA = 6;
				cReadModeB = 1;
			}

			if (memcmp(token, "spot-mob-generator", 18) == 0) {
				cReadModeA = 7;
				cReadModeB = 1;
			}

			if (memcmp(token, "map-location", 12) == 0)
				cReadModeA = 8;

			if (memcmp(token, "initial-point", 13) == 0) {
				cReadModeA = 9;
				cReadModeB = 1;
			}

			if (memcmp(token, "no-attack-area", 14) == 0) {
				cReadModeA = 10;
				cReadModeB = 1;
			}

			if (memcmp(token, "fixed-dayornight-mode", 21) == 0) cReadModeA = 11;

			if (memcmp(token, "fish-point", 10) == 0) {
				cReadModeA = 12;
				cReadModeB = 1;
			}

			if (memcmp(token, "max-fish", 8) == 0) cReadModeA = 13;
			if (memcmp(token, "type", 4) == 0) cReadModeA = 14;
			if (memcmp(token, "level-limit", 11) == 0) cReadModeA = 15;

			if (memcmp(token, "mineral-generator", 17) == 0) {
				cReadModeA = 16;
				cReadModeB = 1;
			}

			if (memcmp(token, "mineral-point", 13) == 0) {
				cReadModeA = 17;
				cReadModeB = 1;
			}

			if (memcmp(token, "max-mineral", 11) == 0) cReadModeA = 18;
			if (memcmp(token, "upper-level-limit", 17) == 0) cReadModeA = 19; // v1.4
			if (memcmp(token, "strategic-point", 15) == 0) { // v1.41
				cReadModeA = 20;
				cReadModeB = 1;
			}

			if (memcmp(token, "energy-sphere-creation-point", 28) == 0) {
				cReadModeA = 21;
				cReadModeB = 1;
			}

			if (memcmp(token, "energy-sphere-goal-point", 24) == 0) {
				cReadModeA = 22;
				cReadModeB = 1;
			}

			if (memcmp(token, "strike-point", 12) == 0) {
				cReadModeA = 23;
				cReadModeB = 1;
			}

			if (memcmp(token, "item-event", 10) == 0) {
				cReadModeA = 24;
				cReadModeB = 1;
			}

			if (memcmp(token, "energy-sphere-auto-creation", 27) == 0) {
				cReadModeA = 0;
				cReadModeB = 0;
				m_pMapList[iMapIndex]->m_bIsEnergySphereAutoCreation = true;
			}

			if (memcmp(token, "mobevent-amount", 15) == 0) {
				cReadModeA = 25;
				cReadModeB = 1;
			}

			if (memcmp(token, "ApocalypseMobGenType", 20) == 0) {
				cReadModeA = 26;
				cReadModeB = 1;
			}

			if (memcmp(token, "ApocalypseBossMob", 17) == 0) {
				cReadModeA = 27;
				cReadModeB = 1;
			}

			if (memcmp(token, "DynamicGateType", 15) == 0) {
				cReadModeA = 28;
				cReadModeB = 1;
			}

			if (memcmp(token, "DynamicGateCoord", 16) == 0) {
				cReadModeA = 29;
				cReadModeB = 1;
			}

			if (memcmp(token, "RecallImpossible", 16) == 0) {
				cReadModeA = 30;
				cReadModeB = 1;
			}

			if (memcmp(token, "ApocalypseMap", 13) == 0) {
				cReadModeA = 31;
				cReadModeB = 1;
			}

			if (memcmp(token, "CitizenLimit", 12) == 0) {
				cReadModeA = 32;
				cReadModeB = 1;
			}

			if (memcmp(token, "HeldenianMap", 12) == 0) {
				cReadModeA = 33;
				cReadModeB = 1;
			}

			if (memcmp(token, "HeldenianTower", 14) == 0) {
				cReadModeA = 34;
				cReadModeB = 1;
			}

			if (memcmp(token, "HeldenianModeMap", 16) == 0) {
				cReadModeA = 35;
				cReadModeB = 1;
			}

			if (memcmp(token, "HeldenianWinningZone", 20) == 0) {
				cReadModeA = 36;
				cReadModeB = 1;
			}

			if (memcmp(token, "HeldenianGateDoor", 17) == 0) {
				cReadModeA = 37;
				cReadModeB = 1;
			}

			if (memcmp(token, "[END-MAP-INFO]", 14) == 0) {
				cReadModeA = 0;
				cReadModeB = 0;
				goto RMI_SKIPDECODING;
			}
		}
		token = pStrTok->pGet();
	}

RMI_SKIPDECODING:
	;

	delete pStrTok;
	delete pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! map info file contents error!");
		return false;
	}

	wsprintf(cTxt, "(!) Map info file decoding(%s) - success! TL(%d) WP(%d) LNPC(%d) MXO(%d) RMG(%d / %d)", cFn, iTeleportLocIndex, iWayPointCfgIndex, iTotalNpcSetting, m_pMapList[iMapIndex]->m_iMaximumObject, m_pMapList[iMapIndex]->m_bRandomMobGenerator, m_pMapList[iMapIndex]->m_cRandomMobGeneratorLevel);
	PutLogList(cTxt);


	m_pMapList[iMapIndex]->_SetupNoAttackArea();
	//Heldenian 3.00 Source
	strncpy(m_cHeldenianMapName, m_pMapList[iMapIndex]->m_cName, 11);
	m_bIsHeldenianReady = true;

	return true;

}

void CGame::Quit() {
	int i;


	G_bIsThread = false;
	Sleep(300);

	if (m_pMainLogSock != nullptr) delete m_pMainLogSock;
	for (i = 0; i < DEF_MAXSUBLOGSOCK; i++)
		if (m_pSubLogSock[i] != nullptr) delete m_pSubLogSock[i];

	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) delete m_pClientList[i];

	for (i = 0; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) delete m_pNpcList[i];

	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) delete m_pMapList[i];

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != nullptr) delete m_pItemConfigList[i];

	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		if (m_pNpcConfigList[i] != nullptr) delete m_pNpcConfigList[i];

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		if (m_pMagicConfigList[i] != nullptr) delete m_pMagicConfigList[i];

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		if (m_pSkillConfigList[i] != nullptr) delete m_pSkillConfigList[i];

	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != nullptr) delete m_pQuestConfigList[i];

	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] != nullptr) delete m_pDynamicObjectList[i];

	delayEvents_.reset();

	for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
		if (m_pNoticeMsgList[i] != nullptr) delete m_pNoticeMsgList[i];

	for (i = 0; i < DEF_MAXFISHS; i++)
		if (m_pFish[i] != nullptr) delete m_pFish[i];

	for (i = 0; i < DEF_MAXMINERALS; i++)
		if (m_pMineral[i] != nullptr) delete m_pMineral[i];

	for (i = 0; i < DEF_MAXPORTIONTYPES; i++) {
		if (m_pPortionConfigList[i] != nullptr) delete m_pPortionConfigList[i];
		if (m_pCraftingConfigList[i] != nullptr) delete m_pCraftingConfigList[i]; // Crafting
	}

	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		if (m_pBuildItemList[i] != nullptr) delete m_pBuildItemList[i];

	for (i = 0; i < DEF_MAXDUPITEMID; i++)
		if (m_pDupItemIDList[i] != nullptr) delete m_pDupItemIDList[i];

	if (m_pNoticementData != nullptr) delete m_pNoticementData;

}

int CGame::iGetLevelExp(int iLevel) {
	int iRet;

	if (iLevel == 0) return 0;

	iRet = iGetLevelExp(iLevel - 1) + iLevel * (50 + (iLevel * (iLevel / 17) * (iLevel / 17)));

	return iRet;
}

int CGame::_iCalcSkillSSNpoint(int iLevel) {
	int iRet;

	if (iLevel < 1) return 1;

	if (iLevel <= 50)
		iRet = iLevel;
	else if (iLevel > 50) {
		iRet = (iLevel * iLevel) / 10;
	}
	return iRet;
}

bool CGame::bCheckLevelUp(int iClientH) // Sobrepaso de lvl fix.
{
	if (m_pClientList[iClientH] == nullptr) return false;

	if (m_pClientList[iClientH]->m_iLevel >= m_iPlayerMaxLevel) {
		if (m_pClientList[iClientH]->m_iExp >= m_iLevelExpTable[m_iPlayerMaxLevel + 1]) {
			m_pClientList[iClientH]->m_iExp = m_iLevelExpTable[m_iPlayerMaxLevel];
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
			m_pClientList[iClientH]->m_iGizonItemUpgradeLeft++;
			if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft > DEF_MAXGIZONPOINT) m_pClientList[iClientH]->m_iGizonItemUpgradeLeft = DEF_MAXGIZONPOINT; // adamas
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 1, 0, nullptr);
		}
		return false;
	}

	/*	if ((m_pClientList[iClientH]->m_iLevel > DEF_LIMITHUNTERLEVEL - 1 ) && (m_pClientList[iClientH]->m_bIsHunter == true ) ) 
		{
			SetNoHunterMode(iClientH,true) ;
	 */

	if (m_pClientList[iClientH]->m_iExp < m_pClientList[iClientH]->m_iNextLevelExp) return false;

	while (m_pClientList[iClientH]->m_iExp > m_pClientList[iClientH]->m_iNextLevelExp
			  && m_pClientList[iClientH]->m_iLevel < m_iPlayerMaxLevel) {

		if (m_pClientList[iClientH]->m_iExp >= m_pClientList[iClientH]->m_iNextLevelExp) {
			m_pClientList[iClientH]->m_iLevel++;
			m_pClientList[iClientH]->m_iLU_Pool += 3; // v2.23
			if (m_pClientList[iClientH]->m_iStr > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iStr = DEF_CHARPOINTLIMIT;
			if (m_pClientList[iClientH]->m_iDex > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iDex = DEF_CHARPOINTLIMIT;
			if (m_pClientList[iClientH]->m_iVit > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iVit = DEF_CHARPOINTLIMIT;
			if (m_pClientList[iClientH]->m_iInt > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iInt = DEF_CHARPOINTLIMIT;
			if (m_pClientList[iClientH]->m_iMag > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iMag = DEF_CHARPOINTLIMIT;
			if (m_pClientList[iClientH]->m_iCharisma > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iCharisma = DEF_CHARPOINTLIMIT;

			SendNotifyMsg(0, iClientH, DEF_NOTIFY_LEVELUP, 0, 0, 0, nullptr);

			m_pClientList[iClientH]->m_iNextLevelExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel + 1]; //iGetLevelExp(m_pClientList[iClientH]->m_iLevel + 1);

			CalcTotalItemEffect(iClientH, -1, false);

			if (m_pClientList[iClientH]->m_iLevel >= DEF_PLAYERMAXLEVEL) {
				wsprintf(G_cTxt, "(!) Character (%s) is over max level", m_pClientList[iClientH]->m_cCharName);
				PutLogFileList(G_cTxt);
			}
		} else return true;
	}
	return false;
}

void CGame::StateChangeHandler(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cStateChange1, cStateChange2, cStateChange3;
	char cStr, cVit, cDex, cInt, cMag, cChar;
	int iOldStr, iOldVit, iOldDex, iOldInt, iOldMag, iOldChar;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0) return;

	cStr = cVit = cDex = cInt = cMag = cChar = 0;

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	cStateChange1 = *cp;
	cp++;

	cStateChange2 = *cp;
	cp++;

	cStateChange3 = *cp;
	cp++;

	iOldStr = m_pClientList[iClientH]->m_iStr;
	iOldVit = m_pClientList[iClientH]->m_iVit;
	iOldDex = m_pClientList[iClientH]->m_iDex;
	iOldInt = m_pClientList[iClientH]->m_iInt;
	iOldMag = m_pClientList[iClientH]->m_iMag;
	iOldChar = m_pClientList[iClientH]->m_iCharisma;

	if (!bChangeState(cStateChange1, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (!bChangeState(cStateChange2, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (!bChangeState(cStateChange3, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if (m_pClientList[iClientH]->m_iGuildRank == 0) {
		if (m_pClientList[iClientH]->m_iCharisma - cChar < 20) {
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
			return;
		}
	}

	if (iOldStr + iOldVit + iOldDex + iOldInt + iOldMag + iOldChar != (179 * 3 + 70)) {
		return;
	}

	if ((cStr < 0 || cVit < 0 || cDex < 0 || cInt < 0 || cMag < 0 || cChar < 0) ||
			  (cStr + cVit + cDex + cInt + cMag + cChar != 3)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((m_pClientList[iClientH]->m_iStr - cStr > DEF_CHARPOINTLIMIT)
			  || (m_pClientList[iClientH]->m_iStr - cStr < 10)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((m_pClientList[iClientH]->m_iDex - cDex > DEF_CHARPOINTLIMIT)
			  || (m_pClientList[iClientH]->m_iDex - cDex < 10)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((m_pClientList[iClientH]->m_iInt - cInt > DEF_CHARPOINTLIMIT)
			  || (m_pClientList[iClientH]->m_iInt - cInt < 10)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((m_pClientList[iClientH]->m_iVit - cVit > DEF_CHARPOINTLIMIT)
			  || (m_pClientList[iClientH]->m_iVit - cVit < 10)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((m_pClientList[iClientH]->m_iMag - cMag > DEF_CHARPOINTLIMIT)
			  || (m_pClientList[iClientH]->m_iMag - cMag < 10)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((m_pClientList[iClientH]->m_iCharisma - cChar > DEF_CHARPOINTLIMIT)
			  || (m_pClientList[iClientH]->m_iCharisma - cChar < 10)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if (m_pClientList[iClientH]->m_iLU_Pool < 3) m_pClientList[iClientH]->m_iLU_Pool = 3;

	m_pClientList[iClientH]->m_iGizonItemUpgradeLeft--;

	m_pClientList[iClientH]->m_iStr -= cStr;
	m_pClientList[iClientH]->m_iVit -= cVit;
	m_pClientList[iClientH]->m_iDex -= cDex;
	m_pClientList[iClientH]->m_iInt -= cInt;
	m_pClientList[iClientH]->m_iMag -= cMag;
	m_pClientList[iClientH]->m_iCharisma -= cChar;

	if (cInt > 0) {
		bCheckMagicInt(iClientH);
	}

	//Re-Calculate HP,MP,SP
	if (m_pClientList[iClientH]->m_iHP > iGetMaxHP(iClientH)) m_pClientList[iClientH]->m_iHP = iGetMaxHP(iClientH);
	if (m_pClientList[iClientH]->m_iMP > iGetMaxMP(iClientH)) m_pClientList[iClientH]->m_iMP = iGetMaxMP(iClientH);
	if (m_pClientList[iClientH]->m_iSP > iGetMaxSP(iClientH)) m_pClientList[iClientH]->m_iSP = iGetMaxSP(iClientH);

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
}


/////////////////////////////////////////////////////////////////////////////////////
//  bool CGame::bCheckMagicInt(int iClientH)  //another retarded korean function


//  date		 :: 2003-04-21
/////////////////////////////////////////////////////////////////////////////////////

bool CGame::bCheckMagicInt(int iClientH) {

	for (int i = 0; i < DEF_MAXMAGICTYPE; i++) {
		if (m_pMagicConfigList[i] != nullptr)
			if (m_pMagicConfigList[i]->m_sIntLimit > (m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt)) {
				m_pClientList[iClientH]->m_cMagicMastery[i] = 0;
			}
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////
//  bChangeState(char cStateChange
//  		  ,char* cStr, char *cVit,char *cDex,char *cInt,char *cMag,char *cChar)


//  date		 :: [2003-04-14]
/////////////////////////////////////////////////////////////////////////////////////

bool CGame::bChangeState(char cStateChange, char *cStr, char *cVit, char *cDex, char *cInt, char *cMag, char *cChar) {
	if (cStateChange == DEF_STR) {
		*cStr += 1;
	} else if (cStateChange == DEF_VIT) {
		*cVit += 1;
	} else if (cStateChange == DEF_DEX) {
		*cDex += 1;
	} else if (cStateChange == DEF_INT) {
		*cInt += 1;
	} else if (cStateChange == DEF_MAG) {
		*cMag += 1;
	} else if (cStateChange == DEF_CHR) {
		*cChar += 1;
	} else {
		return 0;
	}

	return cStateChange;
}


//50Cent - LU Fix

void CGame::LevelUpSettingsHandler(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char * cp;
	int * ip;
	int iStr, iVit, iDex, iInt, iMag, iChar;
	int iTotalSetting = 0;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iLU_Pool <= 0) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);

	ip = (int *) cp;
	iStr = (int) *ip;
	cp += 4;

	ip = (int *) cp;
	iVit = (int) *ip;
	cp += 4;

	ip = (int *) cp;
	iDex = (int) *ip;
	cp += 4;

	ip = (int *) cp;
	iInt = (int) *ip;
	cp += 4;

	ip = (int *) cp;
	iMag = (int) *ip;
	cp += 4;

	ip = (int *) cp;
	iChar = (int) *ip;
	cp += 4;

	if ((iStr + iVit + iDex + iInt + iMag + iChar) > m_pClientList[iClientH]->m_iLU_Pool) { // -3
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}

	// Level-Up Setting���� ���� �ִ��� �˻��Ѵ�.
	if ((m_pClientList[iClientH]->m_iStr + iStr > DEF_CHARPOINTLIMIT) || (iStr < 0))
		return;

	if ((m_pClientList[iClientH]->m_iDex + iDex > DEF_CHARPOINTLIMIT) || (iDex < 0))
		return;

	if ((m_pClientList[iClientH]->m_iInt + iInt > DEF_CHARPOINTLIMIT) || (iInt < 0))
		return;

	if ((m_pClientList[iClientH]->m_iVit + iVit > DEF_CHARPOINTLIMIT) || (iVit < 0))
		return;

	if ((m_pClientList[iClientH]->m_iMag + iMag > DEF_CHARPOINTLIMIT) || (iMag < 0))
		return;

	if ((m_pClientList[iClientH]->m_iCharisma + iChar > DEF_CHARPOINTLIMIT) || (iChar < 0))
		return;

	iTotalSetting = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iVit +
			  m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;

	if (iTotalSetting + m_pClientList[iClientH]->m_iLU_Pool - 3 > ((m_pClientList[iClientH]->m_iLevel - 1)*3 + 70)) {
		m_pClientList[iClientH]->m_iLU_Pool = 3 + (m_pClientList[iClientH]->m_iLevel - 1)*3 + 70 - iTotalSetting;

		if (m_pClientList[iClientH]->m_iLU_Pool < 3)
			m_pClientList[iClientH]->m_iLU_Pool = 3;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if (iTotalSetting + (iStr + iVit + iDex + iInt + iMag + iChar)
			  > ((m_pClientList[iClientH]->m_iLevel - 1)*3 + 70)) {
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}

	m_pClientList[iClientH]->m_iLU_Pool = m_pClientList[iClientH]->m_iLU_Pool - (iStr + iVit + iDex + iInt + iMag + iChar);

	m_pClientList[iClientH]->m_iStr += iStr;
	m_pClientList[iClientH]->m_iVit += iVit;
	m_pClientList[iClientH]->m_iDex += iDex;
	m_pClientList[iClientH]->m_iInt += iInt;
	m_pClientList[iClientH]->m_iMag += iMag;
	m_pClientList[iClientH]->m_iCharisma += iChar;

	SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);

}

/*
void CGame::LevelUpSettingsHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
	char * cp, cStr, cVit, cDex, cInt, cMag, cChar;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

	m_pClientList[iClientH]->m_iLU_Pool += DEF_TOTALLEVELUPPOINT;
	wsprintf(G_cTxt, "(*) Level Up: Char(%s) Level (%d) DEF_TOTALLEVELUPPOINT (%d) m_pClientList[iClientH]->m_iLU_Pool (%d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iLevel, DEF_TOTALLEVELUPPOINT, m_pClientList[iClientH]->m_iLU_Pool);
	PutLogList(G_cTxt);

	cStr = *cp;
	cp++;

	cVit = *cp;
	cp++;

	cDex = *cp;
	cp++;

	cInt = *cp;
	cp++;

	cMag = *cp;
	cp++;

	cChar = *cp;
	cp++;

	
	if ((cStr > DEF_TOTALLEVELUPPOINT) || (cStr < 0))
		return;

	if ((cDex > DEF_TOTALLEVELUPPOINT) || (cDex < 0))
		return;

	if ((cInt > DEF_TOTALLEVELUPPOINT) || (cInt < 0))
		return;

	if ((cVit > DEF_TOTALLEVELUPPOINT) || (cVit < 0))
		return;

	if ((cMag > DEF_TOTALLEVELUPPOINT) || (cMag < 0))
		return;

	if ((cChar > DEF_TOTALLEVELUPPOINT) || (cChar < 0))
		return;

	if ( (cStr + cVit + cDex + cInt + cMag + cChar) > DEF_TOTALLEVELUPPOINT)
		return;

	

	//m_pClientList[iClientH]->m_cLU_Str  = cStr;
	//m_pClientList[iClientH]->m_cLU_Vit  = cVit;
	//m_pClientList[iClientH]->m_cLU_Dex  = cDex;
	//m_pClientList[iClientH]->m_cLU_Int  = cInt;
	//m_pClientList[iClientH]->m_cLU_Mag  = cMag;
	//m_pClientList[iClientH]->m_cLU_Char = cChar;

	/*
	m_pClientList[iClientH]->m_cLU_Str = *cp;
	cp++;

	m_pClientList[iClientH]->m_cLU_Vit = *cp;
	cp++;

	m_pClientList[iClientH]->m_cLU_Dex = *cp;
	cp++;

	m_pClientList[iClientH]->m_cLU_Int = *cp;
	cp++;

	m_pClientList[iClientH]->m_cLU_Mag = *cp;
	cp++;

	m_pClientList[iClientH]->m_cLU_Char = *cp;
	cp++;

	if ((m_pClientList[iClientH]->m_cLU_Str > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Str < 0))
	m_pClientList[iClientH]->m_cLU_Str = 0;

	if ((m_pClientList[iClientH]->m_cLU_Vit > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Vit < 0))
	m_pClientList[iClientH]->m_cLU_Vit = 0;

	if ((m_pClientList[iClientH]->m_cLU_Dex > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Dex < 0))
	m_pClientList[iClientH]->m_cLU_Dex = 0;

	if ((m_pClientList[iClientH]->m_cLU_Int > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Int < 0))
	m_pClientList[iClientH]->m_cLU_Int = 0;

	if ((m_pClientList[iClientH]->m_cLU_Mag > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Mag < 0))
	m_pClientList[iClientH]->m_cLU_Mag = 0;

	if ((m_pClientList[iClientH]->m_cLU_Char > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Char < 0))
	m_pClientList[iClientH]->m_cLU_Char = 0;

	if ( (m_pClientList[iClientH]->m_cLU_Str + m_pClientList[iClientH]->m_cLU_Vit + m_pClientList[iClientH]->m_cLU_Dex +
	m_pClientList[iClientH]->m_cLU_Int + m_pClientList[iClientH]->m_cLU_Mag + m_pClientList[iClientH]->m_cLU_Char) > DEF_TOTALLEVELUPPOINT) {
	
	m_pClientList[iClientH]->m_cLU_Str = m_pClientList[iClientH]->m_cLU_Vit = m_pClientList[iClientH]->m_cLU_Dex =
	m_pClientList[iClientH]->m_cLU_Int = m_pClientList[iClientH]->m_cLU_Mag = m_pClientList[iClientH]->m_cLU_Char = 0;
	}
 */
//}

void CGame::FightzoneReserveHandler(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char cData[100];
	int iFightzoneNum, * ip, iEnableReserveTime;
	uint32_t * dwp, dwGoldCount;
	uint16_t * wp, wResult;
	int iRet, iResult = 1, iCannotReserveDay;
	SYSTEMTIME SysTime;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	GetLocalTime(&SysTime);


	iEnableReserveTime = 2 * 20 * 60 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 5 * 20;

	dwGoldCount = dwGetItemCount(iClientH, "Gold");

	ip = (int *) (pData + DEF_INDEX2_MSGTYPE + 2);

	iFightzoneNum = *ip;


	if ((iFightzoneNum < 1) || (iFightzoneNum > DEF_MAXFIGHTZONE)) return;






	iCannotReserveDay = (SysTime.wDay + m_pClientList[iClientH]->m_cSide + iFightzoneNum) % 2;
	if (iEnableReserveTime <= 0) {


		wResult = DEF_MSGTYPE_REJECT;
		iResult = 0;
	} else if (m_iFightZoneReserve[iFightzoneNum - 1] != 0) {

		wResult = DEF_MSGTYPE_REJECT;
		iResult = -1;
	} else if (dwGoldCount < 1500) {

		wResult = DEF_MSGTYPE_REJECT;
		iResult = -2;
	} else if (iCannotReserveDay) {

		wResult = DEF_MSGTYPE_REJECT;
		iResult = -3;
	} else if (m_pClientList[iClientH]-> m_iFightzoneNumber != 0) {

		wResult = DEF_MSGTYPE_REJECT;
		iResult = -4;
	} else {





		wResult = DEF_MSGTYPE_CONFIRM;


		SetItemCount(iClientH, "Gold", dwGoldCount - 1500);
		iCalcTotalWeight(iClientH);


		m_iFightZoneReserve[iFightzoneNum - 1] = iClientH;

		m_pClientList[iClientH]->m_iFightzoneNumber = iFightzoneNum;
		m_pClientList[iClientH]->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + SysTime.wHour;

		if (SysTime.wHour % 2) m_pClientList[iClientH]->m_iReserveTime += 1;
		else m_pClientList[iClientH]->m_iReserveTime += 2;
		wsprintf(G_cTxt, "(*) Reserve FIGHTZONETICKET : Char(%s) TICKENUMBER (%d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iReserveTime);
		PutLogFileList(G_cTxt);
		PutLogList(G_cTxt);

		m_pClientList[iClientH]->m_iFightZoneTicketNumber = 50;
		iResult = 1;
	}

	std::memset(cData, 0, sizeof (cData));

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_FIGHTZONE_RESERVE;

	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wResult;

	ip = (int *) (cData + DEF_INDEX2_MSGTYPE + 2);
	*ip = iResult;
	ip += 4;



	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 10);

	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			DeleteClient(iClientH, true, true);
			return;
	}
}

bool CGame::bCheckLimitedUser(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return false;

	if ((memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) &&
			  (m_pClientList[iClientH]->m_iExp >= m_iLevelExp20)) {


		m_pClientList[iClientH]->m_iExp = m_iLevelExp20 - 1;
		SendNotifyMsg(0, iClientH, DEF_NOTIFY_TRAVELERLIMITEDLEVEL, 0, 0, 0, nullptr);
		return true;
	}


	return false;
}

void CGame::RequestCivilRightHandler(CClient &client, char */*pData*/) {
	if (client.m_bIsInitComplete == false) return;
	if ((m_bAdminSecurity == true) && (client.m_iAdminUserLevel > 0)) return;

	uint16_t wResult = 1;
	if (memcmp(client.m_cLocation, "NONE", 4) != 0) wResult = 0;
	if (client.m_iLevel < 5) wResult = 0;

	if (wResult == 1) {
		std::memset(client.m_cLocation, 0, sizeof (client.m_cLocation));
		strcpy(client.m_cLocation, m_pMapList[client.m_cMapIndex]->m_cLocationName);
	}

	if (memcmp(client.m_cLocation, "are", 3) == 0)
		client.m_cSide = 1;

	if (memcmp(client.m_cLocation, "elv", 3) == 0)
		client.m_cSide = 2;

	char cData[100];
	uint32_t *dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_CIVILRIGHT;
	uint16_t *wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wResult;

	char *cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cp, client.m_cLocation, 10);
	cp += 10;

	int iRet = client.m_pXSock->iSendMsg(cData, 16);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(client.id_, true, true);
			return;
	}
	SendEventToNearClient_TypeA(client.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	RequestChangePlayMode(client.id_);
}

void CGame::RequestRetrieveItemHandler(CClient &client, char *pData) {
	char * cp, cBankItemIndex, cMsg[100];
	register int i, j, iRet, iItemWeight;
	uint32_t * dwp;
	uint16_t * wp;

	if (client.m_bIsInitComplete == false) return;

	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	cBankItemIndex = *cp;

	if ((cBankItemIndex < 0) || (cBankItemIndex >= DEF_MAXBANKITEMS)) return;
	if (client.m_pItemInBankList[cBankItemIndex] == nullptr) {
		std::memset(cMsg, 0, sizeof (cMsg));

		dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_RETRIEVEITEM;
		wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
		*wp = DEF_MSGTYPE_REJECT;

		iRet = client.m_pXSock->iSendMsg(cMsg, 8);
	} else {
		/*
		if ( (client.m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			 (client.m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) ) {
			//iItemWeight = client.m_pItemInBankList[cBankItemIndex]->m_wWeight * client.m_pItemInBankList[cBankItemIndex]->m_dwCount;
			iItemWeight = iGetItemWeight(client.m_pItemInBankList[cBankItemIndex], client.m_pItemInBankList[cBankItemIndex]->m_dwCount);
		}
		else iItemWeight = iGetItemWeight(client.m_pItemInBankList[cBankItemIndex], 1); //client.m_pItemInBankList[cBankItemIndex]->m_wWeight;
		 */
		iItemWeight = iGetItemWeight(*client.m_pItemInBankList[cBankItemIndex], client.m_pItemInBankList[cBankItemIndex]->m_dwCount);

		if ((iItemWeight + client.m_iCurWeightLoad) > _iCalcMaxLoad(client.id_)) {
			std::memset(cMsg, 0, sizeof (cMsg));

			dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

			iRet = client.m_pXSock->iSendMsg(cMsg, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					DeleteClient(client.id_, true, true);
					break;
			}
			return;
		}

		if ((client.m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
				  (client.m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
			for (i = 0; i < DEF_MAXITEMS; i++)
				if ((client.m_pItemList[i] != nullptr) &&
						  (client.m_pItemList[i]->m_cItemType == client.m_pItemInBankList[cBankItemIndex]->m_cItemType) &&
						  (memcmp(client.m_pItemList[i]->m_cName, client.m_pItemInBankList[cBankItemIndex]->m_cName, 20) == 0)) {
					SetItemCount(client.id_, i, client.m_pItemList[i]->m_dwCount + client.m_pItemInBankList[cBankItemIndex]->m_dwCount);
					client.m_pItemInBankList[cBankItemIndex].reset();
					for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++) {
						if ((client.m_pItemInBankList[j + 1] != nullptr) && (client.m_pItemInBankList[j] == nullptr)) {
							std::swap(client.m_pItemInBankList[j], client.m_pItemInBankList[j + 1]);
						}
					}

					std::memset(cMsg, 0, sizeof (cMsg));
					dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
					*dwp = MSGID_RESPONSE_RETRIEVEITEM;
					wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
					*wp = DEF_MSGTYPE_CONFIRM;

					cp = (char *) (cMsg + DEF_INDEX2_MSGTYPE + 2);
					*cp = cBankItemIndex;
					cp++;
					*cp = i;
					cp++;

					iCalcTotalWeight(client.id_);
					client.m_cArrowIndex = _iGetArrowItemIndex(client.id_);
					iRet = client.m_pXSock->iSendMsg(cMsg, 8);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							DeleteClient(client.id_, true, true);
							return;
					}
					return;
				}
			goto RRIH_NOQUANTITY;
		} else {
RRIH_NOQUANTITY:
			;
			for (i = 0; i < DEF_MAXITEMS; i++) {
				if (client.m_pItemList[i] == nullptr) {
					std::swap(client.m_pItemList[i], client.m_pItemInBankList[cBankItemIndex]);
					client.m_ItemPosList[i].x = 40;
					client.m_ItemPosList[i].y = 30;
					client.m_bIsItemEquipped[i] = false;
					for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++) {
						if ((client.m_pItemInBankList[j + 1] != nullptr) && (client.m_pItemInBankList[j] == nullptr)) {
							std::swap(client.m_pItemInBankList[j], client.m_pItemInBankList[j + 1]);
						}
					}

					std::memset(cMsg, 0, sizeof (cMsg));
					dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
					*dwp = MSGID_RESPONSE_RETRIEVEITEM;
					wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
					*wp = DEF_MSGTYPE_CONFIRM;

					cp = (char *) (cMsg + DEF_INDEX2_MSGTYPE + 2);
					*cp = cBankItemIndex;
					cp++;
					*cp = i;
					cp++;

					iCalcTotalWeight(client.id_);
					client.m_cArrowIndex = _iGetArrowItemIndex(client.id_);
					iRet = client.m_pXSock->iSendMsg(cMsg, 8);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							DeleteClient(client.id_, true, true);
							return;
					}
					return;
				}
			}
			std::memset(cMsg, 0, sizeof (cMsg));
			dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_RETRIEVEITEM;
			wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_REJECT;
			iRet = client.m_pXSock->iSendMsg(cMsg, 8);
		}
	}
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(client.id_, true, true);
			return;
	}
}

bool CGame::bSetItemToBankItem(int iClientH, short sItemIndex) {
	register int i, iRet;
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	short * sp;
	char cData[100];
	class CItem * pItem;


	if (m_pClientList[iClientH] == nullptr) return false;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return false;

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] == nullptr) {
			std::swap(m_pClientList[iClientH]->m_pItemInBankList[i], m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			pItem = m_pClientList[iClientH]->m_pItemInBankList[i].get();
			iCalcTotalWeight(iClientH);

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMTOBANK;

			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

			*cp = i;
			cp++;

			*cp = 1;
			cp++;

			memcpy(cp, pItem->m_cName, 20);
			cp += 20;

			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;

			*cp = pItem->m_cItemType;
			cp++;

			*cp = pItem->m_cEquipPos;
			cp++;

			*cp = (char) 0;
			cp++;

			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;

			*cp = pItem->m_cGenderLimit;
			cp++;

			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;

			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;

			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;

			*cp = pItem->m_cItemColor;
			cp++;

			// v1.432
			sp = (short *) cp;
			*sp = pItem->m_sItemEffectValue2;
			cp += 2;

			// v1.42
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;


			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 55);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					// DeleteClient(iClientH, true, true);
					return true;
			}

			return true;
		}


	return false;
}
// 05/21/2004 - Hypnotoad - send player to jail

void CGame::ApplyPKpenalty(short sAttackerH, short sVictumH) {
	int iV1, iV2;

	if (m_pClientList[sAttackerH] == nullptr) return;
	if (m_pClientList[sVictumH] == nullptr) return;


	if ((m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) && (m_pClientList[sAttackerH]->m_iPKCount == 0)) return;


	m_pClientList[sAttackerH]->m_iPKCount++;

	_bPKLog(DEF_PKLOG_BYPK, sAttackerH, sVictumH, nullptr);


	iV1 = iDice((m_pClientList[sVictumH]->m_iLevel / 2) + 1, 50);
	iV2 = iDice((m_pClientList[sAttackerH]->m_iLevel / 2) + 1, 50);

	m_pClientList[sAttackerH]->m_iExp -= iV1;
	m_pClientList[sAttackerH]->m_iExp -= iV2;
	if (m_pClientList[sAttackerH]->m_iExp < 0) m_pClientList[sAttackerH]->m_iExp = 0;


	SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_PKPENALTY, 0, 0, 0, nullptr);


	SendEventToNearClient_TypeA(sAttackerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);



	//PutLogFileList(G_cTxt);


	m_stCityStatus[m_pClientList[sAttackerH]->m_cSide].iCrimes++;


	m_pClientList[sAttackerH]->m_iRating -= 10;
	if (m_pClientList[sAttackerH]->m_iRating > 10000) m_pClientList[sAttackerH]->m_iRating = 10000;
	if (m_pClientList[sAttackerH]->m_iRating < -10000) m_pClientList[sAttackerH]->m_iRating = -10000;


	if (strcmp(m_pClientList[sAttackerH]->m_cLocation, "aresden") == 0) {
		if ((strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk11") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk12") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk21") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk22") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "aresden") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "huntzone2") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "areuni") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arefarm") == 0)) {



			std::memset(m_pClientList[sAttackerH]->m_cLockedMapName, 0, sizeof (m_pClientList[sAttackerH]->m_cLockedMapName));
			strcpy(m_pClientList[sAttackerH]->m_cLockedMapName, "arejail");
			m_pClientList[sAttackerH]->m_iLockedMapTime = 5 * 2;
			RequestTeleportHandler(sAttackerH, "2   ", "arejail", -1, -1);
			return;
		}
	}

	if (strcmp(m_pClientList[sAttackerH]->m_cLocation, "elvine") == 0) {
		if ((strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk11") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk12") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk21") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk22") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvine") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "huntzone1") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvuni") == 0) ||
				  (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvfarm") == 0)) {


			std::memset(m_pClientList[sAttackerH]->m_cLockedMapName, 0, sizeof (m_pClientList[sAttackerH]->m_cLockedMapName));
			strcpy(m_pClientList[sAttackerH]->m_cLockedMapName, "elvjail");
			m_pClientList[sAttackerH]->m_iLockedMapTime = 5 * 2;
			RequestTeleportHandler(sAttackerH, "2   ", "elvjail", -1, -1);
			return;
		}
	}
}


// 05/17/2004 - Hypnotoad - register pk log

void CGame::PK_KillRewardHandler(short sAttackerH, short sVictumH) {
	if (m_pClientList[sAttackerH] == nullptr) return;
	if (m_pClientList[sVictumH] == nullptr) return;

	_bPKLog(DEF_PKLOG_BYPLAYER, sAttackerH, sVictumH, nullptr);

	if (m_pClientList[sAttackerH]->m_iPKCount != 0) {


	} else {

		m_pClientList[sAttackerH]->m_iRewardGold += iGetExpLevel(m_pClientList[sVictumH]->m_iExp) * 3;


		if (m_pClientList[sAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
			m_pClientList[sAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
		if (m_pClientList[sAttackerH]->m_iRewardGold < 0)
			m_pClientList[sAttackerH]->m_iRewardGold = 0;

		SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_PKCAPTURED, m_pClientList[sVictumH]->m_iPKCount, m_pClientList[sVictumH]->m_iLevel, 0, m_pClientList[sVictumH]->m_cCharName);
	}
}

void CGame::EnemyKillRewardHandler(int iAttackerH, int iClientH) {
	// enemy-kill-mode = 1 | 0
	// if m_bEnemyKillMode is true than death match mode

	// DEATHMATCH MODE:
	// Aresden kills Elvine in Aresden and gets EK
	// Elvine kills Aresden in Aresden and gets an EK
	// Elvine kills Aresden in Elvine and gets an EK
	// Aresden kills Elvine in Elvine and gets an EK

	// CLASSIC MODE:
	// Aresden kills Elvine in Aresden and gets EK
	// Elvine kills Aresden in Aresden and doesnt get an EK
	// Elvine kills Aresden in Elvine and gets an EK
	// Aresden kills Elvine in Elvine and doesnt get an EK

	int iRewardExp, iEK_Level;

	// if the attacker doesnt exist no result
	if (m_pClientList[iAttackerH] == nullptr) return;
	// if the target doesnt exist no result
	if (m_pClientList[iClientH] == nullptr) return;

	// Log the EK (killed by enemy)
	_bPKLog(DEF_PKLOG_BYENERMY, iAttackerH, iClientH, nullptr);

	// set EK level to 30
	iEK_Level = 30;
	// if attacker's level is greater than 80, set ek level to 80
	if (m_pClientList[iAttackerH]->m_iLevel >= 80) iEK_Level = 80;
	// check if attacker level is less than or equal to max level
	if (m_pClientList[iAttackerH]->m_iLevel >= m_iPlayerMaxLevel) {
		// if the
		if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level) {

			if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
					  && (m_bEnemyKillMode == false)) {

				//Happy Hour para vos
				if (m_bHappyHour == true) {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
				} else {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
				}
			}


			if (m_bEnemyKillMode == true) {

				//Happy Hour para vos
				if (m_bHappyHour == true) {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
				} else {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
				}
			}
		}

		m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
		if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
			m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
		if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
			m_pClientList[iAttackerH]->m_iRewardGold = 0;


		SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, nullptr);
		return;
	}

	if (m_pClientList[iAttackerH]->m_iPKCount != 0) {

	} else {

		if (m_pClientList[iClientH]->m_iGuildRank == -1) {


			iRewardExp = (iDice(3, (3 * iGetExpLevel(m_pClientList[iClientH]->m_iExp))) + iGetExpLevel(m_pClientList[iClientH]->m_iExp)) / 3;

			if (m_bIsCrusadeMode == true) {

				m_pClientList[iAttackerH]->m_iExp += (iRewardExp / 3)*4;
				m_pClientList[iAttackerH]->m_iWarContribution += (iRewardExp - (iRewardExp / 3))*12;

				if (m_pClientList[iAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
					m_pClientList[iAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

				m_pClientList[iAttackerH]->m_iConstructionPoint += m_pClientList[iClientH]->m_iLevel / 2;

				if (m_pClientList[iAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
					m_pClientList[iAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

				//testcode
				wsprintf(G_cTxt, "Enemy Player Killed by Player! Construction: +%d WarContribution +%d", m_pClientList[iClientH]->m_iLevel / 2, (iRewardExp - (iRewardExp / 3))*6);
				PutLogList(G_cTxt);


				SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iAttackerH]->m_iConstructionPoint, m_pClientList[iAttackerH]->m_iWarContribution, 0, nullptr);


				if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level) {

					if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
					if (m_bEnemyKillMode == true) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}

				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			} else {

				m_pClientList[iAttackerH]->m_iExp += iRewardExp;

				if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level) {

					if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
							  && (m_bEnemyKillMode == false)) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}

					if (m_bEnemyKillMode == true) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}

				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			}
		} else {

			iRewardExp = (iDice(3, (3 * iGetExpLevel(m_pClientList[iClientH]->m_iExp))) + iGetExpLevel(m_pClientList[iClientH]->m_iExp)) / 3;


			if (m_bIsCrusadeMode == true) {


				m_pClientList[iAttackerH]->m_iExp += (iRewardExp / 3)*4;
				m_pClientList[iAttackerH]->m_iWarContribution += (iRewardExp - (iRewardExp / 3))*12;

				if (m_pClientList[iAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
					m_pClientList[iAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

				m_pClientList[iAttackerH]->m_iConstructionPoint += m_pClientList[iClientH]->m_iLevel / 2;

				if (m_pClientList[iAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
					m_pClientList[iAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

				//testcode
				wsprintf(G_cTxt, "Enemy Player Killed by Player! Construction: +%d WarContribution +%d", m_pClientList[iClientH]->m_iLevel / 2, (iRewardExp - (iRewardExp / 3))*6);
				PutLogList(G_cTxt);


				SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iAttackerH]->m_iConstructionPoint, m_pClientList[iAttackerH]->m_iWarContribution, 0, nullptr);


				if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level) {

					if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
							  && (m_bEnemyKillMode == false)) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}

					if (m_bEnemyKillMode == true) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}

				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			} else {

				m_pClientList[iAttackerH]->m_iExp += iRewardExp;

				if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level) {

					if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
							  && (m_bEnemyKillMode == false)) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}

					if (m_bEnemyKillMode == true) {

						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}

				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			}
		}


		SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, nullptr);

		if (bCheckLimitedUser(iAttackerH) == false) {

			SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
		}

		bCheckLevelUp(iAttackerH);


		m_stCityStatus[m_pClientList[iAttackerH]->m_cSide].iWins++;
	}
}

// 05/22/2004 - Hypnotoad - register in pk log

void CGame::ApplyCombatKilledPenalty(int iClientH, int cPenaltyLevel, bool bIsSAattacked) {
	int iExp;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	// Crusade
	if (m_bIsCrusadeMode == true) {


		if (m_pClientList[iClientH]->m_iPKCount > 0) {
			m_pClientList[iClientH]->m_iPKCount--;
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_PKPENALTY, 0, 0, 0, nullptr);
			// v2.15
			_bPKLog(DEF_PKLOG_REDUCECRIMINAL, 0, iClientH, nullptr);

		}
		return;
	} else {

		if (m_pClientList[iClientH]->m_iPKCount > 0) {
			m_pClientList[iClientH]->m_iPKCount--;
			SendNotifyMsg(0, iClientH, DEF_NOTIFY_PKPENALTY, 0, 0, 0, nullptr);
			// v2.15
			_bPKLog(DEF_PKLOG_REDUCECRIMINAL, 0, iClientH, nullptr);
		}


		iExp = iDice(1, (5 * cPenaltyLevel * m_pClientList[iClientH]->m_iLevel));


		if (m_pClientList[iClientH]->m_bIsNeutral == true) iExp = iExp / 3;


		// if (m_pClientList[iClientH]->m_iLevel == DEF_PLAYERMAXLEVEL) iExp = 0;

		m_pClientList[iClientH]->m_iExp -= iExp;
		if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;

		SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);


		if (m_pClientList[iClientH]->m_bIsNeutral != true) {
			if (m_pClientList[iClientH]->m_iLevel < 80) {

				// v2.03 60 -> 80
				cPenaltyLevel--;
				if (cPenaltyLevel <= 0) cPenaltyLevel = 1;
				_PenaltyItemDrop(iClientH, cPenaltyLevel, bIsSAattacked);
			} else _PenaltyItemDrop(iClientH, cPenaltyLevel, bIsSAattacked);
		}
	}
}

// 05/29/2004 - Hypnotoad - Limits some items from not dropping

void CGame::_PenaltyItemDrop(int iClientH, int iTotal, bool bIsSAattacked) {
	register int i, j, iRemainItem;
	char cItemIndexList[DEF_MAXITEMS], cItemIndex;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	if ((m_pClientList[iClientH]->m_iAlterItemDropIndex != -1) && (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex] != nullptr)) {
		// Testcode
		if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) {


			if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
				m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan--;

			DropItemHandler(iClientH, m_pClientList[iClientH]->m_iAlterItemDropIndex, -1, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_cName);

			m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
		} else {
			// v2.04 testcode
			PutLogFileList("Alter Drop Item Index Error1");

			for (i = 0; i < DEF_MAXITEMS; i++)
				if ((m_pClientList[iClientH]->m_pItemList[i] != nullptr) && (m_pClientList[iClientH]->m_pItemList[i]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {
					m_pClientList[iClientH]->m_iAlterItemDropIndex = i;
					if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
						m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan--;

					DropItemHandler(iClientH, m_pClientList[iClientH]->m_iAlterItemDropIndex, -1, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_cName);
					m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
					return;
				}


			goto PID_DROP;
		}
		return;
	}

PID_DROP:
	;

	for (i = 1; i <= iTotal; i++) {
		iRemainItem = 0;
		std::memset(cItemIndexList, 0, sizeof (cItemIndexList));

		for (j = 0; j < DEF_MAXITEMS; j++)
			if (m_pClientList[iClientH]->m_pItemList[j] != nullptr) {
				cItemIndexList[iRemainItem] = j;
				iRemainItem++;
			}

		if (iRemainItem == 0) return;
		cItemIndex = cItemIndexList[ iDice(1, iRemainItem) - 1 ];


		if ((m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectType != 0) &&
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectValue1 == m_pClientList[iClientH]->m_sCharIDnum1) &&
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectValue2 == m_pClientList[iClientH]->m_sCharIDnum2) &&
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectValue3 == m_pClientList[iClientH]->m_sCharIDnum3)) {

		} else if (
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sIDnum >= 400) &&
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sIDnum != 402) &&
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sIDnum <= 428)) {
		} else if (((m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
				  (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) &&
				  (bIsSAattacked == false)) {

		} else if ((m_pClientList[iClientH]->m_bIsLuckyEffect == true) && (iDice(1, 10) == 5)) {

		} else DropItemHandler(iClientH, cItemIndex, -1, m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_cName);
	}
}

void CGame::GetRewardMoneyHandler(int iClientH) {
	int iRet, iEraseReq, iWeightLeft, iRewardGoldLeft;
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cData[100], cItemName[21];
	class CItem * pItem;
	short * sp;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;



	iWeightLeft = _iCalcMaxLoad(iClientH) - iCalcTotalWeight(iClientH);

	if (iWeightLeft <= 0) return;

	iWeightLeft = iWeightLeft / 2;
	if (iWeightLeft <= 0) return;

	pItem = new class CItem;
	std::memset(cItemName, 0, sizeof (cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(*pItem, cItemName);
	//pItem->m_dwCount = m_pClientList[iClientH]->m_iRewardGold;


	if ((iWeightLeft / iGetItemWeight(*pItem, 1)) >= m_pClientList[iClientH]->m_iRewardGold) {

		pItem->m_dwCount = m_pClientList[iClientH]->m_iRewardGold;
		iRewardGoldLeft = 0;
	} else {

		pItem->m_dwCount = (iWeightLeft / iGetItemWeight(*pItem, 1));
		iRewardGoldLeft = m_pClientList[iClientH]->m_iRewardGold - (iWeightLeft / iGetItemWeight(*pItem, 1));
	}

	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {



		m_pClientList[iClientH]->m_iRewardGold = iRewardGoldLeft;

		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;

		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


		*cp = 1;
		cp++;

		memcpy(cp, pItem->m_cName, 20);
		cp += 20;

		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;

		*cp = pItem->m_cItemType;
		cp++;

		*cp = pItem->m_cEquipPos;
		cp++;

		*cp = (char) 0;
		cp++;

		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;

		*cp = pItem->m_cGenderLimit;
		cp++;

		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;

		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;

		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;

		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;

		*cp = pItem->m_cItemColor;
		cp++;

		*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
		cp++;

		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(pItem->m_dwAttribute & 0x00000001); 
		cp++;
		 */

		if (iEraseReq == 1) delete pItem;


		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);

		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:

				DeleteClient(iClientH, true, true);
				return;
		}


		SendNotifyMsg(0, iClientH, DEF_NOTIFY_REWARDGOLD, 0, 0, 0, nullptr);
	} else {


	}
}

int CGame::_iCalcMaxLoad(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return 0;

	return ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 500 + m_pClientList[iClientH]->m_iLevel * 500);
}

int CGame::iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwLastTime, int iV1) {
	register int i;
	short sPreType;
	uint32_t dwTime, dwRegisterTime;

	m_pMapList[cMapIndex]->bGetDynamicObject(sX, sY, &sPreType, &dwRegisterTime);
	if (sPreType != 0) return 0;

	switch (sType) {
		case DEF_DYNAMICOBJECT_FIRE3:
		case DEF_DYNAMICOBJECT_FIRE:
			if (m_pMapList[cMapIndex]->bGetIsMoveAllowedTile(sX, sY) == false)
				return 0;
			if (dwLastTime != 0) {
				switch (m_pMapList[cMapIndex]->m_cWhetherStatus) {
					case 1: dwLastTime = dwLastTime - (dwLastTime / 2);
						break;
					case 2: dwLastTime = (dwLastTime / 2) - (dwLastTime / 3);
						break;
					case 3: dwLastTime = (dwLastTime / 3) - (dwLastTime / 4);
						break;
				}

				if (dwLastTime == 0) dwLastTime = 1000;
			}
			break;

		case DEF_DYNAMICOBJECT_FISHOBJECT:
		case DEF_DYNAMICOBJECT_FISH:
			if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false)
				return 0;
			break;

		case DEF_DYNAMICOBJECT_ARESDENFLAG1:
		case DEF_DYNAMICOBJECT_ELVINEFLAG1:
		case DEF_DYNAMICOBJECT_MINERAL1:
		case DEF_DYNAMICOBJECT_MINERAL2:
			if (m_pMapList[cMapIndex]->bGetMoveable(sX, sY) == false)
				return 0;
			m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
			break;

	}

	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] == nullptr) {
			dwTime = timeGetTime();

			if (dwLastTime != 0)
				dwLastTime += (iDice(1, 4)*1000);

			m_pDynamicObjectList[i] = new class CDynamicObject(sOwner, cOwnerType, sType, cMapIndex, sX, sY, dwTime, dwLastTime, iV1);
			m_pMapList[cMapIndex]->SetDynamicObject(i, sType, sX, sY, dwTime);
			SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_CONFIRM, cMapIndex, sX, sY, sType, i, 0);

			return i;
		}
	return 0;
}

void CGame::CheckDynamicObjectList() {
	register int i;
	uint32_t dwTime = timeGetTime(), dwRegisterTime;
	short sType;


	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if ((m_pDynamicObjectList[i] != 0) && (m_pDynamicObjectList[i]->m_dwLastTime != 0)) {

			switch (m_pDynamicObjectList[i]->m_sType) {
				case DEF_DYNAMICOBJECT_FIRE3:
				case DEF_DYNAMICOBJECT_FIRE:

					switch (m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_cWhetherStatus) {
						case 0: break;
						case 1:
						case 2:
						case 3:

							m_pDynamicObjectList[i]->m_dwLastTime = m_pDynamicObjectList[i]->m_dwLastTime -
									  (m_pDynamicObjectList[i]->m_dwLastTime / 10) * m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_cWhetherStatus;
							break;
					}
					break;
			}
		}
	}


	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if ((m_pDynamicObjectList[i] != nullptr) && (m_pDynamicObjectList[i]->m_dwLastTime != 0) &&
				  ((dwTime - m_pDynamicObjectList[i]->m_dwRegisterTime) >= m_pDynamicObjectList[i]->m_dwLastTime)) {


			m_pMapList[ m_pDynamicObjectList[i]->m_cMapIndex ]->bGetDynamicObject(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, &sType, &dwRegisterTime);


			if (dwRegisterTime == m_pDynamicObjectList[i]->m_dwRegisterTime) {
				SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, m_pDynamicObjectList[i]->m_sType, i, 0);

				m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, dwTime);
			}

			switch (sType) {
				case DEF_DYNAMICOBJECT_FISHOBJECT:
				case DEF_DYNAMICOBJECT_FISH:

					bDeleteFish(m_pDynamicObjectList[i]->m_sOwner, 2);
					break;
			}


			delete m_pDynamicObjectList[i];
			m_pDynamicObjectList[i] = nullptr;
		}
	}
}

void CGame::CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue) {
	short sSkillIndex;
	int iOldSSN, iSSNpoint, iWeaponIndex;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_pItemList[sWeaponIndex] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;

	sSkillIndex = m_pClientList[iClientH]->m_pItemList[sWeaponIndex]->m_sRelatedSkill;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;

	iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
	m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

	iSSNpoint = m_iSkillSSNpoint[ m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] + 1 ];

	if ((m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
			  (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {

		m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;

		switch (sSkillIndex) {
			case 0: // Mining
			case 5: // Hand-Attack
			case 13: // Manufacturing
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 2)) {
					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 3: // Magic-Resistance
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iLevel * 2)) {
					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 4: // Magic
			case 21: // Staff-Attack
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag)* 2)) {
					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 1: // Fishing
			case 6: // Archery
			case 7: // Short-Sword
			case 8: // Long-Sword
			case 9: // Fencing
			case 10: // Axe-Attack
			case 11: // Shield
			case 14: // Hammer
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iAngelicDex) * 2)) {
					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 2: // Farming
			case 12: // Alchemy
			case 15: // ����óġ
			case 19: // Pretend-Corpse
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) * 2)) {
					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 23: // Poison-Resistance
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2)) {
					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			default:
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;
		}


		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1) {
				// ��չ��Ⱑ ����Ǿ� �־��.
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					// Ȱ���� ��뿡 ���� ��ų�� ����̾��. ���߷��� 1 �ø���.
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1) {
				// ���Ⱑ ����Ǿ� �־��.
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					// ���̳� Mace���� ��뿡 ���� ��ų�� ����̾��. ���߷��� 1 �ø���.
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}
		}

		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {
			// SKill�� ������ 600�� ������ �ٸ� ��ų�� �ϳ��� 1 ������.
			bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);

			SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], 0, nullptr);
		}
	}
}

/*void CGame::CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue)
{
 short sSkillIndex;
 int   iOldSSN, iSSNpoint, iWeaponIndex;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_pItemList[sWeaponIndex] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;

	sSkillIndex = m_pClientList[iClientH]->m_pItemList[sWeaponIndex]->m_sRelatedSkill;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	
	if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;

#ifdef DEF_TESTSERVER
	iValue = iValue * 100;
#endif

	iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
	m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

	iSSNpoint = m_iSkillSSNpoint[ m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]+1 ];

	
	if ( (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
		 (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint) ) {

		
		m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;
		

		switch (sSkillIndex) {
		case 0:
		case 2:
		case 5:
		case 13:
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iStr * 2)) {
				
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 3:
			
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iLevel * 2)) {
				
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 4:
		case 21: 
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iMag * 2)) {
				
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 1:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 14:
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iDex * 2)) {
				
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 12:
		case 14:
		case 15:
		case 19:
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iInt * 2)) {
				
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 23: 
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2)) {
				
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		default:
			m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;
		}

		
		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {
			
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1) {
				
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1) {
				
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}
		}

		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {
			
			bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);
			
			SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], nullptr, nullptr);
		}
	}
}*/


void CGame::CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue) {
	int iOldSSN, iSSNpoint, iWeaponIndex;

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;


	if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;

#ifdef DEF_TESTSERVER
	iValue = iValue * 100;
#endif

	iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
	m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

	iSSNpoint = m_iSkillSSNpoint[ m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] + 1 ];


	if ((m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
			  (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {


		m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;

		switch (sSkillIndex) {
			case 0:
			case 5:
			case 13:
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 2)) {

					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 3:

				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iLevel * 2)) {

					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 4:
			case 21:
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag)* 2)) {

					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 1:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iDex) * 2)) {

					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 2:
			case 12:
			case 14:
			case 15:
			case 19:
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt)* 2)) {

					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			case 23:
				if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2)) {

					m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
					m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;

			default:
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
				break;
		}


		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1) {

				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {

					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1) {

				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {

					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}
		}

		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {

			bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);


			SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], 0, nullptr);
		}
	}
}

void CGame::MobGenerator() {
	register int i, x, j, iNamingValue, iResult, iTotalMob;
	char cNpcName[21], cName_Master[11], cName_Slave[11], cWaypoint[11];
	char cSA;
	int pX, pY, iMapLevel, iProbSA, iKindSA, iResultNum, iNpcID;
	bool bMaster, bFirmBerserk, bIsSpecialEvent;

	if (m_bOnExitProcess == true) return;

	for (i = 0; i < DEF_MAXMAPS; i++) {
		// Random Mob Generator

		/*
		iResultNum = 0;
		// v1.432 ??? ??????????????? ?????? ??? ?????? ??? ??????. 1000???????? 100%  ??? 50%
		if ((m_pMapList[i] != nullptr) && (m_pMapList[i]->m_bRandomMobGenerator == true)) {
		// ???????? ?????? ??? ????? ???.
		if (m_iTotalGameServerClients >= 1000) {
		iResultNum = (m_pMapList[i]->m_iMaximumObject - 30);
		}
		else {
		dV2 = (double)m_iTotalGameServerClients;
		dV3 = (m_pMapList[i]->m_iMaximumObject - 30);

		dV1 = (dV2 / 1000.0f)*dV3;
		iResultNum = (int)dV1;
		}

			if ((m_pMapList[i]->m_bIsHeldenianMap == true) && (m_bIsHeldenianMode == true)) break;
			else if ((m_pMapList[i]->m_bIsHeldenianMap == true) && (m_bHeldenianInitiated == true)) break;

		// v1.432 ??? ???
		iMin = (m_pMapList[i]->m_iMaximumObject - 30) - ((m_pMapList[i]->m_iMaximumObject - 30)/3);
		if (iResultNum > (m_pMapList[i]->m_iMaximumObject - 30)) iResultNum = (m_pMapList[i]->m_iMaximumObject - 30);
		if (iResultNum < iMin) iResultNum = iMin;
		}
		 */

		//if ( (m_pMapList[i] != nullptr) && (m_pMapList[i]->m_bRandomMobGenerator == true) &&
		//	 ((m_pMapList[i]->m_iMaximumObject - 30) > m_pMapList[i]->m_iTotalActiveObject) ) {

		if (m_pMapList[i] != nullptr) {
			//if (m_bIsCrusadeMode == true)
			//	 iResultNum = (m_pMapList[i]->m_iMaximumObject - 30) / 3;
			//else iResultNum = (m_pMapList[i]->m_iMaximumObject - 30);
			iResultNum = (m_pMapList[i]->m_iMaximumObject - 30);
		}

		if ((m_pMapList[i] != nullptr) && (m_pMapList[i]->m_bRandomMobGenerator == true) && (iResultNum > m_pMapList[i]->m_iTotalActiveObject)) {
			if ((m_iMiddlelandMapIndex != -1) && (m_iMiddlelandMapIndex == i) && (m_bIsCrusadeMode == true)) break;

			iNamingValue = m_pMapList[i]->iGetEmptyNamingValue();
			if (iNamingValue != -1) {
				// Master Mob????????.
				std::memset(cName_Master, 0, sizeof (cName_Master));
				wsprintf(cName_Master, "XX%d", iNamingValue);
				cName_Master[0] = '_';
				cName_Master[1] = i + 65;

				std::memset(cNpcName, 0, sizeof (cNpcName));

				bFirmBerserk = false;
				iResult = iDice(1, 100);
				switch (m_pMapList[i]->m_cRandomMobGeneratorLevel) {

					case 1: // arefarm, elvfarm, aresden, elvine
						if ((iResult >= 1) && (iResult < 20)) {
							iResult = 1; // Slime
						} else if ((iResult >= 20) && (iResult < 40)) {
							iResult = 2; // Giant-Ant
						} else if ((iResult >= 40) && (iResult < 85)) {
							iResult = 24; // Rabbit
						} else if ((iResult >= 85) && (iResult < 95)) {
							iResult = 25; // Cat
						} else if ((iResult >= 95) && (iResult <= 100)) {
							iResult = 3; // Orc
						}
						iMapLevel = 1;
						break;

					case 2:
						if ((iResult >= 1) && (iResult < 40)) {
							iResult = 1;
						} else if ((iResult >= 40) && (iResult < 80)) {
							iResult = 2;
						} else iResult = 10;
						iMapLevel = 1;
						break;

					case 3: // v1.4334 ????????? ????
						if ((iResult >= 1) && (iResult < 20)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 3;
									break;
								case 2: iResult = 4;
									break;
							}
						} else if ((iResult >= 20) && (iResult < 25)) {
							iResult = 30;
						} else if ((iResult >= 25) && (iResult < 50)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 5;
									break;
								case 2: iResult = 6;
									break;
								case 3: iResult = 7;
									break;
							}
						} else if ((iResult >= 50) && (iResult < 75)) {
							switch (iDice(1, 7)) {
								case 1:
								case 2: iResult = 8;
									break;
								case 3: iResult = 11;
									break;
								case 4: iResult = 12;
									break;
								case 5: iResult = 18;
									break;
								case 6: iResult = 26;
									break;
								case 7: iResult = 28;
									break;
							}
						} else if ((iResult >= 75) && (iResult <= 100)) {
							switch (iDice(1, 5)) {
								case 1:
								case 2: iResult = 9;
									break;
								case 3: iResult = 13;
									break;
								case 4: iResult = 14;
									break;
								case 5: iResult = 27;
									break;
							}
						}
						iMapLevel = 3;
						break;

					case 4:
						if ((iResult >= 1) && (iResult < 50)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 2;
									break;
								case 2: iResult = 10;
									break;
							}
						} else if ((iResult >= 50) && (iResult < 80)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 8;
									break;
								case 2: iResult = 11;
									break;
							}
						} else if ((iResult >= 80) && (iResult <= 100)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 14;
									break;
								case 2: iResult = 9;
									break;
							}
						}
						iMapLevel = 2;
						break;

					case 5:
						if ((iResult >= 1) && (iResult < 30)) {
							switch (iDice(1, 5)) {
								case 1:
								case 2:
								case 3:
								case 4:
								case 5: iResult = 2;
									break;
							}
						} else if ((iResult >= 30) && (iResult < 60)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 3;
									break;
								case 2: iResult = 4;
									break;
							}
						} else if ((iResult >= 60) && (iResult < 80)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 5;
									break;
								case 2: iResult = 7;
									break;
							}
						} else if ((iResult >= 80) && (iResult < 95)) {
							switch (iDice(1, 3)) {
								case 1:
								case 2: iResult = 8;
									break;
								case 3: iResult = 11;
									break;
							}
						} else if ((iResult >= 95) && (iResult <= 100)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 11;
									break;
								case 2: iResult = 14;
									break;
								case 3: iResult = 9;
									break;
							}
						}
						iMapLevel = 3;
						break;

					case 6: // huntzone3, huntzone4
						if ((iResult >= 1) && (iResult < 60)) {
							switch (iDice(1, 4)) {
								case 1: iResult = 5;
									break; // Skeleton
								case 2: iResult = 6;
									break; // Orc-Mage
								case 3: iResult = 12;
									break; // Cyclops
								case 4: iResult = 11;
									break; // Troll
							}
						} else if ((iResult >= 60) && (iResult < 90)) {
							switch (iDice(1, 5)) {
								case 1:
								case 2: iResult = 8;
									break; // Stone-Golem
								case 3: iResult = 11;
									break; // Troll
								case 4: iResult = 12;
									break; // Cyclops
								case 5: iResult = 43;
									break; // Tentocle
							}
						} else if ((iResult >= 90) && (iResult <= 100)) {
							switch (iDice(1, 9)) {
								case 1: iResult = 26;
									break;
								case 2: iResult = 9;
									break;
								case 3: iResult = 13;
									break;
								case 4: iResult = 14;
									break;
								case 5: iResult = 18;
									break;
								case 6: iResult = 28;
									break;
								case 7: iResult = 27;
									break;
								case 8: iResult = 29;
									break;
							}
						}
						iMapLevel = 4;
						break;

					case 7: // areuni, elvuni
						if ((iResult >= 1) && (iResult < 50)) {
							switch (iDice(1, 5)) {
								case 1: iResult = 3;
									break; // Orc
								case 2: iResult = 6;
									break; // Orc-Mage
								case 3: iResult = 10;
									break; // Amphis
								case 4: iResult = 3;
									break; // Orc
								case 5: iResult = 50;
									break; // Giant-Tree
							}
						} else if ((iResult >= 50) && (iResult < 60)) {
							iResult = 29; // Rudolph
						} else if ((iResult >= 60) && (iResult < 85)) {
							switch (iDice(1, 4)) {
								case 1: iResult = 50;
									break; // Giant-Tree
								case 2:
								case 3: iResult = 6;
									break; // Orc-Mage
								case 4: iResult = 12;
									break; // Troll
							}
						} else if ((iResult >= 85) && (iResult <= 100)) {
							switch (iDice(1, 4)) {
								case 1: iResult = 12;
									break; // Troll
								case 2:
								case 3:
									if (iDice(1, 100) < 3)
										iResult = 17; // Unicorn
									else iResult = 12; // Troll
									break;
								case 4: iResult = 29;
									break; // Cannibal-Plant
							}
						}
						iMapLevel = 4;
						break;

					case 8:
						if ((iResult >= 1) && (iResult < 70)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 4;
									break;
								case 2: iResult = 5;
									break;
							}
						} else if ((iResult >= 70) && (iResult < 95)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 8;
									break;
								case 2: iResult = 11;
									break;
							}
						} else if ((iResult >= 95) && (iResult <= 100)) {
							iResult = 14;
							break;
						}
						iMapLevel = 4;
						break;

					case 9:
						if ((iResult >= 1) && (iResult < 70)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 4;
									break;
								case 2: iResult = 5;
									break;
							}
						} else if ((iResult >= 70) && (iResult < 95)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 8;
									break;
								case 2: iResult = 9;
									break;
								case 3: iResult = 13;
									break;
							}
						} else if ((iResult >= 95) && (iResult <= 100)) {
							switch (iDice(1, 6)) {
								case 1:
								case 2:
								case 3: iResult = 9;
									break;
								case 4:
								case 5: iResult = 14;
									break;
								case 6: iResult = 15;
									break;
							}
						}

						if ((iDice(1, 3) == 1) && (iResult != 16)) bFirmBerserk = true;
						iMapLevel = 5;
						break;

					case 10:
						if ((iResult >= 1) && (iResult < 70)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 9;
									break;
								case 2: iResult = 5;
									break;
								case 3: iResult = 8;
									break;
							}
						} else if ((iResult >= 70) && (iResult < 95)) {
							switch (iDice(1, 3)) {
								case 1:
								case 2: iResult = 13;
									break;
								case 3: iResult = 14;
									break;
							}
						} else if ((iResult >= 95) && (iResult <= 100)) {
							switch (iDice(1, 3)) {
								case 1:
								case 2: iResult = 14;
									break;
								case 3: iResult = 15;
									break;
							}
						}
						// Demon?? Berserk ????? ??? ?????
						if ((iDice(1, 3) == 1) && (iResult != 16)) bFirmBerserk = true;
						iMapLevel = 5;
						break;

					case 11:
						if ((iResult >= 1) && (iResult < 30)) {
							switch (iDice(1, 5)) {
								case 1:
								case 2:
								case 3:
								case 4:
								case 5: iResult = 2;
									break;
							}
						} else if ((iResult >= 30) && (iResult < 60)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 3;
									break;
								case 2: iResult = 4;
									break;
							}
						} else if ((iResult >= 60) && (iResult < 80)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 5;
									break;
								case 2: iResult = 7;
									break;
							}
						} else if ((iResult >= 80) && (iResult < 95)) {
							switch (iDice(1, 3)) {
								case 1:
								case 2: iResult = 10;
									break;
								case 3: iResult = 11;
									break;
							}
						} else if ((iResult >= 95) && (iResult <= 100)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 11;
									break;
								case 2: iResult = 7;
									break;
								case 3: iResult = 8;
									break;
							}
						}
						iMapLevel = 4;
						break;

					case 12:
						if ((iResult >= 1) && (iResult < 50)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 1;
									break;
								case 2: iResult = 2;
									break;
								case 3: iResult = 10;
									break;
							}
						} else if ((iResult >= 50) && (iResult < 85)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 5;
									break;
								case 2: iResult = 4;
									break;
							}
						} else if ((iResult >= 85) && (iResult <= 100)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 8;
									break;
								case 2: iResult = 11;
									break;
								case 3: iResult = 26;
									break;
							}
						}
						iMapLevel = 4;
						break;

					case 13:
						if ((iResult >= 1) && (iResult < 15)) {
							iResult = 4;
							bFirmBerserk = true;
							iTotalMob = 4 - (iDice(1, 2) - 1);
							break;
						} else if ((iResult >= 15) && (iResult < 40)) {
							iResult = 14;
							bFirmBerserk = true;
							iTotalMob = 4 - (iDice(1, 2) - 1);
							break;
						} else if ((iResult >= 40) && (iResult < 60)) {
							iResult = 9;
							bFirmBerserk = true;
							iTotalMob = 4 - (iDice(1, 2) - 1);
							break;
						} else if ((iResult >= 60) && (iResult < 75)) {
							iResult = 13;
							bFirmBerserk = true;
							iTotalMob = 4 - (iDice(1, 2) - 1);
							break;
						} else if ((iResult >= 75) && (iResult < 95)) {
							iResult = 23;
						} else if ((iResult >= 95) && (iResult <= 100)) {
							iResult = 22;
						}
						iMapLevel = 5;
						break;

					case 14: // icebound
						if ((iResult >= 1) && (iResult < 30)) {
							iResult = 23; // Dark-Elf
						} else if ((iResult >= 30) && (iResult < 50)) {
							iResult = 31; // Ice-Golem
						} else if ((iResult >= 50) && (iResult < 70)) {
							iResult = 22; // Beholder
							bFirmBerserk = true;
							iTotalMob = 4 - (iDice(1, 2) - 1);
						} else if ((iResult >= 70) && (iResult < 90)) {
							iResult = 32; // DireBoar
						} else if ((iResult >= 90) && (iResult <= 100)) {
							iResult = 33; // Frost
						}
						iMapLevel = 5;
						break;

					case 15:
						if ((iResult >= 1) && (iResult < 35)) {
							iResult = 23;
							bFirmBerserk = true;
						} else if ((iResult >= 35) && (iResult < 50)) {
							iResult = 22;
							bFirmBerserk = true;
						} else if ((iResult >= 50) && (iResult < 80)) {
							iResult = 15;
						} else if ((iResult >= 80) && (iResult <= 100)) {
							iResult = 21;
						}
						iMapLevel = 4;
						break;

					case 16: // 2ndmiddle, huntzone1, huntzone2,
						if ((iResult >= 1) && (iResult < 40)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 7;
									break; // Scorpion
								case 2: iResult = 2;
									break; // Giant-Ant
								case 3: iResult = 10;
									break; // Amphis
							}
						} else if ((iResult >= 40) && (iResult < 50)) {
							iResult = 30; // Rudolph
						} else if ((iResult >= 50) && (iResult < 85)) {
							switch (iDice(1, 2)) {
								case 1: iResult = 5;
									break; // Skeleton
								case 2: iResult = 4;
									break; // Zombie
							}
						} else if ((iResult >= 85) && (iResult <= 100)) {
							switch (iDice(1, 3)) {
								case 1: iResult = 8;
									break; // Stone-Golem
								case 2: iResult = 11;
									break; // Clay-Golem
								case 3: iResult = 7;
									break; // Scorpion
							}
						}
						iMapLevel = 1;
						break;

					case 17:
						if ((iResult >= 1) && (iResult < 30)) {
							switch (iDice(1, 4)) {
								case 1: iResult = 22;
									break; // Giant-Frog
								case 2: iResult = 8;
									break; // Stone-Golem
								case 3: iResult = 24;
									break; // Rabbit
								case 4: iResult = 5;
									break;
							}
						} else if ((iResult >= 30) && (iResult < 40)) {
							iResult = 30;
						} else if ((iResult >= 40) && (iResult < 70)) {
							iResult = 32;
						} else if ((iResult >= 70) && (iResult < 90)) {
							iResult = 31;
							if (iDice(1, 5) == 1) {
								bFirmBerserk = true;
							}
						} else if ((iResult >= 90) && (iResult <= 100)) {
							iResult = 33;
						}
						iMapLevel = 1;
						break;

					case 18: // druncncity
						if ((iResult >= 1) && (iResult < 2)) {
							iResult = 39; // Tentocle
						} else if ((iResult >= 2) && (iResult < 12)) {
							iResult = 44; // ClawTurtle
						} else if ((iResult >= 12) && (iResult < 50)) {
							iResult = 48; // Nizie
						} else if ((iResult >= 50) && (iResult < 80)) {
							iResult = 45; // Giant-Crayfish
						} else if ((iResult >= 80) && (iResult < 90)) {
							iResult = 34; // Stalker
						} else if ((iResult >= 90) && (iResult <= 100)) {
							iResult = 26; // Giant-Frog
						}
						iMapLevel = 4;
						break;

					case 19:
						if ((iResult >= 1) && (iResult < 15)) {
							iResult = 44;
						} else if ((iResult >= 15) && (iResult < 25)) {
							iResult = 46;
						} else if ((iResult >= 25) && (iResult < 35)) {
							iResult = 21;
						} else if ((iResult >= 35) && (iResult < 60)) {
							iResult = 43;
						} else if ((iResult >= 60) && (iResult < 85)) {
							iResult = 23;
						} else if ((iResult >= 85) && (iResult <= 100)) {
							iResult = 22;
						}
						iMapLevel = 4;
						break;

					case 20:
						if ((iResult >= 1) && (iResult < 2)) {
							iResult = 41;
						} else if ((iResult >= 2) && (iResult < 3)) {
							iResult = 40;
						} else if ((iResult >= 3) && (iResult < 8)) {
							iResult = 53;
						} else if ((iResult >= 8) && (iResult < 9)) {
							iResult = 39;
						} else if ((iResult >= 9) && (iResult < 20)) {
							iResult = 21;
						} else if ((iResult >= 20) && (iResult < 35)) {
							iResult = 16;
						} else if ((iResult >= 35) && (iResult < 45)) {
							iResult = 44;
						} else if ((iResult >= 45) && (iResult < 55)) {
							iResult = 45;
						} else if ((iResult >= 55) && (iResult < 75)) {
							iResult = 28;
						} else if ((iResult >= 75) && (iResult < 95)) {
							iResult = 43;
						} else if ((iResult >= 95) && (iResult < 100)) {
							iResult = 22;
						}
						iMapLevel = 4;
						break;

					case 21:
						if ((iResult >= 1) && (iResult < 94)) {
							iResult = 17; // Unicorn
							bFirmBerserk = true;
						} else if ((iResult >= 94) && (iResult < 95)) {
							iResult = 36; // Wyvern
						} else if ((iResult >= 95) && (iResult < 96)) {
							iResult = 37; // Fire-Wyvern
						} else if ((iResult >= 96) && (iResult < 97)) {
							iResult = 47; // MasterMage-Orc
						} else if ((iResult >= 97) && (iResult < 98)) {
							iResult = 35; // Hellclaw
						} else if ((iResult >= 98) && (iResult < 99)) {
							iResult = 49; // Tigerworm
						} else if ((iResult >= 99) && (iResult <= 100)) {
							iResult = 51; // Abaddon
						}
						iMapLevel = 4;
						break;

				}

				pX = 0;
				pY = 0;

				//				bIsSpecialEvent = true;
				if ((m_bIsSpecialEventTime == true) && (iDice(1, 10) == 3)) bIsSpecialEvent = true;

				if (bIsSpecialEvent == true) {
					switch (m_cSpecialEventType) {
						case 1:
							if (m_pMapList[i]->m_iMaxPx != 0) {
								pX = m_pMapList[i]->m_iMaxPx * 20 + 10;
								pY = m_pMapList[i]->m_iMaxPy * 20 + 10;

								if (pX < 0) pX = 0;
								if (pY < 0) pY = 0;

								if (m_bIsCrusadeMode == true) {
									if (strcmp(m_pMapList[i]->m_cName, "aresden") == 0)
										switch (iDice(1, 6)) {
											case 1: iResult = 20;
												break;
											case 2: iResult = 53;
												break;
											case 3: iResult = 55;
												break;
											case 4: iResult = 57;
												break;
											case 5: iResult = 59;
												break;
											case 6: iResult = 61;
												break;
										} else if (strcmp(m_pMapList[i]->m_cName, "elvine") == 0)
										switch (iDice(1, 6)) {
											case 1: iResult = 19;
												break;
											case 2: iResult = 52;
												break;
											case 3: iResult = 54;
												break;
											case 4: iResult = 56;
												break;
											case 5: iResult = 58;
												break;
											case 6: iResult = 60;
												break;
										}
								}
								wsprintf(G_cTxt, "(!) Mob-Event Map(%s)[%d (%d,%d)]", m_pMapList[i]->m_cName, iResult, pX, pY);
							}
							break;

						case 2:
							if (iDice(1, 3) == 2) {
								if ((memcmp(m_pMapList[i]->m_cLocationName, "aresden", 7) == 0) ||
										  (memcmp(m_pMapList[i]->m_cLocationName, "middled1n", 9) == 0) ||
										  (memcmp(m_pMapList[i]->m_cLocationName, "arefarm", 7) == 0) ||
										  (memcmp(m_pMapList[i]->m_cLocationName, "elvfarm", 7) == 0) ||
										  (memcmp(m_pMapList[i]->m_cLocationName, "elvine", 6) == 0)) {
									if (iDice(1, 30) == 5)
										iResult = 16;
									else iResult = 5;
								} else iResult = 16;
							} else iResult = 17;

							m_bIsSpecialEventTime = false;
							break;
					}
				}

				std::memset(cNpcName, 0, sizeof (cNpcName));
				//Random Monster Spawns
				switch (iResult) {
					case 1: strcpy(cNpcName, "Slime");
						iNpcID = 10;
						iProbSA = 5;
						iKindSA = 1;
						break;
					case 2: strcpy(cNpcName, "Giant-Ant");
						iNpcID = 16;
						iProbSA = 10;
						iKindSA = 2;
						break;
					case 3: strcpy(cNpcName, "Orc");
						iNpcID = 14;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 4: strcpy(cNpcName, "Zombie");
						iNpcID = 18;
						iProbSA = 15;
						iKindSA = 3;
						break;
					case 5: strcpy(cNpcName, "Skeleton");
						iNpcID = 11;
						iProbSA = 35;
						iKindSA = 8;
						break;
					case 6: strcpy(cNpcName, "Orc-Mage");
						iNpcID = 14;
						iProbSA = 30;
						iKindSA = 7;
						break;
					case 7: strcpy(cNpcName, "Scorpion");
						iNpcID = 17;
						iProbSA = 15;
						iKindSA = 3;
						break;
					case 8: strcpy(cNpcName, "Stone-Golem");
						iNpcID = 12;
						iProbSA = 25;
						iKindSA = 5;
						break;
					case 9: strcpy(cNpcName, "Cyclops");
						iNpcID = 13;
						iProbSA = 35;
						iKindSA = 8;
						break;
					case 10: strcpy(cNpcName, "Amphis");
						iNpcID = 22;
						iProbSA = 20;
						iKindSA = 3;
						break;
					case 11: strcpy(cNpcName, "Clay-Golem");
						iNpcID = 23;
						iProbSA = 20;
						iKindSA = 5;
						break;
					case 12: strcpy(cNpcName, "Troll");
						iNpcID = 28;
						iProbSA = 25;
						iKindSA = 3;
						break;
					case 13: strcpy(cNpcName, "Orge");
						iNpcID = 29;
						iProbSA = 25;
						iKindSA = 1;
						break;
					case 14: strcpy(cNpcName, "Hellbound");
						iNpcID = 27;
						iProbSA = 25;
						iKindSA = 8;
						break;
					case 15: strcpy(cNpcName, "Liche");
						iNpcID = 30;
						iProbSA = 30;
						iKindSA = 8;
						break;
					case 16: strcpy(cNpcName, "Demon");
						iNpcID = 31;
						iProbSA = 20;
						iKindSA = 8;
						break;
					case 17: strcpy(cNpcName, "Unicorn");
						iNpcID = 32;
						iProbSA = 35;
						iKindSA = 7;
						break;
					case 18: strcpy(cNpcName, "WereWolf");
						iNpcID = 33;
						iProbSA = 25;
						iKindSA = 1;
						break;
					case 19: strcpy(cNpcName, "YB-Aresden");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 20: strcpy(cNpcName, "YB-Elvine");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 21: strcpy(cNpcName, "Gagoyle");
						iNpcID = 52;
						iProbSA = 20;
						iKindSA = 8;
						break;
					case 22: strcpy(cNpcName, "Beholder");
						iNpcID = 53;
						iProbSA = 20;
						iKindSA = 5;
						break;
					case 23: strcpy(cNpcName, "Dark-Elf");
						iNpcID = 54;
						iProbSA = 20;
						iKindSA = 3;
						break;
					case 24: strcpy(cNpcName, "Rabbit");
						iNpcID = -1;
						iProbSA = 5;
						iKindSA = 1;
						break;
					case 25: strcpy(cNpcName, "Cat");
						iNpcID = -1;
						iProbSA = 10;
						iKindSA = 2;
						break;
					case 26: strcpy(cNpcName, "Giant-Frog");
						iNpcID = 57;
						iProbSA = 10;
						iKindSA = 2;
						break;
					case 27: strcpy(cNpcName, "Mountain-Giant");
						iNpcID = 58;
						iProbSA = 25;
						iKindSA = 1;
						break;
					case 28: strcpy(cNpcName, "Ettin");
						iNpcID = 59;
						iProbSA = 20;
						iKindSA = 8;
						break;
					case 29: strcpy(cNpcName, "Cannibal-Plant");
						iNpcID = 60;
						iProbSA = 20;
						iKindSA = 5;
						break;
					case 30: strcpy(cNpcName, "Rudolph");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 5;
						break;
					case 31: strcpy(cNpcName, "Ice-Golem");
						iNpcID = 65;
						iProbSA = 35;
						iKindSA = 8;
						break;
					case 32: strcpy(cNpcName, "DireBoar");
						iNpcID = 62;
						iProbSA = 20;
						iKindSA = 5;
						break;
					case 33: strcpy(cNpcName, "Frost");
						iNpcID = 63;
						iProbSA = 30;
						iKindSA = 8;
						break;
					case 34: strcpy(cNpcName, "Stalker");
						iNpcID = 48;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 35: strcpy(cNpcName, "Hellclaw");
						iNpcID = 49;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 36: strcpy(cNpcName, "Wyvern");
						iNpcID = 66;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 37: strcpy(cNpcName, "Fire-Wyvern");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 38: strcpy(cNpcName, "Barlog");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 39: strcpy(cNpcName, "Tentocle");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 40: strcpy(cNpcName, "Centaurus");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 41: strcpy(cNpcName, "Giant-Lizard");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 42: strcpy(cNpcName, "Minotaurs");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 43: strcpy(cNpcName, "Tentocle");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 44: strcpy(cNpcName, "Claw-Turtle");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 45: strcpy(cNpcName, "Giant-Crayfish");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 46: strcpy(cNpcName, "Giant-Plant");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 47: strcpy(cNpcName, "MasterMage-Orc");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 48: strcpy(cNpcName, "Nizie");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 49: strcpy(cNpcName, "Tigerworm");
						iNpcID = 50;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 50: strcpy(cNpcName, "Giant-Plant");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 51: strcpy(cNpcName, "Abaddon");
						iNpcID = -1;
						iProbSA = 20;
						iKindSA = 1;
						break;
					case 52: strcpy(cNpcName, "YW-Aresden");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 53: strcpy(cNpcName, "YW-Elvine");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 54: strcpy(cNpcName, "YY-Aresden");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 55: strcpy(cNpcName, "YY-Elvine");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 56: strcpy(cNpcName, "XB-Aresden");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 57: strcpy(cNpcName, "XB-Elvine");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 58: strcpy(cNpcName, "XW-Aresden");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 59: strcpy(cNpcName, "XW-Elvine");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 60: strcpy(cNpcName, "XY-Aresden");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					case 61: strcpy(cNpcName, "XY-Elvine");
						iNpcID = -1;
						iProbSA = 15;
						iKindSA = 1;
						break;
					default: strcpy(cNpcName, "Orc");
						iNpcID = 14;
						iProbSA = 15;
						iKindSA = 1;
						break;
				}

				cSA = 0;
				if (iDice(1, 100) <= iProbSA) {
					cSA = _cGetSpecialAbility(iKindSA);
				}

				if ((bMaster = bCreateNewNpc(cNpcName, cName_Master, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, bFirmBerserk, true)) == false) {
					m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
				} else {

				}
			}

			switch (iResult) {
				case 1: iTotalMob = iDice(1, 5) - 1;
					break;
				case 2: iTotalMob = iDice(1, 5) - 1;
					break;
				case 3: iTotalMob = iDice(1, 5) - 1;
					break;
				case 4: iTotalMob = iDice(1, 3) - 1;
					break;
				case 5: iTotalMob = iDice(1, 3) - 1;
					break;

				case 6: iTotalMob = iDice(1, 3) - 1;
					break;
				case 7: iTotalMob = iDice(1, 3) - 1;
					break;
				case 8: iTotalMob = iDice(1, 2) - 1;
					break;
				case 9: iTotalMob = iDice(1, 2) - 1;
					break;
				case 10: iTotalMob = iDice(1, 5) - 1;
					break;
				case 11: iTotalMob = iDice(1, 3) - 1;
					break;
				case 12: iTotalMob = iDice(1, 5) - 1;
					break;
				case 13: iTotalMob = iDice(1, 3) - 1;
					break;
				case 14: iTotalMob = iDice(1, 2) - 1;
					break;
				case 15: iTotalMob = iDice(1, 3) - 1;
					break;
				case 16: iTotalMob = iDice(1, 2) - 1;
					break;
				case 17: iTotalMob = iDice(1, 2) - 1;
					break;

				case 18: iTotalMob = iDice(1, 5) - 1;
					break;
				case 19: iTotalMob = iDice(1, 2) - 1;
					break;
				case 20: iTotalMob = iDice(1, 2) - 1;
					break;
				case 21: iTotalMob = iDice(1, 5) - 1;
					break;
				case 22: iTotalMob = iDice(1, 2) - 1;
					break;
				case 23: iTotalMob = iDice(1, 2) - 1;
					break;

				case 24: iTotalMob = iDice(1, 4) - 1;
					break;
				case 25: iTotalMob = iDice(1, 2) - 1;
					break;
				case 26: iTotalMob = iDice(1, 3) - 1;
					break;
				case 27: iTotalMob = iDice(1, 3) - 1;
					break;

				case 28: iTotalMob = iDice(1, 3) - 1;
					break;
				case 29: iTotalMob = iDice(1, 5) - 1;
					break;
				case 30: iTotalMob = iDice(1, 3) - 1;
					break;
				case 31: iTotalMob = iDice(1, 3) - 1;
					break;

				case 32: iTotalMob = 1;
					break;
				case 33: iTotalMob = 1;
					break;
				case 34: iTotalMob = 1;
					break;
				case 35: iTotalMob = 1;
					break;
				case 36: iTotalMob = 1;
					break;

				case 37: iTotalMob = 1;
					break;
				case 38: iTotalMob = 1;
					break;
				case 39: iTotalMob = 1;
					break;
				case 40: iTotalMob = 1;
					break;
				case 41: iTotalMob = 1;
					break;

				case 42: iTotalMob = iDice(1, 3) - 1;
					break;
				case 43: iTotalMob = 1;
					break;
				case 44: iTotalMob = iDice(1, 3) - 1;
					break;
				case 45: iTotalMob = 1;
					break;
				default: iTotalMob = 0;
					break;
			}

			if (bMaster == false) iTotalMob = 0;

			if (iTotalMob > 2) {
				switch (iResult) {
					case 1: // Slime
					case 2: // Giant-Ant
					case 3: // Orc
					case 4: // Zombie
					case 5: // Skeleton
					case 6: // Orc-Mage
					case 7: // Scorpion
					case 8: // Stone-Golem
					case 9: // Cyclops
					case 10: // Amphis
					case 11: // Clay-Golem
					case 12: // Troll
					case 13: // Orge
					case 14: // Hellbound
					case 15: // Liche
					case 16: // Demon
					case 17: // Unicorn
					case 18: // WereWolf
					case 19:
					case 20:
					case 21:
					case 22:
					case 23:
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
					case 30:
					case 31:
					case 32:
						if (iDice(1, 5) == 1) iTotalMob = 0; // 75% ???????? ???.
						break;

					case 33:
					case 34:
					case 35:
					case 36:
					case 37:
					case 38:
					case 39:
					case 40:
					case 41:
					case 42:
					case 44:
					case 45:
					case 46:
					case 47:
					case 48:
					case 49:
						if (iDice(1, 5) != 1) iTotalMob = 0; // 75% ???????????????
						break;
				}
			}

			if (bIsSpecialEvent == true) {
				switch (m_cSpecialEventType) {
					case 1:
						if ((iResult != 35) && (iResult != 36) && (iResult != 37) && (iResult != 49)
								  && (iResult != 51) && (iResult != 15) && (iResult != 16) && (iResult != 21)) iTotalMob = 12;
						for (x = 1; x < DEF_MAXCLIENTS; x++)
							if ((iNpcID != -1) && (m_pClientList[x] != nullptr) && (m_pClientList[x]->m_bIsInitComplete == true)) {
								SendNotifyMsg(0, x, DEF_NOTIFY_SPAWNEVENT, pX, pY, iNpcID, nullptr, 0, 0);
							}
						break;

					case 2:
						if ((memcmp(m_pMapList[i]->m_cLocationName, "aresden", 7) == 0) ||
								  (memcmp(m_pMapList[i]->m_cLocationName, "elvine", 6) == 0) ||
								  (memcmp(m_pMapList[i]->m_cLocationName, "elvfarm", 7) == 0) ||
								  (memcmp(m_pMapList[i]->m_cLocationName, "arefarm", 7) == 0)) {
							iTotalMob = 0;
						}
						break;
				}
				m_bIsSpecialEventTime = false;
			}

			for (j = 0; j < iTotalMob; j++) {
				iNamingValue = m_pMapList[i]->iGetEmptyNamingValue();
				if (iNamingValue != -1) {
					std::memset(cName_Slave, 0, sizeof (cName_Slave));
					wsprintf(cName_Slave, "XX%d", iNamingValue);
					cName_Slave[0] = 95; // original '_';
					cName_Slave[1] = i + 65;

					cSA = 0;

					if (iDice(1, 100) <= iProbSA) {
						cSA = _cGetSpecialAbility(iKindSA);
					}

					if (bCreateNewNpc(cNpcName, cName_Slave, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, bFirmBerserk) == false) {
						m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
					} else {
						bSetNpcFollowMode(cName_Slave, cName_Master, DEF_OWNERTYPE_NPC);
					}
				}
			}
		}

		if ((m_pMapList[i] != nullptr) && ((m_pMapList[i]->m_iMaximumObject) > m_pMapList[i]->m_iTotalActiveObject)) {
			for (j = 1; j < DEF_MAXSPOTMOBGENERATOR; j++)
				if ((iDice(1, 3) == 2) && (m_pMapList[i]->m_stSpotMobGenerator[j].bDefined == true) &&
						  (m_pMapList[i]->m_stSpotMobGenerator[j].iMaxMobs > m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs)) {
					iNamingValue = m_pMapList[i]->iGetEmptyNamingValue();
					if (iNamingValue != -1) {

						std::memset(cNpcName, 0, sizeof (cNpcName));
						switch (m_pMapList[i]->m_stSpotMobGenerator[j].iMobType) {
								// spot-mob-generator
							case 10: strcpy(cNpcName, "Slime");
								iProbSA = 5;
								iKindSA = 1;
								break;
							case 16: strcpy(cNpcName, "Giant-Ant");
								iProbSA = 10;
								iKindSA = 2;
								break;
							case 14: strcpy(cNpcName, "Orc");
								iProbSA = 15;
								iKindSA = 1;
								break;
							case 18: strcpy(cNpcName, "Zombie");
								iProbSA = 15;
								iKindSA = 3;
								break;
							case 11: strcpy(cNpcName, "Skeleton");
								iProbSA = 35;
								iKindSA = 8;
								break;
							case 6: strcpy(cNpcName, "Orc-Mage");
								iProbSA = 30;
								iKindSA = 7;
								break;
							case 17: strcpy(cNpcName, "Scorpion");
								iProbSA = 15;
								iKindSA = 3;
								break;
							case 12: strcpy(cNpcName, "Stone-Golem");
								iProbSA = 25;
								iKindSA = 5;
								break;
							case 13: strcpy(cNpcName, "Cyclops");
								iProbSA = 35;
								iKindSA = 8;
								break;
							case 22: strcpy(cNpcName, "Amphis");
								iProbSA = 20;
								iKindSA = 3;
								break;
							case 23: strcpy(cNpcName, "Clay-Golem");
								iProbSA = 20;
								iKindSA = 5;
								break;
							case 24: strcpy(cNpcName, "Guard-Aresden");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 25: strcpy(cNpcName, "Guard-Elvine");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 26: strcpy(cNpcName, "Guard-Neutral");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 27: strcpy(cNpcName, "Hellbound");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 29: strcpy(cNpcName, "Orge");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 30: strcpy(cNpcName, "Liche");
								iProbSA = 30;
								iKindSA = 8;
								break;
							case 31: strcpy(cNpcName, "Demon");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 32: strcpy(cNpcName, "Unicorn");
								iProbSA = 35;
								iKindSA = 7;
								break;
							case 33: strcpy(cNpcName, "WereWolf");
								iProbSA = 25;
								iKindSA = 1;
								break;
							case 34: strcpy(cNpcName, "Dummy");
								iProbSA = 5;
								iKindSA = 1;
								break;
							case 35: strcpy(cNpcName, "Attack-Dummy");
								iProbSA = 5;
								iKindSA = 1;
								break;
							case 48: strcpy(cNpcName, "Stalker");
								iProbSA = 20;
								iKindSA = 3;
								break;
								/*									case 49:
								if (iDice(1,144000) == 13) {
								strcpy(cNpcName,"Hellclaw");
								iProbSA = 20;
								iKindSA = 8;
								}
								break;
								case 50:
								if (iDice(1, 3606) == 135) {
								strcpy(cNpcName,"Tigerworm");
								iProbSA = 20;
								iKindSA = 8;
								}
								else {
								strcpy(cNpcName, "Stalker");
								iProbSA = 15;
								iKindSA = 1;
								}
								break;*/
							case 49: strcpy(cNpcName, "Hellclaw");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 50: strcpy(cNpcName, "Tigerworm");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 54: strcpy(cNpcName, "Dark-Elf");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 53: strcpy(cNpcName, "Beholder");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 52: strcpy(cNpcName, "Gagoyle");
								iProbSA = 20;
								iKindSA = 8;
								break;
								/*									case 40:
								if (iDice(1, 3606) == 135) {
								strcpy(cNpcName,"Hellclaw");
								iProbSA = 20;
								iKindSA = 8;
								}
								break;*/
							case 57: strcpy(cNpcName, "Giant-Frog");
								iProbSA = 10;
								iKindSA = 2;
								break;
							case 58: strcpy(cNpcName, "Mountain-Giant");
								iProbSA = 25;
								iKindSA = 1;
								break;
							case 59: strcpy(cNpcName, "Ettin");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 60: strcpy(cNpcName, "Cannibal-Plant");
								iProbSA = 20;
								iKindSA = 5;
								break;
							case 61: strcpy(cNpcName, "Rudolph");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 62: strcpy(cNpcName, "DireBoar");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 63: strcpy(cNpcName, "Frost");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 65: strcpy(cNpcName, "Ice-Golem");
								iProbSA = 20;
								iKindSA = 8;
								break;
							case 66: strcpy(cNpcName, "Wyvern");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 55: strcpy(cNpcName, "Rabbit");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 67: strcpy(cNpcName, "McGaffin");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 68: strcpy(cNpcName, "Perry");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 69: strcpy(cNpcName, "Devlin");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 73: strcpy(cNpcName, "Fire-Wyvern");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 70: strcpy(cNpcName, "Barlog");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 80: strcpy(cNpcName, "Tentocle");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 71: strcpy(cNpcName, "Centaurus");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 75: strcpy(cNpcName, "Giant-Lizard");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 78: strcpy(cNpcName, "Minotaurs");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 81: strcpy(cNpcName, "Abaddon");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 72: strcpy(cNpcName, "Claw-Turtle");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 74: strcpy(cNpcName, "Giant-Crayfish");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 76: strcpy(cNpcName, "Giant-Plant");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 77: strcpy(cNpcName, "MasterMage-Orc");
								iProbSA = 20;
								iKindSA = 1;
								break;
							case 79: strcpy(cNpcName, "Nizie");
								iProbSA = 20;
								iKindSA = 1;
								break;
							default:
								strcpy(cNpcName, "Orc");
								iProbSA = 15;
								iKindSA = 1;
								break;
						}
						/* NPCs not spawning in pits:
						case 56:  strcpy(cNpcName,"Cat");				iProbSA = 15; iKindSA = 6;  break;
						case 28:  strcpy(cNpcName, "Troll");			iProbSA = 25; iKindSA = 3; break;

						// 15 ShopKeeper-W
						// 19 Gandlf
						// 20 Howard
						// 36 Arrow Gaurd Tower Kit - Aresden, Elvine
						// 37 Cannon Gaurd Tower Kit - Aresden, Elvine
						// 38 Mana Collector Kit - Aresden, Elvine
						// 39 Detector Constructor Kit - Aresden, Elvine
						// 40 Energy Shield Generator - Aresden, Elvine
						// 41 Grand Master Generator - Aresden Elvine
						// 43 Light War Beetle - Aresden, Elvine
						// 44 God's Hand Knight
						// 45 Mounted God's Hand Knight
						// 46 Temple Knight
						// 47 Battle Golem
						// 51 Catapult
						// 64 Crops
						 */
						bFirmBerserk = false;
						if ((iMapLevel == 5) && (iDice(1, 3) == 1)) bFirmBerserk = true;

						std::memset(cName_Master, 0, sizeof (cName_Master));
						wsprintf(cName_Master, "XX%d", iNamingValue);
						cName_Master[0] = 95; // original '_';
						cName_Master[1] = i + 65;

						cSA = 0;
						if ((m_pMapList[i]->m_stSpotMobGenerator[j].iMobType != 34) && (iDice(1, 100) <= iProbSA)) {
							cSA = _cGetSpecialAbility(iKindSA);
						}

						switch (m_pMapList[i]->m_stSpotMobGenerator[j].cType) {
							case 1:
								if (bCreateNewNpc(cNpcName, cName_Master, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOMAREA, &pX, &pY, cWaypoint, &m_pMapList[i]->m_stSpotMobGenerator[j].rcRect, j, -1, false, false, bFirmBerserk) == false) {
									m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
								} else {
									m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs++;
								}
								break;

							case 2:
								if (bCreateNewNpc(cNpcName, cName_Master, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOMWAYPOINT, nullptr, nullptr, m_pMapList[i]->m_stSpotMobGenerator[j].cWaypoint, nullptr, j, -1, false, false, bFirmBerserk) == false) {
									m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
								} else {
									m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs++;
								}
								break;
						}
					}
				}
		}
	}
}

void CGame::CalcNextWayPointDestination(int iNpcH) {
	short sRange, sX, sY;
	register int i, j, iMapIndex;
	bool bFlag;

	switch (m_pNpcList[iNpcH]->m_cMoveType) {
		case DEF_MOVETYPE_GUARD:

			break;

		case DEF_MOVETYPE_SEQWAYPOINT:

			m_pNpcList[iNpcH]->m_cCurWaypoint++;
			if (m_pNpcList[iNpcH]->m_cCurWaypoint >= m_pNpcList[iNpcH]->m_cTotalWaypoint)
				m_pNpcList[iNpcH]->m_cCurWaypoint = 1;
			m_pNpcList[iNpcH]->m_dX = (short) (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[ m_pNpcList[iNpcH]->m_iWayPointIndex[ m_pNpcList[iNpcH]->m_cCurWaypoint ] ].x);
			m_pNpcList[iNpcH]->m_dY = (short) (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[ m_pNpcList[iNpcH]->m_iWayPointIndex[ m_pNpcList[iNpcH]->m_cCurWaypoint ] ].y);
			break;

		case DEF_MOVETYPE_RANDOMWAYPOINT:

			m_pNpcList[iNpcH]->m_cCurWaypoint = (short) ((rand() % (m_pNpcList[iNpcH]->m_cTotalWaypoint - 1)) + 1);
			m_pNpcList[iNpcH]->m_dX = (short) (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[ m_pNpcList[iNpcH]->m_iWayPointIndex[ m_pNpcList[iNpcH]->m_cCurWaypoint ] ].x);
			m_pNpcList[iNpcH]->m_dY = (short) (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[ m_pNpcList[iNpcH]->m_iWayPointIndex[ m_pNpcList[iNpcH]->m_cCurWaypoint ] ].y);
			break;

		case DEF_MOVETYPE_RANDOMAREA:

			sRange = (short) (m_pNpcList[iNpcH]->m_rcRandomArea.right - m_pNpcList[iNpcH]->m_rcRandomArea.left);
			m_pNpcList[iNpcH]->m_dX = (short) ((rand() % sRange) + m_pNpcList[iNpcH]->m_rcRandomArea.left);
			sRange = (short) (m_pNpcList[iNpcH]->m_rcRandomArea.bottom - m_pNpcList[iNpcH]->m_rcRandomArea.top);
			m_pNpcList[iNpcH]->m_dY = (short) ((rand() % sRange) + m_pNpcList[iNpcH]->m_rcRandomArea.top);
			break;

		case DEF_MOVETYPE_RANDOM:

			//m_pNpcList[iNpcH]->m_dX = (rand() % (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeX - 50)) + 15;
			//m_pNpcList[iNpcH]->m_dY = (rand() % (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeY - 50)) + 15;
			iMapIndex = m_pNpcList[iNpcH]->m_cMapIndex;

			for (i = 0; i <= 30; i++) {

				sX = (rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15;
				sY = (rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15;

				bFlag = true;
				for (j = 0; j < DEF_MAXMGAR; j++)
					if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].left != -1) {

						if ((sX >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].left) &&
								  (sX <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].right) &&
								  (sY >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].top) &&
								  (sY <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].bottom)) {

							bFlag = false;
						}
					}
				if (bFlag == true) goto CNW_GET_VALIDLOC_SUCCESS;
			}
			// Fail!
			m_pNpcList[iNpcH]->m_tmp_iError = 0;
			return;

CNW_GET_VALIDLOC_SUCCESS:
			;
			m_pNpcList[iNpcH]->m_dX = sX;
			m_pNpcList[iNpcH]->m_dY = sY;
			break;
	}

	m_pNpcList[iNpcH]->m_tmp_iError = 0; // @@@ !!! @@@
}

void CGame::DeleteNpc(int iNpcH) {
	int i, iNamingValue, iNumItem, iItemID, iItemIDs[MAX_NPCITEMDROP], iSlateID;
	char cTmp[21], cItemName[21];
	class CItem * pItem, * pItem2;
	uint32_t dwCount, dwTime;
	POINT ItemPositions[MAX_NPCITEMDROP];
	char cTemp[256];
	SYSTEMTIME SysTime;
	if (m_pNpcList[iNpcH] == nullptr) return;

	dwTime = timeGetTime();

	//Init number of items to 1 unless its a multidrop;
	iNumItem = 0;
	iItemID = 0; // No current item


	SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
	m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(11, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

	std::memset(cTmp, 0, sizeof (cTmp));
	strcpy(cTmp, (char *) (m_pNpcList[iNpcH]->m_cName + 2));

	iNamingValue = atoi(cTmp);


	m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
	m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalActiveObject--;


	if (m_pNpcList[iNpcH]->m_iSpotMobIndex != 0)
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stSpotMobGenerator[m_pNpcList[iNpcH]->m_iSpotMobIndex].iCurMobs--;

	RemoveFromTarget(iNpcH, DEF_OWNERTYPE_NPC);


	switch (m_pNpcList[iNpcH]->m_sType) {
		case 36:
		case 37:
		case 38:
		case 39:
		case 42:
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCrusadeStructureInfo(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

			for (i = 0; i < DEF_MAXGUILDS; i++)
				if (m_pGuildTeleportLoc[i].m_iV1 == m_pNpcList[iNpcH]->m_iGuildGUID) {
					m_pGuildTeleportLoc[i].m_dwTime = dwTime;
					m_pGuildTeleportLoc[i].m_iV2--;
					if (m_pGuildTeleportLoc[i].m_iV2 < 0) m_pGuildTeleportLoc[i].m_iV2 = 0;
					break;
				}
			break;
		case 64: m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCropsTotalSum();
			break;

	}


	delayEvents_.bRemoveFromDelayEventList(iNpcH, DEF_OWNERTYPE_NPC, 0);
	if ((m_pNpcList[iNpcH]->m_bIsSummoned == false) && (m_pNpcList[iNpcH]->m_bIsUnsummoned == false)) {

		pItem = new class CItem;
		std::memset(cItemName, 0, sizeof (cItemName));
		switch (m_pNpcList[iNpcH]->m_sType) {

			case 10: // Slime
				if (iDice(1, 25) == 1) iItemID = 220;
				break; // SlimeJelly 
				break;

			case 11: // Skeleton
				switch (iDice(1, 2)) {
					case 1:if (iDice(1, 20) == 1) iItemID = 219;
						break; // SkeletonBones
					case 2: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 12: // Stone-Golem
				switch (iDice(1, 2)) {
					case 1: if (iDice(1, 30) == 1) iItemID = 221;
						break; // StoneGolemPiece
					case 2: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 13: // Cyclops
				switch (iDice(1, 6)) {
					case 1: if (iDice(1, 36) == 1) iItemID = 194;
						break; // CyclopsEye
					case 2: if (iDice(1, 40) == 1) iItemID = 195;
						break; // CyclopsHandEdge
					case 3: if (iDice(1, 30) == 1) iItemID = 196;
						break; // CyclopsHeart
					case 4: if (iDice(1, 22) == 1) iItemID = 197;
						break; // CyclopsMeat
					case 5: if (iDice(1, 40) == 1) iItemID = 198;
						break; // CyclopsLeather
					case 6: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 14: // Orc, Orc-Mage
				switch (iDice(1, 4)) {
					case 1: if (iDice(1, 11) == 1) iItemID = 206;
						break; // OrcMeat
					case 2: if (iDice(1, 20) == 1) iItemID = 207;
						break; // OrcLeather
					case 3: if (iDice(1, 21) == 1) iItemID = 208;
						break; // OrcTeeth
					case 4: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 16: //Giant-Ant
				switch (iDice(1, 3)) {
					case 1: if (iDice(1, 9) == 1) iItemID = 192;
						break; // AntLeg
					case 2: if (iDice(1, 10) == 1) iItemID = 193;
						break; // AntFeeler
					case 3: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 17: //Scorpion
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 50) == 1) iItemID = 215;
						break; // ScorpionPincers
					case 2: if (iDice(1, 20) == 1) iItemID = 216;
						break; // ScorpionMeat
					case 3: if (iDice(1, 50) == 1) iItemID = 217;
						break; // ScorpionSting
					case 4: if (iDice(1, 40) == 1) iItemID = 218;
						break; // ScorpionSkin
					case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 18: //Zombie
				bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
				break;
				break;

			case 22: //Amphis
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 15) == 1) iItemID = 188;
						break; // SnakeMeat
					case 2: if (iDice(1, 16) == 1) iItemID = 189;
						break; // SnakeSkin
					case 3: if (iDice(1, 16) == 1) iItemID = 190;
						break; // SnakeTeeth
					case 4: if (iDice(1, 17) == 1) iItemID = 191;
						break; // SnakeTongue
					case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 23: //Clay-Golem
				switch (iDice(1, 2)) {
					case 1: if (iDice(1, 30) == 1) iItemID = 205;
						break; // LumpofClay
					case 2: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;


			case 27: //Hellbound
				switch (iDice(1, 7)) {
					case 1: if (iDice(1, 40) == 1) iItemID = 199;
						break; // HelboundHeart
					case 2: if (iDice(1, 38) == 1) iItemID = 200;
						break; // HelboundLeather
					case 3: if (iDice(1, 38) == 1) iItemID = 201;
						break; // HelboundTail
					case 4: if (iDice(1, 36) == 1) iItemID = 202;
						break; // HelboundTeeth
					case 5: if (iDice(1, 36) == 1) iItemID = 203;
						break; // HelboundClaw
					case 6: if (iDice(1, 50) == 1) iItemID = 204;
						break; // HelboundTongue
					case 7: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 28: //Troll
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 35) == 1) iItemID = 222;
						break; // TrollHeart
					case 2: if (iDice(1, 23) == 1) iItemID = 223;
						break; // TrollMeat
					case 3: if (iDice(1, 25) == 1) iItemID = 224;
						break; // TrollLeather
					case 4: if (iDice(1, 27) == 1) iItemID = 225;
						break; // TrollClaw
					case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 29: //Orge
				switch (iDice(1, 7)) {
					case 1: if (iDice(1, 20) == 1) iItemID = 209;
						break; // OgreHair
					case 2: if (iDice(1, 22) == 1) iItemID = 210;
						break; // OgreHeart
					case 3: if (iDice(1, 25) == 1) iItemID = 211;
						break; // OgreMeat
					case 4: if (iDice(1, 25) == 1) iItemID = 212;
						break; // OgreLeather
					case 5: if (iDice(1, 28) == 1) iItemID = 213;
						break; // OgreTeeth
					case 6: if (iDice(1, 28) == 1) iItemID = 214;
						break; // OgreClaw
					case 7: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 30: //Liche
				bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
				break;
				break;

			case 31: //Demon
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 400) == 123) iItemID = 541;
						break; // DemonHeart
					case 2: if (iDice(1, 1000) == 123) iItemID = 542;
						break; // DemonMeat
					case 3: if (iDice(1, 200) == 123) iItemID = 543;
						break; // DemonLeather
					case 4: if (iDice(1, 300) == 123) iItemID = 540;
						break; // DemonEye
					case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 32: //Unicorn
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 3000) == 396) iItemID = 544;
						break; // UnicornHeart
					case 2: if (iDice(1, 500) == 3) iItemID = 545;
						break; // UnicornHorn
					case 3: if (iDice(1, 100) == 3) iItemID = 546;
						break; // UnicornMeat
					case 4: if (iDice(1, 200) == 3) iItemID = 547;
						break; // UnicornLeather
					case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 33: //WereWolf
				switch (iDice(1, 8)) {
					case 1: if (iDice(1, 30) == 3) iItemID = 551;
						break; // WerewolfTail
					case 2: if (iDice(1, 28) == 3) iItemID = 548;
						break; // WerewolfHeart
					case 3: if (iDice(1, 25) == 3) iItemID = 550;
						break; // WerewolfMeat
					case 4: if (iDice(1, 35) == 3) iItemID = 553;
						break; // WerewolfLeather
					case 5: if (iDice(1, 28) == 3) iItemID = 552;
						break; // WerewolfTeeth
					case 6: if (iDice(1, 28) == 3) iItemID = 554;
						break; // WerewolfClaw
					case 7: if (iDice(1, 38) == 3) iItemID = 549;
						break; // WerewolfNail
					case 8: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
						break;
					default: break;
				}
				break;

			case 48: //Stalker
			case 49: //Hellclaw
			case 50: //Tigerworm
			case 52: //Gagoyle
			case 53: //Beholder
			case 54: //Dark-Elf
			case 55: //Rabbit
			case 56: //Cat
			case 57: //Giant-Frog
			case 58: //Mountain-Giant
			case 59: //Ettin
			case 60: //Cannibal-Plant
			case 61: //Rudolph
			case 62: //DireBoar
			case 63: //Frost
			case 65: //Ice
				bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
				break;

				// new 05/10/2004
			case 66: // Wyvern
				bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, // NPC Type
						  50,
						  5,
						  15,
						  m_pNpcList[iNpcH]->m_sX,
						  m_pNpcList[iNpcH]->m_sY,
						  DEF_ITEMSPREAD_FIXED,
						  4,
						  iItemIDs,
						  ItemPositions,
						  &iNumItem);
				break;

			case 73: // Fire-Wyvern
				bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, // NPC Type
						  50,
						  5,
						  15,
						  m_pNpcList[iNpcH]->m_sX,
						  m_pNpcList[iNpcH]->m_sY,
						  2,
						  4,
						  iItemIDs,
						  ItemPositions,
						  &iNumItem);
				break;
			case 81:
				bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, // NPC Type
						  50,
						  12,
						  20,
						  m_pNpcList[iNpcH]->m_sX,
						  m_pNpcList[iNpcH]->m_sY,
						  DEF_ITEMSPREAD_FIXED,
						  65,
						  iItemIDs,
						  ItemPositions,
						  &iNumItem);
				break;
		}

		dwCount = 1;
		if (iNumItem > 0) {
			GetLocalTime(&SysTime);
			wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
			for (int j = 0; j < iNumItem; j++) {
				if (pItem == nullptr) {
					pItem = new class CItem;
				}
				if (_bInitItemAttr(*pItem, iItemIDs[j]) == false ||
						  m_pMapList[ m_pNpcList[iNpcH]->m_cMapIndex ]->bGetIsMoveAllowedTile(ItemPositions[j].x, ItemPositions[j].y) == false) {
					delete pItem;
					pItem = nullptr;
				} else {
					if (iItemIDs[j] == 90) // Gold
						pItem->m_dwCount = iDice(10, 15000);
					else
						pItem->m_dwCount = dwCount;

					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = (short) timeGetTime();
					m_pMapList[ m_pNpcList[iNpcH]->m_cMapIndex ]->bSetItem(ItemPositions[j].x, ItemPositions[j].y, pItem);
					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
							  ItemPositions[j].x, ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
					_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem);
					pItem = nullptr;
				}
			}
		} else {
			if (_bInitItemAttr(*pItem, iItemID) == false) {
				delete pItem;
				pItem = nullptr;
			} else {
				pItem->m_dwCount = dwCount;

				pItem->m_sTouchEffectType = DEF_ITET_ID;
				pItem->m_sTouchEffectValue1 = iDice(1, 100000);
				pItem->m_sTouchEffectValue2 = iDice(1, 100000);
				pItem->m_sTouchEffectValue3 = (short) timeGetTime();
				m_pMapList[ m_pNpcList[iNpcH]->m_cMapIndex ]->bSetItem(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem);
				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
						  m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
				_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem);
			}
		}

		if (iDice(1, 100000) < 10) {
			pItem2 = new class CItem;
			switch (iDice(1, 4)) {
				case 1: iSlateID = 868;
					break;
				case 2: iSlateID = 869;
					break;
				case 3: iSlateID = 870;
					break;
				case 4: iSlateID = 871;
					break;
			}
			if (_bInitItemAttr(*pItem2, iSlateID) == false) {
				delete pItem2;
				pItem2 = nullptr;
			} else {
				pItem2->m_dwCount = 1;

				pItem2->m_sTouchEffectType = DEF_ITET_ID;
				pItem2->m_sTouchEffectValue1 = iDice(1, 100000);
				pItem2->m_sTouchEffectValue2 = iDice(1, 100000);
				pItem2->m_sTouchEffectValue3 = (short) timeGetTime();

				m_pMapList[ m_pNpcList[iNpcH]->m_cMapIndex ]->bSetItem(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem2);
				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
						  m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem2->m_sSprite, pItem2->m_sSpriteFrame, pItem2->m_cItemColor);
				_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem2);
			}
		}
	}

	delete m_pNpcList[iNpcH];
	m_pNpcList[iNpcH] = nullptr;
}

void CGame::RequestFullObjectData(CClient &client, char *pData) {
	uint32_t * dwp;
	uint16_t wObjectID;
	char * cp, cData[100];
	short * sp, sX, sY;
	int iTemp, iTemp2;
	int * ip, iRet;

	if (client.m_bIsInitComplete == false) return;

	uint16_t *wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	wObjectID = *wp;
	std::memset(cData, 0, sizeof (cData));
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_EVENT_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTSTOP;

	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

	if (wObjectID < 10000) {
		if ((wObjectID == 0) || (wObjectID >= DEF_MAXCLIENTS)) return;
		CClient *targetClientPtr = m_pClientList[wObjectID];
		if (!targetClientPtr) return;
		CClient &targetClient = *targetClientPtr;
		wp = (uint16_t *) cp;
		*wp = wObjectID;
		cp += 2;
		sp = (short *) cp;
		sX = targetClient.m_sX;
		*sp = sX;
		cp += 2;
		sp = (short *) cp;
		sY = targetClient.m_sY;
		*sp = sY;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sType;
		cp += 2;
		*cp = targetClient.m_cDir;
		cp++;
		memcpy(cp, targetClient.m_cCharName, 10);
		cp += 10;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr1;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr2;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr3;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr4;
		cp += 2;
		ip = (int *) cp;
		*ip = targetClient.m_iApprColor;
		cp += 4;
		ip = (int *) cp;
		iTemp = targetClient.m_iStatus;
		iTemp = 0x0FFFFFFF & iTemp;
		iTemp2 = iGetPlayerABSStatus(wObjectID, client.id_);
		iTemp = (iTemp | (iTemp2 << 28));
		*ip = iTemp;
		cp += 4;
		*cp = targetClient.m_bIsKilled;
		cp++;
		iRet = client.m_pXSock->iSendMsg(cData, 41);
	} else {
		wObjectID -= 10000;
		if (((wObjectID) == 0) || ((wObjectID) >= DEF_MAXNPCS)) return;
		CNpc *targetNpcPtr = m_pNpcList[wObjectID];
		if (!targetNpcPtr) return;
		CNpc &targetNpc = *targetNpcPtr;
		wp = (uint16_t *) cp;
		*wp = wObjectID;
		cp += 2;
		sp = (short *) cp;
		sX = targetNpc.m_sX;
		*sp = sX;
		cp += 2;
		sp = (short *) cp;
		sY = targetNpc.m_sY;
		*sp = sY;
		cp += 2;
		sp = (short *) cp;
		*sp = targetNpc.m_sType;
		cp += 2;
		*cp = targetNpc.m_cDir;
		cp++;
		memcpy(cp, targetNpc.m_cName, 5);
		cp += 5;
		sp = (short *) cp;
		*sp = targetNpc.m_sAppr2;
		cp += 2;

		ip = (int *) cp;
		iTemp = targetNpc.m_iStatus;
		iTemp = 0x0FFFFFFF & iTemp;

		iTemp2 = iGetNpcRelationship(wObjectID, client.id_);
		iTemp = (iTemp | (iTemp2 << 28)); //Original : 12
		*ip = iTemp;
		//*sp = DEF_TEST;
		cp += 4; //Original 2
		*cp = targetNpc.m_bIsKilled;
		cp++;
		iRet = client.m_pXSock->iSendMsg(cData, 27); // v1.4 //Original : 25
	}

	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:

			DeleteClient(client.id_, true, true);
			return;
	}
}

int CGame::_iGetArrowItemIndex(int iClientH) {
	register int i;

	if (m_pClientList[iClientH] == nullptr) return -1;

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {


			if ((m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_ARROW) &&
					  (m_pClientList[iClientH]->m_pItemList[i]->m_dwCount > 0))
				return i;
		}

	return -1;
}

void CGame::ItemDepleteHandler(int iClientH, short sItemIndex, bool bIsUseItemResult, bool bIsItemUsed) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return;
	if ((bIsItemUsed == 1) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_MATERIAL) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 380) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 381) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 382)) {
		_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, -1, &*m_pClientList[iClientH]->m_pItemList[sItemIndex], false);
	} else if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 247) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 248)) {
		_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, -1, &*m_pClientList[iClientH]->m_pItemList[sItemIndex], false);
	}
	ReleaseItemHandler(iClientH, sItemIndex, true);
	SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMDEPLETED_ERASEITEM, sItemIndex, (int) bIsUseItemResult, 0, nullptr);
	m_pClientList[iClientH]->m_pItemList[sItemIndex].reset();
	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;
	m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	iCalcTotalWeight(iClientH);
}

void CGame::NpcBehavior_Stop(int iNpcH) {
	char cTargetType;
	short sTarget = 0;
	bool bFlag;

	if (m_pNpcList[iNpcH] == nullptr) return;

	m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;

	switch (m_pNpcList[iNpcH]->m_cActionLimit) {
		case 5:
			switch (m_pNpcList[iNpcH]->m_sType) {
				case 38:
					if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						bFlag = _bNpcBehavior_ManaCollector(iNpcH);
						if (bFlag == true) {
							SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 1);
						}
					}
					break;

				case 39: // Detector
					if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						bFlag = _bNpcBehavior_Detector(iNpcH);

						if (bFlag == true) {
							// ���� �߰��ߴ�. ��� �������� �˷��� �Ѵ�.
							SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 1);
						}
					}
					break;

				case 40: // Energy Shield Generator
					break;

				case 41: // Grand Magic Generator
					if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						_NpcBehavior_GrandMagicGenerator(iNpcH);
					}
					break;

				case 42: // ManaStone: v2.05 ���������� ���������� �������� 5�� ���Ѵ�.
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[iNpcH]->m_iV1 += 5;
					if (m_pNpcList[iNpcH]->m_iV1 >= 5) m_pNpcList[iNpcH]->m_iV1 = 5;
					break;

				default:
					TargetSearch(iNpcH, &sTarget, &cTargetType);
					break;
			}
			break;
	}

	if ((sTarget != 0)) {

		// ��ݸ�ǥ �߰�.
		m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
		m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
		m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
		// ���⼭ ǥȿ ���۰������� ���� �޽��� �߼�.
		return;
	}
}

void CGame::UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID) {
	int iTemp, iMax, iV1, iV2, iV3, iSEV1, iEffectResult = 0;
	uint32_t dwTime;
	short sTemp, sTmpType, sTmpAppr1;
	char cSlateType[20];

	dwTime = timeGetTime();
	std::memset(cSlateType, 0, sizeof (cSlateType));

	//testcode
	//wsprintf(G_cTxt, "%d", sDestItemID);
	//PutLogList(G_cTxt);

	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return;

	if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_PERM) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_SKILL) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST)) {
	} else return;

	if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
			  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT)) {


		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_WARM:

				// �õ� ������ ��� �ص� �Ǿ�ٴ� �޼����� �����ش�.
				if (m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 1) {
					//	SetIceFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);

					delayEvents_.bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_ICE);

					// ȿ�� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (1 * 1000),
							  iClientH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);


					//				SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_ICE, nullptr, nullptr, nullptr);
				}

				m_pClientList[iClientH]->m_dwWarmEffectTime = dwTime;
				break;

			case DEF_ITEMEFFECTTYPE_LOTTERY:

				iTemp = iDice(1, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1);
				if (iTemp == iDice(1, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1)) {


				} else {
					// ²Î!

				}
				break;

			case DEF_ITEMEFFECTTYPE_SLATES:
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != nullptr) {
					// Full Ancient Slate ??
					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 867) {
						// Slates dont work on Heldenian Map
						switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2) {
							case 2: // Bezerk slate
								m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] = true;
								SetBerserkFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
								delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (1000 * 600),
										  iClientH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
								SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, 1, 0, nullptr);
								strcpy(cSlateType, "Berserk");
								break;

							case 1: // Invincible slate
								if (strlen(cSlateType) == 0) {
									strcpy(cSlateType, "Invincible");
								}
							case 3: // Mana slate
								if (strlen(cSlateType) == 0) {
									strcpy(cSlateType, "Mana");
								}
							case 4: // Exp slate
								if (strlen(cSlateType) == 0) {
									strcpy(cSlateType, "Exp");
								}
								SetSlateFlag(iClientH, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2, true);
								delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_ANCIENT_TABLET, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2,
										  dwTime + (1000 * 600), iClientH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
								switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2) {
									case 1:
										iEffectResult = 4;
										break;
									case 3:
										iEffectResult = 5;
										break;
									case 4:
										iEffectResult = 6;
										break;
								}
						}
						if (strlen(cSlateType) > 0)
							_bItemLog(DEF_ITEMLOG_USE, iClientH, strlen(cSlateType), &*m_pClientList[iClientH]->m_pItemList[sItemIndex]);
					}
				}
				break;
			case DEF_ITEMEFFECTTYPE_HP:
				iMax = iGetMaxHP(iClientH);
				if (m_pClientList[iClientH]->m_iHP < iMax) {

					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0) {
						iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
						iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
					} else {
						iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
						iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
						iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
					}

					m_pClientList[iClientH]->m_iHP += (iDice(iV1, iV2) + iV3);
					if (m_pClientList[iClientH]->m_iHP > iMax) m_pClientList[iClientH]->m_iHP = iMax;
					if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 1;

					iEffectResult = 1;
				}
				break;

			case DEF_ITEMEFFECTTYPE_MP:
				iMax = iGetMaxMP(iClientH);
				if (m_pClientList[iClientH]->m_iMP < iMax) {

					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0) {
						iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
						iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
					} else {
						iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
						iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
						iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
					}

					m_pClientList[iClientH]->m_iMP += (iDice(iV1, iV2) + iV3);
					if (m_pClientList[iClientH]->m_iMP > iMax)
						m_pClientList[iClientH]->m_iMP = iMax;

					iEffectResult = 2;
				}
				break;

			case DEF_ITEMEFFECTTYPE_SP:
				iMax = iGetMaxSP(iClientH);
				if (m_pClientList[iClientH]->m_iSP < iMax) {

					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0) {
						iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
						iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
					} else {
						iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
						iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
						iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
					}

					m_pClientList[iClientH]->m_iSP += (iDice(iV1, iV2) + iV3);
					if (m_pClientList[iClientH]->m_iSP > iMax)
						m_pClientList[iClientH]->m_iSP = iMax;

					iEffectResult = 3;
				}

				if (m_pClientList[iClientH]->m_bIsPoisoned == true) {

					m_pClientList[iClientH]->m_bIsPoisoned = false;

					SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, false); // removes poison aura when using a revitalizing potion
					SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, nullptr);
				}
				break;

			case DEF_ITEMEFFECTTYPE_HPSTOCK:
				iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
				iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;

				m_pClientList[iClientH]->m_iHPstock += iDice(iV1, iV2) + iV3;
				if (m_pClientList[iClientH]->m_iHPstock < 0) m_pClientList[iClientH]->m_iHPstock = 0;
				if (m_pClientList[iClientH]->m_iHPstock > 500) m_pClientList[iClientH]->m_iHPstock = 500;


				m_pClientList[iClientH]->m_iHungerStatus += iDice(iV1, iV2) + iV3;
				if (m_pClientList[iClientH]->m_iHungerStatus > 100) m_pClientList[iClientH]->m_iHungerStatus = 100;
				if (m_pClientList[iClientH]->m_iHungerStatus < 0) m_pClientList[iClientH]->m_iHungerStatus = 0;
				break;

			case DEF_ITEMEFFECTTYPE_REPPLUS:
				iMax = 10000;
				if (m_pClientList[iClientH]->m_iRating < iMax) m_pClientList[iClientH]->m_iRating += 1;
				iEffectResult = 7;
				break;

			case DEF_ITEMEFFECTTYPE_STUDYSKILL:

				iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
				iSEV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;

				if (iSEV1 == 0) {

					TrainSkillResponse(true, iClientH, iV1, iV2);
				} else {
					TrainSkillResponse(true, iClientH, iV1, iSEV1);
				}
				break;

			case DEF_ITEMEFFECTTYPE_STUDYMAGIC:

				iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				if (m_pMagicConfigList[iV1] != nullptr)
					RequestStudyMagicHandler(iClientH, m_pMagicConfigList[iV1]->m_cName, false);
				break;

			case DEF_ITEMEFFECTTYPE_ADDBALLPOINTS:
				char cInfoString[56];
				iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				m_pClientList[iClientH]->m_iBallPoints += iV1;

				wsprintf(cInfoString, "%d Ball Points added. Total Amount: %d ", iV1, m_pClientList[iClientH]->m_iBallPoints);
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);

				break;

				/*case DEF_ITEMEFFECTTYPE_LOTTERY:
					iLottery = iDice(1, m_pClientList[iClientH]->m_pItemList[sItemIndex]->
					break;*/

				// New 15/05/2004 Changed
			case DEF_ITEMEFFECTTYPE_MAGIC:

				if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0) {
					if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
						SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);

						delayEvents_.bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
						m_pClientList[ iClientH ]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
					}
				}

				switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) {
					case 1:

						// testcode
						if (bCheckIfIsFlagCarrier(iClientH)) ShowClientMsg(iClientH, "You can not use that item being a flag carrier.");
						else RequestTeleportHandler(iClientH, "1   ");
						break;

					case 2:

						if (bCheckIfIsFlagCarrier(iClientH)) ShowClientMsg(iClientH, "You can not use that item being a flag carrier.");
						else PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 32, true);
						break;

					case 3:

						if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == false)
							PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 34, true);
						break;

					case 4:

						switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2) {
							case 1:

								if (bCheckIfIsFlagCarrier(iClientH)) ShowClientMsg(iClientH, "You can not use that item being a flag carrier.");
								else {
									if (memcmp(m_pClientList[iClientH]->m_cMapName, "bisle", 5) != 0) {
										ItemDepleteHandler(iClientH, sItemIndex, true, true);
										RequestTeleportHandler(iClientH, "2   ", "bisle", -1, -1);
									}
								}

								break;


							case 11:
							case 12:
							case 13:
							case 14:
							case 15:
							case 16:
							case 17:
							case 18:
							case 19:

								SYSTEMTIME SysTime;

								GetLocalTime(&SysTime);


								if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue1 != SysTime.wMonth) ||
										  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue2 != SysTime.wDay) ||
										  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue3 <= SysTime.wHour)) {

								} else {
									char cDestMapName[11];
									std::memset(cDestMapName, 0, sizeof (cDestMapName));
									wsprintf(cDestMapName, "fightzone%d", m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 - 10);
									if (memcmp(m_pClientList[iClientH]->m_cMapName, cDestMapName, 10) != 0) {
										//v1.42
										ItemDepleteHandler(iClientH, sItemIndex, true, true);
										RequestTeleportHandler(iClientH, "2   ", cDestMapName, -1, -1);
									}
								}
								break;
						}
						break;

					case 5:
						// new
						if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] == 0) break;
						if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "GodH", 4) == 0) break;
						if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 > 8) {
							if ((m_bHeldenianInitiated == true) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == true)) {
								PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, true,
										  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
							}
						}
						PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, true,
								  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
						break;
				}
				break;

			case DEF_ITEMEFFECTTYPE_FIRMSTAMINAR:
				m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar > 20 * 30) m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar = 20 * 30;
				break;

			case DEF_ITEMEFFECTTYPE_CHANGEATTR:
				switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) {
					case 1:

						m_pClientList[iClientH]->m_cHairColor++;
						if (m_pClientList[iClientH]->m_cHairColor > 15) m_pClientList[iClientH]->m_cHairColor = 0;

						sTemp = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
						m_pClientList[iClientH]->m_sAppr1 = sTemp;
						break;

					case 2:

						m_pClientList[iClientH]->m_cHairStyle++;
						if (m_pClientList[iClientH]->m_cHairStyle > 7) m_pClientList[iClientH]->m_cHairStyle = 0;

						sTemp = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
						m_pClientList[iClientH]->m_sAppr1 = sTemp;
						break;

					case 3:


						m_pClientList[iClientH]->m_cSkin++;
						if (m_pClientList[iClientH]->m_cSkin > 3)
							m_pClientList[iClientH]->m_cSkin = 1;

						if (m_pClientList[iClientH]->m_cSex == 1) sTemp = 1;
						else if (m_pClientList[iClientH]->m_cSex == 2) sTemp = 4;

						switch (m_pClientList[iClientH]->m_cSkin) {
							case 2: sTemp += 1;
								break;
							case 3: sTemp += 2;
								break;
						}
						m_pClientList[iClientH]->m_sType = sTemp;
						break;

					case 4:

						sTemp = m_pClientList[iClientH]->m_sAppr3 & 0xFF0F;
						if (sTemp == 0) {

							if (m_pClientList[iClientH]->m_cSex == 1)
								m_pClientList[iClientH]->m_cSex = 2;
							else m_pClientList[iClientH]->m_cSex = 1;


							if (m_pClientList[iClientH]->m_cSex == 1) {

								sTmpType = 1;
							} else if (m_pClientList[iClientH]->m_cSex == 2) {

								sTmpType = 4;
							}

							switch (m_pClientList[iClientH]->m_cSkin) {
								case 1:

									break;
								case 2:
									sTmpType += 1;
									break;
								case 3:
									sTmpType += 2;
									break;
							}

							sTmpAppr1 = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
							m_pClientList[iClientH]->m_sType = sTmpType;
							m_pClientList[iClientH]->m_sAppr1 = sTmpAppr1;
							//
						}
						break;
				}

				SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
				break;
		}


		ItemDepleteHandler(iClientH, sItemIndex, true, true);

		switch (iEffectResult) {
			case 1:
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
				break;
			case 2:
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
				break;
			case 3:
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				break;
			case 4: // Invincible
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_INVINCIBLE, 0, 0, 0, nullptr);
				break;
			case 5: // Mana
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_MANA, 0, 0, 0, nullptr);
				break;
			case 6: // EXP
				SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_EXP, 0, 0, 0, nullptr);
				break;
			default:
				break;
			case 7: //Rep
				char cRepMessage[60];
				wsprintf(cRepMessage, "You have Earned 1 Rep Point.");
				ShowClientMsg(iClientH, cRepMessage);
				break;
		}
	} else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) {


		if (_bDepleteDestTypeItemUseEffect(iClientH, dX, dY, sItemIndex, sDestItemID) == true)
			ItemDepleteHandler(iClientH, sItemIndex, true, true);
	} else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) {

		m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	} else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_PERM) {

		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_SHOWLOCATION:
				iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				switch (iV1) {
					case 1:

						if (strcmp(m_pClientList[iClientH]->m_cMapName, "aresden") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 1, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvine") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 2, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "middleland") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 3, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "default") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 4, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone2") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 5, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone1") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 6, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone4") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 7, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone3") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 8, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "arefarm") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 9, 0, nullptr);
						else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvfarm") == 0)
							SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 10, 0, nullptr);
						else SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 0, 0, nullptr);
						break;
				}
				break;
		}
	} else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_SKILL) {


		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) ||
				  (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0) ||
				  (m_pClientList[iClientH]->m_bSkillUsingStatus[ m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill ] == true)) {

			return;
		} else {
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan != 0) {

				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan--;
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0) {


					SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMLIFESPANEND, DEF_EQUIPPOS_NONE, sItemIndex, 0, nullptr);
				} else {

					int iSkillUsingTimeID = (int) timeGetTime();

					delayEvents_.bRegisterDelayEvent(DEF_DELAYEVENTTYPE_USEITEM_SKILL, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill,
							  dwTime + m_pSkillConfigList[ m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill ]->m_sValue2 * 1000,
							  iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_cMapIndex, dX, dY,
							  m_pClientList[iClientH]->m_cSkillMastery[ m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill ], iSkillUsingTimeID, 0);


					m_pClientList[iClientH]->m_bSkillUsingStatus[ m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill ] = true;
					m_pClientList[iClientH]->m_iSkillUsingTimeID[ m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill ] = iSkillUsingTimeID; //v1.12
				}
			}
		}
	}
}

void CGame::Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, bool bExp, int iAttr) {
	int iPartyID, iDamage, iSideCondition, iIndex, iRemainLife, iTemp, iExp, iMaxSuperAttack, iRepDamage;
	char cAttackerSide, cDamageMoveDir;
	uint32_t dwTime;
	register double dTmp1, dTmp2, dTmp3;
	short sAtkX, sAtkY, sTgtX, sTgtY, dX, dY, sItemIndex;

	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;

	if (cAttackerType == DEF_OWNERTYPE_NPC)
		if (m_pNpcList[sAttackerH] == nullptr) return;

	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex] != 0) &&
			  (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsHeldenianMap == 1) && (m_bHeldenianInitiated == true)) return;

	dwTime = timeGetTime();
	iDamage = iDice(sV1, sV2) + sV3;
	if (iDamage <= 0) iDamage = 0;

	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER:
			if ((m_bAdminSecurity == true) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return;
			if (m_pClientList[sAttackerH]->m_cHeroArmourBonus == 2) iDamage += 4;
			if ((m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] == -1) || (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] == -1)) {
				sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
				if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {

					if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 861 || m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 862) {
						float damageTemp = (float) iDamage;
						damageTemp *= 1.5; // O el valor con punto flotante que ustedes dispongan...

						iDamage = (int) damageTemp;
					}

					if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 863 || m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 864) {
						if (m_pClientList[sAttackerH]->m_iRating > 0) {
							iRepDamage = m_pClientList[sAttackerH]->m_iRating / 100;
							if (iRepDamage < 5) iRepDamage = 5;
							iDamage += iRepDamage;
						}
						if (cTargetType == DEF_OWNERTYPE_PLAYER) {
							if (m_pClientList[sTargetH] != nullptr) {
								if (m_pClientList[sTargetH]->m_iRating < 0) {
									iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 10);
									if (iRepDamage > 10) iRepDamage = 10;
									iDamage += iRepDamage;
								}
							}
						}
					}
				}
				sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
				if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
					if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 859) { // NecklaceOfKloness  
						if (cTargetType == DEF_OWNERTYPE_PLAYER) {
							if (m_pClientList[sTargetH] != nullptr) {
								iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 20);
								if (iRepDamage > 5) iRepDamage = 5;
								iDamage += iRepDamage;
							}
						}
					}
				}
			}

			if ((m_bIsCrusadeMode == false) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true) && (cTargetType == DEF_OWNERTYPE_PLAYER)) return;

			dTmp1 = (double) iDamage;
			if ((m_pClientList[sAttackerH]->m_iMag + m_pClientList[sAttackerH]->m_iAngelicMag) <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double) (m_pClientList[sAttackerH]->m_iMag + m_pClientList[sAttackerH]->m_iAngelicMag);
			dTmp2 = dTmp2 / 3.3f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iDamage = (int) (dTmp3 + 0.5f);

			iDamage += m_pClientList[sAttackerH]->m_iAddMagicalDamage;
			if (iDamage <= 0) iDamage = 0;

			if (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone == true)
				iDamage += iDamage / 3;

			if (bCheckHeldenianMap(sAttackerH, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1) {
				iDamage += iDamage / 3;
			}

			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == true) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
				if (m_pClientList[sAttackerH]->m_iLevel <= 80) {
					iDamage += (iDamage * 7) / 10;
				} else if (m_pClientList[sAttackerH]->m_iLevel <= 100) {
					iDamage += iDamage / 2;
				} else
					iDamage += iDamage / 3;
			}


			cAttackerSide = m_pClientList[sAttackerH]->m_cSide;
			sAtkX = m_pClientList[sAttackerH]->m_sX;
			sAtkY = m_pClientList[sAttackerH]->m_sY;
			iPartyID = m_pClientList[sAttackerH]->m_iPartyID;
			break;

		case DEF_OWNERTYPE_NPC:
			cAttackerSide = m_pNpcList[sAttackerH]->m_cSide;
			sAtkX = m_pNpcList[sAttackerH]->m_sX;
			sAtkY = m_pNpcList[sAttackerH]->m_sY;
			break;
	}

	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:

			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsInitComplete == false) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;

			if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return;

			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == false) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;

			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsOwnLocation == true)) return;

			if ((dwTime - m_pClientList[sTargetH]->m_dwTime) > DEF_RAGPROTECTIONTIME) return;
			if ((m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->m_bIsAttackEnabled == false) && (m_pClientList[sTargetH]->m_iAdminUserLevel == 0)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsNeutral == true) && (m_pClientList[sTargetH]->m_iPKCount == 0)) return;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;



			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				if (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) {
					iSideCondition = iGetPlayerRelationship(sAttackerH, sTargetH);
					if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {

					} else {
						if (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone == true) {
							if (m_pClientList[sAttackerH]->m_iGuildGUID != m_pClientList[sTargetH]->m_iGuildGUID) {

							} else return;
						} else return;
					}
				}

				if (m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->iGetAttribute(m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY, 0x00000005) != 0) return;
			}

			ClearSkillUsingStatus(sTargetH);

			switch (iAttr) {
				case 1:
					if (m_pClientList[sTargetH]->m_iAddAbsEarth != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsEarth;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;

				case 2:
					if (m_pClientList[sTargetH]->m_iAddAbsAir != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsAir;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;

				case 3:
					if (m_pClientList[sTargetH]->m_iAddAbsFire != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsFire;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;

				case 4:
					if (m_pClientList[sTargetH]->m_iAddAbsWater != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsWater;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;

				default: break;
			}

			iIndex = m_pClientList[sTargetH]->m_iMagicDamageSaveItemIndex;
			if ((iIndex != -1) && (iIndex >= 0) && (iIndex < DEF_MAXITEMS)) {

				switch (m_pClientList[sTargetH]->m_pItemList[iIndex]->m_sIDnum) {
					case 335:
						dTmp1 = (double) iDamage;
						dTmp2 = dTmp1 * 0.2f;
						dTmp3 = dTmp1 - dTmp2;
						iDamage = (int) (dTmp3 + 0.5f);
						break;

					case 337:
						dTmp1 = (double) iDamage;
						dTmp2 = dTmp1 * 0.1f;
						dTmp3 = dTmp1 - dTmp2;
						iDamage = (int) (dTmp3 + 0.5f);
						break;
				}
				if (iDamage <= 0) iDamage = 0;

				iRemainLife = m_pClientList[sTargetH]->m_pItemList[iIndex]->m_wCurLifeSpan;
				if (iRemainLife <= iDamage) {
					ItemDepleteHandler(sTargetH, iIndex, true, true);
				} else {
					m_pClientList[sTargetH]->m_pItemList[iIndex]->m_wCurLifeSpan -= iDamage;
				}
			}

			if (m_pClientList[sTargetH]->m_iAddAbsMD != 0) {
				dTmp1 = (double) iDamage;
				dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsMD;
				dTmp3 = (dTmp2 / 100.0f) * dTmp1;
				iDamage = iDamage - (int) dTmp3;
			}

			if (cTargetType == DEF_OWNERTYPE_PLAYER) {
				iDamage -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
				if (iDamage <= 0) iDamage = 0;
			}

			if ((m_pClientList[sTargetH]->m_bIsLuckyEffect == true) &&
					  (iDice(1, 10) == 5) && (m_pClientList[sTargetH]->m_iHP <= iDamage)) {
				iDamage = m_pClientList[sTargetH]->m_iHP - 1;
			}

			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
				iDamage = iDamage / 2;

			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true)) {
				switch (m_pClientList[sTargetH]->m_iSpecialAbilityType) {
					case 51:
					case 52:
						return;
				}
			}

			m_pClientList[sTargetH]->m_iHP -= iDamage;
			if (m_pClientList[sTargetH]->m_iHP <= 0) {
				ClientKilledHandler(sTargetH, sAttackerH, cAttackerType, iDamage);
			} else {
				if (iDamage > 0) {
					if (m_pClientList[sTargetH]->m_iAddTransMana > 0) {
						dTmp1 = (double) m_pClientList[sTargetH]->m_iAddTransMana;
						dTmp2 = (double) iDamage;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2 + 1.0f;

						iTemp = (2 * (m_pClientList[sTargetH]->m_iMag + m_pClientList[sTargetH]->m_iAngelicMag)) + (2 * m_pClientList[sTargetH]->m_iLevel) + ((m_pClientList[sTargetH]->m_iInt + m_pClientList[sTargetH]->m_iAngelicInt) / 2);
						m_pClientList[sTargetH]->m_iMP += (int) dTmp3;
						if (m_pClientList[sTargetH]->m_iMP > iTemp) m_pClientList[sTargetH]->m_iMP = iTemp;
					}

					if (m_pClientList[sTargetH]->m_iAddChargeCritical > 0) {
						if (iDice(1, 100) < (m_pClientList[sTargetH]->m_iAddChargeCritical)) {
							iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
							if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
							SendNotifyMsg(0, sTargetH, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
						}
					}

					SendNotifyMsg(0, sTargetH, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
					SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);

					if (m_pClientList[sTargetH]->m_bSkillUsingStatus[19] != true) {
						m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->ClearOwner(0, sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
					}

					if (m_pClientList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {
						SendNotifyMsg(0, sTargetH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ], 0, nullptr);
						m_pClientList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
						delayEvents_.bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
					}
				}
			}

			sTgtX = m_pClientList[sTargetH]->m_sX;
			sTgtY = m_pClientList[sTargetH]->m_sY;
			break;

		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if ((m_bIsCrusadeMode == true) && (cAttackerSide == m_pNpcList[sTargetH]->m_cSide)) return;

			sTgtX = m_pNpcList[sTargetH]->m_sX;
			sTgtY = m_pNpcList[sTargetH]->m_sY;

			switch (m_pNpcList[sTargetH]->m_cActionLimit) {
				case 1:
				case 2:
					return;

				case 4:
					if (sTgtX == sAtkX) {
						if (sTgtY == sAtkY) return;
						else if (sTgtY > sAtkY) cDamageMoveDir = 5;
						else if (sTgtY < sAtkY) cDamageMoveDir = 1;
					} else if (sTgtX > sAtkX) {
						if (sTgtY == sAtkY) cDamageMoveDir = 3;
						else if (sTgtY > sAtkY) cDamageMoveDir = 4;
						else if (sTgtY < sAtkY) cDamageMoveDir = 2;
					} else if (sTgtX < sAtkX) {
						if (sTgtY == sAtkY) cDamageMoveDir = 7;
						else if (sTgtY > sAtkY) cDamageMoveDir = 6;
						else if (sTgtY < sAtkY) cDamageMoveDir = 8;
					}

					dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, nullptr) == false) {
						cDamageMoveDir = iDice(1, 8);
						dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
						if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, nullptr) == false) return;
					}

					m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
					m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
					m_pNpcList[sTargetH]->m_sX = dX;
					m_pNpcList[sTargetH]->m_sY = dY;
					m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;

					SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);

					dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, nullptr) == false) {
						cDamageMoveDir = iDice(1, 8);
						dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, nullptr) == false) return;
					}

					m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
					m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
					m_pNpcList[sTargetH]->m_sX = dX;
					m_pNpcList[sTargetH]->m_sY = dY;
					m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;

					SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);

					if (bCheckEnergySphereDestination(sTargetH, sAttackerH, cAttackerType) == true) {
						DeleteNpc(sTargetH);
					}
					return;
			}

			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				switch (m_pNpcList[sTargetH]->m_sType) {
					case 40:
					case 41:
						if ((m_pClientList[sAttackerH]->m_cSide == 0) || (m_pNpcList[sTargetH]->m_cSide == m_pClientList[sAttackerH]->m_cSide)) return;
						break;
				}
			}

			switch (m_pNpcList[sTargetH]->m_sType) {
				case 67: // McGaffin
				case 68: // Perry
				case 69: // Devlin
					return;
			}

			if (m_pNpcList[sTargetH]->m_iAbsDamage > 0) {
				dTmp1 = (double) iDamage;
				dTmp2 = (double) (m_pNpcList[sTargetH]->m_iAbsDamage) / 100.0f;
				dTmp3 = dTmp1 * dTmp2;
				dTmp2 = dTmp1 - dTmp3;
				iDamage = (int) dTmp2;
				if (iDamage < 0) iDamage = 1;
			}

			if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
				iDamage = iDamage / 2;

			m_pNpcList[sTargetH]->m_iHP -= iDamage;
			if (m_pNpcList[sTargetH]->m_iHP < 0) {
				NpcKilledHandler(sAttackerH, cAttackerType, sTargetH, iDamage);
			} else {
				switch (cAttackerType) {
					case DEF_OWNERTYPE_PLAYER:
						if ((m_pNpcList[sTargetH]->m_sType != 21) && (m_pNpcList[sTargetH]->m_sType != 55) && (m_pNpcList[sTargetH]->m_sType != 56)
								  && (m_pNpcList[sTargetH]->m_cSide == cAttackerSide)) return;
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide) return;
						break;
				}

				SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);

				if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0)) {
					if ((cAttackerType == DEF_OWNERTYPE_NPC) &&
							  (m_pNpcList[sAttackerH]->m_sType == m_pNpcList[sTargetH]->m_sType) &&
							  (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide)) return;

					m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
					m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
					m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;

					m_pNpcList[sTargetH]->m_dwTime = dwTime;

					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
						delayEvents_.bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
					}

					if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) &&
							  (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
						if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
							iExp = iDamage;
							if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;

							if (m_pClientList[sAttackerH]->m_iAddExp > 0) {
								dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double) iExp;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iExp += (int) dTmp3;
							}

							if (m_pClientList[sAttackerH]->m_iLevel > 100) {
								switch (m_pNpcList[sTargetH]->m_sType) {
									case 55:
									case 56:
										iExp = 0;
										break;
									default: break;
								}
							}

							if (bExp == true)
								GetExp(sAttackerH, iExp, true);
							else GetExp(sAttackerH, (iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
						} else {
							iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;

							if (m_pClientList[sAttackerH]->m_iAddExp > 0) {
								dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double) iExp;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iExp += (int) dTmp3;
							}

							if (m_pClientList[sAttackerH]->m_iLevel > 100) {
								switch (m_pNpcList[sTargetH]->m_sType) {
									case 55:
									case 56:
										iExp = 0;
										break;
									default: break;
								}
							}

							if (bExp == true)
								GetExp(sAttackerH, iExp, true);
							else GetExp(sAttackerH, (iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
					}
				}
			}
			break;
	}
}

void CGame::processDelayedEvent(CDelayEvent &ev) {
	int iTemp;
	int iSkillNum, iResult;
	switch (ev.m_iDelayType) {

		case DEF_DELAYEVENTTYPE_ANCIENT_TABLET:
			if ((m_pClientList[ev.m_iTargetH]->m_iStatus & 0x400000) != 0) {
				iTemp = 1;
			} else if ((m_pClientList[ev.m_iTargetH]->m_iStatus & 0x800000) != 0) {
				iTemp = 3;
			} else if ((m_pClientList[ev.m_iTargetH]->m_iStatus & 0x10000) != 0) {
				iTemp = 4;
			}

			SendNotifyMsg(0, ev.m_iTargetH, DEF_NOTIFY_SLATE_STATUS, iTemp, 0, 0, nullptr);
			SetSlateFlag(ev.m_iTargetH, iTemp, false);
			break;

		case DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT:
			CalcMeteorStrikeEffectHandler(ev.m_cMapIndex);
			break;

		case DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE:
			DoMeteorStrikeDamageHandler(ev.m_cMapIndex);
			break;

		case DEF_DELAYEVENTTYPE_METEORSTRIKE:
			MeteorStrikeHandler(ev.m_cMapIndex);
			break;

		case DEF_DELAYEVENTTYPE_USEITEM_SKILL:

			switch (ev.m_cTargetType) {
				case DEF_OWNERTYPE_PLAYER:
					iSkillNum = ev.m_iEffectType;

					if (m_pClientList[ev.m_iTargetH] == nullptr) break;

					if (m_pClientList[ev.m_iTargetH]->m_bSkillUsingStatus[iSkillNum] == false) break;

					if (m_pClientList[ev.m_iTargetH]->m_iSkillUsingTimeID[iSkillNum] != ev.m_iV2) break;


					m_pClientList[ev.m_iTargetH]->m_bSkillUsingStatus[iSkillNum] = false;
					m_pClientList[ev.m_iTargetH]->m_iSkillUsingTimeID[iSkillNum] = 0;


					iResult = iCalculateUseSkillItemEffect(ev.m_iTargetH, ev.m_cTargetType,
							  ev.m_iV1, iSkillNum, ev.m_cMapIndex, ev.m_dX, ev.m_dY);


					SendNotifyMsg(0, ev.m_iTargetH, DEF_NOTIFY_SKILLUSINGEND, iResult, 0, 0, nullptr);
					break;
			}
			break;

		case DEF_DELAYEVENTTYPE_DAMAGEOBJECT:
			break;

		case DEF_DELAYEVENTTYPE_MAGICRELEASE:
			// Removes the aura after time
			switch (ev.m_cTargetType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[ ev.m_iTargetH ] == nullptr) break;

					SendNotifyMsg(0, ev.m_iTargetH, DEF_NOTIFY_MAGICEFFECTOFF,
							  ev.m_iEffectType, m_pClientList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.m_iEffectType ], 0, nullptr);

					m_pClientList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.m_iEffectType ] = 0;

					// Inbitition casting
					if (ev.m_iEffectType == DEF_MAGICTYPE_INHIBITION)
						m_pClientList[ev.m_iTargetH]->m_bInhibition = false;

					// Invisibility ȿ�� ����
					if (ev.m_iEffectType == DEF_MAGICTYPE_INVISIBILITY)
						SetInvisibilityFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

					// Berserk ȿ�� ����
					if (ev.m_iEffectType == DEF_MAGICTYPE_BERSERK)
						SetBerserkFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

					// Confusion
					if (ev.m_iEffectType == DEF_MAGICTYPE_CONFUSE)
						switch (ev.m_iV1) {
							case 3: SetIllusionFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
							case 4: SetIllusionMovementFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
						}

					// Protection Magic
					if (ev.m_iEffectType == DEF_MAGICTYPE_PROTECT) {
						switch (ev.m_iV1) {
							case 1:
								SetProtectionFromArrowFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
						}
					}


					// polymorph ȿ�� ����
					if (ev.m_iEffectType == DEF_MAGICTYPE_POLYMORPH) {
						m_pClientList[ev.m_iTargetH]->m_sType = m_pClientList[ev.m_iTargetH]->m_sOriginalType;
						SendEventToNearClient_TypeA(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}

					// Ice ȿ�� ����
					if (ev.m_iEffectType == DEF_MAGICTYPE_ICE)
						SetIceFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
					break;

					/*case DEF_DELAYEVENTTYPE_MAGICRELEASE:
					// Removes the aura after time
					switch (ev.m_cTargetType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[ ev.m_iTargetH ] == nullptr) break;

						SendNotifyMsg(0, ev.m_iTargetH, DEF_NOTIFY_MAGICEFFECTOFF,
										 ev.m_iEffectType, m_pClientList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.m_iEffectType ], 0, nullptr);

						m_pClientList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.m_iEffectType ] = nullptr;

						// Inhibition Casting
						if (ev.m_iEffectType == DEF_MAGICTYPE_INHIBITION)
							m_pClientList[ev.m_iTargetH]->m_bInhibition = false;

								
						if (ev.m_iEffectType == DEF_MAGICTYPE_INVISIBILITY)
							SetInvisibilityFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

								
						if (ev.m_iEffectType == DEF_MAGICTYPE_BERSERK)
							SetBerserkFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

						// Illusion
						if (ev.m_iEffectType == DEF_MAGICTYPE_CONFUSE)
							SetIllusionFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

						// Protection Magic
						if (ev.m_iEffectType == DEF_MAGICTYPE_PROTECT) {
							switch(ev.m_iV1){
								case 1:
									SetProtectionFromArrowFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
									break;
								case 2:
								case 5:
									SetMagicProtectionFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
									break;
								case 3:
								case 4:
									SetDefenseShieldFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
									break;
							}
						}

								
						if (ev.m_iEffectType == DEF_MAGICTYPE_POLYMORPH) {
							m_pClientList[ev.m_iTargetH]->m_sType = m_pClientList[ev.m_iTargetH]->m_sOriginalType;
							SendEventToNearClient_TypeA(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
						}

								
						if (ev.m_iEffectType == DEF_MAGICTYPE_ICE)
							SetIceFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
						break;*/

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[ ev.m_iTargetH ] == nullptr) break;

					m_pNpcList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.m_iEffectType ] = 0;


					if (ev.m_iEffectType == DEF_MAGICTYPE_INVISIBILITY)
						SetInvisibilityFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);


					if (ev.m_iEffectType == DEF_MAGICTYPE_BERSERK)
						SetBerserkFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);


					if (ev.m_iEffectType == DEF_MAGICTYPE_POLYMORPH) {
						m_pNpcList[ev.m_iTargetH]->m_sType = m_pNpcList[ev.m_iTargetH]->m_sOriginalType;
						SendEventToNearClient_TypeA(ev.m_iTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}


					if (ev.m_iEffectType == DEF_MAGICTYPE_ICE)
						SetIceFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);

					// Illusion
					if (ev.m_iEffectType == DEF_MAGICTYPE_CONFUSE)
						SetIllusionFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);

					// Protection Magic
					if (ev.m_iEffectType == DEF_MAGICTYPE_PROTECT) {
						switch (ev.m_iV1) {
							case 1:
								SetProtectionFromArrowFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
								break;
						}
					}
					//	if (ev.m_iEffectType == /*notcoded*/)

					break;
			}
			break;
	}
}