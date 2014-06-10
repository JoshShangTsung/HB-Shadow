#include "Skill.h"
#include <cstring>
CSkill::CSkill() {
	std::memset(m_cName, 0, sizeof(m_cName));
}
