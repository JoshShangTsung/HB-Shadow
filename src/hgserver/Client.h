// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__39CC7700_789F_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_CLIENT_H__39CC7700_789F_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

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
#define DEF_MAXGUILDSMAN	128 // �ִ� ���� �� 

#define	DEF_MAXMAGICTYPE	100	// �����Ϸ��� �α׼������뵵 �ٲپ�� �Ѵ�.
#define DEF_MAXSKILLTYPE	60

#define DEF_MAXPARTYMEMBERS	8

#define DEF_SPECABLTYTIMESEC	1200

class CClient  
{
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

	BOOL  m_bIsInitComplete;
	BOOL  m_bIsMsgSendAvailable;

	char  m_cMapName[11];
	char  m_cMapIndex;
	short m_sX, m_sY;
	
	char  m_cGuildName[21];		// ����� �̸� 
	char  m_cLocation[11];      //  <- ���̸��� �״�� ����ȴ�. �Ҽ� ���� 
	int   m_iGuildRank;			// -1�̸� ���ǹ�. 0�̸� ��� ������. ����� ��峻������ ���� 
	int   m_iGuildGUID;
	
	char  m_cDir;
	short m_sType;				// ���� Ÿ�� <- ����� �ÿ� ���Ѵ�. 
	short m_sOriginalType;		// �������� Ÿ��
	short m_sAppr1;
	short m_sAppr2;
	short m_sAppr3;
	short m_sAppr4;
	int   m_iApprColor;			// v1.4 ���� �÷����̺�
	int   m_iStatus;

	DWORD m_dwTime, m_dwHPTime, m_dwMPTime, m_dwSPTime, m_dwAutoSaveTime, m_dwHungerTime, m_dwWarmEffectTime;
	// Player Ư��ġ 

	char m_cSex, m_cSkin, m_cHairStyle, m_cHairColor, m_cUnderwear;

	int  m_iAngelicStr; // By Snoopy81
	int  m_iAngelicInt; // By Snoopy81
	int  m_iAngelicDex; // By Snoopy81
	int  m_iAngelicMag; // By Snoopy81

	int  m_iHP;						// Hit Point
	int  m_iHPstock;
	int  m_iMP;
	int  m_iSP;
	int  m_iExp;
	int m_iNextLevelExp;
	BOOL m_bIsKilled;

	int  m_iDefenseRatio;		// Defense Ratio
	int  m_iHitRatio;			// Hit Ratio

	// v1.432 ������� �ʴ´�.
	//int  m_iHitRatio_ItemEffect_SM; // ������ ������� ���� HitRatio ���氪
	//int  m_iHitRatio_ItemEffect_L;

	int  m_iDamageAbsorption_Armor[DEF_MAXITEMEQUIPPOS];		// ���� ������� ���� Damage ��� ȿ��
	int  m_iDamageAbsorption_Shield;	// Parrying ��� ������� Damage ��� ȿ�� 

	int  m_iLevel;
	int  m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma;
	//char m_cLU_Str, m_cLU_Int, m_cLU_Vit, m_cLU_Dex, m_cLU_Mag, m_cLU_Char;   // �������ÿ� �Ҵ�Ǿ� �ö󰡴� Ư��ġ��.
	int  m_iLuck; 
	int  m_iLU_Pool;
	char m_cAura;
	//MOG var - 3.51
	int m_iGizonItemUpgradeLeft;

	int m_iAddTransMana, m_iAddChargeCritical;

	int  m_iEnemyKillCount, m_iPKCount, m_iRewardGold;
	int  m_iCurWeightLoad;		// ���� �� ����ǰ ���� 

	char m_cSide;				// �÷��̾��� �� 
	
	BOOL m_bInhibition;

	char m_cAttackDiceThrow_SM;	// ���ġ �ֻ��� ������ ȸ�� 
	char m_cAttackDiceRange_SM;	// ���ġ �ֻ��� ���� 
	char m_cAttackDiceThrow_L;	// ���ġ �ֻ��� ������ ȸ�� 
	char m_cAttackDiceRange_L;	// ���ġ �ֻ��� ���� 
	char m_cAttackBonus_SM;		// ��� ���ʽ�
	char m_cAttackBonus_L;		// ��� ���ʽ�

	class CItem * m_pItemList[DEF_MAXITEMS];
	POINT m_ItemPosList[DEF_MAXITEMS];
	class CItem * m_pItemInBankList[DEF_MAXBANKITEMS];
	
	BOOL  m_bIsItemEquipped[DEF_MAXITEMS];
	short m_sItemEquipmentStatus[DEF_MAXITEMEQUIPPOS];
	char  m_cArrowIndex;		// �÷��̾ Ȱ�� ����Ҷ� ȭ�� ������ �ε���. �ʱⰪ�� -1(�Ҵ� �ȵ�)

