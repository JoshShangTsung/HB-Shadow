#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "Item.h"
#include "GuildsMan.h"
#include "Magic.h"
#include "GlobalDef.h"

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

	char m_cWarType;
	char m_cVar;
	int m_iRecentWalkTime;
	int m_iRecentRunTime;
	short m_sV1;
	char m_cHeroArmourBonus;
	int iDmgShowon;

	BOOL bCreateNewParty();
	DWORD m_dwAFKCheckTime;

	// Hack Checkers
	DWORD m_dwMagicFreqTime, m_dwMoveFreqTime, m_dwAttackFreqTime;
	BOOL m_bIsMoveBlocked, m_bMagicItem;
	DWORD dwClientTime;
	BOOL m_bMagicConfirm;
	int m_iSpellCount;
	BOOL m_bMagicPauseTime;
	//int m_iUninteruptibleCheck;
	//char m_cConnectionCheck;

	CClient(HWND hWnd);
	virtual ~CClient();

	char m_cCharName[11];
	char m_cAccountName[11];
	char m_cAccountPassword[11];

	BOOL m_bIsInitComplete;
	BOOL m_bIsMsgSendAvailable;

	char m_cMapName[11];
	char m_cMapIndex;
	short m_sX, m_sY;

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

	DWORD m_dwTime, m_dwHPTime, m_dwMPTime, m_dwSPTime, m_dwAutoSaveTime, m_dwHungerTime, m_dwWarmEffectTime;

	char m_cSex, m_cSkin, m_cHairStyle, m_cHairColor, m_cUnderwear;

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
	BOOL m_bIsKilled;

	int m_iDefenseRatio; // Defense Ratio
	int m_iHitRatio; // Hit Ratio

	//int  m_iHitRatio_ItemEffect_SM;
	//int  m_iHitRatio_ItemEffect_L;

	int m_iDamageAbsorption_Armor[DEF_MAXITEMEQUIPPOS];
	int m_iDamageAbsorption_Shield;

	int m_iLevel;
	int m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma;
	//char m_cLU_Str, m_cLU_Int, m_cLU_Vit, m_cLU_Dex, m_cLU_Mag, m_cLU_Char;
	int m_iLuck;
	int m_iLU_Pool;
	char m_cAura;
	int m_iGizonItemUpgradeLeft;

	int m_iAddTransMana, m_iAddChargeCritical;

	int m_iEnemyKillCount, m_iPKCount, m_iRewardGold;
	int m_iCurWeightLoad;

	char m_cSide;

	BOOL m_bInhibition;

	char m_cAttackDiceThrow_SM;
	char m_cAttackDiceRange_SM;
	char m_cAttackDiceThrow_L;
	char m_cAttackDiceRange_L;
	char m_cAttackBonus_SM;
	char m_cAttackBonus_L;

	class CItem * m_pItemList[DEF_MAXITEMS];
	POINT m_ItemPosList[DEF_MAXITEMS];
	class CItem * m_pItemInBankList[DEF_MAXBANKITEMS];

	BOOL m_bIsItemEquipped[DEF_MAXITEMS];
	short m_sItemEquipmentStatus[DEF_MAXITEMEQUIPPOS];
	char m_cArrowIndex;

	char m_cMagicMastery[DEF_MAXMAGICTYPE];
	unsigned char m_cSkillMastery[DEF_MAXSKILLTYPE];

	int m_iSkillSSN[DEF_MAXSKILLTYPE];
	BOOL m_bSkillUsingStatus[DEF_MAXSKILLTYPE];
	int m_iSkillUsingTimeID[DEF_MAXSKILLTYPE];

	char m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	int m_iWhisperPlayerIndex;
	char m_cProfile[256];

	int m_iHungerStatus;

	DWORD m_dwWarBeginTime;
	BOOL m_bIsWarLocation;

	BOOL m_bIsPoisoned;
	int m_iPoisonLevel;
	DWORD m_dwPoisonTime;

	int m_iPenaltyBlockYear, m_iPenaltyBlockMonth, m_iPenaltyBlockDay;

	int m_iFightzoneNumber, m_iReserveTime, m_iFightZoneTicketNumber;

	class XSocket * m_pXSock;

	int m_iAdminUserLevel;
	int m_iRating;

	int m_iTimeLeft_ShutUp;
	int m_iTimeLeft_Rating;
	int m_iTimeLeft_ForceRecall;
	int m_iTimeLeft_FirmStaminar;

	BOOL m_bIsOnServerChange;

	int m_iExpStock;
	DWORD m_dwExpStockTime;

	int m_iAutoExpAmount;
	DWORD m_dwAutoExpTime;

	DWORD m_dwRecentAttackTime;

	int m_iAllocatedFish;
	int m_iFishChance;

	char m_cIPaddress[21];
	BOOL m_bIsSafeAttackMode;

	BOOL m_bIsOnWaitingProcess;

	int m_iSuperAttackLeft;
	int m_iSuperAttackCount;

	short m_sUsingWeaponSkill;

	int m_iManaSaveRatio;

	BOOL m_bIsLuckyEffect;
	int m_iSideEffect_MaxHPdown;

	int m_iComboAttackCount;
	int m_iDownSkillIndex;

	int m_iMagicDamageSaveItemIndex;

	short m_sCharIDnum1, m_sCharIDnum2, m_sCharIDnum3;

	int m_iAbuseCount;

	BOOL m_bIsBWMonitor;

	//BOOL  m_bIsExchangeMode;		// ���� ������ ��ȯ ����ΰ�? 
	//int   m_iExchangeH;				// ��ȯ�� ����� �ε��� 
	//char  m_cExchangeName[11];		// ��ȯ�� ����� �̸� 
	//char  m_cExchangeItemName[21];	// ��ȯ�ϰ��� �ϴ� ������ �̸� 
	//char  m_cExchangeItemIndex;  // ��ȯ�� ������ �ε��� 
	//int   m_iExchangeItemAmount; // ��ȯ�� ������ ���� 
	//BOOL  m_bIsExchangeConfirm;  // ��ȯ Ȯ�� 

	BOOL m_bIsExchangeMode; // Is In Exchange Mode? 
	int m_iExchangeH; // Client ID to Exchanging with 
	char m_cExchangeName[11]; // Name of Client to Exchanging with 
	char m_cExchangeItemName[4][21]; // Name of Item to exchange 

	char m_cExchangeItemIndex[4]; // ItemID to Exchange
	int m_iExchangeItemAmount[4]; // Ammount to exchange with

	BOOL m_bIsExchangeConfirm; // Has the user hit confirm? 
	int iExchangeCount; //Keeps track of items which are on list

	int m_iQuest; 
	int m_iQuestID; 
	int m_iAskedQuest; 
	int m_iCurQuestCount; 

	int m_iQuestRewardType; 
	int m_iQuestRewardAmount; 

	int m_iContribution; 

	BOOL m_bQuestMatchFlag_Loc; 
	BOOL m_bIsQuestCompleted; 

	int m_iCustomItemValue_Attack;
	int m_iCustomItemValue_Defense;

	int m_iMinAP_SM; 
	int m_iMinAP_L;

	int m_iMaxAP_SM; 
	int m_iMaxAP_L;

	BOOL m_bIsNeutral; 
	BOOL m_bIsObserverMode; 

	int m_iSpecialEventID; 

	int m_iSpecialWeaponEffectType; 
	int m_iSpecialWeaponEffectValue; 
	
	
	

	
	int m_iAddHP, m_iAddSP, m_iAddMP;
	int m_iAddAR, m_iAddPR, m_iAddDR;
	int m_iAddMR, m_iAddAbsPD, m_iAddAbsMD;
	int m_iAddCD, m_iAddExp, m_iAddGold; 

	int m_iAddResistMagic; 
	int m_iAddPhysicalDamage; 
	int m_iAddMagicalDamage;

	int m_iAddAbsAir; 
	int m_iAddAbsEarth;
	int m_iAddAbsFire;
	int m_iAddAbsWater;

	int m_iLastDamage;

	int m_iMoveMsgRecvCount, m_iAttackMsgRecvCount, m_iRunMsgRecvCount, m_iSkillMsgRecvCount;
	DWORD m_dwMoveLAT, m_dwRunLAT, m_dwAttackLAT;

	int m_iSpecialAbilityTime; 
	BOOL m_bIsSpecialAbilityEnabled; 
	DWORD m_dwSpecialAbilityStartTime; 
	int m_iSpecialAbilityLastSec; 

	int m_iSpecialAbilityType; 
	
	
	
	
	int m_iSpecialAbilityEquipPos; 
	BOOL m_bIsAdminCommandEnabled;
	int m_iAlterItemDropIndex; 

	int m_iWarContribution; 

	DWORD m_dwSpeedHackCheckTime; 
	int m_iSpeedHackCheckExp;
	DWORD m_dwLogoutHackCheck;

	DWORD m_dwInitCCTimeRcv;
	DWORD m_dwInitCCTime;

	char m_cLockedMapName[11]; 
	int m_iLockedMapTime; 

	int m_iCrusadeDuty; 
	DWORD m_dwCrusadeGUID; 
	DWORD m_dwHeldenianGUID;
	BOOL m_bInRecallImpossibleMap;

	

	struct {
		char cType;
		char cSide;
		short sX, sY;
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iCSIsendPoint;

	char m_cSendingMapName[11];
	BOOL m_bIsSendingMapStatus;

	
	int m_iConstructionPoint;

	char m_cConstructMapName[11];
	int m_iConstructLocX, m_iConstructLocY;

	
	BOOL m_bIsPlayerCivil;
	BOOL m_bIsAttackModeChange;

	
	
	int m_iPartyID;
	int m_iPartyStatus;
	int m_iReqJoinPartyClientH;
	char m_cReqJoinPartyName[12];

	int m_iPartyRank; 
	int m_iPartyMemberCount; 
	int m_iPartyGUID; 

	struct {
		int iIndex;
		char cName[11];
	} m_stPartyMemberName[DEF_MAXPARTYMEMBERS];

	
	DWORD m_dwLastActionTime, m_dwLastDamageTime;
	int m_iDeadPenaltyTime;

	
	char m_cWhisperPlayerName[11];
	BOOL m_bIsAdminOrderGoto;
	BOOL m_bIsInsideWarehouse;
	BOOL m_bIsInsideWizardTower;
	BOOL m_bIsInsideOwnTown;
	BOOL m_bIsOwnLocation;
	BOOL m_pIsProcessingAllowed;
	BOOL m_bIsClientConnected;

	
	char m_cHeroArmorBonus;

	
	BOOL m_bIsBeingResurrected;

	DWORD m_dwFightzoneDeadTime;
	char m_cSaveCount;
	int m_iBallPoints;

	DWORD m_dwDSLAT, m_dwDSLATOld, m_dwDSLATS;
	int m_iDSCount;
};