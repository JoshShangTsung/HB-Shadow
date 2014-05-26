#include "TeleportLoc.h"
#include <cstring>
CTeleportLoc::CTeleportLoc() {

	std::memset(m_cDestMapName, 0, sizeof(m_cDestMapName));
	//std::memset(m_cDestMapName2, 0, sizeof(m_cDestMapName2));
	m_sSrcX = -1;
	m_sSrcY = -1;
	m_sDestX = -1;
	m_sDestY = -1;
	m_sDestX2 = -1;
	m_sDestY2 = -1;

	m_iV1 = NULL;
	m_iV2 = NULL;
	m_dwTime = NULL;
	m_dwTime2 = NULL;

}