	char           m_cMagicMastery[DEF_MAXMAGICTYPE];
	unsigned char  m_cSkillMastery[DEF_MAXSKILLTYPE]; // v1.4

	int   m_iSkillSSN[DEF_MAXSKILLTYPE];
	BOOL  m_bSkillUsingStatus[DEF_MAXSKILLTYPE];
	int   m_iSkillUsingTimeID[DEF_MAXSKILLTYPE]; //v1.12

	char  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	int   m_iWhisperPlayerIndex;
	char  m_cProfile[256];

	int   m_iHungerStatus;		// ����� ����Ʈ. �̰� 0�̵Ǹ� ���¹̳ʰ� ������ ������ ü�µ� ����̻� ���� �ʽ��ϴ�. 

	DWORD m_dwWarBeginTime;		// �� ���� �� ��ϵǴ� �ð�. 
	BOOL  m_bIsWarLocation;		// ���� �� �ִ����� ǥ�� 

	BOOL  m_bIsPoisoned;		// �ߵ��Ǿ������ ���� 
	int   m_iPoisonLevel;       // ���� ���� 
	DWORD m_dwPoisonTime;		// �ߵ� �ð�.
	
	int   m_iPenaltyBlockYear, m_iPenaltyBlockMonth, m_iPenaltyBlockDay; // v1.4

	//v1.4311-3 �߰� ���� ���� ������ ��ȣ�� ������ ������ �ð�
	int   m_iFightzoneNumber , m_iReserveTime, m_iFightZoneTicketNumber ; 

	class XSocket * m_pXSock;

	int   m_iAdminUserLevel;     // ���� ����. 0�̸� ��ȿ. ��ȣ�� �ö� ���� ������ Ŀ���.
	int   m_iRating;

	int   m_iTimeLeft_ShutUp;	 // �� ���� 0�� �ƴϸ� ä�ø޽����� ��޵��� �ʴ´�.
	int   m_iTimeLeft_Rating;	 // �ٸ� �÷��̾��� ������ �ű�� ���� ���� �ð�. 0�̸� ��� ����  
	int   m_iTimeLeft_ForceRecall;  // ���� ���ݵǱ� ���� �����ִ� �ð�ƽ 
	int   m_iTimeLeft_FirmStaminar; // ���¹̳ʰ� �޾� ������� �ʴ� �ð� �� 

	BOOL  m_bIsOnServerChange;     // �� ���� Ȱ��ȭ �Ǿ� ������ ������ ������ ���� �� ī������ ���� �ʴ´�.

	int   m_iExpStock;			 // �׿��ִ� ����ġ 
	DWORD m_dwExpStockTime;		 // ExpStock ��� �ð�.

	int   m_iAutoExpAmount;		 // Auto-Exp �ð� ���� ���� ����ġ 
	DWORD m_dwAutoExpTime;		 // Auto-Exp ��� �ð�.

	DWORD m_dwRecentAttackTime;  // ���� �ֱٿ� ����ߴ� �ð� 

	int   m_iAllocatedFish;		 // �� ���� 0�� �ƴϸ� �̺�Ʈ ���ø���� �̾߱��. 
	int   m_iFishChance;		 // ���� ���� ���� 
	
	char  m_cIPaddress[21];		 // Ŭ���̾�Ʈ�� IP address
	BOOL  m_bIsSafeAttackMode;

	BOOL  m_bIsOnWaitingProcess; // �ڷ���Ʈ�� ó���� ��ٸ��� ���¶�� 
	
	int   m_iSuperAttackLeft;	 // v1.2 �ʻ�� ��� ���� Ƚ�� 
	int   m_iSuperAttackCount;   // v1.2 �ʻ�� ��� ���� ī��Ʈ. �� ī��Ʈ�� �� ���� �ʻ�� ��� Ƚ���� �þ��. 

	short m_sUsingWeaponSkill;	 // v1.2 ���� ����ϴ� ������ ��ų �ε��� 

	int   m_iManaSaveRatio;		 // v1.2 ���� ��� ����Ʈ 
	
	BOOL  m_bIsLuckyEffect;		 // v1.2 ��� ȿ�� 
	int   m_iSideEffect_MaxHPdown; // v1.4 �ִ� HP �� ȿ�� 

	int   m_iComboAttackCount;   // v1.3 ��Ÿ ��� ī��Ʈ 
	int   m_iDownSkillIndex;	 // v1.3 �ٸ� ��ų�� �ö󰥶� ���� ��ų �ε��� 

	int   m_iMagicDamageSaveItemIndex; // v1.3 ���� ��� ����� �� ������ �ε��� (������ -1)

	short m_sCharIDnum1, m_sCharIDnum2, m_sCharIDnum3; // v1.3 �� ĳ���Ͱ� ���� ������!

