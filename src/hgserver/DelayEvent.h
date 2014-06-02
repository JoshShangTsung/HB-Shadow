#pragma once
#include <cstdint>
#include <set>
#include <functional>

enum class DelayEventType {
	DAMAGEOBJECT,
	MAGICRELEASE,
	USEITEM_SKILL,
	METEORSTRIKE,
	DOMETEORSTRIKEDAMAGE,
	CALCMETEORSTRIKEEFFECT,
	ANCIENT_TABLET
};

struct DelayEvent {
	DelayEventType delayType_;
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
	void process(const std::function<void(const DelayEvent&) > &f);
	bool add(DelayEventType iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	void clear();
private:
	std::multiset<DelayEvent> events_;
};