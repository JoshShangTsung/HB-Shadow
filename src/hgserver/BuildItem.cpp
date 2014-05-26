#include "BuildItem.h"

CBuildItem::CBuildItem() {
	ZeroMemory(m_cName, sizeof (m_cName));
	m_sItemID = -1;

	m_iSkillLimit = 0;

	for (int i = 0; i < 6; i++) {
		m_iMaterialItemID[i] = NULL;
		m_iMaterialItemCount[i] = NULL;
		m_iMaterialItemValue[i] = NULL;
		m_iIndex[i] = -1;
	}

	m_iMaxValue = 0;
	m_iAverageValue = 0;
	m_iMaxSkill = 0;
	m_wAttribute = 0;
}
