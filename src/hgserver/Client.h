#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "Item.h"
#include "GuildsMan.h"
#include "Magic.h"
#include "GlobalDef.h"
#include "Map.h"
#include "Unit.h"
#include <memory>
#include <array>
#include <set>
#define DEF_CLIENTSOCKETBLOCKLIMIT	15
#define DEF_MSGBUFFERSIZE	30000
#define DEF_MAXITEMS		50
#define DEF_MAXBANKITEMS	120
#define DEF_MAXGUILDSMAN	128
#define	DEF_MAXMAGICTYPE	100
#define DEF_MAXSKILLTYPE	60
#define DEF_MAXPARTYMEMBERS	8
#define DEF_SPECABLTYTIMESEC	1200
struct CGame;

template<typename ValType, size_t S> struct Collection {
	constexpr static size_t SLOTS = S;
	typedef ValType value_type;
	typedef value_type& ref_type;
	typedef value_type* iterator;
	ref_type operator[](size_t index) {
		return m_pItemList[index];
	}

	iterator begin() {
		return m_pItemList.begin();
	}

	iterator end() {
		return m_pItemList.end();
	}
private:
	std::array<value_type, S> m_pItemList;
};

typedef Collection<std::unique_ptr<CItem>, DEF_MAXITEMS> Inventory;
typedef Collection<std::unique_ptr<CItem>, DEF_MAXBANKITEMS> BankInventory;
struct CClient;
typedef std::shared_ptr<CClient> ClientPtr;
typedef std::weak_ptr<CClient> ClientWPtr;
struct CNpc;
class CClient : public Unit, public std::enable_shared_from_this<CClient> {
public:
	CClient(CGame &game, int index, std::unique_ptr<XSocket> &&socket);
	CGame &game_;
	int id_;
	char m_cWarType;
	char m_cVar;
	int m_iRecentWalkTime;
	int m_iRecentRunTime;
	short m_sV1;
	char m_cHeroArmourBonus;
	int iDmgShowon;
	bool bCreateNewParty();
	uint32_t m_dwAFKCheckTime;
	// Hack Checkers
	uint32_t m_dwMagicFreqTime;
	uint32_t m_dwMoveFreqTime;
	uint32_t m_dwAttackFreqTime;
	bool m_bIsMoveBlocked;
	bool m_bMagicItem;
	uint32_t dwClientTime;
	bool m_bMagicConfirm;
	int m_iSpellCount;
	bool m_bMagicPauseTime;
	//int m_iUninteruptibleCheck;
	//char m_cConnectionCheck;
	char m_cCharName[11];
	char m_cAccountName[11];
	char m_cAccountPassword[11];
	bool m_bIsInitComplete;
	bool m_bIsMsgSendAvailable;
	MapPtr map_;
	short m_sX;
	short m_sY;
	char m_cGuildName[21];
	char m_cLocation[11];
	int m_iGuildRank;
	int m_iGuildGUID;
	char m_cDir;
	short m_sType;
	short m_sOriginalType;
	short m_sAppr1;
	short m_sAppr2;
	short m_sAppr3;
	short m_sAppr4;
	int m_iApprColor;
	int m_iStatus;
	uint32_t m_dwTime;
	uint32_t m_dwHPTime;
	uint32_t m_dwMPTime;
	uint32_t m_dwSPTime;
	uint32_t m_dwAutoSaveTime;
	uint32_t m_dwHungerTime;
	uint32_t m_dwWarmEffectTime;
	char m_cSex;
	char m_cSkin;
	char m_cHairStyle;
	char m_cHairColor;
	char m_cUnderwear;
	int m_iAngelicStr;
	int m_iAngelicInt;
	int m_iAngelicDex;
	int m_iAngelicMag;
	int m_iHP; // Hit Point
	int m_iHPstock;
	int m_iMP;
	int m_iSP;
	int m_iExp;
	int m_iNextLevelExp;
	bool m_bIsKilled;
	int m_iDefenseRatio; // Defense Ratio
	int m_iHitRatio; // Hit Ratio
	//int  m_iHitRatio_ItemEffect_SM;
	//int  m_iHitRatio_ItemEffect_L;
	int m_iDamageAbsorption_Armor[DEF_MAXITEMEQUIPPOS];
	int m_iDamageAbsorption_Shield;
	int m_iLevel;
	int m_iStr;
	int m_iInt;
	int m_iVit;
	int m_iDex;
	int m_iMag;
	int m_iCharisma;
	//char m_cLU_Str, m_cLU_Int, m_cLU_Vit, m_cLU_Dex, m_cLU_Mag, m_cLU_Char;
	int m_iLuck;
	int m_iLU_Pool;
	char m_cAura;
	int m_iGizonItemUpgradeLeft;
	int m_iAddTransMana;
	int m_iAddChargeCritical;
	int m_iEnemyKillCount;
	int m_iPKCount;
	int m_iRewardGold;
	int m_iCurWeightLoad;
	char m_cSide;
	bool m_bInhibition;
	char m_cAttackDiceThrow_SM;
	char m_cAttackDiceRange_SM;
	char m_cAttackDiceThrow_L;
	char m_cAttackDiceRange_L;
	char m_cAttackBonus_SM;
	char m_cAttackBonus_L;

