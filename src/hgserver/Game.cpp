#include "Game.h"
#include "NpcArchetype.h"
#include <cstring>
#include <algorithm>
class CDebugWindow *DbgWnd;
extern char G_cTxt[512];
extern char G_cData50000[50000];
// extern void PutDebugMsg(char * cStr);	// 2002-09-09 #2
extern FILE * pLogFile;
extern HWND G_hWnd;
extern bool G_bIsThread;
extern void ThreadProc(void *ch);

CGame::CGame(HWND hWnd) : m_pClientList(*this), dynamicObjects_(*this, m_pMapList, m_pClientList) {
	int i;
	int x;
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
		m_pNpcArchetypes[i] = nullptr;
	m_pNpcList.clear();
	for (i = 0; i < DEF_MSGQUENESIZE; i++)
		m_pMsgQuene[i] = nullptr;
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_pMagicConfigList[i] = nullptr;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_pSkillConfigList[i] = nullptr;
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		m_pQuestConfigList[i] = nullptr;
	dynamicObjects_.clear();
	delayEvents_.clear();
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
	m_iTotalMaps = 0;
	m_MaxUserSysTime.wHour = 0;
	m_MaxUserSysTime.wMinute = 0;
	m_bIsServerShutdowned = false;
	m_cShutDownCode = 0;
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
			auto ptr = m_pClientList.add(i, std::move(socket));
			CClient &client = *ptr;
			client.m_dwSPTime = client.m_dwMPTime =
					  client.m_dwHPTime = client.m_dwAutoSaveTime =
					  client.m_dwTime = client.m_dwHungerTime = client.m_dwExpStockTime =
					  client.m_dwRecentAttackTime = client.m_dwAutoExpTime = client.m_dwSpeedHackCheckTime = timeGetTime();
			std::memset(client.m_cIPaddress, 0, sizeof (client.m_cIPaddress));
			client.m_pXSock->iGetPeerAddress(client.m_cIPaddress);
			wsprintf(G_cTxt, "<%d> Client Connected: (%s)", i, client.m_cIPaddress);
			PutLogList(G_cTxt);
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
	int iRet;
	uint32_t dwTime = timeGetTime();
	iTmp = WM_ONCLIENTSOCKETEVENT;
	int iClientH = message - iTmp;
	auto &clientPtr = m_pClientList[iClientH];
	if (clientPtr == nullptr) return;
	auto &client = *clientPtr;
	iRet = client.m_pXSock->iOnSocketEvent(wParam, lParam);
	switch (iRet) {
		case DEF_XSOCKEVENT_READCOMPLETE:
			client.OnClientRead();
			client.m_dwTime = timeGetTime();
			break;
		case DEF_XSOCKEVENT_BLOCK:
			PutLogList("Socket BLOCKED!");
			break;
		case DEF_XSOCKEVENT_CONFIRMCODENOTMATCH:
			wsprintf(G_cTxt, "<%d> Confirmcode notmatch!", iClientH);
			PutLogList(G_cTxt);
			client.DeleteClient(false, true);
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
			client.DeleteClient(true, true);
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

	if (m_pMainLogSock != nullptr) delete m_pMainLogSock;
	for (i = 0; i < DEF_MAXSUBLOGSOCK; i++)
		if (m_pSubLogSock[i] != nullptr) delete m_pSubLogSock[i];
	m_pClientList.clear();
	m_pNpcList.clear();
	m_pMapList.clear();
	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != nullptr) delete m_pItemConfigList[i];
	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		if (m_pNpcArchetypes[i] != nullptr) delete m_pNpcArchetypes[i];
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		if (m_pMagicConfigList[i] != nullptr) delete m_pMagicConfigList[i];
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		if (m_pSkillConfigList[i] != nullptr) delete m_pSkillConfigList[i];
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != nullptr) delete m_pQuestConfigList[i];
	dynamicObjects_.clear();
	delayEvents_.clear();
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
		m_pNpcArchetypes[i] = nullptr;
	m_pNpcList.clear();
	for (i = 0; i < DEF_MSGQUENESIZE; i++)
		m_pMsgQuene[i] = nullptr;
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_pMagicConfigList[i] = nullptr;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_pSkillConfigList[i] = nullptr;
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		m_pQuestConfigList[i] = nullptr;
	dynamicObjects_.clear();
	delayEvents_.clear();
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
	m_iTotalMaps = 0;
	m_MaxUserSysTime.wHour = 0;
	m_MaxUserSysTime.wMinute = 0;
	m_bIsServerShutdowned = false;
	m_cShutDownCode = 0;
	middlelandMap_.reset();
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

