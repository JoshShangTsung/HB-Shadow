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

	WORD m_wDynamicObjectID;
	short m_sDynamicObjectType;
	DWORD m_dwDynamicObjectRegisterTime;

	bool m_bIsMoveAllowed, m_bIsTeleport, m_bIsWater, m_bIsFarm, m_bIsTempMoveAllowed;

	// ������ ǥ�ÿ� 
	int m_iOccupyStatus; // Aresden�̸� -������, Elvine�̸� +�� �������. �� ���� �� ������ ���⼺ 
	int m_iOccupyFlagIndex; // �� Ÿ�� ���� �� �ִ� ���ɱ���� �ε��� 

	// Crusade
	int m_iAttribute; // Ÿ���� ���� ��Ʈ: ù��° ��Ʈ(���� ��ȣ) �ι�° ��Ʈ(���� ��� ��ȣ) ����° ��Ʈ(��������)


};
