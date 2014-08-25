#include "Client.h"

CClient::CClient(HWND hWnd) {
	register int i;

	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);

	ZeroMemory(m_cProfile, sizeof (m_cProfile));
	strcpy(m_cProfile, "__________");

	ZeroMemory(m_cCharName, sizeof (m_cCharName));
	ZeroMemory(m_cAccountName, sizeof (m_cAccountName));
	ZeroMemory(m_cAccountPassword, sizeof (m_cAccountPassword));

	ZeroMemory(m_cGuildName, sizeof (m_cGuildName));
	ZeroMemory(m_cLocation, sizeof (m_cLocation));
	strcpy(m_cLocation, "NONE");
	m_iGuildRank = -1;
	m_iGuildGUID = -1;

	m_bIsInitComplete = false;

	//m_cLU_Str = m_cLU_Int = m_cLU_Vit = m_cLU_Dex = m_cLU_Mag = m_cLU_Char = 0;
	m_iLU_Pool = 0;
	m_cAura = 0;

	//m_iHitRatio_ItemEffect_SM = 0;
	//m_iHitRatio_ItemEffect_L  = 0;
	m_cVar = 0;
	m_iEnemyKillCount = 0;
	m_iPKCount = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
	m_dwLogoutHackCheck = 0;
	m_dwAFKCheckTime = 0;
	iDmgShowon = 0;

	// Charges
	m_iAddTransMana = 0;
	m_iAddChargeCritical = 0;

	m_bIsSafeAttackMode = false;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
		m_sItemEquipmentStatus[i] = -1;

	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pItemList[i] = NULL;
		m_ItemPosList[i].x = 40;
		m_ItemPosList[i].y = 30;
		m_bIsItemEquipped[i] = false;
	}
	m_cArrowIndex = -1;

	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		m_pItemInBankList[i] = NULL;
	}

	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = NULL;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = NULL;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = false;
		m_iSkillUsingTimeID[i] = NULL;
	}

	m_iAngelicStr = 0;
	m_iAngelicInt = 0;
	m_iAngelicDex = 0;
	m_iAngelicMag = 0;

	m_cMapIndex = -1;
	m_sX = -1;
	m_sY = -1;
	m_cDir = 5;
	m_sType = 0;
	m_sOriginalType = 0;
	m_sAppr1 = 0;
	m_sAppr2 = 0;
	m_sAppr3 = 0;
	m_sAppr4 = 0;
	m_iApprColor = 0;
	m_iStatus = 0;

	m_cSex = 0;
	m_cSkin = 0;
	m_cHairStyle = 0;
	m_cHairColor = 0;
	m_cUnderwear = 0;

	m_cAttackDiceThrow_SM = 0;
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM = 0;
	m_cAttackBonus_L = 0;

	m_cSide = 0;

	m_iHitRatio = 0;
	m_iDefenseRatio = 0;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i] = 0;
	m_iDamageAbsorption_Shield = 0;

	m_iHPstock = 0;
	m_bIsKilled = false;

	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++)
		m_cMagicEffectStatus[i] = 0;

	m_iWhisperPlayerIndex = -1;
	ZeroMemory(m_cWhisperPlayerName, sizeof (m_cWhisperPlayerName));

	m_iHungerStatus = 100; 

	m_bIsWarLocation = false;
	m_dwAFKCheckTime = NULL;
	m_bIsPoisoned = false;
	m_iPoisonLevel = NULL;

	m_iAdminUserLevel = 0;
	m_iRating = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall = 0;
	m_iTimeLeft_FirmStaminar = 0;

	m_iRecentWalkTime = 0;
	m_iRecentRunTime = 0;
	m_sV1 = 0;

	m_bIsOnServerChange = false;
	m_bInhibition = false;

	m_iExpStock = 0;

	m_iAllocatedFish = NULL;
	m_iFishChance = 0;

	ZeroMemory(m_cIPaddress, sizeof (m_cIPaddress));
	m_bIsOnWaitingProcess = false;

	m_iSuperAttackLeft = 0;
	m_iSuperAttackCount = 0;

	m_sUsingWeaponSkill = 5;

	m_iManaSaveRatio = 0;
	m_iAddResistMagic = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage = 0;
	m_bIsLuckyEffect = false;
	m_iSideEffect_MaxHPdown = 0;

	m_iAddAbsAir = 0;
	m_iAddAbsEarth = 0;
	m_iAddAbsFire = 0;
	m_iAddAbsWater = 0;

	m_iComboAttackCount = 0;
	m_iDownSkillIndex = -1;
	m_bInRecallImpossibleMap = 0;

	m_iMagicDamageSaveItemIndex = -1;

	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;

	m_iPartyID = 0;
	m_iPartyStatus = 0;
	m_iReqJoinPartyClientH = 0;
	ZeroMemory(m_cReqJoinPartyName, sizeof (m_cReqJoinPartyName));

	/*m_iPartyRank = -1;
	m_iPartyMemberCount = 0;
	m_iPartyGUID        = 0;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}*/

	m_iAbuseCount = 0;
	m_bIsBWMonitor = false;
	m_bIsExchangeMode = false;

	m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime = 0;

	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0;

	m_iExchangeH = NULL;
	ZeroMemory(m_cExchangeName, sizeof (m_cExchangeName));
	ZeroMemory(m_cExchangeItemName, sizeof (m_cExchangeItemName));

	for (i = 0; i < 4; i++) {
		m_cExchangeItemIndex[i] = -1;
		m_iExchangeItemAmount[i] = 0;
	}

	m_bIsExchangeConfirm = false;

	m_iQuest = NULL;
	m_iQuestID = NULL;
	m_iAskedQuest = NULL;
	m_iCurQuestCount = NULL;

	m_iQuestRewardType = NULL;
	m_iQuestRewardAmount = NULL;

	m_iContribution = NULL;
	m_bQuestMatchFlag_Loc = false;
	m_bIsQuestCompleted = false;

	m_cHeroArmourBonus = 0;

	m_bIsNeutral = false;
	m_bIsObserverMode = false;

	m_iSpecialEventID = 200081;

	m_iSpecialWeaponEffectType = 0;
	m_iSpecialWeaponEffectValue = 0;

	m_iAddHP = m_iAddSP = m_iAddMP = 0;
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;

	m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
	m_iSpecialAbilityType = NULL;
	m_bIsSpecialAbilityEnabled = false;
	m_iSpecialAbilityLastSec = 0;

	m_iSpecialAbilityEquipPos = 0;

	m_iMoveMsgRecvCount = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount = 0;
	m_iSkillMsgRecvCount = 0;

	m_bIsAdminCommandEnabled = false;
	m_iAlterItemDropIndex = -1;

	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;

	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;

	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;

	ZeroMemory(m_cLockedMapName, sizeof (m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = NULL;

	m_iCrusadeDuty = NULL;
	m_dwCrusadeGUID = NULL;
	m_dwHeldenianGUID = NULL;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}

	m_iCSIsendPoint = NULL;

	m_bIsSendingMapStatus = false;
	ZeroMemory(m_cSendingMapName, sizeof (m_cSendingMapName));

	m_iConstructionPoint = NULL;

	ZeroMemory(m_cConstructMapName, sizeof (m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;

	m_bIsAdminOrderGoto = false;
	m_bIsInsideWarehouse = false;
	m_bIsInsideWizardTower = false;
	m_bIsInsideOwnTown = false;
	m_bIsOwnLocation = false;
	m_pIsProcessingAllowed = false;
	m_bIsClientConnected = false;



	m_iGizonItemUpgradeLeft = 0;
	m_iBallPoints = 0;
	m_cHeroArmorBonus = 0;

	m_bIsBeingResurrected = false;
	m_bMagicConfirm = false;
	m_bMagicItem = false;
	m_iSpellCount = 0;
	m_bMagicPauseTime = false;

	m_dwDSLAT = m_dwDSLATOld = m_dwDSLATS = 0;
	m_iDSCount = 0;

}

CClient::~CClient() {
	int i;

	if (m_pXSock != NULL) delete m_pXSock;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pItemList[i] != NULL) {
			delete m_pItemList[i];
			m_pItemList[i] = NULL;
		}
	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pItemInBankList[i] != NULL) {
			delete m_pItemInBankList[i];
			m_pItemInBankList[i] = NULL;
		}
}

bool CClient::bCreateNewParty() {
	int i;

	if (m_iPartyRank != -1) return false;

	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof (m_stPartyMemberName[i].cName));
	}

	return true;
}

