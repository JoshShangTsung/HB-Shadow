#pragma once

class CMineral {
public:
	CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain);
	char m_cType;

	char m_cMapIndex;
	int m_sX;
	int m_sY;
	int m_iDifficulty;
	short m_sDynamicObjectHandle;

	int m_iRemain;
};
