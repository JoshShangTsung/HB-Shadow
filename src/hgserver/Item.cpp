#include "Item.h"
#include <cstring>

CItem::CItem() {
	std::memset(m_cName, 0, sizeof (m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;

	m_sItemEffectValue1 = 0;
	m_sItemEffectValue2 = 0;
	m_sItemEffectValue3 = 0;

	m_sItemEffectValue4 = 0;
	m_sItemEffectValue5 = 0;
	m_sItemEffectValue6 = 0;

	m_dwCount = 1;
	m_sTouchEffectType = TouchEffectType::NONE;
	m_sTouchEffectValue1 = 0;
	m_sTouchEffectValue2 = 0;
	m_sTouchEffectValue3 = 0;

	m_cItemColor = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;

	m_sSpecialEffectValue1 = 0;
	m_sSpecialEffectValue2 = 0;

	m_wCurLifeSpan = 0;
	m_dwAttribute = 0;

	m_cCategory = 0;
	m_sIDnum = 0;

	m_bIsForSale = false;
}

int CItem::iGetItemWeight(int iCount) const {
	int iWeight = (this->m_wWeight);
	if (iCount < 0) iCount = 1;
	iWeight = iWeight * iCount;
	if (this->m_sIDnum == 90) iWeight = iWeight / 20;
	if (iWeight <= 0) iWeight = 1;
	return iWeight;
}