#pragma once
#include <cstdint>

#define DEF_MAXDELAYEVENTS		60000

#define DEF_DELAYEVENTTYPE_DAMAGEOBJECT				1
#define DEF_DELAYEVENTTYPE_MAGICRELEASE				2
#define DEF_DELAYEVENTTYPE_USEITEM_SKILL			3
#define DEF_DELAYEVENTTYPE_METEORSTRIKE				4
#define DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE		5
#define DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT	6
#define DEF_DELAYEVENTTYPE_ANCIENT_TABLET			7

class CDelayEvent {
public:
	int m_iDelayType;
	int m_iEffectType;

	char m_cMapIndex;
	int m_dX, m_dY;

	int m_iTargetH;
	char m_cTargetType;
	int m_iV1, m_iV2, m_iV3;

	uint32_t m_dwTriggerTime;
};

struct DelayEvents {

	bool bRemoveFromDelayEventList(int iH, char cType, int iEffectType) {
		register int i;

		for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
			if (m_pDelayEventList[i] != nullptr) {

				if (iEffectType == 0) {

					if ((m_pDelayEventList[i]->m_iTargetH == iH) && (m_pDelayEventList[i]->m_cTargetType == cType)) {
						delete m_pDelayEventList[i];
						m_pDelayEventList[i] = nullptr;
					}
				} else {

					if ((m_pDelayEventList[i]->m_iTargetH == iH) && (m_pDelayEventList[i]->m_cTargetType == cType) &&
							  (m_pDelayEventList[i]->m_iEffectType == iEffectType)) {
						delete m_pDelayEventList[i];
						m_pDelayEventList[i] = nullptr;
					}
				}
			}

		return true;
	}

	void DelayEventProcessor(const std::function<void(CDelayEvent&)> &f) {
		uint32_t dwTime = timeGetTime();

		for (int i = 0; i < DEF_MAXDELAYEVENTS; i++)
			if ((m_pDelayEventList[i] != nullptr) && (m_pDelayEventList[i]->m_dwTriggerTime < dwTime)) {
				f(*m_pDelayEventList[i]);
				delete m_pDelayEventList[i];
				m_pDelayEventList[i] = nullptr;
			}
	}

	bool bRegisterDelayEvent(int iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3) {
		register int i;

		for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
			if (m_pDelayEventList[i] == nullptr) {
				m_pDelayEventList[i] = new class CDelayEvent;
				m_pDelayEventList[i]->m_iDelayType = iDelayType;
				m_pDelayEventList[i]->m_iEffectType = iEffectType;
				m_pDelayEventList[i]->m_cMapIndex = cMapIndex;
				m_pDelayEventList[i]->m_dX = dX;
				m_pDelayEventList[i]->m_dY = dY;
				m_pDelayEventList[i]->m_iTargetH = iTargetH;
				m_pDelayEventList[i]->m_cTargetType = cTargetType;
				m_pDelayEventList[i]->m_iV1 = iV1;
				m_pDelayEventList[i]->m_iV2 = iV2;
				m_pDelayEventList[i]->m_iV3 = iV3;
				m_pDelayEventList[i]->m_dwTriggerTime = dwLastTime;
				return true;
			}
		return false;
	}

	void reset() {
		for (int i = 0; i < DEF_MAXDELAYEVENTS; i++) {
			if (m_pDelayEventList[i] != nullptr) {
				delete m_pDelayEventList[i];
			}
		}
	}
private:
	class CDelayEvent * m_pDelayEventList[DEF_MAXDELAYEVENTS];
};