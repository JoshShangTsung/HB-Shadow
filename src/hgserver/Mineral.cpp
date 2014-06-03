#include "Mineral.h"

CMineral::CMineral(char cType, MapPtr map, int sX, int sY, int iRemain) {
	m_cType = cType;
	map_ = map;
	m_sX = sX;
	m_sY = sY;

	m_iRemain = iRemain;
	m_iDifficulty = 0;
}