	Inventory m_pItemList;
	std::array<POINT, DEF_MAXITEMS> m_ItemPosList;
	BankInventory m_pItemInBankList;
	std::array<bool, DEF_MAXITEMS> m_bIsItemEquipped;
	std::array<short, DEF_MAXITEMEQUIPPOS> m_sItemEquipmentStatus;
	char m_cArrowIndex;
	std::array<char, DEF_MAXMAGICTYPE> m_cMagicMastery;
	std::array<unsigned char, DEF_MAXSKILLTYPE> m_cSkillMastery;
	std::array<int, DEF_MAXSKILLTYPE> m_iSkillSSN;
	std::array<bool, DEF_MAXSKILLTYPE> m_bSkillUsingStatus;
	std::array<int, DEF_MAXSKILLTYPE> m_iSkillUsingTimeID;
	std::array<char, DEF_MAXMAGICEFFECTS> m_cMagicEffectStatus;
	ClientWPtr whisperedPlayer_;
	char m_cProfile[256];
	int m_iHungerStatus;
	uint32_t m_dwWarBeginTime;
	bool m_bIsWarLocation;
	bool m_bIsPoisoned;
	int m_iPoisonLevel;
	uint32_t m_dwPoisonTime;
	int m_iPenaltyBlockYear;
	int m_iPenaltyBlockMonth;
	int m_iPenaltyBlockDay;
	int m_iFightzoneNumber;
	int m_iReserveTime;
	int m_iFightZoneTicketNumber;
	std::unique_ptr<XSocket> m_pXSock;
	int m_iAdminUserLevel;
	int m_iRating;
	int m_iTimeLeft_ShutUp;
	int m_iTimeLeft_Rating;
	int m_iTimeLeft_ForceRecall;
	int m_iTimeLeft_FirmStaminar;
	bool m_bIsOnServerChange;
	int m_iExpStock;
	uint32_t m_dwExpStockTime;
	int m_iAutoExpAmount;
	uint32_t m_dwAutoExpTime;
	uint32_t m_dwRecentAttackTime;
	int m_iAllocatedFish;
	int m_iFishChance;
	char m_cIPaddress[21];
	bool m_bIsSafeAttackMode;
	bool m_bIsOnWaitingProcess;
	int m_iSuperAttackLeft;
	int m_iSuperAttackCount;
	short m_sUsingWeaponSkill;
	int m_iManaSaveRatio;
	bool m_bIsLuckyEffect;
	int m_iSideEffect_MaxHPdown;
	int m_iComboAttackCount;
	int m_iDownSkillIndex;
	int m_iMagicDamageSaveItemIndex;
	short m_sCharIDnum1;
	short m_sCharIDnum2;
	short m_sCharIDnum3;
	int m_iAbuseCount;
	bool m_bIsBWMonitor;
	//bool  m_bIsExchangeMode;
	//int   m_iExchangeH;
	//char  m_cExchangeName[11];
	//char  m_cExchangeItemName[21];
	//char  m_cExchangeItemIndex;
	//int   m_iExchangeItemAmount;
	//bool  m_bIsExchangeConfirm;
	bool m_bIsExchangeMode; // Is In Exchange Mode?
	ClientWPtr exchangingWith_;
	char m_cExchangeItemName[4][21]; // Name of Item to exchange
	char m_cExchangeItemIndex[4]; // ItemID to Exchange
	int m_iExchangeItemAmount[4]; // Ammount to exchange with
	bool m_bIsExchangeConfirm; // Has the user hit confirm?
	int iExchangeCount; //Keeps track of items which are on list
	int m_iQuest;
	int m_iQuestID;
	int m_iAskedQuest;
	int m_iCurQuestCount;
	int m_iQuestRewardType;
	int m_iQuestRewardAmount;
	int m_iContribution;
	bool m_bQuestMatchFlag_Loc;
	bool m_bIsQuestCompleted;
	int m_iCustomItemValue_Attack;
	int m_iCustomItemValue_Defense;
	int m_iMinAP_SM;
	int m_iMinAP_L;
	int m_iMaxAP_SM;
	int m_iMaxAP_L;
	bool m_bIsNeutral;
	bool m_bIsObserverMode;
	int m_iSpecialEventID;
	int m_iSpecialWeaponEffectType;
	int m_iSpecialWeaponEffectValue;
	int m_iAddHP;
	int m_iAddSP;
	int m_iAddMP;
	int m_iAddAR;
	int m_iAddPR;
	int m_iAddDR;
	int m_iAddMR;
	int m_iAddAbsPD;
	int m_iAddAbsMD;
	int m_iAddCD;
	int m_iAddExp;
	int m_iAddGold;
	int m_iAddResistMagic;
	int m_iAddPhysicalDamage;
	int m_iAddMagicalDamage;
	int m_iAddAbsAir;
	int m_iAddAbsEarth;
	int m_iAddAbsFire;
	int m_iAddAbsWater;
	int m_iLastDamage;
	int m_iMoveMsgRecvCount;
	int m_iAttackMsgRecvCount;
	int m_iRunMsgRecvCount;
	int m_iSkillMsgRecvCount;
	uint32_t m_dwMoveLAT;
	uint32_t m_dwRunLAT;
	uint32_t m_dwAttackLAT;
	int m_iSpecialAbilityTime;
	bool m_bIsSpecialAbilityEnabled;
	uint32_t m_dwSpecialAbilityStartTime;
	int m_iSpecialAbilityLastSec;
	int m_iSpecialAbilityType;
	int m_iSpecialAbilityEquipPos;
	bool m_bIsAdminCommandEnabled;
	int m_iAlterItemDropIndex;
	int m_iWarContribution;
	uint32_t m_dwSpeedHackCheckTime;
	int m_iSpeedHackCheckExp;
	uint32_t m_dwLogoutHackCheck;
	uint32_t m_dwInitCCTimeRcv;
	uint32_t m_dwInitCCTime;
	MapPtr lockedMap_;
	int m_iLockedMapTime;
	int m_iCrusadeDuty;
	uint32_t m_dwCrusadeGUID;
	uint32_t m_dwHeldenianGUID;
	bool m_bInRecallImpossibleMap;

