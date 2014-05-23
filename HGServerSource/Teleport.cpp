#include "Teleport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeleport::CTeleport()
{

	ZeroMemory(m_cTeleportNpcName, sizeof(m_cTeleportNpcName));
	ZeroMemory(m_cSourceMap, sizeof(m_cSourceMap));
	ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));
	ZeroMemory(m_cTeleportSide, sizeof(m_cTeleportSide));

	m_sDestinationX = -1;
	m_sDestinationY = -1;
	m_sTeleportCost = -1;
	m_sTeleportMinLevel = 0;
	m_sTeleportMaxLevel = 0;
	m_bTeleportHunt = FALSE;
	m_bTeleportNtrl = FALSE;
	m_bTeleportCrmnl = FALSE;
}

CTeleport::~CTeleport()
{										    

}

