#include "Portion.h"
#include <cstring>

CPortion::CPortion() {
	int i;

	std::memset(m_cName, 0, sizeof (m_cName));
	m_iSkillLimit = 0;
	m_iDifficulty = 0;

	for (i = 0; i < 12; i++)
		m_sArray[i] = -1;
}
