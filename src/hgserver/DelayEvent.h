#pragma once
#include <windows.h>

#define DEF_DELAYEVENTTYPE_DAMAGEOBJECT				1
#define DEF_DELAYEVENTTYPE_MAGICRELEASE				2
#define DEF_DELAYEVENTTYPE_USEITEM_SKILL			3
#define DEF_DELAYEVENTTYPE_METEORSTRIKE				4
#define DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE		5
#define DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT	6
#define DEF_DELAYEVENTTYPE_ANCIENT_TABLET			7

class CDelayEvent {
public:
	int m_iDelayType = 0;
	int m_iEffectType = 0;

	char m_cMapIndex = 0;
	int m_dX = 0;
	int m_dY = 0;

	int m_iTargetH = 0;
	char m_cTargetType = 0;
	int m_iV1 = 0;
	int m_iV2 = 0;
	int m_iV3 = 0;

	uint32_t m_dwTriggerTime = 0;
};
