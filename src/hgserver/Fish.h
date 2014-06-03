#pragma once
#include "Map.h"
struct CItem;
class CFish {
public:
	CFish(MapPtr map, short sX, short sY, short sType, class CItem * pItem, int iDifficulty);
	virtual ~CFish();

	MapPtr map_;
	short m_sX, m_sY;

	short m_sType;
	class CItem * m_pItem;

	short m_sDynamicObjectHandle;

	short m_sEngagingCount;
	int m_iDifficulty;
};
