#include "Portion.h"
#include <cstring>
CPortion::CPortion() {
	int i;

	std::memset(m_cName, 0, sizeof(m_cName));
	m_iSkillLimit = 0;
	m_iDifficulty = 0;

	for (i = 0; i < 6; i++) {
		m_sArray[i].id_ = ItemId::none;
		m_sArray[i].num_ = -1;
	}
}

bool operator==(const CPortion::E &a, const CPortion::E &b) {
	return a.id_ == b.id_ && a.num_ == b.num_;
}

bool operator!=(const CPortion::E &a, const CPortion::E &b) {
	return !(a==b);
}
