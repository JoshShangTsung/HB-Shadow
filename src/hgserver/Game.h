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

#define DEF_EXPSTOCKTIME		1000*10		
#define DEF_MSGQUENESIZE		100000
#define DEF_AUTOEXPTIME			1000*60*6
#define DEF_TOTALLEVELUPPOINT	3


#define DEF_MAXDYNAMICOBJECTS	60000
#define DEF_MAXDELAYEVENTS		60000
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

#define DEF_MAXGUILDS					1000
#define DEF_MAXONESERVERUSERS			800

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

#define DEF_MAXFIGHTZONE 10 

//============================
#define DEF_LEVELLIMIT		20						
//============================

//============================
#define DEF_MINIMUMHITRATIO 15				
//============================		

//============================
#define DEF_MAXIMUMHITRATIO	99			
//============================

//============================
#define DEF_PLAYERMAXLEVEL	100			
//============================

//============================
// New Changed 12/05/2004
#define DEF_GMGMANACONSUMEUNIT	15			
//============================

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

class CGame {
public:

	int iUpgradeHeroCapeRequirements(int iClientH, int iItemIndex);
	ItemId HeroItemChecker(ItemId iItemID, short sEnemyKill, char cContribution, char cSide);
	void AdminOrder_GetFightzoneTicket(int iClientH);

