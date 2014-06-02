#include "DelayEvent.h"
#include <algorithm>
#include <windows.h>

bool operator<(const DelayEvent &a, const DelayEvent &b) {
	return a.m_dwTriggerTime < b.m_dwTriggerTime;
}

namespace {

	template<typename C, typename F> void erase(C &c, const F &f) {
		for (auto iter = c.begin(); iter != c.end();) {
			if (f(*iter)) {
				c.erase(iter++);
			} else {
				++iter;
			}
		}
	}
}

bool DelayEvents::remove(int iH, char cType, int iEffectType) {
	if (iEffectType == 0) {
		erase(events_, [iH, cType](const DelayEvent & ev) {
			return ev.m_iTargetH == iH && ev.m_cTargetType == cType;
		});
	} else {
		erase(events_, [iH, cType, iEffectType](const DelayEvent & ev) {
			return ev.m_iTargetH == iH && ev.m_cTargetType == cType && ev.effectType_ == iEffectType;
		});
	}
	return true;
}

void DelayEvents::process(const std::function<void(const DelayEvent&) > &f) {
	const uint32_t dwTime = timeGetTime();
	DelayEvent tmp;
	tmp.m_dwTriggerTime = dwTime;
	auto iter = events_.upper_bound(tmp);
	std::for_each(events_.begin(), iter, f);
	events_.erase(iter, events_.end());
}

bool DelayEvents::add(DelayEventType iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3) {
	DelayEvent ev;
	ev.delayType_ = iDelayType;
	ev.effectType_ = iEffectType;
	ev.mapIndex_ = cMapIndex;
	ev.x_ = dX;
	ev.y_ = dY;
	ev.m_iTargetH = iTargetH;
	ev.m_cTargetType = cTargetType;
	ev.v1_ = iV1;
	ev.v2_ = iV2;
	ev.v3_ = iV3;
	ev.m_dwTriggerTime = dwLastTime;
	events_.insert(ev);
	return true;
}

void DelayEvents::clear() {
	events_.clear();
}
