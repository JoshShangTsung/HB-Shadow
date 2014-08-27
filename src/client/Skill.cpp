#include "Skill.h"
#include <cstring>
CSkill::CSkill() {
	std::memset(m_cName, 0, sizeof(m_cName));

	m_iLevel = 0;
	m_bIsUseable = false;
	m_cUseMethod = 0;
}
