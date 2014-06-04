#pragma once
#include <stdint.h>

class CTeleportLoc {
public:
	CTeleportLoc();

	short m_sSrcX;
	short m_sSrcY;

	char m_cDestMapName[11], m_cDir;
	char m_cDestMapName2[11];
	short m_sDestX;
	short m_sDestY;
	short m_sDestX2;
	short m_sDestY2;

	int m_iV1;
	int m_iV2;
	uint32_t m_dwTime;
	uint32_t m_dwTime2;

};