void CGame::DisplayInfo(HDC hdc) {
	char cTxt[350];
	int i;
	int iLine;
	wsprintf(cTxt, "Server-Name(%s) Max.Level(%d) Players(%d/%d) Crusade(%d:%d) SLSock(%d:%d) RBT(%d)", m_cServerName, m_iPlayerMaxLevel, (int) m_pClientList.getTotalClients(), (int) m_pClientList.getMaxClients(), m_dwCrusadeGUID, (int) m_bIsCrusadeMode, m_iSubLogSockActiveCount, m_iSubLogSockFailCount, m_iAutoRebootingCount);
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


void CGame::CheckClientResponseTime() {

	uint32_t dwTime;
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
	for (auto &clientIter : m_pClientList) {
		clientIter.update(dwTime);
	}
}

void CGame::OnMainLogSocketEvent(UINT /*message*/, WPARAM wParam, LPARAM lParam) {
	int iRet;
	if (m_pMainLogSock == nullptr) return;
	iRet = m_pMainLogSock->iOnSocketEvent(wParam, lParam);
	switch (iRet) {
		case DEF_XSOCKEVENT_CONNECTIONESTABLISH:

			PutLogList("(!!!) Main-log-socket connected!");
			bSendMsgToLS(MSGID_REQUEST_REGISTERGAMESERVER);
			break;
		case DEF_XSOCKEVENT_READCOMPLETE:

			OnMainLogRead();
			break;
		case DEF_XSOCKEVENT_BLOCK:
			PutLogList("(!!!) Main-log-socket BLOCKED!");
			break;
		case DEF_XSOCKEVENT_CONFIRMCODENOTMATCH:
		case DEF_XSOCKEVENT_MSGSIZETOOLARGE:

		case DEF_XSOCKEVENT_SOCKETERROR:

		case DEF_XSOCKEVENT_SOCKETCLOSED:

			delete m_pMainLogSock;
			m_pMainLogSock = nullptr;
			PutLogList("(!!!) Main-log-socket connection lost!");
			m_bIsLogSockAvailable = false;

			if (m_bOnExitProcess == false) {
				m_cShutDownCode = 3;
				m_bOnExitProcess = true;
				m_dwExitProcessTime = timeGetTime();

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


void CGame::ResponsePlayerDataHandler(char * pData, uint32_t dwSize) {
	uint16_t * wp;
	char * cp, cCharName[11], cTxt[120];
	std::memset(cCharName, 0, sizeof (cCharName));
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	cp += 10;
	for (auto &clientIter : m_pClientList) {
		if (memcmp(clientIter.m_cCharName, cCharName, 10) == 0) {
			wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
			switch (*wp) {
				case DEF_LOGRESMSGTYPE_CONFIRM:
					clientIter.InitPlayerData(pData, dwSize);
					break;
				case DEF_LOGRESMSGTYPE_REJECT:
					wsprintf(G_cTxt, "(HACK?) Not existing character(%s) data request! Rejected!", clientIter.m_cCharName);
					PutLogList(G_cTxt);
					//PutLogFileList(G_cTxt); // v1.4
					clientIter.DeleteClient(false, false);
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


bool CGame::_bDecodeItemConfigFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iItemConfigListIndex;
	int iTemp;
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

bool _bGetIsStringIsNumber(char * pStr) {
	int i;
	for (i = 0; i < (int) strlen(pStr); i++)
		if ((pStr[i] != '-') && ((pStr[i] < (char) '0') || (pStr[i] > (char) '9'))) return false;
	return true;
}

bool CGame::bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType) {
	register int i, iIndex, iFollowIndex;
	char cTmpName[11], cFollowSide;
	std::memset(cTmpName, 0, sizeof (cTmpName));
	MapPtr map;
	iFollowIndex = -1;
	for (i = 1; i < DEF_MAXNPCS; i++)
		if ((m_pNpcList[i] != nullptr) && (memcmp(m_pNpcList[i]->m_cName, pName, 5) == 0)) {
			iIndex = i;
			map = m_pNpcList[i]->map_;
			goto NEXT_STEP_SNFM1;
		}
NEXT_STEP_SNFM1:
	;
	switch (cFollowOwnerType) {
		case DEF_OWNERTYPE_NPC:
			for (i = 1; i < DEF_MAXNPCS; i++)
				if ((m_pNpcList[i] != nullptr) && (memcmp(m_pNpcList[i]->m_cName, pFollowName, 5) == 0)) {
					if (m_pNpcList[i]->map_ != map) return false;
					iFollowIndex = i;
					cFollowSide = m_pNpcList[i]->m_cSide;
					goto NEXT_STEP_SNFM2;
				}
			break;
		case DEF_OWNERTYPE_PLAYER:
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pFollowName, 10) == 0)) {
					if (m_pClientList[i]->map_ != map) return false;
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

void CGame::NpcProcess() {
	register int i, iMaxHP;
	uint32_t dwTime;
	uint32_t dwActionTime;
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
					m_pNpcList[i]->NpcBehavior_Dead();
					break;
				case DEF_BEHAVIOR_STOP:
					m_pNpcList[i]->NpcBehavior_Stop();
					break;
				case DEF_BEHAVIOR_MOVE:
					m_pNpcList[i]->NpcBehavior_Move();
					break;
				case DEF_BEHAVIOR_ATTACK:
					m_pNpcList[i]->NpcBehavior_Attack();
					break;
				case DEF_BEHAVIOR_FLEE:
					m_pNpcList[i]->NpcBehavior_Flee();
					break;
			}
			if ((m_pNpcList[i] != nullptr) && (m_pNpcList[i]->m_iHP != 0) && (m_pNpcList[i]->m_bIsSummoned == true)) {
				switch (m_pNpcList[i]->m_sType) {
					case 29:
						if ((dwTime - m_pNpcList[i]->m_dwSummonedTime) > 1000 * 90)
							m_pNpcList[i]->NpcKilledHandler(0, 0, 0);
						break;
					default:
						if ((dwTime - m_pNpcList[i]->m_dwSummonedTime) > DEF_SUMMONTIME)
							m_pNpcList[i]->NpcKilledHandler(0, 0, 0);
						break;
				}
			}
		}
	}
}
// 05/29/2004 - Hypnotoad - GM chat tweak


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
//  int CGame::iClientMotion_Attack_Handler(CClient &client, short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse, bool bIsDash)
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

void CGame::MsgProcess() {
	char * pData, cFrom, cKey;
	uint32_t dwMsgSize, * dwpMsgID;
	uint16_t * wpMsgType;
	int i;
	if ((m_bF5pressed == true) && (m_bF1pressed == true)) {
		PutLogList("(XXX) RELOADING CONFIGS MANUALY...");
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				m_pClientList[i]->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA);
			}
		bInit();
	}
	if ((m_bF1pressed == true) && (m_bF4pressed == true) && (m_bOnExitProcess == false)) {
		m_cShutDownCode = 2;
		m_bOnExitProcess = true;
		m_dwExitProcessTime = timeGetTime();
		PutLogList("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Local command)!!!");
		bSendMsgToLS(MSGID_GAMESERVERSHUTDOWNED);
		auto ml = middlelandMap_.lock();
		if (ml) {
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
	int iClientH;
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
					client.DeleteClient(true, true);
					break;
				}*/
				auto &clientPtr = m_pClientList[iClientH];
				if (!clientPtr) {
					continue;
				}
				auto &client = *clientPtr;
				dwpMsgID = (uint32_t *) (pData + DEF_INDEX4_MSGID);
				client.processClientMsg(*dwpMsgID, pData, dwMsgSize, cKey);
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
					m_pClientList[i]->DeleteClient(true, true);
					return;
			}
			return;
		}
	wsprintf(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
	PutLogList(cTxt);
}

void CGame::ResponseDisbandGuildHandler(char * pData, uint32_t /*dwMsgSize*/) {
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
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)) {
			wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
			switch (*wp) {
				case DEF_LOGRESMSGTYPE_CONFIRM:

					wResult = DEF_MSGTYPE_CONFIRM;
					wsprintf(cTxt, "(!) Disband guild(%s) success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
					PutLogList(cTxt);

					m_pClientList[i]->SendGuildMsg(DEF_NOTIFY_GUILDDISBANDED, 0, 0, nullptr);

					std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
					memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
					m_pClientList[i]->m_iGuildRank = -1;
					m_pClientList[i]->m_iGuildGUID = -1;
					break;
				case DEF_LOGRESMSGTYPE_REJECT:

					wResult = DEF_MSGTYPE_REJECT;
					wsprintf(cTxt, "(!) Disband guild(%s) Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
					PutLogList(cTxt);
					break;
			}
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_DISBANDGUILD;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = wResult;

			iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					m_pClientList[i]->DeleteClient(true, true);
					return;
			}
			return;
		}
	wsprintf(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
	PutLogList(cTxt);
}
// 05/29/2004 - Hypnotoad - Purchase Dicount updated to take charisma into consideration

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
							std::memset(m_pNpcArchetypes[iNpcConfigListIndex]->m_cNpcName, 0, sizeof (m_pNpcArchetypes[iNpcConfigListIndex]->m_cNpcName));
							memcpy(m_pNpcArchetypes[iNpcConfigListIndex]->m_cNpcName, token, strlen(token));
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_sType = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iHitDice = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iDefenseRatio = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iHitRatio = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iMinBravery = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iExpDiceMin = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iExpDiceMax = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iGoldDiceMin = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iGoldDiceMax = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cAttackDiceThrow = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cAttackDiceRange = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cSize = atoi(token);
							cReadModeB = 14;
							break;
						case 14:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cSide = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cActionLimit = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_dwActionTime = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cResistMagic = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cMagicLevel = atoi(token);
							cReadModeB = 19;
							break;
						case 19:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cDayOfWeekLimit = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cChatMsgPresence = atoi(token);
							if (m_pNpcArchetypes[iNpcConfigListIndex]->m_cChatMsgPresence == 1) {
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cTargetSearchRange = atoi(token);
							cReadModeB = 22;
							break;
						case 22:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pNpcArchetypes[iNpcConfigListIndex]->m_dwRegenTime = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_cAttribute = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iAbsDamage = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iMaxMana = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iMagicHitRatio = atoi(token);
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
							m_pNpcArchetypes[iNpcConfigListIndex]->m_iAttackRange = atoi(token);
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
				m_pNpcArchetypes[iNpcConfigListIndex] = new class NpcArchetype();
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
	char cTmpName[21];
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pNpcName);
	NpcArchetype *npcArchPtr = nullptr;
	for (int i = 0; i < DEF_MAXNPCTYPES; i++) {
		if (m_pNpcArchetypes[i] != nullptr) {
			if (memcmp(cTmpName, m_pNpcArchetypes[i]->m_cNpcName, 20) == 0) {
				npcArchPtr = m_pNpcArchetypes[i];
				break;
			}
		}
	}
	if (!npcArchPtr) {
		return false;
	}
	auto &npcArch = *npcArchPtr;
	int iTemp;
	int sTemp;
	double dV1;
	double dV2;
	double dV3;
	std::memset(pNpc->m_cNpcName, 0, sizeof (pNpc->m_cNpcName));
	memcpy(pNpc->m_cNpcName, npcArch.m_cNpcName, 20);
	pNpc->m_sType = npcArch.m_sType;
	if (npcArch.m_iHitDice <= 5) {
		pNpc->m_iHP = abs(iDice(npcArch.m_iHitDice, 4) + npcArch.m_iHitDice);
	} else {
		pNpc->m_iHP = abs((npcArch.m_iHitDice * 4) + npcArch.m_iHitDice + iDice(1, npcArch.m_iHitDice));
	}
	if (pNpc->m_iHP == 0) {
		pNpc->m_iHP = 1;
	}
	pNpc->m_iExpDiceMin = npcArch.m_iExpDiceMin;
	pNpc->m_iExpDiceMax = npcArch.m_iExpDiceMax;
	pNpc->m_iGoldDiceMin = npcArch.m_iGoldDiceMin * 5;
	pNpc->m_iGoldDiceMax = npcArch.m_iGoldDiceMax * 5;
	pNpc->m_iExp = (iDice(1, (npcArch.m_iExpDiceMax - npcArch.m_iExpDiceMin)) + npcArch.m_iExpDiceMin) * 500;
	pNpc->m_iHitDice = npcArch.m_iHitDice;
	pNpc->m_iDefenseRatio = npcArch.m_iDefenseRatio;
	pNpc->m_iHitRatio = npcArch.m_iHitRatio;
	pNpc->m_iMinBravery = npcArch.m_iMinBravery;
	pNpc->m_cAttackDiceThrow = npcArch.m_cAttackDiceThrow;
	pNpc->m_cAttackDiceRange = npcArch.m_cAttackDiceRange;
	pNpc->m_cSize = npcArch.m_cSize;
	pNpc->m_cSide = npcArch.m_cSide;
	pNpc->m_cActionLimit = npcArch.m_cActionLimit;
	pNpc->m_dwActionTime = npcArch.m_dwActionTime;
	pNpc->m_dwRegenTime = npcArch.m_dwRegenTime;
	pNpc->m_cResistMagic = npcArch.m_cResistMagic;
	pNpc->m_cMagicLevel = npcArch.m_cMagicLevel;
	pNpc->m_iMaxMana = npcArch.m_iMaxMana; // v1.4
	pNpc->m_iMana = npcArch.m_iMaxMana;
	pNpc->m_cChatMsgPresence = npcArch.m_cChatMsgPresence;
	pNpc->m_cDayOfWeekLimit = npcArch.m_cDayOfWeekLimit;
	pNpc->m_cTargetSearchRange = npcArch.m_cTargetSearchRange;
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
	pNpc->m_iAbsDamage = npcArch.m_iAbsDamage;
	pNpc->m_iMagicHitRatio = npcArch.m_iMagicHitRatio;
	pNpc->m_iAttackRange = npcArch.m_iAttackRange;
	pNpc->m_cSpecialAbility = cSA;
	pNpc->m_iBuildCount = npcArch.m_iMinBravery;
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

/*********************************************************************************************************************
 **  int CGame::iDice(int iThrow, int iRange)																		**
 **  description			:: produces a random number between the throw and range										**
 **  last updated		:: November 20, 2004; 10:24 PM; Hypnotoad													**
 **	return value		:: int																						**
 **********************************************************************************************************************/
int iDice(int iThrow, int iRange) {
	if (iRange <= 0) return 0;
	int iRet = 0;
	for (register int i = 1; i <= iThrow; i++) {
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
					m_pNpcList[iVictimH]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH != 0) && (sOwnerH != sAttackerH) &&
							  (memcmp(m_pClientList[sAttackerH]->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
							  (m_pClientList[sAttackerH]->m_iLevel >= m_pClientList[sOwnerH]->m_iLevel)) {
						if (iDice(1, 3) == 2) {
							iExp = (m_pNpcList[iVictimH]->m_iExp / 3);
							if (m_pClientList[sAttackerH]->m_iLevel >= m_iPlayerMaxLevel) iExp = 0;
							if (iExp > 0) {
								m_pClientList[sOwnerH]->m_iExp += iExp;
								if (m_pClientList[sOwnerH]->bCheckLimitedUser() == false) {
									m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
								}
								m_pClientList[sOwnerH]->bCheckLevelUp();
							}
						}
					}
				}
			break;
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



enum class TeleportType {
	TYPE_0, // Forced
	TYPE_1, // Recall
	TYPE_2 // ??
};

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

void CGame::Quit() {
	int i;
	G_bIsThread = false;
	Sleep(300);
	if (m_pMainLogSock != nullptr) delete m_pMainLogSock;
	for (i = 0; i < DEF_MAXSUBLOGSOCK; i++)
		if (m_pSubLogSock[i] != nullptr) delete m_pSubLogSock[i];
	m_pClientList.clear();
	m_pNpcList.clear();
	m_pMapList.clear();
	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != nullptr) delete m_pItemConfigList[i];
	for (i = 0; i < DEF_MAXNPCTYPES; i++)
		if (m_pNpcArchetypes[i] != nullptr) delete m_pNpcArchetypes[i];
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		if (m_pMagicConfigList[i] != nullptr) delete m_pMagicConfigList[i];
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		if (m_pSkillConfigList[i] != nullptr) delete m_pSkillConfigList[i];
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != nullptr) delete m_pQuestConfigList[i];
	dynamicObjects_.clear();
	delayEvents_.clear();
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

void CGame::ApplyPKpenalty(short sAttackerH, short sVictumH) {
	int iV1;
	int iV2;
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
	m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_PKPENALTY, 0, 0, 0, nullptr);
	m_pClientList[sAttackerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	//PutLogFileList(G_cTxt);
	m_stCityStatus[m_pClientList[sAttackerH]->m_cSide].iCrimes++;
	m_pClientList[sAttackerH]->m_iRating -= 10;
	if (m_pClientList[sAttackerH]->m_iRating > 10000) m_pClientList[sAttackerH]->m_iRating = 10000;
	if (m_pClientList[sAttackerH]->m_iRating < -10000) m_pClientList[sAttackerH]->m_iRating = -10000;
	if (strcmp(m_pClientList[sAttackerH]->m_cLocation, "aresden") == 0) {
		if ((strcmp(m_pClientList[sAttackerH]->map_->m_cName, "arebrk11") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "arebrk12") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "arebrk21") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "arebrk22") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "aresden") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "huntzone2") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "areuni") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "arefarm") == 0)) {
			m_pClientList[sAttackerH]->lockedMap_ = m_pMapList.byName("arejail");
			m_pClientList[sAttackerH]->m_iLockedMapTime = 5 * 2;
			m_pClientList[sAttackerH]->RequestTeleportHandler("2   ", "arejail", -1, -1);
			return;
		}
	}
	if (strcmp(m_pClientList[sAttackerH]->m_cLocation, "elvine") == 0) {
		if ((strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvbrk11") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvbrk12") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvbrk21") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvbrk22") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvine") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "huntzone1") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvuni") == 0) ||
				  (strcmp(m_pClientList[sAttackerH]->map_->m_cName, "elvfarm") == 0)) {
			m_pClientList[sAttackerH]->lockedMap_ = m_pMapList.byName("elvjail");
			m_pClientList[sAttackerH]->m_iLockedMapTime = 5 * 2;
			m_pClientList[sAttackerH]->RequestTeleportHandler("2   ", "elvjail", -1, -1);
			return;
		}
	}
}

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
		m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_PKCAPTURED, m_pClientList[sVictumH]->m_iPKCount, m_pClientList[sVictumH]->m_iLevel, 0, m_pClientList[sVictumH]->m_cCharName);
	}
}

