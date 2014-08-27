#include "BuildItem.h"
#include <cstring>
CBuildItem::CBuildItem() {
	std::memset(m_cName, 0, sizeof(m_cName));
	m_sItemID = ItemId::none;

	m_iSkillLimit = 0;

	for (int i = 0; i < 6; i++) {
		m_iMaterialItemID[i] = ItemId::none;
		m_iMaterialItemCount[i] = 0;
		m_iMaterialItemValue[i] = 0;
		m_iIndex[i] = -1;
	}

	m_iMaxValue = 0;
	m_iAverageValue = 0;
	m_iMaxSkill = 0;
	m_wAttribute = 0;
}
