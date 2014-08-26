#pragma once
#include "Item.h"

class CPortion {
public:
	CPortion();
	char m_cName[21];
	struct E {
		ItemId id_;
		short num_;
	};
	E m_sArray[6];

	int m_iSkillLimit, m_iDifficulty;

};

bool operator==(const CPortion::E &a, const CPortion::E &b);
bool operator!=(const CPortion::E &a, const CPortion::E &b);
