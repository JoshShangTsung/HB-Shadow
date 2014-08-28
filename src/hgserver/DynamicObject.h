#pragma once
#include <cstdint>

class CDynamicObject {
public:

	CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1) {
		m_sOwner = sOwner;
		m_cOwnerType = cOwnerType;

		m_sType = sType;
		m_cMapIndex = cMapIndex;
		m_sX = sX;
		m_sY = sY;

		m_dwRegisterTime = dwRegisterTime;
		m_dwLastTime = dwLastTime;

		m_iCount = 0;
		m_iV1 = iV1;
	}
	short m_sOwner = 0;
	char m_cOwnerType = 0;

	short m_sType = 0;
	char m_cMapIndex = 0;
	short m_sX = 0;
	short m_sY = 0;
	uint32_t m_dwRegisterTime = 0;
	uint32_t m_dwLastTime = 0;

	int m_iCount = 0;
	int m_iV1 = 0;
};