void CGame::Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, bool bExp, int iAttr) {
	int iPartyID;
	int iDamage;
	int iSideCondition;
	int iIndex;
	int iRemainLife;
	int iTemp;
	int iExp;
	int iMaxSuperAttack;
	int iRepDamage;
	char cAttackerSide;
	char cDamageMoveDir;
	uint32_t dwTime;
	register double dTmp1, dTmp2, dTmp3;
	short sAtkX;
	short sAtkY;
	short sTgtX;
	short sTgtY;
	short dX;
	short dY;
	short sItemIndex;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;
	if (cAttackerType == DEF_OWNERTYPE_NPC)
		if (m_pNpcList[sAttackerH] == nullptr) return;
	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->map_ != 0) &&
			  (m_pClientList[sAttackerH]->map_->m_bIsHeldenianMap == 1) && (m_bHeldenianInitiated == true)) return;
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
			if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true)
				iDamage += iDamage / 3;
			if (m_pClientList[sAttackerH]->bCheckHeldenianMap() == 1) {
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
			if ((m_pClientList[sTargetH]->map_->m_bIsAttackEnabled == false) && (m_pClientList[sTargetH]->m_iAdminUserLevel == 0)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsNeutral == true) && (m_pClientList[sTargetH]->m_iPKCount == 0)) return;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				if (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) {
					iSideCondition = m_pClientList[sAttackerH]->iGetPlayerRelationship(sTargetH);
					if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {
					} else {
						if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true) {
							if (m_pClientList[sAttackerH]->m_iGuildGUID != m_pClientList[sTargetH]->m_iGuildGUID) {
							} else return;
						} else return;
					}
				}
				if (m_pClientList[sTargetH]->map_->iGetAttribute(m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY, 0x00000005) != 0) return;
			}
			m_pClientList[sTargetH]->ClearSkillUsingStatus();
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
					m_pClientList[sTargetH]->ItemDepleteHandler(iIndex, true, true);
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
				m_pClientList[sTargetH]->ClientKilledHandler(sAttackerH, cAttackerType, iDamage);
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
							m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
						}
					}
					m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
					m_pClientList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
					if (m_pClientList[sTargetH]->m_bSkillUsingStatus[19] != true) {
						m_pClientList[sTargetH]->map_->ClearOwner(0, sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						m_pClientList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
					}
					if (m_pClientList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ], 0, nullptr);
						m_pClientList[sTargetH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
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
					if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) {
						cDamageMoveDir = iDice(1, 8);
						dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
						if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) return;
					}
					m_pNpcList[sTargetH]->map_->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
					m_pNpcList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
					m_pNpcList[sTargetH]->m_sX = dX;
					m_pNpcList[sTargetH]->m_sY = dY;
					m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;
					m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
					dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
					if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) {
						cDamageMoveDir = iDice(1, 8);
						dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
						if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) return;
					}
					m_pNpcList[sTargetH]->map_->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
					m_pNpcList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
					m_pNpcList[sTargetH]->m_sX = dX;
					m_pNpcList[sTargetH]->m_sY = dY;
					m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;
					m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
					if (m_pNpcList[sTargetH]->bCheckEnergySphereDestination(sAttackerH, cAttackerType) == true) {
						m_pNpcList[sTargetH]->DeleteNpc();
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
				m_pNpcList[sTargetH]->NpcKilledHandler(sAttackerH, cAttackerType, iDamage);
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
				m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
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
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
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
								m_pClientList[sAttackerH]->GetExp(iExp, true);
							else m_pClientList[sAttackerH]->GetExp((iExp / 2), true);
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
								m_pClientList[sAttackerH]->GetExp(iExp, true);
							else m_pClientList[sAttackerH]->GetExp((iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
					}
				}
			}
			break;
	}
}

