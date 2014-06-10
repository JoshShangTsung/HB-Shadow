#pragma once
#include <stdint.h>

class CTeleportLoc {
public:
	CTeleportLoc();

	short m_sSrcX, m_sSrcY;

	char m_cDestMapName[11], m_cDir;
	char m_cDestMapName2[11];
	short m_sDestX, m_sDestY;
	short m_sDestX2, m_sDestY2;

	int m_iV1, m_iV2;
	uint32_t m_dwTime, m_dwTime2;

};
