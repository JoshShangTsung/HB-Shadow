#pragma once
#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <direct.h>
#include <vector>
#include "winmain.h"
#include "StrTok.h"
#include "Xsocket.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "ActionID.h"
#include "UserMessages.h"
#include "NetMessages.h"
#include "MessageIndex.h"
#include "Misc.h"
#include "Msg.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObject.h"
#include "DelayEvent.h"
#include "Version.h"
#include "Fish.h"
#include "DynamicObject.h"
#include "DynamicObjectID.h"
#include "Portion.h"
#include "Mineral.h"
#include "Quest.h"
#include "BuildItem.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "TempNpcItem.h"
#define DEF_MAXADMINS				50
#define DEF_MAXAGRICULTURE			200
#define DEF_MAXNPCTYPES				200
#define DEF_MAXBUILDITEMS			300
#define DEF_SERVERSOCKETBLOCKLIMIT	300
#define DEF_MAXBANNED				500
#define DEF_MAXBALLITEMS			50
#define DEF_MAXNPCITEMS				1000
#define DEF_MAXITEMTYPES			5000
#define DEF_CLIENTTIMEOUT			10000
#define DEF_SPUPTIME				10000
#define DEF_POISONTIME				12000
#define DEF_HPUPTIME				15000
#define DEF_MPUPTIME				20000
#define DEF_HUNGERTIME				60000
#define DEF_NOTICETIME				80000
#define DEF_SUMMONTIME				300000
#define DEF_AUTOSAVETIME			600000
#define DEF_AFKTIME                    150000 // Five Minutes
#define MAX_HELDENIANTOWER			200
#define DEF_EXPSTOCKTIME		1000*10
#define DEF_MSGQUENESIZE		100000
#define DEF_AUTOEXPTIME			1000*60*6
#define DEF_TOTALLEVELUPPOINT	3
#define DEF_MAXDYNAMICOBJECTS	60000
#define DEF_GUILDSTARTRANK		12
#define DEF_SSN_LIMIT_MULTIPLY_VALUE	2
#define DEF_MAXNOTIFYMSGS		300
#define DEF_MAXSKILLPOINTS		700
#define DEF_NIGHTTIME			40
#define DEF_CHARPOINTLIMIT		1000
#define DEF_RAGPROTECTIONTIME	7000
#define DEF_MAXREWARDGOLD		99999999
#define DEF_ATTACKAI_NORMAL				1
#define DEF_ATTACKAI_EXCHANGEATTACK		2
#define DEF_ATTACKAI_TWOBYONEATTACK		3
#define DEF_MAXFISHS					200
#define DEF_MAXMINERALS					200
#define	DEF_MAXCROPS					200
#define DEF_MAXENGAGINGFISH				30
#define DEF_MAXPORTIONTYPES				500
#define DEF_SPECIALEVENTTIME			300000
#define DEF_MAXQUESTTYPE				200
#define DEF_DEF_MAXHELDENIANDOOR			10
#define DEF_MAXSUBLOGSOCK				10
#define DEF_ITEMLOG_GIVE				1
#define DEF_ITEMLOG_DROP				2
#define DEF_ITEMLOG_GET					3
#define DEF_ITEMLOG_DEPLETE				4
#define DEF_ITEMLOG_NEWGENDROP			5
#define DEF_ITEMLOG_DUPITEMID			6
#define DEF_ITEMLOG_BUY					7
#define DEF_ITEMLOG_SELL				8
#define DEF_ITEMLOG_RETRIEVE			9
#define DEF_ITEMLOG_DEPOSIT				10
#define DEF_ITEMLOG_EXCHANGE			11
#define DEF_ITEMLOG_MAKE				13
#define DEF_ITEMLOG_SUMMONMONSTER		14
#define DEF_ITEMLOG_POISONED			15
#define DEF_ITEMLOG_REPAIR				17
#define DEF_ITEMLOG_SKILLLEARN			12
#define DEF_ITEMLOG_MAGICLEARN			16
#define DEF_ITEMLOG_USE					32
#define DEF_MAXDUPITEMID				100
#define DEF_MAXGUILDS					1000
#define DEF_MAXCONSTRUCTNUM				10
#define DEF_MAXSCHEDULE					10
#define DEF_MAXHELDENIAN				10
#define DEF_CRITICALSMINLVL            20
#define DEF_GOLDMINLVL                5000
#define DEF_CRITICALSMAXLVL            180
#define DEF_MAJESTICSMAXLVL            2
#define DEF_CRITICALSGM                2000
#define DEF_MAXCRITICALS            5000

