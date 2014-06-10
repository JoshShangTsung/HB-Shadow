#pragma once
#include <stdint.h>

#define DEF_MAXITEMEQUIPPOS		15

enum EquipPos {
	NONE, // 0
	HEAD, // 1
	BODY, // 2
	ARMS, // 3
	PANTS, // 4
	LEGGINGS, // 5
	NECK, // 6
	LHAND, // 7
	RHAND, // 8
	TWOHAND, // 9
	RFINGER, // 10
	LFINGER, // 11
	BACK, // 12
	RELEASEALL // 13
};

enum class ItemType {
	NOTUSED = -1,
	NONE = 0,
	EQUIP = 1,
	APPLY = 2,
	USE_DEPLETE = 3,
	INSTALL = 4,
	CONSUME = 5,
	ARROW = 6,
	EAT = 7,
	USE_SKILL = 8,
	USE_PERM = 9,
	USE_SKILL_ENABLEDIALOGBOX = 10,
	USE_DEPLETE_DEST = 11,
	MATERIAL = 12
};

enum class ItemEffectType {
	NONE, // 0
	ATTACK, // 1
	DEFENSE, // 2
	ATTACK_ARROW, // 3
	HP, // 4
	MP, // 5
	SP, // 6
	HPSTOCK, // 7
	GET, // 8
	STUDYSKILL, // 9
	SHOWLOCATION, // 10
	MAGIC, // 11
	CHANGEATTR, // 12
	ATTACK_MANASAVE, // 13
	ADDEFFECT, // 14
	MAGICDAMAGESAVE, // 15
	OCCUPYFLAG, // 16
	DYE, // 17
	STUDYMAGIC, // 18
	ATTACK_MAXHPDOWN, // 19
	ATTACK_DEFENSE, // 20
	MATERIAL_ATTR, // 21
	FIRMSTAMINAR, // 22
	LOTTERY, // 23
	ATTACK_SPECABLTY, // 24
	DEFENSE_SPECABLTY, // 25
	ALTERITEMDROP, // 26
	CONSTRUCTIONKIT, // 27
	WARM, // 28 Unfreeze pot ?
	FARMING, // 30
	SLATES, // 31
	ARMORDYE, // 32
	ADDBALLPOINTS, // 33
	REPPLUS // 34
};

enum class TouchEffectType {
	NONE = 0,
	UNIQUE_OWNER = 1,
	ID = 2,
	DATE = 3
};

class CItem {
public:
	CItem();
	int iGetItemWeight(int iCount) const;
	char m_cName[21];

	short m_sIDnum;
	ItemType m_cItemType;
	EquipPos m_cEquipPos;
	ItemEffectType m_sItemEffectType;
	short m_sItemEffectValue1;
	short m_sItemEffectValue2;
	short m_sItemEffectValue3;
	short m_sItemEffectValue4;
	short m_sItemEffectValue5;
	short m_sItemEffectValue6;
	uint16_t m_wMaxLifeSpan;
	short m_sSpecialEffect;

	short m_sSpecialEffectValue1;
	short m_sSpecialEffectValue2;

	short m_sSprite;
	short m_sSpriteFrame;

	char m_cApprValue;
	char m_cSpeed;

	uint32_t m_wPrice;
	uint16_t m_wWeight;
	short m_sLevelLimit;
	char m_cGenderLimit;

	short m_sRelatedSkill;

	char m_cCategory;
	bool m_bIsForSale;

	uint32_t m_dwCount;
	TouchEffectType m_sTouchEffectType;
	short m_sTouchEffectValue1;
	short m_sTouchEffectValue2;
	short m_sTouchEffectValue3;
	char m_cItemColor;
	short m_sItemSpecEffectValue1;
	short m_sItemSpecEffectValue2;
	short m_sItemSpecEffectValue3;
	uint16_t m_wCurLifeSpan;
	uint32_t m_dwAttribute;
	void RequestBallItem(int iClientH, char *pData, uint32_t dwMsgSize);
	bool bReadBallSystemConfigFile(char * cFn);
};

class CBallSystem {
public:
	short m_sBS_ID;
	char m_cName[20];
	int m_iReqPoints;
};