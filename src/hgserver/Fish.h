#pragma once
#include <windows.h>
#include "item.h"

class CFish {
public:
	CFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty);
	~CFish();

	char m_cMapIndex =0;
	short m_sX = 0;
	short m_sY = 0; 

	short m_sType = 0; 
	class CItem * m_pItem = nullptr; 

	short m_sDynamicObjectHandle = 0;

	short m_sEngagingCount = 0; 
	int m_iDifficulty = 0; 
};
