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
#define DEF_MAXMAPS					100
#define DEF_MAXAGRICULTURE			200
#define DEF_MAXNPCTYPES				200
#define DEF_MAXBUILDITEMS			300
#define DEF_SERVERSOCKETBLOCKLIMIT	300
#define DEF_MAXBANNED				500
#define DEF_MAXBALLITEMS			50
#define DEF_MAXNPCITEMS				1000
#define DEF_MAXCLIENTS				2000
#define DEF_MAXNPCS					15000
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

#define DEF_EXPSTOCKTIME		1000*10		// ExpStock�� ����ϴ� �ð� ���� 
#define DEF_MSGQUENESIZE		100000		// �޽��� ť ������ 10���� 
#define DEF_AUTOEXPTIME			1000*60*6	// �ڵ����� ����ġ�� �ö󰡴� �ð����� 
#define DEF_TOTALLEVELUPPOINT	3			// �������� �Ҵ��ϴ� �� ����Ʈ �� 


#define DEF_MAXDYNAMICOBJECTS	60000
#define DEF_MAXDELAYEVENTS		60000
#define DEF_GUILDSTARTRANK		12

#define DEF_SSN_LIMIT_MULTIPLY_VALUE	2	// SSN-limit ���ϴ� �� 

#define DEF_MAXNOTIFYMSGS		300			// �ִ� �������� �޽��� 
#define DEF_MAXSKILLPOINTS		700			// ��ų ����Ʈ�� ���� 
#define DEF_NIGHTTIME			40

#define DEF_CHARPOINTLIMIT		1000		// ������ Ư��ġ�� �ִ밪 
#define DEF_RAGPROTECTIONTIME	7000		// �� �� �̻� ������ ������ ���� ��ȣ�� �޴��� 
#define DEF_MAXREWARDGOLD		99999999	// ����� �ִ�ġ 

#define DEF_ATTACKAI_NORMAL				1	// ������ ��� 
#define DEF_ATTACKAI_EXCHANGEATTACK		2	// ��ȯ ��� - ���� 
#define DEF_ATTACKAI_TWOBYONEATTACK		3	// 2-1 ���, ���� 

#define DEF_MAXFISHS					200
#define DEF_MAXMINERALS					200
#define	DEF_MAXCROPS					200
#define DEF_MAXENGAGINGFISH				30  // �� ����⿡ ���ø� �õ��� �� �ִ� �ִ� �ο� 
#define DEF_MAXPORTIONTYPES				500 // �ִ� ���� ���� ���� 

#define DEF_SPECIALEVENTTIME			300000 //600000 // 10��
#define DEF_MAXQUESTTYPE				200
#define DEF_DEF_MAXHELDENIANDOOR			10

#define DEF_MAXSUBLOGSOCK				10

#define DEF_ITEMLOG_GIVE				1
#define DEF_ITEMLOG_DROP				2
#define DEF_ITEMLOG_GET					3
#define DEF_ITEMLOG_DEPLETE				4
#define DEF_ITEMLOG_NEWGENDROP			5
#define DEF_ITEMLOG_DUPITEMID			6

// New 07/05/2004
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

#define DEF_MAXGUILDS					1000 // ���ÿ� ������ �� �ִ� ���� 
#define DEF_MAXONESERVERUSERS			800	// 800 // �� �������� ����� �� �ִ� �ִ� ����ڼ�. �ʰ�� ��� ��Ȱ�� Ȥ�� �?�� ����, ������� �������.

#define DEF_MAXCONSTRUCTNUM				10
#define DEF_MAXSCHEDULE					10
#define DEF_MAXHELDENIAN				10

//MIHD Criticals CONFIG
#define DEF_CRITICALSMINLVL            20
#define DEF_GOLDMINLVL                5000
#define DEF_CRITICALSMAXLVL            180
#define DEF_MAJESTICSMAXLVL            2
#define DEF_CRITICALSGM                2000
#define DEF_MAXCRITICALS            5000
//FIN Criticals Config


//v1.4311-3  �������� �ִ� ����
#define DEF_MAXFIGHTZONE 10 

//============================
#define DEF_LEVELLIMIT		20				// ü���� ���� ����ġ!!!			
//============================

//============================
#define DEF_MINIMUMHITRATIO 15				// ���� ���� Ȯ�� 
//============================		

//============================
#define DEF_MAXIMUMHITRATIO	99				// �ִ� ���� Ȯ��
//============================

//============================
#define DEF_PLAYERMAXLEVEL	180				// �ִ� ����: Npc.cfg ���Ͽ� �����Ǿ� ���� ���� ��� m_iPlayerMaxLevel�� �Էµȴ�.
//============================

//============================
// New Changed 12/05/2004
#define DEF_GMGMANACONSUMEUNIT	15			// Grand Magic Generator ���� ��� ����.
//============================

#define DEF_MAXCONSTRUCTIONPOINT 30000		// �ִ� ��ȯ ����Ʈ 
#define DEF_MAXSUMMONPOINTS		 30000
#define DEF_MAXWARCONTRIBUTION	 200000


// MOG Definitions - 3.51
// Level up MSG
#define MSGID_LEVELUPSETTINGS				0x11A01000
// 2003-04-14 ���� ����Ʈ�� ���� ������ ���� �ִ�...
// Stat Point Change MSG
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

class CGame {
public:

	int iUpgradeHeroCapeRequirements(int iClientH, int iItemIndex);
	int HeroItemChecker(int iItemID, short sEnemyKill, char cContribution, char cSide);
	void AdminOrder_GetFightzoneTicket(int iClientH);

