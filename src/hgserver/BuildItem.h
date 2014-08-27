#pragma once
#include <cstdint>
#include "Item.h"

class CBuildItem {
public:
	char m_cName[21]{};
	ItemId m_sItemID = ItemId::none;

	int m_iSkillLimit = 0;

	ItemId m_iMaterialItemID[6]{ItemId::none,ItemId::none,ItemId::none,ItemId::none,ItemId::none,ItemId::none};
	int m_iMaterialItemCount[6]{};
	int m_iMaterialItemValue[6]{};
	int m_iIndex[6]{-1, -1, -1, -1, -1, -1};

	int m_iMaxValue = 0;
	int m_iAverageValue = 0;
	int m_iMaxSkill = 0;
	uint16_t m_wAttribute = 0;
};
