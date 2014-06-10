#pragma once
#include <stdint.h>

enum MagicType {
	DAMAGE_SPOT = 1,
	HPUP_SPOT, // 2
	DAMAGE_AREA, // 3
	SPDOWN_SPOT, // 4
	SPDOWN_AREA, // 5
	SPUP_SPOT, // 6
	SPUP_AREA, // 7
	TELEPORT, // 8
	SUMMON, // 9
	CREATE, // 10
	PROTECT, // 11
	HOLDOBJECT, // 12
	INVISIBILITY, // 13
	CREATE_DYNAMIC, // 14
	POSSESSION, // 15
	CONFUSE, // 16
	POISON, // 17
	BERSERK, // 18
	DAMAGE_LINEAR, // 19
	POLYMORPH, // 20
	DAMAGE_AREA_NOSPOT, // 21
	TREMOR, // 22
	ICE, // 23
	PURITYLIFE_SPOT, // 24
	DAMAGE_AREA_NOSPOT_SPDOWN, // 25
	ICE_LINEAR, // 26
	DAMAGE_AREA_ARMOR_BREAK, // 27
	CANCELLATION, // 28
	DAMAGE_LINEAR_SPDOWN, // 29
	INHIBITION, // 30
	RESURRECTION, // 31
	SCAN // 32
};
/*
#define DEF_MAGICTYPE_DAMAGE_SPOT				1
#define DEF_MAGICTYPE_HPUP_SPOT					2
#define DEF_MAGICTYPE_DAMAGE_AREA				3
#define DEF_MAGICTYPE_SPDOWN_SPOT				4
#define DEF_MAGICTYPE_SPDOWN_AREA				5
#define DEF_MAGICTYPE_SPUP_SPOT					6
#define DEF_MAGICTYPE_SPUP_AREA					7
#define DEF_MAGICTYPE_TELEPORT					8
#define DEF_MAGICTYPE_SUMMON					9
#define DEF_MAGICTYPE_CREATE					10
#define DEF_MAGICTYPE_PROTECT					11
#define DEF_MAGICTYPE_HOLDOBJECT				12
#define DEF_MAGICTYPE_INVISIBILITY				13
#define DEF_MAGICTYPE_CREATE_DYNAMIC			14
#define DEF_MAGICTYPE_POSSESSION				15
#define DEF_MAGICTYPE_CONFUSE					16 // Mass-Illusion, Illusion, Illusion-Movement, Mass-Illusion-Movement
#define DEF_MAGICTYPE_POISON					17
#define DEF_MAGICTYPE_BERSERK					18
#define DEF_MAGICTYPE_DAMAGE_LINEAR				19
#define DEF_MAGICTYPE_POLYMORPH					20
#define DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT		21 // Meteor-Strike, Mass-Magic-Missile
#define DEF_MAGICTYPE_TREMOR					22
#define DEF_MAGICTYPE_ICE						23
#define DEF_MAGICTYPE_PURITYLIFE_SPOT            24 // purity
// 24
#define DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN	25
#define DEF_MAGICTYPE_ICE_LINEAR				26 // Blizzard
// 27
#define DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK  28
#define DEF_MAGICTYPE_CANCELLATION				29
#define DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN		30 // Earth-Shock-Wave
#define DEF_MAGICTYPE_INHIBITION				31 // Inhibition-Casting
#define DEF_MAGICTYPE_RESURRECTION				32 // Resurrection
#define DEF_MAGICTYPE_SCAN						33 // Scan
*/
#define DEF_MAXMAGICEFFECTS			100

class CMagic {
public:
	CMagic();

	char m_cName[31];

	MagicType m_sType;
	uint32_t m_dwDelayTime;
	uint32_t m_dwLastTime;
	short m_sValue1;
	short m_sValue2;
	short m_sValue3;
	short m_sValue4;
	short m_sValue5;
	short m_sValue6;
	short m_sValue7;
	short m_sValue8;
	short m_sValue9;
	short m_sValue10;
	short m_sValue11;
	short m_sValue12;
	short m_sIntLimit;
	int m_iGoldCost;

	char m_cCategory;
	int m_iAttribute;
};
