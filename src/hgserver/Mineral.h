#pragma once

class CMineral {
public:

	CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain) {
		m_cType = cType;
		m_cMapIndex = cMapIndex;
		m_sX = sX;
		m_sY = sY;
		m_iRemain = iRemain;
		m_iDifficulty = 0;
	}
	char m_cType = 0;

	char m_cMapIndex = 0;
	int m_sX = 0;
	int m_sY = 0;
	int m_iDifficulty = 0;
	short m_sDynamicObjectHandle = 0;

	int m_iRemain = 0;
};
