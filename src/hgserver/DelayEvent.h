#pragma once
#include <cstdint>
#include <set>
#include <functional>

#define DEF_DELAYEVENTTYPE_DAMAGEOBJECT				1
#define DEF_DELAYEVENTTYPE_MAGICRELEASE				2
#define DEF_DELAYEVENTTYPE_USEITEM_SKILL			3
#define DEF_DELAYEVENTTYPE_METEORSTRIKE				4
#define DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE		5
#define DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT	6
#define DEF_DELAYEVENTTYPE_ANCIENT_TABLET			7

struct DelayEvent {
	int delayType_;
	int effectType_;

	char mapIndex_;
	int x_;
	int y_;

	int m_iTargetH;
	char m_cTargetType;
	int v1_;
	int v2_;
	int v3_;

	uint32_t m_dwTriggerTime;
};

bool operator<(const DelayEvent &a, const DelayEvent &b);

struct DelayEvents {

	bool remove(int iH, char cType, int iEffectType);
	void process(const std::function<void(const DelayEvent&)> &f);
	bool add(int iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	void reset();
private:
	std::multiset<DelayEvent> events_;
};