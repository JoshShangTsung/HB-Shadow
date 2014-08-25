#include "Tile.h"

CTile::CTile() {
	int i;

	m_bIsMoveAllowed = true;
	m_bIsTeleport = false;
	m_bIsWater = false;
	m_bIsFarm = false;

	m_sOwner = NULL;
	m_cOwnerClass = NULL;

	m_sDeadOwner = NULL;
	m_cDeadOwnerClass = NULL;

	for (i = 0; i < DEF_TILE_PER_ITEMS; i++)
		m_pItem[i] = NULL;
	m_cTotalItem = 0;

	m_wDynamicObjectID = NULL;
	m_sDynamicObjectType = NULL;

	m_bIsTempMoveAllowed = true;

	m_iOccupyStatus = NULL;
	m_iOccupyFlagIndex = NULL;

	m_iAttribute = NULL;
}

CTile::~CTile() {
	int i;
	for (i = 0; i < DEF_TILE_PER_ITEMS; i++)
		if (m_pItem[i] != NULL) delete m_pItem[i];
}
