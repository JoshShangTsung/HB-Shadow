#pragma once
#include "Magic.h"
#include "Unit.h"
#include "Map.h"

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

struct Npcs;
struct CGame;
class CNpc: public Unit, public std::enable_shared_from_this<CNpc> {
public:
	CNpc(int id, CGame &game, const char * pName5);
	const int id_;
	CGame &game_;
	char m_pMagicConfigList[100];

	char m_cNpcName[21];

	char m_sAreaSize;

	char m_cName[6];
	MapPtr map_;
	short m_sX;
	short m_sY;
	short m_dX;
	short m_dY;
	short m_vX;
	short m_vY;
	int m_tmp_iError;
	RECT m_rcRandomArea;

	char m_cDir;
	char m_cAction;
	char m_cTurn;

	short m_sType;
	short m_sOriginalType;
	short m_sAppr2;
	int m_iStatus;

	uint32_t m_dwTime;
	uint32_t m_dwActionTime;
	uint32_t m_dwHPupTime;
	uint32_t m_dwMPupTime;
	uint32_t m_dwDeadTime;
	uint32_t m_dwRegenTime;

	int m_iHP; // Hit Point
	int m_iExp;

	int m_iHitDice;
	int m_iDefenseRatio; // Defense Ratio
	int m_iHitRatio; // HitRatio
	int m_iMagicHitRatio;
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
	int m_iMana;
	int m_iMaxMana;

	char m_cMoveType;
	char m_cBehavior;
	short m_sBehaviorTurnCount;
	char m_cTargetSearchRange;

	int m_iFollowOwnerIndex;
	char m_cFollowOwnerType;
	bool m_bIsSummoned;
	uint32_t m_dwSummonedTime;

	int m_iTargetIndex;
	char m_cTargetType;
	char m_cCurWaypoint;
	char m_cTotalWaypoint;

	int m_iSpotMobIndex;
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
	int m_iSummonControlMode;
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
	bool markedForDeletion_ = false;

	void RemoveEventNpc();
	void _NpcBehavior_GrandMagicGenerator();
	bool _bNpcBehavior_Detector();
	bool _bNpcBehavior_ManaCollector();
	bool bCheckEnergySphereDestination(short sAttackerH, char cAttackerType);
	void NpcDeadItemGenerator(short sAttackerH, char cAttackerType);
	void NpcRequestAssistance();
	int iGetNpcRelationship_SendEvent(int iOpponentH);
	void NpcMagicHandler(short dX, short dY, short sType);
	void NpcBehavior_Stop();
	void DeleteNpc();
	void CalcNextWayPointDestination();
	void NpcBehavior_Flee();
	int iGetDangerValue(short dX, short dY);
	void NpcBehavior_Dead();
	void NpcKilledHandler(short sAttackerH, char cAttackerType, short sDamage);
	void NpcBehavior_Attack();
	void TargetSearch(short * pTarget, char * pTargetType);
	void NpcBehavior_Move();
	void SetIceFlag(bool bStatus);
	void SetProtectionFromArrowFlag(bool bStatus);
	void SetMagicProtectionFlag(bool bStatus);
	void SetDefenseShieldFlag(bool bStatus);
	void SetInvisibilityFlag(bool bStatus);
	void SetPoisonFlag(bool bStatus);
	void SetBerserkFlag(bool bStatus);
	void RemoveFromTarget(int iCode);
	bool bCheckResistingPoisonSuccess();
	bool _bItemLog(int iAction, char * cName, class CItem * pItem);
private:
	UnitPtr _getPtr() override;
	CMap &_getMap() override;
	int _getEffectiveIceResist() override;
	void _sendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3) override;
};


#define DEF_MAXNPCS					15000
struct Npcs {
	typedef std::shared_ptr<CNpc> value_type;
	typedef value_type& ref_type;
	ref_type operator[](size_t index) {
		return m_pNpcList[index];
	}
	void clear() {
		m_pNpcList = {{}};
	}
	void cull() {
		for (auto &ptr: m_pNpcList) {
			if (ptr && ptr->markedForDeletion_) {
				ptr.reset();
			}
		}
	}
private:
	typedef std::array<value_type, DEF_MAXNPCS> List;
	List m_pNpcList;
};