	int   m_iAbuseCount;		// ��ŷ ������ �ľǿ� 
	
	BOOL  m_bIsBWMonitor;		// BadWord ������ΰ�?

	//BOOL  m_bIsExchangeMode;		// ���� ������ ��ȯ ����ΰ�? 
	//int   m_iExchangeH;				// ��ȯ�� ����� �ε��� 
	//char  m_cExchangeName[11];		// ��ȯ�� ����� �̸� 
	//char  m_cExchangeItemName[21];	// ��ȯ�ϰ��� �ϴ� ������ �̸� 
	//char  m_cExchangeItemIndex;  // ��ȯ�� ������ �ε��� 
	//int   m_iExchangeItemAmount; // ��ȯ�� ������ ���� 
	//BOOL  m_bIsExchangeConfirm;  // ��ȯ Ȯ�� 

	BOOL  m_bIsExchangeMode;			// Is In Exchange Mode? 
	int   m_iExchangeH;					// Client ID to Exchanging with 
	char  m_cExchangeName[11];			// Name of Client to Exchanging with 
	char  m_cExchangeItemName[4][21];	// Name of Item to exchange 

	char  m_cExchangeItemIndex[4];		// ItemID to Exchange
	int   m_iExchangeItemAmount[4];		// Ammount to exchange with

	BOOL  m_bIsExchangeConfirm;			// Has the user hit confirm? 
	int	  iExchangeCount;				//Keeps track of items which are on list

	int   m_iQuest;				 // ���� �Ҵ�� Quest 
	int   m_iQuestID;			 // �Ҵ���� Quest�� ID�� 
	int   m_iAskedQuest;		 // ��� ��Ʈ 
	int   m_iCurQuestCount;		 // ���� ��Ʈ ���� 
	
	int   m_iQuestRewardType;	 // ��Ʈ �ذ�� ��ǰ ���� -> �������� ID���̴�.
	int   m_iQuestRewardAmount;	 // ��ǰ ���� 

	int   m_iContribution;		 // ���ÿ� ���� ���嵵. 

	BOOL  m_bQuestMatchFlag_Loc; // ��Ʈ ���� ������ �Ǵ��ϱ� ����.
	BOOL  m_bIsQuestCompleted;   // ��Ʈ�� �Ϸ�Ǿ�°�? 

	int   m_iCustomItemValue_Attack;
	int   m_iCustomItemValue_Defense;

	int   m_iMinAP_SM;			// Custom-Item�� ȿ��� ���� �ּ� AP
	int   m_iMinAP_L;

	int   m_iMaxAP_SM;			// Custom-Item�� ȿ��� ���� �ִ� AP
	int   m_iMaxAP_L;

	BOOL  m_bIsNeutral;			// v1.5 �߸����θ� �Ǻ��ϱ� ���� �÷���. ó�� �ӵ��� ���̱� �����̴�.
	BOOL  m_bIsObserverMode;	// v1.5 ����� ������� �Ǻ� 

	int   m_iSpecialEventID;	// Ư�� ��� ��Ȯ�ο� �÷��� 

	int   m_iSpecialWeaponEffectType;	// ��� ������ ȿ�� ����
	int   m_iSpecialWeaponEffectValue;	// ��� ������ ȿ�� ��
	// ��� ������ ȿ�� ����: 
	// 0-None 1-�ʻ�������߰� 2-�ߵ�ȿ�� 3-������  
	// 5-��ø�� 6-������ 7-������ 8-��ȭ�� 9-��빮����

	// v1.42
	int   m_iAddHP, m_iAddSP, m_iAddMP; 
	int   m_iAddAR, m_iAddPR, m_iAddDR;
	int   m_iAddMR, m_iAddAbsPD, m_iAddAbsMD; 
	int   m_iAddCD, m_iAddExp, m_iAddGold;		// ���� ���� ����� �����. ������ʹ� ���������� ���ȴ�.

	int   m_iAddResistMagic;					// v1.2 �߰� ���� ���� 
	int   m_iAddPhysicalDamage;					// v1.2 ���� ����� �߰� ����Ʈ 
	int   m_iAddMagicalDamage;	

	int   m_iAddAbsAir;							// �Ӽ��� ����� ���
	int   m_iAddAbsEarth;
	int   m_iAddAbsFire;
	int   m_iAddAbsWater;
	
	int   m_iLastDamage;

	int   m_iMoveMsgRecvCount, m_iAttackMsgRecvCount, m_iRunMsgRecvCount, m_iSkillMsgRecvCount;
	DWORD m_dwMoveLAT, m_dwRunLAT, m_dwAttackLAT;