#define DEF_MAXFIGHTZONE 10
#define DEF_LEVELLIMIT		20
#define DEF_MINIMUMHITRATIO 15
#define DEF_MAXIMUMHITRATIO	99
#define DEF_PLAYERMAXLEVEL	180
#define DEF_GMGMANACONSUMEUNIT	15
#define DEF_MAXCONSTRUCTIONPOINT 30000
#define DEF_MAXSUMMONPOINTS		 30000
#define DEF_MAXWARCONTRIBUTION	 200000
#define MSGID_LEVELUPSETTINGS				0x11A01000
#define MSGID_STATECHANGEPOINT				0x11A01001
#define DEF_STR 0x01 //Correct
#define DEF_DEX 0x02 //Correct
#define DEF_INT 0x03 //Correct
#define DEF_VIT 0x04 //Correct
#define DEF_MAG 0x05 //Correct
#define DEF_CHR 0x06 //Correct
#define DEF_MAXGIZONPOINT		999
//#define DEF_TEST 0xFFFF0000
//#define DEF_TESTSERVER
#define NO_MSGSPEEDCHECK
#define DEF_PLAYER_MAX_EXPERIENCE 203484288

constexpr char _tmp_cTmpDirX[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};
constexpr char _tmp_cTmpDirY[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
int iDice(int iThrow, int iRange);
bool _bGetIsStringIsNumber(char * pStr);

class CGame {
public:
	CGame(HWND hWnd);
	~CGame();
	int HeroItemChecker(int iItemID, short sEnemyKill, char cContribution, char cSide);
	void SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp);
	void ReadMajesticSettings();
	void HeldenianWinner();
	void AutomaticHeldenianEnd();
	void SetHeldenianMode();
	void AutomatedHeldenianTimer();
	void LocalStartHeldenianMode(short sV1, short sV2, uint32_t dwHeldenianGUID);
	void GlobalStartHeldenianMode();
	void HeldenianWarEnder();
	void HeldenianWarStarter();
	bool UpdateHeldenianStatus();
	void _CreateHeldenianGUID(uint32_t dwHeldenianGUID, int iWinnerSide);
	void NotifyStartHeldenianFightMode();
	void GlobalEndHeldenianMode();
	void LocalEndHeldenianMode();
	bool bNotifyHeldenianWinner();
	bool bReadHeldenianGUIDFile(const char * cFn);
	void GlobalUpdateConfigs(char cConfigType);
	void LocalUpdateConfigs(char cConfigType);
	void SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void CalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, char cAttackerType, int iArmorType);
	bool bReadScheduleConfigFile(const char *pFn);
	bool bReadBannedListConfigFile(const char *pFn);
	bool bReadAdminListConfigFile(const char *pFn);
	bool bSendMsgToLS(uint32_t dwMsg, const char *data = nullptr);
	void ManualEndCrusadeMode(int iWinnerSide); // 2.17 (x) 2.14 ( )
	void CrusadeWarStarter();
	bool bReadCrusadeGUIDFile(const char * cFn);
	void _CreateCrusadeGUID(uint32_t dwCrusadeGUID, int iWinnerSide);
	void GlobalStartCrusadeMode();
	void GlobalStartApocalypseMode();
	void GlobalEndApocalypseMode();
	void LocalStartApocalypseMode(uint32_t dwApocalypseGUID);
	void LocalEndApocalypseMode();
	void _CreateApocalypseGUID(uint32_t dwApocalypseGUID);
	bool bReadApocalypseGUIDFile(const char * cFn);
	void SendThunder(int iClient, short sX, short sY, short sV3, short sV4);
	void GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char * pMapName);
	void SyncMiddlelandMapInfo();
	void RemoveCrusadeStructures();
	void _LinkStrikePointMapIndex();
	void MeteorStrikeMsgHandler(char cAttackerSide);
	void CollectedManaHandler(uint16_t wAresdenMana, uint16_t wElvineMana);
	void SendCollectedMana();
	void CreateCrusadeStructures();
	void GrandMagicResultHandler(char *cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char * cData);
	void GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char * pMapName);
	void GSM_ConstructionPoint(int iGuildGUID, int iPoint);
	bool bReadCrusadeStructureConfigFile(const char * cFn);
	void SaveOccupyFlagData();
	void LocalEndCrusadeMode(int iWinnerSide);
	void LocalStartCrusadeMode(uint32_t dwGuildGUID);
	bool bReadSettingsConfigFile(const char * cFn);
	bool bReadAdminSetConfigFile(const char * cFn);
	void SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SendMsg(short sOwnerH, char cOwnerType, bool bStatus, long lPass);
	bool bChangeState(char cStateChange, char* cStr, char *cVit, char *cDex, char *cInt, char *cMag, char *cChar);
	void SetStatusFlag(short sOwnerH, char cOwnerType, bool bStatus, int iPass);
	void SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus);
	bool bCopyItemContents(class CItem * pOriginal, class CItem * pCopy);
	int iGetMapLocationSide(char * pMapName);
	void ChatMsgHandlerGSM(int iMsgType, int iV1, char * pName, char * pData, uint32_t dwMsgSize);
	void GSM_RequestFindCharacter(uint16_t wReqServerID, uint16_t wReqClientH, char *pName, char * pFinder); // New 16/05/2001 Changed
	void ServerStockMsgHandler(char * pData);
	void AgingMapSectorInfo();
	void UpdateMapSectorInfo();
	bool bGetItemNameWhenDeleteNpc(int & iItemID, short sNpcType);
	void Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void RequestCheckAccountPasswordHandler(char * pData, uint32_t dwMsgSize);
	void SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void _bDecodeNoticementFileContents(char * pData, uint32_t dwMsgSize);
	void _AdjustRareItemValue(class CItem &pItem);
	bool _bCheckDupItemID(class CItem * pItem);
	bool _bDecodeDupItemIDFileContents(char * pData, uint32_t dwMsgSize);
	int iGetPlayerABSStatus(int iWhatH, int iRecvH);
	char _cGetSpecialAbility(int iKindSA);
	bool _bDecodeBuildItemConfigFileContents(char * pData, uint32_t dwMsgSize);
	bool _bCheckSubLogSocketIndex();
	void OnSubLogRead(int iIndex);
	void OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool _bDecodeQuestConfigFileContents(char * pData, uint32_t dwMsgSize);
	void _BWM_Command_Shutup(char * pData);
	bool _bDecodeOccupyFlagSaveFileContents(char * pData, uint32_t dwMsgSize);
	int _iComposeFlagStatusContents(char * pData);
	int iGetComboAttackBonus(int iSkill, int iComboCount);
	bool bDeleteMineral(int iIndex);
	void MineralGenerator();
	bool _bDecodePortionConfigFileContents(char * pData, uint32_t dwMsgSize);
	bool bOnClose();
	void ForceDisconnectAccount(char * pAccountName, uint16_t wCount);
	void SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SpecialEventHandler();
	int _iForcePlayerDisconect(int iNum);
	int iGetMapIndex(char * pMapName);
	int iGetNpcRelationship(int iWhatH, int iRecvH);
	int iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus);
	void WhetherProcessor();
	void FishGenerator();
	void FishProcessor();
	bool bDeleteFish(int iHandle, int iDelMode);
	int iCreateFish(MapPtr map, short sX, short sY, short sDifficulty, class CItem * pItem, int iDifficulty, uint32_t dwLastTime);
	int iGetExpLevel(int iExp);
	void ResponseSavePlayerDataReplyHandler(char * pData, uint32_t dwMsgSize);
	void NoticeHandler();
	bool bReadNotifyMsgListFile(const char * cFn);
	void CheckDayOrNightMode();
	bool bCheckBadWord(char * pString);
	bool bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType);
	void bSetNpcAttackMode(char * cName, int iTargetH, char cTargetType, bool bIsPermAttack);
	int iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, MapPtr map, int dX, int dY);
	int _iGetTotalClients();
	void SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus);
	int iGetFollowerNumber(short sOwnerH, char cOwnerType);
	int _iCalcSkillSSNpoint(int iLevel);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	UnitPtr getUnit(short sHandle, char cType);
	bool bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	void Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, bool bExp, int iAttr = 0);
	void Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void MobGenerator();
	void PK_KillRewardHandler(short sAttackerH, short sVictumH);
	void ApplyPKpenalty(short sAttackerH, short sVictumH);
	int iGetLevelExp(int iLevel);
	void Quit();
	int _iGetSkillNumber(char * pSkillName);
	int _iGetMagicNumber(char * pMagicName, int * pReqInt, int * pCost);
	bool _bDecodeSkillConfigFileContents(char * pData, uint32_t dwMsgSize);
	bool _bDecodeMagicConfigFileContents(char * pData, uint32_t dwMsgSize);
	void ReleaseFollowMode(short sOwnerH, char cOwnerType);
	bool bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType);
	void GuildNotifyHandler(char * pData, uint32_t dwMsgSize);
	void CalculateGuildEffect(int iVictimH, char cVictimType, short sAttackerH);
	void OnStartGameSignal();
	bool _bInitNpcAttr(class CNpc * pNpc, const char * pNpcName, short sClass, char cSA);
	bool _bDecodeNpcConfigFileContents(char * pData, uint32_t dwMsgSize);
	void ResponseDisbandGuildHandler(char * pData, uint32_t dwMsgSize);
	void ResponseCreateNewGuildHandler(char * pData, uint32_t dwMsgSize);
	bool __fastcall bGetMsgQuene(char * pFrom, char * pData, uint32_t * pMsgSize, int * pIndex, char * pKey);
	void MsgProcess();
	bool __fastcall bPutMsgQuene(char cFrom, char * pData, uint32_t dwMsgSize, int iIndex, char cKey);
	int iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, bool bNearAttack = false, bool bIsDash = false, bool bArrowUse = false);
	void RemoveFromTarget(short sTargetH, char cTargetType, int iCode = 0);
	void NpcProcess();
	bool _bInitItemAttr(class CItem &pItem, const char * pItemName);
	bool bReadProgramConfigFile(const char * cFn);
	void GameProcess();
	void ResponsePlayerDataHandler(char * pData, uint32_t dwSize);
	void OnMainLogRead();
	void OnMainLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void CheckClientResponseTime();
	void OnTimer(char cType);
	void DisplayInfo(HDC hdc);
	bool bInit();
	void OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool bAccept(class XSocket * pXSock);
	void FightzoneReserveProcessor();
	void FireBow(short iClientH, short dX, short dY);
	void PartyOperationResultHandler(char *pData);
	void PartyOperationResult_Delete(int iPartyID);
	bool _bItemLog(int iAction, int iGiveH, int iRecvH, class CItem * pItem, bool bForceItemLog = false);
	bool _bCheckGoodItem(class CItem * pItem);
	bool bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY, int iItemSpreadType, int iSpreadRange, int *iItemIDs, POINT *BasePos, int *iNumItem);
	void processDelayedEvent(const DelayEvent &ev);
	void GSM_RequestShutupPlayer(char * pGMName, uint16_t wReqServerID, uint16_t wReqClientH, uint16_t wTime, char * pPlayer);
	bool _bPKLog(int iAction, int iAttackerH, int iVictumH, char * pNPC);
	bool _bDecodeItemConfigFileContents(char * pData, uint32_t dwMsgSize);
	bool _bRegisterMap(char * pName);
	bool bReadBallSystemConfigFile(const char * cFn);
	
	void AFKChecker();
	void RemoveCrusadeNpcs(void);
	void RemoveCrusadeRecallTime(void);
	bool _bInitItemAttr(class CItem &pItem, int iItemID);
	void PURITYLIFE(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void EndHappyHour();
	void StarHappyHour();
	void HappyHuor();
	void closeHappyHour();
	void openHappyHour();
	void StartCaptureTheFlag();
	void EndCaptureTheFlag(int iOption);
	void _CreateCTFGUID(int iWinnerSide);
	void EnergySphereProcessor(bool bIsAdminCreate = false, CClient *clientPtr = 0);
	bool _bCrusadeLog(int iAction, int iData, const char * cName);
	
	int m_iMajesticPointsPerLevel;
	int m_iMajesticMinLevel;
	int m_iMajesticPointsMax;
	char m_cServerName[11];
	char m_cGameServerAddr[16];
	char m_cGameServerAddrInternal[16];
	char m_cGameServerAddrExternal[16];
	int m_iGameServerMode;
	char m_cLogServerAddr[16];
	int m_iGameServerPort;
	int m_iLogServerPort;
	int m_iLimitedUserExp;
	int m_iLevelExp20;
	Clients m_pClientList;
	Npcs m_pNpcList;
	Maps m_pMapList;
	class CNpcItem * m_pTempNpcItem[DEF_MAXNPCITEMS];
	DynamicObjects dynamicObjects_;
	DelayEvents delayEvents_;
	class CBallSystem * m_pBallItemConfigList[DEF_MAXBALLITEMS];
	class CMsg * m_pMsgQuene[DEF_MSGQUENESIZE];
	int m_iQueneHead;
	int m_iQueneTail;
	int m_iTotalMaps;
	class XSocket * m_pMainLogSock;
	class CMisc m_Misc;
	bool m_bIsGameStarted;
	bool m_bIsLogSockAvailable;
	bool m_bIsItemAvailable;
	bool m_bIsBuildItemAvailable;
	bool m_bIsNpcAvailable;
	bool m_bIsMagicAvailable;
	bool m_bIsSkillAvailable;
	bool m_bIsPortionAvailable;
	bool m_bIsQuestAvailable;
	bool m_bIsTeleportAvailable;
	class CItem * m_pItemConfigList[DEF_MAXITEMTYPES];
	class NpcArchetype * m_pNpcArchetypes[DEF_MAXNPCTYPES];
	class CMagic * m_pMagicConfigList[DEF_MAXMAGICTYPE];
	class CSkill * m_pSkillConfigList[DEF_MAXSKILLTYPE];
	class CQuest * m_pQuestConfigList[DEF_MAXQUESTTYPE];
	class CPortion * m_pCraftingConfigList[DEF_MAXPORTIONTYPES];
	char m_pMsgBuffer[DEF_MSGBUFFERSIZE + 1];
	HWND m_hWnd;
	int m_iTotalBots;
	int m_iMaxBots;
	int m_iTotalGameServerBots;
	int m_iTotalGameServerMaxBots;
	SYSTEMTIME m_MaxUserSysTime;
	bool m_bF1pressed;
	bool m_bF4pressed;
	bool m_bF12pressed;
	bool m_bF5pressed;
	bool m_bOnExitProcess;
	uint32_t m_dwExitProcessTime;
	uint32_t m_dwWhetherTime;
	uint32_t m_dwGameTime1;
	uint32_t m_dwGameTime2;
	uint32_t m_dwGameTime3;
	uint32_t m_dwGameTime4;
	uint32_t m_dwGameTime5;
	uint32_t m_dwGameTime6;
	uint32_t m_dwGameTime7;
	uint32_t m_dwFishTime;
	bool m_bIsCrusadeWarStarter;
	int m_iLatestCrusadeDayOfWeek;
	char m_cDayOrNight;
	int m_iSkillSSNpoint[102];
	class CMsg * m_pNoticeMsgList[DEF_MAXNOTIFYMSGS];
	int m_iTotalNoticeMsg;
	int m_iPrevSendNoticeMsg;
	uint32_t m_dwNoticeTime;
	uint32_t m_dwSpecialEventTime;
	bool m_bIsSpecialEventTime;
	char m_cSpecialEventType;
	int m_iLevelExpTable[3500]; //New 22/10/04
	class CFish * m_pFish[DEF_MAXFISHS];
	class CPortion * m_pPortionConfigList[DEF_MAXPORTIONTYPES];
	bool m_bIsServerShutdowned;
	char m_cShutDownCode;
	class CMineral * m_pMineral[DEF_MAXMINERALS];
	MapWPtr middlelandMap_;
	int m_iAresdenMapIndex;
	int m_iElvineMapIndex;
	int m_iBTFieldMapIndex;
	int m_iGodHMapIndex;
	int m_iAresdenOccupyTiles;
	int m_iElvineOccupyTiles;
	int m_iCurMsgs;
	int m_iMaxMsgs;
	uint32_t m_dwCanFightzoneReserveTime;
	int m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	int m_iFightzoneNoForceRecall;

	struct {
		__int64 iFunds;
		__int64 iCrimes;
		__int64 iWins;
	} m_stCityStatus[3];
	int m_iStrategicStatus;
	class XSocket * m_pSubLogSock[DEF_MAXSUBLOGSOCK];
	int m_iSubLogSockInitIndex;
	bool m_bIsSubLogSockAvailable[DEF_MAXSUBLOGSOCK];
	int m_iCurSubLogSockIndex;
	int m_iSubLogSockFailCount;
	int m_iSubLogSockActiveCount;
	int m_iAutoRebootingCount;
	class CBuildItem * m_pBuildItemList[DEF_MAXBUILDITEMS];
	class CItem * m_pDupItemIDList[DEF_MAXDUPITEMID];
	char * m_pNoticementData;
	uint32_t m_dwNoticementDataSize;
	uint32_t m_dwMapSectorInfoTime;
	int m_iMapSectorInfoUpdateCount;
	int m_iCrusadeCount;
	bool m_bIsCrusadeMode;
	bool m_bIsApocalypseMode;
	uint32_t m_dwApocalypseGUID;
	int m_iLogChatOption;

	struct {
		char cMapName[11];
		char cType;
		int dX;
		int dY;
	} m_stCrusadeStructures[DEF_MAXCRUSADESTRUCTURES];
	int m_iCollectedMana[3];
	int m_iAresdenMana;
	int m_iElvineMana;
	class CTeleportLoc m_pGuildTeleportLoc[DEF_MAXGUILDS];
	char m_cHeldenianMapName[11];
	int m_iLastCrusadeWinner;

	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	struct {
		char cType;
		char cSide;
		short sX;
		short sY;
	} m_stMiddleCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];

	struct {
		char m_cBannedIPaddress[21];
	} m_stBannedList[DEF_MAXBANNED];

	struct {
		char m_cGMName[11];
	} m_stAdminList[DEF_MAXADMINS];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stCrusadeWarSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int StartiHour;
		int StartiMinute;
		int EndiHour;
		int EndiMinute;
	} m_stHeldenianSchedule[DEF_MAXHELDENIAN];
	int m_iTotalMiddleCrusadeStructures;

	int m_iNpcConstructionPoint[DEF_MAXNPCTYPES];
	uint32_t m_dwCrusadeGUID;
	short m_sLastCrusadeDate;
	int m_iCrusadeWinnerSide;
	int m_iPlayerMaxLevel;

	struct {
		int iTotalMembers;
		int iIndex[9];
	} m_stPartyInfo[DEF_MAXCLIENTS];
	int m_iAdminLevelWho;
	int m_iAdminLevelGMKill;
	int m_iAdminLevelGMRevive;
	int m_iAdminLevelGMCloseconn;
	int m_iAdminLevelGMCheckRep;
	int m_iAdminLevelEnergySphere;
	int m_iAdminLevelShutdown;
	int m_iAdminLevelObserver;
	int m_iAdminLevelShutup;
	int m_iAdminLevelCallGaurd;
	int m_iAdminLevelSummonDemon;
	int m_iAdminLevelSummonDeath;
	int m_iAdminLevelReserveFightzone;
	int m_iAdminLevelCreateFish;
	int m_iAdminLevelTeleport;
	int m_iAdminLevelCheckIP;
	int m_iAdminLevelPolymorph;
	int m_iAdminLevelSetInvis;
	int m_iAdminLevelSetZerk;
	int m_iAdminLevelSetIce;
	int m_iAdminLevelGetNpcStatus;
	int m_iAdminLevelSetAttackMode;
	int m_iAdminLevelUnsummonAll;
	int m_iAdminLevelUnsummonDemon;
	int m_iAdminLevelSummon;
	int m_iAdminLevelSummonAll;
	int m_iAdminLevelSummonPlayer;
	int m_iAdminLevelDisconnectAll;
	int m_iAdminLevelEnableCreateItem;
	int m_iAdminLevelCreateItem;
	int m_iAdminLevelStorm;
	int m_iAdminLevelWeather;
	int m_iAdminLevelSetStatus;
	int m_iAdminLevelGoto;
	int m_iAdminLevelMonsterCount;
	int m_iAdminLevelSetRecallTime;
	int m_iAdminLevelUnsummonBoss;
	int m_iAdminLevelClearNpc;
	int m_iAdminLevelTime;
	int m_iAdminLevelPushPlayer;
	int m_iAdminLevelSummonGuild;
	int m_iAdminLevelCheckStatus;
	int m_iAdminLevelCleanMap;
	short m_sSlateSuccessRate;
	short m_sForceRecallTime;
	int m_iPrimaryDropRate;
	int m_iSecondaryDropRate;
	int m_iFinalShutdownCount;
	bool m_bEnemyKillMode;
	int m_iEnemyKillAdjust;
	bool m_bAdminSecurity;
	short m_sRaidTimeMonday;
	short m_sRaidTimeTuesday;
	short m_sRaidTimeWednesday;
	short m_sRaidTimeThursday;
	short m_sRaidTimeFriday;
	short m_sRaidTimeSaturday;
	short m_sRaidTimeSunday;
	bool m_bManualTime;
	int m_iSummonGuildCost;
	bool m_bIsHeldenianMode;
	bool m_bIsHeldenianTeleport;
	int m_sCharPointLimit;
	short m_sCharStatLimit;
	bool m_bAllow100AllSkill;
	short m_sCharSkillLimit;
	char m_cRepDropModifier;
	char m_cSecurityNumber[11];
	short m_sMaxPlayerLevel;
	bool var_89C;
	bool var_8A0;
	char m_cHeldenianVictoryType;
	char m_sLastHeldenianWinner;
	char m_cHeldenianModeType;
	short m_iHeldenianAresdenDead;
	short m_iHeldenianElvineDead;
	short m_iHeldenianAresdenLeftTower;
	short m_iHeldenianElvineLeftTower;
	uint32_t m_dwHeldenianGUID;
	uint32_t m_dwHeldenianStartHour;
	uint32_t m_dwHeldenianStartMinute;
	int m_dwHeldenianStartTime;
	int m_dwHeldenianFinishTime;
	bool m_bReceivedItemList;
	bool m_bHeldenianInitiated;
	bool m_bHeldenianRunning;
	bool m_bHeldinianDuty;
	bool m_bHeldenianDutyMultiplyer;
	bool m_bIsHeldenianReady;
	bool m_bIsHeldenianScheduleLoaded;
	bool m_bIsHeldenianSchedule;
	bool m_bHappyHour;
	uint32_t dwHappyHourTime;
	bool m_bIsCTFMode;
	short m_sFlagCountWin;
	short m_sElvineFlagCount;
	short m_sAresdenFlagCount;
	bool m_bIsElvineFlagCaptured;
	bool m_bIsAresdenFlagCaptured;
	int m_iFlagCarrierIndex;
	short iH;
};