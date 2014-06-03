#include "Fish.h"
#include "item.h"

CFish::CFish(MapPtr map, short sX, short sY, short sType, class CItem * pItem, int iDifficulty) {
	map_ = map;
	m_sX = sX;
	m_sY = sY;
	m_sType = sType;
	m_pItem = pItem;

	m_sEngagingCount = 0;
	m_iDifficulty = iDifficulty;

	if (m_iDifficulty <= 0)
		m_iDifficulty = 1;
}

CFish::~CFish() {
	if (m_pItem != nullptr) delete m_pItem;
}