	int   m_iSpecialAbilityTime;				// Ư�� �ɷ��� ����ϱ� ���ؼ��� �� ���� 0�� �Ǿ�� �Ѵ�. 
	BOOL  m_bIsSpecialAbilityEnabled;			// Ư�� �ɷ��� Ȱ��ȭ �� �����ΰ�?
	DWORD m_dwSpecialAbilityStartTime;			// Ư�� �ɷ��� ����ϱ� ������ �ð�
	int   m_iSpecialAbilityLastSec;				// Ư�� �ɷ� ���� �ð�.

	int   m_iSpecialAbilityType;				// �Ҵ�� Ư�� �ɷ� ����
												// �����
												// 0:�ɷ� ����  1:��ݽ� �� HP 50% ����  2:�õ� ȿ��  3: ���� ȿ��  4: �� ��ų  5:���� ����� ��ŭ�� HP�� ��´�.
												// �����
												// 50: ���� ���� 0�� ����. 51:�ش� ���� ����� ��ȿȭ  52: ��5�� ���� ����� ��ȿȭ
	int   m_iSpecialAbilityEquipPos;			// ���� ��� Ư��ȿ�� ���Ǵ� ������ �ǹ���.
	BOOL  m_bIsAdminCommandEnabled;
	int   m_iAlterItemDropIndex;				// ������ ��� �������� ������ �ε��� 

	int   m_iWarContribution;					// ���� ���嵵 

	DWORD m_dwSpeedHackCheckTime;				// �ӵ����� �˻� ��ƾ 
	int   m_iSpeedHackCheckExp;		
	DWORD m_dwLogoutHackCheck;

	DWORD m_dwInitCCTimeRcv;
	DWORD m_dwInitCCTime;

	char  m_cLockedMapName[11];					// ���� �� �̸�
	int   m_iLockedMapTime;						// �� ���� 0 �̻��̸� ���� �ڷ���Ʈ �ص� ���� ������ ����.

	int   m_iCrusadeDuty;						// ũ�缼�̵忡�� ���� ����: 1-�뺴. 2-�Ǽ���. 3-���ְ�
	DWORD m_dwCrusadeGUID;						// ũ�缼�̵� GUID
	DWORD m_dwHeldenianGUID;
	BOOL m_bInRecallImpossibleMap;

	// �� ��Ʈ���Ĵ� ���� ������ �����ϴ� ���̴�. �ѹ� ���� �� �� ��� ������ ������ ���� ������ ����Ѵ�.
	struct {
		char cType;
		char cSide;
		short sX, sY;
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iCSIsendPoint;

	char m_cSendingMapName[11];
	BOOL m_bIsSendingMapStatus;

	// ���ְ��� �Ǽ��� �� �ִ� ����Ʈ. �Ϲ� �÷��̾��� �ڽ��� �ൿ�� ���� �����̴�.
	int  m_iConstructionPoint;

	char m_cConstructMapName[11];
	int  m_iConstructLocX, m_iConstructLocY;
	
	// 2.06
	BOOL m_bIsPlayerCivil;
	BOOL m_bIsAttackModeChange;

	// New 06/05/2004
	// Party Stuff
	int m_iPartyID;
	int m_iPartyStatus;
	int m_iReqJoinPartyClientH;
	char m_cReqJoinPartyName[12];

	int   m_iPartyRank;										// Party�������� ��ġ. -1�̸� ���ǹ�. 1�̸� ��Ƽ ����. 12�� ��� 
	int   m_iPartyMemberCount;								// ��Ƽ �ο� ���ѿ� 
	int   m_iPartyGUID;										// v1.42 Party GUID
	struct {
	int  iIndex;
	char cName[11];
	} m_stPartyMemberName[DEF_MAXPARTYMEMBERS];

	// New 07/05/2004
	DWORD m_dwLastActionTime, m_dwLastDamageTime;
	int m_iDeadPenaltyTime;

	// New 16/05/2004
	char m_cWhisperPlayerName[11];
	BOOL m_bIsAdminOrderGoto;
	BOOL m_bIsInsideWarehouse;
	BOOL m_bIsInsideWizardTower;
	BOOL m_bIsInsideOwnTown;
	BOOL m_bIsOwnLocation;
	BOOL m_pIsProcessingAllowed;
	BOOL m_bIsClientConnected;

	// Updated 10/11/2004 - 24/05/2004
	char m_cHeroArmorBonus;

	// New 25/05/2004
	BOOL m_bIsBeingResurrected;

	DWORD m_dwFightzoneDeadTime;
	char m_cSaveCount;
	int m_iBallPoints;

	DWORD m_dwDSLAT, m_dwDSLATOld, m_dwDSLATS;
	int m_iDSCount;
};

#endif // !defined(AFX_CLIENT_H__39CC7700_789F_11D2_A8E6_00001C7030A6__INCLUDED_)

