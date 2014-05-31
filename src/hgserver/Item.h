#pragma once
#include <stdint.h>

#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0	
#define DEF_EQUIPPOS_HEAD		1	//66Ah	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD]
#define DEF_EQUIPPOS_BODY		2	//66Ch	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY]
#define DEF_EQUIPPOS_ARMS		3	//66Eh	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS]
#define DEF_EQUIPPOS_PANTS		4	//670h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS]
#define DEF_EQUIPPOS_LEGGINGS	5	//672h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS]
#define DEF_EQUIPPOS_NECK		6   //674h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK]
#define DEF_EQUIPPOS_LHAND		7	//676h  m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND]
#define DEF_EQUIPPOS_RHAND		8	//678h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]
#define DEF_EQUIPPOS_TWOHAND	9	//67Ah	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]
#define DEF_EQUIPPOS_RFINGER	10	//67Ch
#define DEF_EQUIPPOS_LFINGER	11	//67Eh
#define DEF_EQUIPPOS_BACK		12  //680h
#define DEF_EQUIPPOS_RELEASEALL	13	//682h

#define DEF_ITEMTYPE_NOTUSED	-1	
#define DEF_ITEMTYPE_NONE		 0
#define DEF_ITEMTYPE_EQUIP		 1
#define DEF_ITEMTYPE_APPLY		 2
#define DEF_ITEMTYPE_USE_DEPLETE 3       
#define DEF_ITEMTYPE_INSTALL	 4
#define DEF_ITEMTYPE_CONSUME	 5
#define DEF_ITEMTYPE_ARROW		 6
#define DEF_ITEMTYPE_EAT		 7
#define DEF_ITEMTYPE_USE_SKILL   8
#define DEF_ITEMTYPE_USE_PERM    9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12


#define DEF_ITEMEFFECTTYPE_NONE				0
#define DEF_ITEMEFFECTTYPE_ATTACK			1		// ���ġ: value1 D value2 + value3
#define DEF_ITEMEFFECTTYPE_DEFENSE			2		// ���ɷ� 
#define DEF_ITEMEFFECTTYPE_ATTACK_ARROW		3		
#define DEF_ITEMEFFECTTYPE_HP   		    4		// HP��� ȿ�� 
#define DEF_ITEMEFFECTTYPE_MP   		    5		// MP��� ȿ�� 
#define DEF_ITEMEFFECTTYPE_SP   		    6		// SP��� ȿ�� 
#define DEF_ITEMEFFECTTYPE_HPSTOCK 		    7		
#define DEF_ITEMEFFECTTYPE_GET			    8		
#define DEF_ITEMEFFECTTYPE_STUDYSKILL		9		// ����� ���� �������̴�.
#define DEF_ITEMEFFECTTYPE_SHOWLOCATION		10		// ��ġ�� ǥ���ϴ� ������. 
#define DEF_ITEMEFFECTTYPE_MAGIC			11		// ����ϸ� ���� ȿ�� ��� ������ 
#define DEF_ITEMEFFECTTYPE_CHANGEATTR		12		// �÷��̾��� �Ӽ��� ��ȯ��Ų��. ����, �Ӹ����, �Ǻλ�, ��Ÿ�� 
#define DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE	13		// ���ȿ��� ���Ҿ� ������ ��� ȿ�� �ִ�.(������)
#define DEF_ITEMEFFECTTYPE_ADDEFFECT	    14		// �߰� ȿ��
#define DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE	15		// �������� ���� ����� ������� ���Ѵ�. ������ ����̷� 
#define DEF_ITEMEFFECTTYPE_OCCUPYFLAG		16		// ���� ��� 
#define DEF_ITEMEFFECTTYPE_DYE				17		// ������. 
#define DEF_ITEMEFFECTTYPE_STUDYMAGIC		18		// ������ ���� �������̴�.
#define DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19		// ��� ȿ��� �Բ� �ִ�  HP �� HP ȸ������ �������� ������ 
#define DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE	20		// ��� ȿ��� �Բ� ���ʰ� ���� ���� �� ȿ�� �ִ� ������ 
#define DEF_ITEMEFFECTTYPE_MATERIAL_ATTR	21		// ���� ���� ��� �Ӽ� 
#define DEF_ITEMEFFECTTYPE_FIRMSTAMINAR		22		// ���¹̳� ���� ������ 
#define DEF_ITEMEFFECTTYPE_LOTTERY			23		// ���� ������
#define DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY		24	// ��� ������ Ư�� ȿ�� 
#define DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25	// �� ������ Ư�� ȿ�� 
#define DEF_ITEMEFFECTTYPE_ALTERITEMDROP		26	// �ٸ� ������ ������ �� ��� �� �������� �������.
#define DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT		27	// ���� ŰƮ
#define DEF_ITEMEFFECTTYPE_WARM				28		// Unfreeze pot ?
#define DEF_ITEMEFFECTTYPE_FARMING			30
#define DEF_ITEMEFFECTTYPE_SLATES			31
#define DEF_ITEMEFFECTTYPE_ARMORDYE			32
#define DEF_ITEMEFFECTTYPE_ADDBALLPOINTS 33
#define DEF_ITEMEFFECTTYPE_REPPLUS 34
#define DEF_ITET_UNIQUE_OWNER				1		// ItemTouchEffect: ������ ���� ������ 
#define DEF_ITET_ID							2		// �׳� ������ ���̵�
#define DEF_ITET_DATE						3		// ��밡���� ��¥�� ������ ������ 

class CItem {
public:
	CItem();

	char m_cName[21];

	short m_sIDnum; // �������� ���� ��ȣ
	char m_cItemType;
	char m_cEquipPos;
	short m_sItemEffectType;
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3;
	short m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6;
	uint16_t m_wMaxLifeSpan;
	short m_sSpecialEffect;

	short m_sSpecialEffectValue1, m_sSpecialEffectValue2;

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
	short m_sTouchEffectType;
	short m_sTouchEffectValue1, m_sTouchEffectValue2, m_sTouchEffectValue3;
	char m_cItemColor;
	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
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