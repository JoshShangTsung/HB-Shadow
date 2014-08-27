#pragma once
#include <cstdint>

class CTile {
public:
	CTile();
	void Clear();

	uint32_t m_dwOwnerTime;
	uint32_t m_dwEffectTime;
	uint32_t m_dwDeadOwnerTime;
	uint32_t m_dwDynamicObjectTime;

	int m_iChatMsg;
	int m_cItemColor; // v1.4
	int m_iEffectType;
	int m_iDeadApprColor; // v1.4
	int m_iEffectFrame;
	int m_iEffectTotalFrame;
	int m_iApprColor; // v1.4
	int m_iDeadChatMsg;

	uint16_t m_wDeadObjectID;
	uint16_t m_wObjectID;

	short m_sOwnerType; // +B2C
	short m_sAppr1; // +B2E
	short m_sAppr2; // +B30
	short m_sAppr3; // +B32
	short m_sAppr4; // +B34
	int m_iStatus; // +B38

	short m_sDeadOwnerType; // +B3C
	short m_sDeadAppr1;
	short m_sDeadAppr2;
	short m_sDeadAppr3;
	short m_sDeadAppr4;

	int m_iDeadStatus;
	short m_sV1;
	short m_sV2;
	short m_sV3; // +B50
	short m_sDynamicObjectType;

	short m_sItemSprite;
	short m_sItemSpriteFrame;

	char m_cDeadOwnerFrame;
	char m_cOwnerAction; // +B59
	char m_cOwnerFrame; // +B5A
	char m_cDir;
	char m_cDeadDir;

	char m_cDynamicObjectFrame;
	char m_cDynamicObjectData1;
	char m_cDynamicObjectData2;
	char m_cDynamicObjectData3;
	char m_cDynamicObjectData4;
	char m_cOwnerName[12];
	char m_cDeadOwnerName[12];
};