	void GetAngelHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	void SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp);

	void ReadMajesticSettings();
	void ReqCreateCraftingHandler(int iClientH, char *pData);
	//Heldenian 3.00 Source Code
	void HeldenianWinner();
	void RequestHeldenianScroll(int iClientH, char * pData, uint32_t dwMsgSize);
	void CheckHeldenianResultCalculation(int iClientH);
	void AutomaticHeldenianEnd();
	void SetHeldenianMode();
	void AutomatedHeldenianTimer();
	void LocalStartHeldenianMode(short sV1, short sV2, uint32_t dwHeldenianGUID);
	void GlobalStartHeldenianMode();
	void HeldenianWarEnder();
	void HeldenianWarStarter();
	bool UpdateHeldenianStatus();
	void _CreateHeldenianGUID(uint32_t dwHeldenianGUID, int iWinnerSide);
	void ManualStartHeldenianMode(int iClientH, char *pData, uint32_t dwMsgSize);
	void ManualEndHeldenianMode(int iClientH, char *pData, uint32_t dwMsgSize);
	void NotifyStartHeldenianFightMode();
	void GlobalEndHeldenianMode();
	void LocalEndHeldenianMode();
	bool bNotifyHeldenianWinner();
	void RequestHeldenianTeleport(int iClientH, char * pData, uint32_t dwMsgSize);
	void RequestNpcSpeakTeleport(int iClientH, char * pData, uint32_t dwMsgSize);
	bool bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType);
	bool bReadHeldenianGUIDFile(const char * cFn);
	void RemoveEventNpc(int iNpcH);

	bool _bCheckCharacterData(int iClientH);
	//bool _bDecodeNpcItemConfigFileContents(char * pData, uint32_t dwMsgSize);
	void GlobalUpdateConfigs(char cConfigType);
	void LocalUpdateConfigs(char cConfigType);
	void RemoveOccupyFlags(int iMapIndex);
	void SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void CalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, char cAttackerType, int iArmorType);
	char _cCheckHeroItemEquipped(int iClientH);
	bool bPlantSeedBag(int iMapIndex, int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH);
	void _CheckFarmingAction(short sAttackerH, short sTargetH, bool bType);

	void GreenBall_Weather(int iClientH, char * pData, uint32_t dwMsgSize);
	bool bReadScheduleConfigFile(const char *pFn);

	// KLKS clean tiles
	void AdminOrder_CleanMap(int iClientH, char * pData, uint32_t dwMsgSize);

	// Lists
	bool bReadBannedListConfigFile(const char *pFn);
	bool bReadAdminListConfigFile(const char *pFn);

	void AdminOrder_CheckStats(int iClientH, char *pData, uint32_t dwMsgSize);
	void Command_RedBall(int iClientH, char *pData, uint32_t dwMsgSize);
	void Command_BlueBall(int iClientH, char *pData, uint32_t dwMsgSize);
	void Command_YellowBall(int iClientH, char* pData, uint32_t dwMsgSize);

	// Crusade
	void ManualEndCrusadeMode(int iWinnerSide); // 2.17 (x) 2.14 ( )
	void CrusadeWarStarter();
	bool bReadCrusadeGUIDFile(const char * cFn);
	void _CreateCrusadeGUID(uint32_t dwCrusadeGUID, int iWinnerSide);
	void GlobalStartCrusadeMode();

	// Apocalypse
	void GlobalStartApocalypseMode();
	void GlobalEndApocalypseMode();
	void LocalStartApocalypseMode(uint32_t dwApocalypseGUID);
	void LocalEndApocalypseMode();
	void _CreateApocalypseGUID(uint32_t dwApocalypseGUID);
	bool bReadApocalypseGUIDFile(const char * cFn);
	void SendThunder(int iClient, short sX, short sY, short sV3, short sV4);
	void DoAbaddonThunderDamageHandler(char cMapIndex);

	void GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char * pMapName);
	void SyncMiddlelandMapInfo();
	void RemoveCrusadeStructures();
	void _SendMapStatus(int iClientH);
	void MapStatusHandler(int iClientH, int iMode, char * pMapName);
	void SelectCrusadeDutyHandler(int iClientH, int iDuty);
	void RequestSummonWarUnitHandler(int iClientH, int dX, int dY, short cType, char cNum, char cMode);
	void RequestGuildTeleportHandler(int iClientH);
	void RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, const char * pMapName);
	void MeteorStrikeHandler(int iMapIndex);
	void _LinkStrikePointMapIndex();
	void MeteorStrikeMsgHandler(char cAttackerSide);
	void _NpcBehavior_GrandMagicGenerator(int iNpcH);
	void CollectedManaHandler(uint16_t wAresdenMana, uint16_t wElvineMana);
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
	bool bReadCrusadeStructureConfigFile(const char * cFn);
	void SaveOccupyFlagData();
	void LocalEndCrusadeMode(int iWinnerSide);
	void LocalStartCrusadeMode(uint32_t dwGuildGUID);
	void CheckCrusadeResultCalculation(int iClientH);
	bool _bNpcBehavior_Detector(int iNpcH);
	bool _bNpcBehavior_ManaCollector(int iNpcH);
	bool __bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH);

	//void AdminOrder_SummonGuild(int iClientH, char * pData, uint32_t dwMsgSize);

	// Acidx commands
	void AdminOrder_Time(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_CheckRep(int iClientH, char *pData);
	void AdminOrder_Pushplayer(int iClientH, char * pData, uint32_t dwMsgSize);

	void AdminOrder_CheckRep(int iClientH, char *pData, uint32_t dwMsgSize);

	void SetForceRecallTime(int iClientH);
	void ApplyCombatKilledPenalty(int iClientH, int cPenaltyLevel, bool bIsSAattacked);

	//	void AdminOrder_ClearTiles(int iClientH);
	void AdminOrder_ClearNpc(int iClientH);

	// Settings.cfg
	bool bReadSettingsConfigFile(const char * cFn);

	//  bool bReadTeleportConfigFile(char * cFn);
	//	void RequestTeleportD2Handler(int iClientH, char * pData);

	// Daryl - AdminSettings.cfg
	bool bReadAdminSetConfigFile(const char * cFn);


	// Hack Checks
	bool bCheckClientMoveFrequency(int iClientH, uint32_t dwClientTime);
	bool bCheckClientMagicFrequency(int iClientH, uint32_t dwClientTime);
	bool bCheckClientAttackFrequency(int iClientH, uint32_t dwClientTime);

	// bool bCheckClientInvisibility(short iClientH);

	//Hypnotoad functions
	void SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus);

	void RequestChangePlayMode(int iClientH);
	void GetHeroMantleHandler(int iClientH, ItemId iItemID, char * pString);
	void AdminOrder_Weather(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SendMSG(int iClientH, char *pData, uint32_t dwMsgSize);
	void SendMsg(short sOwnerH, char cOwnerType, bool bStatus, long lPass);
	bool bCheckMagicInt(int iClientH);
	bool bChangeState(char cStateChange, char* cStr, char *cVit, char *cDex, char *cInt, char *cMag, char *cChar);
	void StateChangeHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetStatus(int iClientH, char *pData, uint32_t dwMsgSize);
	void SetStatusFlag(short sOwnerH, char cOwnerType, bool bStatus, int iPass);
	void SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void AdminOrder_SummonStorm(int iClientH, char* pData, uint32_t dwMsgSize);
	void AdminOrder_CallMagic(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonDeath(int iClientH);
	void AdminOrder_SetZerk(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetFreeze(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_Kill(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_Revive(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetObserverMode(int iClientH);
	void AdminOrder_EnableAdminCommand(int iClientH, char *pData, uint32_t dwMsgSize);
	void AdminOrder_CreateItem(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_Summon(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonAll(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonPlayer(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_UnsummonDemon(int iClientH);
	void AdminOrder_UnsummonAll(int iClientH);
	void AdminOrder_SetAttackMode(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonDemon(int iClientH);
	void AdminOrder_SetInvi(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_Polymorph(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_GetNpcStatus(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_CheckIP(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_CreateFish(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_Teleport(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_ReserveFightzone(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_CloseConn(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_CallGuard(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_DisconnectAll(int iClientH, char * pData, uint32_t dwMsgSize);

	bool bCopyItemContents(class CItem * pOriginal, class CItem * pCopy);
	int iGetMapLocationSide(char * pMapName);
	void ChatMsgHandlerGSM(int iMsgType, int iV1, char * pName, char * pData, uint32_t dwMsgSize);
	void RemoveClientShortCut(int iClientH);
	bool bAddClientShortCut(int iClientH);

	void GSM_RequestFindCharacter(uint16_t wReqServerID, uint16_t wReqClientH, char *pName, char * pFinder); // New 16/05/2001 Changed
	void ServerStockMsgHandler(char * pData);
	void RequestHelpHandler(int iClientH);

	void CheckConnectionHandler(int iClientH, char *pData);

	void AgingMapSectorInfo();
	void UpdateMapSectorInfo();
	//bool bGetItemNameWhenDeleteNpc(char * pItemName, short sNpcType);
	bool bGetItemNameWhenDeleteNpc(ItemId & iItemID, NpcType sNpcType);
	int iGetItemWeight(class CItem * pItem, int iCount);
	void CancelQuestHandler(int iClientH);
	void ActivateSpecialAbilityHandler(int iClientH);
	void EnergySphereProcessor(bool bIsAdminCreate = false, int iClientH = 0);
	bool bCheckEnergySphereDestination(int iNpcH, short sAttackerH, char cAttackerType);
	void JoinPartyHandler(int iClientH, int iV1, char *pMemberName);
	void CreateNewPartyHandler(int iClientH);
	void _DeleteRandomOccupyFlag(int iMapIndex);
	void RequestSellItemListHandler(int iClientH, char * pData);
	void RequestRestartHandler(int iClientH);
	int iRequestPanningMapDataRequest(int iClientH, char * pData);
	void GetMagicAbilityHandler(int iClientH);
	void Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void _TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY);
	void RequestCheckAccountPasswordHandler(char * pData, uint32_t dwMsgSize);
	int _iTalkToNpcResult_Guard(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, NpcType * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus);
	void _bDecodeNoticementFileContents(char * pData, uint32_t dwMsgSize);
	void RequestNoticementHandler(int iClientH, char * pData);
	void _AdjustRareItemValue(class CItem * pItem);
	bool _bCheckDupItemID(class CItem * pItem);
	bool _bDecodeDupItemIDFileContents(char * pData, uint32_t dwMsgSize);
	void NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType);
	int iGetPlayerABSStatus(int iWhatH, int iRecvH);
	void CheckSpecialEvent(int iClientH);
	char _cGetSpecialAbility(int iKindSA);
	void BuildItemHandler(int iClientH, char * pData);
	bool _bDecodeBuildItemConfigFileContents(char * pData, uint32_t dwMsgSize);
	bool _bCheckSubLogSocketIndex();
	void OnSubLogRead(int iIndex);
	void OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void _CheckStrategicPointOccupyStatus(char cMapIndex);
	void GetMapInitialPoint(int iMapIndex, short * pX, short * pY, char * pPlayerLocation = 0);
	int iGetMaxHP(int iClientH);
	int iGetMaxMP(int iClientH);
	int iGetMaxSP(int iClientH);
	void _ClearQuestStatus(int iClientH);
	bool _bCheckItemReceiveCondition(int iClientH, class CItem * pItem);
	void SendItemNotifyMsg(int iClientH, uint16_t wMsgType, class CItem * pItem, int iV1);

	int _iTalkToNpcResult_WTower(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_WHouse(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_BSmith(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_GShop(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_GuildHall(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	bool _bCheckIsQuestCompleted(int iClientH);
	void _CheckQuestEnvironment(int iClientH);
	void _SendQuestContents(int iClientH);
	void QuestAcceptedHandler(int iClientH);
	bool _bDecodeQuestConfigFileContents(char * pData, uint32_t dwMsgSize);

	void CancelExchangeItem(int iClientH);
	bool bAddItem(int iClientH, class CItem * pItem, char cMode);
	void ConfirmExchangeItem(int iClientH);
	void SetExchangeItem(int iClientH, int iItemIndex, int iAmount);
	void ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName);

	void _BWM_Command_Shutup(char * pData);
	void _BWM_Init(int iClientH, char * pData);
	void CheckUniqueItemEquipment(int iClientH);
	void _SetItemPos(int iClientH, char * pData);
	//void GetHeroMantleHandler(int iClientH);

	bool _bDecodeOccupyFlagSaveFileContents(char * pData, uint32_t dwMsgSize);
	void GetOccupyFlagHandler(int iClientH);
	int _iComposeFlagStatusContents(char * pData);
	void SetSummonMobAction(int iClientH, int iMode, uint32_t dwMsgSize, char * pData = 0);
	bool __bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, bool bAdminFlag);
	bool _bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID);
	void SetDownSkillIndexHandler(int iClientH, int iSkillIndex);
	int iGetComboAttackBonus(int iSkill, int iComboCount);
	int _iGetWeaponSkillType(int iClientH);
	void CheckFireBluring(char cMapIndex, int sX, int sY);
	void NpcTalkHandler(int iClientH, int iWho);
	bool bDeleteMineral(int iIndex);
	void _CheckMiningAction(int iClientH, int dX, int dY);
	int iCreateMineral(char cMapIndex, int tX, int tY, char cLevel);
	void MineralGenerator();
	void LocalSavePlayerData(int iClientH);
	bool _bDecodePortionConfigFileContents(char * pData, uint32_t dwMsgSize);
	void ReqCreatePortionHandler(int iClientH, char * pData);
	void _CheckAttackType(int iClientH, short * spType);
	bool bOnClose();
	void ForceDisconnectAccount(char * pAccountName, uint16_t wCount);
	void NpcRequestAssistance(int iNpcH);
	void ToggleSafeAttackModeHandler(int iClientH);
	void SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus);
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
	bool bDeleteFish(int iHandle, int iDelMode);
	int iCreateFish(char cMapIndex, short sX, short sY, short sDifficulty, class CItem * pItem, int iDifficulty, uint32_t dwLastTime);
	void UserCommand_DissmissGuild(int iClientH, char * pData, uint32_t dwMsgSize);
	void UserCommand_BanGuildsman(int iClientH, char * pData, uint32_t dwMsgSize);
	int iGetExpLevel(int iExp);
	void ___RestorePlayerRating(int iClientH);
	void CalcExpStock(int iClientH);
	void ResponseSavePlayerDataReplyHandler(char * pData, uint32_t dwMsgSize);
	void NoticeHandler();
	bool bReadNotifyMsgListFile(const char * cFn);
	void SetPlayerReputation(int iClientH, char * pMsg, char cValue, uint32_t dwMsgSize);
	void ShutUpPlayer(int iClientH, char * pMsg, uint32_t dwMsgSize);
	void CheckDayOrNightMode();
	bool bCheckBadWord(char * pString);
	bool bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType);
	void PoisonEffect(int iClientH, int iV1);
	void bSetNpcAttackMode(char * cName, int iTargetH, char cTargetType, bool bIsPermAttack);
	bool _bGetIsPlayerHostile(int iClientH, int sOwnerH);
	bool bAnalyzeCriminalAction(int iClientH, short dX, short dY, bool bIsCheck = false);
	void RequestAdminUserMode(int iClientH, char * pData);
	int _iGetPlayerNumberOnSpot(short dX, short dY, char cMapIndex, char cRange);
	void CalcTotalItemEffect(int iClientH, int iEquipItemID, bool bNotify = true);
	void ___RestorePlayerCharacteristics(int iClientH);
	void GetPlayerProfile(int iClientH, char * pMsg, uint32_t dwMsgSize);
	void SetPlayerProfile(int iClientH, char * pMsg, uint32_t dwMsgSize);
	void ToggleWhisperPlayer(int iClientH, char * pMsg, uint32_t dwMsgSize);
	void CheckAndNotifyPlayerConnection(int iClientH, char * pMsg, uint32_t dwSize);
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
	void SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus);
	bool bRemoveFromDelayEventList(int iH, char cType, int iEffectType);
	void DelayEventProcessor();
	bool bRegisterDelayEvent(int iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	int iGetFollowerNumber(short sOwnerH, char cOwnerType);
	int _iCalcSkillSSNpoint(int iLevel);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	bool bCheckTotalSkillMasteryPoints(int iClientH, int iSkill);
	bool bSetItemToBankItem(int iClientH, class CItem * pItem);
	void NpcMagicHandler(int iNpcH, short dX, short dY, short sType);
	bool bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	bool bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	void Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, bool bExp, int iAttr = 0);
	void Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID);
	void NpcBehavior_Stop(int iNpcH);
	void ItemDepleteHandler(int iClientH, short sItemIndex, bool bIsUseItemResult, bool bIsItemUsed);
	int _iGetArrowItemIndex(int iClientH);
	void RequestFullObjectData(int iClientH, char * pData);
	void DeleteNpc(int iNpcH);
	void CalcNextWayPointDestination(int iNpcH);
	void MobGenerator();
	void CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue);
	void CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue);
	void CheckDynamicObjectList();
	int iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwLastTime, int iV1 = 0);
	int _iCalcMaxLoad(int iClientH);
	void GetRewardMoneyHandler(int iClientH);
	void _PenaltyItemDrop(int iClientH, int iTotal, bool bIsSAattacked = false);
	//void ApplyCombatKilledPenalty(int iClientH, char cPenaltyLevel, bool bIsSAattacked = false);
	void EnemyKillRewardHandler(int iAttackerH, int iClientH);
	void PK_KillRewardHandler(short sAttackerH, short sVictumH);
	void ApplyPKpenalty(short sAttackerH, short sVictumH);
	bool bSetItemToBankItem(int iClientH, short sItemIndex);
	void RequestRetrieveItemHandler(int iClientH, char *pData);
	void RequestCivilRightHandler(int iClientH, char * pData);
	bool bCheckLimitedUser(int iClientH);
	void LevelUpSettingsHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	void FightzoneReserveHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	bool bCheckLevelUp(int iClientH);
	int iGetLevelExp(int iLevel);
	void TimeManaPointsUp(int iClientH);
	void TimeStaminarPointsUp(int iClientH);
	void Quit();
	bool __bReadMapInfo(int iMapIndex);
	bool bBankItemToPlayer(int iClientH, short sItemIndex);
	bool bPlayerItemToBank(int iClientH, short sItemIndex);
	int _iGetSkillNumber(char * pSkillName);
	void TrainSkillResponse(bool bSuccess, int iClientH, int iSkillNum, int iSkillLevel);
	int _iGetMagicNumber(char * pMagicName, int * pReqInt, int * pCost);
	void RequestStudyMagicHandler(int iClientH, char * pName, bool bIsPurchase = true);
	bool _bDecodeSkillConfigFileContents(char * pData, uint32_t dwMsgSize);
	bool _bDecodeMagicConfigFileContents(char * pData, uint32_t dwMsgSize);
	void ReleaseFollowMode(short sOwnerH, char cOwnerType);
	bool bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType);
	void RequestTeleportHandler(int iClientH, const char * pData, const char * cMapName = 0, int dX = -1, int dY = -1);
	void PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect = false, int iV1 = 0);
	int iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir);
	void ToggleCombatModeHandler(int iClientH);
	void GuildNotifyHandler(char * pData, uint32_t dwMsgSize);
	void SendGuildMsg(int iClientH, uint16_t wNotifyMsgType, short sV1, short sV2, char * pString);
	void DelayEventProcess();
	void TimeHitPointsUp(int iClientH);
	void CalculateGuildEffect(int iVictimH, char cVictimType, short sAttackerH);
	void OnStartGameSignal();
	int iDice(int iThrow, int iRange);
	bool _bInitNpcAttr(class CNpc * pNpc, const char * pNpcName, short sClass, char cSA);
	bool _bDecodeNpcConfigFileContents(char * pData, uint32_t dwMsgSize);
	void ReleaseItemHandler(int iClientH, short sItemIndex, bool bNotice);
	void ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage);
	int SetItemCount(int iClientH, const char * pItemName, uint32_t dwCount);
	int SetItemCount(int iClientH, int iItemIndex, uint32_t dwCount);
	uint32_t dwGetItemCount(int iClientH, const char * pName);
	void DismissGuildRejectHandler(int iClientH, char * pName);
	void DismissGuildApproveHandler(int iClientH, char * pName);
	void JoinGuildRejectHandler(int iClientH, char * pName);
	void JoinGuildApproveHandler(int iClientH, char * pName);
	void SendNotifyMsg(int iFromH, int iToH, uint16_t wMsgType, uint32_t sV1, uint32_t sV2, uint32_t sV3, const char * pString, uint32_t sV4 = 0, uint32_t sV5 = 0, uint32_t sV6 = 0, uint32_t sV7 = 0, uint32_t sV8 = 0, uint32_t sV9 = 0, char * pString2 = 0);
	void GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName);
	void RequestPurchaseItemHandler(int iClientH, char * pItemName, int iNum);
	void ResponseDisbandGuildHandler(char * pData, uint32_t dwMsgSize);
	void RequestDisbandGuildHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	void RequestCreateNewGuildHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	void ResponseCreateNewGuildHandler(char * pData, uint32_t dwMsgSize);
	int iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir);

	bool bEquipItemHandler(int iClientH, short sItemIndex, bool bNotify = true);
	bool _bAddClientItemList(int iClientH, class CItem * pItem, int * pDelReq);
	int iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir);
	void DropItemHandler(int iClientH, short sItemIndex, int iAmount, char * pItemName, bool bByPlayer = true);
	void ClientCommonHandler(int iClientH, char * pData);
	bool __fastcall bGetMsgQuene(char * pFrom, char * pData, uint32_t * pMsgSize, int * pIndex, char * pKey);
	void MsgProcess();
	bool __fastcall bPutMsgQuene(char cFrom, char * pData, uint32_t dwMsgSize, int iIndex, char cKey);
	void NpcBehavior_Flee(int iNpcH);
	int iGetDangerValue(int iNpcH, short dX, short dY);
	void NpcBehavior_Dead(int iNpcH);
	void NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage);
	//int  iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, bool bNearAttack = false);
	int iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, bool bNearAttack = false, bool bIsDash = false, bool bArrowUse = false);
	void RemoveFromTarget(short sTargetH, char cTargetType, int iCode = 0);
	void NpcBehavior_Attack(int iNpcH);
	void TargetSearch(int iNpcH, short * pTarget, char * pTargetType);
	void NpcBehavior_Move(int iNpcH);
	bool bGetEmptyPosition(short * pX, short * pY, char cMapIndex);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int * pError);
	int iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse = true, bool bIsDash = false);
	void ChatMsgHandler(int iClientH, char * pData, uint32_t dwMsgSize);
	void NpcProcess();
	int bCreateNewNpc(const char * pNpcName, char * pName, const char * pMapName, short sClass, char cSA, NpcMoveType cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned = false, bool bFirmBerserk = false, bool bIsMaster = false, int iGuildGUID = 0);
	//bool bCreateNewNpc(char * pNpcName, char * pName, char * pMapName, short sX, short sY);
	bool _bReadMapInfoFiles(int iMapIndex);

	bool _bGetIsStringIsNumber(char * pStr);
	bool _bInitItemAttr(class CItem * pItem, const char * pItemName);
	bool bReadProgramConfigFile(const char * cFn);
	void GameProcess();
	void InitPlayerData(int iClientH, char * pData, uint32_t dwSize);
	void ResponsePlayerDataHandler(char * pData, uint32_t dwSize);
	bool bSendMsgToLS(uint32_t dwMsg, int iClientH, bool bFlag = true, char *pData = 0);
	void OnMainLogRead();
	void OnMainLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void CheckClientResponseTime();
	void OnTimer(char cType);
	int iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char * pData);
	void SendEventToNearClient_TypeB(uint32_t dwMsgID, uint16_t wMsgType, char cMapIndex, short sX, short sY, short sV1, short sV2, short sV3, short sV4 = 0);
	void SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3);
	void DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout = true, bool bForceCloseConn = false);
	int iComposeInitMapData(short sX, short sY, int iClientH, char * pData);
	void RequestInitDataHandler(int iClientH, char * pData, char cKey);
	void RequestInitPlayerHandler(int iClientH, char * pData, char cKey);
	int iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType);
	void ClientMotionHandler(int iClientH, char * pData);
	void DisplayInfo(HDC hdc);
	void OnClientRead(int iClientH);
	bool bInit();
	void OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool bAccept(class XSocket * pXSock);
	void GetFightzoneTicketHandler(int iClientH);
	void FightzoneReserveProcessor();

	// New 06/05/2004
	// Upgrades
	bool bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, bool bBonus = false);
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
	void RequestJoinPartyHandler(int iClientH, char *pData, uint32_t dwMsgSize);
	void RequestDismissPartyHandler(int iClientH);
	void GetPartyInfoHandler(int iClientH);
	void PartyOperationResult_Info(int iClientH, char * pName, int iTotal, char *pNameList);
	void RequestDeletePartyHandler(int iClientH);
	void RequestAcceptJoinPartyHandler(int iClientH, int iResult);
	void GetExp(int iClientH, int iExp, bool bIsAttackerOwn = false);

	// New 07/05/2004
	// Guild Codes
	void RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex);

	// Item Logs
	bool _bItemLog(int iAction, int iClientH, char * cName, class CItem * pItem);
	bool _bItemLog(int iAction, int iGiveH, int iRecvH, class CItem * pItem, bool bForceItemLog = false);
	bool _bCheckGoodItem(class CItem * pItem);

	bool bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex);
	void ArmorLifeDecrement(int iAttackerH, int iTargetH, char cOwnerType, int iValue);

	// MultiDrops
	bool bGetMultipleItemNamesWhenDeleteNpc(NpcType sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY, int iItemSpreadType, int iSpreadRange, ItemId *iItemIDs, POINT *BasePos, int *iNumItem);

	// Majestic Code By Diuuude
	int m_iMajesticPointsPerLevel;
	int m_iMajesticMinLevel;
	int m_iMajesticPointsMax;


	// Player shutup
	void GSM_RequestShutupPlayer(char * pGMName, uint16_t wReqServerID, uint16_t wReqClientH, uint16_t wTime, char * pPlayer);

	// PK Logs
	bool _bPKLog(int iAction, int iAttackerH, int iVictumH, char * pNPC);

	CGame(HWND hWnd);
	virtual ~CGame();

	char m_cServerName[11]{};
	char m_cGameServerAddr[16];
	char m_cGameServerAddrInternal[16];
	char m_cGameServerAddrExternal[16];
	int m_iGameServerMode;
	char m_cLogServerAddr[16];
	int m_iGameServerPort;
	int m_iLogServerPort;

	int m_iLimitedUserExp;

	int m_iLevelExp20;

	bool _bDecodeItemConfigFileContents(char * pData, uint32_t dwMsgSize);
	int _iComposePlayerDataFileContents(int iClientH, char * pData);
	bool _bDecodePlayerDatafileContents(int iClientH, char * pData, uint32_t dwSize);
	bool _bRegisterMap(char * pName);

	class CClient * m_pClientList[DEF_MAXCLIENTS]{};
	class CNpc * m_pNpcList[DEF_MAXNPCS]{};
	class CMap * m_pMapList[DEF_MAXMAPS]{};
	class CDynamicObject * m_pDynamicObjectList[DEF_MAXDYNAMICOBJECTS]{};
	class CDelayEvent * m_pDelayEventList[DEF_MAXDELAYEVENTS]{};
	class CBallSystem * m_pBallItemConfigList[DEF_MAXBALLITEMS];

	class CMsg * m_pMsgQuene[DEF_MSGQUENESIZE]{};
	int m_iQueneHead = 0;
	int m_iQueneTail = 0;
	int m_iTotalMaps = 0;
	class XSocket * m_pMainLogSock = nullptr;
	class CMisc m_Misc;
	bool m_bIsGameStarted = false;
	bool m_bIsLogSockAvailable = false;
	bool m_bIsItemAvailable = false;
	bool m_bIsBuildItemAvailable = false;
	bool m_bIsNpcAvailable = false;
	bool m_bIsMagicAvailable = false;
	bool m_bIsSkillAvailable = false;
	bool m_bIsPortionAvailable = false;
	bool m_bIsQuestAvailable = false;
	bool m_bIsTeleportAvailable;
	class CItem * m_pItemConfigList[DEF_MAXITEMTYPES]{};
	class CNpc * m_pNpcConfigList[DEF_MAXNPCTYPES]{};
	class CMagic * m_pMagicConfigList[DEF_MAXMAGICTYPE]{};
	class CSkill * m_pSkillConfigList[DEF_MAXSKILLTYPE]{};
	class CQuest * m_pQuestConfigList[DEF_MAXQUESTTYPE]{};

	class CPortion * m_pCraftingConfigList[DEF_MAXPORTIONTYPES]{};

	char m_pMsgBuffer[DEF_MSGBUFFERSIZE + 1];

	HWND m_hWnd = 0;
	int m_iTotalClients = 0;
	int m_iMaxClients = 0;
	int m_iTotalBots;
	int m_iMaxBots;
	int m_iTotalGameServerBots;
	int m_iTotalGameServerMaxBots;
	SYSTEMTIME m_MaxUserSysTime{0,0,0,0,0,0,0,0};

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
	uint32_t m_dwFishTime; //Orkl - added 7 for advertiser	
	// Crusade Schedule
	bool m_bIsCrusadeWarStarter;
	int m_iLatestCrusadeDayOfWeek;

	char m_cDayOrNight;
	int m_iSkillSSNpoint[102];

	class CMsg * m_pNoticeMsgList[DEF_MAXNOTIFYMSGS]{};
	int m_iTotalNoticeMsg;
	int m_iPrevSendNoticeMsg;
	uint32_t m_dwNoticeTime;
	uint32_t m_dwSpecialEventTime;
	bool m_bIsSpecialEventTime;
	char m_cSpecialEventType;

	int m_iLevelExpTable[3500]; //New 22/10/04

	class CFish * m_pFish[DEF_MAXFISHS]{};
	class CPortion * m_pPortionConfigList[DEF_MAXPORTIONTYPES]{};

	bool m_bIsServerShutdowned = false;
	char m_cShutDownCode = 0;
	class CMineral * m_pMineral[DEF_MAXMINERALS]{};

	int m_iMiddlelandMapIndex = -1; 
	int m_iAresdenMapIndex;
	int m_iElvineMapIndex;
	int m_iBTFieldMapIndex;
	int m_iGodHMapIndex;
	int m_iAresdenOccupyTiles = 0;
	int m_iElvineOccupyTiles = 0;
	int m_iCurMsgs = 0;
	int m_iMaxMsgs = 0;

	uint32_t m_dwCanFightzoneReserveTime;

	int m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	int m_iFightzoneNoForceRecall;

	struct {
		__int64 iFunds = 0;
		__int64 iCrimes = 0;
		__int64 iWins = 0;

	} m_stCityStatus[3]{};

	int m_iStrategicStatus;

	class XSocket * m_pSubLogSock[DEF_MAXSUBLOGSOCK]{};
	int m_iSubLogSockInitIndex;
	bool m_bIsSubLogSockAvailable[DEF_MAXSUBLOGSOCK]{};
	int m_iCurSubLogSockIndex;
	int m_iSubLogSockFailCount;
	int m_iSubLogSockActiveCount;
	int m_iAutoRebootingCount = 0;

	class CBuildItem * m_pBuildItemList[DEF_MAXBUILDITEMS]{};
	class CItem * m_pDupItemIDList[DEF_MAXDUPITEMID]{};

	char * m_pNoticementData;
	uint32_t m_dwNoticementDataSize;

	uint32_t m_dwMapSectorInfoTime;
	int m_iMapSectorInfoUpdateCount;

	int m_iCrusadeCount;
	bool m_bIsCrusadeMode;

	// Apocalypse
	bool m_bIsApocalypseMode;
	uint32_t m_dwApocalypseGUID;

	// Daryl - Chat logging option
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
	int m_iLastCrusadeWinner; // New 13/05/2004

	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	struct {
		NpcType cType;
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

	int m_iClientShortCut[DEF_MAXCLIENTS + 1];

	int m_iNpcConstructionPoint[DEF_MAXNPCTYPES];
	uint32_t m_dwCrusadeGUID;
	short m_sLastCrusadeDate;
	int m_iCrusadeWinnerSide;
	int m_iPlayerMaxLevel = DEF_PLAYERMAXLEVEL;

	struct {
		int iTotalMembers =0;
		int iIndex[9] {};
	} m_stPartyInfo[DEF_MAXCLIENTS]{};

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
	short m_sSlateSuccessRate = 0;

	// 17/05/2004
	short m_sForceRecallTime;

	// 22/05/2004
	int m_iPrimaryDropRate;
	int m_iSecondaryDropRate;



	// 25/05/2004
	int m_iFinalShutdownCount;

	// New 06/07/2004
	bool m_bEnemyKillMode = false;
	int m_iEnemyKillAdjust = 1;
	bool m_bAdminSecurity = true;

	// Configurable Raid Time 
	short m_sRaidTimeMonday = 0;
	short m_sRaidTimeTuesday = 0;
	short m_sRaidTimeWednesday = 0;
	short m_sRaidTimeThursday = 0;
	short m_sRaidTimeFriday = 0;
	short m_sRaidTimeSaturday = 0;
	short m_sRaidTimeSunday = 0;

	bool m_bManualTime;
	int m_iSummonGuildCost;

	//Heldenian
	bool m_bIsHeldenianMode;
	bool m_bIsHeldenianTeleport;
	//char	m_cHeldenianType;

	// Slate exploit
	int m_sCharPointLimit = 0;

	// Limit Checks
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
	bool m_bReceivedItemList = false;
	bool m_bHeldenianInitiated;
	bool m_bHeldenianRunning;
	// HBX 3.00 Heldenian Source
	bool m_bHeldinianDuty;
	bool m_bHeldenianDutyMultiplyer;
	bool m_bIsHeldenianReady;
	bool m_bIsHeldenianScheduleLoaded;
	bool m_bIsHeldenianSchedule;
	bool bReadBallSystemConfigFile(const char * cFn);

private:
	int __iSearchForQuest(int iClientH, int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, NpcType * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_Cityhall(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, NpcType * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void _ClearExchangeStatus(int iToH);
	int _iGetItemSpaceLeft(int iClientH);

public:
	void AFKChecker();
	void AdminOrder_GoTo(int iClientH, char* pData, uint32_t dwMsgSize);
	void AdminOrder_MonsterCount(int iClientH, char* pData, uint32_t dwMsgSize);
	void AdminOrder_SetForceRecallTime(int iClientH, char* pData, uint32_t dwMsgSize);
	void AdminOrder_UnsummonBoss(int iClientH);
	void RemoveCrusadeNpcs(void);
	void RemoveCrusadeRecallTime(void);
	bool _bCrusadeLog(int iAction, int iClientH, int iData, const char * cName);
	int iGetPlayerABSStatus(int iClientH);
	bool _bInitItemAttr(class CItem * pItem, ItemId iItemID);
	void ReqCreateSlateHandler(int iClientH, char* pData);
	void SetSlateFlag(int iClientH, short sType, bool bFlag);
	void CheckForceRecallTime(int iClientH);
	void SetPlayingStatus(int iClientH);
	void ForceChangePlayMode(int iClientH, bool bNotify);
	void ShowVersion(int iClientH);
	void ShowClientMsg(int iClientH, const char* pMsg);
	void RequestResurrectPlayer(int iClientH, bool bResurrect);
	void GetDkSet(int iClientH);
	void AdminOrder_BanIP(int iClientH, char *pData, uint32_t dwMsgSize);
	void PlayerOrder_ShowDamage(int iClientH);
	void PlayerOrder_GetCrits(int iClientH);
	void RequestRango(int iClientH, int iObjectID); // Morla2.2 - Pide EK y PK
	void GetTradeEKMantleHandler(int iClientH, int iItemID, char * pString); // By Luqah
	void PlayerCommandAddMaster(int iClientH, char * pData, uint32_t dwMsgSize);
	void PlayerOrder_DeleteGuildMaster(int iClientH, char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetCrit(int iClientH, char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetEK(int iClientH, char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetMAJ(int iClientH, char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetRep(int iClientH, char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetPK(int iClientH, char *pData, uint32_t dwMsgSize);
	void RequestBallItem(int iClientH, char *pData, uint32_t dwMsgSize);
	void _TradeItem(int iClientH, char *pData, uint32_t dwMsgSize);
#define DEF_PLAYER_MAX_EXPERIENCE 203484288 // juan249 - Exp negativa fix.y reemplazan los numeros 121586796 por la exp de un personaje max lvl de su servidor.
	void PURITYLIFE(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void DesbugCharacter(int iClientH, char * pData, uint32_t dwMsgSize);
	void CheckDenialServiceAttack(int iClientH, uint32_t dwClientTime);

	//HappyHour para vos
	bool m_bHappyHour;

	uint32_t dwHappyHourTime;

	void EndHappyHour();
	void StarHappyHour();
	void HappyHuor();
	void ManualStartHappyHour(int iClientH, char *pData, uint32_t dwMsgSize);
	void closeHappyHour();
	void openHappyHour();

	//50Cent - Capture The Flag
	void StartCaptureTheFlag();
	void RequestCheckFlag(int iClientH);
	bool bCheckIfIsFlagCarrier(int iClientH);
	void SetFlagCarrierFlag(int iClientH, bool bFlagMode);
	void EndCaptureTheFlag(int iOption);
	void _CreateCTFGUID(int iWinnerSide);

	//50Cent - Capture The Flag
	bool m_bIsCTFMode;
	short m_sFlagCountWin;
	short m_sElvineFlagCount;
	short m_sAresdenFlagCount;
	bool m_bIsElvineFlagCaptured;
	bool m_bIsAresdenFlagCaptured;
	int m_iFlagCarrierIndex;

	//50Cent - 100% Skill
	void PlayerOrder_GetSkills(int iClientH);

	short iH;

};