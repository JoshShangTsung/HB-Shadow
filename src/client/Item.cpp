#include "Item.h"
#include <cstring>
CItem::CItem() {
	std::memset(m_cName, 0, sizeof(m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;
	m_dwAttribute = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;
}