	void GetAngelHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp);

	void ReadMajesticSettings();
	void ReqCreateCraftingHandler(int iClientH, char *pData);
	//Heldenian 3.00 Source Code
	void HeldenianWinner();
	void RequestHeldenianScroll(int iClientH, char * pData, DWORD dwMsgSize);
	void CheckHeldenianResultCalculation(int iClientH);
	void AutomaticHeldenianEnd();
	void SetHeldenianMode();
	void AutomatedHeldenianTimer();
	void LocalStartHeldenianMode(short sV1, short sV2, DWORD dwHeldenianGUID);
	void GlobalStartHeldenianMode();
	void HeldenianWarEnder();
	void HeldenianWarStarter();
	BOOL UpdateHeldenianStatus();
	void _CreateHeldenianGUID(DWORD dwHeldenianGUID, int iWinnerSide);
	void ManualStartHeldenianMode(int iClientH, char *pData, DWORD dwMsgSize);
	void ManualEndHeldenianMode(int iClientH, char *pData, DWORD dwMsgSize);
	void NotifyStartHeldenianFightMode();
	void GlobalEndHeldenianMode();
	void LocalEndHeldenianMode();
	BOOL bNotifyHeldenianWinner();
	void RequestHeldenianTeleport(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestNpcSpeakTeleport(int iClientH, char * pData, DWORD dwMsgSize);
	BOOL bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType);
	BOOL bReadHeldenianGUIDFile(char * cFn);
	void RemoveEventNpc(int iNpcH);

	BOOL _bCheckCharacterData(int iClientH);
	//BOOL _bDecodeNpcItemConfigFileContents(char * pData, DWORD dwMsgSize);
	void GlobalUpdateConfigs(char cConfigType);
	void LocalUpdateConfigs(char cConfigType);
	void RemoveOccupyFlags(int iMapIndex);
	void SetHeroFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void CalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, char cAttackerType, int iArmorType);
	char _cCheckHeroItemEquipped(int iClientH);
	BOOL bPlantSeedBag(int iMapIndex, int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH);
	void _CheckFarmingAction(short sAttackerH, short sTargetH, BOOL bType);

	void GreenBall_Weather(int iClientH, char * pData, DWORD dwMsgSize);
	BOOL bReadScheduleConfigFile(char *pFn);

	// KLKS clean tiles
	void AdminOrder_CleanMap(int iClientH, char * pData, DWORD dwMsgSize);

	// Lists
	BOOL bReadBannedListConfigFile(const char *pFn);
	BOOL bReadAdminListConfigFile(const char *pFn);

	void AdminOrder_CheckStats(int iClientH, char *pData, DWORD dwMsgSize);
	void Command_RedBall(int iClientH, char *pData, DWORD dwMsgSize);
	void Command_BlueBall(int iClientH, char *pData, DWORD dwMsgSize);
	void Command_YellowBall(int iClientH, char* pData, DWORD dwMsgSize);

	// Crusade
	void ManualEndCrusadeMode(int iWinnerSide); // 2.17 (x) 2.14 ( )
	void CrusadeWarStarter();
	BOOL bReadCrusadeGUIDFile(char * cFn);
	void _CreateCrusadeGUID(DWORD dwCrusadeGUID, int iWinnerSide);
	void GlobalStartCrusadeMode();

	// Apocalypse
	void GlobalStartApocalypseMode();
	void GlobalEndApocalypseMode();
	void LocalStartApocalypseMode(DWORD dwApocalypseGUID);
	void LocalEndApocalypseMode();
	void _CreateApocalypseGUID(DWORD dwApocalypseGUID);
	BOOL bReadApocalypseGUIDFile(char * cFn);
	void SendThunder(int iClient, short sX, short sY, short sV3, short sV4);
	void DoAbaddonThunderDamageHandler(char cMapIndex);

	void GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char * pMapName);
	void SyncMiddlelandMapInfo();
	void RemoveCrusadeStructures();
	void _SendMapStatus(int iClientH);
	void MapStatusHandler(int iClientH, int iMode, char * pMapName);
	void SelectCrusadeDutyHandler(int iClientH, int iDuty);
	void RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode);
	void RequestGuildTeleportHandler(int iClientH);
	void RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, const char * pMapName);
	void MeteorStrikeHandler(int iMapIndex);
	void _LinkStrikePointMapIndex();
	void MeteorStrikeMsgHandler(char cAttackerSide);
	void _NpcBehavior_GrandMagicGenerator(int iNpcH);
	void CollectedManaHandler(WORD wAresdenMana, WORD wElvineMana);
	void SendCollectedMana();
	void CreateCrusadeStructures();
	void _GrandMagicLaunchMsgSend(int iType, char cAttackerSide);
	void GrandMagicResultHandler(char *cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char * cData);
	void CalcMeteorStrikeEffectHandler(int iMapIndex);
	void DoMeteorStrikeDamageHandler(int iMapIndex);
	void RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char * pMapName);
	void GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char * pMapName);
	void GSM_ConstructionPoint(int iGuildGUID, int iPoint);
	void CheckCommanderConstructionPoint(int iClientH);
	BOOL bReadCrusadeStructureConfigFile(char * cFn);
	void SaveOccupyFlagData();
	void LocalEndCrusadeMode(int iWinnerSide);
	void LocalStartCrusadeMode(DWORD dwGuildGUID);
	void CheckCrusadeResultCalculation(int iClientH);
	BOOL _bNpcBehavior_Detector(int iNpcH);
	BOOL _bNpcBehavior_ManaCollector(int iNpcH);
	BOOL __bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH);

	//void AdminOrder_SummonGuild(int iClientH, char * pData, DWORD dwMsgSize);

	// Acidx commands
	void AdminOrder_Time(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CheckRep(int iClientH, char *pData);
	void AdminOrder_Pushplayer(int iClientH, char * pData, DWORD dwMsgSize);

	void AdminOrder_CheckRep(int iClientH, char *pData, DWORD dwMsgSize);

	void SetForceRecallTime(int iClientH);
	void ApplyCombatKilledPenalty(int iClientH, int cPenaltyLevel, BOOL bIsSAattacked);

	//	void AdminOrder_ClearTiles(int iClientH);
	void AdminOrder_ClearNpc(int iClientH);

	// Settings.cfg
	BOOL bReadSettingsConfigFile(const char * cFn);

	//  BOOL bReadTeleportConfigFile(char * cFn);
	//	void RequestTeleportD2Handler(int iClientH, char * pData);

	// Daryl - AdminSettings.cfg
	BOOL bReadAdminSetConfigFile(const char * cFn);


	// Hack Checks
	BOOL bCheckClientMoveFrequency(int iClientH, DWORD dwClientTime);
	BOOL bCheckClientMagicFrequency(int iClientH, DWORD dwClientTime);
	BOOL bCheckClientAttackFrequency(int iClientH, DWORD dwClientTime);

	// BOOL bCheckClientInvisibility(short iClientH);

	//Hypnotoad functions
	void SetDefenseShieldFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetMagicProtectionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetIllusionMovementFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetIllusionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);

	void RequestChangePlayMode(int iClientH);
	void GetHeroMantleHandler(int iClientH, int iItemID, char * pString);
	void AdminOrder_Weather(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SendMSG(int iClientH, char *pData, DWORD dwMsgSize);
	void SendMsg(short sOwnerH, char cOwnerType, BOOL bStatus, long lPass);
	BOOL bCheckMagicInt(int iClientH);
	BOOL bChangeState(char cStateChange, char* cStr, char *cVit, char *cDex, char *cInt, char *cMag, char *cChar);
	void StateChangeHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SetStatus(int iClientH, char *pData, DWORD dwMsgSize);
	void SetStatusFlag(short sOwnerH, char cOwnerType, BOOL bStatus, int iPass);
	void SetPoisonFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void AdminOrder_SummonStorm(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CallMagic(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SummonDeath(int iClientH);
	void AdminOrder_SetZerk(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SetFreeze(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Kill(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Revive(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SetObserverMode(int iClientH);
	void AdminOrder_EnableAdminCommand(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_CreateItem(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Summon(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SummonAll(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SummonPlayer(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_UnsummonDemon(int iClientH);
	void AdminOrder_UnsummonAll(int iClientH);
	void AdminOrder_SetAttackMode(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SummonDemon(int iClientH);
	void AdminOrder_SetInvi(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Polymorph(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_GetNpcStatus(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CheckIP(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CreateFish(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Teleport(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_ReserveFightzone(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CloseConn(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CallGuard(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_DisconnectAll(int iClientH, char * pData, DWORD dwMsgSize);

	BOOL bCopyItemContents(class CItem * pOriginal, class CItem * pCopy);
	int iGetMapLocationSide(char * pMapName);
	void ChatMsgHandlerGSM(int iMsgType, int iV1, char * pName, char * pData, DWORD dwMsgSize);
	void RemoveClientShortCut(int iClientH);
	BOOL bAddClientShortCut(int iClientH);

	void GSM_RequestFindCharacter(WORD wReqServerID, WORD wReqClientH, char *pName, char * pFinder); // New 16/05/2001 Changed
	void ServerStockMsgHandler(char * pData);
	void RequestHelpHandler(int iClientH);

	void CheckConnectionHandler(int iClientH, char *pData);

	void AgingMapSectorInfo();
	void UpdateMapSectorInfo();
	//BOOL bGetItemNameWhenDeleteNpc(char * pItemName, short sNpcType);
	BOOL bGetItemNameWhenDeleteNpc(int & iItemID, short sNpcType);
	int iGetItemWeight(class CItem * pItem, int iCount);
	void CancelQuestHandler(int iClientH);
	void ActivateSpecialAbilityHandler(int iClientH);
	void EnergySphereProcessor(BOOL bIsAdminCreate = FALSE, int iClientH = 0);
	BOOL bCheckEnergySphereDestination(int iNpcH, short sAttackerH, char cAttackerType);
	void JoinPartyHandler(int iClientH, int iV1, char *pMemberName);
	void CreateNewPartyHandler(int iClientH);
	void _DeleteRandomOccupyFlag(int iMapIndex);
	void RequestSellItemListHandler(int iClientH, char * pData);
	void RequestRestartHandler(int iClientH);
	int iRequestPanningMapDataRequest(int iClientH, char * pData);
	void GetMagicAbilityHandler(int iClientH);
	void Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, BOOL bExp, int iAttr);
	void _TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY);
	void RequestCheckAccountPasswordHandler(char * pData, DWORD dwMsgSize);
	int _iTalkToNpcResult_Guard(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void SetIceFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void _bDecodeNoticementFileContents(char * pData, DWORD dwMsgSize);
	void RequestNoticementHandler(int iClientH, char * pData);
	void _AdjustRareItemValue(class CItem * pItem);
	BOOL _bCheckDupItemID(class CItem * pItem);
	BOOL _bDecodeDupItemIDFileContents(char * pData, DWORD dwMsgSize);
	void NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType);
	int iGetPlayerABSStatus(int iWhatH, int iRecvH);
	void CheckSpecialEvent(int iClientH);
	char _cGetSpecialAbility(int iKindSA);
	void BuildItemHandler(int iClientH, char * pData);
	BOOL _bDecodeBuildItemConfigFileContents(char * pData, DWORD dwMsgSize);
	BOOL _bCheckSubLogSocketIndex();
	void OnSubLogRead(int iIndex);
	void OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void _CheckStrategicPointOccupyStatus(char cMapIndex);
	void GetMapInitialPoint(int iMapIndex, short * pX, short * pY, char * pPlayerLocation = NULL);
	int iGetMaxHP(int iClientH);
	int iGetMaxMP(int iClientH);
	int iGetMaxSP(int iClientH);
	void _ClearQuestStatus(int iClientH);
	BOOL _bCheckItemReceiveCondition(int iClientH, class CItem * pItem);
	void SendItemNotifyMsg(int iClientH, WORD wMsgType, class CItem * pItem, int iV1);

	int _iTalkToNpcResult_WTower(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_WHouse(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_BSmith(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_GShop(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_GuildHall(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	BOOL _bCheckIsQuestCompleted(int iClientH);
	void _CheckQuestEnvironment(int iClientH);
	void _SendQuestContents(int iClientH);
	void QuestAcceptedHandler(int iClientH);
	BOOL _bDecodeQuestConfigFileContents(char * pData, DWORD dwMsgSize);

	void CancelExchangeItem(int iClientH);
	BOOL bAddItem(int iClientH, class CItem * pItem, char cMode);
	void ConfirmExchangeItem(int iClientH);
	void SetExchangeItem(int iClientH, int iItemIndex, int iAmount);
	void ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char * pItemName);

	void _BWM_Command_Shutup(char * pData);
	void _BWM_Init(int iClientH, char * pData);
	void CheckUniqueItemEquipment(int iClientH);
	void _SetItemPos(int iClientH, char * pData);
	//void GetHeroMantleHandler(int iClientH);

	BOOL _bDecodeOccupyFlagSaveFileContents(char * pData, DWORD dwMsgSize);
	void GetOccupyFlagHandler(int iClientH);
	int _iComposeFlagStatusContents(char * pData);
	void SetSummonMobAction(int iClientH, int iMode, DWORD dwMsgSize, char * pData = NULL);
	BOOL __bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, BOOL bAdminFlag);
	BOOL _bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID);
	void SetDownSkillIndexHandler(int iClientH, int iSkillIndex);
	int iGetComboAttackBonus(int iSkill, int iComboCount);
	int _iGetWeaponSkillType(int iClientH);
	void CheckFireBluring(char cMapIndex, int sX, int sY);
	void NpcTalkHandler(int iClientH, int iWho);
	BOOL bDeleteMineral(int iIndex);
	void _CheckMiningAction(int iClientH, int dX, int dY);
	int iCreateMineral(char cMapIndex, int tX, int tY, char cLevel);
	void MineralGenerator();
	void LocalSavePlayerData(int iClientH);
	BOOL _bDecodePortionConfigFileContents(char * pData, DWORD dwMsgSize);
	void ReqCreatePortionHandler(int iClientH, char * pData);
	void _CheckAttackType(int iClientH, short * spType);
	BOOL bOnClose();
	void ForceDisconnectAccount(char * pAccountName, WORD wCount);
	void NpcRequestAssistance(int iNpcH);
	void ToggleSafeAttackModeHandler(int iClientH);
	void SetBerserkFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SpecialEventHandler();
	int iGetPlayerRelationship_SendEvent(int iClientH, int iOpponentH);
	int iGetNpcRelationship_SendEvent(int iNpcH, int iOpponentH);
	int _iForcePlayerDisconect(int iNum);
	int iGetMapIndex(char * pMapName);
	//int iGetNpcRelationship(int iClientH, int iOpponentH);
	int iGetNpcRelationship(int iWhatH, int iRecvH);
	int iGetPlayerRelationship(int iClientH, int iOpponentH);
	int iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus);
	void WhetherProcessor();
	int _iCalcPlayerNum(char cMapIndex, short dX, short dY, char cRadius);
	void FishGenerator();
	void ReqGetFishThisTimeHandler(int iClientH);
	void FishProcessor();
	int iCheckFish(int iClientH, char cMapIndex, short dX, short dY);
	BOOL bDeleteFish(int iHandle, int iDelMode);
	int iCreateFish(char cMapIndex, short sX, short sY, short sDifficulty, class CItem * pItem, int iDifficulty, DWORD dwLastTime);
	void UserCommand_DissmissGuild(int iClientH, char * pData, DWORD dwMsgSize);
	// v1.4311-3 ���� �Լ� ����  Expire -> Ban ���� 
	void UserCommand_BanGuildsman(int iClientH, char * pData, DWORD dwMsgSize);
	// v1.4311-3 �߰� �Լ� ���� �̺�Ʈ�� ����Ͽ� ��ڰ� �������� �����س��´�.
	int iGetExpLevel(int iExp);
	void ___RestorePlayerRating(int iClientH);
	void CalcExpStock(int iClientH);
	void ResponseSavePlayerDataReplyHandler(char * pData, DWORD dwMsgSize);
	void NoticeHandler();
	BOOL bReadNotifyMsgListFile(const char * cFn);
	void SetPlayerReputation(int iClientH, char * pMsg, char cValue, DWORD dwMsgSize);
	void ShutUpPlayer(int iClientH, char * pMsg, DWORD dwMsgSize);
	void CheckDayOrNightMode();
	BOOL bCheckBadWord(char * pString);
	BOOL bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType);
	void PoisonEffect(int iClientH, int iV1);
	void bSetNpcAttackMode(char * cName, int iTargetH, char cTargetType, BOOL bIsPermAttack);
	BOOL _bGetIsPlayerHostile(int iClientH, int sOwnerH);
	BOOL bAnalyzeCriminalAction(int iClientH, short dX, short dY, BOOL bIsCheck = FALSE);
	void RequestAdminUserMode(int iClientH, char * pData);
	int _iGetPlayerNumberOnSpot(short dX, short dY, char cMapIndex, char cRange);
	void CalcTotalItemEffect(int iClientH, int iEquipItemID, BOOL bNotify = TRUE);
	void ___RestorePlayerCharacteristics(int iClientH);
	void GetPlayerProfile(int iClientH, char * pMsg, DWORD dwMsgSize);
	void SetPlayerProfile(int iClientH, char * pMsg, DWORD dwMsgSize);
	void ToggleWhisperPlayer(int iClientH, char * pMsg, DWORD dwMsgSize);
	void CheckAndNotifyPlayerConnection(int iClientH, char * pMsg, DWORD dwSize);
	int iCalcTotalWeight(int iClientH);
	void ReqRepairItemCofirmHandler(int iClientH, char cItemID, char * pString);
	void ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char * pString);
	void ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char * pString);
	void ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char * pItemName);
	void UseSkillHandler(int iClientH, int iV1, int iV2, int iV3);
	int iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, char cMapIndex, int dX, int dY);
	void ClearSkillUsingStatus(int iClientH);
	void DynamicObjectEffectProcessor();
	int _iGetTotalClients();
	void SendObjectMotionRejectMsg(int iClientH);
	void SetInvisibilityFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	BOOL bRemoveFromDelayEventList(int iH, char cType, int iEffectType);
	void DelayEventProcessor();
	BOOL bRegisterDelayEvent(int iDelayType, int iEffectType, DWORD dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	int iGetFollowerNumber(short sOwnerH, char cOwnerType);
	int _iCalcSkillSSNpoint(int iLevel);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	BOOL bCheckTotalSkillMasteryPoints(int iClientH, int iSkill);
	BOOL bSetItemToBankItem(int iClientH, class CItem * pItem);
	void NpcMagicHandler(int iNpcH, short dX, short dY, short sType);
	BOOL bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	BOOL bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	void Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, BOOL bExp, int iAttr = 0);
	void Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, BOOL bExp, int iAttr);
	void UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID);
	void NpcBehavior_Stop(int iNpcH);
	void ItemDepleteHandler(int iClientH, short sItemIndex, BOOL bIsUseItemResult, BOOL bIsItemUsed);
	int _iGetArrowItemIndex(int iClientH);
	void RequestFullObjectData(int iClientH, char * pData);
	void DeleteNpc(int iNpcH);
	void CalcNextWayPointDestination(int iNpcH);
	void MobGenerator();
	void CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue);
	void CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue);
	void CheckDynamicObjectList();
	int iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwLastTime, int iV1 = 0);
	int _iCalcMaxLoad(int iClientH);
	void GetRewardMoneyHandler(int iClientH);
	void _PenaltyItemDrop(int iClientH, int iTotal, BOOL bIsSAattacked = FALSE);
	//void ApplyCombatKilledPenalty(int iClientH, char cPenaltyLevel, BOOL bIsSAattacked = FALSE);
	void EnemyKillRewardHandler(int iAttackerH, int iClientH);
	void PK_KillRewardHandler(short sAttackerH, short sVictumH);
	void ApplyPKpenalty(short sAttackerH, short sVictumH);
	BOOL bSetItemToBankItem(int iClientH, short sItemIndex);
	void RequestRetrieveItemHandler(int iClientH, char *pData);
	void RequestCivilRightHandler(int iClientH, char * pData);
	BOOL bCheckLimitedUser(int iClientH);
	void LevelUpSettingsHandler(int iClientH, char * pData, DWORD dwMsgSize);
	// v1.4311-3 ���� �Լ�  ������ ���� �Լ� ���� FightzoneReserveHandler
	void FightzoneReserveHandler(int iClientH, char * pData, DWORD dwMsgSize);
	BOOL bCheckLevelUp(int iClientH);
	int iGetLevelExp(int iLevel);
	void TimeManaPointsUp(int iClientH);
	void TimeStaminarPointsUp(int iClientH);
	void Quit();
	BOOL __bReadMapInfo(int iMapIndex);
	BOOL bBankItemToPlayer(int iClientH, short sItemIndex);
	BOOL bPlayerItemToBank(int iClientH, short sItemIndex);
	int _iGetSkillNumber(char * pSkillName);
	void TrainSkillResponse(BOOL bSuccess, int iClientH, int iSkillNum, int iSkillLevel);
	int _iGetMagicNumber(char * pMagicName, int * pReqInt, int * pCost);
	void RequestStudyMagicHandler(int iClientH, char * pName, BOOL bIsPurchase = TRUE);
	BOOL _bDecodeSkillConfigFileContents(char * pData, DWORD dwMsgSize);
	BOOL _bDecodeMagicConfigFileContents(char * pData, DWORD dwMsgSize);
	void ReleaseFollowMode(short sOwnerH, char cOwnerType);
	BOOL bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType);
	void RequestTeleportHandler(int iClientH, const char * pData, const char * cMapName = NULL, int dX = -1, int dY = -1);
	void PlayerMagicHandler(int iClientH, int dX, int dY, short sType, BOOL bItemEffect = FALSE, int iV1 = 0);
	int iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir);
	void ToggleCombatModeHandler(int iClientH);
	void GuildNotifyHandler(char * pData, DWORD dwMsgSize);
	void SendGuildMsg(int iClientH, WORD wNotifyMsgType, short sV1, short sV2, char * pString);
	void DelayEventProcess();
	void TimeHitPointsUp(int iClientH);
	void CalculateGuildEffect(int iVictimH, char cVictimType, short sAttackerH);
	void OnStartGameSignal();
	int iDice(int iThrow, int iRange);
	BOOL _bInitNpcAttr(class CNpc * pNpc, char * pNpcName, short sClass, char cSA);
	BOOL _bDecodeNpcConfigFileContents(char * pData, DWORD dwMsgSize);
	void ReleaseItemHandler(int iClientH, short sItemIndex, BOOL bNotice);
	void ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage);
	int SetItemCount(int iClientH, char * pItemName, DWORD dwCount);
	int SetItemCount(int iClientH, int iItemIndex, DWORD dwCount);
	DWORD dwGetItemCount(int iClientH, char * pName);
	void DismissGuildRejectHandler(int iClientH, char * pName);
	void DismissGuildApproveHandler(int iClientH, char * pName);
	void JoinGuildRejectHandler(int iClientH, char * pName);
	void JoinGuildApproveHandler(int iClientH, char * pName);
	void SendNotifyMsg(int iFromH, int iToH, WORD wMsgType, DWORD sV1, DWORD sV2, DWORD sV3, char * pString, DWORD sV4 = 0, DWORD sV5 = 0, DWORD sV6 = 0, DWORD sV7 = 0, DWORD sV8 = 0, DWORD sV9 = 0, char * pString2 = NULL);
	void GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char * pItemName);
	void RequestPurchaseItemHandler(int iClientH, char * pItemName, int iNum);
	void ResponseDisbandGuildHandler(char * pData, DWORD dwMsgSize);
	void RequestDisbandGuildHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestCreateNewGuildHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void ResponseCreateNewGuildHandler(char * pData, DWORD dwMsgSize);
	int iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir);

	BOOL bEquipItemHandler(int iClientH, short sItemIndex, BOOL bNotify = TRUE);
	BOOL _bAddClientItemList(int iClientH, class CItem * pItem, int * pDelReq);
	int iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir);
	void DropItemHandler(int iClientH, short sItemIndex, int iAmount, char * pItemName, BOOL bByPlayer = TRUE);
	void ClientCommonHandler(int iClientH, char * pData);
	BOOL __fastcall bGetMsgQuene(char * pFrom, char * pData, DWORD * pMsgSize, int * pIndex, char * pKey);
	void MsgProcess();
	BOOL __fastcall bPutMsgQuene(char cFrom, char * pData, DWORD dwMsgSize, int iIndex, char cKey);
	void NpcBehavior_Flee(int iNpcH);
	int iGetDangerValue(int iNpcH, short dX, short dY);
	void NpcBehavior_Dead(int iNpcH);
	void NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage);
	//int  iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, BOOL bNearAttack = FALSE);
	int iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, BOOL bNearAttack = FALSE, BOOL bIsDash = FALSE, BOOL bArrowUse = FALSE);
	void RemoveFromTarget(short sTargetH, char cTargetType, int iCode = 0);
	void NpcBehavior_Attack(int iNpcH);
	void TargetSearch(int iNpcH, short * pTarget, char * pTargetType);
	void NpcBehavior_Move(int iNpcH);
	BOOL bGetEmptyPosition(short * pX, short * pY, char cMapIndex);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int * pError);
	int iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, WORD wTargetObjectID, BOOL bResponse = TRUE, BOOL bIsDash = FALSE);
	void ChatMsgHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void NpcProcess();
	int bCreateNewNpc(char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, BOOL bHideGenMode, BOOL bIsSummoned = FALSE, BOOL bFirmBerserk = FALSE, BOOL bIsMaster = FALSE, int iGuildGUID = 0);
	//BOOL bCreateNewNpc(char * pNpcName, char * pName, char * pMapName, short sX, short sY);
	BOOL _bReadMapInfoFiles(int iMapIndex);

	BOOL _bGetIsStringIsNumber(char * pStr);
	BOOL _bInitItemAttr(class CItem * pItem, char * pItemName);
	BOOL bReadProgramConfigFile(const char * cFn);
	void GameProcess();
	void InitPlayerData(int iClientH, char * pData, DWORD dwSize);
	void ResponsePlayerDataHandler(char * pData, DWORD dwSize);
	BOOL bSendMsgToLS(DWORD dwMsg, int iClientH, BOOL bFlag = TRUE, char *pData = NULL);
	void OnMainLogRead();
	void OnMainLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void CheckClientResponseTime();
	void OnTimer(char cType);
	int iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char * pData);
	void SendEventToNearClient_TypeB(DWORD dwMsgID, WORD wMsgType, char cMapIndex, short sX, short sY, short sV1, short sV2, short sV3, short sV4 = 0);
	void SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, DWORD dwMsgID, WORD wMsgType, short sV1, short sV2, short sV3);
	void DeleteClient(int iClientH, BOOL bSave, BOOL bNotify, BOOL bCountLogout = TRUE, BOOL bForceCloseConn = FALSE);
	int iComposeInitMapData(short sX, short sY, int iClientH, char * pData);
	void RequestInitDataHandler(int iClientH, char * pData, char cKey);
	void RequestInitPlayerHandler(int iClientH, char * pData, char cKey);
	int iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType);
	void ClientMotionHandler(int iClientH, char * pData);
	void DisplayInfo(HDC hdc);
	void OnClientRead(int iClientH);
	BOOL bInit();
	void OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL bAccept(class XSocket * pXSock);
	// v1.4311-3 �Լ� ���� ����� �޴� �Լ� .. void GetFightzoneTicketHandler(int iClientH);
	void GetFightzoneTicketHandler(int iClientH);
	// v1.4311-3 2 �ð����� ������ ������ �ʱ�ȭ ��Ų��.
	void FightzoneReserveProcessor();

	// New 06/05/2004
	// Upgrades
	BOOL bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, BOOL bBonus = FALSE);
	void RequestItemUpgradeHandler(int iClientH, int iItemIndex);

	// ArchAngle's Codes
	void StormBringer(int iClientH, short dX, short dY);
	void FireBow(short iClientH, short dX, short dY);

	//Party Codes
	void RequestCreatePartyHandler(int iClientH);
	void PartyOperationResultHandler(char *pData);
	void PartyOperationResult_Create(int iClientH, char *pName, int iResult, int iPartyID);
	void PartyOperationResult_Join(int iClientH, char *pName, int iResult, int iPartyID);
	void PartyOperationResult_Dismiss(int iClientH, char *pName, int iResult, int iPartyID);
	void PartyOperationResult_Delete(int iPartyID);
	void RequestJoinPartyHandler(int iClientH, char *pData, DWORD dwMsgSize);
	void RequestDismissPartyHandler(int iClientH);
	void GetPartyInfoHandler(int iClientH);
	void PartyOperationResult_Info(int iClientH, char * pName, int iTotal, char *pNameList);
	void RequestDeletePartyHandler(int iClientH);
	void RequestAcceptJoinPartyHandler(int iClientH, int iResult);
	void GetExp(int iClientH, int iExp, BOOL bIsAttackerOwn = FALSE);

	// New 07/05/2004
	// Guild Codes
	void RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex);

	// Item Logs
	BOOL _bItemLog(int iAction, int iClientH, char * cName, class CItem * pItem);
	BOOL _bItemLog(int iAction, int iGiveH, int iRecvH, class CItem * pItem, BOOL bForceItemLog = FALSE);
	BOOL _bCheckGoodItem(class CItem * pItem);

	BOOL bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex);
	void ArmorLifeDecrement(int iAttackerH, int iTargetH, char cOwnerType, int iValue);

	// MultiDrops
	BOOL bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
			  int iItemSpreadType, int iSpreadRange,
			  int *iItemIDs, POINT *BasePos, int *iNumItem);

	// Majestic Code By Diuuude
	int m_iMajesticPointsPerLevel;
	int m_iMajesticMinLevel;
	int m_iMajesticPointsMax;


	// Player shutup
	void GSM_RequestShutupPlayer(char * pGMName, WORD wReqServerID, WORD wReqClientH, WORD wTime, char * pPlayer);

	// PK Logs
	BOOL _bPKLog(int iAction, int iAttackerH, int iVictumH, char * pNPC);

	CGame(HWND hWnd);
	virtual ~CGame();

	char m_cServerName[11];
	char m_cGameServerAddr[16];
	char m_cGameServerAddrInternal[16];
	char m_cGameServerAddrExternal[16];
	int m_iGameServerMode;
	char m_cLogServerAddr[16];
	int m_iGameServerPort;
	int m_iLogServerPort;

	int m_iLimitedUserExp, m_iLevelExp20;

	BOOL _bDecodeItemConfigFileContents(char * pData, DWORD dwMsgSize);
	int _iComposePlayerDataFileContents(int iClientH, char * pData);
	BOOL _bDecodePlayerDatafileContents(int iClientH, char * pData, DWORD dwSize);
	BOOL _bRegisterMap(char * pName);

	class CClient * m_pClientList[DEF_MAXCLIENTS];
	class CNpc * m_pNpcList[DEF_MAXNPCS];
	class CMap * m_pMapList[DEF_MAXMAPS];
	class CNpcItem * m_pTempNpcItem[DEF_MAXNPCITEMS];
	class CDynamicObject * m_pDynamicObjectList[DEF_MAXDYNAMICOBJECTS];
	class CDelayEvent * m_pDelayEventList[DEF_MAXDELAYEVENTS];
	class CBallSystem * m_pBallItemConfigList[DEF_MAXBALLITEMS];

	class CMsg * m_pMsgQuene[DEF_MSGQUENESIZE];
	int m_iQueneHead, m_iQueneTail;
	int m_iTotalMaps;
	class XSocket * m_pMainLogSock;
	class CMisc m_Misc;
	BOOL m_bIsGameStarted;
	BOOL m_bIsLogSockAvailable;
	BOOL m_bIsItemAvailable, m_bIsBuildItemAvailable, m_bIsNpcAvailable, m_bIsMagicAvailable;
	BOOL m_bIsSkillAvailable, m_bIsPortionAvailable, m_bIsQuestAvailable, m_bIsTeleportAvailable;
	class CItem * m_pItemConfigList[DEF_MAXITEMTYPES];
	class CNpc * m_pNpcConfigList[DEF_MAXNPCTYPES];
	class CMagic * m_pMagicConfigList[DEF_MAXMAGICTYPE];
	class CSkill * m_pSkillConfigList[DEF_MAXSKILLTYPE];
	class CQuest * m_pQuestConfigList[DEF_MAXQUESTTYPE];

	class CPortion * m_pCraftingConfigList[DEF_MAXPORTIONTYPES];

	char m_pMsgBuffer[DEF_MSGBUFFERSIZE + 1];

	HWND m_hWnd;
	int m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients;
	int m_iTotalBots, m_iMaxBots, m_iTotalGameServerBots, m_iTotalGameServerMaxBots;
	SYSTEMTIME m_MaxUserSysTime;

	BOOL m_bF1pressed, m_bF4pressed, m_bF12pressed, m_bF5pressed;
	BOOL m_bOnExitProcess;
	DWORD m_dwExitProcessTime;

	DWORD m_dwWhetherTime, m_dwGameTime1, m_dwGameTime2, m_dwGameTime3, m_dwGameTime4, m_dwGameTime5, m_dwGameTime6, m_dwGameTime7, m_dwFishTime; //Orkl - added 7 for advertiser	
	// Crusade Schedule
	BOOL m_bIsCrusadeWarStarter;
	int m_iLatestCrusadeDayOfWeek;

	char m_cDayOrNight; // 1�̸� �ְ�, 2�� �߰� 
	int m_iSkillSSNpoint[102];

	class CMsg * m_pNoticeMsgList[DEF_MAXNOTIFYMSGS];
	int m_iTotalNoticeMsg, m_iPrevSendNoticeMsg;
	DWORD m_dwNoticeTime, m_dwSpecialEventTime;
	BOOL m_bIsSpecialEventTime;
	char m_cSpecialEventType;

	int m_iLevelExpTable[3500]; //New 22/10/04

	class CFish * m_pFish[DEF_MAXFISHS];
	class CPortion * m_pPortionConfigList[DEF_MAXPORTIONTYPES];

	BOOL m_bIsServerShutdowned;
	char m_cShutDownCode;
	class CMineral * m_pMineral[DEF_MAXMINERALS];

	int m_iMiddlelandMapIndex; // �������� �̵鷣�� �� �ε���: �� ���� -1�̸� �̵鷣�� ���� ������ �ǹ� 
	int m_iAresdenMapIndex; // �Ʒ����� �� �ε��� 
	int m_iElvineMapIndex; // ������ �� �ε���
	int m_iBTFieldMapIndex;
	int m_iGodHMapIndex;
	int m_iAresdenOccupyTiles;
	int m_iElvineOccupyTiles;
	int m_iCurMsgs, m_iMaxMsgs;

	// v1.4311-3 ���� ����  ����� �ð��� ������ ���� DWORD m_dwCanFightzoneReserveTime 
	DWORD m_dwCanFightzoneReserveTime;

	// v1.4311-3 ���� ���� �� �������� ���� ���θ� ���� int  m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	int m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	// v1.4311-3 ���� ���� �����忡 ������ ��� ��ɾ�� ���� �ʵǰ� ��.
	int m_iFightzoneNoForceRecall;

	struct {
		__int64 iFunds;
		__int64 iCrimes;
		__int64 iWins;

	} m_stCityStatus[3];

	int m_iStrategicStatus;

	class XSocket * m_pSubLogSock[DEF_MAXSUBLOGSOCK];
	int m_iSubLogSockInitIndex;
	BOOL m_bIsSubLogSockAvailable[DEF_MAXSUBLOGSOCK];
	int m_iCurSubLogSockIndex;
	int m_iSubLogSockFailCount;
	int m_iSubLogSockActiveCount; // v1.5 ���� ���� ���°� �����ǰ� �ִ� sub-log-socket ����
	int m_iAutoRebootingCount; // v1.5 �ڵ� ����� �� Ƚ�� 

	class CBuildItem * m_pBuildItemList[DEF_MAXBUILDITEMS];
	class CItem * m_pDupItemIDList[DEF_MAXDUPITEMID];

	char * m_pNoticementData;
	DWORD m_dwNoticementDataSize;

	DWORD m_dwMapSectorInfoTime;
	int m_iMapSectorInfoUpdateCount;

	// Crusade ó����
	int m_iCrusadeCount; // 0�̸� ����� ������. 1�̸� ī��Ʈ �ٿ� ����. 
	BOOL m_bIsCrusadeMode; // ũ�缼�̵� ���

	// Apocalypse
	BOOL m_bIsApocalypseMode;
	DWORD m_dwApocalypseGUID;

	// Daryl - Chat logging option
	BOOL m_bLogChatOption;

	struct {
		char cMapName[11]; // ��ġ�Ǵ� �� ��ġ
		char cType; // ���๰ ����. ��� NPC ������.
		int dX, dY; // ��ġ�Ǵ� �ʻ��� ��ġ

	} m_stCrusadeStructures[DEF_MAXCRUSADESTRUCTURES];


	int m_iCollectedMana[3];
	int m_iAresdenMana, m_iElvineMana;

	class CTeleportLoc m_pGuildTeleportLoc[DEF_MAXGUILDS];

	char m_cHeldenianMapName[11];
	int m_iLastCrusadeWinner; // New 13/05/2004

	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	// �̵鷣���� ������ �����ϴ� ����ü. �̵鷣�带 ������� �ʴ� ���������� �� ������ �޾� �����Ѵ�.

	struct {
		char cType; // �̰� NULL�̸� ���ǵ��� �������� �ǹ�.
		char cSide; // ���̵�
		short sX, sY; // ��ġ�� ��ġ 
	} m_stMiddleCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];

	struct {
		char m_cBannedIPaddress[21]; // Ŭ���̾�Ʈ�� IP address
	} m_stBannedList[DEF_MAXBANNED];

	struct {
		char m_cGMName[11];
	} m_stAdminList[DEF_MAXADMINS];

	// Crusade Scheduler

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

	// �� ���� �ȿ��� ���� ������ Ŭ���̾�Ʈ�� �ε����� ������ ����Ǿ� �ִ�. 0�� ������ ����Ʈ�� ���̶�� �ǹ�
	int m_iClientShortCut[DEF_MAXCLIENTS + 1];

	int m_iNpcConstructionPoint[DEF_MAXNPCTYPES];
	DWORD m_dwCrusadeGUID;
	short m_sLastCrusadeDate;
	int m_iCrusadeWinnerSide;
	int m_iPlayerMaxLevel;

	struct {
		int iTotalMembers;
		int iIndex[9];
	} m_stPartyInfo[DEF_MAXCLIENTS];

	// Daryl - Admin level adjustments
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

	// 09/26/2004
	short m_sSlateSuccessRate;

	// 17/05/2004
	short m_sForceRecallTime;

	// 22/05/2004
	int m_iPrimaryDropRate, m_iSecondaryDropRate;



	// 25/05/2004
	int m_iFinalShutdownCount;

	// New 06/07/2004
	BOOL m_bEnemyKillMode;
	int m_iEnemyKillAdjust;
	BOOL m_bAdminSecurity;

	// Configurable Raid Time 
	short m_sRaidTimeMonday;
	short m_sRaidTimeTuesday;
	short m_sRaidTimeWednesday;
	short m_sRaidTimeThursday;
	short m_sRaidTimeFriday;
	short m_sRaidTimeSaturday;
	short m_sRaidTimeSunday;

	BOOL m_bManualTime;
	int m_iSummonGuildCost;

	//Heldenian
	BOOL m_bIsHeldenianMode;
	BOOL m_bIsHeldenianTeleport;
	//char	m_cHeldenianType;

	// Slate exploit
	int m_sCharPointLimit;

	// Limit Checks
	short m_sCharStatLimit;
	BOOL m_bAllow100AllSkill;
	short m_sCharSkillLimit;
	char m_cRepDropModifier;
	char m_cSecurityNumber[11];
	short m_sMaxPlayerLevel;

	BOOL var_89C, var_8A0;
	char m_cHeldenianVictoryType, m_sLastHeldenianWinner, m_cHeldenianModeType;
	short m_iHeldenianAresdenDead, m_iHeldenianElvineDead;
	short m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower;
	DWORD m_dwHeldenianGUID, m_dwHeldenianStartHour, m_dwHeldenianStartMinute;
	int m_dwHeldenianStartTime, m_dwHeldenianFinishTime;
	BOOL m_bReceivedItemList;
	BOOL m_bHeldenianInitiated;
	BOOL m_bHeldenianRunning;
	// HBX 3.00 Heldenian Source
	BOOL m_bHeldinianDuty;
	BOOL m_bHeldenianDutyMultiplyer;
	BOOL m_bIsHeldenianReady;
	BOOL m_bIsHeldenianScheduleLoaded;
	BOOL m_bIsHeldenianSchedule;
	BOOL bReadBallSystemConfigFile(const char * cFn);

