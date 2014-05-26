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

	m_bIsSummoned = FALSE;
	m_bIsPermAttackMode = FALSE;

	m_dwRegenTime = NULL;
	m_bIsKilled = FALSE;

	m_sOriginalType = NULL;
	m_iSummonControlMode = NULL;

	m_cAttribute = NULL;
	m_iAbsDamage = NULL;
	m_iStatus = NULL;
	m_sAppr2 = NULL;

	m_iAttackRange = 1;
	m_cSpecialAbility = 0;

	m_iExp = 0;

	m_iBuildCount = 0;
	m_iManaStock = 0;
	m_bIsUnsummoned = FALSE;
	m_cCropType = 0;
	m_cCropSkill = 0;

	m_bIsMaster = FALSE;
	m_iGuildGUID = NULL;
	m_iV1 = 0;

	m_iNpcItemType = 0;
	m_iNpcItemMax = 0;

}