	struct CrusadeStructureInfo {
		char cType;
		char cSide;
		short sX;
		short sY;
	};
	Collection<CrusadeStructureInfo, DEF_MAXCRUSADESTRUCTURES> m_stCrusadeStructureInfo;
	int m_iCSIsendPoint;
	char m_cSendingMapName[11];
	bool m_bIsSendingMapStatus;
	int m_iConstructionPoint;
	char m_cConstructMapName[11];
	int m_iConstructLocX;
	int m_iConstructLocY;
	bool m_bIsPlayerCivil;
	bool m_bIsAttackModeChange;
	int m_iPartyID;
	int m_iPartyStatus;
	int m_iReqJoinPartyClientH;
	char m_cReqJoinPartyName[12];
	int m_iPartyRank;
	int m_iPartyMemberCount;
	int m_iPartyGUID;

	struct PartyMember {
		int iIndex;
		char cName[11];
	};
	Collection<PartyMember, DEF_MAXPARTYMEMBERS> m_stPartyMemberName;
	uint32_t m_dwLastActionTime;
	uint32_t m_dwLastDamageTime;
	int m_iDeadPenaltyTime;
	bool m_bIsAdminOrderGoto;
	bool m_bIsInsideWarehouse;
	bool m_bIsInsideWizardTower;
	bool m_bIsInsideOwnTown;
	bool m_bIsOwnLocation;
	bool m_pIsProcessingAllowed;
	bool m_bIsClientConnected;
	char m_cHeroArmorBonus;
	bool m_bIsBeingResurrected;
	uint32_t m_dwFightzoneDeadTime;
	char m_cSaveCount;
	int m_iBallPoints;
	uint32_t m_dwDSLAT;
	uint32_t m_dwDSLATOld;
	uint32_t m_dwDSLATS;
	int m_iDSCount;
	bool markedForDeletion_ = false;
	void ClientKilledHandler(int iAttackerH, char cAttackerType, short sDamage);
	void SendNotifyMsg(int iFromH, uint16_t wMsgType, uint32_t sV1, uint32_t sV2, uint32_t sV3, const char * pString, uint32_t sV4 = 0, uint32_t sV5 = 0, uint32_t sV6 = 0, uint32_t sV7 = 0, uint32_t sV8 = 0, uint32_t sV9 = 0, char * pString2 = nullptr);
	int iUpgradeHeroCapeRequirements(int iItemIndex);
	void RequestItemUpgradeHandler(int iItemIndex);
	void ItemDepleteHandler(short sItemIndex, bool bIsUseItemResult, bool bIsItemUsed);
	int iCalcTotalWeight();
	bool bSetItemToBankItem(class CItem * pItem);
	bool bSetItemToBankItem(short sItemIndex);
	int _iCalcMaxLoad();
	void CalculateSSN_ItemIndex(short sWeaponIndex, int iValue);
	void CalculateSSN_SkillIndex(short sSkillIndex, int iValue);
	void _ClearQuestStatus();
	int iGetMaxHP();
	int iGetMaxMP();
	int iGetMaxSP();
	void AdminOrder_GetFightzoneTicket();
	void GetAngelHandler(char * pData, uint32_t dwMsgSize);
	void processClientMsg(uint32_t msgId, char *pData, uint32_t dwMsgSize, char cKey);
	void RequestResurrectPlayer(bool bResurrect);
	void RequestTeleportHandler(const char * pData, const char * cMapName = nullptr, int dX = -1, int dY = -1);
	void CalcTotalItemEffect(int iEquipItemID, bool bNotify = true);
	void ReqCreateCraftingHandler(char *pData);
	void ClientCommonHandler(char * pData);
	void ReqCreateSlateHandler(char* pData);
	void DeleteClient(bool bSave, bool bNotify, bool bCountLogout = true, bool bForceCloseConn = false);
	void ArmorLifeDecrement(int iTargetH, char cOwnerType, int iValue);
	void ReleaseItemHandler(short sItemIndex, bool bNotice);
	void RequestRestartHandler();
	void update(uint32_t dwTime);
	int iComposeMoveMapData(short sX, short sY, char cDir, char * pData);
	int iComposeInitMapData(short sX, short sY, char * pData);
	void RequestHeldenianScroll(char * pData, uint32_t dwMsgSize);
	void CheckHeldenianResultCalculation();
	void ManualStartHeldenianMode(char *pData, uint32_t dwMsgSize);
	void ManualEndHeldenianMode(char *pData, uint32_t dwMsgSize);
	void RequestHeldenianTeleport(char * pData, uint32_t dwMsgSize);
	void RequestNpcSpeakTeleport(char * pData, uint32_t dwMsgSize);
	bool _bCheckCharacterData();
	char _cCheckHeroItemEquipped();
	bool bPlantSeedBag(int dX, int dY, int iItemEffectValue1, int iItemEffectValue2);
	void GreenBall_Weather(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CleanMap(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CheckStats(char *pData, uint32_t dwMsgSize);
	void Command_RedBall(char *pData, uint32_t dwMsgSize);
	void Command_BlueBall(char *pData, uint32_t dwMsgSize);
	void Command_YellowBall(char* pData, uint32_t dwMsgSize);
	void _SendMapStatus();
	void MapStatusHandler(int iMode, char * pMapName);
	void SelectCrusadeDutyHandler(int iDuty);
	void RequestSummonWarUnitHandler(int dX, int dY, char cType, char cNum, char cMode);
	void RequestGuildTeleportHandler();
	void RequestSetGuildTeleportLocHandler(int dX, int dY, int iGuildGUID, const char * pMapName);
	void RequestSetGuildConstructLocHandler(int dX, int dY, int iGuildGUID, char * pMapName);
	void CheckCommanderConstructionPoint();
	void CheckCrusadeResultCalculation();
	bool __bSetConstructionKit(int dX, int dY, int iType, int iTimeCost);
	void AdminOrder_Time(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CheckRep(char *pData);
	void AdminOrder_Pushplayer(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CheckRep(char *pData, uint32_t dwMsgSize);
	void SetForceRecallTime();
	void ApplyCombatKilledPenalty(int cPenaltyLevel, bool bIsSAattacked);
	void AdminOrder_ClearNpc();
	bool bCheckClientMoveFrequency(uint32_t dwClientTime);
	bool bCheckClientMagicFrequency(uint32_t dwClientTime);
	bool bCheckClientAttackFrequency(uint32_t dwClientTime);
	void RequestChangePlayMode();
	void GetHeroMantleHandler(int iItemID, char * pString);
	void AdminOrder_Weather(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SendMSG(char *pData, uint32_t dwMsgSize);
	bool bCheckMagicInt();
	void StateChangeHandler(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetStatus(char *pData, uint32_t dwMsgSize);
	void AdminOrder_SummonStorm(char* pData, uint32_t dwMsgSize);
	void AdminOrder_CallMagic(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonDeath();
	void AdminOrder_SetZerk(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetFreeze(char * pData, uint32_t dwMsgSize);
	void AdminOrder_Kill(char * pData, uint32_t dwMsgSize);
	void AdminOrder_Revive(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetObserverMode();
	void AdminOrder_EnableAdminCommand(char *pData, uint32_t dwMsgSize);
	void AdminOrder_CreateItem(char * pData, uint32_t dwMsgSize);
	void AdminOrder_Summon(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonAll(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonPlayer(char * pData, uint32_t dwMsgSize);
	void AdminOrder_UnsummonDemon();
	void AdminOrder_UnsummonAll();
	void AdminOrder_SetAttackMode(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SummonDemon();
	void AdminOrder_SetInvi(char * pData, uint32_t dwMsgSize);
	void AdminOrder_Polymorph(char * pData, uint32_t dwMsgSize);
	void AdminOrder_GetNpcStatus(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CheckIP(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CreateFish(char * pData, uint32_t dwMsgSize);
	void AdminOrder_Teleport(char * pData, uint32_t dwMsgSize);
	void AdminOrder_ReserveFightzone(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CloseConn(char * pData, uint32_t dwMsgSize);
	void AdminOrder_CallGuard(char * pData, uint32_t dwMsgSize);
	void AdminOrder_DisconnectAll(char * pData, uint32_t dwMsgSize);
	void RequestHelpHandler();
	void CheckConnectionHandler(char *pData);
	void CancelQuestHandler();
	void ActivateSpecialAbilityHandler();
	void JoinPartyHandler(int iV1, char *pMemberName);
	void CreateNewPartyHandler();
	void RequestSellItemListHandler(char * pData);
	int iRequestPanningMapDataRequest(char * pData);
	void GetMagicAbilityHandler();
	void _TamingHandler(int iSkillNum, MapPtr map, int dX, int dY);
	int _iTalkToNpcResult_Guard(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void RequestNoticementHandler(char * pData);
	void CheckSpecialEvent();
	void BuildItemHandler(char * pData);
	bool _bCheckItemReceiveCondition(class CItem * pItem);
	void SendItemNotifyMsg(uint16_t wMsgType, class CItem * pItem, int iV1);
	int _iTalkToNpcResult_WTower(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_WHouse(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_BSmith(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_GShop(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_GuildHall(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	bool _bCheckIsQuestCompleted();
	void _CheckQuestEnvironment();
	void _SendQuestContents();
	void QuestAcceptedHandler();
	void CancelExchangeItem();
	bool bAddItem(class CItem * pItem, char cMode);
	void ConfirmExchangeItem();
	void SetExchangeItem(int iItemIndex, int iAmount);
	void ExchangeItemHandler(short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName);
	void _BWM_Init(char * pData);
	void CheckUniqueItemEquipment();
	void _SetItemPos(char * pData);
	void GetOccupyFlagHandler();
	void SetSummonMobAction(int iMode, uint32_t dwMsgSize, char * pData = nullptr);
	bool _bDepleteDestTypeItemUseEffect(int dX, int dY, short sItemIndex, short sDestItemID);
	void SetDownSkillIndexHandler(int iSkillIndex);
	int _iGetWeaponSkillType();
	void NpcTalkHandler(int iWho);
	void _CheckMiningAction(int dX, int dY);
	void LocalSavePlayerData();
	void ReqCreatePortionHandler(char * pData);
	void _CheckAttackType(short * spType);
	void ToggleSafeAttackModeHandler();
	int iGetPlayerRelationship_SendEvent(int iOpponentH);
	int iGetPlayerRelationship(int iOpponentH);
	void ReqGetFishThisTimeHandler();
	int iCheckFish(MapPtr map, short dX, short dY);
	void UserCommand_DissmissGuild(char * pData, uint32_t dwMsgSize);
	void UserCommand_BanGuildsman(char * pData, uint32_t dwMsgSize);
	void ___RestorePlayerRating();
	void CalcExpStock();
	void SetPlayerReputation(char * pMsg, char cValue, uint32_t dwMsgSize);
	void ShutUpPlayer(char * pMsg, uint32_t dwMsgSize);
	void PoisonEffect(int iV1);
	bool _bGetIsPlayerHostile(int sOwnerH);
	bool bAnalyzeCriminalAction(short dX, short dY, bool bIsCheck = false);
	void ___RestorePlayerCharacteristics();
	void GetPlayerProfile(char * pMsg, uint32_t dwMsgSize);
	void SetPlayerProfile(char * pMsg, uint32_t dwMsgSize);
	void ToggleWhisperPlayer(char * pMsg, uint32_t dwMsgSize);
	void CheckAndNotifyPlayerConnection(char * pMsg, uint32_t dwSize);
	void ReqRepairItemCofirmHandler(char cItemID, char * pString);
	void ReqRepairItemHandler(char cItemID, char cRepairWhom, char * pString);
	void ReqSellItemConfirmHandler(char cItemID, int iNum, char * pString);
	void ReqSellItemHandler(char cItemID, char cSellToWhom, int iNum, char * pItemName);
	void UseSkillHandler(int iV1, int iV2, int iV3);
	void ClearSkillUsingStatus();
	void SendObjectMotionRejectMsg();
	bool bCheckTotalSkillMasteryPoints(int iSkill);
	void UseItemHandler(short sItemIndex, short dX, short dY, short sDestItemID);
	int _iGetArrowItemIndex();
	void RequestFullObjectData(char * pData);
	void GetRewardMoneyHandler();
	void _PenaltyItemDrop(int iTotal, bool bIsSAattacked = false);
	void EnemyKillRewardHandler(int iAttackerH);
	void RequestRetrieveItemHandler(char *pData);
	void RequestCivilRightHandler(char * pData);
	bool bCheckLimitedUser();
	void LevelUpSettingsHandler(char * pData, uint32_t dwMsgSize);
	void FightzoneReserveHandler(char * pData, uint32_t dwMsgSize);
	bool bCheckLevelUp();
	void TimeManaPointsUp();
	void TimeStaminarPointsUp();
	bool bBankItemToPlayer(short sItemIndex);
	bool bPlayerItemToBank(short sItemIndex);
	void TrainSkillResponse(bool bSuccess,int iSkillNum, int iSkillLevel);
	void RequestStudyMagicHandler(char * pName, bool bIsPurchase = true);
	void PlayerMagicHandler(int dX, int dY, short sType, bool bItemEffect = false, int iV1 = 0);
	int iClientMotion_Magic_Handler(short sX, short sY, char cDir);
	void ToggleCombatModeHandler();
	void SendGuildMsg(uint16_t wNotifyMsgType, short sV1, short sV2, char * pString);
	void TimeHitPointsUp();
	int SetItemCount(const char * pItemName, uint32_t dwCount);
	int SetItemCount(int iItemIndex, uint32_t dwCount);
	uint32_t dwGetItemCount(const char * pName);
	void DismissGuildRejectHandler(char * pName);
	void DismissGuildApproveHandler(char * pName);
	void JoinGuildRejectHandler(char * pName);
	void JoinGuildApproveHandler(char * pName);
	void GiveItemHandler(short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName);
	void RequestPurchaseItemHandler(char * pItemName, int iNum);
	void RequestDisbandGuildHandler(char * pData, uint32_t dwMsgSize);
	void RequestCreateNewGuildHandler(char * pData, uint32_t dwMsgSize);
	int iClientMotion_Stop_Handler(short sX, short sY, char cDir);
	bool bEquipItemHandler(short sItemIndex, bool bNotify = true);
	bool _bAddClientItemList(class CItem * pItem, int * pDelReq);
	int iClientMotion_GetItem_Handler(short sX, short sY, char cDir);
	void DropItemHandler(short sItemIndex, int iAmount, char * pItemName, bool bByPlayer = true);
	int iClientMotion_Attack_Handler(short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse = true, bool bIsDash = false);
	void ChatMsgHandler(char * pData, uint32_t dwMsgSize);
	void InitPlayerData(char * pData, uint32_t dwSize);
	bool bSendMsgToLS(uint32_t dwMsg,bool bFlag = true, char *pData = nullptr);
	void RequestInitDataHandler(char * pData, char cKey);
	void RequestInitPlayerHandler(char * pData, char cKey);
	int iClientMotion_Move_Handler(short sX, short sY, char cDir, char cMoveType);
	void ClientMotionHandler(char * pData);
	void OnClientRead();
	void GetFightzoneTicketHandler();
	bool bCheckIsItemUpgradeSuccess(int iItemIndex, int iSomH, bool bBonus = false);
	void StormBringer(short dX, short dY);
	void RequestCreatePartyHandler();
	void PartyOperationResult_Create(char *pName, int iResult, int iPartyID);
	void PartyOperationResult_Join(char *pName, int iResult, int iPartyID);
	void PartyOperationResult_Dismiss(char *pName, int iResult, int iPartyID);
	void RequestJoinPartyHandler(char *pData, uint32_t dwMsgSize);
	void RequestDismissPartyHandler();
	void GetPartyInfoHandler();
	void PartyOperationResult_Info(char * pName, int iTotal, char *pNameList);
	void RequestDeletePartyHandler();
	void RequestAcceptJoinPartyHandler(int iResult);
	void GetExp(int iExp, bool bIsAttackerOwn = false);
	void RequestGuildNameHandler(int iObjectID, int iIndex);
	bool _bItemLog(int iAction,char * cName, class CItem * pItem);
	int _iComposePlayerDataFileContents(char * pData);
	bool _bDecodePlayerDatafileContents(char * pData, uint32_t dwSize);
	int __iSearchForQuest(int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_Cityhall(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iGetItemSpaceLeft();
	void AdminOrder_GoTo(char* pData, uint32_t dwMsgSize);
	void AdminOrder_MonsterCount(char* pData, uint32_t dwMsgSize);
	void AdminOrder_SetForceRecallTime(char* pData, uint32_t dwMsgSize);
	void AdminOrder_UnsummonBoss();
	bool _bCrusadeLog(int iAction,int iData, const char * cName);
	void _CheckFarmingAction(CNpc &targetNpc, bool bType);
	int iGetPlayerABSStatus();
	void SetSlateFlag(short sType, bool bFlag);
	void CheckForceRecallTime();
	void SetPlayingStatus();
	void ForceChangePlayMode(bool bNotify);
	void ShowVersion();
	void ShowClientMsg(const char* pMsg);
	void GetDkSet();
	void AdminOrder_BanIP(char *pData, uint32_t dwMsgSize);
	void PlayerOrder_ShowDamage();
	void PlayerOrder_GetCrits();
	void RequestRango(int iObjectID); // Morla2.2 - Pide EK y PK
	void GetTradeEKMantleHandler(int iItemID, char * pString); // By Luqah
	void PlayerCommandAddMaster(char * pData, uint32_t dwMsgSize);
	void PlayerOrder_DeleteGuildMaster(char * pData, uint32_t dwMsgSize);
	void AdminOrder_SetCrit(char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetEK(char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetMAJ(char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetRep(char *pData, uint32_t dwMsgSize);
	void AdminOrder_SetPK(char *pData, uint32_t dwMsgSize);
	void RequestBallItem(char *pData, uint32_t dwMsgSize);
	void _TradeItem(char *pData, uint32_t dwMsgSize);
	void DesbugCharacter(char * pData, uint32_t dwMsgSize);
	void CheckDenialServiceAttack(uint32_t dwClientTime);
	void ManualStartHappyHour(char *pData, uint32_t dwMsgSize);
	void RequestCheckFlag();
	bool bCheckIfIsFlagCarrier();
	void SetFlagCarrierFlag(bool bFlagMode);
	void PlayerOrder_GetSkills();
	void _ClearExchangeStatus();
	void RemoveFromTarget();
	void SetAngelFlag(int iStatus, int iTemp);
	void SetIceFlag(bool bStatus);
	void SetHeroFlag(bool bStatus);
	void SetInvisibilityFlag(bool bStatus);
	void SetIllusionFlag(bool bStatus);
	void SetDefenseShieldFlag(bool bStatus);
	void SetMagicProtectionFlag(bool bStatus);
	void SetProtectionFromArrowFlag(bool bStatus);
	void SetIllusionMovementFlag(bool bStatus);
	void SetBerserkFlag(bool bStatus);
	void SetPoisonFlag(bool bStatus);
	void SetInhibitionCastingFlag(bool bStatus);
	bool bCheckHeldenianMap();
	void Effect_Damage_Spot(short sTargetH, char cTargetType, short sV1, short sV2, short sV3, bool bExp, int iAttr = 0);
	void Effect_HpUp_Spot(short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void PURITYLIFE(short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot_DamageMove(short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr);
	void Effect_SpDown_Spot(short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpUp_Spot(short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void RemoveFromTarget(int iCode);
	bool bCheckResistingPoisonSuccess();	
private:

	CMap& _getMap() override;
	UnitPtr _getPtr() override;
	int _getEffectiveIceResist() override;
	void _sendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3) override;
};
#define DEF_MAXCLIENTS				2000

struct Clients {
	typedef std::shared_ptr<CClient> value_type;
	typedef value_type &ref_type;

	Clients(CGame &game) : game_(game) {
	}

	void clear() {
		m_pClientList = Arr();
	}

	std::shared_ptr<CClient> add(int index, std::unique_ptr<XSocket>&& socket) {
		std::shared_ptr<CClient> ptr = std::make_shared<CClient>(game_, index, std::move(socket));
		m_pClientList[index] = ptr;
		bAddClientShortCut(index);
		totalClients_++;
		if (totalClients_ > maxClients_) {
			maxClients_ = totalClients_;
		}
		return ptr;
	}
	ref_type operator[](size_t index) {
		return m_pClientList[index];
	}

	std::size_t getTotalClients() const {
		return totalClients_;
	}

	std::size_t getMaxClients() const {
		return maxClients_;
	}

	void cull() {
		for (auto &ptr : m_pClientList) {
			if (ptr && ptr->markedForDeletion_) {
				RemoveClientShortCut(ptr->id_);
				ptr.reset();
				--totalClients_;
			}
		}
	}

	struct Iter {

		Iter(Clients &clients, size_t index) : clients_(clients), index_(index) {
		}

		CClient &operator*() {
			return *clients_[index_];
		}

		void operator++() {
			do {
				++index_;
			} while (index_ < DEF_MAXCLIENTS && !clients_[index_] && clients_[index_]->markedForDeletion_);
		}

		bool operator!=(const Iter &o) const {
			return index_ != o.index_;
		}
	private:
		Clients &clients_;
		size_t index_;
	};

	Iter begin() {
		Iter ret(*this, 0);
		++ret;
		return ret;
	}

	Iter end() {
		return Iter(*this, DEF_MAXCLIENTS);
	}
private:
	CGame &game_;
	size_t totalClients_ = 0;
	size_t maxClients_ = 0;
	typedef std::array<value_type, DEF_MAXCLIENTS> Arr;
	Arr m_pClientList;
	std::set<int> m_iClientShortCut;

	bool bAddClientShortCut(int iClientH) {
		m_iClientShortCut.insert(iClientH);
		return true;
	}

	void RemoveClientShortCut(int iClientH) {
		m_iClientShortCut.erase(iClientH);
	}
};