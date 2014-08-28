#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "Item.h"
#include "GuildsMan.h"
#include "Magic.h"
#include "GlobalDef.h"
#include "Npc.h"

#define DEF_CLIENTSOCKETBLOCKLIMIT	15

#define DEF_MSGBUFFERSIZE	30000
#define DEF_MAXITEMS		50
#define DEF_MAXBANKITEMS	120
#define DEF_MAXGUILDSMAN	128

#define	DEF_MAXMAGICTYPE	100
#define DEF_MAXSKILLTYPE	60

#define DEF_MAXPARTYMEMBERS	8

#define DEF_SPECABLTYTIMESEC	1200

class CClient {
public:
	CClient(HWND hWnd);
	virtual ~CClient();
	bool bCreateNewParty();

	char m_cWarType = 0;
	char m_cVar = 0;
	int m_iRecentWalkTime = 0;
	int m_iRecentRunTime = 0;
	short m_sV1 = 0;
	char m_cHeroArmourBonus = 0;
	int iDmgShowon = 0;

	uint32_t m_dwAFKCheckTime = 0;

	// Hack Checkers
	uint32_t m_dwMagicFreqTime = 0;
	uint32_t m_dwMoveFreqTime = 0;
	uint32_t m_dwAttackFreqTime = 0;
	bool m_bIsMoveBlocked = false;
	bool m_bMagicItem = false;
	uint32_t dwClientTime = 0;
	bool m_bMagicConfirm = false;
	int m_iSpellCount = 0;
	bool m_bMagicPauseTime = false;
	//int m_iUninteruptibleCheck;
	//char m_cConnectionCheck;

	char m_cCharName[11]{};
	char m_cAccountName[11]{};
	char m_cAccountPassword[11]{};

	bool m_bIsInitComplete = false;
	bool m_bIsMsgSendAvailable = false;

	char m_cMapName[11];
	char m_cMapIndex = -1;
	short m_sX = -1;
	short m_sY = -1;

	char m_cGuildName[21]{};
	char m_cLocation[11]{};
	int m_iGuildRank = -1;
	int m_iGuildGUID = -1;

	char m_cDir = 5;
	short m_sType = 0;
	short m_sOriginalType = 0;
	short m_sAppr1 = 0;
	short m_sAppr2 = 0;
	short m_sAppr3 = 0;
	short m_sAppr4 = 0;
	int m_iApprColor = 0;
	int m_iStatus = 0;

	uint32_t m_dwTime = 0;
	uint32_t m_dwHPTime = 0;
	uint32_t m_dwMPTime = 0;
	uint32_t m_dwSPTime = 0;
	uint32_t m_dwAutoSaveTime = 0;
	uint32_t m_dwHungerTime = 0;
	uint32_t m_dwWarmEffectTime = 0;

	char m_cSex = 0;
	char m_cSkin = 0;
	char m_cHairStyle = 0;
	char m_cHairColor = 0;
	char m_cUnderwear = 0;

	int m_iAngelicStr = 0;
	int m_iAngelicInt = 0;
	int m_iAngelicDex = 0;
	int m_iAngelicMag = 0;

	int m_iHP = 0; // Hit Point
	int m_iHPstock = 0;
	int m_iMP = 0;
	int m_iSP = 0;
	int m_iExp = 0;
	int m_iNextLevelExp = 0;
	bool m_bIsKilled = false;

	int m_iDefenseRatio = 0;
	int m_iHitRatio = 0;

	int m_iDamageAbsorption_Armor[DEF_MAXITEMEQUIPPOS]{};
	int m_iDamageAbsorption_Shield = 0;

	int m_iLevel = 0;
	int m_iStr = 0;
	int m_iInt = 0;
	int m_iVit = 0;
	int m_iDex = 0;
	int m_iMag = 0;
	int m_iCharisma = 0;
	int m_iLuck = 0;
	int m_iLU_Pool = 0;
	char m_cAura = 0;
	int m_iGizonItemUpgradeLeft = 0;

	int m_iAddTransMana = 0;
	int m_iAddChargeCritical = 0;

	int m_iEnemyKillCount = 0;
	int m_iPKCount = 0;
	int m_iRewardGold = 0;
	int m_iCurWeightLoad = 0;

	char m_cSide = 0;

	bool m_bInhibition = false;

