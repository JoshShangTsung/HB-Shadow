#pragma once
#include "Item.h"

class CPortion {
public:
	char m_cName[21]{};
	struct E {
		ItemId id_ = ItemId::none;
		short num_ = -1;
	};
	E m_sArray[6]{};

	int m_iSkillLimit = 0;
	int m_iDifficulty = 0;

};

bool operator==(const CPortion::E &a, const CPortion::E &b);
bool operator!=(const CPortion::E &a, const CPortion::E &b);
