#pragma once

#include <windows.h>
#include "Magic.h"

#define DEF_MAXWAYPOINTS			10

#define DEF_MOVETYPE_STOP			0	
#define DEF_MOVETYPE_SEQWAYPOINT	1		
#define DEF_MOVETYPE_RANDOMWAYPOINT	2		
#define DEF_MOVETYPE_FOLLOW			3		
#define DEF_MOVETYPE_RANDOMAREA		4		
#define DEF_MOVETYPE_RANDOM			5		
#define DEF_MOVETYPE_GUARD			6		

#define DEF_BEHAVIOR_STOP			0
#define DEF_BEHAVIOR_MOVE			1
#define DEF_BEHAVIOR_ATTACK			2
#define DEF_BEHAVIOR_FLEE			3
#define DEF_BEHAVIOR_DEAD			4

class CNpc {
public:
	CNpc(const char * pName5);

	char m_pMagicConfigList[100];

	char m_cNpcName[21];

	char m_sAreaSize = 0;

	char m_cName[6];
	char m_cMapIndex;
	short m_sX, m_sY;
	short m_dX, m_dY;
	short m_vX, m_vY;
	int m_tmp_iError;
	RECT m_rcRandomArea; // MOVETYPE_RANDOMAREA

	char m_cDir;
	char m_cAction;
	char m_cTurn;

	short m_sType;
	short m_sOriginalType;
	short m_sAppr2;
	int m_iStatus;

	DWORD m_dwTime;
	DWORD m_dwActionTime;
	DWORD m_dwHPupTime, m_dwMPupTime;
	DWORD m_dwDeadTime, m_dwRegenTime;

	int m_iHP; // Hit Point 
	int m_iExp; // ExpDice

	int m_iHitDice;
	int m_iDefenseRatio; // Defense Ratio
	int m_iHitRatio; // HitRatio
	int m_iMagicHitRatio; // Ratio
	int m_iMinBravery;
	int m_iExpDiceMin;
	int m_iExpDiceMax;
	int m_iGoldDiceMin;
	int m_iGoldDiceMax;

	char m_cSide;
	char m_cActionLimit;

	char m_cSize; // 0: Small-Medium 1: Large
	char m_cAttackDiceThrow;
	char m_cAttackDiceRange;
	char m_cAttackBonus;
	char m_cBravery;
	char m_cResistMagic;
	char m_cMagicLevel;
	char m_cDayOfWeekLimit;
	char m_cChatMsgPresence;
	int m_iMana; // MagicLevel*30
	int m_iMaxMana;

	char m_cMoveType;
	char m_cBehavior;
	short m_sBehaviorTurnCount;
	char m_cTargetSearchRange;

	int m_iFollowOwnerIndex;
	char m_cFollowOwnerType;
	bool m_bIsSummoned;
	DWORD m_dwSummonedTime;

	int m_iTargetIndex;
	char m_cTargetType;
	char m_cCurWaypoint;
	char m_cTotalWaypoint;

	int m_iSpotMobIndex; // spot-mob-generator
	int m_iWayPointIndex[DEF_MAXWAYPOINTS + 1];
	char m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	bool m_bIsPermAttackMode; 
	int m_iNoDieRemainExp;
	int m_iAttackStrategy;
	int m_iAILevel;

	int m_iAttackRange;
	int m_iAttackCount;
	bool m_bIsKilled;
	bool m_bIsUnsummoned;

	int m_iLastDamage;
	int m_iSummonControlMode; // 0?? Free, 1 Hold 2 Tgt 
	char m_cAttribute;
	int m_iAbsDamage;

	int m_iItemRatio;
	int m_iAssignedItem;

	char m_cSpecialAbility;

	/*
	case 0: break;
	case 1:  "Penetrating Invisibility"
	case 2:  "Breaking Magic Protection"
	case 3:  "Absorbing Physical Damage"
	case 4:  "Absorbing Magical Damage"
	case 5:  "Poisonous"
	case 6:  "Extremely Poisonous"
	case 7:  "Explosive"
	case 8:  "Hi-Explosive" 
	 */

	int m_iBuildCount; 
	int m_iManaStock; 
	bool m_bIsMaster;
	int m_iGuildGUID;

	char m_cCropType;
	char m_cCropSkill;

	int m_iV1;
	char m_cArea;

	int m_iNpcItemType;
	int m_iNpcItemMax;
};