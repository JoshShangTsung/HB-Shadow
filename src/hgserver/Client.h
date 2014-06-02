#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "Item.h"
#include "GuildsMan.h"
#include "Magic.h"
#include "GlobalDef.h"
#include <memory>
#include <array>

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
class CClient {
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
	uint32_t m_dwMagicFreqTime, m_dwMoveFreqTime, m_dwAttackFreqTime;
	bool m_bIsMoveBlocked, m_bMagicItem;
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

	uint32_t m_dwTime, m_dwHPTime, m_dwMPTime, m_dwSPTime, m_dwAutoSaveTime, m_dwHungerTime, m_dwWarmEffectTime;

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
	bool m_bIsKilled;

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

	bool m_bInhibition;

	char m_cAttackDiceThrow_SM;
	char m_cAttackDiceRange_SM;
	char m_cAttackDiceThrow_L;
	char m_cAttackDiceRange_L;
	char m_cAttackBonus_SM;
	char m_cAttackBonus_L;

	std::array<std::unique_ptr<CItem>, DEF_MAXITEMS> m_pItemList;
	std::array<POINT, DEF_MAXITEMS> m_ItemPosList;
	std::array<std::unique_ptr<CItem>, DEF_MAXBANKITEMS> m_pItemInBankList;

	std::array<bool, DEF_MAXITEMS> m_bIsItemEquipped;
	std::array<short, DEF_MAXITEMEQUIPPOS> m_sItemEquipmentStatus;
	char m_cArrowIndex;

	std::array<char, DEF_MAXMAGICTYPE> m_cMagicMastery;
	std::array<unsigned char, DEF_MAXSKILLTYPE> m_cSkillMastery;

	std::array<int, DEF_MAXSKILLTYPE> m_iSkillSSN;
	std::array<bool, DEF_MAXSKILLTYPE> m_bSkillUsingStatus;
	std::array<int, DEF_MAXSKILLTYPE> m_iSkillUsingTimeID;

	std::array<char, DEF_MAXMAGICEFFECTS> m_cMagicEffectStatus;

	int m_iWhisperPlayerIndex;
	char m_cProfile[256];

	int m_iHungerStatus;

	uint32_t m_dwWarBeginTime;
	bool m_bIsWarLocation;

	bool m_bIsPoisoned;
	int m_iPoisonLevel;
	uint32_t m_dwPoisonTime;

	int m_iPenaltyBlockYear, m_iPenaltyBlockMonth, m_iPenaltyBlockDay;

	int m_iFightzoneNumber, m_iReserveTime, m_iFightZoneTicketNumber;

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

	short m_sCharIDnum1, m_sCharIDnum2, m_sCharIDnum3;

	int m_iAbuseCount;

	bool m_bIsBWMonitor;

	//bool  m_bIsExchangeMode;		// ���� ������ ��ȯ ����ΰ�? 
	//int   m_iExchangeH;				// ��ȯ�� ����� �ε��� 
	//char  m_cExchangeName[11];		// ��ȯ�� ����� �̸� 
	//char  m_cExchangeItemName[21];	// ��ȯ�ϰ��� �ϴ� ������ �̸� 
	//char  m_cExchangeItemIndex;  // ��ȯ�� ������ �ε��� 
	//int   m_iExchangeItemAmount; // ��ȯ�� ������ ���� 
	//bool  m_bIsExchangeConfirm;  // ��ȯ Ȯ�� 

	bool m_bIsExchangeMode; // Is In Exchange Mode? 
	int m_iExchangeH; // Client ID to Exchanging with 
	char m_cExchangeName[11]; // Name of Client to Exchanging with 
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
	uint32_t m_dwMoveLAT, m_dwRunLAT, m_dwAttackLAT;

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

	char m_cLockedMapName[11];
	int m_iLockedMapTime;

	int m_iCrusadeDuty;
	uint32_t m_dwCrusadeGUID;
	uint32_t m_dwHeldenianGUID;
	bool m_bInRecallImpossibleMap;

	struct CrusadeStructureInfo {
		char cType;
		char cSide;
		short sX, sY;
	};
	std::array<CrusadeStructureInfo, DEF_MAXCRUSADESTRUCTURES> m_stCrusadeStructureInfo;
	int m_iCSIsendPoint;

	char m_cSendingMapName[11];
	bool m_bIsSendingMapStatus;


	int m_iConstructionPoint;

	char m_cConstructMapName[11];
	int m_iConstructLocX, m_iConstructLocY;


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
	std::array<PartyMember, DEF_MAXPARTYMEMBERS> m_stPartyMemberName;


	uint32_t m_dwLastActionTime, m_dwLastDamageTime;
	int m_iDeadPenaltyTime;


	char m_cWhisperPlayerName[11];
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

	uint32_t m_dwDSLAT, m_dwDSLATOld, m_dwDSLATS;
	int m_iDSCount;
	void ClientKilledHandler(int iAttackerH, char cAttackerType, short sDamage);
};

#define DEF_MAXCLIENTS				2000

struct Clients {
	typedef CClient *value_type;
	typedef value_type &ref_type;

	ref_type operator[](size_t index) {
		return m_pClientList[index];
	}
private:
	std::array<value_type, DEF_MAXCLIENTS> m_pClientList;
};