void CGame::processDelayedEvent(const DelayEvent &ev) {
	int iTemp;
	int iSkillNum;
	int iResult;
	switch (ev.delayType_) {
		case DelayEventType::ANCIENT_TABLET:
			if ((m_pClientList[ev.m_iTargetH]->m_iStatus & 0x400000) != 0) {
				iTemp = 1;
			} else if ((m_pClientList[ev.m_iTargetH]->m_iStatus & 0x800000) != 0) {
				iTemp = 3;
			} else if ((m_pClientList[ev.m_iTargetH]->m_iStatus & 0x10000) != 0) {
				iTemp = 4;
			}
			m_pClientList[ev.m_iTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SLATE_STATUS, iTemp, 0, 0, nullptr);
			m_pClientList[ev.m_iTargetH]->SetSlateFlag(iTemp, false);
			break;
		case DelayEventType::CALCMETEORSTRIKEEFFECT:
			ev.map_->CalcMeteorStrikeEffectHandler();
			break;
		case DelayEventType::DOMETEORSTRIKEDAMAGE:
			ev.map_->DoMeteorStrikeDamageHandler();
			break;
		case DelayEventType::METEORSTRIKE:
			ev.map_->MeteorStrikeHandler();
			break;
		case DelayEventType::USEITEM_SKILL:
			switch (ev.m_cTargetType) {
				case DEF_OWNERTYPE_PLAYER:
					iSkillNum = ev.effectType_;
					if (m_pClientList[ev.m_iTargetH] == nullptr) break;
					if (m_pClientList[ev.m_iTargetH]->m_bSkillUsingStatus[iSkillNum] == false) break;
					if (m_pClientList[ev.m_iTargetH]->m_iSkillUsingTimeID[iSkillNum] != ev.v2_) break;
					m_pClientList[ev.m_iTargetH]->m_bSkillUsingStatus[iSkillNum] = false;
					m_pClientList[ev.m_iTargetH]->m_iSkillUsingTimeID[iSkillNum] = 0;
					iResult = iCalculateUseSkillItemEffect(ev.m_iTargetH, ev.m_cTargetType,
							  ev.v1_, iSkillNum, ev.map_, ev.x_, ev.y_);
					m_pClientList[ev.m_iTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, iResult, 0, 0, nullptr);
					break;
			}
			break;
		case DelayEventType::DAMAGEOBJECT:
			break;
		case DelayEventType::MAGICRELEASE:
			// Removes the aura after time
			switch (ev.m_cTargetType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[ ev.m_iTargetH ] == nullptr) break;
					m_pClientList[ev.m_iTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF,
							  ev.effectType_, m_pClientList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.effectType_ ], 0, nullptr);
					m_pClientList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.effectType_ ] = 0;
					// Inbitition casting
					if (ev.effectType_ == DEF_MAGICTYPE_INHIBITION)
						m_pClientList[ev.m_iTargetH]->m_bInhibition = false;

					if (ev.effectType_ == DEF_MAGICTYPE_INVISIBILITY)
						SetInvisibilityFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

					if (ev.effectType_ == DEF_MAGICTYPE_BERSERK)
						SetBerserkFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
					// Confusion
					if (ev.effectType_ == DEF_MAGICTYPE_CONFUSE)
						switch (ev.v1_) {
							case 3: SetIllusionFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
							case 4: SetIllusionMovementFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
						}
					// Protection Magic
					if (ev.effectType_ == DEF_MAGICTYPE_PROTECT) {
						switch (ev.v1_) {
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

					if (ev.effectType_ == DEF_MAGICTYPE_POLYMORPH) {
						m_pClientList[ev.m_iTargetH]->m_sType = m_pClientList[ev.m_iTargetH]->m_sOriginalType;
						m_pClientList[ev.m_iTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}

					if (ev.effectType_ == DEF_MAGICTYPE_ICE)
						SetIceFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
					break;
					/*case DelayEventType::MAGICRELEASE:
					// Removes the aura after time
					switch (ev.m_cTargetType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[ ev.m_iTargetH ] == nullptr) break;
						m_pClientList[ev.m_iTargetH]->SendNotifyMsg(0,DEF_NOTIFY_MAGICEFFECTOFF,
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
							m_pClientList[ev.m_iTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
						}

						if (ev.m_iEffectType == DEF_MAGICTYPE_ICE)
							SetIceFlag(ev.m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
						break;*/
				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[ ev.m_iTargetH ] == nullptr) break;
					m_pNpcList[ ev.m_iTargetH ]->m_cMagicEffectStatus[ ev.effectType_ ] = 0;
					if (ev.effectType_ == DEF_MAGICTYPE_INVISIBILITY)
						SetInvisibilityFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
					if (ev.effectType_ == DEF_MAGICTYPE_BERSERK)
						SetBerserkFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
					if (ev.effectType_ == DEF_MAGICTYPE_POLYMORPH) {
						m_pNpcList[ev.m_iTargetH]->m_sType = m_pNpcList[ev.m_iTargetH]->m_sOriginalType;
						m_pNpcList[ev.m_iTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}
					if (ev.effectType_ == DEF_MAGICTYPE_ICE)
						SetIceFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
					// Illusion
					if (ev.effectType_ == DEF_MAGICTYPE_CONFUSE)
						SetIllusionFlag(ev.m_iTargetH, DEF_OWNERTYPE_NPC, false);
					// Protection Magic
					if (ev.effectType_ == DEF_MAGICTYPE_PROTECT) {
						switch (ev.v1_) {
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

void CGame::MobGenerator() {
	register int i, x, j, iNamingValue, iResult, iTotalMob;
	char cNpcName[21], cName_Master[11], cName_Slave[11], cWaypoint[11];
	char cSA;
	int pX;
	int pY;
	int iMapLevel;
	int iProbSA;
	int iKindSA;
	int iResultNum;
	int iNpcID;
	bool bMaster;
	bool bFirmBerserk;
	bool bIsSpecialEvent;
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
			auto ml = middlelandMap_.lock();
			if ((ml) && (ml == m_pMapList[i]) && (m_bIsCrusadeMode == true)) break;
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
				if ((bMaster = m_pMapList[i]->bCreateNewNpc(cNpcName, cName_Master, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, bFirmBerserk, true)) == false) {
					m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
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
								m_pClientList[x]->SendNotifyMsg(0, DEF_NOTIFY_SPAWNEVENT, pX, pY, iNpcID, nullptr, 0, 0);
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
					if (m_pMapList[i]->bCreateNewNpc(cNpcName, cName_Slave, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, bFirmBerserk) == false) {
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
								if (m_pMapList[i]->bCreateNewNpc(cNpcName, cName_Master, (rand() % 3), cSA, DEF_MOVETYPE_RANDOMAREA, &pX, &pY, cWaypoint, &m_pMapList[i]->m_stSpotMobGenerator[j].rcRect, j, -1, false, false, bFirmBerserk) == false) {
									m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
								} else {
									m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs++;
								}
								break;
							case 2:
								if (m_pMapList[i]->bCreateNewNpc(cNpcName, cName_Master, (rand() % 3), cSA, DEF_MOVETYPE_RANDOMWAYPOINT, nullptr, nullptr, m_pMapList[i]->m_stSpotMobGenerator[j].cWaypoint, nullptr, j, -1, false, false, bFirmBerserk) == false) {
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

bool CGame::bSendMsgToLS(uint32_t dwMsg, const char *pData) {
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	int i;
	int iSize;
	char cAccountName[11], cAddress[16], cTxt[120], * cp;
	char cGuildLoc[11];
	int iSendSize;
	std::memset(G_cData50000, 0, sizeof (G_cData50000));
	std::memset(cAccountName, 0, sizeof (cAccountName));
	std::memset(cAddress, 0, sizeof (cAddress));
	std::memset(cGuildLoc, 0, sizeof (cGuildLoc));
	switch (dwMsg) {
		case MSGID_GAMEITEMLOG:
			if (_bCheckSubLogSocketIndex() == false) return false;
			//		if (this->markedForDeletion_) return false;
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
		case MSGID_GAMESERVERSHUTDOWNED:
			if (m_pMainLogSock == nullptr) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMESERVERSHUTDOWNED;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			iRet = m_pMainLogSock->iSendMsg(G_cData50000, 6);
			return true;
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

bool CGame::_bCrusadeLog(int iAction, int /*iData*/, const char * cName) {
	char cTxt[200];
	std::memset(cTxt, 0, sizeof (cTxt));
	switch (iAction) {
		case DEF_CRUSADELOG_ENDCRUSADE:
			if (cName == nullptr) return false;
			wsprintf(cTxt, "\tEnd Crusade\t%s", cName);
			break;
		case DEF_CRUSADELOG_STARTCRUSADE:
			wsprintf(cTxt, "\tStart Crusade");
			break;
		default:
			return false;
	}
	bSendMsgToLS(MSGID_GAMECRUSADELOG, cTxt);
	return true;
}
