#include "Npc.h"
#include <cstring>
CNpc::CNpc(const char * pName5) {
	int i;
	std::memset(m_cName, 0, sizeof(m_cName));
	memcpy(m_cName, pName5, 5);

	for (i = 0; i < DEF_MAXWAYPOINTS; i++)
		m_iWayPointIndex[i] = -1;

	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++)
		m_cMagicEffectStatus[i] = 0;

	m_bIsSummoned = false;
	m_bIsPermAttackMode = false;

	m_dwRegenTime = 0;
	m_bIsKilled = false;

	m_sOriginalType = 0;
	m_iSummonControlMode = 0;

	m_cAttribute = 0;
	m_iAbsDamage = 0;
	m_iStatus = 0;
	m_sAppr2 = 0;

	m_iAttackRange = 1;
	m_cSpecialAbility = 0;

	m_iExp = 0;

	m_iBuildCount = 0;
	m_iManaStock = 0;
	m_bIsUnsummoned = false;
	m_cCropType = 0;
	m_cCropSkill = 0;

	m_bIsMaster = false;
	m_iGuildGUID = 0;
	m_iV1 = 0;

	m_iNpcItemType = 0;
	m_iNpcItemMax = 0;

}