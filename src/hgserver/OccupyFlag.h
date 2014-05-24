#pragma once
#include <windows.h>

class COccupyFlag {
public:
	COccupyFlag(int dX, int dY, char cSide, int iEKNum, int iDOI);
	char m_cSide;
	int m_iEKCount;
	int m_sX, m_sY;

	int m_iDynamicObjectIndex;
};
