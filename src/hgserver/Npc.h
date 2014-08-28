#pragma once

#include <windows.h>
#include "Magic.h"
#include <cstring>

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

	CNpc(const char * pName5) {
		std::memcpy(m_cName, pName5, 5);

		for (int i = 0; i < DEF_MAXWAYPOINTS; i++)
			m_iWayPointIndex[i] = -1;
	}

	char m_pMagicConfigList[100]{};

	char m_cNpcName[21]{};

	char m_sAreaSize = 0;

	char m_cName[6]{};
	char m_cMapIndex = 0;
	short m_sX = 0;
	short m_sY = 0;
	short m_dX = 0;
	short m_dY = 0;
	short m_vX = 0;
	short m_vY = 0;
	int m_tmp_iError = 0;
	RECT m_rcRandomArea{0, 0, 0, 0}; // MOVETYPE_RANDOMAREA

	char m_cDir = 0;
	char m_cAction = 0;
	char m_cTurn = 0;

	short m_sType = 0;
	short m_sOriginalType = 0;
	short m_sAppr2 = 0;
	int m_iStatus = 0;

	uint32_t m_dwTime = 0;
	uint32_t m_dwActionTime = 0;
	uint32_t m_dwHPupTime = 0;
	uint32_t m_dwMPupTime = 0;
	uint32_t m_dwDeadTime = 0;
	uint32_t m_dwRegenTime = 0;

	int m_iHP = 0; // Hit Point 
	int m_iExp = 0; // ExpDice

	int m_iHitDice = 0;
	int m_iDefenseRatio = 0; // Defense Ratio
	int m_iHitRatio = 0; // HitRatio
	int m_iMagicHitRatio = 0; // Ratio
	int m_iMinBravery = 0;
	int m_iExpDiceMin = 0;
	int m_iExpDiceMax = 0;
	int m_iGoldDiceMin = 0;
	int m_iGoldDiceMax = 0;

	char m_cSide = 0;
	char m_cActionLimit = 0;

	char m_cSize = 0; // 0: Small-Medium 1: Large
	char m_cAttackDiceThrow = 0;
	char m_cAttackDiceRange = 0;
	char m_cAttackBonus = 0;
	char m_cBravery = 0;
	char m_cResistMagic = 0;
	char m_cMagicLevel = 0;
	char m_cDayOfWeekLimit = 0;
	char m_cChatMsgPresence = 0;
	int m_iMana = 0; // MagicLevel*30
	int m_iMaxMana = 0;

	char m_cMoveType = 0;
	char m_cBehavior = 0;
	short m_sBehaviorTurnCount = 0;
	char m_cTargetSearchRange = 0;

	int m_iFollowOwnerIndex = 0;
	char m_cFollowOwnerType = 0;
	bool m_bIsSummoned = false;
	uint32_t m_dwSummonedTime = 0;

	int m_iTargetIndex = 0;
	char m_cTargetType = 0;
	char m_cCurWaypoint = 0;
	char m_cTotalWaypoint = 0;

	int m_iSpotMobIndex = 0; // spot-mob-generator
	int m_iWayPointIndex[DEF_MAXWAYPOINTS + 1]{};
	char m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS]{};

	bool m_bIsPermAttackMode = false;
	int m_iNoDieRemainExp = 0;
	int m_iAttackStrategy = 0;
	int m_iAILevel = 0;

	int m_iAttackRange = 1;
	int m_iAttackCount = 0;
	bool m_bIsKilled = false;
	bool m_bIsUnsummoned = false;

	int m_iLastDamage = 0;
	int m_iSummonControlMode = 0; // 0?? Free, 1 Hold 2 Tgt 
	char m_cAttribute = 0;
	int m_iAbsDamage = 0;

	int m_iItemRatio = 0;
	int m_iAssignedItem = 0;

	char m_cSpecialAbility = 0;

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

	int m_iBuildCount = 0;
	int m_iManaStock = 0;
	bool m_bIsMaster = false;
	int m_iGuildGUID = 0;

	char m_cCropType = 0;
	char m_cCropSkill = 0;

	int m_iV1 = 0;
	char m_cArea = 0;

	int m_iNpcItemType = 0;
	int m_iNpcItemMax = 0;
};