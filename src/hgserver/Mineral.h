#pragma once
#include "Map.h"

class CMineral {
public:
	CMineral(char cType, MapPtr map, int sX, int sY, int iRemain);
	char m_cType;

	MapPtr map_;
	int m_sX, m_sY;
	int m_iDifficulty;
	short m_sDynamicObjectHandle;

	int m_iRemain;
};
