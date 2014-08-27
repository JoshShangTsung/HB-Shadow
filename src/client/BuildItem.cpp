#include "BuildItem.h"
#include <cstring>

CBuildItem::CBuildItem() {
	std::memset(m_cName, 0, sizeof(m_cName));

	m_bBuildEnabled = false;
	m_iSkillLimit = 0;
	m_iMaxSkill = 0;

	m_iSprH = 0;
	m_iSprFrame = 0;

	std::memset(m_cElementName1, 0, sizeof(m_cElementName1));
	std::memset(m_cElementName2, 0, sizeof(m_cElementName2));
	std::memset(m_cElementName3, 0, sizeof(m_cElementName3));
	std::memset(m_cElementName4, 0, sizeof(m_cElementName4));
	std::memset(m_cElementName5, 0, sizeof(m_cElementName5));
	std::memset(m_cElementName6, 0, sizeof(m_cElementName6));

	for (int i = 0; i < 7; i++) {
		m_iElementCount[i] = 0;
		m_bElementFlag[i] = 0;
	}
}