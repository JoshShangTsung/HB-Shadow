#pragma once
#include <cstdint>

#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0
#define DEF_EQUIPPOS_HEAD		1
#define DEF_EQUIPPOS_BODY		2
#define DEF_EQUIPPOS_ARMS		3
#define DEF_EQUIPPOS_PANTS		4
#define DEF_EQUIPPOS_BOOTS		5
#define DEF_EQUIPPOS_NECK		6
#define DEF_EQUIPPOS_LHAND		7
#define DEF_EQUIPPOS_RHAND		8
#define DEF_EQUIPPOS_TWOHAND	9
#define DEF_EQUIPPOS_RFINGER	10
#define DEF_EQUIPPOS_LFINGER	11
#define DEF_EQUIPPOS_BACK		12
#define DEF_EQUIPPOS_FULLBODY	13

#define DEF_ITEMTYPE_NONE			0
#define DEF_ITEMTYPE_EQUIP			1
#define DEF_ITEMTYPE_APPLY			2
#define DEF_ITEMTYPE_USE_DEPLETE	3
#define DEF_ITEMTYPE_INSTALL		4
#define DEF_ITEMTYPE_CONSUME		5
#define DEF_ITEMTYPE_ARROW			6
#define DEF_ITEMTYPE_EAT			7
#define DEF_ITEMTYPE_USE_SKILL		8
#define DEF_ITEMTYPE_USE_PERM		9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12

class CItem {
public:
	CItem();
	char m_cName[21];
	char m_cItemType;
	char m_cEquipPos;
	char m_cItemColor;
	char m_cSpeed;
	char m_cGenderLimit;
	//	short m_sItemEffectType;
	short m_sLevelLimit;
	short m_sSprite;
	short m_sSpriteFrame;
	short m_sX;
	short m_sY;
	short m_sItemSpecEffectValue1;
	short m_sItemSpecEffectValue2;
	short m_sItemSpecEffectValue3;
	short m_sItemEffectValue1;
	short m_sItemEffectValue2;
	short m_sItemEffectValue3;
	short m_sItemEffectValue4;
	short m_sItemEffectValue5;
	short m_sItemEffectValue6;
	uint16_t m_wCurLifeSpan;
	uint16_t m_wMaxLifeSpan;
	uint16_t m_wPrice;
	uint16_t m_wWeight;
	uint32_t m_dwCount;
	uint32_t m_dwAttribute;
};
