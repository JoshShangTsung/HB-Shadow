#pragma once

#include <windows.h>
#include "Item.h"

#define DEF_TILE_PER_ITEMS	12

class CTile {
public:
	CTile();
	virtual ~CTile();

	char m_cOwnerClass; // DEF_OT_PLAYER / DEF_OT_NPC
	short m_sOwner;

	char m_cDeadOwnerClass; // DEF_OT_PLAYER / DEF_OT_NPC �׾��ִ� �÷��̾�� �̰��� 
	short m_sDeadOwner;

	class CItem * m_pItem[DEF_TILE_PER_ITEMS];
	char m_cTotalItem;

	uint16_t m_wDynamicObjectID;
	short m_sDynamicObjectType;
	uint32_t m_dwDynamicObjectRegisterTime;

	bool m_bIsMoveAllowed;
	bool m_bIsTeleport;
	bool m_bIsWater;
	bool m_bIsFarm;
	bool m_bIsTempMoveAllowed;

	
	int m_iOccupyStatus; 
	int m_iOccupyFlagIndex; 

	// Crusade
	int m_iAttribute;


};