	char m_cAttackDiceThrow_SM = 0;
	char m_cAttackDiceRange_SM = 0;
	char m_cAttackDiceThrow_L = 0;
	char m_cAttackDiceRange_L = 0;
	char m_cAttackBonus_SM = 0;
	char m_cAttackBonus_L = 0;

	class CItem * m_pItemList[DEF_MAXITEMS]{};
	POINT m_ItemPosList[DEF_MAXITEMS]{};
	class CItem * m_pItemInBankList[DEF_MAXBANKITEMS]{};

	bool m_bIsItemEquipped[DEF_MAXITEMS]{};
	short m_sItemEquipmentStatus[DEF_MAXITEMEQUIPPOS]{};
	char m_cArrowIndex = -1;

	char m_cMagicMastery[DEF_MAXMAGICTYPE]{};
	unsigned char m_cSkillMastery[DEF_MAXSKILLTYPE]{};

	int m_iSkillSSN[DEF_MAXSKILLTYPE]{};
	bool m_bSkillUsingStatus[DEF_MAXSKILLTYPE]{};
	int m_iSkillUsingTimeID[DEF_MAXSKILLTYPE]{};

	char m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS]{};

	int m_iWhisperPlayerIndex = -1;
	char m_cProfile[256]{};

	int m_iHungerStatus = 100;

	uint32_t m_dwWarBeginTime = 0;
	bool m_bIsWarLocation = false;

	bool m_bIsPoisoned = false;
	int m_iPoisonLevel = 0;
	uint32_t m_dwPoisonTime = 0;

	int m_iPenaltyBlockYear = 0;
	int m_iPenaltyBlockMonth = 0;
	int m_iPenaltyBlockDay = 0;

	int m_iFightzoneNumber = 0;
	int m_iReserveTime = 0;
	int m_iFightZoneTicketNumber = 0;

	class XSocket * m_pXSock = nullptr;

	int m_iAdminUserLevel = 0;
	int m_iRating = 0;

	int m_iTimeLeft_ShutUp = 0;
	int m_iTimeLeft_Rating = 0;
	int m_iTimeLeft_ForceRecall = 0;
	int m_iTimeLeft_FirmStaminar = 0;

	bool m_bIsOnServerChange = false;

	int m_iExpStock = 0;
	uint32_t m_dwExpStockTime = 0;

	int m_iAutoExpAmount = 0;
	uint32_t m_dwAutoExpTime = 0;

	uint32_t m_dwRecentAttackTime = 0;

	int m_iAllocatedFish = 0;
	int m_iFishChance = 0;

	char m_cIPaddress[21]{};
	bool m_bIsSafeAttackMode = false;

	bool m_bIsOnWaitingProcess = false;

	int m_iSuperAttackLeft = 0;
	int m_iSuperAttackCount = 0;

	short m_sUsingWeaponSkill = 5;

	int m_iManaSaveRatio = 0;

	bool m_bIsLuckyEffect = false;
	int m_iSideEffect_MaxHPdown = 0;

	int m_iComboAttackCount = 0;
	int m_iDownSkillIndex = -1;

	int m_iMagicDamageSaveItemIndex = -1;

	short m_sCharIDnum1 = 0;
	short m_sCharIDnum2 = 0;
	short m_sCharIDnum3 = 0;

	int m_iAbuseCount = 0;

	bool m_bIsBWMonitor = false;

	bool m_bIsExchangeMode = false; // Is In Exchange Mode? 
	int m_iExchangeH = 0; // Client ID to Exchanging with 
	char m_cExchangeName[11]{}; // Name of Client to Exchanging with 
	char m_cExchangeItemName[4][21]{}; // Name of Item to exchange 

	char m_cExchangeItemIndex[4]{}; // ItemID to Exchange
	int m_iExchangeItemAmount[4]{}; // Ammount to exchange with

	bool m_bIsExchangeConfirm = false; // Has the user hit confirm? 
	int iExchangeCount = 0; //Keeps track of items which are on list

	int m_iQuest = 0;
	int m_iQuestID = 0;
	int m_iAskedQuest = 0;
	int m_iCurQuestCount = 0;

	int m_iQuestRewardType = 0;
	int m_iQuestRewardAmount = 0;

	int m_iContribution = 0;

	bool m_bQuestMatchFlag_Loc = false;
	bool m_bIsQuestCompleted = false;

	int m_iCustomItemValue_Attack = 0;
	int m_iCustomItemValue_Defense = 0;

	int m_iMinAP_SM = 0;
	int m_iMinAP_L = 0;

	int m_iMaxAP_SM = 0;
	int m_iMaxAP_L = 0;

	bool m_bIsNeutral = false;
	bool m_bIsObserverMode = false;

	int m_iSpecialEventID = 200081;

	int m_iSpecialWeaponEffectType = 0;
	int m_iSpecialWeaponEffectValue = 0;

	int m_iAddHP = 0;
	int m_iAddSP = 0;
	int m_iAddMP = 0;
	int m_iAddAR = 0;
	int m_iAddPR = 0;
	int m_iAddDR = 0;
	int m_iAddMR = 0;
	int m_iAddAbsPD = 0;
	int m_iAddAbsMD = 0;
	int m_iAddCD = 0;
	int m_iAddExp = 0;
	int m_iAddGold = 0;

	int m_iAddResistMagic = 0;
	int m_iAddPhysicalDamage = 0;
	int m_iAddMagicalDamage = 0;

	int m_iAddAbsAir = 0;
	int m_iAddAbsEarth = 0;
	int m_iAddAbsFire = 0;
	int m_iAddAbsWater = 0;

	int m_iLastDamage = 0;

	int m_iMoveMsgRecvCount = 0;
	int m_iAttackMsgRecvCount = 0;
	int m_iRunMsgRecvCount = 0;
	int m_iSkillMsgRecvCount = 0;
	uint32_t m_dwMoveLAT = 0;
	uint32_t m_dwRunLAT = 0;
	uint32_t m_dwAttackLAT = 0;

	int m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
	bool m_bIsSpecialAbilityEnabled = false;
	uint32_t m_dwSpecialAbilityStartTime = 0;
	int m_iSpecialAbilityLastSec = 0;

	int m_iSpecialAbilityType = 0;

	int m_iSpecialAbilityEquipPos = 0;
	bool m_bIsAdminCommandEnabled = false;
	int m_iAlterItemDropIndex = -1;

	int m_iWarContribution = 0;

	uint32_t m_dwSpeedHackCheckTime = 0;
	int m_iSpeedHackCheckExp = 0;
	uint32_t m_dwLogoutHackCheck = 0;

	uint32_t m_dwInitCCTimeRcv = 0;
	uint32_t m_dwInitCCTime = 0;

	char m_cLockedMapName[11]{};
	int m_iLockedMapTime = 0;

	int m_iCrusadeDuty = 0;
	uint32_t m_dwCrusadeGUID = 0;
	uint32_t m_dwHeldenianGUID = 0;
	bool m_bInRecallImpossibleMap = false;

	struct {
		NpcType cType = NpcType::none;
		char cSide = 0;
		short sX = 0;
		short sY = 0;
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES]{};
	int m_iCSIsendPoint = 0;

	char m_cSendingMapName[11]{};
	bool m_bIsSendingMapStatus = false;


	int m_iConstructionPoint = 0;

	char m_cConstructMapName[11]{};
	int m_iConstructLocX = -1;
	int m_iConstructLocY = -1;


	bool m_bIsPlayerCivil = false;
	bool m_bIsAttackModeChange = false;



	int m_iPartyID = 0;
	int m_iPartyStatus = 0;
	int m_iReqJoinPartyClientH = 0;
	char m_cReqJoinPartyName[12] {};

	int m_iPartyRank = 0;
	int m_iPartyMemberCount = 0;
	int m_iPartyGUID = 0;

	struct {
		int iIndex = 0;
		char cName[11] {};
	} m_stPartyMemberName[DEF_MAXPARTYMEMBERS]{};


	uint32_t m_dwLastActionTime = 0;
	uint32_t m_dwLastDamageTime = 0;
	int m_iDeadPenaltyTime = 0;


	char m_cWhisperPlayerName[11]{};
	bool m_bIsAdminOrderGoto = false;
	bool m_bIsInsideWarehouse = false;
	bool m_bIsInsideWizardTower = false;
	bool m_bIsInsideOwnTown = false;
	bool m_bIsOwnLocation = false;
	bool m_pIsProcessingAllowed = false;
	bool m_bIsClientConnected = false;


	char m_cHeroArmorBonus = 0;


	bool m_bIsBeingResurrected = false;

	uint32_t m_dwFightzoneDeadTime = 0;
	char m_cSaveCount = 0;
	int m_iBallPoints = 0;

	uint32_t m_dwDSLAT = 0;
	uint32_t m_dwDSLATOld = 0;
	uint32_t m_dwDSLATS = 0;
	int m_iDSCount = 0;
};