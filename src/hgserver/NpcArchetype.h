#pragma once
#include <cstdint>
#include "Map.h"

struct NpcArchetype {
	char m_cNpcName[21];
	short m_sType;
	int m_iHitDice;
	int m_iDefenseRatio;
	int m_iHitRatio;
	int m_iMinBravery;
	int m_iExpDiceMin;
	int m_iExpDiceMax;
	int m_iGoldDiceMin;
	int m_iGoldDiceMax;
	char m_cAttackDiceThrow;
	char m_cAttackDiceRange;
	char m_cSize;
	char m_cSide;
	char m_cActionLimit;
	uint32_t m_dwActionTime;
	char m_cResistMagic;
	char m_cMagicLevel;
	char m_cDayOfWeekLimit;
	char m_cChatMsgPresence;
	char m_cTargetSearchRange;
	uint32_t m_dwRegenTime;
	char m_cAttribute;
	int m_iAbsDamage;
	int m_iMaxMana;
	int m_iMagicHitRatio;
	int m_iAttackRange;
};
