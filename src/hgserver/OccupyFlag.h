#pragma once

class COccupyFlag {
public:

	COccupyFlag(int dX, int dY, char cSide, int iEKNum, int iDOI) {
		m_sX = dX;
		m_sY = dY;

		m_cSide = cSide;
		m_iEKCount = iEKNum;

		m_iDynamicObjectIndex = iDOI;
	}
	char m_cSide = 0;
	int m_iEKCount = 0;
	int m_sX = 0;
	int m_sY = 0;

	int m_iDynamicObjectIndex = 0;
};
