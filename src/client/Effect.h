#pragma once
#include <cstdint>

class CEffect {
public:
	CEffect();
	short m_sType;
	char m_cFrame;
	char m_cMaxFrame;
	char m_cDir;
	uint32_t m_dwTime;
	uint32_t m_dwFrameTime;
	int m_sX;
	int m_sY;
	int m_dX;
	int m_dY;
	int m_mX;
	int m_mY;
	int m_mX2;
	int m_mY2;
	int m_mX3;
	int m_mY3;
	int m_iErr;
	int m_rX;
	int m_rY;
	int m_iV1;
};
