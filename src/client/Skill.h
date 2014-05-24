#pragma once
#include <windows.h>

class CSkill {
public:
	CSkill();

	char m_cName[21];

	int m_iLevel;
	BOOL m_bIsUseable;
	char m_cUseMethod;
};