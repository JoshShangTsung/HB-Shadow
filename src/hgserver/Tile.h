#pragma once

#include <windows.h>
#include "Item.h"

#define DEF_TILE_PER_ITEMS	12

class CTile {
public:
	~CTile();

	char m_cOwnerClass = 0; // DEF_OT_PLAYER / DEF_OT_NPC
	short m_sOwner = 0;

	char m_cDeadOwnerClass = 0; // DEF_OT_PLAYER / DEF_OT_NPC
	short m_sDeadOwner = 0;

	class CItem * m_pItem[DEF_TILE_PER_ITEMS]{};
	char m_cTotalItem = 0;

	uint16_t m_wDynamicObjectID = 0;
	short m_sDynamicObjectType = 0;
	uint32_t m_dwDynamicObjectRegisterTime = 0;

	bool m_bIsMoveAllowed = true;
	bool m_bIsTeleport = false;
	bool m_bIsWater = false;
	bool m_bIsFarm = false;
	bool m_bIsTempMoveAllowed = true;

	int m_iOccupyStatus = 0;
	int m_iOccupyFlagIndex = 0;

	// Crusade
	int m_iAttribute = 0;
};
