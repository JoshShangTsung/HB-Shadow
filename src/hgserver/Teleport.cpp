#include "Teleport.h"
#include <cstring>
CTeleport::CTeleport() {

	std::memset(m_cTeleportNpcName, 0, sizeof(m_cTeleportNpcName));
	std::memset(m_cSourceMap, 0, sizeof(m_cSourceMap));
	std::memset(m_cTargetMap, 0, sizeof(m_cTargetMap));
	std::memset(m_cTeleportSide, 0, sizeof(m_cTeleportSide));

	m_sDestinationX = -1;
	m_sDestinationY = -1;
	m_sTeleportCost = -1;
	m_sTeleportMinLevel = 0;
	m_sTeleportMaxLevel = 0;
	m_bTeleportHunt = false;
	m_bTeleportNtrl = false;
	m_bTeleportCrmnl = false;
}
