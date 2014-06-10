#pragma once
#include <windows.h>
#include "item.h"

class CFish {
public:
	CFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty);
	virtual ~CFish();

	char m_cMapIndex;
	short m_sX, m_sY; 

	short m_sType; 
	class CItem * m_pItem; 

	short m_sDynamicObjectHandle;

	short m_sEngagingCount; 
	int m_iDifficulty; 
};
