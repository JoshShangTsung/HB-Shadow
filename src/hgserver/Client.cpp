#include "Client.h"
#include "Game.h"
#include "WINMAIN.H"
#include <cstring>

extern char G_cTxt[512];

CClient::CClient(CGame &game, int index, std::unique_ptr<XSocket> &&socket): game_(game), id_(index), m_pXSock(std::move(socket)) {
	std::memset(m_cProfile, 0, sizeof(m_cProfile));
	strcpy(m_cProfile, "__________");

	std::memset(m_cCharName, 0, sizeof(m_cCharName));
	std::memset(m_cAccountName, 0, sizeof(m_cAccountName));
	std::memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));

	std::memset(m_cGuildName, 0, sizeof(m_cGuildName));
	std::memset(m_cLocation, 0, sizeof(m_cLocation));
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

	for (int i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
		m_sItemEquipmentStatus[i] = -1;

	for (int i = 0; i < DEF_MAXITEMS; i++) {
		m_pItemList[i] = nullptr;
		m_ItemPosList[i].x = 40;
		m_ItemPosList[i].y = 30;
		m_bIsItemEquipped[i] = false;
	}
	m_cArrowIndex = -1;

	for (int i = 0; i < DEF_MAXBANKITEMS; i++) {
		m_pItemInBankList[i] = nullptr;
	}

	for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = 0;

	for (int i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = 0;

	for (int i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = false;
		m_iSkillUsingTimeID[i] = 0;
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

	for (int i = 0; i < DEF_MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i] = 0;
	m_iDamageAbsorption_Shield = 0;

	m_iHPstock = 0;
	m_bIsKilled = false;

	for (int i = 0; i < DEF_MAXMAGICEFFECTS; i++)
		m_cMagicEffectStatus[i] = 0;

	m_iWhisperPlayerIndex = -1;
	std::memset(m_cWhisperPlayerName, 0, sizeof(m_cWhisperPlayerName));

	m_iHungerStatus = 100; 

	m_bIsWarLocation = false;
	m_dwAFKCheckTime = 0;
	m_bIsPoisoned = false;
	m_iPoisonLevel = 0;

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

	m_iAllocatedFish = 0;
	m_iFishChance = 0;

	std::memset(m_cIPaddress, 0, sizeof(m_cIPaddress));
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
	std::memset(m_cReqJoinPartyName, 0, sizeof(m_cReqJoinPartyName));

	/*m_iPartyRank = -1;
	m_iPartyMemberCount = 0;
	m_iPartyGUID        = 0;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		std::memset(m_stPartyMemberName[i].cName, 0, sizeof(m_stPartyMemberName[i].cName));
	}*/

	m_iAbuseCount = 0;
	m_bIsBWMonitor = false;
	m_bIsExchangeMode = false;

	m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime = 0;

	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0;

	m_iExchangeH = 0;
	std::memset(m_cExchangeName, 0, sizeof(m_cExchangeName));
	std::memset(m_cExchangeItemName, 0, sizeof(m_cExchangeItemName));

	for (int i = 0; i < 4; i++) {
		m_cExchangeItemIndex[i] = -1;
		m_iExchangeItemAmount[i] = 0;
	}

	m_bIsExchangeConfirm = false;

	m_iQuest = 0;
	m_iQuestID = 0;
	m_iAskedQuest = 0;
	m_iCurQuestCount = 0;

	m_iQuestRewardType = 0;
	m_iQuestRewardAmount = 0;

	m_iContribution = 0;
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
	m_iSpecialAbilityType = 0;
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

	std::memset(m_cLockedMapName, 0, sizeof(m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = 0;

	m_iCrusadeDuty = 0;
	m_dwCrusadeGUID = 0;
	m_dwHeldenianGUID = 0;

	for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = 0;
		m_stCrusadeStructureInfo[i].cSide = 0;
		m_stCrusadeStructureInfo[i].sX = 0;
		m_stCrusadeStructureInfo[i].sY = 0;
	}

	m_iCSIsendPoint = 0;

	m_bIsSendingMapStatus = false;
	std::memset(m_cSendingMapName, 0, sizeof(m_cSendingMapName));

	m_iConstructionPoint = 0;

	std::memset(m_cConstructMapName, 0, sizeof(m_cConstructMapName));
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

bool CClient::bCreateNewParty() {
	int i;

	if (m_iPartyRank != -1) return false;

	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		std::memset(m_stPartyMemberName[i].cName, 0, sizeof(m_stPartyMemberName[i].cName));
	}

	return true;
}

void CClient::ClientKilledHandler(int iAttackerH, char cAttackerType, short sDamage) {
	char cAttackerName[21], cEKMsg[1000];
	short sAttackerWeapon;
	int i, iExH;
	bool bIsSAattacked = false;

	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;

	// �������� ��� �ð��� �Է��Ѵ�.
	// 2002-7-4 �������� ������ �ø� �� �ֵ���
	if (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cName, "fight", 5) == 0) {
		this->m_dwFightzoneDeadTime = timeGetTime();
		wsprintf(G_cTxt, "Fightzone Dead Time: %d", this->m_dwFightzoneDeadTime);
		PutLogList(G_cTxt);
	}

	this->m_bIsKilled = true;
	// HP�� 0�̴�.
	this->m_iHP = 0;

	// ���� ��ȯ ����� ��ȯ�� ����Ѵ�.
	if (this->m_bIsExchangeMode == true) {
		iExH = this->m_iExchangeH;
		game_._ClearExchangeStatus(iExH);
		game_._ClearExchangeStatus(id_);
	}


	game_.RemoveFromTarget(id_, DEF_OWNERTYPE_PLAYER);

	std::memset(cAttackerName, 0, sizeof (cAttackerName));
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER_INDIRECT:
		case DEF_OWNERTYPE_PLAYER:
			if (game_.m_pClientList[iAttackerH] != nullptr)
				memcpy(cAttackerName, game_.m_pClientList[iAttackerH]->m_cCharName, 10);
			break;
		case DEF_OWNERTYPE_NPC:
			if (game_.m_pNpcList[iAttackerH] != nullptr)
#ifdef DEF_LOCALNPCNAME     // v2.14 NPC �̸� �߹�ȭ�� ���� ����
				wsprintf(cAttackerName, "NPCNPCNPC@%d", m_pNpcList[iAttackerH]->m_sType);
#else
				memcpy(cAttackerName, game_.m_pNpcList[iAttackerH]->m_cNpcName, 20);
#endif
			break;
		default:
			break;
	}

	this->SendNotifyMsg(0,DEF_NOTIFY_KILLED, 0, 0, 0, cAttackerName);
	// �ٸ� Ŭ���̾�Ʈ���� �״� ���� ���.
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		sAttackerWeapon = ((game_.m_pClientList[iAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
	} else sAttackerWeapon = 1;
	game_.SendEventToNearClient_TypeA(id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
	game_.m_pMapList[this->m_cMapIndex]->ClearOwner(12, id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
	game_.m_pMapList[this->m_cMapIndex]->SetDeadOwner(id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
	if (game_.m_pMapList[this->m_cMapIndex]->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;
	if ((game_.m_pMapList[this->m_cMapIndex] != nullptr) &&
			  (game_.m_bIsHeldenianMode == true) &&
			  (game_.m_pMapList[this->m_cMapIndex]->m_bIsHeldenianMap == true)) {
		if (this->m_cSide == 1) {
			game_.m_iHeldenianAresdenDead++;
		} else if (this->m_cSide == 2) {
			game_.m_iHeldenianElvineDead++;
		}
		game_.UpdateHeldenianStatus();
	}


	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		// v1.432
		// Ư�� �ɷ��� �ִ� ����� ����� ���ߴ�.
		switch (game_.m_pClientList[iAttackerH]->m_iSpecialAbilityType) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				bIsSAattacked = true;
				break;
		}

		if (iAttackerH == id_) return; // �����̴�.

		if (memcmp(this->m_cLocation, "NONE", 4) == 0) {
			// ����ڰ� �������̴�.
			if (this->m_iPKCount == 0) {



				game_.ApplyPKpenalty(iAttackerH, id_);
			} else {


				game_.PK_KillRewardHandler(iAttackerH, id_);
			}
		} else {
			// ����ڰ� �����ڰ� �ƴ϶� �� ���� �Ҽ��̴�.
			if (this->m_iGuildRank == -1) {


				if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {
					// ����ڰ� �������̴�.
					if (this->m_iPKCount == 0) {

						game_.ApplyPKpenalty(iAttackerH, id_);
					} else {
						// �����ڴ� PK�� ��Ƶ� ������ ���� ���Ѵ�.

					}
				} else {

					if (memcmp(this->m_cLocation, game_.m_pClientList[iAttackerH]->m_cLocation, 10) == 0) {

						if (this->m_iPKCount == 0) {
							// ����ڰ� ������ ����. PK�̴�.
							game_.ApplyPKpenalty(iAttackerH, id_);
						} else {
							// �����ڸ� ��Ҵ�.
							game_.PK_KillRewardHandler(iAttackerH, id_);
						}
					} else {
						// ����ڰ� �ٸ� ���� �Ҽ�. ������ �������
						game_.EnemyKillRewardHandler(iAttackerH, id_);
					}
				}
			} else {
				// ����ڴ� �����̴�.

				if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {
					// ����ڰ� �������̴�.
					if (this->m_iPKCount == 0) {
						// ������ ������ �����ڰ� �׿���. �����ڴ� PK�� �ȴ�.
						game_.ApplyPKpenalty(iAttackerH, id_);
					} else {
						// �����ڴ� PK�� ��Ƶ� ������ ���� ���Ѵ�.

					}
				} else {

					if (memcmp(this->m_cLocation, game_.m_pClientList[iAttackerH]->m_cLocation, 10) == 0) {

						if (this->m_iPKCount == 0) {
							// ����ڰ� ������ ����. PK�̴�.
							game_.ApplyPKpenalty(iAttackerH, id_);
						} else {
							// �����ڸ� ��Ҵ�.
							game_.PK_KillRewardHandler(iAttackerH, id_);
						}
					} else {
						// ����ڰ� �ٸ� ���� �Ҽ�. ������ �������
						game_.EnemyKillRewardHandler(iAttackerH, id_);
					}
				}
			}
		}


		if (this->m_iPKCount == 0) {
			// Innocent
			if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {
				//�����ڿ��� ��ݹ޾� �׾��.

				//this->m_iExp -= iDice(1, 100);
				//if (this->m_iExp < 0) this->m_iExp = 0;
				//this->SendNotifyMsg(nullptr,DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
			} else {
				if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, this->m_cLocation, 10) == 0) {


					//this->m_iExp -= iDice(1, 100);
					//if (this->m_iExp < 0) this->m_iExp = 0;
					//this->SendNotifyMsg(nullptr,DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
				} else {

					game_.ApplyCombatKilledPenalty(id_, 2, bIsSAattacked);
				}
			}
		} else if ((this->m_iPKCount >= 1) && (this->m_iPKCount <= 3)) {
			// Criminal
			game_.ApplyCombatKilledPenalty(id_, 3, bIsSAattacked);
		} else if ((this->m_iPKCount >= 4) && (this->m_iPKCount <= 11)) {
			// Murderer
			game_.ApplyCombatKilledPenalty(id_, 6, bIsSAattacked);
		} else if ((this->m_iPKCount >= 12)) {
			// Slaughterer
			game_.ApplyCombatKilledPenalty(id_, 12, bIsSAattacked);
		}
	} else if (cAttackerType == DEF_OWNERTYPE_NPC) {

		game_._bPKLog(DEF_PKLOG_BYNPC, id_, 0, cAttackerName);


		if (this->m_iPKCount == 0) {
			// Innocent
			game_.ApplyCombatKilledPenalty(id_, 1, bIsSAattacked);
		} else if ((this->m_iPKCount >= 1) && (this->m_iPKCount <= 3)) {
			// Criminal
			game_.ApplyCombatKilledPenalty(id_, 3, bIsSAattacked);
		} else if ((this->m_iPKCount >= 4) && (this->m_iPKCount <= 11)) {
			// Murderer
			game_.ApplyCombatKilledPenalty(id_, 6, bIsSAattacked);
		} else if ((this->m_iPKCount >= 12)) {
			// Slaughterer
			game_.ApplyCombatKilledPenalty(id_, 12, bIsSAattacked);
		}
		// ���� ����� NPC�� ����� ����Ʈ��� ���ְ�� �Ǽ� ����Ʈ �ΰ�
		if (game_.m_pNpcList[iAttackerH]->m_iGuildGUID != 0) {

			if (game_.m_pNpcList[iAttackerH]->m_cSide != this->m_cSide) {
				// ����� ������ Ȥ�� ����Ʈ�� �� �÷��̾ �׿���. �ٷ� �뺸�Ѵ�.
				// ���� ������ ���ְ��� �ִٸ� ��ٷ� �뺸. ������ �ٸ� ������ �˷���.
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((game_.m_pClientList[i] != nullptr) && (game_.m_pClientList[i]->m_iGuildGUID == game_.m_pNpcList[iAttackerH]->m_iGuildGUID) &&
							  (game_.m_pClientList[i]->m_iCrusadeDuty == 3)) {
						game_.m_pClientList[i]->m_iConstructionPoint += (this->m_iLevel / 2);

						if (game_.m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
							game_.m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

						//testcode
						wsprintf(G_cTxt, "Enemy Player Killed by Npc! Construction +%d", (this->m_iLevel / 2));
						PutLogList(G_cTxt);
						// ���ְ�� �ٷ� �뺸.
						game_.m_pClientList[i]->SendNotifyMsg(0,DEF_NOTIFY_CONSTRUCTIONPOINT, game_.m_pClientList[i]->m_iConstructionPoint, game_.m_pClientList[i]->m_iWarContribution, 0, nullptr);
						return;
					}
			}
		}
	} else if (cAttackerType == DEF_OWNERTYPE_PLAYER_INDIRECT) {
		game_._bPKLog(DEF_PKLOG_BYOTHER, id_, 0, nullptr);
		// �÷��̾ �׾����� ����ڰ� �������̴�. �ƹ��� ������ ���.
		// this->m_iExp -= iDice(1, 50);
		// if (this->m_iExp < 0) this->m_iExp = 0;

		// this->SendNotifyMsg(nullptr,DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
	}
	//----------------------------EK Announcer-------------------------
	//---- Function: CGame::ClientKilledHandler                    ----
	//---- Description: Announces a message to alert all connected ----
	//---- this-> an EK has taken place                           ----
	//---- Version: HBX 2.03 Build                                 ----
	//---- Date: November 07 2005                                  ----
	//---- By: Daxation                                            ----
	//---- Notes: Add char cEKMsg[1000]                            ----
	//-----------------------------------------------------------------
	std::memset(cEKMsg, 0, sizeof (cEKMsg));
	//Multiple EK Messages
	//Note - Remove section '01' and replace with alternative code for a single message
	//Alternative code: wsprintf(cEKMsg, "%s killed %s", cAttackerName, this->m_cCharName);
	// 01
	switch (iDice(1, 10))
		//You can add extra messages by creating a new case.
		//Remember to increase iDice
	{
		case 1: // To reverse the order the names appear in the message reverse the last 2 parameters
			wsprintf(cEKMsg, "%s whooped %s's ass!", cAttackerName, this->m_cCharName);
			break;
		case 2:
			wsprintf(cEKMsg, "%s smashed %s's face into the ground!", cAttackerName, this->m_cCharName);
			break;
		case 3:
			wsprintf(cEKMsg, "%s was sliced to pieces by %s", this->m_cCharName, cAttackerName);
			break;
		case 4:
			wsprintf(cEKMsg, "%s says LAG LAG!! but gets PWNED by %s", this->m_cCharName, cAttackerName);
			break;
		case 5:
			wsprintf(cEKMsg, "%s sent %s off too pie heaven!", cAttackerName, this->m_cCharName);
			break;
		case 6:
			wsprintf(cEKMsg, "%s got beat by %s's ugly stick!", cAttackerName, this->m_cCharName);
			break;
		case 7:
			wsprintf(cEKMsg, "%s OwneD! %s", cAttackerName, this->m_cCharName);
			break;
		case 8:
			wsprintf(cEKMsg, "%s Sended %s To Revival Zone! Too Bad ;(", cAttackerName, this->m_cCharName);
			break;
		case 9:
			wsprintf(cEKMsg, "%s says: I CAN OWN YOU! But gets OWNED by %s", this->m_cCharName, cAttackerName);
			break;
		case 10:
			wsprintf(cEKMsg, "%s KilleD %s", cAttackerName, this->m_cCharName);
			break;
	}
	// 01
	for (i = 1; i < DEF_MAXCLIENTS; i++) // Check all this->
	{
		if ((game_.m_pClientList[i] != nullptr)) // Check if this->is avtice
		{
			game_.m_pClientList[i]->SendNotifyMsg(0,DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cEKMsg); // Send message to client
			// Log EK
			wsprintf(G_cTxt, "%s killed %s", cAttackerName, this->m_cCharName); // Log message
			PutLogFileList(G_cTxt); // Enter into logs
		}
	}
	//-----------------------------------------------------------------
	//----                   End EK Announcer Code                 ----
	//-----------------------------------------------------------------
}


void CClient::SendNotifyMsg(int iFromH, uint16_t wMsgType, uint32_t sV1, uint32_t sV2, uint32_t sV3, const char * pString, uint32_t sV4, uint32_t sV5, uint32_t sV6, uint32_t sV7, uint32_t sV8, uint32_t sV9, char * pString2) {
	char cData[1000];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	short * sp;
	int * ip, iRet, i;

	std::memset(cData, 0, sizeof (cData));

	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_NOTIFY;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;

	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);


	switch (wMsgType) {
		case DEF_NOTIFY_HELDENIANCOUNT:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV3;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV4;
			cp += 2;

			cp += 14;

			iRet = this->m_pXSock->iSendMsg(cData, 14);

			break;

		case DEF_NOTIFY_NOMOREAGRICULTURE:
		case DEF_NOTIFY_AGRICULTURESKILLLIMIT:
		case DEF_NOTIFY_AGRICULTURENOAREA:
		case DEF_NOTIFY_HAPPYHOURSTAR:
		case DEF_NOTIFY_HAPPYHOUREND:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

			// Crafting
		case DEF_NOTIFY_CRAFTING_FAIL: //reversed by Snoopy: 0x0BF1:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;
			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

			// New 18/05/2004
		case DEF_NOTIFY_SPAWNEVENT:
			*cp = (char) sV3;
			cp++;

			sp = (short *) cp;
			*cp = sV1;
			cp += 2;

			sp = (short *) cp;
			*cp = sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 11);
			break;

		case DEF_NOTIFY_QUESTCOUNTER:
			ip = (int *) cp;
			*ip = sV1;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_REQRANGO: // Morla2.2 - Notify Rango
			ip = (int *) cp;
			*ip = sV1;
			cp += 4;

			ip = (int *) cp;
			*ip = sV2;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_APOCGATECLOSE:
		case DEF_NOTIFY_APOCGATEOPEN:
			ip = (int *) cp;
			*ip = sV1;
			cp += 4;

			ip = (int *) cp;
			*ip = sV2;
			cp += 4;

			memcpy(cp, pString, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_ABADDONKILLED:
			memcpy(cp, game_.m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 24);
			break;

		case DEF_NOTIFY_APOCFORCERECALLPLAYERS:
		case DEF_NOTIFY_APOCGATESTARTMSG:
		case DEF_NOTIFY_APOCGATEENDMSG:
		case DEF_NOTIFY_NORECALL:
			//50Cent - Capture The Flag
		case DEF_NOTIFY_CAPTURETHEFLAGSTART:
		case DEF_NOTIFY_ARESDENCAPTUREDELVINEFLAG:
		case DEF_NOTIFY_ELVINECAPTUREDARESDENFLAG:
		case DEF_NOTIFY_ELVINEFLAGBACKTOCH:
		case DEF_NOTIFY_ARESDENFLAGBACKTOCH:
		case DEF_NOTIFY_ELVINEWINSROUND:
		case DEF_NOTIFY_ARESDENWINSROUND:
		case DEF_NOTIFY_ELVINEWINCTF:
		case DEF_NOTIFY_ARESDENWINCTF:
		case DEF_NOTIFY_TIECTF:

			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_FORCERECALLTIME:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

			// New 16/05/2004
			//0xB4E2, 0xBEB
		case DEF_NOTIFY_MONSTERCOUNT:
		case DEF_NOTIFY_SLATE_STATUS:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

			//0x0BE5, 0x0BE7, 0x0BE8, 0x0BEA
		case DEF_NOTIFY_0BE8:
		case DEF_NOTIFY_HELDENIANTELEPORT:
		case DEF_NOTIFY_HELDENIANEND:
		case DEF_NOTIFY_RESURRECTPLAYER:
		case DEF_NOTIFY_SLATE_EXP:
		case DEF_NOTIFY_SLATE_MANA:
		case DEF_NOTIFY_SLATE_INVINCIBLE:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SLATE_CREATEFAIL:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SLATE_CREATESUCCESS:
			dwp = (uint32_t *) cp;
			*dwp = sV1;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

			// New 07/05/2004
			// Party Notify Msg's
		case DEF_NOTIFY_PARTY:

			switch (sV1) {
				case 4:
				case 6:
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV1;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV2;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV3;
					cp += 2;
					memcpy(cp, pString, 10);
					cp += 10;
					iRet = this->m_pXSock->iSendMsg(cData, 12 + 10);
					this->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
					break;

				case 5:
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV1;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV2;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV3;
					cp += 2;
					memcpy(cp, pString, sV3 * 11);
					cp += sV3 * 11;
					iRet = this->m_pXSock->iSendMsg(cData, 12 + sV3 * 11);
					this->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
					break;

				default:
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV1;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV2;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV3;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = (uint16_t) sV4;
					cp += 2;
					iRet = this->m_pXSock->iSendMsg(cData, 14);
					this->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
					break;
			}
			break;

		case DEF_NOTIFY_REQGUILDNAMEANSWER:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 30);
			break;

			// New 06/05/2004
			// Upgrade Notify Msg's
		case DEF_NOTIFY_ITEMUPGRADEFAIL:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_ITEMATTRIBUTECHANGE:
		case DEF_NOTIFY_GIZONITEMUPGRADELEFT:
			sp = (short *) cp;
			*sp = sV1;
			cp += 2;

			dwp = (uint32_t *) cp;
			*dwp = sV2;
			cp += 4;

			dwp = (uint32_t *) cp;
			*dwp = sV3;
			cp += 4;

			dwp = (uint32_t *) cp;
			*dwp = sV4;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 20);
			break;

		case DEF_NOTIFY_UPGRADEHEROCAPE:
		case DEF_NOTIFY_GIZONITEMCANGE:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			*cp = (char) sV2;
			cp++;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV4;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV5;
			cp += 2;

			*cp = (char) sV6;
			cp++;

			*cp = (char) sV7;
			cp++;

			dwp = (uint32_t *) cp;
			*dwp = sV8;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 41);
			break;

			// 2.06 - by KLKS
		case DEF_NOTIFY_CHANGEPLAYMODE:
			memcpy(cp, pString, 10);
			cp += 10;
			iRet = this->m_pXSock->iSendMsg(cData, 16);
			break;
			//

		case DEF_NOTIFY_TCLOC:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV4;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV5;
			cp += 2;

			memcpy(cp, pString2, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 34);
			break;

			//New 11/05/2004
		case DEF_NOTIFY_GRANDMAGICRESULT:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV3;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV4;
			cp += 2;

			if (sV9 > 0) {
				memcpy(cp, pString2, (sV9 * 2) + 2);
				cp += (sV9 * 2) + 2;
			} else {
				sp = (short *) cp;
				*sp = 0;
				cp += 2;
			}
			iRet = this->m_pXSock->iSendMsg(cData, (sV9 * 2) + 26);
			break;

		case DEF_NOTIFY_MAPSTATUSNEXT:
			memcpy(cp, pString, sV1);
			cp += sV1;
			iRet = this->m_pXSock->iSendMsg(cData, 6 + sV1);
			break;

		case DEF_NOTIFY_MAPSTATUSLAST:
			memcpy(cp, pString, sV1);
			cp += sV1;
			iRet = this->m_pXSock->iSendMsg(cData, 6 + sV1);
			break;

		case DEF_NOTIFY_LOCKEDMAP:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 18);
			break;

		case DEF_NOTIFY_BUILDITEMSUCCESS:
		case DEF_NOTIFY_BUILDITEMFAIL:

			if (sV1 >= 0) {
				sp = (short *) cp;
				*sp = (short) sV1;
				cp += 2;
			} else {
				sp = (short *) cp;
				*sp = (short) sV1 + 10000;
				cp += 2;
			}

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_HELP:
		case DEF_NOTIFY_QUESTREWARD:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV3;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 38);
			break;

		case DEF_NOTIFY_CANNOTCONSTRUCT:
		case DEF_NOTIFY_METEORSTRIKECOMING:
		case DEF_NOTIFY_METEORSTRIKEHIT:
		case DEF_NOTIFY_HELPFAILED:
		case DEF_NOTIFY_SPECIALABILITYENABLED:
		case DEF_NOTIFY_FORCEDISCONN:
		case DEF_NOTIFY_OBSERVERMODE:
		case DEF_NOTIFY_QUESTCOMPLETED:
		case DEF_NOTIFY_QUESTABORTED:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_QUESTCONTENTS:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV4;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV5;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV9;
			cp += 2;

			if (pString2 != nullptr) memcpy(cp, pString2, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 44);
			break;

		case DEF_NOTIFY_ENERGYSPHERECREATED:
		case DEF_NOTIFY_ITEMCOLORCHANGE:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_NOMORECRUSADESTRUCTURE:
		case DEF_NOTIFY_EXCHANGEITEMCOMPLETE:
		case DEF_NOTIFY_CANCELEXCHANGEITEM:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SETEXCHANGEITEM:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			*cp = (char) sV5;
			cp++;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			memcpy(cp, game_.m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			// v1.42
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV9;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 57);
			break;

		case DEF_NOTIFY_OPENEXCHANGEWINDOW:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			*cp = (char) sV5;
			cp++;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			memcpy(cp, game_.m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			// v1.42
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV9;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 57);
			break;

		case DEF_NOTIFY_NOTFLAGSPOT:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_ITEMPOSLIST:
			for (i = 0; i < DEF_MAXITEMS; i++) {
				sp = (short *) cp;
				*sp = (short) this->m_ItemPosList[i].x;
				cp += 2;
				sp = (short *) cp;
				*sp = (short) this->m_ItemPosList[i].y;
				cp += 2;
			}
			iRet = this->m_pXSock->iSendMsg(cData, 6 + DEF_MAXITEMS * 4);
			break;

		case DEF_NOTIFY_ENEMYKILLS:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_CRUSADE:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			ip = (int *) cp;
			*ip = (int) sV3;
			cp += 4;

			ip = (int *) cp;
			*ip = (int) sV4;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 22);
			break;

		case DEF_NOTIFY_CONSTRUCTIONPOINT:
		case DEF_NOTIFY_SPECIALABILITYSTATUS:
		case DEF_NOTIFY_DAMAGEMOVE:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 12);
			break;

		case DEF_NOTIFY_DOWNSKILLINDEXSET:
		case DEF_NOTIFY_RESPONSE_CREATENEWPARTY:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_ADMINIFO:
			switch (sV1) {
				case 1: {
					auto &npc = *game_.m_pNpcList[sV2];
					ip = (int *) cp;
					*ip = npc.m_iHP;
					cp += 4;

					ip = (int *) cp;
					*ip = npc.m_iDefenseRatio;
					cp += 4;

					ip = (int *) cp;
					*ip = npc.m_bIsSummoned;
					cp += 4;

					ip = (int *) cp;
					*ip = npc.m_cActionLimit;
					cp += 4;

					ip = (int *) cp;
					*ip = npc.m_iHitDice;
					cp += 4;

					dwp = (uint32_t *) cp; // v1.4
					*dwp = npc.m_dwDeadTime;
					cp += 4;

					dwp = (uint32_t *) cp;
					*dwp = npc.m_dwRegenTime;
					cp += 4;

					ip = (int *) cp;
					*ip = (int) npc.m_bIsKilled;
					cp += 4;

					iRet = this->m_pXSock->iSendMsg(cData, 26 + 12);
				} break;
			}
			break;

		case DEF_NOTIFY_HELDENIANSTART:
		case DEF_NOTIFY_NPCTALK:
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV3;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV4;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV5;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV6;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV7;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV8;
			cp += 2;

			sp = (short *) cp;
			*sp = (short) sV9;
			cp += 2;

			if (pString != nullptr) memcpy(cp, pString, 20);
			cp += 20;

			if (pString2 != nullptr) memcpy(cp, pString2, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 64);
			break;

		case DEF_NOTIFY_CRAFTING_SUCCESS: //reversed by Snoopy: 0x0BF0
		case DEF_NOTIFY_PORTIONSUCCESS:
		case DEF_NOTIFY_LOWPORTIONSKILL:
		case DEF_NOTIFY_PORTIONFAIL:
		case DEF_NOTIFY_NOMATCHINGPORTION:

			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SUPERATTACKLEFT:
			sp = (short *) cp;
			*sp = this->m_iSuperAttackLeft;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_SAFEATTACKMODE:
			*cp = this->m_bIsSafeAttackMode;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_QUERY_JOINPARTY:
		case DEF_NOTIFY_IPACCOUNTINFO:
			strcpy(cp, pString);
			cp += strlen(pString);

			iRet = this->m_pXSock->iSendMsg(cData, 6 + strlen(pString) + 1);
			break;

		case DEF_NOTIFY_REWARDGOLD:
			dwp = (uint32_t *) cp;
			*dwp = this->m_iRewardGold;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_SERVERSHUTDOWN:
			*cp = (char) sV1;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_GLOBALATTACKMODE:
		case DEF_NOTIFY_WHETHERCHANGE:
			*cp = (char) sV1;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_FISHCANCELED:
		case DEF_NOTIFY_FISHSUCCESS:
		case DEF_NOTIFY_FISHFAIL:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_DEBUGMSG:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_FISHCHANCE:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_ENERGYSPHEREGOALIN:
		case DEF_NOTIFY_EVENTFISHMODE:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV3;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 32);
			break;

		case DEF_NOTIFY_NOTICEMSG:
			memcpy(cp, pString, strlen(pString));
			cp += strlen(pString);

			*cp = 0;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, strlen(pString) + 7);
			break;

		case DEF_NOTIFY_CANNOTRATING:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_RATINGPLAYER:
			*cp = (char) sV1;
			cp++;

			memcpy(cp, pString, 10);
			cp += 10;

			ip = (int *) cp;
			*ip = this->m_iRating;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 22);
			break;

		case DEF_NOTIFY_ADMINUSERLEVELLOW:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_PLAYERSHUTUP:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			memcpy(cp, pString, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 19);
			break;

		case DEF_NOTIFY_TIMECHANGE:
			*cp = (char) sV1;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_TOBERECALLED:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_HUNGER:
			*cp = (char) sV1;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 7);
			break;

		case DEF_NOTIFY_PLAYERPROFILE:
			if (strlen(pString) > 100) {
				memcpy(cp, pString, 100);
				cp += 100;
			} else {
				memcpy(cp, pString, strlen(pString));
				cp += strlen(pString);
			}
			*cp = 0;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 7 + strlen(pString));
			break;

			// New 10/05/2004 Changed
		case DEF_NOTIFY_WHISPERMODEON:
		case DEF_NOTIFY_WHISPERMODEOFF:
		case DEF_NOTIFY_PLAYERNOTONGAME:
			memcpy(cp, pString, 10);
			cp += 10;

			memcpy(cp, "          ", 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 27);
			break;

			// New 15/05/2004 Changed
		case DEF_NOTIFY_PLAYERONGAME:
			memcpy(cp, pString, 10);
			cp += 10;

			if (pString[0] != 0) {
				memcpy(cp, pString2, 14);
				cp += 14;
			}

			iRet = this->m_pXSock->iSendMsg(cData, 31);
			break;

			// New 06/05/2004
		case DEF_NOTIFY_ITEMSOLD:
		case DEF_NOTIFY_ITEMREPAIRED:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV1;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 14);
			break;

			// New 06/05/2004
		case DEF_NOTIFY_REPAIRITEMPRICE:
		case DEF_NOTIFY_SELLITEMPRICE:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV1;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV3;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV4;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 42);
			break;

		case DEF_NOTIFY_CANNOTREPAIRITEM:
		case DEF_NOTIFY_CANNOTSELLITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 30);

			break;

		case DEF_NOTIFY_SHOWMAP:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_SKILLUSINGEND:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_TOTALUSERS:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) game_.m_iTotalGameServerClients; //_iGetTotalClients();
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 8);
			break;

		case DEF_NOTIFY_MAGICEFFECTOFF:
		case DEF_NOTIFY_MAGICEFFECTON:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV3;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 16);
			break;

		case DEF_NOTIFY_CANNOTITEMTOBANK:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_SERVERCHANGE:
			memcpy(cp, this->m_cMapName, 10);
			cp += 10;
			if (game_.m_iGameServerMode == 1)
				if (memcmp(game_.m_cLogServerAddr, game_.m_cGameServerAddr, 15) == 0)
					memcpy(cp, game_.m_cGameServerAddrExternal, 15);
				else
					memcpy(cp, game_.m_cLogServerAddr, 15);
			else
				memcpy(cp, game_.m_cLogServerAddr, 15);
			cp += 15;
			ip = (int *) cp;
			*ip = game_.m_iLogServerPort;
			cp += 4;
			iRet = this->m_pXSock->iSendMsg(cData, 16 + 19);
			break;

		case DEF_NOTIFY_SKILL:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_SETITEMCOUNT:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) sV2;
			cp += 4;

			*cp = (char) sV3;
			cp++;

			iRet = this->m_pXSock->iSendMsg(cData, 13);
			break;

		case DEF_NOTIFY_ITEMDEPLETED_ERASEITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 12);
			break;

		case DEF_NOTIFY_DROPITEMFIN_ERASEITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 12);
			break;

		case DEF_NOTIFY_CANNOTGIVEITEM:
		case DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 32);
			break;

		case DEF_NOTIFY_GIVEITEMFIN_ERASEITEM:
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;

			ip = (int *) cp;
			*ip = (int) sV2;
			cp += 4;

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 32);
			break;

		case DEF_NOTIFY_ENEMYKILLREWARD:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iExp;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iEnemyKillCount;
			cp += 4;
			memcpy(cp, game_.m_pClientList[sV1]->m_cCharName, 10);
			cp += 10;
			memcpy(cp, game_.m_pClientList[sV1]->m_cGuildName, 20);
			cp += 20;
			sp = (short *) cp;
			*sp = (short) game_.m_pClientList[sV1]->m_iGuildRank;
			cp += 2;
			sp = (short *) cp;
			*sp = (short) this->m_iWarContribution;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 48);
			break;

		case DEF_NOTIFY_PKCAPTURED:


			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV1;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = (uint16_t) sV2;
			cp += 2;
			memcpy(cp, pString, 10);
			cp += 10;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iRewardGold;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = this->m_iExp;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 28);
			break;

		case DEF_NOTIFY_PKPENALTY:

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iExp;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iStr;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iVit;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iDex;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iInt;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iMag;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iCharisma;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iPKCount;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 38);
			break;

		case DEF_NOTIFY_TRAVELERLIMITEDLEVEL:
		case DEF_NOTIFY_LIMITEDLEVEL:

			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iExp;
			cp += 4;
			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_ITEMRELEASED:
		case DEF_NOTIFY_ITEMLIFESPANEND:

			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;
			sp = (short *) cp;
			*sp = (short) sV2;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_KILLED:

			memcpy(cp, pString, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 26);
			break;

		case DEF_NOTIFY_EXP:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iExp;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iRating;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 14);
			break;

		case DEF_NOTIFY_HP: // Fixed Party Info by xXx// 50cent
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iHP;
			cp += 4;
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iMP;
			cp += 4;

			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				int iH = game_.m_stPartyInfo[this->m_iPartyID].iIndex[i];
				if (game_.m_pClientList[iH] != nullptr) {
					game_.m_pClientList[iH]->SendNotifyMsg(0,DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = this->m_pXSock->iSendMsg(cData, 14);
			break;

		case DEF_NOTIFY_PARTYMEMBERSTATUS:// 50 cent
			for (i = 0; i < game_.m_stPartyInfo[this->m_iPartyID].iTotalMembers; i++) {
				ip = (int *) cp;
				*ip = (uint32_t) game_.m_pClientList[game_.m_stPartyInfo[this->m_iPartyID].iIndex[i]]->m_iHP;
				cp += 4;

				ip = (int *) cp;
				*ip = (uint32_t) game_.iGetMaxHP(game_.m_stPartyInfo[this->m_iPartyID].iIndex[i]);
				cp += 4;

				ip = (int *) cp;
				*ip = (uint32_t) game_.m_pClientList[game_.m_stPartyInfo[this->m_iPartyID].iIndex[i]]->m_iMP;
				cp += 4;

				ip = (int *) cp;
				*ip = (uint32_t) game_.iGetMaxMP(game_.m_stPartyInfo[this->m_iPartyID].iIndex[i]);
				cp += 4;
				//screemin fix/update :D -  Poison mode
				ip = (int *) cp;
				*ip = (uint32_t) game_.m_pClientList[game_.m_stPartyInfo[this->m_iPartyID].iIndex[i]]->m_bIsPoisoned;
				cp += 2;

			}
			iRet = this->m_pXSock->iSendMsg(cData, 6 + (game_.m_stPartyInfo[this->m_iPartyID].iTotalMembers * 18));
			break;

		case DEF_NOTIFY_MP: // Fixed Party Info by xXx
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iMP;
			cp += 4;

			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				int iH = game_.m_stPartyInfo[this->m_iPartyID].iIndex[i];
				if (game_.m_pClientList[iH] != nullptr) {
					game_.m_pClientList[iH]->SendNotifyMsg(0,DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;



		case DEF_NOTIFY_SP: // Fixed Party Info by xXx
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iSP;
			cp += 4;

			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				int iH = game_.m_stPartyInfo[this->m_iPartyID].iIndex[i];
				if (game_.m_pClientList[iH] != nullptr) {
					game_.m_pClientList[iH]->SendNotifyMsg(0,DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

		case DEF_NOTIFY_CHARISMA:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iCharisma;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;

			//MOG Fixes
		case DEF_NOTIFY_STATECHANGE_FAILED:
		case DEF_NOTIFY_SETTING_FAILED:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;

		case DEF_NOTIFY_STATECHANGE_SUCCESS:
		{
			int i;

			for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
				*cp = this->m_cMagicMastery[i];
				cp++;
			}

			for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
				*cp = this->m_cSkillMastery[i];
				cp++;
			}

			iRet = this->m_pXSock->iSendMsg(cData, 6 + DEF_MAXMAGICTYPE + DEF_MAXSKILLTYPE);
		}
			break;

		case DEF_NOTIFY_SETTING_SUCCESS:
		case DEF_NOTIFY_LEVELUP:
			ip = (int *) cp;
			*ip = this->m_iLevel;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iStr;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iVit;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iDex;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iInt;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iMag;
			cp += 4;

			ip = (int *) cp;
			*ip = this->m_iCharisma;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 34);
			break;

			/*
		case DEF_NOTIFY_LEVELUP:
			ip  = (int *)cp;
			 *ip = to.m_iLevel;
			cp += 4;

			ip   = (int *)cp;
			 *ip  = to.m_iStr;
			cp  += 4;


			ip   = (int *)cp;
			 *ip  = to.m_iDex;
			cp  += 4;

			ip   = (int *)cp;
			 *ip  = to.m_iInt;
			cp  += 4;

			ip   = (int *)cp;
			 *ip  = to.m_iMag;
			cp  += 4;

			ip   = (int *)cp;
			 *ip  = to.m_iCharisma;
			cp  += 4;

			iRet = to.m_pXSock->iSendMsg(cData, 34);
			break;
			 */
		case DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
		case DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION:
		case DEF_NOTIFY_CANNOTJOINMOREGUILDSMAN:

			memcpy(cp, game_.m_pClientList[iFromH]->m_cCharName, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 16);
			break;

		case DEF_COMMONTYPE_JOINGUILDAPPROVE:
			if (game_.m_pClientList[iFromH] != nullptr)
				memcpy(cp, game_.m_pClientList[iFromH]->m_cGuildName, 20);
			else memcpy(cp, "?", 1);
			cp += 20;

			sp = (short *) cp;
			*sp = DEF_GUILDSTARTRANK;
			cp += 2;

			iRet = this->m_pXSock->iSendMsg(cData, 28);
			break;

		case DEF_COMMONTYPE_JOINGUILDREJECT:
		case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
		case DEF_COMMONTYPE_DISMISSGUILDREJECT:
			if (game_.m_pClientList[iFromH] != nullptr)
				memcpy(cp, game_.m_pClientList[iFromH]->m_cGuildName, 20);
			else memcpy(cp, "?", 1);
			cp += 20;

			sp = (short *) cp;
			*sp = DEF_GUILDSTARTRANK;
			cp += 2;

			memcpy(cp, this->m_cLocation, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 38);
			break;

		case DEF_NOTIFY_GUILDDISBANDED:

			memcpy(cp, pString, 20);
			cp += 20;

			memcpy(cp, this->m_cLocation, 10);
			cp += 10;

			iRet = this->m_pXSock->iSendMsg(cData, 36);
			break;


		case DEF_NOTIFY_FIGHTZONERESERVE:
			ip = (int *) cp;
			*ip = (int) sV1;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;


		case DEF_NOTIFY_NOGUILDMASTERLEVEL:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;


		case DEF_NOTIFY_CANNOTBANGUILDMAN:
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			break;
	}

	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:


			//DeleteClient(iToH, true, true);
			return;
	}
}

