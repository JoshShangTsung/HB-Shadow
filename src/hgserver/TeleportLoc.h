#pragma once
#include <cstdint>

class CTeleportLoc {
public:
	short m_sSrcX = -1;
	short m_sSrcY = -1;

	char m_cDestMapName[11]{};
	char m_cDir = 0;
	char m_cDestMapName2[11]{};
	short m_sDestX = -1;
	short m_sDestY = -1;
	short m_sDestX2 = -1;
	short m_sDestY2 = -1;

	int m_iV1 = 0;
	int m_iV2 = 0;
	uint32_t m_dwTime = 0;
	uint32_t m_dwTime2 = 0;

};