private:
	int __iSearchForQuest(int iClientH, int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_Cityhall(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void _ClearExchangeStatus(int iToH);
	int _iGetItemSpaceLeft(int iClientH);

public:
	void AFKChecker();
	void AdminOrder_GoTo(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_MonsterCount(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetForceRecallTime(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_UnsummonBoss(int iClientH);
	void RemoveCrusadeNpcs(void);
	void RemoveCrusadeRecallTime(void);
	BOOL _bCrusadeLog(int iAction, int iClientH, int iData, char * cName);
	int iGetPlayerABSStatus(int iClientH);
	BOOL _bInitItemAttr(class CItem * pItem, int iItemID);
	void ReqCreateSlateHandler(int iClientH, char* pData);
	void SetSlateFlag(int iClientH, short sType, bool bFlag);
	void CheckForceRecallTime(int iClientH);
	void SetPlayingStatus(int iClientH);
	void ForceChangePlayMode(int iClientH, bool bNotify);
	void ShowVersion(int iClientH);
	void ShowClientMsg(int iClientH, char* pMsg);
	void RequestResurrectPlayer(int iClientH, bool bResurrect);
	void GetDkSet(int iClientH);
	void AdminOrder_BanIP(int iClientH, char *pData, DWORD dwMsgSize);
	void PlayerOrder_ShowDamage(int iClientH);
	void PlayerOrder_GetCrits(int iClientH);
	void RequestRango(int iClientH, int iObjectID); // Morla2.2 - Pide EK y PK
	void GetTradeEKMantleHandler(int iClientH, int iItemID, char * pString); // By Luqah
	void PlayerCommandAddMaster(int iClientH, char * pData, DWORD dwMsgSize);
	void PlayerOrder_DeleteGuildMaster(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SetCrit(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_SetEK(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_SetMAJ(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_SetRep(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_SetPK(int iClientH, char *pData, DWORD dwMsgSize);
	void RequestBallItem(int iClientH, char *pData, DWORD dwMsgSize);
	void _TradeItem(int iClientH, char *pData, DWORD dwMsgSize);
#define DEF_PLAYER_MAX_EXPERIENCE 203484288 // juan249 - Exp negativa fix.y reemplazan los numeros 121586796 por la exp de un personaje max lvl de su servidor.
	void PURITYLIFE(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void DesbugCharacter(int iClientH, char * pData, DWORD dwMsgSize);
	void CheckDenialServiceAttack(int iClientH, DWORD dwClientTime);

	//HappyHour para vos
	BOOL m_bHappyHour;

	DWORD dwHappyHourTime;

	void EndHappyHour();
	void StarHappyHour();
	void HappyHuor();
	void ManualStartHappyHour(int iClientH, char *pData, DWORD dwMsgSize);
	void closeHappyHour();
	void openHappyHour();

	//50Cent - Capture The Flag
	void StartCaptureTheFlag();
	void RequestCheckFlag(int iClientH);
	BOOL bCheckIfIsFlagCarrier(int iClientH);
	void SetFlagCarrierFlag(int iClientH, BOOL bFlagMode);
	void EndCaptureTheFlag(int iOption);
	void _CreateCTFGUID(int iWinnerSide);

	//50Cent - Capture The Flag
	BOOL m_bIsCTFMode;
	short m_sFlagCountWin;
	short m_sElvineFlagCount;
	short m_sAresdenFlagCount;
	BOOL m_bIsElvineFlagCaptured;
	BOOL m_bIsAresdenFlagCaptured;
	int m_iFlagCarrierIndex;

	//50Cent - 100% Skill
	void PlayerOrder_GetSkills(int iClientH);

	short iH;

};