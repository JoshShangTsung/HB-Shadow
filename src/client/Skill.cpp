#include "Skill.h"

CSkill::CSkill() {
	ZeroMemory(m_cName, sizeof (m_cName));

	m_iLevel = 0;
	m_bIsUseable = FALSE;
	m_cUseMethod = NULL;
}