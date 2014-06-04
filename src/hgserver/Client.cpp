#include "Client.h"
#include "Game.h"
#include <vector>
#include <algorithm>
#include "WINMAIN.H"
#include <cstring>
extern char G_cTxt[512];
extern char G_cData50000[50000];

CClient::CClient(CGame &game, int index, std::unique_ptr<XSocket> &&socket) : game_(game), id_(index), m_pXSock(std::move(socket)) {
	std::memset(m_cProfile, 0, sizeof (m_cProfile));
	strcpy(m_cProfile, "__________");
	std::memset(m_cCharName, 0, sizeof (m_cCharName));
	std::memset(m_cAccountName, 0, sizeof (m_cAccountName));
	std::memset(m_cAccountPassword, 0, sizeof (m_cAccountPassword));
	std::memset(m_cGuildName, 0, sizeof (m_cGuildName));
	std::memset(m_cLocation, 0, sizeof (m_cLocation));
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
	std::memset(m_cIPaddress, 0, sizeof (m_cIPaddress));
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
	std::memset(m_cReqJoinPartyName, 0, sizeof (m_cReqJoinPartyName));
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
	std::memset(m_cExchangeItemName, 0, sizeof (m_cExchangeItemName));
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
	std::memset(m_cSendingMapName, 0, sizeof (m_cSendingMapName));
	m_iConstructionPoint = 0;
	std::memset(m_cConstructMapName, 0, sizeof (m_cConstructMapName));
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
		std::memset(m_stPartyMemberName[i].cName, 0, sizeof (m_stPartyMemberName[i].cName));
	}
	return true;
}

void CClient::ClientKilledHandler(int iAttackerH, char cAttackerType, short sDamage) {
	char cAttackerName[21], cEKMsg[1000];
	short sAttackerWeapon;
	bool bIsSAattacked = false;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;


	if (memcmp(map_->m_cName, "fight", 5) == 0) {
		this->m_dwFightzoneDeadTime = timeGetTime();
		wsprintf(G_cTxt, "Fightzone Dead Time: %d", this->m_dwFightzoneDeadTime);
		PutLogList(G_cTxt);
	}
	this->m_bIsKilled = true;

	this->m_iHP = 0;

	if (this->m_bIsExchangeMode == true) {
		auto with = this->exchangingWith_.lock();
		if (with) {
			with->_ClearExchangeStatus();
		}
		this->_ClearExchangeStatus();
	}
	this->RemoveFromTarget();
	std::memset(cAttackerName, 0, sizeof (cAttackerName));
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER_INDIRECT:
		case DEF_OWNERTYPE_PLAYER:
			if (game_.m_pClientList[iAttackerH] != nullptr)
				memcpy(cAttackerName, game_.m_pClientList[iAttackerH]->m_cCharName, 10);
			break;
		case DEF_OWNERTYPE_NPC:
			if (game_.m_pNpcList[iAttackerH] != nullptr)
#ifdef DEF_LOCALNPCNAME
				wsprintf(cAttackerName, "NPCNPCNPC@%d", m_pNpcList[iAttackerH]->m_sType);
#else
				memcpy(cAttackerName, game_.m_pNpcList[iAttackerH]->m_cNpcName, 20);
#endif
			break;
		default:
			break;
	}
	this->SendNotifyMsg(0, DEF_NOTIFY_KILLED, 0, 0, 0, cAttackerName);

	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		sAttackerWeapon = ((game_.m_pClientList[iAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
	} else sAttackerWeapon = 1;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
	this->map_->ClearOwner(12, id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
	this->map_->SetDeadOwner(id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
	if (this->map_->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;
	if ((this->map_ != nullptr) &&
			  (game_.m_bIsHeldenianMode == true) &&
			  (this->map_->m_bIsHeldenianMap == true)) {
		if (this->m_cSide == 1) {
			game_.m_iHeldenianAresdenDead++;
		} else if (this->m_cSide == 2) {
			game_.m_iHeldenianElvineDead++;
		}
		game_.UpdateHeldenianStatus();
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		// v1.432

		switch (game_.m_pClientList[iAttackerH]->m_iSpecialAbilityType) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				bIsSAattacked = true;
				break;
		}
		if (iAttackerH == id_) return;
		if (memcmp(this->m_cLocation, "NONE", 4) == 0) {

			if (this->m_iPKCount == 0) {
				game_.ApplyPKpenalty(iAttackerH, id_);
			} else {
				game_.PK_KillRewardHandler(iAttackerH, id_);
			}
		} else {

			if (this->m_iGuildRank == -1) {
				if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {

					if (this->m_iPKCount == 0) {
						game_.ApplyPKpenalty(iAttackerH, id_);
					} else {

					}
				} else {
					if (memcmp(this->m_cLocation, game_.m_pClientList[iAttackerH]->m_cLocation, 10) == 0) {
						if (this->m_iPKCount == 0) {

							game_.ApplyPKpenalty(iAttackerH, id_);
						} else {

							game_.PK_KillRewardHandler(iAttackerH, id_);
						}
					} else {

						this->EnemyKillRewardHandler(iAttackerH);
					}
				}
			} else {

				if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {

					if (this->m_iPKCount == 0) {

						game_.ApplyPKpenalty(iAttackerH, id_);
					} else {

					}
				} else {
					if (memcmp(this->m_cLocation, game_.m_pClientList[iAttackerH]->m_cLocation, 10) == 0) {
						if (this->m_iPKCount == 0) {

							game_.ApplyPKpenalty(iAttackerH, id_);
						} else {

							game_.PK_KillRewardHandler(iAttackerH, id_);
						}
					} else {

						this->EnemyKillRewardHandler(iAttackerH);
					}
				}
			}
		}
		if (this->m_iPKCount == 0) {
			// Innocent
			if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0) {

				//this->m_iExp -= iDice(1, 100);
				//if (this->m_iExp < 0) this->m_iExp = 0;
				//this->SendNotifyMsg(nullptr,DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
			} else {
				if (memcmp(game_.m_pClientList[iAttackerH]->m_cLocation, this->m_cLocation, 10) == 0) {
					//this->m_iExp -= iDice(1, 100);
					//if (this->m_iExp < 0) this->m_iExp = 0;
					//this->SendNotifyMsg(nullptr,DEF_NOTIFY_EXP, nullptr, nullptr, nullptr, nullptr);
				} else {
					this->ApplyCombatKilledPenalty(2, bIsSAattacked);
				}
			}
		} else if ((this->m_iPKCount >= 1) && (this->m_iPKCount <= 3)) {
			// Criminal
			this->ApplyCombatKilledPenalty(3, bIsSAattacked);
		} else if ((this->m_iPKCount >= 4) && (this->m_iPKCount <= 11)) {
			// Murderer
			this->ApplyCombatKilledPenalty(6, bIsSAattacked);
		} else if ((this->m_iPKCount >= 12)) {
			// Slaughterer
			this->ApplyCombatKilledPenalty(12, bIsSAattacked);
		}
	} else if (cAttackerType == DEF_OWNERTYPE_NPC) {
		game_._bPKLog(DEF_PKLOG_BYNPC, id_, 0, cAttackerName);
		if (this->m_iPKCount == 0) {
			// Innocent
			this->ApplyCombatKilledPenalty(1, bIsSAattacked);
		} else if ((this->m_iPKCount >= 1) && (this->m_iPKCount <= 3)) {
			// Criminal
			this->ApplyCombatKilledPenalty(3, bIsSAattacked);
		} else if ((this->m_iPKCount >= 4) && (this->m_iPKCount <= 11)) {
			// Murderer
			this->ApplyCombatKilledPenalty(6, bIsSAattacked);
		} else if ((this->m_iPKCount >= 12)) {
			// Slaughterer
			this->ApplyCombatKilledPenalty(12, bIsSAattacked);
		}

		if (game_.m_pNpcList[iAttackerH]->m_iGuildGUID != 0) {
			if (game_.m_pNpcList[iAttackerH]->m_cSide != this->m_cSide) {
				for (auto &iterClient : game_.m_pClientList) {
					if ((iterClient.m_iGuildGUID == game_.m_pNpcList[iAttackerH]->m_iGuildGUID) && (iterClient.m_iCrusadeDuty == 3)) {
						iterClient.m_iConstructionPoint += (this->m_iLevel / 2);
						if (iterClient.m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
							iterClient.m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
						//testcode
						wsprintf(G_cTxt, "Enemy Player Killed by Npc! Construction +%d", (this->m_iLevel / 2));
						PutLogList(G_cTxt);

						iterClient.SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, iterClient.m_iConstructionPoint, iterClient.m_iWarContribution, 0, nullptr);
						return;
					}
				}
			}
		}
	} else if (cAttackerType == DEF_OWNERTYPE_PLAYER_INDIRECT) {
		game_._bPKLog(DEF_PKLOG_BYOTHER, id_, 0, nullptr);
	}
	std::memset(cEKMsg, 0, sizeof (cEKMsg));
	constexpr const char *msgs[] = {
		"%s whooped %s's ass!",
		"%s smashed %s's face into the ground!",
		"%s was sliced to pieces by %s",
		"%s says LAG LAG!! but gets PWNED by %s",
		"%s sent %s off too pie heaven!",
		"%s got beat by %s's ugly stick!",
		"%s OwneD! %s",
		"%s Sended %s To Revival Zone! Too Bad ;(",
		"%s says: I CAN OWN YOU! But gets OWNED by %s",
		"%s KilleD %s"
	};
	wsprintf(cEKMsg, msgs[iDice(1, 10) - 1], cAttackerName, this->m_cCharName);
	for (auto &iterClient : game_.m_pClientList) {
		iterClient.SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cEKMsg);
	}
	wsprintf(G_cTxt, "%s killed %s", cAttackerName, this->m_cCharName);
	PutLogFileList(G_cTxt);
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
			sp = (short *) cp;
			*sp = (short) sV1;
			cp += 2;
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
				case 1:
				{
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
				}
					break;
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
			*wp = (uint16_t) game_.m_pClientList.getTotalClients();
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
			memcpy(cp, this->map_->m_cName, 10);
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
					game_.m_pClientList[iH]->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = this->m_pXSock->iSendMsg(cData, 14);
			break;
		case DEF_NOTIFY_PARTYMEMBERSTATUS:
		{
			auto &partyInfo = game_.m_stPartyInfo[this->m_iPartyID];
			for (i = 0; i < partyInfo.iTotalMembers; i++) {
				auto &partyMember = *game_.m_pClientList[partyInfo.iIndex[i]];
				ip = (int *) cp;
				*ip = (uint32_t) partyMember.m_iHP;
				cp += 4;
				ip = (int *) cp;
				*ip = (uint32_t) partyMember.iGetMaxHP();
				cp += 4;
				ip = (int *) cp;
				*ip = (uint32_t) partyMember.m_iMP;
				cp += 4;
				ip = (int *) cp;
				*ip = (uint32_t) partyMember.iGetMaxMP();
				cp += 4;
				ip = (int *) cp;
				*ip = (uint32_t) partyMember.m_bIsPoisoned;
				cp += 2;
			}
			iRet = this->m_pXSock->iSendMsg(cData, 6 + partyInfo.iTotalMembers * 18);
		}
			break;
		case DEF_NOTIFY_MP:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iMP;
			cp += 4;
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				int iH = game_.m_stPartyInfo[this->m_iPartyID].iIndex[i];
				if (game_.m_pClientList[iH] != nullptr) {
					game_.m_pClientList[iH]->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
				}
			}
			iRet = this->m_pXSock->iSendMsg(cData, 10);
			break;
		case DEF_NOTIFY_SP:
			dwp = (uint32_t *) cp;
			*dwp = (uint32_t) this->m_iSP;
			cp += 4;
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
				int iH = game_.m_stPartyInfo[this->m_iPartyID].iIndex[i];
				if (game_.m_pClientList[iH] != nullptr) {
					game_.m_pClientList[iH]->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
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

int CClient::iUpgradeHeroCapeRequirements(int iItemIndex) {
	int iAfterItemID;
	int iRequiredEnemyKills;
	int iRequiredContribution;
	int iStoneNumber;
	int i;
	int iBeforeItemID;
	iAfterItemID = 0;
	iRequiredEnemyKills = 10000;
	iRequiredContribution = 10000;
	iStoneNumber = 0;
	i = 0;
	iBeforeItemID = this->m_pItemList[iItemIndex]->m_sIDnum;
	if (iBeforeItemID == 400) {
		game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, id_, (int) & * this->m_pItemList[iItemIndex], nullptr);
		iAfterItemID = 427;
		iRequiredEnemyKills = 30;
		iRequiredContribution = 50;
		iStoneNumber = 657;
	} else if (iBeforeItemID == 401) {
		game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, id_, (int) & * this->m_pItemList[iItemIndex], nullptr);
		iAfterItemID = 428;
		iRequiredEnemyKills = 30;
		iRequiredContribution = 50;
		iStoneNumber = 657;
	} else {
		return -1;
	}
	if ((iRequiredEnemyKills > this->m_iEnemyKillCount) || (iRequiredEnemyKills == 10000)) return 0;
	if ((iRequiredContribution > this->m_iContribution) || (iRequiredContribution == 10000)) return 0;
	for (i = 0; i < DEF_MAXITEMS; i++) {
		if ((this->m_pItemList[i] != nullptr) && (this->m_pItemList[i]->m_sIDnum == iStoneNumber)) break;
	}
	if ((i == 50) || (iStoneNumber == 0)) return 0;
	if (game_._bInitItemAttr(*this->m_pItemList[iItemIndex], iAfterItemID) == false) return -1;
	this->m_iEnemyKillCount -= iRequiredEnemyKills;
	this->m_iContribution -= iRequiredContribution;
	if (this->m_pItemList[i] != nullptr) {
		this->ItemDepleteHandler(i, false, true);
		return 1;
	}
	return -1;
}

void CClient::RequestItemUpgradeHandler(int iItemIndex) {
	int i;
	int iItemX;
	int iItemY;
	int iSoM;
	int iSoX;
	int iSomH;
	int iSoxH;
	int iCheckHeroCape; // v2.172
	uint32_t dwTemp;
	uint32_t dwSWEType;
	uint32_t iValue;
	double dV1;
	double dV2;
	double dV3;
	short sItemUpgrade = 2;
	if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
	auto &itemPtr = m_pItemList[iItemIndex];
	if (!itemPtr) return;
	auto &item = *itemPtr;
	if (game_.HeroItemChecker(item.m_sIDnum, 0, 0, 0) != 0) {
		if ((item.m_sTouchEffectValue1 != this->m_sCharIDnum1) ||
				  (item.m_sTouchEffectValue2 != this->m_sCharIDnum2) ||
				  (item.m_sTouchEffectValue3 != this->m_sCharIDnum3)) {
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
			return;
		}
		iCheckHeroCape = this->iUpgradeHeroCapeRequirements(iItemIndex);
		if (iCheckHeroCape > 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_UPGRADEHEROCAPE, iItemIndex,
					  item.m_cItemType,
					  item.m_wCurLifeSpan,
					  item.m_cName,
					  item.m_sSprite,
					  item.m_sSpriteFrame,
					  item.m_cItemColor,
					  item.m_sItemSpecEffectValue2,
					  item.m_dwAttribute);
			game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, -1, &item, false);
		} else {
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
			game_._bItemLog(DEF_ITEMLOG_UPGRADEFAIL, this->id_, -1, &item, false);
		}
		return;
	}
	iValue = (item.m_dwAttribute & 0xF0000000) >> 28;
	if (iValue >= 15) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, 0, 0, nullptr);
		return;
	}
	if ((item.m_sIDnum == 717) && (iValue >= 7)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, 0, 0, nullptr);
		return;
	}
	switch (item.m_cCategory) {
		case 46: // Pendants are category 46
			if (item.m_cItemType != 1) {
				this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
				return; // Pendants are type 1
			}
			if (item.m_cEquipPos < 11) {
				this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
				return; // Pendants are left finger or more
			}
			if (item.m_sItemEffectType != 14) {
				this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
				return; // Pendants are EffectType 14
			}
			switch (item.m_sItemEffectValue1) {
				default: // Other items are not upgradable
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
					return; // Pendants are EffectType 14
				case 16: // AngelicPandent(STR)
				case 17: // AngelicPandent(DEX)
				case 18: // AngelicPandent(INT)
				case 19: // AngelicPandent(MAG)
					if (this->m_iGizonItemUpgradeLeft <= 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					if (iValue >= 10) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					switch (iValue) {
						case 0: sItemUpgrade = 10;
							break;
						case 1: sItemUpgrade = 11;
							break;
						case 2: sItemUpgrade = 13;
							break;
						case 3: sItemUpgrade = 16;
							break;
						case 4: sItemUpgrade = 20;
							break;
						case 5: sItemUpgrade = 25;
							break;
						case 6: sItemUpgrade = 31;
							break;
						case 7: sItemUpgrade = 38;
							break;
						case 8: sItemUpgrade = 46;
							break;
						case 9: sItemUpgrade = 55;
							break;
						default:
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
							return;
							break;
					}
					/*if (   (item.m_sTouchEffectValue1 != this->m_sCharIDnum1)
					 || (item.m_sTouchEffectValue2 != this->m_sCharIDnum2)
					 || (item.m_sTouchEffectValue3 != this->m_sCharIDnum3))
					{ this->SendNotifyMsg(0,DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
					 return;
					}*/
					if ((this->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					int iDicePTA = iDice(1, 100);
					if (iDicePTA <= 70) {
						this->m_iGizonItemUpgradeLeft -= sItemUpgrade;
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
						iValue++;
						if (iValue > 10) iValue = 10;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					} else {
						this->m_iGizonItemUpgradeLeft--;
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
					}
					return;
					break;
			}
			break;
		case 1:
			switch (item.m_sIDnum) {
				case 703: // SangAhFlameberge
				case 709: // DarkKnightFlameberge
				case 718: // DarkKnightGreatSword
				case 727: // DarkKnightFlamebergW
				case 736: // SangAhGiantSword
				case 737: // DarkKnightGiantSword
				case 745: // BlackKnightTemple
				case 2000://BlackKnightHammer
				case 2001://BlackKnightBHammer
				case 2002://BlackKnightBarHammer
					if (this->m_iGizonItemUpgradeLeft <= 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;
					/*	if (item.m_sTouchEffectType != 0) {
							if ((item.m_sTouchEffectValue1 != this->m_sCharIDnum1) ||
								(item.m_sTouchEffectValue2 != this->m_sCharIDnum2) ||
								(item.m_sTouchEffectValue3 != this->m_sCharIDnum3)) {
								this->SendNotifyMsg(0,DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
								return;
							}
						}*/
					if ((this->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					this->m_iGizonItemUpgradeLeft -= sItemUpgrade;
					this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
					if ((iValue == 0) && (item.m_sIDnum == 703)) { // SangAhFlameberge
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 736) == false) { // SangAhGiantSword
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					} else if ((iValue == 0) && ((item.m_sIDnum == 709) || (item.m_sIDnum == 727))) { // DarkKnightFlameberge DarkKnightFlamebergW
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 737) == false) { // DarkKnightGiantSword
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					} else if ((iValue >= 6) && (item.m_sIDnum == 737)) { // DarkKnightGiantSword
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 745) == false) { // BlackKnightTemple
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					} else if ((iValue >= 12) && (item.m_sIDnum == 745)) { // BlackKnightTemple
						iValue += 2;
						if (iValue > 15) iValue = 15;
						if (iValue == 15) item.m_cItemColor = 9;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					}//50Cent - New DKHammer Upgrade
					else if ((iValue == 0) && ((item.m_sIDnum == 2000))) { // BlackKnightHammer
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 2001) == false) { // BlackKnightBHammer
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					} else if ((iValue >= 6) && (item.m_sIDnum == 2001)) { // BlackKnightBHammer
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 2002) == false) { // BlackKnightBarHammer
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					} else if ((iValue >= 12) && (item.m_sIDnum == 2002)) { // BlackKnightBarHammer
						iValue += 2;
						if (iValue > 15) iValue = 15;
						if (iValue == 15) item.m_cItemColor = 9;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					} else {
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					}
					break;
				case 717: // DarkKnightRapier
					if (this->m_iGizonItemUpgradeLeft <= 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;
					if ((item.m_sTouchEffectValue1 != this->m_sCharIDnum1) ||
							  (item.m_sTouchEffectValue2 != this->m_sCharIDnum2) ||
							  (item.m_sTouchEffectValue3 != this->m_sCharIDnum3)) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
						return;
					}
					if ((this->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					this->m_iGizonItemUpgradeLeft -= sItemUpgrade;
					this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
					iValue++;
					if (iValue > 7) iValue = 7;
					dwTemp = item.m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF;
					item.m_dwAttribute = dwTemp | (iValue << 28);
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
					game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
					break;
				default:
					if ((item.m_dwAttribute & 0x00F00000) != 0) {
						dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
						if (dwSWEType == 9) {
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
							return;
						}
					}
					iSoX = iSoM = 0;
					for (i = 0; i < DEF_MAXITEMS; i++)
						if (this->m_pItemList[i] != nullptr) {
							switch (this->m_pItemList[i]->m_sIDnum) {
								case 656: iSoX++;
									iSoxH = i;
									break;
								case 657: iSoM++;
									iSomH = i;
									break;
							}
						}
					if (iSoX > 0) {
						if (this->bCheckIsItemUpgradeSuccess(iItemIndex, iSoxH) == false) {
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							iValue = (item.m_dwAttribute & 0xF0000000) >> 28;
							if (iValue >= 1) this->ItemDepleteHandler(iItemIndex, false, true);
							this->ItemDepleteHandler(iSoxH, false, true);
							return;
						}
						if ((item.m_dwAttribute & 0x00000001) != 0) {
							iValue++;
							if (iValue > 10) {
								iValue = 10;
							} else {
								dwTemp = item.m_dwAttribute;
								dwTemp = dwTemp & 0x0FFFFFFF;
								item.m_dwAttribute = dwTemp | (iValue << 28);
								this->ItemDepleteHandler(iSoxH, false, true);
							}
						} else {
							iValue++;
							if (iValue > 7) {
								iValue = 7;
							} else {
								dwTemp = item.m_dwAttribute;
								dwTemp = dwTemp & 0x0FFFFFFF;
								item.m_dwAttribute = dwTemp | (iValue << 28);
								this->ItemDepleteHandler(iSoxH, false, true);
							}
						}
					}
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
					break;
			}
			return;
		case 3:
			if ((item.m_dwAttribute & 0x00F00000) != 0) {
				dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
				if (dwSWEType == 9) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
					return;
				}
			}
			iSoX = iSoM = 0;
			for (i = 0; i < DEF_MAXITEMS; i++)
				if (this->m_pItemList[i] != nullptr) {
					switch (this->m_pItemList[i]->m_sIDnum) {
						case 656: iSoX++;
							iSoxH = i;
							break;
						case 657: iSoM++;
							iSomH = i;
							break;
					}
				}
			if (iSoX > 0) {
				if (this->bCheckIsItemUpgradeSuccess(iItemIndex, iSoxH) == false) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
					iValue = (item.m_dwAttribute & 0xF0000000) >> 28;
					if (iValue >= 1) this->ItemDepleteHandler(iItemIndex, false, true);
					this->ItemDepleteHandler(iSoxH, false, true);
					return;
				}
				if ((item.m_dwAttribute & 0x00000001) != 0) {
					iValue++;
					if (iValue > 10) {
						iValue = 10;
					} else {
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->ItemDepleteHandler(iSoxH, false, true);
					}
				} else {
					iValue++;
					if (iValue > 7) {
						iValue = 7;
					} else {
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->ItemDepleteHandler(iSoxH, false, true);
					}
				}
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
			return;
		case 5:
			if ((item.m_dwAttribute & 0x00F00000) != 0) {
				dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
				if (dwSWEType == 8) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
					return;
				}
			}
			switch (item.m_sIDnum) {
				case 623:
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
					return;
				default:
					break;
			}
			iSoX = iSoM = 0;
			for (i = 0; i < DEF_MAXITEMS; i++)
				if (this->m_pItemList[i] != nullptr) {
					switch (this->m_pItemList[i]->m_sIDnum) {
						case 656: iSoX++;
							iSoxH = i;
							break;
						case 657: iSoM++;
							iSomH = i;
							break;
					}
				}
			if (iSoM > 0) {
				if (this->bCheckIsItemUpgradeSuccess(iItemIndex, iSomH, true) == false) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
					iValue = (item.m_dwAttribute & 0xF0000000) >> 28;
					if (iValue >= 1) this->ItemDepleteHandler(iItemIndex, false, true);
					this->ItemDepleteHandler(iSomH, false, true);
					return;
				}
				iValue++;
				if (iValue > 10) {
					iValue = 10;
				} else {
					dwTemp = item.m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF;
					item.m_dwAttribute = dwTemp | (iValue << 28);
					if ((item.m_dwAttribute & 0x00000001) != 0) {
						dV1 = (double) item.m_wMaxLifeSpan;
						dV2 = 0.2f * dV1;
						dV3 = dV1 + dV2;
					} else {
						dV1 = (double) item.m_wMaxLifeSpan;
						dV2 = 0.15f * dV1;
						dV3 = dV1 + dV2;
					}
					item.m_sItemSpecEffectValue1 = (short) dV3;
					if (item.m_sItemSpecEffectValue1 < 0) item.m_sItemSpecEffectValue1 = item.m_wMaxLifeSpan;
					item.m_wMaxLifeSpan = item.m_sItemSpecEffectValue1;
					item.m_wCurLifeSpan += dV2;
					this->ItemDepleteHandler(iSomH, false, true);
				}
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, item.m_sItemSpecEffectValue1, nullptr, item.m_sItemSpecEffectValue2);
			return;
		case 6:
		case 15:
			switch (item.m_sIDnum) {
				case 621: // MerienPlateMailM
				case 622: // MerienPlateMailW
				case 700: // SangAhHauberk
				case 701: // SangAhFullHel
				case 702: // SangAhLeggings
				case 704: // SangAhPlateMail
				case 706: // DarkKnightHauberk
				case 707: // DarkKnightFullHelm
				case 708: // DarkKnightLeggings
				case 710: // DarkKnightPlateMail
				case 711: // DarkMageHauberk
				case 712: // DarkMageChainMail
				case 713: // DarkMageLeggings
				case 716: // DarkMageLedderArmor
				case 719: // DarkMageScaleMail
				case 724: // DarkKnightHauberkW
				case 725: // DarkKnightFullHelmW
				case 726: // DarkKnightLeggingsW
				case 728: // DarkKnightPlateMailW
				case 729: // DarkMageHauberkW
				case 730: // DarkMageChainMailW
				case 731: // DarkMageLeggingsW
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
					return;
				default:
					if ((item.m_dwAttribute & 0x00F00000) != 0) {
						dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
						if (dwSWEType == 8) {
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
							return;
						}
					}
					iSoX = iSoM = 0;
					for (i = 0; i < DEF_MAXITEMS; i++)
						if (this->m_pItemList[i] != nullptr) {
							switch (this->m_pItemList[i]->m_sIDnum) {
								case 656: iSoX++;
									iSoxH = i;
									break;
								case 657: iSoM++;
									iSomH = i;
									break;
							}
						}
					if (iSoM > 0) {
						if (this->bCheckIsItemUpgradeSuccess(iItemIndex, iSomH, true) == false) {
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							iValue = (item.m_dwAttribute & 0xF0000000) >> 28;
							if (iValue >= 1) this->ItemDepleteHandler(iItemIndex, false, true);
							this->ItemDepleteHandler(iSomH, false, true);
							return;
						}
						iValue++;
						if (iValue > 10) {
							iValue = 10;
						} else {
							dwTemp = item.m_dwAttribute;
							dwTemp = dwTemp & 0x0FFFFFFF;
							item.m_dwAttribute = dwTemp | (iValue << 28);
							if ((item.m_dwAttribute & 0x00000001) != 0) {
								dV1 = (double) item.m_wMaxLifeSpan;
								dV2 = 0.2f * dV1;
								dV3 = dV1 + dV2;
							} else {
								dV1 = (double) item.m_wMaxLifeSpan;
								dV2 = 0.15f * dV1;
								dV3 = dV1 + dV2;
							}
							item.m_sItemSpecEffectValue1 = (short) dV3;
							if (item.m_sItemSpecEffectValue1 < 0) item.m_sItemSpecEffectValue1 = item.m_wMaxLifeSpan;
							item.m_wMaxLifeSpan = item.m_sItemSpecEffectValue1;
							item.m_wCurLifeSpan += dV2;
							this->ItemDepleteHandler(iSomH, false, true);
						}
					}
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, item.m_sItemSpecEffectValue1, nullptr, item.m_sItemSpecEffectValue2);
			return;
		case 8:
			switch (item.m_sIDnum) {
				case 291: // MagicWand(MS30-LLF)
				case 714: // DarkMageMagicStaff
				case 732: // DarkMageMagicStaffW
				case 738: // DarkMageMagicWand
				case 746: // BlackMageTemple
					/*if ((item.m_sTouchEffectValue1 != this->m_sCharIDnum1) ||
						(item.m_sTouchEffectValue2 != this->m_sCharIDnum2) ||
						(item.m_sTouchEffectValue3 != this->m_sCharIDnum3)) {
						this->SendNotifyMsg(0,DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, nullptr);
						return;
					}*/
					if (this->m_iGizonItemUpgradeLeft <= 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;
					if ((this->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, nullptr);
						return;
					}
					this->m_iGizonItemUpgradeLeft -= sItemUpgrade;
					this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
					if (iValue == 0) {
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
					}
					if ((iValue >= 4) && ((item.m_sIDnum == 714) || (item.m_sIDnum == 732))) {
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 738) == false) { // DarkMageMagicWand
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, item.m_cItemType,
								  item.m_wCurLifeSpan, item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					}
					if ((iValue >= 6) && (item.m_sIDnum == 738)) {
						iItemX = this->m_ItemPosList[iItemIndex].x;
						iItemY = this->m_ItemPosList[iItemIndex].y;
						item = CItem();
						this->m_ItemPosList[iItemIndex].x = iItemX;
						this->m_ItemPosList[iItemIndex].y = iItemY;
						if (game_._bInitItemAttr(item, 746) == false) { // BlackMageTemple
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							return;
						}
						item.m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
						item.m_sTouchEffectValue1 = this->m_sCharIDnum1;
						item.m_sTouchEffectValue2 = this->m_sCharIDnum2;
						item.m_sTouchEffectValue3 = this->m_sCharIDnum3;
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, item.m_cItemType,
								  item.m_wCurLifeSpan, item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					}
					if ((iValue >= 12) && (item.m_sIDnum == 746)) {
						iValue += 2;
						if (iValue > 15) iValue = 15;
						if (iValue == 15) item.m_cItemColor = 9;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
						this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
								  item.m_cItemType,
								  item.m_wCurLifeSpan,
								  item.m_cName,
								  item.m_sSprite,
								  item.m_sSpriteFrame,
								  item.m_cItemColor,
								  item.m_sItemSpecEffectValue2,
								  item.m_dwAttribute);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					} else {
						iValue += 2;
						if (iValue > 15) iValue = 15;
						dwTemp = item.m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						item.m_dwAttribute = dwTemp | (iValue << 28);
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
						game_._bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, this->id_, (int) - 1, &item);
						break;
					}
				default:
					iSoX = iSoM = 0;
					for (i = 0; i < DEF_MAXITEMS; i++)
						if (this->m_pItemList[i] != nullptr) {
							switch (this->m_pItemList[i]->m_sIDnum) {
								case 656: iSoX++;
									iSoxH = i;
									break;
								case 657: iSoM++;
									iSomH = i;
									break;
							}
						}
					if (iSoX > 0) {
						if (this->bCheckIsItemUpgradeSuccess(iItemIndex, iSoxH) == false) {
							this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
							iValue = (item.m_dwAttribute & 0xF0000000) >> 28; // v2.172
							if (iValue >= 1) this->ItemDepleteHandler(iItemIndex, false, true);
							this->ItemDepleteHandler(iSoxH, false, true);
							return;
						}
						iValue++;
						if (iValue > 7)
							iValue = 7;
						else {
							dwTemp = item.m_dwAttribute;
							dwTemp = dwTemp & 0x0FFFFFFF;
							item.m_dwAttribute = dwTemp | (iValue << 28);
							this->ItemDepleteHandler(iSoxH, false, true);
						}
					}
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
					break;
			}
			break;
		default:
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, item.m_dwAttribute, 0, nullptr);
			break;
	}
}

void CClient::ItemDepleteHandler(short sItemIndex, bool bIsUseItemResult, bool bIsItemUsed) {
	if (this->m_bIsInitComplete == false) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	auto &itemPtr = this->m_pItemList[sItemIndex];
	if (!itemPtr) return;
	auto &item = *itemPtr;
	if ((bIsItemUsed) ||
			  (item.m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (item.m_cItemType == DEF_ITEMTYPE_EAT) ||
			  (item.m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
			  (item.m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) ||
			  (item.m_cItemType == DEF_ITEMTYPE_MATERIAL) ||
			  (item.m_sIDnum == 380) ||
			  (item.m_sIDnum == 381) ||
			  (item.m_sIDnum == 382)) {
		game_._bItemLog(DEF_ITEMLOG_DEPLETE, id_, -1, &item, false);
	} else if ((item.m_sIDnum == 247) ||
			  (item.m_sIDnum == 248)) {
		game_._bItemLog(DEF_ITEMLOG_DEPLETE, id_, -1, &item, false);
	}
	this->ReleaseItemHandler(sItemIndex, true);
	this->SendNotifyMsg(0, DEF_NOTIFY_ITEMDEPLETED_ERASEITEM, sItemIndex, (int) bIsUseItemResult, 0, nullptr);
	itemPtr.reset();
	this->m_bIsItemEquipped[sItemIndex] = false;
	this->m_cArrowIndex = this->_iGetArrowItemIndex();
	this->iCalcTotalWeight();
}

int CClient::iCalcTotalWeight() {
	register int i, iWeight;
	short sItemIndex;
	this->m_iAlterItemDropIndex = -1;
	for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++) {
		auto &itemPtr = this->m_pItemList[sItemIndex];
		if (itemPtr != nullptr) {
			auto &item = *itemPtr;
			switch (item.m_sItemEffectType) {
				case DEF_ITEMEFFECTTYPE_ALTERITEMDROP:
					if (item.m_wCurLifeSpan > 0) {
						this->m_iAlterItemDropIndex = sItemIndex;
					}
					break;
			}
		}
	}
	iWeight = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			iWeight += this->m_pItemList[i]->iGetItemWeight(this->m_pItemList[i]->m_dwCount);
		}
	this->m_iCurWeightLoad = iWeight;
	return iWeight;
}

bool CClient::bSetItemToBankItem(short sItemIndex) {
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
	auto &itemPtr = this->m_pItemList[sItemIndex];
	if (!itemPtr) return false;
	auto &item = *itemPtr;
	int index = 0;
	for (auto &bankItemPtr : this->m_pItemInBankList) {
		if (bankItemPtr) {
			++index;
			continue;
		}
		std::swap(bankItemPtr, itemPtr);
		this->iCalcTotalWeight();
		char cData[100];
		uint32_t * dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		uint16_t * wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMTOBANK;
		char *cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = index;
		cp++;
		*cp = index;
		cp++;
		memcpy(cp, item.m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = item.m_dwCount;
		cp += 4;
		*cp = item.m_cItemType;
		cp++;
		*cp = item.m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		short *sp = (short *) cp;
		*sp = item.m_sLevelLimit;
		cp += 2;
		*cp = item.m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = item.m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = item.m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = item.m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = item.m_sSpriteFrame;
		cp += 2;
		*cp = item.m_cItemColor;
		cp++;
		// v1.432
		sp = (short *) cp;
		*sp = item.m_sItemEffectValue2;
		cp += 2;
		// v1.42
		dwp = (uint32_t *) cp;
		*dwp = item.m_dwAttribute;
		cp += 4;
		int iRet = this->m_pXSock->iSendMsg(cData, 55);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// DeleteClient(id_, true, true);
				return true;
		}
		return true;
	}
	return false;
}

bool CClient::bSetItemToBankItem(CItem * pItem) {
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	short * sp;
	if (pItem == nullptr) return false;
	int index = 0;
	for (auto &bankItemPtr : m_pItemInBankList) {
		if (bankItemPtr) {
			++index;
			continue;
		}
		bankItemPtr.reset(pItem);
		char cData[100];
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMTOBANK;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = index;
		cp++;
		*cp = index;
		cp++;
		memcpy(cp, pItem->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;
		*cp = pItem->m_cItemType;
		cp++;
		*cp = pItem->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;
		*cp = pItem->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;
		*cp = pItem->m_cItemColor;
		cp++;
		// v1.432
		sp = (short *) cp;
		*sp = pItem->m_sItemEffectValue2;
		cp += 2;
		// v1.42
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		int iRet = this->m_pXSock->iSendMsg(cData, 55);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// DeleteClient(id_, true, true);
				return true;
		}
		return true;
	}
	return false;
}

int CClient::_iCalcMaxLoad() {
	return (this->m_iStr + this->m_iAngelicStr + this->m_iLevel) * 500;
}

void CClient::CalculateSSN_ItemIndex(short sWeaponIndex, int iValue) {
	int iOldSSN;
	int iSSNpoint;
	int iWeaponIndex;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_pItemList[sWeaponIndex] == nullptr) return;
	if (this->m_bIsKilled == true) return;
	short sSkillIndex = this->m_pItemList[sWeaponIndex]->m_sRelatedSkill;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (this->m_cSkillMastery[sSkillIndex] == 0) return;
	iOldSSN = this->m_iSkillSSN[sSkillIndex];
	this->m_iSkillSSN[sSkillIndex] += iValue;
	iSSNpoint = game_.m_iSkillSSNpoint[ this->m_cSkillMastery[sSkillIndex] + 1 ];
	if ((this->m_cSkillMastery[sSkillIndex] < 100) &&
			  (this->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {
		this->m_cSkillMastery[sSkillIndex]++;
		switch (sSkillIndex) {
			case 0: // Mining
			case 5: // Hand-Attack
			case 13: // Manufacturing
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iStr + this->m_iAngelicStr) * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 3: // Magic-Resistance
				if (this->m_cSkillMastery[sSkillIndex] > (this->m_iLevel * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 4: // Magic
			case 21: // Staff-Attack
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iMag + this->m_iAngelicMag)* 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 1: // Fishing
			case 6: // Archery
			case 7: // Short-Sword
			case 8: // Long-Sword
			case 9: // Fencing
			case 10: // Axe-Attack
			case 11: // Shield
			case 14: // Hammer
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iDex + this->m_iAngelicDex) * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 2: // Farming
			case 12: // Alchemy
			case 15:
			case 19: // Pretend-Corpse
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iInt + this->m_iAngelicInt) * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 23: // Poison-Resistance
				if (this->m_cSkillMastery[sSkillIndex] > (this->m_iVit * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			default:
				this->m_iSkillSSN[sSkillIndex] = 0;
				break;
		}
		if (this->m_iSkillSSN[sSkillIndex] == 0) {
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1) {

				iWeaponIndex = this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
				if (this->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {

					this->m_iHitRatio++;
				}
			}
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1) {

				iWeaponIndex = this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ];
				if (this->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {

					this->m_iHitRatio++;
				}
			}
		}
		if (this->m_iSkillSSN[sSkillIndex] == 0) {

			this->bCheckTotalSkillMasteryPoints(sSkillIndex);
			this->SendNotifyMsg(0, DEF_NOTIFY_SKILL, sSkillIndex, this->m_cSkillMastery[sSkillIndex], 0, nullptr);
		}
	}
}

void CClient::CalculateSSN_SkillIndex(short sSkillIndex, int iValue) {
	int iOldSSN;
	int iSSNpoint;
	int iWeaponIndex;
	if (this->m_bIsInitComplete == false) return;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_cSkillMastery[sSkillIndex] == 0) return;
#ifdef DEF_TESTSERVER
	iValue = iValue * 100;
#endif
	iOldSSN = this->m_iSkillSSN[sSkillIndex];
	this->m_iSkillSSN[sSkillIndex] += iValue;
	iSSNpoint = game_.m_iSkillSSNpoint[ this->m_cSkillMastery[sSkillIndex] + 1 ];
	if ((this->m_cSkillMastery[sSkillIndex] < 100) &&
			  (this->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {
		this->m_cSkillMastery[sSkillIndex]++;
		switch (sSkillIndex) {
			case 0:
			case 5:
			case 13:
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iStr + this->m_iAngelicStr) * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 3:
				if (this->m_cSkillMastery[sSkillIndex] > (this->m_iLevel * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 4:
			case 21:
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iMag + this->m_iAngelicMag)* 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 1:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iDex + this->m_iDex) * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 2:
			case 12:
			case 14:
			case 15:
			case 19:
				if (this->m_cSkillMastery[sSkillIndex] > ((this->m_iInt + this->m_iAngelicInt)* 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			case 23:
				if (this->m_cSkillMastery[sSkillIndex] > (this->m_iVit * 2)) {
					this->m_cSkillMastery[sSkillIndex]--;
					this->m_iSkillSSN[sSkillIndex] = iOldSSN;
				} else this->m_iSkillSSN[sSkillIndex] = 0;
				break;
			default:
				this->m_iSkillSSN[sSkillIndex] = 0;
				break;
		}
		if (this->m_iSkillSSN[sSkillIndex] == 0) {
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1) {
				iWeaponIndex = this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
				if (this->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					this->m_iHitRatio++;
				}
			}
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1) {
				iWeaponIndex = this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ];
				if (this->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					this->m_iHitRatio++;
				}
			}
		}
		if (this->m_iSkillSSN[sSkillIndex] == 0) {
			this->bCheckTotalSkillMasteryPoints(sSkillIndex);
			this->SendNotifyMsg(0, DEF_NOTIFY_SKILL, sSkillIndex, this->m_cSkillMastery[sSkillIndex], 0, nullptr);
		}
	}
}

void CClient::_ClearQuestStatus() {
	this->m_iQuest = 0;
	this->m_iQuestID = 0;
	this->m_iQuestRewardType = 0;
	this->m_iQuestRewardAmount = 0;
	this->m_bIsQuestCompleted = false;
}

int CClient::iGetMaxHP() {
	int iRet = (this->m_iVit * 3) + (this->m_iLevel * 2) + ((this->m_iStr + this->m_iAngelicStr) / 2);
	if (this->m_iSideEffect_MaxHPdown != 0)
		iRet = iRet - (iRet / this->m_iSideEffect_MaxHPdown);
	return iRet;
}

int CClient::iGetMaxMP() {
	int iRet = (2 * (this->m_iMag + this->m_iAngelicMag)) + (2 * this->m_iLevel) + ((this->m_iInt + this->m_iAngelicInt) / 2);
	return iRet;
}

int CClient::iGetMaxSP() {
	int iRet = (2 * (this->m_iStr + this->m_iAngelicStr)) + (2 * this->m_iLevel);
	return iRet;
}

void CClient::AdminOrder_GetFightzoneTicket() {
	int iReserveTime;
	int iFightzoneTN;
	int iFightzoneN;
	char cTemp[21];
	SYSTEMTIME SysTime;
	if (memcmp(this->map_->m_cName, "fightzone", 9) == 0) {
		iReserveTime = this->m_iReserveTime;
		GetLocalTime(&SysTime);
		this->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + (SysTime.wHour + 3);
		std::memset(cTemp, 0, sizeof (cTemp));
		strcpy(cTemp, (this->map_->m_cName + 9));
		iFightzoneN = this->m_iFightzoneNumber;
		iFightzoneTN = this->m_iFightZoneTicketNumber;
		this->m_iFightZoneTicketNumber = 10;
		this->m_iFightzoneNumber = atoi(cTemp);
		this->GetFightzoneTicketHandler();
		this->GetFightzoneTicketHandler();
		this->GetFightzoneTicketHandler();
		this->m_iFightzoneNumber = iFightzoneN;
		this->m_iFightZoneTicketNumber = iFightzoneTN;
		this->m_iReserveTime = iReserveTime;
	} else {
		iReserveTime = this->m_iReserveTime;
		GetLocalTime(&SysTime);
		this->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + (SysTime.wHour + 2);
		std::memset(cTemp, 0, sizeof (cTemp));
		strcpy(cTemp, (this->map_->m_cName + 9));
		iFightzoneN = this->m_iFightzoneNumber;
		iFightzoneTN = this->m_iFightZoneTicketNumber;
		this->m_iFightZoneTicketNumber = 10;
		this->m_iFightzoneNumber = 1;
		this->GetFightzoneTicketHandler();
		this->GetFightzoneTicketHandler();
		this->GetFightzoneTicketHandler();
		this->m_iFightzoneNumber = iFightzoneN;
		this->m_iFightZoneTicketNumber = iFightzoneTN;
		this->m_iReserveTime = iReserveTime;
	}
}

void CClient::GetAngelHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char *cp, cData[256], cTmpName[21];
	int iAngel;
	int iRet;
	int iEraseReq;
	int iRequiredMagesty;
	char cItemName[21];
	short * sp;
	uint16_t * wp;
	int * ip;
	uint32_t * dwp;
	if (this->m_bIsInitComplete == false) return;
	if (this->_iGetItemSpaceLeft() == 0) {
		this->SendItemNotifyMsg(DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, cp);
	cp += 20;
	ip = (int *) cp;
	iAngel = (int) *ip;
	cp += 2;
	wsprintf(G_cTxt, "PC(%s) obtained an Angel (%d).   %s(%d %d)", this->m_cCharName, iAngel, this->map_->m_cName, this->m_sX, this->m_sY);
	PutLogList(G_cTxt);
	switch (iAngel) {
		case 1:
			wsprintf(cItemName, "AngelicPandent(STR)");
			iRequiredMagesty = 5;
			break;
		case 2:
			wsprintf(cItemName, "AngelicPandent(DEX)");
			iRequiredMagesty = 5;
			break;
		case 3:
			wsprintf(cItemName, "AngelicPandent(INT)");
			iRequiredMagesty = 5;
			break;
		case 4:
			wsprintf(cItemName, "AngelicPandent(MAG)");
			iRequiredMagesty = 5;
			break;
		default:
			PutLogList("Gail asked to create a wrong item!");
			break;
	}
	CItem *pItem = new class CItem;
	if (pItem == nullptr) return;
	if (game_._bInitItemAttr(*pItem, cItemName) == true) {
		pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
		pItem->m_sTouchEffectValue1 = this->m_sCharIDnum1;
		pItem->m_sTouchEffectValue2 = this->m_sCharIDnum2;
		pItem->m_sTouchEffectValue3 = this->m_sCharIDnum3;
		if (this->_bAddClientItemList(pItem, &iEraseReq)) {
			this->m_iGizonItemUpgradeLeft -= iRequiredMagesty;
			this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
			std::memset(cData, 0, sizeof (cData));
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMOBTAINED;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char) 0;
			cp++;
			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor;
			cp++;
			*cp = (char) pItem->m_sItemSpecEffectValue2;
			cp++;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;
			if (iEraseReq == 1) delete pItem;
			iRet = this->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					return;
					break;
			}
		} else {
			this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
			this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, this->m_sX, this->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					break;
			}
		}
	} else {
		delete pItem;
		pItem = nullptr;
	}
}

void CClient::processClientMsg(uint32_t msgId, char *pData, uint32_t dwMsgSize, char cKey) {
	switch (msgId) {
		case DEF_REQUEST_RESURRECTPLAYER_YES:
			RequestResurrectPlayer(true);
			break;
		case DEF_REQUEST_RESURRECTPLAYER_NO:
			RequestResurrectPlayer(false);
			break;
		case DEF_REQUEST_CRITICALSADD:
			this->PlayerOrder_GetCrits();
			break;
		case DEF_REQUEST_SHOWBALLPOINTS:
			this->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, nullptr);
			break;
		case DEF_REQUEST_ANGEL:
			this->GetAngelHandler(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_SELLITEMLIST:
			this->RequestSellItemListHandler(pData);
			break;
		case MSGID_REQUEST_RESTART:
			this->RequestRestartHandler();
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
			break;
		case MSGID_REQUEST_PANNING:
			this->iRequestPanningMapDataRequest(pData);
			break;
		case MSGID_REQUEST_NOTICEMENT:
			this->RequestNoticementHandler(pData);
			break;
		case MSGID_BWM_COMMAND_SHUTUP:
			game_._BWM_Command_Shutup(pData);
			break;
		case MSGID_BWM_INIT:
			this->_BWM_Init(pData);
			break;
		case MSGID_REQUEST_SETITEMPOS:
			this->_SetItemPos(pData);
			break;
		case MSGID_REQUEST_FULLOBJECTDATA:
			this->RequestFullObjectData(pData);
			break;
		case MSGID_REQUEST_RETRIEVEITEM:
			this->RequestRetrieveItemHandler(pData);
			break;
		case MSGID_REQUEST_CIVILRIGHT:
			this->RequestCivilRightHandler(pData);
			break;
		case MSGID_REQUEST_TELEPORT:
			this->RequestTeleportHandler("1");
			break;
		case MSGID_REQUEST_INITPLAYER:
			this->RequestInitPlayerHandler(pData, cKey);
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
			break;
		case MSGID_REQUEST_INITDATA:
		{
			uint32_t dwTime = timeGetTime();
			// Anti Bump
			if (this->m_bIsClientConnected == true) {
				if (game_.m_pClientList[id_] == nullptr) break;
				wsprintf(G_cTxt, "(!!!) Client (%s) connection closed!. Sniffer suspect!.", this->m_cCharName);
				PutLogList(G_cTxt);
				this->map_->ClearOwner(2, id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
				game_.delayEvents_.remove(id_, DEF_OWNERTYPE_PLAYER, 0);
				this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, false);
				if ((dwTime - game_.m_dwGameTime2) > 3000) {
					this->m_bIsClientConnected = false;
					this->markedForDeletion_ = true;
				}
				break;
			} else {
				this->m_bIsClientConnected = true;
				this->RequestInitDataHandler(pData, cKey);
			}
		}
			break;
		case MSGID_COMMAND_COMMON:
			ClientCommonHandler(pData);
			break;
		case MSGID_COMMAND_MOTION:
			this->ClientMotionHandler(pData);
			break;
		case MSGID_COMMAND_CHECKCONNECTION:
			this->CheckConnectionHandler(pData);
			break;
		case MSGID_COMMAND_CHATMSG:
			this->ChatMsgHandler(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_CREATENEWGUILD:
			this->RequestCreateNewGuildHandler(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_DISBANDGUILD:
			this->RequestDisbandGuildHandler(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_FIGHTZONE_RESERVE:
			this->FightzoneReserveHandler(pData, dwMsgSize);
			break;
		case MSGID_LEVELUPSETTINGS:
			this->LevelUpSettingsHandler(pData, dwMsgSize);
			break;
		case MSGID_STATECHANGEPOINT:
			this->StateChangeHandler(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_HELDENIANTELEPORT:
			this->RequestHeldenianTeleport(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_NPCSPEAKTELEPORT:
			this->RequestNpcSpeakTeleport(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_HELDENIAN_SCROLL:
			this->RequestHeldenianScroll(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_CITYHALLTELEPORT:
			if (memcmp(this->m_cLocation, "aresden", 7) == 0) {
				RequestTeleportHandler("2   ", "elvine", 205, 63);
			} else if (memcmp(this->m_cLocation, "elvine", 6) == 0) {
				RequestTeleportHandler("2   ", "aresden", 49, 49);
			}
			break;
		default:
		{
			char m_msgBuff[1000];
			wsprintf(m_msgBuff, "Unknown message received! (0x%.8X) Delete Client", msgId);
			PutLogList(m_msgBuff);
			this->DeleteClient(true, true); // v1.4
		}
			break;
	}
}

void CClient::RequestResurrectPlayer(bool bResurrect) {
	if (bResurrect == false) {
		this->m_bIsBeingResurrected = false;
		return;
	}
	char buff[100];
	if (this->m_bIsBeingResurrected == false) {
		wsprintf(buff, "(!!!) Player(%s) Tried To Use Resurrection Hack", this->m_cCharName);
		PutHackLogFileList(buff);
		this->DeleteClient(true, true, true, true);
		return;
	}
	wsprintf(buff, "(*) Resurrect Player! %s", this->m_cCharName);
	PutLogList(buff);
	this->m_bIsKilled = false;
	// Player's HP becomes half of the Max HP.
	this->m_iHP = this->iGetMaxHP() / 2;
	// Player's MP
	this->m_iMP = ((this->m_iMag * 2)+(this->m_iLevel / 2)) + this->m_iInt / 2;
	// Player's SP
	this->m_iSP = (this->m_iStr * 2)+(this->m_iLevel / 2);
	// Player's Hunger
	this->m_iHungerStatus = 100;
	this->m_bIsBeingResurrected = false;
	this->RequestTeleportHandler("2   ", this->map_->m_cName, this->m_sX, this->m_sY);
}

void CClient::RequestTeleportHandler(const char * pData, const char * cMapName, int dX, int dY) {
	char * pBuffer, cTempMapName[21];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cDestMapName[11], cDir, cMapIndex, cQuestRemain;
	short * sp, sX, sY, sSummonPoints;
	int * ip, i, iRet, iSize, iDestX, iDestY, iMapSide;
	bool bRet;
	bool bIsLockedMapNotify;
	SYSTEMTIME SysTime;
	this->m_dwLastActionTime = this->m_dwAFKCheckTime = timeGetTime();
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsOnWaitingProcess == true) return;
	if ((this->map_->m_bIsRecallImpossible == true) && (this->m_iAdminUserLevel == 0) &&
			  (this->m_bIsKilled == false) && (this->m_iHP > 0)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_NORECALL, 0, 0, 0, nullptr);
		return;
	}
	if ((memcmp(this->m_cLocation, "elvine", 6) == 0)
			  && (this->m_iTimeLeft_ForceRecall > 0)
			  && (memcmp(this->map_->m_cLocationName, "aresden", 7) == 0)
			  && ((pData[0] == '1'))
			  && (this->m_iAdminUserLevel == 0)
			  && (game_.m_bIsCrusadeMode == false)) return;
	if ((memcmp(this->m_cLocation, "aresden", 7) == 0)
			  && (this->m_iTimeLeft_ForceRecall > 0)
			  && (memcmp(this->map_->m_cLocationName, "elvine", 6) == 0)
			  && ((pData[0] == '1'))
			  && (this->m_iAdminUserLevel == 0)
			  && (game_.m_bIsCrusadeMode == false)) return;
	bIsLockedMapNotify = false;
	if (this->m_bIsExchangeMode == true) {
		auto with = this->exchangingWith_.lock();
		if (with) {
			with->_ClearExchangeStatus();
		}
		this->_ClearExchangeStatus();
	}
	if ((memcmp(this->m_cLocation, "NONE", 4) == 0) && (pData[0] == '1'))
		return;
	this->RemoveFromTarget();
	this->map_->ClearOwner(13, id_, DEF_OWNERTYPE_PLAYER,
			  this->m_sX,
			  this->m_sY);
	this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
	sX = this->m_sX;
	sY = this->m_sY;
	std::memset(cDestMapName, 0, sizeof (cDestMapName));
	bRet = this->map_->bSearchTeleportDest(sX, sY, cDestMapName, &iDestX, &iDestY, &cDir);
	// Crusade
	if ((this->lockedMap_) && (this->m_iLockedMapTime > 0)) {
		iMapSide = game_.iGetMapLocationSide(cDestMapName);
		if (iMapSide > 3) iMapSide -= 2; // New 18/05/2004
		if ((iMapSide != 0) && (this->m_cSide == iMapSide)) {
		} else {
			iDestX = -1;
			iDestY = -1;
			bIsLockedMapNotify = true;
			std::memset(cDestMapName, 0, sizeof (cDestMapName));
			strcpy(cDestMapName, this->lockedMap_->m_cName);
		}
	}
	if ((bRet == true) && (cMapName == nullptr)) {
		for (i = 0; i < DEF_MAXMAPS; i++)
			if (game_.m_pMapList[i] != nullptr) {
				if (memcmp(game_.m_pMapList[i]->m_cName, cDestMapName, 10) == 0) {
					if (pData[0] == '3' && this->m_iAdminUserLevel == 0 && (this->m_iTimeLeft_ForceRecall > 0)) {
						std::vector<std::string> whitelist;
						if ((memcmp(this->m_cLocation, "elvine", 6) == 0)
								  && (memcmp(this->map_->m_cLocationName, "aresden", 7) == 0)) {
							// Elv in ares
							whitelist = {"middleland", "huntzone2", "aresdend1", "arefarm"};
						}
						if ((memcmp(this->m_cLocation, "aresden", 7) == 0)
								  && (memcmp(this->map_->m_cLocationName, "elvine", 6) == 0)) {
							// Ares in elv
							whitelist = {"middleland", "huntzone1", "elvined1", "elvfarm"};
						}
						if (!whitelist.empty()) {
							std::string target = cDestMapName;
							if (std::find(whitelist.begin(), whitelist.end(), target) == whitelist.end()) {
								return;
							}
						}
					}
					this->m_sX = iDestX;
					this->m_sY = iDestY;
					this->m_cDir = cDir;
					this->map_ = game_.m_pMapList[i];
					goto RTH_NEXTSTEP;
				}
			}
		this->m_sX = iDestX;
		this->m_sY = iDestY;
		this->m_cDir = cDir;
		this->map_ = game_.m_pMapList.byName(cDestMapName);
		// New 18/05/2004
		this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
				  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
		this->SetSlateFlag(DEF_NOTIFY_SLATECLEAR, false);
		this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, false);
		// !!!!
		this->m_bIsOnServerChange = true;
		this->m_bIsOnWaitingProcess = true;
		return;
	} else {
		switch (pData[0]) {
			case '0':
				// Forced Recall.
				std::memset(cTempMapName, 0, sizeof (cTempMapName));
				if (memcmp(this->m_cLocation, "NONE", 4) == 0) {
					strcpy(cTempMapName, "default");
				} else if (memcmp(this->m_cLocation, "arehunter", 9) == 0) {
					strcpy(cTempMapName, "arefarm");
				} else if (memcmp(this->m_cLocation, "elvhunter", 9) == 0) {
					strcpy(cTempMapName, "elvfarm");
				} else strcpy(cTempMapName, this->m_cLocation);
				// Crusade
				if ((this->lockedMap_) && (this->m_iLockedMapTime > 0)) {
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, this->lockedMap_->m_cName);
				}
				for (i = 0; i < DEF_MAXMAPS; i++)
					if (game_.m_pMapList[i] != nullptr) {
						if (memcmp(game_.m_pMapList[i]->m_cName, cTempMapName, 10) == 0) {
							game_.m_pMapList[i]->GetMapInitialPoint(&this->m_sX, &this->m_sY, this->m_cLocation);
							this->map_ = game_.m_pMapList[i];
							goto RTH_NEXTSTEP;
						}
					}
				this->m_sX = -1;
				this->m_sY = -1;
				this->map_ = game_.m_pMapList.byName(cTempMapName);
				// New 18/05/2004
				this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
						  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
				this->SetSlateFlag(DEF_NOTIFY_SLATECLEAR, false);
				this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, false);
				this->m_bIsOnServerChange = true;
				this->m_bIsOnWaitingProcess = true;
				return;
			case '1':
				// if (memcmp(m_pMapList[ this->m_cMapIndex ]->m_cName, "resurr", 6) == 0) return;
				std::memset(cTempMapName, 0, sizeof (cTempMapName));
				if (memcmp(this->m_cLocation, "NONE", 4) == 0) {
					strcpy(cTempMapName, "default");
				} else {
					if (this->m_iLevel > 80)
						if (memcmp(this->m_cLocation, "are", 3) == 0)
							strcpy(cTempMapName, "aresden");
						else strcpy(cTempMapName, "elvine");
					else {
						if (memcmp(this->m_cLocation, "are", 3) == 0)
							strcpy(cTempMapName, "aresden");
						else strcpy(cTempMapName, "elvine");
					}
				}
				// Crusade
				if ((this->lockedMap_) && (this->m_iLockedMapTime > 0)) {
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, this->lockedMap_->m_cName);
				}
				for (i = 0; i < DEF_MAXMAPS; i++)
					if (game_.m_pMapList[i] != nullptr) {
						if (memcmp(game_.m_pMapList[i]->m_cName, cTempMapName, 10) == 0) {
							game_.m_pMapList[i]->GetMapInitialPoint(&this->m_sX, &this->m_sY, this->m_cLocation);
							this->map_ = game_.m_pMapList[i];
							goto RTH_NEXTSTEP;
						}
					}
				this->m_sX = -1;
				this->m_sY = -1;
				this->map_ = game_.m_pMapList.byName(cTempMapName);
				// New 18/05/2004
				this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
						  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
				this->SetSlateFlag(DEF_NOTIFY_SLATECLEAR, false);
				this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, false);
				// !!!
				this->m_bIsOnServerChange = true;
				this->m_bIsOnWaitingProcess = true;
				return;
			case '2':
				// Crusade
				if ((this->lockedMap_) && (this->m_iLockedMapTime > 0)) {
					dX = -1;
					dY = -1;
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, this->lockedMap_->m_cName);
				} else {
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, cMapName);
				}
				cMapIndex = game_.iGetMapIndex(cTempMapName);
				if (cMapIndex == -1) {
					this->m_sX = dX;
					this->m_sY = dY;
					this->map_ = game_.m_pMapList.byName(cTempMapName);
					// New 18/05/2004
					this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
							  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
					this->SetSlateFlag(DEF_NOTIFY_SLATECLEAR, false);
					this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, false);
					// !!!
					this->m_bIsOnServerChange = true;
					this->m_bIsOnWaitingProcess = true;
					return;
				}
				this->m_sX = dX;
				this->m_sY = dY;
				this->map_ = game_.m_pMapList[cMapIndex];
				break;
		}
	}
RTH_NEXTSTEP:
	;
	// New 17/05/2004
	this->SetPlayingStatus();
	int iTemp;
	int iTemp2;
	iTemp = this->m_iStatus;
	iTemp = 0x0FFFFFFF & iTemp;
	iTemp2 = this->iGetPlayerABSStatus();
	iTemp = iTemp | (iTemp2 << 28);
	this->m_iStatus = iTemp;
	// Crusade
	if (bIsLockedMapNotify == true) this->SendNotifyMsg(0, DEF_NOTIFY_LOCKEDMAP, this->m_iLockedMapTime, 0, 0, this->lockedMap_->m_cName);
	pBuffer = new char [DEF_MSGBUFFERSIZE + 1];
	memset(pBuffer, 0, DEF_MSGBUFFERSIZE + 1);
	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITDATA;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);
	if (this->m_bIsObserverMode == false)
		this->map_->bGetEmptyPosition(&this->m_sX, &this->m_sY);
	else this->map_->GetMapInitialPoint(&this->m_sX, &this->m_sY);
	sp = (short *) cp;
	*sp = id_; // Player ObjectID
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sX - 14 - 5;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sY - 12 - 5;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sType;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr1;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr2;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr3;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr4;
	cp += 2;
	// v1.4 ApprColor
	ip = (int *) cp;
	*ip = this->m_iApprColor;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iStatus;
	cp += 4; //Original 2
	memcpy(cp, this->map_->m_cName, 10);
	cp += 10;
	memcpy(cp, this->map_->m_cLocationName, 10);
	cp += 10;
	if (this->map_->m_bIsFixedDayMode == true)
		*cp = 1;
	else *cp = game_.m_cDayOrNight;
	cp++;
	if (this->map_->m_bIsFixedDayMode == true)
		*cp = 0;
	else *cp = this->map_->m_cWhetherStatus;
	cp++;
	// v1.4 Contribution
	ip = (int *) cp;
	*ip = this->m_iContribution;
	cp += 4;
	if (this->m_bIsObserverMode == false) {
		this->map_->SetOwner(id_,
				  DEF_OWNERTYPE_PLAYER,
				  this->m_sX,
				  this->m_sY);
	}
	// v1.41
	*cp = (char) this->m_bIsObserverMode;
	cp++;
	// v1.41
	ip = (int *) cp;
	*ip = this->m_iRating;
	cp += 4;
	// v1.44
	ip = (int *) cp;
	*ip = this->m_iHP;
	cp += 4;
	//Unknown variable
	*cp = 0;
	cp++;
	iSize = this->iComposeInitMapData(this->m_sX - 10, this->m_sY - 7, cp);
	iRet = this->m_pXSock->iSendMsg(pBuffer, 46 + iSize + 4 + 4 + 1 + 4 + 4 + 3); //Zabuza fix
	//iRet = this->m_pXSock->iSendMsg(pBuffer, 46 + iSize +4 +4 +1 +4 +4); // v1.41
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}
	if (pBuffer != nullptr) delete pBuffer;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
	if ((memcmp(this->m_cLocation, "are", 3) == 0) &&
			  (memcmp(this->map_->m_cLocationName, "elvine", 6) == 0) &&
			  (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;
		// New 17/05/2004
		this->CheckForceRecallTime();
	} else if ((memcmp(this->m_cLocation, "elv", 3) == 0) &&
			  (memcmp(this->map_->m_cLocationName, "aresden", 7) == 0) &&
			  (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;
		// New 17/05/2004
		this->CheckForceRecallTime();
	} else if (this->map_->m_bIsFightZone == true) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;
		this->SetForceRecallTime();
		GetLocalTime(&SysTime);
		this->m_iTimeLeft_ForceRecall = 2 * 20 * 60 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
	} else {
		this->m_bIsWarLocation = false;
		this->m_iTimeLeft_ForceRecall = 0;
		this->SetForceRecallTime();
	}
	// No entering enemy shops
	int iMapside;
	int iMapside2;
	iMapside = game_.iGetMapLocationSide(this->map_->m_cName);
	if (iMapside > 3) iMapside2 = iMapside - 2;
	else iMapside2 = iMapside;
	this->m_bIsInsideOwnTown = false;
	if ((this->m_cSide != iMapside2) && (iMapside != 0)) {
		if ((iMapside <= 2) && (this->m_iAdminUserLevel < 1)) {
			if (this->m_cSide != 0) {
				this->m_dwWarBeginTime = timeGetTime();
				this->m_bIsWarLocation = true;
				this->m_iTimeLeft_ForceRecall = 1;
				this->m_bIsInsideOwnTown = true;
			}
		}
	} else {
		if (this->map_->m_bIsFightZone == true &&
				  game_.m_iFightzoneNoForceRecall == false &&
				  this->m_iAdminUserLevel == 0) {
			this->m_dwWarBeginTime = timeGetTime();
			this->m_bIsWarLocation = true;
			GetLocalTime(&SysTime);
			this->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
		} else {
			if (memcmp(this->map_->m_cLocationName, "arejail", 7) == 0 ||
					  memcmp(this->map_->m_cLocationName, "elvjail", 7) == 0) {
				if (this->m_iAdminUserLevel == 0) {
					this->m_bIsWarLocation = true;
					this->m_dwWarBeginTime = timeGetTime();
					if (this->m_iTimeLeft_ForceRecall == 0)
						this->m_iTimeLeft_ForceRecall = 100;
					else if (this->m_iTimeLeft_ForceRecall > 100)
						this->m_iTimeLeft_ForceRecall = 100;
				}
			}
		}
	}
	this->SendNotifyMsg(0, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, nullptr);
	// v1.3
	this->SendNotifyMsg(0, DEF_NOTIFY_DOWNSKILLINDEXSET, this->m_iDownSkillIndex, 0, 0, nullptr);
	// V1.3
	this->SendNotifyMsg(0, DEF_NOTIFY_ITEMPOSLIST, 0, 0, 0, nullptr);
	// v1.4
	this->_SendQuestContents();
	this->_CheckQuestEnvironment();
	// v1.432
	if (this->m_iSpecialAbilityTime == 0)
		this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, nullptr);
	if (game_.m_bIsCrusadeMode == true) {
		if (this->m_dwCrusadeGUID == 0) {
			this->m_iCrusadeDuty = 0;
			this->m_iConstructionPoint = 0;
			this->m_dwCrusadeGUID = m_dwCrusadeGUID;
		} else if (this->m_dwCrusadeGUID != m_dwCrusadeGUID) {
			this->m_iCrusadeDuty = 0;
			this->m_iConstructionPoint = 0;
			this->m_iWarContribution = 0;
			this->m_dwCrusadeGUID = m_dwCrusadeGUID;
			this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) game_.m_bIsCrusadeMode, 0, 0, nullptr, -1);
		}
		this->m_cVar = 1;
		this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) game_.m_bIsCrusadeMode, this->m_iCrusadeDuty, 0, nullptr);
	} else if (game_.m_bIsHeldenianMode == true) {
		sSummonPoints = this->m_iCharisma * 300;
		if (sSummonPoints > DEF_MAXSUMMONPOINTS) sSummonPoints = DEF_MAXSUMMONPOINTS;
		if (this->m_dwHeldenianGUID == 0) {
			this->m_dwHeldenianGUID = m_dwHeldenianGUID;
			this->m_iConstructionPoint = sSummonPoints;
		} else if (this->m_dwHeldenianGUID != m_dwHeldenianGUID) {
			this->m_iConstructionPoint = sSummonPoints;
			this->m_iWarContribution = 0;
			this->m_dwHeldenianGUID = m_dwHeldenianGUID;
		}
		this->m_cVar = 2;
		if (game_.m_bIsHeldenianMode == true) {
			this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
		}
		if (game_.m_bHeldenianInitiated == true) {
			this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
		}
		this->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, this->m_iConstructionPoint, this->m_iWarContribution, 0, nullptr);
		game_.UpdateHeldenianStatus();
	} else if ((this->m_cVar == 1) && (this->m_dwCrusadeGUID == m_dwCrusadeGUID)) {
		this->m_iCrusadeDuty = 0;
		this->m_iConstructionPoint = 0;
	} else {
		if (this->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (this->m_cVar == 1) {
				this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) game_.m_bIsCrusadeMode, 0, 0, nullptr, -1);
			}
		} else {
			this->m_dwCrusadeGUID = 0;
			this->m_iWarContribution = 0;
			this->m_dwCrusadeGUID = 0;
		}
	}
	// v1.42
	if (memcmp(this->map_->m_cName, "fight", 5) == 0) {
		wsprintf(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", this->m_cCharName, this->map_->m_cName, this->m_bIsObserverMode);
		PutLogEventFileList(G_cTxt);
	}
	// Crusade
	this->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, this->m_iConstructionPoint, this->m_iWarContribution, 1, nullptr);
	// v2.15
	this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
	if (game_.m_bIsHeldenianMode == true) {
		this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
		if (game_.m_bHeldenianInitiated == true) {
			this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
		} else {
			game_.UpdateHeldenianStatus();
		}
	}
	if (this->m_iQuest != 0) {
		cQuestRemain = (game_.m_pQuestConfigList[this->m_iQuest]->m_iMaxCount - this->m_iCurQuestCount);
		this->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, nullptr);
		this->_bCheckIsQuestCompleted();
	}
}

void CClient::CalcTotalItemEffect(int iEquipItemID, bool bNotify) {
	int i;
	int iTemp;
	char cEquipPos;
	double dV1;
	double dV2;
	double dV3;
	uint32_t dwSWEType;
	uint32_t dwSWEValue;
	short sTemp;
	if ((this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) &&
			  (this->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)) {
		if (this->m_pItemList[this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]] != nullptr) {
			this->m_bIsItemEquipped[this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]] = false;
			this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] = -1;
		}
	}
	this->m_iAngelicStr = 0;
	this->m_iAngelicInt = 0;
	this->m_iAngelicDex = 0;
	this->m_iAngelicMag = 0;
	this->SetAngelFlag(0, 0);
	this->m_cAttackDiceThrow_SM = 0;
	this->m_cAttackDiceRange_SM = 0;
	this->m_cAttackBonus_SM = 0;
	this->m_cAttackDiceThrow_L = 0;
	this->m_cAttackDiceRange_L = 0;
	this->m_cAttackBonus_L = 0;
	this->m_iHitRatio = 0;
	this->m_iDefenseRatio = this->m_iDex * 2;
	this->m_iDamageAbsorption_Shield = 0;
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) {
		this->m_iDamageAbsorption_Armor[i] = 0;
	}
	this->m_iManaSaveRatio = 0;
	this->m_iAddResistMagic = 0;
	this->m_iAddPhysicalDamage = 0;
	this->m_iAddMagicalDamage = 0;
	this->m_bIsLuckyEffect = false;
	this->m_iMagicDamageSaveItemIndex = -1;
	this->m_iSideEffect_MaxHPdown = 0;
	this->m_iAddAbsAir = 0;
	this->m_iAddAbsEarth = 0;
	this->m_iAddAbsFire = 0;
	this->m_iAddAbsWater = 0;
	this->m_iCustomItemValue_Attack = 0;
	this->m_iCustomItemValue_Defense = 0;
	this->m_iMinAP_SM = 0;
	this->m_iMinAP_L = 0;
	this->m_iMaxAP_SM = 0;
	this->m_iMaxAP_L = 0;
	this->m_iSpecialWeaponEffectType = 0;
	this->m_iSpecialWeaponEffectValue = 0;
	this->m_iAddHP = this->m_iAddSP = this->m_iAddMP = 0;
	this->m_iAddAR = this->m_iAddPR = this->m_iAddDR = 0;
	this->m_iAddMR = this->m_iAddAbsPD = this->m_iAddAbsMD = 0;
	this->m_iAddCD = this->m_iAddExp = this->m_iAddGold = 0;
	int iPrevSAType = this->m_iSpecialAbilityType;
	this->m_iSpecialAbilityType = 0;
	this->m_iSpecialAbilityLastSec = 0;
	this->m_iSpecialAbilityEquipPos = 0;
	this->m_iAddTransMana = 0;
	this->m_iAddChargeCritical = 0;
	this->m_iAlterItemDropIndex = -1;
	for (short sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++) {
		auto &itemPtr = this->m_pItemList[sItemIndex];
		if (this->m_pItemList[sItemIndex] != nullptr) {
			auto &item = *itemPtr;
			switch (item.m_sItemEffectType) {
				case DEF_ITEMEFFECTTYPE_ALTERITEMDROP:
					if (item.m_wCurLifeSpan > 0) {
						this->m_iAlterItemDropIndex = sItemIndex;
					}
					break;
			}
		}
	}
	for (short sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++) {
		auto &itemPtr = this->m_pItemList[sItemIndex];
		if ((itemPtr != nullptr) &&
				  (this->m_bIsItemEquipped[sItemIndex] == true)) {
			auto &item = *itemPtr;
			cEquipPos = item.m_cEquipPos;
			switch (item.m_sItemEffectType) {
				case DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE:
					this->m_iMagicDamageSaveItemIndex = sItemIndex;
					break;
				case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
				case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE:
				case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
				case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
				case DEF_ITEMEFFECTTYPE_ATTACK:
					this->m_cAttackDiceThrow_SM = item.m_sItemEffectValue1;
					this->m_cAttackDiceRange_SM = item.m_sItemEffectValue2;
					this->m_cAttackBonus_SM = item.m_sItemEffectValue3;
					this->m_cAttackDiceThrow_L = item.m_sItemEffectValue4;
					this->m_cAttackDiceRange_L = item.m_sItemEffectValue5;
					this->m_cAttackBonus_L = item.m_sItemEffectValue6;
					iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
					//testcode
					//wsprintf(G_cTxt, "Add Damage: %d", iTemp);
					//PutLogList(G_cTxt);
					this->m_iAddPhysicalDamage += iTemp;
					this->m_iAddMagicalDamage += iTemp;
					this->m_iHitRatio += this->m_cSkillMastery[item.m_sRelatedSkill];
					//this->m_iHitRatio_ItemEffect_SM += item.m_sSM_HitRatio;
					//this->m_iHitRatio_ItemEffect_L  += item.m_sL_HitRatio;
					this->m_sUsingWeaponSkill = item.m_sRelatedSkill;
					if ((item.m_dwAttribute & 0x00000001) != 0) {
						this->m_iCustomItemValue_Attack += item.m_sItemSpecEffectValue2;
						if (this->m_iCustomItemValue_Attack > 100)
							this->m_iCustomItemValue_Attack = 100;
						if (this->m_iCustomItemValue_Attack < -100)
							this->m_iCustomItemValue_Attack = -100;
						if (this->m_iCustomItemValue_Attack > 0) {
							dV2 = (double) this->m_iCustomItemValue_Attack;
							dV1 = (dV2 / 100.0f)*(5.0f);
							this->m_iMinAP_SM = this->m_cAttackDiceThrow_SM +
									  this->m_cAttackBonus_SM + (int) dV1;
							this->m_iMinAP_L = this->m_cAttackDiceThrow_L +
									  this->m_cAttackBonus_L + (int) dV1;
							if (this->m_iMinAP_SM < 1) this->m_iMinAP_SM = 1;
							if (this->m_iMinAP_L < 1) this->m_iMinAP_L = 1;
							if (this->m_iMinAP_SM > (this->m_cAttackDiceThrow_SM * this->m_cAttackDiceRange_SM + this->m_cAttackBonus_SM))
								this->m_iMinAP_SM = (this->m_cAttackDiceThrow_SM * this->m_cAttackDiceRange_SM + this->m_cAttackBonus_SM);
							if (this->m_iMinAP_L > (this->m_cAttackDiceThrow_L * this->m_cAttackDiceRange_L + this->m_cAttackBonus_L))
								this->m_iMinAP_L = (this->m_cAttackDiceThrow_L * this->m_cAttackDiceRange_L + this->m_cAttackBonus_L);
							//testcode
							//wsprintf(G_cTxt, "MinAP: %d %d +(%d)", this->m_iMinAP_SM, this->m_iMinAP_L, (int)dV1);
							//PutLogList(G_cTxt);
						} else if (this->m_iCustomItemValue_Attack < 0) {
							dV2 = (double) this->m_iCustomItemValue_Attack;
							dV1 = (dV2 / 100.0f)*(5.0f);
							this->m_iMaxAP_SM = this->m_cAttackDiceThrow_SM * this->m_cAttackDiceRange_SM
									  + this->m_cAttackBonus_SM + (int) dV1;
							this->m_iMaxAP_L = this->m_cAttackDiceThrow_L * this->m_cAttackDiceRange_L
									  + this->m_cAttackBonus_L + (int) dV1;
							if (this->m_iMaxAP_SM < 1) this->m_iMaxAP_SM = 1;
							if (this->m_iMaxAP_L < 1) this->m_iMaxAP_L = 1;
							if (this->m_iMaxAP_SM < (this->m_cAttackDiceThrow_SM * this->m_cAttackDiceRange_SM + this->m_cAttackBonus_SM))
								this->m_iMaxAP_SM = (this->m_cAttackDiceThrow_SM * this->m_cAttackDiceRange_SM + this->m_cAttackBonus_SM);
							if (this->m_iMaxAP_L < (this->m_cAttackDiceThrow_L * this->m_cAttackDiceRange_L + this->m_cAttackBonus_L))
								this->m_iMaxAP_L = (this->m_cAttackDiceThrow_L * this->m_cAttackDiceRange_L + this->m_cAttackBonus_L);
							//testcode
							//wsprintf(G_cTxt, "MaxAP: %d %d +(%d)", this->m_iMaxAP_SM, this->m_iMaxAP_L, (int)dV1);
							//PutLogList(G_cTxt);
						}
					}
					if ((item.m_dwAttribute & 0x00F00000) != 0) {
						dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
						dwSWEValue = (item.m_dwAttribute & 0x000F0000) >> 16;
						this->m_iSpecialWeaponEffectType = (int) dwSWEType;
						this->m_iSpecialWeaponEffectValue = (int) dwSWEValue;
						switch (dwSWEType) {
							case 7:
								this->m_cAttackDiceRange_SM++;
								this->m_cAttackDiceRange_L++;
								break;
							case 9:
								this->m_cAttackDiceRange_SM += 2;
								this->m_cAttackDiceRange_L += 2;
								break;
						}
					}
					if ((item.m_dwAttribute & 0x0000F000) != 0) {
						dwSWEType = (item.m_dwAttribute & 0x0000F000) >> 12;
						dwSWEValue = (item.m_dwAttribute & 0x00000F00) >> 8;
						switch (dwSWEType) {
							case 0: break;
							case 1: this->m_iAddPR += (int) dwSWEValue * 7;
								break;
							case 2: this->m_iAddAR += (int) dwSWEValue * 7;
								break;
							case 3: this->m_iAddDR += (int) dwSWEValue * 7;
								break;
							case 4: this->m_iAddHP += (int) dwSWEValue * 7;
								break;
							case 5: this->m_iAddSP += (int) dwSWEValue * 7;
								break;
							case 6: this->m_iAddMP += (int) dwSWEValue * 7;
								break;
							case 7: this->m_iAddMR += (int) dwSWEValue * 7;
								break;
								//case 8: this->m_iDamageAbsorption_Armor[item.m_cEquipPos] += (int)dwSWEValue*3; break;
								// PA Fixed by KahBur.
							case 8: this->m_iAddAbsPD += (int) dwSWEValue * 3;
								break;
							case 9: this->m_iAddAbsMD += (int) dwSWEValue * 3;
								break;
							case 10: this->m_iAddCD += (int) dwSWEValue;
								break;
							case 11: this->m_iAddExp += (int) dwSWEValue * 10;
								break;
							case 12: this->m_iAddGold += (int) dwSWEValue * 10;
								break;
						}
						switch (dwSWEType) {
								// PA Fixed by KahBur.
							case 8: if (this->m_iAddAbsPD > 80) this->m_iAddAbsPD = 80;
								break;
							case 9: if (this->m_iAddAbsMD > 80) this->m_iAddAbsMD = 80;
								break;
						}
						switch (dwSWEType) {
							case 9: if (this->m_iAddAbsMD > 80) this->m_iAddAbsMD = 80;
								break;
						}
					}
					switch (item.m_sItemEffectType) {
						case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
							this->m_iSideEffect_MaxHPdown = item.m_sSpecialEffect;
							break;
						case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
							this->m_iManaSaveRatio += item.m_sItemEffectValue4;
							if (this->m_iManaSaveRatio > 80) this->m_iManaSaveRatio = 80;
							break;
						case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE:
							this->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] += item.m_sSpecialEffect;
							break;
						case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
							this->m_iSpecialAbilityType = item.m_sSpecialEffect;
							this->m_iSpecialAbilityLastSec = item.m_sSpecialEffectValue1;
							this->m_iSpecialAbilityEquipPos = (int) cEquipPos;
							if ((bNotify == true) && (iEquipItemID == (int) sItemIndex))
								this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 2, this->m_iSpecialAbilityType, this->m_iSpecialAbilityTime, nullptr);
							break;
					}
					break;
				case DEF_ITEMEFFECTTYPE_ADDEFFECT:
					switch (item.m_sItemEffectValue1) {
						case 1:
							this->m_iAddResistMagic += item.m_sItemEffectValue2;
							break;
						case 2:
							this->m_iManaSaveRatio += item.m_sItemEffectValue2;
							if (this->m_iManaSaveRatio > 80) this->m_iManaSaveRatio = 80;
							break;
						case 3:
							this->m_iAddPhysicalDamage += item.m_sItemEffectValue2;
							break;
						case 4:
							this->m_iDefenseRatio += item.m_sItemEffectValue2;
							break;
						case 5:
							if (item.m_sItemEffectValue2 != 0)
								this->m_bIsLuckyEffect = true;
							else this->m_bIsLuckyEffect = false;
							break;
						case 6:
							this->m_iAddMagicalDamage += item.m_sItemEffectValue2;
							break;
						case 7:
							this->m_iAddAbsAir += item.m_sItemEffectValue2;
							break;
						case 8:
							this->m_iAddAbsEarth += item.m_sItemEffectValue2;
							break;
						case 9:
							this->m_iAddAbsFire += item.m_sItemEffectValue2;
							break;
						case 10:
							this->m_iAddAbsWater += item.m_sItemEffectValue2;
							break;
						case 11:
							this->m_iAddPR += item.m_sItemEffectValue2;
							break;
						case 12: // Adds To Hit Bonus (Xelima Neck)
							this->m_iHitRatio += item.m_sItemEffectValue2;
							break;
						case 13: // Magin Ruby		Characters Hp recovery rate(% applied) added by the purity formula.
							this->m_iAddHP += (item.m_sItemSpecEffectValue2 / 5);
							break;
						case 14: // Magin Diamond	Attack probability(physical&magic) added by the purity formula.
							this->m_iAddAR += (item.m_sItemSpecEffectValue2 / 5);
							break;
						case 15: // Magin Emerald	Magical damage decreased(% applied) by the purity formula.
							this->m_iAddAbsMD += (item.m_sItemSpecEffectValue2 / 10);
							if (this->m_iAddAbsMD > 80) this->m_iAddAbsMD = 80;
							break;
						case 30: // Magin Sapphire	Phisical damage decreased(% applied) by the purity formula.
							iTemp = (item.m_sItemSpecEffectValue2 / 10);
							this->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] += iTemp;
							this->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] += iTemp;
							this->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] += iTemp;
							this->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] += iTemp;
							break;
							/*Functions rates confirm.
							Magic Diamond: Completion rate / 5 = Functions rate. ? Maximum 20. (not%)
							Magic Ruby: Completion rate / 5 = Functions rate.(%) ? Maximum 20%.
							Magic Emerald: Completion rate / 10 = Functions rate.(%) ? Maximum 10%.
							Magic Sapphire: Completion rate / 10 = Functions rate.(%) ? Maximum 10%.*/
							// ******* Angel Code - Begin ******* //
						case 16: // Angel STR//AngelicPandent(STR)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicStr = iTemp;
							this->SetAngelFlag(1, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
						case 17: // Angel DEX //AngelicPandent(DEX)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicDex = iTemp;
							this->SetAngelFlag(2, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
						case 18: // Angel INT//AngelicPandent(INT)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicInt = iTemp;
							this->SetAngelFlag(3, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
						case 19: // Angel MAG//AngelicPandent(MAG)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicMag = iTemp;
							this->SetAngelFlag(4, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
					}
					break;
				case DEF_ITEMEFFECTTYPE_ATTACK_ARROW:
					if ((this->m_cArrowIndex != -1) &&
							  (this->m_pItemList[this->m_cArrowIndex] == nullptr)) {
						this->m_cArrowIndex = this->_iGetArrowItemIndex();
					} else if (this->m_cArrowIndex == -1)
						this->m_cArrowIndex = this->_iGetArrowItemIndex();
					if (this->m_cArrowIndex == -1) {
						this->m_cAttackDiceThrow_SM = 0;
						this->m_cAttackDiceRange_SM = 0;
						this->m_cAttackBonus_SM = 0;
						this->m_cAttackDiceThrow_L = 0;
						this->m_cAttackDiceRange_L = 0;
						this->m_cAttackBonus_L = 0;
					} else {
						this->m_cAttackDiceThrow_SM = item.m_sItemEffectValue1;
						this->m_cAttackDiceRange_SM = item.m_sItemEffectValue2;
						this->m_cAttackBonus_SM = item.m_sItemEffectValue3;
						this->m_cAttackDiceThrow_L = item.m_sItemEffectValue4;
						this->m_cAttackDiceRange_L = item.m_sItemEffectValue5;
						this->m_cAttackBonus_L = item.m_sItemEffectValue6;
					}
					this->m_iHitRatio += this->m_cSkillMastery[item.m_sRelatedSkill];
					break;
				case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
				case DEF_ITEMEFFECTTYPE_DEFENSE:
					this->m_iDefenseRatio += item.m_sItemEffectValue1;
					if ((item.m_dwAttribute & 0x00000001) != 0) {
						this->m_iCustomItemValue_Defense += item.m_sItemSpecEffectValue2;
						dV2 = (double) item.m_sItemSpecEffectValue2;
						dV3 = (double) item.m_sItemEffectValue1;
						dV1 = (double) (dV2 / 100.0f) * dV3;
						dV1 = dV1 / 2.0f;
						this->m_iDefenseRatio += (int) dV1;
						if (this->m_iDefenseRatio <= 0) this->m_iDefenseRatio = 1;
					}
					if ((item.m_dwAttribute & 0x00F00000) != 0) {
						dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
						dwSWEValue = (item.m_dwAttribute & 0x000F0000) >> 16;
						switch (dwSWEType) {
							case 7:
								this->m_cAttackDiceRange_SM++;
								this->m_cAttackDiceRange_L++;
								break;
							case 9:
								this->m_cAttackDiceRange_SM += 2;
								this->m_cAttackDiceRange_L += 2;
								break;
							case 11:
								this->m_iAddTransMana += dwSWEValue;
								if (this->m_iAddTransMana > 13) this->m_iAddTransMana = 13;
								break;
							case 12:
								this->m_iAddChargeCritical += dwSWEValue;
								if (this->m_iAddChargeCritical > 20) this->m_iAddChargeCritical = 20;
								break;
						}
					}
					if ((item.m_dwAttribute & 0x0000F000) != 0) {
						dwSWEType = (item.m_dwAttribute & 0x0000F000) >> 12;
						dwSWEValue = (item.m_dwAttribute & 0x00000F00) >> 8;
						switch (dwSWEType) {
							case 0: break;
							case 1: this->m_iAddPR += (int) dwSWEValue * 7;
								break;
							case 2: this->m_iAddAR += (int) dwSWEValue * 7;
								break;
							case 3: this->m_iAddDR += (int) dwSWEValue * 7;
								break;
							case 4: this->m_iAddHP += (int) dwSWEValue * 7;
								break;
							case 5: this->m_iAddSP += (int) dwSWEValue * 7;
								break;
							case 6: this->m_iAddMP += (int) dwSWEValue * 7;
								break;
							case 7: this->m_iAddMR += (int) dwSWEValue * 7;
								break;
							case 8: this->m_iAddAbsPD += (int) dwSWEValue * 3;
								break;
							case 9: this->m_iAddAbsMD += (int) dwSWEValue * 3;
								break;
							case 10: this->m_iAddCD += (int) dwSWEValue;
								break;
							case 11: this->m_iAddExp += (int) dwSWEValue * 10;
								break;
							case 12: this->m_iAddGold += (int) dwSWEValue * 10;
								break;
						}
						switch (dwSWEType) {
							case 8: if (this->m_iAddAbsPD > 80) this->m_iAddAbsPD = 80;
								break;
							case 9: if (this->m_iAddAbsMD > 80) this->m_iAddAbsMD = 80;
								break;
						}
						switch (dwSWEType) {
							case 9: if (this->m_iAddAbsMD > 80) this->m_iAddAbsMD = 80;
								break;
						}
					}
					switch (cEquipPos) {
						case DEF_EQUIPPOS_LHAND:
							this->m_iDamageAbsorption_Shield = (item.m_sItemEffectValue1) - (item.m_sItemEffectValue1) / 3;
							break;
						default:
							this->m_iDamageAbsorption_Armor[item.m_cEquipPos] += (item.m_sItemEffectValue2);
							break;
					}
					switch (item.m_sItemEffectType) {
						case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
							this->m_iSpecialAbilityType = item.m_sSpecialEffect;
							this->m_iSpecialAbilityLastSec = item.m_sSpecialEffectValue1;
							this->m_iSpecialAbilityEquipPos = (int) cEquipPos;
							if ((bNotify == true) && (iEquipItemID == (int) sItemIndex))
								this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 2, this->m_iSpecialAbilityType, this->m_iSpecialAbilityTime, nullptr);
							break;
					}
					break;
			}
		}
	}
	if ((iPrevSAType != 0) && (this->m_iSpecialAbilityType == 0) && (bNotify == true)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 4, 0, 0, nullptr);
		if (this->m_bIsSpecialAbilityEnabled == true) {
			this->m_bIsSpecialAbilityEnabled = false;
			this->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0xFF0F;
			this->m_sAppr4 = sTemp;
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		}
	}
	if ((iPrevSAType != 0) && (this->m_iSpecialAbilityType != 0) &&
			  (iPrevSAType != this->m_iSpecialAbilityType) && (bNotify == true)) {
		if (this->m_bIsSpecialAbilityEnabled == true) {
			this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 3, 0, 0, nullptr);
			this->m_bIsSpecialAbilityEnabled = false;
			this->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0xFF0F;
			this->m_sAppr4 = sTemp;
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
		}
	}
	this->m_iDefenseRatio += this->m_iAngelicDex * 2;
	if (this->m_iHP > this->iGetMaxHP()) this->m_iHP = this->iGetMaxHP();
	if (this->m_iMP > this->iGetMaxMP()) this->m_iMP = this->iGetMaxMP();
	if (this->m_iSP > this->iGetMaxSP()) this->m_iSP = this->iGetMaxSP();
}

void CClient::ReqCreateCraftingHandler(char *pData) {
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cI[6], cCraftingName[21], cData[120];
	int iRet;
	int i;
	int j;
	int iEraseReq;
	int iRiskLevel;
	int iDifficulty;
	int iNeededContrib = 0;
	short * sp, sTemp;
	short sItemIndex[6], sItemPurity[6], sItemNumber[6], sItemArray[12];
	bool bDup;
	bool bFlag;
	bool bNeedLog;
	class CItem * pItem;
	this->m_iSkillMsgRecvCount++;
	for (i = 0; i < 6; i++) {
		cI[i] = -1;
		sItemIndex[i] = -1;
		sItemNumber[i] = 0;
		sItemPurity[i] = -1;
	}
	cp = (char *) (pData + 11);
	cp += 20;
	cI[0] = *cp;
	cp++;
	cI[1] = *cp;
	cp++;
	cI[2] = *cp;
	cp++;
	cI[3] = *cp;
	cp++;
	cI[4] = *cp;
	cp++;
	cI[5] = *cp;
	cp++;
	for (i = 0; i < 6; i++) {
		if (cI[i] >= DEF_MAXITEMS) return;
		if ((cI[i] >= 0) && (this->m_pItemList[cI[i]] == nullptr)) return;
	}
	for (i = 0; i < 6; i++)
		if (cI[i] >= 0) {
			bDup = false;
			for (j = 0; j < 6; j++)
				if (sItemIndex[j] == cI[i]) {
					sItemNumber[j]++;
					bDup = true;
				}
			if (bDup == false) {
				for (j = 0; j < 6; j++)
					if (sItemIndex[j] == -1) {
						sItemIndex[j] = cI[i];
						sItemNumber[j]++;
						goto RCPH_LOOPBREAK;
					}
RCPH_LOOPBREAK:
				;
			}
		}
	for (i = 0; i < 6; i++)
		if (sItemIndex[i] != -1) {
			if (sItemIndex[i] < 0) return;
			if ((sItemIndex[i] >= 0) && (sItemIndex[i] >= DEF_MAXITEMS)) return;
			if (this->m_pItemList[sItemIndex[i]] == nullptr) return;
			if (this->m_pItemList[sItemIndex[i]]->m_dwCount < sItemNumber[i]) return;
			sItemPurity[i] = this->m_pItemList[sItemIndex[i]]->m_sItemSpecEffectValue2;
			if ((this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_NONE)
					  && (this->m_pItemList[sItemIndex[i]]->m_sSprite == 6)
					  && (this->m_pItemList[sItemIndex[i]]->m_sSpriteFrame == 129)) {
				sItemPurity[i] = 100; // Merien stones considered 100% purity.
			}
			if (this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) {
				sItemPurity[i] = -1; // Diamonds / Emeralds.etc.. never have purity
			}
			if (sItemNumber[i] > 1) // No purity for stacked items
			{
				sItemPurity[i] = -1;
			}
			if ((this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_EQUIP)
					  && (this->m_pItemList[sItemIndex[i]]->m_cEquipPos == DEF_EQUIPPOS_NECK)) {
				iNeededContrib = 10; // Necks Crafting requires 10 contrib
			}
		}
	// Bubble Sort
	bFlag = true;
	while (bFlag == true) {
		bFlag = false;
		for (i = 0; i < 5; i++)
			if ((sItemIndex[i] != -1) && (sItemIndex[i + 1] != -1)) {
				if ((this->m_pItemList[sItemIndex[i]]->m_sIDnum) < (this->m_pItemList[sItemIndex[i + 1]]->m_sIDnum)) {
					sTemp = sItemIndex[i + 1];
					sItemIndex[i + 1] = sItemIndex[i];
					sItemIndex[i] = sTemp;
					sTemp = sItemPurity[i + 1];
					sItemPurity[i + 1] = sItemPurity[i];
					sItemPurity[i] = sTemp;
					sTemp = sItemNumber[i + 1];
					sItemNumber[i + 1] = sItemNumber[i];
					sItemNumber[i] = sTemp;
					bFlag = true;
				}
			}
	}
	j = 0;
	for (i = 0; i < 6; i++) {
		if (sItemIndex[i] != -1)
			sItemArray[j] = this->m_pItemList[sItemIndex[i]]->m_sIDnum;
		else sItemArray[j] = sItemIndex[i];
		sItemArray[j + 1] = sItemNumber[i];
		j += 2;
	}
	// Search Crafting you wanna build
	std::memset(cCraftingName, 0, sizeof (cCraftingName));
	for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
		if (game_.m_pCraftingConfigList[i] != nullptr) {
			bFlag = false;
			for (j = 0; j < 12; j++) {
				if (game_.m_pCraftingConfigList[i]->m_sArray[j] != sItemArray[j]) bFlag = true; // one item mismatch
			}
			if (bFlag == false) // good Crafting receipe
			{
				std::memset(cCraftingName, 0, sizeof (cCraftingName));
				memcpy(cCraftingName, game_.m_pCraftingConfigList[i]->m_cName, 20);
				iRiskLevel = game_.m_pCraftingConfigList[i]->m_iSkillLimit; // % to loose item if crafting fails
				iDifficulty = game_.m_pCraftingConfigList[i]->m_iDifficulty;
			}
		}
	// Check if recipe is OK
	if (strlen(cCraftingName) == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CRAFTING_FAIL, 1, 0, 0, nullptr); // "There is not enough material"
		return;
	}
	// Check for Contribution
	if (this->m_iContribution < iNeededContrib) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CRAFTING_FAIL, 2, 0, 0, nullptr); // "There is not enough Contribution Point"
		return;
	}
	// Check possible Failure
	if (iDice(1, 100) > iDifficulty) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CRAFTING_FAIL, 3, 0, 0, nullptr); // "Crafting failed"
		// Remove parts...
		pItem = nullptr;
		pItem = new class CItem;
		if (pItem == nullptr) return;
		for (i = 0; i < 6; i++)
			if (sItemIndex[i] != -1) { // Deplete any Merien Stone
				if ((this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_NONE)
						  && (this->m_pItemList[sItemIndex[i]]->m_sSprite == 6)
						  && (this->m_pItemList[sItemIndex[i]]->m_sSpriteFrame == 129)) {
					this->ItemDepleteHandler(sItemIndex[i], false, false);
				} else
					// Risk to deplete any other items (not stackable ones) // DEF_ITEMTYPE_CONSUME
					if ((this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_EQUIP)
						  || (this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_MATERIAL)) {
					if (iDice(1, 100) < iRiskLevel) {
						this->ItemDepleteHandler(sItemIndex[i], false, false);
					}
				}
			}
		return;
	}
	// Purity
	int iPurity;
	int iTot = 0, iCount = 0;
	for (i = 0; i < 6; i++) {
		if (sItemIndex[i] != -1) {
			if (sItemPurity[i] != -1) {
				iTot += sItemPurity[i];
				iCount++;
			}
		}
	}
	if (iCount == 0) {
		iPurity = 20 + iDice(1, 80); // Wares have random purity (20%..100%)
		bNeedLog = false;
	} else {
		iPurity = iTot / iCount;
		iTot = (iPurity * 4) / 5;
		iCount = iPurity - iTot;
		iPurity = iTot + iDice(1, iCount); // Jewel completion depends off Wares purity
		bNeedLog = true;
	}
	if (iNeededContrib != 0) {
		iPurity = 0; // Necks require contribution but no purity/completion
		bNeedLog = true;
	}
	//this->CalculateSSN_SkillIndex(12, 1);
	if (strlen(cCraftingName) != 0) {
		pItem = nullptr;
		pItem = new class CItem;
		if (pItem == nullptr) return;
		for (i = 0; i < 6; i++) {
			if (sItemIndex[i] != -1) {
				if (this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) {
					this->SetItemCount(sItemIndex[i],
							  this->m_pItemList[sItemIndex[i]]->m_dwCount - sItemNumber[i]);
				} else // So if item is not Type 5 (stackable items), you deplete item
				{
					this->ItemDepleteHandler(sItemIndex[i], false, false);
				}
			}
		}
		if (iNeededContrib != 0) {
			this->m_iContribution -= iNeededContrib;
			// No known msg to send info to client, so client will compute shown Contrib himself.
		}
		this->SendNotifyMsg(0, DEF_NOTIFY_CRAFTING_SUCCESS, 0, 0, 0, nullptr);
		this->m_iExpStock += iDice(2, 100);
		if ((game_._bInitItemAttr(*pItem, cCraftingName) == true)) { // // Snoopy: Added Purity to Oils/Elixirs
			if (iPurity != 0) {
				pItem->m_sItemSpecEffectValue2 = iPurity;
				pItem->m_dwAttribute = 1;
			}
			pItem->m_sTouchEffectType = DEF_ITET_ID;
			pItem->m_sTouchEffectValue1 = iDice(1, 100000);
			pItem->m_sTouchEffectValue2 = iDice(1, 100000);
			// pItem->m_sTouchEffectValue3 = timeGetTime();
			SYSTEMTIME SysTime;
			char cTemp[256];
			GetLocalTime(&SysTime);
			std::memset(cTemp, 0, sizeof (cTemp));
			wsprintf(cTemp, "%d%2d", (short) SysTime.wMonth, (short) SysTime.wDay);
			pItem->m_sTouchEffectValue3 = atoi(cTemp);
			// SNOOPY log anything above WAREs
			if (bNeedLog) {
				wsprintf(G_cTxt, "PC(%s) Crafting (%s) Purity(%d)"
						  , this->m_cCharName
						  , pItem->m_cName
						  , pItem->m_sItemSpecEffectValue2);
				//	PutSkillLogFileList(G_cTxt);
			}
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				std::memset(cData, 0, sizeof (cData));
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char) 0;
				cp++;
				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
				cp++;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwAttribute;
				cp += 4;
				/*	*cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-Item
				cp++;	*/
				if (iEraseReq == 1) delete pItem;
				iRet = this->m_pXSock->iSendMsg(cData, 53);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						break;
				}
				//if ((pItem->m_wPrice * pItem->m_dwCount) > 1000)
				//	SendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, id_);
			} else {
				this->map_->bSetItem(this->m_sX,
						  this->m_sY, pItem);
				this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
						  this->m_sX, this->m_sY,
						  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						break;
				}
			}
		} else {
			delete pItem;
			pItem = nullptr;
		}
	}
}

void CClient::ClientCommonHandler(char * pData) {
	uint16_t * wp, wCommand;
	short * sp, sX, sY;
	int * ip, iV1, iV2, iV3, iV4;
	char * cp, cDir, * pString;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	wCommand = *wp;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	sX = *sp;
	cp += 2;
	sp = (short *) cp;
	sY = *sp;
	cp += 2;
	cDir = *cp;
	cp++;
	ip = (int *) cp;
	iV1 = *ip;
	cp += 4;
	ip = (int *) cp;
	iV2 = *ip;
	cp += 4;
	ip = (int *) cp;
	iV3 = *ip;
	cp += 4;
	pString = cp;
	cp += 30;
	ip = (int *) cp;
	iV4 = *ip;
	cp += 4;
	switch (wCommand) {
			// New 15/05/2004
		case DEF_COMMONTYPE_REQ_CREATESLATE:
			this->ReqCreateSlateHandler(pData);
			break;
			// 2.06 - by KLKS
		case DEF_COMMONTYPE_REQ_CHANGEPLAYMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> MSGID_REQUEST_CIVILRIGHT");
			this->RequestChangePlayMode();
			break;
			//
		case DEF_COMMONTYPE_SETGUILDTELEPORTLOC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDTELEPORTLOC");
			this->RequestSetGuildTeleportLocHandler(iV1, iV2, this->m_iGuildGUID, "middleland");
			break;
		case DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC");
			this->RequestSetGuildConstructLocHandler(iV1, iV2, this->m_iGuildGUID, pString);
			break;
		case DEF_COMMONTYPE_GUILDTELEPORT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GUILDTELEPORT");
			this->RequestGuildTeleportHandler();
			break;
		case DEF_COMMONTYPE_SUMMONWARUNIT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SUMMONWARUNIT");
			this->RequestSummonWarUnitHandler(sX, sY, iV1, iV2, iV3);
			break;
		case DEF_COMMONTYPE_REQUEST_HELP:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_HELP");
			this->RequestHelpHandler();
			break;
		case DEF_COMMONTYPE_REQUEST_MAPSTATUS:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_MAPSTATUS");
			this->MapStatusHandler(iV1, pString);
			break;
		case DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY");
			this->SelectCrusadeDutyHandler(iV1);
			break;
		case DEF_COMMONTYPE_REQUEST_CANCELQUEST:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_CANCELQUEST");
			this->CancelQuestHandler();
			break;
		case DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY");
			this->ActivateSpecialAbilityHandler();
			break;
		case DEF_COMMONTYPE_REQUEST_JOINPARTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_JOINPARTY");
			this->JoinPartyHandler(iV1, pString);
			break;
		case DEF_COMMONTYPE_GETMAGICABILITY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GETMAGICABILITY");
			this->GetMagicAbilityHandler();
			break;
		case DEF_COMMONTYPE_BUILDITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_BUILDITEM");
			this->BuildItemHandler(pData);
			break;
		case DEF_COMMONTYPE_QUESTACCEPTED:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_QUESTACCEPTED");
			this->QuestAcceptedHandler();
			break;
		case DEF_COMMONTYPE_CANCELEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CANCELEXCHANGEITEM");
			this->CancelExchangeItem();
			break;
		case DEF_COMMONTYPE_CONFIRMEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CONFIRMEXCHANGEITEM");
			this->ConfirmExchangeItem();
			break;
		case DEF_COMMONTYPE_SETEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETEXCHANGEITEM");
			this->SetExchangeItem(iV1, iV2);
			break;
		case DEF_COMMONTYPE_REQ_GETHEROMANTLE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETHEROMANTLE");
			this->GetHeroMantleHandler(iV1, pString);
			break;
		case DEF_COMMONTYPE_REQ_GETTRADEEK: // By Luqah
			this->GetTradeEKMantleHandler(iV1, pString);
			break;
		case DEF_COMMONTYPE_REQ_GETOCCUPYFLAG:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFLAG");
			this->GetOccupyFlagHandler();
			break;
		case DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX");
			this->SetDownSkillIndexHandler(iV1);
			break;
		case DEF_COMMONTYPE_TALKTONPC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TALKTONPC");
			// works for client, but for npc it returns middleland
			// if ((m_pMapList[m_pNpcList[iV1]->m_cMapIndex]->m_cName) != (m_pMapList[this->m_cMapIndex]->m_cName)) break;
			this->NpcTalkHandler(iV1);
			break;
		case DEF_COMMONTYPE_REQ_CREATEPORTION:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_CREATEPORTION");
			this->ReqCreatePortionHandler(pData);
			break;
		case DEF_COMMONTYPE_REQ_GETFISHTHISTIME:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETFISHTHISTIME");
			this->ReqGetFishThisTimeHandler();
			break;
		case DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM");
			this->ReqRepairItemCofirmHandler(iV1, pString);
			break;
		case DEF_COMMONTYPE_REQ_REPAIRITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEM");
			this->ReqRepairItemHandler(iV1, iV2, pString);
			break;
		case DEF_COMMONTYPE_REQ_SELLITEMCONFIRM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEMCONFIRM");
			this->ReqSellItemConfirmHandler(iV1, iV2, pString);
			break;
		case DEF_COMMONTYPE_REQ_SELLITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEM");
			this->ReqSellItemHandler(iV1, iV2, iV3, pString);
			break;
		case DEF_COMMONTYPE_REQ_USESKILL:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USESKILL");
			this->UseSkillHandler(iV1, iV2, iV3);
			break;
		case DEF_COMMONTYPE_REQ_USEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USEITEM");
			this->UseItemHandler(iV1, iV2, iV3, iV4);
			break;
		case DEF_COMMONTYPE_REQ_GETREWARDMONEY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETREWARDMONEY");
			this->GetRewardMoneyHandler();
			break;
		case DEF_COMMONTYPE_ITEMDROP:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_ITEMDROP");
			this->DropItemHandler(iV1, iV2, pString, true);
			break;
		case DEF_COMMONTYPE_EQUIPITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EQUIPITEM");
			this->bEquipItemHandler(iV1);
			// .....
			break;
		case DEF_COMMONTYPE_REQ_PURCHASEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_PURCHASEITEM");
			this->RequestPurchaseItemHandler(pString, iV1);
			break;
		case DEF_COMMONTYPE_REQ_STUDYMAGIC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_STUDYMAGIC");
			this->RequestStudyMagicHandler(pString);
			break;
		case DEF_COMMONTYPE_REQ_TRAINSKILL:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_TRAINSKILL");
			//RequestTrainSkillHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_GIVEITEMTOCHAR:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GIVEITEMTOCHAR");
			this->GiveItemHandler(cDir, iV1, iV2, iV3, iV4, pString);
			break;
		case DEF_COMMONTYPE_EXCHANGEITEMTOCHAR:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EXCHANGEITEMTOCHAR");
			this->ExchangeItemHandler(cDir, iV1, iV2, iV3, iV4, pString);
			break;
		case DEF_COMMONTYPE_JOINGUILDAPPROVE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDAPPROVE");
			this->JoinGuildApproveHandler(pString);
			break;
		case DEF_COMMONTYPE_JOINGUILDREJECT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDREJECT");
			this->JoinGuildRejectHandler(pString);
			break;
		case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDAPPROVE");
			this->DismissGuildApproveHandler(pString);
			break;
		case DEF_COMMONTYPE_DISMISSGUILDREJECT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDREJECT");
			this->DismissGuildRejectHandler(pString);
			break;
		case DEF_COMMONTYPE_RELEASEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_RELEASEITEM");
			this->ReleaseItemHandler(iV1, true);
			break;
		case DEF_COMMONTYPE_TOGGLECOMBATMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLECOMBATMODE");
			this->ToggleCombatModeHandler();
			break;
		case DEF_COMMONTYPE_MAGIC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_MAGIC");
			this->PlayerMagicHandler(iV1, iV2, (iV3 - 100));
			break;
		case DEF_COMMONTYPE_TOGGLESAFEATTACKMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLESAFEATTACKMODE");
			this->ToggleSafeAttackModeHandler();
			break;
		case DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET");
			this->GetFightzoneTicketHandler();
			break;
			// Upgrade Item
		case DEF_COMMONTYPE_UPGRADEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_UPGRADEITEM");
			this->RequestItemUpgradeHandler(iV1);
			break;
		case DEF_COMMONTYPE_REQGUILDNAME:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
			this->RequestGuildNameHandler(iV1, iV2);
			break;
		case DEF_COMMONTYPE_REQRANGO: // MORLA 2.2 - Llama al rango del pj
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
			this->RequestRango(iV1);
			break;
		case DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY");
			this->RequestAcceptJoinPartyHandler(iV1);
			break;
			// Crafting
		case DEF_COMMONTYPE_CRAFTITEM:
			this->ReqCreateCraftingHandler(pData);
			break;
		default:
			wsprintf(G_cTxt, "Unknown message received! (0x%.8X)", wCommand);
			PutLogList(G_cTxt);
			break;
	}
}

void CClient::ReqCreateSlateHandler(char* pData) {
	int i;
	int iRet;
	short *sp;
	char cItemID[4], ctr[4];
	char *cp, cSlateColour, cData[120];
	bool bIsSlatePresent = false;
	CItem * pItem;
	int iSlateType;
	int iEraseReq;
	uint32_t *dwp;
	uint16_t *wp;
	if (this->m_bIsOnServerChange == true) return;
	for (i = 0; i < 4; i++) {
		cItemID[i] = 0;
		ctr[i] = 0;
	}
	cp = (char *) pData;
	cp += 11;
	// 14% chance of creating slates
	if (iDice(1, 100) < game_.m_sSlateSuccessRate) bIsSlatePresent = true;
	try {
		// make sure slates really exist
		for (i = 0; i < 4; i++) {
			cItemID[i] = *cp;
			cp++;
			if (this->m_pItemList[cItemID[i]] == nullptr || cItemID[i] > DEF_MAXITEMS) {
				bIsSlatePresent = false;
				this->SendNotifyMsg(0, DEF_NOTIFY_SLATE_CREATEFAIL, 0, 0, 0, nullptr);
				return;
			}
			//No duping
			if (this->m_pItemList[cItemID[i]]->m_sIDnum == 868) // LU
				ctr[0] = 1;
			else if (this->m_pItemList[cItemID[i]]->m_sIDnum == 869) // LD
				ctr[1] = 1;
			else if (this->m_pItemList[cItemID[i]]->m_sIDnum == 870) // RU
				ctr[2] = 1;
			else if (this->m_pItemList[cItemID[i]]->m_sIDnum == 871) // RD
				ctr[3] = 1;
		}
	} catch (...) {
		//Crash Hacker Caught
		bIsSlatePresent = false;
		this->SendNotifyMsg(0, DEF_NOTIFY_SLATE_CREATEFAIL, 0, 0, 0, nullptr);
		wsprintf(G_cTxt, "TSearch Slate Hack: (%s) Player: (%s) - creating slates without correct item!", this->m_cIPaddress, this->m_cCharName);
		PutHackLogFileList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	// Are all 4 slates present ??
	if (ctr[0] != 1 || ctr[1] != 1 || ctr[2] != 1 || ctr[3] != 1) {
		bIsSlatePresent = false;
		return;
	}
	if (this->m_iAdminUserLevel > 3) bIsSlatePresent = true;
	// if we failed, kill everything
	if (!bIsSlatePresent) {
		for (i = 0; i < 4; i++) {
			if (this->m_pItemList[cItemID[i]] != nullptr) {
				this->ItemDepleteHandler(cItemID[i], false, false);
			}
		}
		this->SendNotifyMsg(0, DEF_NOTIFY_SLATE_CREATEFAIL, 0, 0, 0, nullptr);
		return;
	}
	// make the slates
	for (i = 0; i < 4; i++) {
		if (this->m_pItemList[cItemID[i]] != nullptr) {
			this->ItemDepleteHandler(cItemID[i], false, false);
		}
	}
	pItem = new class CItem;
	i = iDice(1, 1000);
	if (i < 50) { // Hp slate
		iSlateType = 1;
		cSlateColour = 32;
	} else if (i < 250) { // Bezerk slate
		iSlateType = 2;
		cSlateColour = 3;
	} else if (i < 750) { // Exp slate
		iSlateType = 4;
		cSlateColour = 7;
	} else if (i < 950) { // Mana slate
		iSlateType = 3;
		cSlateColour = 37;
	} else if (i < 1001) { // Hp slate
		iSlateType = 1;
		cSlateColour = 32;
	}
	// Notify client
	this->SendNotifyMsg(0, DEF_NOTIFY_SLATE_CREATESUCCESS, iSlateType, 0, 0, nullptr);
	std::memset(cData, 0, sizeof (cData));
	// Create slates
	if (game_._bInitItemAttr(*pItem, 867) == false) {
		delete pItem;
		return;
	} else {
		pItem->m_sTouchEffectType = DEF_ITET_ID;
		pItem->m_sTouchEffectValue1 = iDice(1, 100000);
		pItem->m_sTouchEffectValue2 = iDice(1, 100000);
		pItem->m_sTouchEffectValue3 = (short) timeGetTime();
		game_._bItemLog(DEF_ITEMLOG_GET, id_, -1, pItem);
		pItem->m_sItemSpecEffectValue2 = iSlateType;
		pItem->m_cItemColor = cSlateColour;
		if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
			std::memset(cData, 0, sizeof (cData));
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMOBTAINED;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char) 0;
			cp++;
			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor;
			cp++;
			*cp = (char) pItem->m_sItemSpecEffectValue2;
			cp++;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;
			if (iEraseReq == 1) delete pItem;
			iRet = this->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					return;
			}
		} else {
			this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
			this->map_->SendEventToNearClient_TypeB(MSGID_MAGICCONFIGURATIONCONTENTS, DEF_COMMONTYPE_ITEMDROP,
					  this->m_sX, this->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame,
					  pItem->m_cItemColor);
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					break;
			}
		}
	}
	return;
}

void CClient::DeleteClient(bool bSave, bool bNotify, bool bCountLogout, bool bForceCloseConn) {
	int i;
	if (this->m_bIsInitComplete == true) {
		if (memcmp(this->map_->m_cName, "fight", 5) == 0) {
			wsprintf(G_cTxt, "Char(%s)-Exit(%s)", this->m_cCharName, this->map_->m_cName);
			PutLogEventFileList(G_cTxt);
		}
		if (this->m_bIsExchangeMode == true) {
			auto with = this->exchangingWith_.lock();
			if (with) {
				with->_ClearExchangeStatus();
			}
			this->_ClearExchangeStatus();
		}
		if ((this->m_iAllocatedFish != 0) && (game_.m_pFish[this->m_iAllocatedFish] != nullptr))
			game_.m_pFish[this->m_iAllocatedFish]->m_sEngagingCount--;
		if (bNotify == true)
			this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
		this->RemoveFromTarget();
		for (auto &iterClient : game_.m_pClientList) {
			auto whisperedPlayer = iterClient.whisperedPlayer_.lock();
			if (whisperedPlayer && whisperedPlayer->id_ == id_) {
				iterClient.whisperedPlayer_.reset();
				iterClient.SendNotifyMsg(0, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, this->m_cCharName);
			}
		}
		this->map_->ClearOwner(2, id_, DEF_OWNERTYPE_PLAYER,
				  this->m_sX,
				  this->m_sY);
		game_.delayEvents_.remove(id_, DEF_OWNERTYPE_PLAYER, 0);
	}
	if ((bSave == true) && (this->m_bIsOnServerChange == false)) {
		if (this->m_bIsKilled == true) {
			this->m_sX = -1;
			this->m_sY = -1;
			auto tmpMap = this->map_;
			this->map_ = nullptr;
			if (this->m_cSide == 0) {
				this->map_ = game_.m_pMapList.byName("default");
			} else {
				if (memcmp(this->m_cLocation, "are", 3) == 0) {
					if (game_.m_bIsCrusadeMode == true) {
						if (this->m_iDeadPenaltyTime > 0) {
							this->lockedMap_ = game_.m_pMapList.byName("aresden");
							this->m_iLockedMapTime = 60 * 5;
							this->m_iDeadPenaltyTime = 60 * 10;
						} else {
							this->m_iDeadPenaltyTime = 60 * 10;
						}
					}
					if (strcmp(tmpMap->m_cName, "elvine") == 0) {
						this->map_ = this->lockedMap_ = game_.m_pMapList.byName("elvjail");
						this->m_iLockedMapTime = 10 * 2;
					} else if (this->m_iLevel > 80)
						this->map_ = game_.m_pMapList.byName("resurr1");
					else this->map_ = game_.m_pMapList.byName("resurr1");
				} else {
					if (game_.m_bIsCrusadeMode == true) {
						if (this->m_iDeadPenaltyTime > 0) {
							this->lockedMap_ = game_.m_pMapList.byName("elvine");
							this->m_iLockedMapTime = 60 * 5;
							this->m_iDeadPenaltyTime = 60 * 10;
						} else {
							this->m_iDeadPenaltyTime = 60 * 10;
						}
					}
					if (strcmp(tmpMap->m_cName, "aresden") == 0) {
						this->map_ = this->lockedMap_ = game_.m_pMapList.byName("arejail");
						this->m_iLockedMapTime = 10 * 2;
					} else if (this->m_iLevel > 80)
						this->map_ = game_.m_pMapList.byName("resurr2");
					else this->map_ = game_.m_pMapList.byName("resurr2");
				}
			}
		} else if (bForceCloseConn == true) {
			this->map_ = this->lockedMap_ = game_.m_pMapList.byName("bisle");
			this->m_sX = -1;
			this->m_sY = -1;
			this->m_iLockedMapTime = 10 * 60;
		}
		if (memcmp(this->map_->m_cName, "fight", 5) == 0 || this->m_bIsObserverMode == true) {
			this->map_.reset();
			if (this->m_cSide == 0) {
				switch (iDice(1, 2)) {
					case 1:
						this->map_ = game_.m_pMapList.byName("aresden");
						break;
					case 2:
						this->map_ = game_.m_pMapList.byName("elvine");
						break;
				}
			} else {
				this->map_ = game_.m_pMapList.byName(this->m_cLocation);
			}
			this->m_sX = -1;
			this->m_sY = -1;
		}
		if (this->m_bIsInitComplete == true) {
			if (this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, bCountLogout) == false) {
				this->LocalSavePlayerData();
			}
		} else {
			this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, bCountLogout);
		}
	} else {
		if (this->m_bIsOnServerChange == false) {
			this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, bCountLogout);
		} else {
			this->bSendMsgToLS(MSGID_REQUEST_SETACCOUNTWAITSTATUS, false);
		}
	}
	if (this->m_iPartyID != 0) {
		for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
			if (game_.m_stPartyInfo[this->m_iPartyID].iIndex[i] == id_) {
				game_.m_stPartyInfo[this->m_iPartyID].iIndex[i] = 0;
				game_.m_stPartyInfo[this->m_iPartyID].iTotalMembers--;
				wsprintf(G_cTxt, "PartyID:%d member:%d Out(Delete) Total:%d", this->m_iPartyID, id_, game_.m_stPartyInfo[this->m_iPartyID].iTotalMembers);
				PutLogList(G_cTxt);
				goto DC_LOOPBREAK1;
			}
DC_LOOPBREAK1:
		;
		for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
			if ((game_.m_stPartyInfo[this->m_iPartyID].iIndex[i] == 0) && (game_.m_stPartyInfo[this->m_iPartyID].iIndex[i + 1] != 0)) {
				game_.m_stPartyInfo[this->m_iPartyID].iIndex[i] = game_.m_stPartyInfo[this->m_iPartyID].iIndex[i + 1];
				game_.m_stPartyInfo[this->m_iPartyID].iIndex[i + 1] = 0;
			}
	}
	if (this->bCheckIfIsFlagCarrier()) {
		this->SetFlagCarrierFlag(false);
		this->SetIceFlag(false);
	}
	this->markedForDeletion_ = true;
}

void CClient::ArmorLifeDecrement(int iTargetH, char cOwnerType, int /*iValue*/) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (game_.m_pClientList[iTargetH] == nullptr) return;
			break;
		case DEF_OWNERTYPE_NPC: return;
		default: return;
	}
	auto &target = *game_.m_pClientList[iTargetH];
	if (this->m_cSide == target.m_cSide) return;
	if (target.m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0) return;

	struct Damage {
		std::size_t position_;
		std::size_t minEndurance_;
	};
	std::vector<Damage> damages[] = {
		{
			{DEF_EQUIPPOS_BODY, 380}
		},
		{
			{DEF_EQUIPPOS_PANTS, 250}
		},
		{
			{DEF_EQUIPPOS_LEGGINGS, 250}
		},
		{
			{DEF_EQUIPPOS_ARMS, 250}
		},
		{
			{DEF_EQUIPPOS_HEAD, 250}
		},
		{
			{DEF_EQUIPPOS_HEAD, 250},
			{DEF_EQUIPPOS_LEGGINGS, 250}
		},
		{
			{DEF_EQUIPPOS_LEGGINGS, 250},
			{DEF_EQUIPPOS_PANTS, 250}
		},
		{
			{DEF_EQUIPPOS_PANTS, 250},
			{DEF_EQUIPPOS_ARMS, 250}
		},
		{
			{DEF_EQUIPPOS_ARMS, 250}
		},
		{
			{DEF_EQUIPPOS_ARMS, 250},
			{DEF_EQUIPPOS_BODY, 250}
		},
		{
			{DEF_EQUIPPOS_BODY, 250},
			{DEF_EQUIPPOS_LEGGINGS, 250}
		},
		{
			{DEF_EQUIPPOS_BODY, 250}
		},
		{
			{DEF_EQUIPPOS_BODY, 250},
			{DEF_EQUIPPOS_PANTS, 250}
		}
	};
	constexpr size_t numItems = sizeof (damages) / sizeof (damages[0]);
	const int index = iDice(1, numItems) - 1;
	for (const Damage &dmg : damages[index]) {
		int itemIndex = target.m_sItemEquipmentStatus[dmg.position_];
		if ((itemIndex != -1) && (target.m_pItemList[itemIndex] != nullptr)) {
			auto &item = *target.m_pItemList[itemIndex];
			if ((target.m_cSide != 0) && (item.m_wCurLifeSpan > 0)) {
				item.m_wCurLifeSpan -= 50;
				if (item.m_wCurLifeSpan <= dmg.minEndurance_) {
					target.ReleaseItemHandler(itemIndex, true);
					target.SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, item.m_cEquipPos, itemIndex, 0, nullptr);
				}
			}
			if (item.m_wCurLifeSpan <= 0) {
				item.m_wCurLifeSpan = 0;
				target.SendNotifyMsg(0, DEF_NOTIFY_ITEMLIFESPANEND, item.m_cEquipPos, itemIndex, 0, nullptr);
				target.ReleaseItemHandler(itemIndex, true);
			}
		}
	}
}

void CClient::ReleaseItemHandler(short sItemIndex, bool bNotice) {
	short sTemp;
	int iTemp;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	auto &itemPtr = this->m_pItemList[sItemIndex];
	if (itemPtr == nullptr) return;
	auto &item = *itemPtr;
	if (item.m_cItemType != DEF_ITEMTYPE_EQUIP) return;
	if (this->m_bIsItemEquipped[sItemIndex] == false) return;
	char cHeroArmorType = this->_cCheckHeroItemEquipped();
	if (cHeroArmorType != 0x0FFFFFFFF) this->m_cHeroArmourBonus = 0;
	char cEquipPos = item.m_cEquipPos;
	if (cEquipPos == DEF_EQUIPPOS_RHAND) {
		if (this->m_pItemList[sItemIndex] != nullptr) {
			if ((item.m_sIDnum == 865) || (item.m_sIDnum == 866)) {
				this->m_cMagicMastery[94] = false;
				this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
			}
		}
	}
	switch (cEquipPos) {
		case DEF_EQUIPPOS_RHAND:
			sTemp = this->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			this->m_sAppr2 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0x0FFFFFFF;
			this->m_iApprColor = iTemp;
			iTemp = this->m_iStatus;
			iTemp = iTemp & 0xFFFFFFF0;
			this->m_iStatus = iTemp;
			break;
		case DEF_EQUIPPOS_LHAND:
			sTemp = this->m_sAppr2;
			sTemp = sTemp & 0xFFF0;
			this->m_sAppr2 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xF0FFFFFF;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_TWOHAND:
			sTemp = this->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			this->m_sAppr2 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0x0FFFFFFF;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_BODY:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			this->m_sAppr3 = sTemp;
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0xFF7F;
			this->m_sAppr4 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFF0FFFFF;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_BACK:
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0xF0FF;
			this->m_sAppr4 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_ARMS:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0xFFF0;
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFF0FFF;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_PANTS:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0xF0FF;
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFF0FF;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_LEGGINGS:
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0x0FFF;
			this->m_sAppr4 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFF0F;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_HEAD:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0xFF0F;
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF0;
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_RELEASEALL:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			this->m_iApprColor = iTemp;
			break;
	}
	if (item.m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) {
		this->m_sAppr4 = this->m_sAppr4 & 0xFFF3;
	}
	if (item.m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY) {
		this->m_sAppr4 = this->m_sAppr4 & 0xFFFC;
	}
	this->m_bIsItemEquipped[sItemIndex] = false;
	this->m_sItemEquipmentStatus[cEquipPos] = -1;
	if (bNotice == true)
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	this->CalcTotalItemEffect(sItemIndex, true);
}

void CClient::RequestRestartHandler() {
	char cTmpMap[32];
	if (this->m_bIsKilled == true) {
		strcpy(cTmpMap, this->map_->m_cName);
		std::memset(this->map_->m_cName, 0, sizeof (this->map_->m_cName));
		if (strcmp(this->m_cLocation, "NONE") == 0) {
			strcpy(this->map_->m_cName, "default");
		} else {
			if (strcmp(this->m_cLocation, "aresden") == 0) {
				if (game_.m_bIsCrusadeMode == true) {
					if (this->m_iDeadPenaltyTime > 0) {
						this->lockedMap_ = game_.m_pMapList.byName("aresden");
						this->m_iLockedMapTime = 60 * 5;
						this->m_iDeadPenaltyTime = 60 * 10;
					} else {
						this->lockedMap_ = game_.m_pMapList.byName("resurr1");
						this->m_iDeadPenaltyTime = 60 * 10;
					}
				}
				if (strcmp(cTmpMap, "elvine") == 0) {
					this->map_ = this->lockedMap_ = game_.m_pMapList.byName("elvjail");
					this->m_iLockedMapTime = 10 * 2;
				} else if (this->m_iLevel > 80) {
					this->map_ = game_.m_pMapList.byName("resurr1");
				} else {
					this->map_ = game_.m_pMapList.byName("aresden");
				}
			} else {
				if (game_.m_bIsCrusadeMode == true) {
					if (this->m_iDeadPenaltyTime > 0) {
						this->lockedMap_ = game_.m_pMapList.byName("elvine");
						this->m_iLockedMapTime = 60 * 5;
						this->m_iDeadPenaltyTime = 60 * 10;
					} else {
						this->map_ = game_.m_pMapList.byName("resurr2");
						this->m_iDeadPenaltyTime = 60 * 10;
					}
				}
				if (strcmp(cTmpMap, "aresden") == 0) {
					this->map_ = this->lockedMap_ = game_.m_pMapList.byName("arejail");
					this->m_iLockedMapTime = 10 * 2;
				} else if (this->m_iLevel > 80) {
					this->map_ = game_.m_pMapList.byName("resurr2");
				} else {
					this->map_ = game_.m_pMapList.byName("elvine");
				}
			}
		}
		this->m_bIsKilled = false;
		this->m_iHP = (3 * this->m_iVit) + (2 * this->m_iLevel) + (this->m_iStr / 2);
		this->m_iHungerStatus = 100;
		this->RequestTeleportHandler("2   ", this->map_->m_cName, -1, -1);
	}
}

void CClient::update(uint32_t dwTime) {
	int iPlusTime;
	int iMaxSuperAttack;
	int iValue;
	short sTemp;
	short sItemIndex;
	int iMapside;
	int iMapside2;
	if ((dwTime - this->m_dwTime) > DEF_CLIENTTIMEOUT) {
		if (this->m_bIsInitComplete == true) {
			//Testcode
			wsprintf(G_cTxt, "Client Timeout: %s", this->m_cIPaddress);
			PutLogList(G_cTxt);
			this->DeleteClient(true, true);
		} else if ((dwTime - this->m_dwTime) > DEF_CLIENTTIMEOUT) {
			this->DeleteClient(false, false);
		}
	} else if (this->m_bIsInitComplete == true) {
		this->m_iTimeLeft_ShutUp--;
		if (this->m_iTimeLeft_ShutUp < 0) this->m_iTimeLeft_ShutUp = 0;
		this->m_iTimeLeft_Rating--;
		if (this->m_iTimeLeft_Rating < 0) this->m_iTimeLeft_Rating = 0;
		if (((dwTime - this->m_dwHungerTime) > DEF_HUNGERTIME) && (this->m_bIsKilled == false)) {
			if ((this->m_iLevel < DEF_LEVELLIMIT) || (this->m_iAdminUserLevel >= 1)) {
			} else this->m_iHungerStatus--;
			if (this->m_iHungerStatus <= 0) this->m_iHungerStatus = 0;
			this->m_dwHungerTime = dwTime;
			if ((this->m_iHP > 0) && (this->m_iHungerStatus < 40)) {//50Cent - Hunger Fix
				this->SendNotifyMsg(0, DEF_NOTIFY_HUNGER, this->m_iHungerStatus, 0, 0, nullptr);
			}
		}
		if (this->_bCheckCharacterData() == false) {
			this->DeleteClient(true, true);
			return;
		}
		if ((this->m_iHungerStatus <= 30) && (this->m_iHungerStatus >= 0))
			iPlusTime = (30 - this->m_iHungerStatus)*1000;
		else iPlusTime = 0;
		iPlusTime = abs(iPlusTime);
		if ((dwTime - this->m_dwHPTime) > (uint32_t) (DEF_HPUPTIME + iPlusTime)) {
			this->TimeHitPointsUp();
			this->m_dwHPTime = dwTime;
		}
		if ((dwTime - this->m_dwMPTime) > (uint32_t) (DEF_MPUPTIME + iPlusTime)) {
			this->TimeManaPointsUp();
			this->m_dwMPTime = dwTime;
		}
		if ((dwTime - this->m_dwSPTime) > (uint32_t) (DEF_SPUPTIME + iPlusTime)) {
			this->TimeStaminarPointsUp();
			this->m_dwSPTime = dwTime;
		}
		if ((this->m_bIsPoisoned == true) && ((dwTime - this->m_dwPoisonTime) > DEF_POISONTIME)) {
			this->PoisonEffect(0);
			this->m_dwPoisonTime = dwTime;
		}
		if ((this->map_->m_bIsFightZone == false) &&
				  ((dwTime - this->m_dwAutoSaveTime) > (uint32_t) DEF_AUTOSAVETIME)) {
			this->bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA);
			this->m_dwAutoSaveTime = dwTime;
		}
		if ((dwTime - this->m_dwExpStockTime) > (uint32_t) DEF_EXPSTOCKTIME) {
			this->m_dwExpStockTime = dwTime;
			this->CalcExpStock();
			this->CheckUniqueItemEquipment();
			this->CheckCrusadeResultCalculation();
			this->CheckHeldenianResultCalculation(); // new
		}
		if ((dwTime - this->m_dwAutoExpTime) > (uint32_t) DEF_AUTOEXPTIME) {
			iValue = (this->m_iLevel / 2);
			if (iValue <= 0) iValue = 1;
			if (this->m_iAutoExpAmount < iValue) {
				if ((this->m_iExp + iValue) < game_.m_iLevelExpTable[this->m_iLevel + 1]) {
					//this->m_iExpStock += iValue;
					this->GetExp(iValue, false);
					this->CalcExpStock();
				}
			}
			this->m_iAutoExpAmount = 0;
			this->m_dwAutoExpTime = dwTime;
		}
		if (this->m_iSpecialAbilityTime == 3) {
			this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, nullptr);
			// New 25/05/2004
			// After the time up, add magic back
			sItemIndex = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
			if (sItemIndex != -1) {
				if ((this->m_pItemList[sItemIndex]->m_sIDnum == 865) || (this->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
					if ((this->m_iInt + this->m_iAngelicInt) > 99 && (this->m_iMag + this->m_iAngelicMag) > 99) {
						this->m_cMagicMastery[94] = true;
						this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
					}
				}
			}
		}
		this->m_iSpecialAbilityTime -= 3;
		if (this->m_iSpecialAbilityTime < 0) this->m_iSpecialAbilityTime = 0;
		if (this->m_bIsSpecialAbilityEnabled == true) {
			if (((dwTime - this->m_dwSpecialAbilityStartTime) / 1000) > this->m_iSpecialAbilityLastSec) {
				this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 3, 0, 0, nullptr);
				this->m_bIsSpecialAbilityEnabled = false;
				this->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
				sTemp = this->m_sAppr4;
				sTemp = sTemp & 0xFF0F;
				this->m_sAppr4 = sTemp;
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			}
		}
		//Crusade
		this->m_iLockedMapTime -= 3;
		if (this->m_iLockedMapTime < 0) {
			this->m_iLockedMapTime = 0;
			this->lockedMap_.reset();
		}

		this->m_iDeadPenaltyTime -= 3;
		if (this->m_iDeadPenaltyTime < 0) this->m_iDeadPenaltyTime = 0;
		if ((this->m_bIsWarLocation == true)) {
			// Crusade
			if (game_.m_bIsCrusadeMode == false)
				if (this->m_bIsInsideOwnTown == false)
					this->m_iTimeLeft_ForceRecall--;
			if (this->m_iTimeLeft_ForceRecall <= 0) {

				this->m_iTimeLeft_ForceRecall = 0;
				this->m_dwWarBeginTime = dwTime;
				this->m_bIsWarLocation = false;

				this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
				this->RequestTeleportHandler("1   ");
			}
		}
		if ((game_.m_bIsHeldenianMode == true) && (this->map_ != 0)) {
			if (game_.bCheckHeldenianMap(this->id_, game_.m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1) {
				this->SetHeroFlag(true);
			} else {
				this->SetHeroFlag(false);
			}
		}
		if (markedForDeletion_) return;
		if (this->m_iSkillMsgRecvCount >= 2) {
			//PutLogFileList(G_cTxt);
			this->DeleteClient(true, true);
		} else {
			this->m_iSkillMsgRecvCount = 0;
		}
		if (markedForDeletion_) return;
		//if (this->m_iLevel < this->map_->m_iLevelLimit) {
		if ((this->m_iLevel < this->map_->m_iLevelLimit) && (this->m_iAdminUserLevel < 2)) {
			this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
			this->RequestTeleportHandler("0   ");
		}
		if (markedForDeletion_) return;
		//if ( (this->map_->m_iUpperLevelLimit != 0) &&
		//	 (this->m_iLevel > this->map_->m_iUpperLevelLimit) ) {
		if ((this->map_->m_iUpperLevelLimit != 0) &&
				  (this->m_iLevel > this->map_->m_iUpperLevelLimit) && (this->m_iAdminUserLevel < 2)) {
			this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
			if ((this->m_cSide == 1) &&
					  (this->m_iAdminUserLevel == 0)) {
				this->RequestTeleportHandler("2   ", "aresden", -1, -1);
			} else if ((this->m_cSide == 2) &&
					  (this->m_iAdminUserLevel == 0)) {
				this->RequestTeleportHandler("2   ", "elvine", -1, -1);
			}
		}
		if (markedForDeletion_) return;
		if ((strcmp(this->m_cLocation, "elvine") != 0) &&
				  (strcmp(this->m_cLocation, "elvhunter") != 0) &&
				  (strcmp(this->m_cLocation, "arehunter") != 0) &&
				  (strcmp(this->m_cLocation, "aresden") != 0) &&
				  (this->m_iLevel >= 20) &&
				  (this->m_iAdminUserLevel == 0)) {
			wsprintf(G_cTxt, "Traveller Hack: (%s) Player: (%s) is a traveller and is greater than level 19.", this->m_cIPaddress, this->m_cCharName);
			PutHackLogFileList(G_cTxt);
			this->DeleteClient(true, true);
		}
		if (markedForDeletion_) return;
		if (((memcmp(this->m_cLocation, "aresden", 7) == 0) || (memcmp(this->m_cLocation, "elvine", 6) == 0)) &&
				  (game_.m_bIsCrusadeMode == true)) {
			iMapside = game_.iGetMapLocationSide(this->map_->m_cName);
			if (iMapside > 3) iMapside2 = iMapside - 2;
			else iMapside2 = iMapside;
			if ((this->m_cSide != iMapside2) && (iMapside != 0)) {
				if ((iMapside <= 2) && (this->m_iAdminUserLevel < 1)) {
					if (this->m_cSide != 0) {
						this->m_dwWarBeginTime = timeGetTime();
						this->m_bIsWarLocation = true;
						this->m_iTimeLeft_ForceRecall = 1;
						this->RequestTeleportHandler("1   ");
						this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
					}
				}
			}
		}
		if (markedForDeletion_) return;
		if (((memcmp(this->m_cLocation, "arehunter", 9) == 0) || (memcmp(this->m_cLocation, "elvhunter", 9) == 0)) &&
				  ((strcmp(this->map_->m_cName, "2ndmiddle") == 0) || (strcmp(this->map_->m_cName, "middleland") == 0))) {
			this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
			this->RequestTeleportHandler("1   ");
		}
		if (game_.m_bIsApocalypseMode == true) {
			if (memcmp(this->map_->m_cName, "abaddon", 7) == 0) {
				this->SendNotifyMsg(0, DEF_NOTIFY_APOCGATEOPEN, 167, 169, 0, this->map_->m_cName);
			} else if (memcmp(this->map_->m_cName, "icebound", 8) == 0) {
				this->SendNotifyMsg(0, DEF_NOTIFY_APOCGATEOPEN, 89, 31, 0, this->map_->m_cName);
			}
		}
		//50Cent - Capture The Flag
		if (game_.m_bIsCTFMode) {
			this->RequestCheckFlag();
			if (this->bCheckIfIsFlagCarrier()) {
				if (this->m_iHP >= 1) {
					this->SetInvisibilityFlag(false);
					this->SetIceFlag(true);
				}
			}
		}
		if (markedForDeletion_) return;
		if ((game_.m_bIsApocalypseMode == true) &&
				  (memcmp(this->map_->m_cName, "icebound", 8) == 0) &&
				  ((this->m_sX == 89 && this->m_sY == 31) ||
				  (this->m_sX == 89 && this->m_sY == 32) ||
				  (this->m_sX == 90 && this->m_sY == 31) ||
				  (this->m_sX == 90 && this->m_sY == 32))) {
			this->RequestTeleportHandler("2   ", "druncncity", -1, -1);
		}
		if (markedForDeletion_) return;
		if ((memcmp(this->m_cLocation, "are", 3) == 0) &&
				  (strcmp(this->map_->m_cName, "elvfarm") == 0)) {
			this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
			this->RequestTeleportHandler("0   ");
		}
		if (markedForDeletion_) return;
		if ((memcmp(this->m_cLocation, "elv", 3) == 0) &&
				  (strcmp(this->map_->m_cName, "arefarm") == 0)) {
			this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
			this->RequestTeleportHandler("0   ");
		}
		if (markedForDeletion_) return;
		if ((strcmp(this->map_->m_cName, "middleland") == 0)
				  && (strcmp(this->m_cLocation, "NONE") == 0) &&
				  (this->m_iAdminUserLevel < 1)) {

			this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
			this->RequestTeleportHandler("0   ");
		}
		if (this->m_bInRecallImpossibleMap == true) {
			this->m_iTimeLeft_ForceRecall--;
			if (this->m_iTimeLeft_ForceRecall <= 0) {
				this->m_iTimeLeft_ForceRecall = 0;
				this->m_bInRecallImpossibleMap = false;
				this->SendNotifyMsg(0, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, nullptr);
				this->RequestTeleportHandler("0   ");
			}
		}
		if (markedForDeletion_) return;
		this->m_iSuperAttackCount++;
		if (this->m_iSuperAttackCount > 12) {
			this->m_iSuperAttackCount = 0;
			iMaxSuperAttack = (this->m_iLevel / 10);
			if (this->m_iSuperAttackLeft < iMaxSuperAttack) this->m_iSuperAttackLeft++;
			this->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
		}
		this->m_iTimeLeft_FirmStaminar--;
		if (this->m_iTimeLeft_FirmStaminar < 0) this->m_iTimeLeft_FirmStaminar = 0;
		if (markedForDeletion_) return;
		if (this->m_bIsSendingMapStatus == true) this->_SendMapStatus();
		if (this->m_iConstructionPoint > 0) {
			this->CheckCommanderConstructionPoint();
		}
	}
}

bool CGame::bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType) {
	short tX;
	short tY;
	int iRet;
	class CTile * pTile;
	iRet = 0;
	if (m_pClientList[sAttackerH] == nullptr) return 0;
	if ((m_bIsHeldenianMode == 1) || (m_cHeldenianModeType == 1)) {
		if (cType == DEF_OWNERTYPE_PLAYER) {
			if ((m_pClientList[sAttackerH]->map_ != nullptr) && (m_pClientList[sAttackerH]->m_cSide > 0)) {
				tX = m_pClientList[sAttackerH]->m_sX;
				tY = m_pClientList[sAttackerH]->m_sY;
				if ((tX < 0) || (tX >= m_pClientList[sAttackerH]->map_->m_sSizeX) ||
						  (tY < 0) || (tY >= m_pClientList[sAttackerH]->map_->m_sSizeY)) return 0;
				pTile = (class CTile *)(m_pClientList[sAttackerH]->map_->m_pTile + tX + tY * m_pClientList[sAttackerH]->map_->m_sSizeY);
				if (pTile == nullptr) return 0;
				if (pTile->m_iOccupyStatus != 0) {
					if (pTile->m_iOccupyStatus < 0) {
						if (m_pClientList[sAttackerH]->m_cSide == 1) {
							iRet = 1;
						}
					} else if (pTile->m_iOccupyStatus > 0) {
						if (m_pClientList[sAttackerH]->m_cSide == 2) {
							iRet = 1;
						}
					}
				}
			}
		} else if (cType == DEF_OWNERTYPE_NPC) {
			if ((m_pNpcList[sAttackerH]->map_ != nullptr) && (iMapIndex != -1) && (m_pNpcList[sAttackerH]->m_cSide > 0)) {
				tX = m_pNpcList[sAttackerH]->m_sX;
				tY = m_pNpcList[sAttackerH]->m_sY;
				pTile = (class CTile *)(m_pNpcList[sAttackerH]->map_->m_pTile + tX + tY * m_pNpcList[sAttackerH]->map_->m_sSizeY);
				if (pTile == nullptr) return 0;
				if (pTile->m_iOccupyStatus != 0) {
					if (pTile->m_iOccupyStatus < 0) {
						if (m_pNpcList[sAttackerH]->m_cSide == 1) {
							iRet = 1;
						}
					} else if (pTile->m_iOccupyStatus > 0) {
						if (m_pNpcList[sAttackerH]->m_cSide == 2) {
							iRet = 1;
						}
					}
				}
			}
		}
	}
	return iRet;
}

CMap& CClient::_getMap() {
	return *map_;
}

UnitPtr CClient::_getPtr() {
	return shared_from_this();
}

int CClient::_getEffectiveIceResist() {
	if (this->m_iAdminUserLevel > 0) {
		return 100;
	}
	if (this->m_dwWarmEffectTime != 0) {
		if ((timeGetTime() - this->m_dwWarmEffectTime) < 1000 * 30) {
			return 100;
		}
	}
	return this->m_iAddAbsWater * 2;
}

void CClient::_sendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3) {
	int * ip;
	char * cp_a, * cp_s, * cp_sv, cData_All[200], cData_Srt[200], cData_Srt_Av[200];
	uint32_t * dwp;
	uint16_t * wp;
	int * ipStatus, iDumm;
	short * sp, sRange, sX, sY;
	bool cOwnerSend;
	char cKey;
	int iTemp3;
	int iTemp;
	int iTemp2;
	std::memset(cData_All, 0, sizeof (cData_All));
	std::memset(cData_Srt, 0, sizeof (cData_Srt));
	std::memset(cData_Srt_Av, 0, sizeof (cData_Srt_Av));
	ipStatus = (int *) &iDumm;
	cKey = (rand() % 255) + 1;
	dwp = (uint32_t *) (cData_All + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData_All + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;
	dwp = (uint32_t *) (cData_Srt + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData_Srt + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;
	dwp = (uint32_t *) (cData_Srt_Av + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	wp = (uint16_t *) (cData_Srt_Av + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;
	cp_a = (char *) (cData_All + DEF_INDEX2_MSGTYPE + 2);
	cp_s = (char *) (cData_Srt + DEF_INDEX2_MSGTYPE + 2);
	cp_sv = (char *) (cData_Srt_Av + DEF_INDEX2_MSGTYPE + 2);
	if ((dwMsgID == MSGID_EVENT_LOG) || (wMsgType == DEF_OBJECTMOVE) || (wMsgType == DEF_OBJECTRUN) ||
			  (wMsgType == DEF_OBJECTATTACKMOVE) || (wMsgType == DEF_OBJECTDAMAGEMOVE) || (wMsgType == DEF_OBJECTDYING))
		sRange = 1;
	else sRange = 0;
	switch (wMsgType) {
		case DEF_OBJECTNULLACTION:
		case DEF_OBJECTDAMAGE:
		case DEF_OBJECTDYING:
		case DEF_MSGTYPE_CONFIRM:
			cOwnerSend = true;
			break;
		default:
			cOwnerSend = false;
			break;
	}
	wp = (uint16_t *) cp_a;
	*wp = id_;
	cp_a += 2;
	sp = (short *) cp_a;
	sX = this->m_sX;
	*sp = sX;
	cp_a += 2;
	sp = (short *) cp_a;
	sY = this->m_sY;
	*sp = sY;
	cp_a += 2;
	sp = (short *) cp_a;
	*sp = this->m_sType;
	cp_a += 2;
	*cp_a = this->m_cDir;
	cp_a++;
	memcpy(cp_a, this->m_cCharName, 10);
	cp_a += 10;
	sp = (short *) cp_a;
	*sp = this->m_sAppr1;
	cp_a += 2;
	sp = (short *) cp_a;
	*sp = this->m_sAppr2;
	cp_a += 2;
	sp = (short *) cp_a;
	*sp = this->m_sAppr3;
	cp_a += 2;
	sp = (short *) cp_a;
	*sp = this->m_sAppr4;
	cp_a += 2;
	ip = (int *) cp_a;
	*ip = this->m_iApprColor;
	cp_a += 4;
	ip = (int *) cp_a;
	ipStatus = ip;
	*ip = this->m_iStatus;
	cp_a += 4;
	iTemp3 = this->m_iStatus & 0x0F0FFFF7F; //0F0FFFF7Fh
	if (wMsgType == DEF_OBJECTNULLACTION) {
		if (this->m_bIsKilled == true)
			*cp_a = 1;
		else *cp_a = 0;
	} else *cp_a = 0;
	cp_a++;
	wp = (uint16_t *) cp_s;
	*wp = id_ + 30000;
	cp_s += 2;
	*cp_s = this->m_cDir;
	cp_s++;
	*cp_s = (unsigned char) sV1;
	cp_s++;
	*cp_s = (unsigned char) sV2;
	cp_s++;
	sp = (short *) cp_s;
	sX = this->m_sX;
	*sp = sX;
	cp_s += 2;
	sp = (short *) cp_s;
	sY = this->m_sY;
	*sp = sY;
	cp_s += 2;
	wp = (uint16_t *) cp_sv;
	*wp = id_ + 30000;
	cp_sv += 2;
	*cp_sv = this->m_cDir;
	cp_sv++;
	*cp_sv = sV1 - sX;
	cp_sv++;
	*cp_sv = sV2 - sY;
	cp_sv++;
	sp = (short *) cp_sv;
	*sp = sV3;
	cp_sv += 2;
	for (auto &clientIter : game_.m_pClientList) {
		if (clientIter.m_bIsInitComplete == true) {
			if ((clientIter.map_ == this->map_) &&
					  (clientIter.m_sX >= this->m_sX - 10 - sRange) &&
					  (clientIter.m_sX <= this->m_sX + 10 + sRange) &&
					  (clientIter.m_sY >= this->m_sY - 8 - sRange) &&
					  (clientIter.m_sY <= this->m_sY + 8 + sRange)) {
				if (this->m_cSide != clientIter.m_cSide) {
					if (clientIter.m_iAdminUserLevel > 0) {
						iTemp = this->m_iStatus;
					} else if (clientIter.id_ != id_) {
						iTemp = iTemp3;
					} else {
						iTemp = this->m_iStatus;
					}
				} else {
					iTemp = this->m_iStatus;
				}
				iTemp = 0x0FFFFFFF & iTemp;
				iTemp2 = this->iGetPlayerABSStatus();
				iTemp = (iTemp | (iTemp2 << 28));
				*ipStatus = iTemp;
				if ((clientIter.m_sX >= this->m_sX - 9) &&
						  (clientIter.m_sX <= this->m_sX + 9) &&
						  (clientIter.m_sY >= this->m_sY - 7) &&
						  (clientIter.m_sY <= this->m_sY + 7)) {
					switch (wMsgType) {
						case DEF_MSGTYPE_CONFIRM:
						case DEF_MSGTYPE_REJECT:
						case DEF_OBJECTNULLACTION:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_All, 43, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_All, 43, cKey);
							break;
						case DEF_OBJECTATTACK:
						case DEF_OBJECTATTACKMOVE:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
							break;
						case DEF_OBJECTMAGIC:
						case DEF_OBJECTDAMAGE:
						case DEF_OBJECTDAMAGEMOVE:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 11, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 11, cKey);
							break;
						case DEF_OBJECTDYING:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 15, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 15, cKey);
							break;
						default:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 9, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 9, cKey);
							break;
					}
				} else {
					switch (wMsgType) {
						case DEF_MSGTYPE_CONFIRM:
						case DEF_MSGTYPE_REJECT:
						case DEF_OBJECTNULLACTION:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_All, 43, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_All, 43, cKey);
							break;
						case DEF_OBJECTATTACK:
						case DEF_OBJECTATTACKMOVE:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
							break;
						case DEF_OBJECTMAGIC:
						case DEF_OBJECTDAMAGE:
						case DEF_OBJECTDAMAGEMOVE:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 11, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 11, cKey);
							break;
						case DEF_OBJECTDYING:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 15, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_Srt, 15, cKey);
							break;
						default:
							if (cOwnerSend == true)
								clientIter.m_pXSock->iSendMsg(cData_All, 43, cKey);
							else
								if (clientIter.id_ != id_)
								clientIter.m_pXSock->iSendMsg(cData_All, 43, cKey);
							break;
					}
				}
			}
		}
	}
}

int CClient::iComposeInitMapData(short sX, short sY, char * pData) {
	register int * ip, ix, iy, iSize, iTileExists;
	class CTile * pTileSrc, * pTile;
	unsigned char ucHeader;
	short * sp, * pTotal;
	int sTemp;
	int sTemp2;
	uint16_t * wp;
	char * cp;
	pTotal = (short *) pData;
	cp = (char *) (pData + 2);
	iSize = 2;
	iTileExists = 0;
	pTileSrc = (class CTile *)(this->map_->m_pTile +
			  (sX) + (sY) * this->map_->m_sSizeY);
	for (iy = 0; iy < 16; iy++) {
		for (ix = 0; ix < 21; ix++) {
			if (((sX + ix) == 100) && ((sY + iy) == 100))
				sX = sX;
			pTile = (class CTile *)(pTileSrc + ix + iy * this->map_->m_sSizeY);
			//If player not same side and is invied (Beholder Hack)

			if (pTile->m_sOwner && (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER) && (game_.m_pClientList[pTile->m_sOwner] != nullptr) && (pTile->m_sOwner != this->id_)) {
				if ((game_.m_pClientList[pTile->m_sOwner]->m_cSide != 0) &&
						  (game_.m_pClientList[pTile->m_sOwner]->m_cSide != this->m_cSide) &&
						  ((game_.m_pClientList[pTile->m_sOwner]->m_iStatus & 0x00000010) != 0)) {
					continue;
				}
			}
			if ((pTile->m_sOwner != 0) || (pTile->m_sDeadOwner != 0) ||
					  (pTile->m_pItem[0] != nullptr) || (pTile->m_sDynamicObjectType != 0)) {
				iTileExists++;
				sp = (short *) cp;
				*sp = ix;
				cp += 2;
				sp = (short *) cp;
				*sp = iy;
				cp += 2;
				iSize += 4;
				ucHeader = 0;
				if (pTile->m_sOwner != 0) {
					if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER) {
						if (game_.m_pClientList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
						else {
							// ###debugcode
							wsprintf(G_cTxt, "Empty player handle: %d", pTile->m_sOwner);
							//PutLogFileList(G_cTxt);
							//
							pTile->m_sOwner = 0;
						}
					}
					if (pTile->m_cOwnerClass == DEF_OWNERTYPE_NPC) {
						if (game_.m_pNpcList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
						else pTile->m_sOwner = 0;
					}
				}
				if (pTile->m_sDeadOwner != 0) {
					if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_PLAYER) {
						if (game_.m_pClientList[pTile->m_sDeadOwner] != nullptr) ucHeader = ucHeader | 0x02;
						else pTile->m_sDeadOwner = 0;
					}
					if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_NPC) {
						if (game_.m_pNpcList[pTile->m_sDeadOwner] != nullptr) ucHeader = ucHeader | 0x02;
						else pTile->m_sDeadOwner = 0;
					}
				}
				if (pTile->m_pItem[0] != nullptr) ucHeader = ucHeader | 0x04;
				if (pTile->m_sDynamicObjectType != 0) ucHeader = ucHeader | 0x08;
				//
				*cp = ucHeader;
				cp++;
				iSize++;
				if ((ucHeader & 0x01) != 0) {
					switch (pTile->m_cOwnerClass) {
						case DEF_OWNERTYPE_PLAYER:
							// Object ID number(Player) : 1~10000
							sp = (short *) cp;
							*sp = pTile->m_sOwner;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = game_.m_pClientList[pTile->m_sOwner]->m_cDir;
							cp++;
							iSize++;
							// Appearance1
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr1;
							cp += 2;
							iSize += 2;
							// Appearance2
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Appearance3
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr3;
							cp += 2;
							iSize += 2;
							// Appearance4
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr4;
							cp += 2;
							iSize += 2;
							// v1.4 ApprColor
							ip = (int *) cp;
							*ip = game_.m_pClientList[pTile->m_sOwner]->m_iApprColor;
							cp += 4;
							iSize += 4;
							// Status
							ip = (int *) cp;
							sTemp = game_.m_pClientList[pTile->m_sOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = game_.m_pClientList[pTile->m_sOwner]->iGetPlayerABSStatus(); //(short)iGetPlayerRelationship(this->id_, pTile->m_sOwner);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, game_.m_pClientList[pTile->m_sOwner]->m_cCharName, 10);
							cp += 10;
							iSize += 10;
							break;
						case DEF_OWNERTYPE_NPC:
							// Object ID number(NPC) : 10000~
							sp = (short *) cp;
							*sp = pTile->m_sOwner + 10000;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = game_.m_pNpcList[pTile->m_sOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = game_.m_pNpcList[pTile->m_sOwner]->m_cDir;
							cp++;
							iSize++;
							// Appr2
							sp = (short *) cp;
							*sp = game_.m_pNpcList[pTile->m_sOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Status
							ip = (int *) cp;
							sTemp = game_.m_pNpcList[pTile->m_sOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = game_.iGetNpcRelationship(pTile->m_sOwner, this->id_);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, game_.m_pNpcList[pTile->m_sOwner]->m_cName, 5);
							cp += 5;
							iSize += 5;
							break;
					}
				}
				if ((ucHeader & 0x02) != 0) {
					switch (pTile->m_cDeadOwnerClass) {
						case DEF_OWNERTYPE_PLAYER:
							// Object ID number : 1~10000
							sp = (short *) cp;
							*sp = pTile->m_sDeadOwner;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = game_.m_pClientList[pTile->m_sDeadOwner]->m_cDir;
							cp++;
							iSize++;
							// Appearance1
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr1;
							cp += 2;
							iSize += 2;
							// Appearance2
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Appearance3
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr3;
							cp += 2;
							iSize += 2;
							// Appearance4
							sp = (short *) cp;
							*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr4;
							cp += 2;
							iSize += 2;
							// v1.4 ApprColor
							ip = (int *) cp;
							*ip = game_.m_pClientList[pTile->m_sDeadOwner]->m_iApprColor;
							cp += 4;
							iSize += 4;
							// Status
							ip = (int *) cp;
							sTemp = game_.m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = game_.iGetPlayerABSStatus(pTile->m_sDeadOwner, this->id_); //(short)iGetPlayerRelationship(this->id_, pTile->m_sDeadOwner);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, game_.m_pClientList[pTile->m_sDeadOwner]->m_cCharName, 10);
							cp += 10;
							iSize += 10;
							break;
						case DEF_OWNERTYPE_NPC:
							// Object ID number : 10000	~
							sp = (short *) cp;
							*sp = pTile->m_sDeadOwner + 10000;
							cp += 2;
							iSize += 2;
							// object type
							sp = (short *) cp;
							*sp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_sType;
							cp += 2;
							iSize += 2;
							// dir
							*cp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_cDir;
							cp++;
							iSize++;
							// Appr2
							sp = (short *) cp;
							*sp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_sAppr2;
							cp += 2;
							iSize += 2;
							// Status
							ip = (int *) cp;
							sTemp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_iStatus;
							sTemp = 0x0FFFFFFF & sTemp;
							sTemp2 = game_.iGetNpcRelationship(pTile->m_sDeadOwner, this->id_);
							sTemp = (sTemp | (sTemp2 << 28)); //Original : 12
							*ip = sTemp;
							//*sp = DEF_TEST;
							cp += 4; //Original 2
							iSize += 4; //Original 2
							// Name
							memcpy(cp, game_.m_pNpcList[pTile->m_sDeadOwner]->m_cName, 5);
							cp += 5;
							iSize += 5;
							break;
					}
				}
				if (pTile->m_pItem[0] != nullptr) {
					sp = (short *) cp;
					*sp = pTile->m_pItem[0]->m_sSprite;
					cp += 2;
					iSize += 2;
					sp = (short *) cp;
					*sp = pTile->m_pItem[0]->m_sSpriteFrame;
					cp += 2;
					iSize += 2;
					*cp = pTile->m_pItem[0]->m_cItemColor;
					cp++;
					iSize++;
				}
				if (pTile->m_sDynamicObjectType != 0) {
					wp = (uint16_t *) cp;
					*wp = pTile->m_wDynamicObjectID;
					cp += 2;
					iSize += 2;
					sp = (short *) cp;
					*sp = pTile->m_sDynamicObjectType;
					cp += 2;
					iSize += 2;
				}
			} // Big if
		} // while(1)
	}
	*pTotal = iTileExists;
	return iSize;
}

int CClient::iComposeMoveMapData(short sX, short sY, char cDir, char * pData) {
	constexpr int _tmp_iMoveLocX[9][37] = {
		// 0
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		// 1
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, -1, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//2
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
			20, 20, 20, 20, 20, 20, -1},
		//3
		{20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
			20, 20, 20, 20, 20, 20, -1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//4
		{20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
			20, 20, 20, 20, 20, 20, 19, 18, 17, 16,
			15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
			5, 4, 3, 2, 1, 0, -1},
		//5
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, -1, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//6
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, -1},
		//7
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, -1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//8
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, -1}
	};
	constexpr int _tmp_iMoveLocY[9][37] = {
		// 0
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//1
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//2
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, -1},
		//3
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, -1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//4
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 15, 15, 15, 15,
			15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
			15, 15, 15, 15, 15, 15, -1},
		//5
		{15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
			15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
			15, -1, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		//6
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 15, 15, 15, 15,
			15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
			15, 15, 15, 15, 15, 15, -1},
		//7
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, -1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0},
		// 8
		{15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
			5, 4, 3, 2, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, -1}
	};

	register int * ip, ix, iy, iSize, iTileExists, iIndex;
	class CTile * pTileSrc, * pTile;
	unsigned char ucHeader;
	short * sp, * pTotal;
	int iTemp;
	int iTemp2;
	uint16_t * wp;
	char * cp;
	pTotal = (short *) pData;
	cp = (char *) (pData + 2);
	iSize = 2;
	iTileExists = 0;
	pTileSrc = (class CTile *)(this->map_->m_pTile +
			  (sX) + (sY) * this->map_->m_sSizeY);
	iIndex = 0;
	while (1) {
		ix = _tmp_iMoveLocX[cDir][iIndex];
		iy = _tmp_iMoveLocY[cDir][iIndex];
		if ((ix == -1) || (iy == -1)) break;
		iIndex++;
		pTile = (class CTile *)(pTileSrc + ix + iy * this->map_->m_sSizeY);
		//If player not same side and is invied (Beholder Hack)
		// there is another person on the tiles, and the owner is not the player
		//xxxxxx
		if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER && (game_.m_pClientList[pTile->m_sOwner] != nullptr) && (pTile->m_sOwner != this->id_))
			if ((game_.m_pClientList[pTile->m_sOwner]->m_cSide != 0) &&
					  (game_.m_pClientList[pTile->m_sOwner]->m_cSide != this->m_cSide) &&
					  ((game_.m_pClientList[pTile->m_sOwner]->m_iStatus & 0x00000010) != 0)) {
				continue;
			}
		if ((pTile->m_sOwner != 0) || (pTile->m_sDeadOwner != 0) ||
				  (pTile->m_pItem[0] != nullptr) || (pTile->m_sDynamicObjectType != 0)) {
			iTileExists++;
			sp = (short *) cp;
			*sp = ix;
			cp += 2;
			sp = (short *) cp;
			*sp = iy;
			cp += 2;
			iSize += 4;
			ucHeader = 0;
			if (pTile->m_sOwner != 0) {
				if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER) {
					if (game_.m_pClientList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
					else pTile->m_sOwner = 0;
				}
				if (pTile->m_cOwnerClass == DEF_OWNERTYPE_NPC) {
					if (game_.m_pNpcList[pTile->m_sOwner] != nullptr) ucHeader = ucHeader | 0x01;
					else pTile->m_sOwner = 0;
				}
			}
			if (pTile->m_sDeadOwner != 0) {
				if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_PLAYER) {
					if (game_.m_pClientList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
					else pTile->m_sDeadOwner = 0;
				}
				if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_NPC) {
					if (game_.m_pNpcList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
					else pTile->m_sDeadOwner = 0;
				}
			}
			if (pTile->m_pItem[0] != nullptr) ucHeader = ucHeader | 0x04;
			if (pTile->m_sDynamicObjectType != 0) ucHeader = ucHeader | 0x08;
			*cp = ucHeader;
			cp++;
			iSize++;
			if ((ucHeader & 0x01) != 0) {
				switch (pTile->m_cOwnerClass) {
					case DEF_OWNERTYPE_PLAYER:
						sp = (short *) cp;
						*sp = pTile->m_sOwner;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sOwner]->m_sType;
						cp += 2;
						iSize += 2;
						*cp = game_.m_pClientList[pTile->m_sOwner]->m_cDir;
						cp++;
						iSize++;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr1;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr3;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sOwner]->m_sAppr4;
						cp += 2;
						iSize += 2;
						ip = (int *) cp;
						*ip = game_.m_pClientList[pTile->m_sOwner]->m_iApprColor;
						cp += 4;
						iSize += 4;
						ip = (int *) cp;
						if (this->m_cSide != game_.m_pClientList[pTile->m_sOwner]->m_cSide) {
							if (this->id_ != pTile->m_sOwner) {
								iTemp = game_.m_pClientList[pTile->m_sOwner]->m_iStatus & 0x0F0FFFF7F;
							} else {
								iTemp = game_.m_pClientList[pTile->m_sOwner]->m_iStatus;
							}
						} else {
							iTemp = game_.m_pClientList[pTile->m_sOwner]->m_iStatus;
						}
						//iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = game_.iGetPlayerABSStatus(pTile->m_sOwner, this->id_);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;
						memcpy(cp, game_.m_pClientList[pTile->m_sOwner]->m_cCharName, 10);
						cp += 10;
						iSize += 10;
						break;
					case DEF_OWNERTYPE_NPC:
						sp = (short *) cp;
						*sp = pTile->m_sOwner + 10000;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pNpcList[pTile->m_sOwner]->m_sType;
						cp += 2;
						iSize += 2;
						*cp = game_.m_pNpcList[pTile->m_sOwner]->m_cDir;
						cp++;
						iSize++;
						sp = (short *) cp;
						*sp = game_.m_pNpcList[pTile->m_sOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;
						ip = (int *) cp;
						iTemp = game_.m_pNpcList[pTile->m_sOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = game_.iGetNpcRelationship(pTile->m_sOwner, this->id_);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;
						memcpy(cp, game_.m_pNpcList[pTile->m_sOwner]->m_cName, 5);
						cp += 5;
						iSize += 5;
				}//end switch
			}// if ((ucHeader & 0x01) != 0)
			if ((ucHeader & 0x02) != 0) {
				switch (pTile->m_cDeadOwnerClass) {
					case DEF_OWNERTYPE_PLAYER:
						sp = (short *) cp;
						*sp = pTile->m_sDeadOwner;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sType;
						cp += 2;
						iSize += 2;
						*cp = game_.m_pClientList[pTile->m_sDeadOwner]->m_cDir;
						cp++;
						iSize++;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr1;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr3;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pClientList[pTile->m_sDeadOwner]->m_sAppr4;
						cp += 2;
						iSize += 2;
						ip = (int *) cp;
						*ip = game_.m_pClientList[pTile->m_sDeadOwner]->m_iApprColor;
						cp += 4;
						iSize += 4;
						ip = (int *) cp;
						if (this->m_cSide != game_.m_pClientList[pTile->m_sDeadOwner]->m_cSide) {
							if (this->id_ != pTile->m_sDeadOwner) {
								iTemp = game_.m_pClientList[pTile->m_sDeadOwner]->m_iStatus & 0x0F0FFFF7F;
							} else {
								iTemp = game_.m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
							}
						} else {
							iTemp = game_.m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
						}
						//iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = game_.iGetPlayerABSStatus(pTile->m_sDeadOwner, this->id_);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;
						memcpy(cp, game_.m_pClientList[pTile->m_sDeadOwner]->m_cCharName, 10);
						cp += 10;
						iSize += 10;
						break;
					case DEF_OWNERTYPE_NPC:
						sp = (short *) cp;
						*sp = pTile->m_sDeadOwner + 10000;
						cp += 2;
						iSize += 2;
						sp = (short *) cp;
						*sp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_sType;
						cp += 2;
						iSize += 2;
						*cp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_cDir;
						cp++;
						iSize++;
						sp = (short *) cp;
						*sp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_sAppr2;
						cp += 2;
						iSize += 2;
						ip = (int *) cp;
						iTemp = game_.m_pNpcList[pTile->m_sDeadOwner]->m_iStatus;
						iTemp = 0x0FFFFFFF & iTemp;
						iTemp2 = game_.iGetNpcRelationship(pTile->m_sDeadOwner, this->id_);
						iTemp = (iTemp | (iTemp2 << 28));
						*ip = iTemp;
						cp += 4;
						iSize += 4;
						memcpy(cp, game_.m_pNpcList[pTile->m_sDeadOwner]->m_cName, 5);
						cp += 5;
						iSize += 5;
						break;
				}//End Switch
			}// if ((ucHeader & 0x02) != 0)
			if (pTile->m_pItem[0] != nullptr) {
				sp = (short *) cp;
				*sp = pTile->m_pItem[0]->m_sSprite;
				cp += 2;
				iSize += 2;
				sp = (short *) cp;
				*sp = pTile->m_pItem[0]->m_sSpriteFrame;
				cp += 2;
				iSize += 2;
				*cp = pTile->m_pItem[0]->m_cItemColor;
				cp++;
				iSize++;
			}
			if (pTile->m_sDynamicObjectType != 0) {
				wp = (uint16_t *) cp;
				*wp = pTile->m_wDynamicObjectID;
				cp += 2;
				iSize += 2;
				sp = (short *) cp;
				*sp = pTile->m_sDynamicObjectType;
				cp += 2;
				iSize += 2;
			} //(pTile->m_sDynamicObjectType != nullptr)
		} //(pTile->m_sOwner != nullptr)
	} // end While(1)
	*pTotal = iTileExists;
	return iSize;
}

bool CClient::bSendMsgToLS(uint32_t dwMsg, bool bFlag, char* pData) {
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	int i;
	int iSize;
	char cCharName[11], cAccountName[11], cAccountPassword[11], cAddress[16], cGuildName[21], cTxt[120], * cp;
	char cGuildLoc[11], cTemp[120];
	int * ip, iSendSize;
	/*

	m_iCurSubLogSockIndex++;
	if (m_iCurSubLogSockIndex >= DEF_MAXSUBLOGSOCK) m_iCurSubLogSockIndex = 0;

	iCnt = 0;
	bLoopFlag = false;
	while (bLoopFlag == false) {
		if (m_pSubLogSock[m_iCurSubLogSockIndex] != nullptr)
			 bLoopFlag = true;
		else m_iCurSubLogSockIndex++;
		iCnt++;
		if (iCnt >= DEF_MAXSUBLOGSOCK) {

			if (m_bOnExitProcess == false) {
				m_cShutDownCode      = 3;
				m_bOnExitProcess     = true;
				m_dwExitProcessTime  = timeGetTime();

				PutLogList("(!) GAME SERVER SHUTDOWN PROCESS STARTED(by Log-server connection Lost)!!!");
			}
			return false;
		}
	}
	 */
	// v1.41
	std::memset(G_cData50000, 0, sizeof (G_cData50000));
	std::memset(cCharName, 0, sizeof (cCharName));
	std::memset(cAccountName, 0, sizeof (cAccountName));
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));
	std::memset(cAddress, 0, sizeof (cAddress));
	std::memset(cGuildName, 0, sizeof (cGuildName));
	std::memset(cGuildLoc, 0, sizeof (cGuildLoc));
	switch (dwMsg) {
			// New 07/05/2004
		case MSGID_GAMEMASTERLOG:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			if (pData == nullptr) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMEMASTERLOG;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			iSize = strlen(pData);
			memcpy((char *) cp, pData, iSize);
			iRet = game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 6 + iSize);
			iSendSize = 6 + iSize;
			break;
			// v2.15
		case MSGID_GAMEITEMLOG:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			//		if (this->markedForDeletion_) return false;
			if (pData == nullptr) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMEITEMLOG;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			iSize = strlen(pData);
			memcpy((char *) cp, pData, iSize);
			iRet = game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 6 + iSize);
			iSendSize = 6 + iSize;
			break;
		case MSGID_SENDSERVERSHUTDOWNMSG:
			if (game_.m_pMainLogSock == nullptr) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_SENDSERVERSHUTDOWNMSG;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			iRet = game_.m_pMainLogSock->iSendMsg(G_cData50000, 6);
			break;
		case MSGID_GAMESERVERSHUTDOWNED:
			if (game_.m_pMainLogSock == nullptr) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_GAMESERVERSHUTDOWNED;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			iRet = game_.m_pMainLogSock->iSendMsg(G_cData50000, 6);
			return true;
		case MSGID_REQUEST_SETACCOUNTWAITSTATUS:
		case MSGID_REQUEST_SETACCOUNTINITSTATUS:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = dwMsg;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cp, this->m_cAccountName, 10);
			cp += 10;
			ip = (int *) cp;
			*ip = this->m_iLevel;
			cp += 4;
			iRet = game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 20);
			iSendSize = 16;
			break;
		case MSGID_ENTERGAMECONFIRM:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_ENTERGAMECONFIRM;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cp, this->m_cAccountName, 10);
			cp += 10;
			memcpy(cp, this->m_cAccountPassword, 10);
			cp += 10;
			memcpy(cp, game_.m_cServerName, 10);
			cp += 10;
			std::memset(cTxt, 0, sizeof (cTxt));
			this->m_pXSock->iGetPeerAddress(cTxt);
			memcpy(cp, cTxt, 16);
			cp += 16;
			ip = (int *) cp;
			*ip = this->m_iLevel;
			cp += 4;
			//testcode
			wsprintf(G_cTxt, "Confirmed. Account: (%s) Name: (%s) Level: (%d)", this->m_cAccountName, this->m_cCharName, this->m_iLevel);
			PutLogList(G_cTxt);
			if ((game_.m_bIsCrusadeMode == false) && (this->m_dwCrusadeGUID == m_dwCrusadeGUID)) {
				break;
			} else
				this->m_dwCrusadeGUID = m_dwCrusadeGUID;
			iRet = game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 56);
			iSendSize = 56;
			break;
		case MSGID_REQUEST_REGISTERGAMESERVER:
			if (game_.m_pMainLogSock == nullptr) return false;
			wsprintf(cTxt, "(!) Try to register game server(%s)", game_.m_cServerName);
			PutLogList(cTxt);
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_REGISTERGAMESERVER;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cAccountName, m_cServerName, 10);
			if (game_.m_iGameServerMode == 1) {
				memcpy(cAddress, game_.m_cGameServerAddrExternal, strlen(game_.m_cGameServerAddrExternal));
			}
			if (game_.m_iGameServerMode == 2) {
				memcpy(cAddress, game_.m_cGameServerAddr, strlen(game_.m_cGameServerAddr));
			}
			memcpy(cp, cAccountName, 10);
			cp += 10;
			memcpy(cp, cAddress, 16);
			cp += 16;
			wp = (uint16_t *) cp;
			*wp = game_.m_iGameServerPort;
			cp += 2;
			*cp = game_.m_iTotalMaps;
			cp++;
			for (i = 0; i < game_.m_iTotalMaps; i++) {
				memcpy(cp, game_.m_pMapList[i]->m_cName, 11);
				cp += 11;
			}
			iRet = game_.m_pMainLogSock->iSendMsg(G_cData50000, 35 + game_.m_iTotalMaps * 11);
			return true;
		case MSGID_REQUEST_PLAYERDATA:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_PLAYERDATA;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			//testcode
			if (strlen(this->m_cCharName) == 0) PutLogList("(X) CharName nullptr!");
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cAccountName, this->m_cAccountName, 10);
			memcpy(cAccountPassword, this->m_cAccountPassword, 10);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;
			std::memset(cTemp, 0, sizeof (cTemp));
			iRet = this->m_pXSock->iGetPeerAddress(cTemp);
			memcpy((char *) cp, cTemp, 15);
			cp += 15;
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 52);
			iSendSize = 52;
			break;
		case MSGID_REQUEST_SAVEPLAYERDATA_REPLY:
		case MSGID_REQUEST_SAVEPLAYERDATA:
		case MSGID_REQUEST_SAVEPLAYERDATALOGOUT:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = dwMsg;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cAccountName, this->m_cAccountName, 10);
			memcpy(cAccountPassword, this->m_cAccountPassword, 10);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;
			*cp = (char) bFlag;
			cp++;
			iSize = this->_iComposePlayerDataFileContents(cp);
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 37 + iSize);
			iSendSize = 37 + iSize;
			break;
		case MSGID_REQUEST_NOSAVELOGOUT:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_NOSAVELOGOUT;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cAccountName, this->m_cAccountName, 10);
			memcpy(cAccountPassword, this->m_cAccountPassword, 10);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;
			*cp = (char) bFlag;
			cp++;
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 37);
			iSendSize = 37;
			break;
		case MSGID_REQUEST_CREATENEWGUILD:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_CREATENEWGUILD;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cAccountName, this->m_cAccountName, 10);
			memcpy(cAccountPassword, this->m_cAccountPassword, 10);
			memcpy(cGuildName, this->m_cGuildName, 20);
			memcpy(cGuildLoc, this->m_cLocation, 10);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;
			memcpy((char *) cp, cGuildName, 20);
			cp += 20;
			memcpy((char *) cp, cGuildLoc, 10);
			cp += 10;
			ip = (int *) cp;
			*ip = this->m_iGuildGUID;
			cp += 4;
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 75);
			iSendSize = 75;
			break;
		case MSGID_REQUEST_DISBANDGUILD:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_DISBANDGUILD;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cAccountName, this->m_cAccountName, 10);
			memcpy(cAccountPassword, this->m_cAccountPassword, 10);
			memcpy(cGuildName, this->m_cGuildName, 20);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountName, 10);
			cp += 10;
			memcpy((char *) cp, cAccountPassword, 10);
			cp += 10;
			memcpy((char *) cp, cGuildName, 20);
			cp += 20;
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 56);
			iSendSize = 56;
			break;
		case MSGID_REQUEST_HELDENIAN_WINNER:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_HELDENIAN_WINNER;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			if (game_.m_cHeldenianVictoryType == 1)
				memcpy(cp, "aresden", 7);
			else if (game_.m_cHeldenianVictoryType == 2)
				memcpy(cp, "elvine", 6);
			else
				memcpy(cp, "draw", 4);
			cp += 7;
			ip = (int *) cp;
			*ip = game_.m_dwHeldenianStartTime;
			cp += 4;
			ip = (int *) cp;
			*ip = game_.m_dwHeldenianFinishTime;
			cp += 4;
			ip = (int *) cp;
			*ip = game_.m_cHeldenianModeType;
			cp += 4;
			iRet = game_.m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 21);
			iSendSize = 21;
			break;
		case MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cGuildName, this->m_cGuildName, 20);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cGuildName, 20);
			cp += 20;
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 36);
			iSendSize = 36;
			break;
		case MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN:
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			if (this->markedForDeletion_) return false;
			dwp = (uint32_t *) (G_cData50000 + DEF_INDEX4_MSGID);
			*dwp = MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN;
			wp = (uint16_t *) (G_cData50000 + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_CONFIRM;
			cp = (char *) (G_cData50000 + DEF_INDEX2_MSGTYPE + 2);
			memcpy(cCharName, this->m_cCharName, 10);
			memcpy(cGuildName, this->m_cGuildName, 20);
			memcpy((char *) cp, cCharName, 10);
			cp += 10;
			memcpy((char *) cp, cGuildName, 20);
			cp += 20;
			iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 36);
			iSendSize = 36;
			break;
	}
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			wsprintf(G_cTxt, "(!!!) Sub-log-socket(%d) send error!", game_.m_iCurSubLogSockIndex);
			PutLogList(G_cTxt);
			PutLogFileList(G_cTxt);
			delete game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex];
			game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex] = nullptr;
			game_.m_bIsSubLogSockAvailable[game_.m_iCurSubLogSockIndex] = false;
			game_.m_iSubLogSockActiveCount--;
			game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex] = new class XSocket(game_.m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
			game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->bConnect(game_.m_cLogServerAddr, game_.m_iLogServerPort, (WM_ONLOGSOCKETEVENT + game_.m_iCurSubLogSockIndex + 1));
			game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->bInitBufferSize(DEF_MSGBUFFERSIZE);
			wsprintf(G_cTxt, "(!) Try to reconnect sub-log-socket(%d)... Addr:%s  Port:%d", game_.m_iCurSubLogSockIndex, game_.m_cLogServerAddr, game_.m_iLogServerPort);
			PutLogList(G_cTxt);
			game_.m_iSubLogSockFailCount++;
			if (game_._bCheckSubLogSocketIndex() == false) return false;
			game_.m_pSubLogSock[game_.m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, iSendSize);
			return false;
	}
	return true;
}

void CClient::InitPlayerData(char * pData, uint32_t dwSize) {
	char * cp, cName[11], cData[256], cTxt[256], cGuildStatus, cQuestRemain;
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	int i;
	int iTotalPoints;
	bool bRet;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == true) return;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cName, 0, sizeof (cName));
	memcpy(cName, cp, 10);
	cp += 10;
	//this->m_cAccountStatus = *cp;
	cp++;
	cGuildStatus = *cp;
	cp++;
	this->m_iHitRatio = 0;
	this->m_iDefenseRatio = 0;
	this->m_cSide = 0;
	bRet = this->_bDecodePlayerDatafileContents(cp, dwSize - 19);
	if (bRet == false) {
		wsprintf(G_cTxt, "(HACK?) Character(%s) data error!", this->m_cCharName);
		this->DeleteClient(false, true); //!!!
		return;
	}
	if (m_bHappyHour == true) {
		StarHappyHour();
	}
	//***************************************************************************
	this->___RestorePlayerCharacteristics();
	this->___RestorePlayerRating();
	if ((this->m_sX == -1) && (this->m_sY == -1)) {
		this->map_->GetMapInitialPoint(&this->m_sX, &this->m_sY, this->m_cLocation);
	}
	// New 17/05/2004
	this->SetPlayingStatus();
	int iTemp;
	int iTemp2;
	iTemp = this->m_iStatus;
	iTemp = 0x0FFFFFFF & iTemp;
	iTemp2 = this->iGetPlayerABSStatus();
	iTemp = iTemp | (iTemp2 << 28);
	this->m_iStatus = iTemp;
	if (this->m_iLevel > 100)
		if (this->m_bIsPlayerCivil == true)
			this->ForceChangePlayMode(false);
	this->m_iNextLevelExp = m_iLevelExpTable[this->m_iLevel + 1]; //iGetLevelExp(this->m_iLevel + 1);
	this->CalcTotalItemEffect(-1, true); //false
	this->iCalcTotalWeight();
	// New 23/05/2004
	//GM Invi comes here
	if (this->m_iAdminUserLevel > 0) {
		this->SetInvisibilityFlag(DEF_OWNERTYPE_PLAYER, true);
	}
	if ((this->m_iLevel > 2) && (this->m_iAdminUserLevel == 0) &&
			  (this->m_iExp < iGetLevelExp(this->m_iLevel - 1) - 3000)) {
		this->m_iExp = iGetLevelExp(this->m_iLevel - 1) - 3000;
		wsprintf(G_cTxt, "Problema, posible editaci�n: (%s) Jugador: (%s) tiene la experiencia bajo 0 - (Exp:%d).", this->m_cIPaddress, this->m_cCharName, this->m_iExp);
		PutHackLogFileList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	iTotalPoints = 0;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		iTotalPoints += this->m_cSkillMastery[i];
	if ((iTotalPoints - 21 > m_sCharSkillLimit) && (this->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) - has more than allowed skill points (%d).", this->m_cIPaddress, this->m_cCharName, iTotalPoints);
		PutHackLogFileList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	this->CheckSpecialEvent();
	this->bCheckMagicInt();
	if ((cGuildStatus == 0) && (memcmp(this->m_cGuildName, "NONE", 4) != 0)) {
		std::memset(this->m_cGuildName, 0, sizeof (this->m_cGuildName));
		strcpy(this->m_cGuildName, "NONE");
		this->m_iGuildRank = -1;
		this->m_iGuildGUID = -1;
		this->SendNotifyMsg(0, DEF_NOTIFY_GUILDDISBANDED, 0, 0, 0, this->m_cGuildName);
	}
	if (this->m_iQuest != 0) {
		cQuestRemain = (m_pQuestConfigList[this->m_iQuest]->m_iMaxCount - this->m_iCurQuestCount);
		this->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, nullptr);
		this->_bCheckIsQuestCompleted();
	}
	if (this->markedForDeletion_) {
		wsprintf(cTxt, "<%d> InitPlayerData error - Socket error! Disconnected.", iClientH);
		PutLogList(cTxt);
		return;
	}
	std::memset(cData, 0, sizeof (cData));
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITPLAYER;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	iRet = this->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			wsprintf(cTxt, "<%d> InitPlayerData - Socket error! Disconnected.", iClientH);
			PutLogList(cTxt);
			this->DeleteClient(false, true); //!!!!!
			return;
	}
	this->m_bIsInitComplete = true;
	bSendMsgToLS(MSGID_ENTERGAMECONFIRM, iClientH);
	this->m_dwLastActionTime = this->m_dwAFKCheckTime = timeGetTime();
	this->PlayerOrder_GetSkills();
	return;
}

bool CClient::_bDecodePlayerDatafileContents(char * pData, uint32_t dwSize) {
	char * pContents, * token, * pOriginContents, cTmpName[11], cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA;
	char cReadModeB;
	int i;
	int iItemIndex;
	int iItemInBankIndex;
	int iTotalGold;
	int iNotUsedItemPrice;
	class CStrTok * pStrTok;
	short sTmpType;
	short sTmpAppr1;
	bool bRet;
	int iTemp;
	SYSTEMTIME SysTime;
	__int64 iDateSum1;
	__int64 iDateSum2;
	bool bIsNotUsedItemFound = false;
	if (this->markedForDeletion_) return false;
	iTotalGold = 0;
	iItemIndex = 0;
	iItemInBankIndex = 0;
	iNotUsedItemPrice = 0;
	cReadModeA = 0;
	cReadModeB = 0;
	pContents = new char[dwSize + 2];
	ZeroMemory(pContents, dwSize + 2);
	memcpy(pContents, pData, dwSize);
	pOriginContents = pContents;
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					this->map_ = m_pMapList.byName(token);
					if (!this->map_) {
						wsprintf(cTxt, "(!) Player(%s) tries to enter unknown map : %s", this->m_cCharName, cTmpName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					cReadModeA = 0;
					break;
				case 2:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_sX = atoi(token);
					cReadModeA = 0;
					break;
				case 3:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_sY = atoi(token);
					cReadModeA = 0;
					break;
				case 4:
					/*
					if (_bGetIsStringIsNumber(token) == false) {
					wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
					PutLogList(cTxt);
					delete pContents;
					delete pStrTok;
					return false;
					}
					this->m_cAccountStatus = atoi(token);
					 */
					cReadModeA = 0;
					break;
				case 5:
					switch (cReadModeB) {
						case 1:
							// New 07/05/2004
							// v2.12
							if (iItemIndex >= DEF_MAXITEMS) {
								delete pContents;
								delete pStrTok;
								return false;
							}
							if (_bInitItemAttr(*this->m_pItemList[iItemIndex], token) == false) {
								wsprintf(cTxt, "(!!!) Client(%s)-Item(%s) is not existing Item! Conection closed.", this->m_cCharName, token);
								PutLogList(cTxt);
								// Debug code @@@@@@@@@@@@@@@
								HANDLE hFile;
								DWORD nWrite;
								hFile = CreateFile("Error.Log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
								WriteFile(hFile, (char *) pContents, dwSize + 2, &nWrite, nullptr);
								CloseHandle(hFile);
								///////////// @@@@@@@@@@@@@@@
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							// m_dwCount
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							iTemp = atoi(token);
							if (iTemp < 0) iTemp = 1;
							if (iGetItemWeight(*this->m_pItemList[iItemIndex], iTemp) > this->_iCalcMaxLoad()) {
								iTemp = 1;
								wsprintf(G_cTxt, "(!) Player�(%s) Item (%s) too heavy for player to carry", this->m_cCharName, this->m_pItemList[iItemIndex]->m_cName);
								PutLogFileList(G_cTxt);
								PutLogList(G_cTxt);
							}
							this->m_pItemList[iItemIndex]->m_dwCount = (uint32_t) iTemp;
							cReadModeB = 3;
							if (memcmp(this->m_pItemList[iItemIndex]->m_cName, "Gold", 4) == 0)
								iTotalGold += iTemp;
							break;
						case 3:
							// m_sTouchEffectType
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sTouchEffectType = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// m_sTouchEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// m_sTouchEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							// m_sTouchEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							// m_cItemColor
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_cItemColor = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							// m_sItemSpecEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = atoi(token);
							cReadModeB = 9;
							break;
						case 9:
							// m_sItemSpecEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 = atoi(token);
							cReadModeB = 10;
							break;
						case 10:
							// m_sItemSpecEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue3 = atoi(token);
							cReadModeB = 11;
							break;
						case 11:
							// m_wCurLifeSpan
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_wCurLifeSpan = atoi(token);
							cReadModeB = 12;
							break;
						case 12:
							// m_dwAttribute
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemList[iItemIndex]->m_dwAttribute = atoi(token);
							if (this->m_pItemList[iItemIndex]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER) {
								if ((this->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != this->m_sCharIDnum1) ||
										  (this->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != this->m_sCharIDnum2) ||
										  (this->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != this->m_sCharIDnum3)) {
									wsprintf(cTxt, "(!) ´Ù¸¥ »ç¶÷ÀÇ ¾ÆÀÌÅÛ ¼ÒÁö: Player(%s) Item(%s) %d %d %d - %d %d %d", this->m_cCharName, this->m_pItemList[iItemIndex]->m_cName,
											  this->m_pItemList[iItemIndex]->m_sTouchEffectValue1,
											  this->m_pItemList[iItemIndex]->m_sTouchEffectValue2,
											  this->m_pItemList[iItemIndex]->m_sTouchEffectValue3,
											  this->m_sCharIDnum1,
											  this->m_sCharIDnum2,
											  this->m_sCharIDnum3);
									PutLogList(cTxt);
									//PutLogFileList(cTxt);
								}
							}
							cReadModeA = 0;
							cReadModeB = 0;
							if ((this->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0) {
								this->m_pItemList[iItemIndex]->m_wMaxLifeSpan = this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;
							}
							_AdjustRareItemValue(*this->m_pItemList[iItemIndex]);
							if (this->m_pItemList[iItemIndex]->m_wCurLifeSpan > this->m_pItemList[iItemIndex]->m_wMaxLifeSpan)
								this->m_pItemList[iItemIndex]->m_wCurLifeSpan = this->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
							// v1.433
							if ((this->m_pItemList[iItemIndex]->m_wCurLifeSpan == 0) &&
									  (this->m_pItemList[iItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {
								wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¼ö¸í 0Â¥¸® Èñ»ý¼® ¼ÒÁö!", this->m_cCharName);
								PutLogFileList(G_cTxt);
								this->m_pItemList[iItemIndex]->m_wCurLifeSpan = 1;
							}
							if (this->m_pItemList[iItemIndex]->m_cItemType == DEF_ITEMTYPE_NOTUSED) {
								iNotUsedItemPrice += this->m_pItemList[iItemIndex]->m_wPrice;
								this->m_pItemList[iItemIndex].reset();
								// v1.41
								bIsNotUsedItemFound = true;
							} else
								if (_bCheckDupItemID(& * this->m_pItemList[iItemIndex]) == true) {
								_bItemLog(DEF_ITEMLOG_DUPITEMID, iClientH, 0, & * this->m_pItemList[iItemIndex]);
								iNotUsedItemPrice += this->m_pItemList[iItemIndex]->m_wPrice;
								this->m_pItemList[iItemIndex].reset();
							} else iItemIndex++;
							break;
					}
					break;
				case 6:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cSex = atoi(token);
					cReadModeA = 0;
					break;
				case 7:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cSkin = atoi(token);
					cReadModeA = 0;
					break;
				case 8:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cHairStyle = atoi(token);
					cReadModeA = 0;
					break;
				case 9:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cHairColor = atoi(token);
					cReadModeA = 0;
					break;
				case 10:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cUnderwear = atoi(token);
					cReadModeA = 0;
					break;
				case 11:
					for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
						this->m_sItemEquipmentStatus[i] = -1;
					for (i = 0; i < DEF_MAXITEMS; i++)
						this->m_bIsItemEquipped[i] = false;
					if (bIsNotUsedItemFound == false) {
						for (i = 0; i < DEF_MAXITEMS; i++) {
							wsprintf(cTxt, "%d", token[i]);
							if ((token[i] == '1') && (this->m_pItemList[i] != nullptr)) {
								if (this->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_EQUIP)
									this->m_bIsItemEquipped[i] = true;
								else this->m_bIsItemEquipped[i] = false;
							} else this->m_bIsItemEquipped[i] = false;
							if ((!this->markedForDeletion_) && (this->m_bIsItemEquipped[i] == true)) {
								if (this->bEquipItemHandler(i) == false) // false
									this->m_bIsItemEquipped[i] = false;
							}
						}
					}
					cReadModeA = 0;
					break;
				case 12:
					std::memset(this->m_cGuildName, 0, sizeof (this->m_cGuildName));
					strcpy(this->m_cGuildName, token);
					cReadModeA = 0;
					break;
				case 13:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iGuildRank = atoi(token);
					cReadModeA = 0;
					break;
				case 14:
					// HP
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iHP = atoi(token);
					cReadModeA = 0;
					break;
				case 15:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					//this->m_iOriginDefenseRatio = atoi(token);
					//this->m_iCurDefenseRatio = this->m_iOriginDefenseRatio;
					cReadModeA = 0;
					break;
				case 16:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					//this->m_iOriginHitRatio = atoi(token);
					//this->m_iCurHitRatio = this->m_iOriginHitRatio;
					cReadModeA = 0;
					break;
				case 17:
					// Level
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iLevel = atoi(token);
					cReadModeA = 0;
					break;
				case 18:
					// Str
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iStr = atoi(token);
					cReadModeA = 0;
					break;
				case 19:
					// Int
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iInt = atoi(token);
					cReadModeA = 0;
					break;
				case 20:
					// Vit
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iVit = atoi(token);
					cReadModeA = 0;
					break;
				case 21:
					// Dex
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iDex = atoi(token);
					cReadModeA = 0;
					break;
				case 22:
					// Mag
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iMag = atoi(token);
					cReadModeA = 0;
					break;
				case 23:
					// Charisma
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iCharisma = atoi(token);
					cReadModeA = 0;
					break;
				case 24:
					// Luck
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iLuck = atoi(token);
					cReadModeA = 0;
					break;
				case 25:
					// Exp
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iExp = atoi(token);
					cReadModeA = 0;
					break;
				case 26:
					for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
						this->m_cMagicMastery[i] = token[i] - 48;
					}
					cReadModeA = 0;
					break;
				case 27:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cSkillMastery[cReadModeB] = atoi(token);
					cReadModeB++;
					if (cReadModeB >= DEF_MAXSKILLTYPE) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;
					//
				case 28:
					switch (cReadModeB) {
						case 1:
							if (iItemInBankIndex >= DEF_MAXBANKITEMS) {
								delete pContents;
								delete pStrTok;
								return false;
							}
							if (_bInitItemAttr(*this->m_pItemInBankList[iItemInBankIndex], token) == false) {
								wsprintf(cTxt, "(!!!) Client(%s)-Bank Item(%s) is not existing Item! Conection closed.", this->m_cCharName, token);
								PutLogList(cTxt);
								// Debug code @@@@@@@@@@@@@@@
								HANDLE hFile;
								DWORD nWrite;
								hFile = CreateFile("Error.Log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
								WriteFile(hFile, (char *) pOriginContents, dwSize + 2, &nWrite, nullptr);
								CloseHandle(hFile);
								///////////// @@@@@@@@@@@@@@@
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							// m_dwCount
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							iTemp = atoi(token);
							if (iTemp < 0) iTemp = 1;
							if (iGetItemWeight(*this->m_pItemInBankList[iItemInBankIndex], iTemp) > this->_iCalcMaxLoad()) {
								iTemp = 1;
								wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¾ÆÀÌÅÛ(%s) °³¼ö ¿À¹öÇÃ·Î¿ì", this->m_cCharName, this->m_pItemInBankList[iItemInBankIndex]->m_cName);
								PutLogFileList(G_cTxt);
								PutLogList(G_cTxt);
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_dwCount = (uint32_t) iTemp;
							cReadModeB = 3;
							if (memcmp(this->m_pItemInBankList[iItemInBankIndex]->m_cName, "Gold", 4) == 0)
								iTotalGold += iTemp;
							break;
						case 3:
							// m_sTouchEffectType
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectType = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// m_sTouchEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue1 = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// m_sTouchEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue2 = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							// m_sTouchEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue3 = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							// m_cItemColor
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_cItemColor = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							// m_sItemSpecEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1 = atoi(token);
							cReadModeB = 9;
							break;
						case 9:
							// m_sItemSpecEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue2 = atoi(token);
							cReadModeB = 10;
							break;
						case 10:
							// m_sItemSpecEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue3 = atoi(token);
							cReadModeB = 11;
							break;
						case 11:
							// m_wCurLifeSpan
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = atoi(token);
							cReadModeB = 12;
							break;
						case 12:
							// m_dwAttribute
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							if ((this->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute & 0x00000001) != 0) {
								this->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan = this->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1;
							}
							int iValue = (this->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute & 0xF0000000) >> 28;
							if (iValue > 0) {
								switch (this->m_pItemInBankList[iItemInBankIndex]->m_cCategory) {
									case 5: // ¹æÆÐ
									case 6: // ¹æ¾î±¸
										this->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan = this->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1;
										break;
								}
							}
							_AdjustRareItemValue(*this->m_pItemInBankList[iItemInBankIndex]);
							if (this->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan > this->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan)
								this->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = this->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan;
							// v1.433
							if ((this->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan == 0) &&
									  (this->m_pItemInBankList[iItemInBankIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {
								wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¼ö¸í 0Â¥¸® Èñ»ý¼® ¼ÒÁö!", this->m_cCharName);
								PutLogFileList(G_cTxt);
								this->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = 1;
							}
							if (this->m_pItemInBankList[iItemInBankIndex]->m_cItemType == DEF_ITEMTYPE_NOTUSED) {
								iNotUsedItemPrice += this->m_pItemInBankList[iItemInBankIndex]->m_wPrice;
								this->m_pItemInBankList[iItemInBankIndex].reset();
							} else
								if (_bCheckDupItemID(& * this->m_pItemInBankList[iItemInBankIndex]) == true) {
								_bItemLog(DEF_ITEMLOG_DUPITEMID, iClientH, 0, & * this->m_pItemInBankList[iItemInBankIndex]);
								iNotUsedItemPrice += this->m_pItemInBankList[iItemInBankIndex]->m_wPrice;
								this->m_pItemInBankList[iItemInBankIndex].reset();
							} else iItemInBankIndex++;
							break;
					}
					break;
					//
				case 29:
					std::memset(this->m_cLocation, 0, sizeof (this->m_cLocation));
					strcpy(this->m_cLocation, token);
					if (memcmp(this->m_cLocation + 3, "hunter", 6) == 0)
						this->m_bIsPlayerCivil = true;
					cReadModeA = 0;
					break;
					/* 2.03 Code - Fixed by KLKS
								case 29:

									std::memset(this->m_cLocation, 0, sizeof(this->m_cLocation));
									strcpy(this->m_cLocation, token);
									cReadModeA = 0;
									break;
					 */
				case 30:
					// m_iMP
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iMP = atoi(token);
					cReadModeA = 0;
					break;
				case 31:
					// m_iSP
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iSP = atoi(token);
					cReadModeA = 0;
					break;
				case 32:
					// m_cLU_Pool
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iLU_Pool = atoi(token);
					cReadModeA = 0;
					break;
					/*
					case 33:
						// m_cLU_Vit
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						this->m_cLU_Vit = atoi(token);
						cReadModeA = 0;
						break;
					case 34:
						// m_cLU_Dex
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						this->m_cLU_Dex = atoi(token);
						cReadModeA = 0;
						break;
					case 35:
						// m_cLU_Int
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						this->m_cLU_Int = atoi(token);
						cReadModeA = 0;
						break;
					case 36:
						// m_cLU_Mag
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						this->m_cLU_Mag = atoi(token);
						cReadModeA = 0;
						break;
					case 37:
						// m_cLU_Char
						if (_bGetIsStringIsNumber(token) == false) {
							wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
							PutLogList(cTxt);
							delete pContents;
							delete pStrTok;
							return false;
						}
						this->m_cLU_Char = atoi(token);
						cReadModeA = 0;
						break;
					 */
				case 38:
					// m_iEnemyKillCount
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iEnemyKillCount = atoi(token);
					cReadModeA = 0;
					break;
				case 39:
					// m_iPKCount
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iPKCount = atoi(token);
					cReadModeA = 0;
					break;
				case 40:
					// m_iRewardGold
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iRewardGold = atoi(token);
					cReadModeA = 0;
					break;
				case 41:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iSkillSSN[cReadModeB] = atoi(token);
					cReadModeB++;
					if (cReadModeB >= DEF_MAXSKILLTYPE) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;
				case 42:
					if (token != nullptr) {
						std::memset(this->m_cProfile, 0, sizeof (this->m_cProfile));
						strcpy(this->m_cProfile, token);
					}
					cReadModeA = 0;
					break;
				case 43:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iHungerStatus = atoi(token);
					cReadModeA = 0;
					break;
				case 44:
					// AdminUserLevel
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iAdminUserLevel = 0; //Default it to 0
					if (atoi(token) > 0) {
						for (i = 0; i < DEF_MAXADMINS; i++) {
							if (strlen(m_stAdminList[i].m_cGMName) == 0) break; //No more GM's on list
							if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(this->m_cCharName))) {
								if (memcmp(m_stAdminList[i].m_cGMName, this->m_cCharName, strlen(this->m_cCharName)) == 0) {
									this->m_iAdminUserLevel = atoi(token);
									break; //Break goes to cReadModeA = 0;, so no need to do it again
								}
							}
						}
					}
					cReadModeA = 0;
					break;
					/*case 44:
					// AdminUserLevel
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					for (i = 0; i < DEF_MAXADMINS; i++) {
						if (atoi(token) > 0) {
							if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(this->m_cCharName))) {
								this->m_iAdminUserLevel = atoi(token);
								cReadModeA = 0;
								break;
							}
							else {
								this->m_iAdminUserLevel = 0;
							}
						else this->m_iAdminUserLevel = 0;
						}
					}
					cReadModeA = 0;
					break;*/
				case 45:
					// TimeLeft_ShutUp
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iTimeLeft_ShutUp = atoi(token);
					cReadModeA = 0;
					break;
				case 46:
					// TimeLeft_Rating
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iTimeLeft_Rating = atoi(token);
					cReadModeA = 0;
					break;
				case 47:
					// Rating
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iRating = atoi(token);
					cReadModeA = 0;
					break;
				case 48:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iGuildGUID = atoi(token);
					cReadModeA = 0;
					break;
				case 49:
					// Down Skill Index
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iDownSkillIndex = atoi(token);
					cReadModeA = 0;
					break;
				case 50:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_ItemPosList[cReadModeB - 1].x = atoi(token);
					if (this->m_ItemPosList[cReadModeB - 1].x < -10) this->m_ItemPosList[cReadModeB - 1].x = -10; //KaoZureS adaptado darmart123
					cReadModeB++;
					if (cReadModeB > 50) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;
				case 51:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_ItemPosList[cReadModeB - 1].y = atoi(token);
					if (this->m_ItemPosList[cReadModeB - 1].y < -10) this->m_ItemPosList[cReadModeB - 1].y = -10;
					cReadModeB++;
					if (cReadModeB > 50) {
						cReadModeA = 0;
						cReadModeB = 0;
					}
					break;
				case 52:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_sCharIDnum1 = atoi(token);
					cReadModeA = 0;
					break;
				case 53:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_sCharIDnum2 = atoi(token);
					cReadModeA = 0;
					break;
				case 54:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_sCharIDnum3 = atoi(token);
					cReadModeA = 0;
					break;
				case 55:
					switch (cReadModeB) {
						case 1:
							// Penalty Block Year
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iPenaltyBlockYear = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							// Penalty Block Month
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iPenaltyBlockMonth = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							// Penalty Block day
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iPenaltyBlockDay = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 56:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iQuest = atoi(token);
					cReadModeA = 0;
					break;
				case 57:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iCurQuestCount = atoi(token);
					cReadModeA = 0;
					break;
				case 58:
					cReadModeA = 0;
					break;
				case 59:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iQuestRewardType = atoi(token);
					cReadModeA = 0;
					break;
				case 60:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iQuestRewardAmount = atoi(token);
					cReadModeA = 0;
					break;
				case 61:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iContribution = atoi(token);
					cReadModeA = 0;
					break;
				case 62:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iQuestID = atoi(token);
					cReadModeA = 0;
					break;
				case 63:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_bIsQuestCompleted = (bool) atoi(token);
					cReadModeA = 0;
					break;
				case 64:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iTimeLeft_ForceRecall = (bool) atoi(token);
					cReadModeA = 0;
					break;
				case 65:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iTimeLeft_FirmStaminar = (bool) atoi(token);
					cReadModeA = 0;
					break;
				case 66:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iSpecialEventID = (bool) atoi(token);
					cReadModeA = 0;
					break;
				case 67:
					switch (cReadModeB) {
						case 1:
							// FightZone Number
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iFightzoneNumber = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iReserveTime = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iFightZoneTicketNumber = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 70:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iSuperAttackLeft = (bool) atoi(token);
					cReadModeA = 0;
					break;
				case 71:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iSpecialAbilityTime = atoi(token);
					cReadModeA = 0;
					break;
				case 72:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iWarContribution = atoi(token);
					cReadModeA = 0;
					break;
				case 73:
					if (strlen(token) > 10) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->lockedMap_ = m_pMapList.byName(token);
					cReadModeA = 0;
					break;
				case 74:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iLockedMapTime = atoi(token);
					cReadModeA = 0;
					break;
				case 75:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iCrusadeDuty = atoi(token);
					cReadModeA = 0;
					break;
				case 76:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iConstructionPoint = atoi(token);
					cReadModeA = 0;
					break;
				case 77:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_dwCrusadeGUID = atoi(token);
					cReadModeA = 0;
					break;
				case 78:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iDeadPenaltyTime = atoi(token);
					cReadModeA = 0;
					break;
				case 79: // v2.06 12-4
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iPartyID = atoi(token);
					if (this->m_iPartyID != 0) this->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
					cReadModeA = 0;
					break;
				case 80:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iGizonItemUpgradeLeft = atoi(token);
					cReadModeA = 0;
					break;
				case 81:
					if (_bGetIsStringIsNumber(token) == false) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! (BallPoints) - Connection closed. ", this->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iBallPoints = atoi(token);
					cReadModeA = 0;
					break;
			}
		} else {
			if (memcmp(token, "character-loc-map", 17) == 0) cReadModeA = 1;
			if (memcmp(token, "character-loc-x", 15) == 0) cReadModeA = 2;
			if (memcmp(token, "character-loc-y", 15) == 0) cReadModeA = 3;
			if (memcmp(token, "character-account-status", 21) == 0) cReadModeA = 4;
			if (memcmp(token, "character-item", 14) == 0) {
				cReadModeA = 5;
				cReadModeB = 1;
				this->m_pItemList[iItemIndex].reset(new CItem);
			}
			if (memcmp(token, "character-bank-item", 18) == 0) {
				cReadModeA = 28;
				cReadModeB = 1;
				this->m_pItemInBankList[iItemInBankIndex].reset(new class CItem);
			}
			if (memcmp(token, "sex-status", 10) == 0) cReadModeA = 6;
			if (memcmp(token, "skin-status", 11) == 0) cReadModeA = 7;
			if (memcmp(token, "hairstyle-status", 16) == 0) cReadModeA = 8;
			if (memcmp(token, "haircolor-status", 16) == 0) cReadModeA = 9;
			if (memcmp(token, "underwear-status", 16) == 0) cReadModeA = 10;
			if (memcmp(token, "item-equip-status", 17) == 0) cReadModeA = 11;
			if (memcmp(token, "character-guild-name", 20) == 0) cReadModeA = 12;
			if (memcmp(token, "character-guild-rank", 20) == 0) cReadModeA = 13;
			if (memcmp(token, "character-HP", 12) == 0) cReadModeA = 14;
			if (memcmp(token, "character-DefenseRatio", 22) == 0) cReadModeA = 15;
			if (memcmp(token, "character-HitRatio", 18) == 0) cReadModeA = 16;
			if (memcmp(token, "character-LEVEL", 15) == 0) cReadModeA = 17;
			if (memcmp(token, "character-STR", 13) == 0) cReadModeA = 18;
			if (memcmp(token, "character-INT", 13) == 0) cReadModeA = 19;
			if (memcmp(token, "character-VIT", 13) == 0) cReadModeA = 20;
			if (memcmp(token, "character-DEX", 13) == 0) cReadModeA = 21;
			if (memcmp(token, "character-MAG", 13) == 0) cReadModeA = 22;
			if (memcmp(token, "character-CHARISMA", 18) == 0) cReadModeA = 23;
			if (memcmp(token, "character-LUCK", 14) == 0) cReadModeA = 24;
			if (memcmp(token, "character-EXP", 13) == 0) cReadModeA = 25;
			if (memcmp(token, "magic-mastery", 13) == 0) cReadModeA = 26;
			if (memcmp(token, "skill-mastery", 13) == 0) {
				cReadModeA = 27;
				cReadModeB = 0;
			}
			if (memcmp(token, "character-location", 18) == 0) cReadModeA = 29;
			if (memcmp(token, "character-MP", 12) == 0) cReadModeA = 30;
			if (memcmp(token, "character-SP", 12) == 0) cReadModeA = 31;
			if (memcmp(token, "character-LU_Pool", 17) == 0) cReadModeA = 32;
			/*
			if (memcmp(token, "character-LU_Str", 16) == 0)     cReadModeA = 32;
			if (memcmp(token, "character-LU_Vit", 16) == 0)     cReadModeA = 33;
			if (memcmp(token, "character-LU_Dex", 16) == 0)     cReadModeA = 34;
			if (memcmp(token, "character-LU_Int", 16) == 0)     cReadModeA = 35;
			if (memcmp(token, "character-LU_Mag", 16) == 0)     cReadModeA = 36;
			if (memcmp(token, "character-LU_Char",17) == 0)     cReadModeA = 37;
			 */
			if (memcmp(token, "character-EK-Count", 18) == 0) cReadModeA = 38;
			if (memcmp(token, "character-PK-Count", 18) == 0) cReadModeA = 39;
			if (memcmp(token, "character-reward-gold", 21) == 0) cReadModeA = 40;
			if (memcmp(token, "skill-SSN", 9) == 0) cReadModeA = 41;
			if (memcmp(token, "character-profile", 17) == 0) cReadModeA = 42;
			if (memcmp(token, "hunger-status", 13) == 0) cReadModeA = 43;
			if (memcmp(token, "admin-user-level", 16) == 0) cReadModeA = 44;
			if (memcmp(token, "timeleft-shutup", 15) == 0) cReadModeA = 45;
			if (memcmp(token, "timeleft-rating", 15) == 0) cReadModeA = 46;
			if (memcmp(token, "character-RATING", 16) == 0) cReadModeA = 47;
			if (memcmp(token, "character-guild-GUID", 20) == 0) cReadModeA = 48;
			if (memcmp(token, "character-downskillindex", 24) == 0) cReadModeA = 49;
			if (memcmp(token, "item-position-x", 16) == 0) {
				cReadModeA = 50;
				cReadModeB = 1;
			}
			if (memcmp(token, "item-position-y", 16) == 0) {
				cReadModeA = 51;
				cReadModeB = 1;
			}
			if (memcmp(token, "character-IDnum1", 16) == 0) cReadModeA = 52;
			if (memcmp(token, "character-IDnum2", 16) == 0) cReadModeA = 53;
			if (memcmp(token, "character-IDnum3", 16) == 0) cReadModeA = 54;
			if (memcmp(token, "penalty-block-date", 18) == 0) {
				cReadModeA = 55;
				cReadModeB = 1;
			}
			if (memcmp(token, "character-quest-number", 22) == 0) cReadModeA = 56;
			if (memcmp(token, "current-quest-count", 19) == 0) cReadModeA = 57;
			if (memcmp(token, "quest-reward-type", 17) == 0) cReadModeA = 59;
			if (memcmp(token, "quest-reward-amount", 19) == 0) cReadModeA = 60;
			if (memcmp(token, "character-contribution", 22) == 0) cReadModeA = 61;
			if (memcmp(token, "character-quest-ID", 18) == 0) cReadModeA = 62;
			if (memcmp(token, "character-quest-completed", 25) == 0) cReadModeA = 63;
			if (memcmp(token, "timeleft-force-recall", 21) == 0) cReadModeA = 64;
			if (memcmp(token, "timeleft-firm-staminar", 22) == 0) cReadModeA = 65;
			if (memcmp(token, "special-event-id", 16) == 0) cReadModeA = 66;
			if (memcmp(token, "super-attack-left", 17) == 0) cReadModeA = 70;
			if (memcmp(token, "reserved-fightzone-id", 21) == 0) {
				cReadModeA = 67;
				cReadModeB = 1;
			}
			if (memcmp(token, "special-ability-time", 20) == 0) cReadModeA = 71;
			if (memcmp(token, "character-war-contribution", 26) == 0) cReadModeA = 72;
			if (memcmp(token, "locked-map-name", 15) == 0) cReadModeA = 73;
			if (memcmp(token, "locked-map-time", 15) == 0) cReadModeA = 74;
			if (memcmp(token, "crusade-job", 11) == 0) cReadModeA = 75;
			if (memcmp(token, "construct-point", 15) == 0) cReadModeA = 76;
			if (memcmp(token, "crusade-GUID", 12) == 0) cReadModeA = 77;
			if (memcmp(token, "dead-penalty-time", 17) == 0) cReadModeA = 78;
			if (memcmp(token, "party-id", 8) == 0) cReadModeA = 79; // v2.06 12-4
			if (memcmp(token, "gizon-item-upgade-left", 22) == 0) cReadModeA = 80;
			if (memcmp(token, "character-ball-points", 21) == 0) cReadModeA = 81;
			if (memcmp(token, "[EOF]", 5) == 0) goto DPDC_STOP_DECODING;
		}
		token = pStrTok->pGet();
		//token = strtok( nullptr, seps );
	}
DPDC_STOP_DECODING:
	;
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		wsprintf(cTxt, "(!!!) Player(%s) data file contents error(%d %d)! Connection closed.", this->m_cCharName, cReadModeA, cReadModeB);
		PutLogList(cTxt);
		// Debug code @@@@@@@@@@@@@@@
		HANDLE hFile2;
		DWORD nWrite2;
		hFile2 = CreateFile("Error.Log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		WriteFile(hFile2, (char *) pOriginContents, dwSize + 2, &nWrite2, nullptr);
		CloseHandle(hFile2);
		///////////// @@@@@@@@@@@@@@@
		return false;
	}
	bRet = this->map_->bIsValidLoc(this->m_sX, this->m_sY);
	if (bRet == false) {
		if ((this->m_sX != -1) || (this->m_sY != -1)) {
			wsprintf(cTxt, "Invalid location error! %s (%d, %d)", this->m_cCharName, this->m_sX, this->m_sY);
			PutLogList(cTxt);
			return false;
		}
	}
	/*	if ((this->m_cLU_Str > DEF_TOTALLEVELUPPOINT) || (this->m_cLU_Str < 0))
	return false;
	if ((this->m_cLU_Vit > DEF_TOTALLEVELUPPOINT) || (this->m_cLU_Vit < 0))
	return false;
	if ((this->m_cLU_Dex > DEF_TOTALLEVELUPPOINT) || (this->m_cLU_Dex < 0))
	return false;
	if ((this->m_cLU_Int > DEF_TOTALLEVELUPPOINT) || (this->m_cLU_Int < 0))
	return false;
	if ((this->m_cLU_Mag > DEF_TOTALLEVELUPPOINT) || (this->m_cLU_Mag < 0))
	return false;
	if ((this->m_cLU_Char > DEF_TOTALLEVELUPPOINT) || (this->m_cLU_Char < 0))
	return false;
	 */
	//	if ( (this->m_cLU_Str + this->m_cLU_Vit + this->m_cLU_Dex +
	//  		  this->m_cLU_Int + this->m_cLU_Mag + this->m_cLU_Char) > DEF_TOTALLEVELUPPOINT)
	if ((this->m_iLU_Pool < 0) || (this->m_iLU_Pool > DEF_CHARPOINTLIMIT))
		return false;
	if ((this->m_iStr < 10) || (this->m_iStr > DEF_CHARPOINTLIMIT))
		return false;
	if ((this->m_iDex < 10) || (this->m_iDex > DEF_CHARPOINTLIMIT))
		return false;
	if ((this->m_iVit < 10) || (this->m_iVit > DEF_CHARPOINTLIMIT))
		return false;
	if ((this->m_iInt < 10) || (this->m_iInt > DEF_CHARPOINTLIMIT))
		return false;
	if ((this->m_iMag < 10) || (this->m_iMag > DEF_CHARPOINTLIMIT))
		return false;
	if ((this->m_iCharisma < 10) || (this->m_iCharisma > DEF_CHARPOINTLIMIT))
		return false;
	//	if ((this->m_iStr + this->m_iDex + this->m_iVit +
	//		 this->m_iInt + this->m_iMag + this->m_iCharisma)
	//		 !=	((this->m_iLevel-1)*3 + 70)) return false;
	//if ((this->m_cAccountStatus != 2) && (this->m_iLevel > DEF_LEVELLIMIT))
	//	return false;
	if ((m_Misc.bCheckValidName(this->m_cCharName) == false) || (m_Misc.bCheckValidName(this->m_cAccountName) == false))
		return false;
	if (this->m_iPenaltyBlockYear != 0) {
		GetLocalTime(&SysTime);
		iDateSum1 = (__int64) (this->m_iPenaltyBlockYear * 10000 + this->m_iPenaltyBlockMonth * 100 + this->m_iPenaltyBlockDay);
		iDateSum2 = (__int64) (SysTime.wYear * 10000 + SysTime.wMonth * 100 + SysTime.wDay);
		if (iDateSum1 >= iDateSum2) return false;
	}
	if (this->m_iReserveTime != 0) {
		GetLocalTime(&SysTime);
		iDateSum1 = (__int64) this->m_iReserveTime;
		iDateSum2 = (__int64) (SysTime.wMonth * 10000 + SysTime.wDay * 100 + SysTime.wHour);
		if (iDateSum2 >= iDateSum1) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -2, 0, 0, nullptr);
			this->m_iFightzoneNumber = 0;
			this->m_iReserveTime = 0;
			this->m_iFightZoneTicketNumber = 0;
		}
	}
	if (this->m_iAdminUserLevel < 0) this->m_iAdminUserLevel = 0;
	// ============================================================================================================
	if (this->m_cSex == 1) {
		sTmpType = 1;
	} else if (this->m_cSex == 2) {
		sTmpType = 4;
	}
	switch (this->m_cSkin) {
		case 1:
			break;
		case 2:
			sTmpType += 1;
			break;
		case 3:
			sTmpType += 2;
			break;
	}
	if (this->m_iAdminUserLevel >= 10)
		sTmpType = this->m_iAdminUserLevel;
	sTmpAppr1 = (this->m_cHairStyle << 8) | (this->m_cHairColor << 4) | (this->m_cUnderwear);
	this->m_sType = sTmpType;
	this->m_sAppr1 = sTmpAppr1;
	this->iCalcTotalWeight();
	//if (iTotalGold > 800000) {
	//PutLogFileList(G_cTxt);
	//}
	// v.135
	/*
	if ((this->m_cSkillMastery[0] >= 70) || (this->m_cSkillMastery[1] >= 70) ||
	(this->m_cSkillMastery[12] >= 70)) {
	wsprintf(G_cTxt, "(!) ÇØÅ· ¿ëÀÇÀÚ(%s) ³ôÀº ½ºÅ³ (%d %d %d)", this->m_cCharName,
	this->m_cSkillMastery[0], this->m_cSkillMastery[1],
	this->m_cSkillMastery[12]);
	PutLogFileList(G_cTxt);
	}
	 */
	if (this->m_sCharIDnum1 == 0) {
		int _i;
		int _iTemp1;
		int _iTemp2;
		short _sID1;
		short _sID2;
		short _sID3;
		_iTemp1 = 1;
		_iTemp2 = 1;
		for (_i = 0; _i < 10; _i++) {
			_iTemp1 += this->m_cCharName[_i];
			_iTemp2 += abs(this->m_cCharName[_i] ^ this->m_cCharName[_i]);
		}
		_sID1 = (short) timeGetTime();
		_sID2 = (short) _iTemp1;
		_sID3 = (short) _iTemp2;
		this->m_sCharIDnum1 = _sID1;
		this->m_sCharIDnum2 = _sID2;
		this->m_sCharIDnum3 = _sID3;
	}
	this->m_iRewardGold += iNotUsedItemPrice;
	//
	this->m_iSpeedHackCheckExp = this->m_iExp;
	// v1.41
	if (memcmp(this->m_cLocation, "NONE", 4) == 0) this->m_bIsNeutral = true;
	return true;
}

int CClient::_iComposePlayerDataFileContents(char * pData) {
	SYSTEMTIME SysTime;
	char cTxt[120], cTmp[21];
	POINT TempItemPosList[DEF_MAXITEMS];
	int i;
	int iPos;
	if (this->markedForDeletion_) return 0;
	GetLocalTime(&SysTime);
	strcat(pData, "[FILE-DATE]\n\n");
	wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "[NAME-ACCOUNT]\n\n");
	strcat(pData, "character-name     = ");
	strcat(pData, this->m_cCharName);
	strcat(pData, "\n");
	strcat(pData, "account-name       = ");
	strcat(pData, this->m_cAccountName);
	strcat(pData, "\n\n");
	strcat(pData, "[STATUS]\n\n");
	strcat(pData, "character-profile 	=");
	if (strlen(this->m_cProfile) == 0) {
		strcat(pData, "__________");
	} else strcat(pData, this->m_cProfile);
	strcat(pData, "\n");
	strcat(pData, "character-location   = ");
	strcat(pData, this->m_cLocation);
	strcat(pData, "\n");
	strcat(pData, "character-guild-name = ");
	if (this->m_iGuildRank != -1) {
		strcat(pData, this->m_cGuildName);
	} else strcat(pData, "NONE");
	strcat(pData, "\n");
	strcat(pData, "character-guild-GUID = ");
	if (this->m_iGuildRank != -1) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d", this->m_iGuildGUID);
		strcat(pData, cTxt);
	} else strcat(pData, "-1");
	strcat(pData, "\n");
	strcat(pData, "character-guild-rank = ");
	itoa(this->m_iGuildRank, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "character-loc-map = ");
	strcat(pData, this->map_->m_cName);
	strcat(pData, "\n");
	strcat(pData, "character-loc-x   = ");
	itoa(this->m_sX, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "character-loc-y   = ");
	itoa(this->m_sY, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");
	//
	if (this->m_iHP <= 0)
		this->m_iHP = this->iGetMaxHP();
	wsprintf(cTxt, "character-HP       = %d", this->m_iHP);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-MP       = %d", this->m_iMP);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	if (this->m_iSP < 0) this->m_iSP = 0;
	wsprintf(cTxt, "character-SP       = %d", this->m_iSP);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-LEVEL    = %d", this->m_iLevel);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-RATING   = %d", this->m_iRating);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-STR      = %d", this->m_iStr);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-INT      = %d", this->m_iInt);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-VIT      = %d", this->m_iVit);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-DEX      = %d", this->m_iDex);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-MAG      = %d", this->m_iMag);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-CHARISMA = %d", this->m_iCharisma);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-LUCK     = %d", this->m_iLuck);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-EXP      = %d", this->m_iExp);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-LU_Pool  = %d", this->m_iLU_Pool);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	/*
	wsprintf(cTxt, "character-LU_Str   = %d", this->m_cLU_Str);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "character-LU_Vit   = %d", this->m_cLU_Vit);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "character-LU_Dex   = %d", this->m_cLU_Dex);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "character-LU_Int   = %d", this->m_cLU_Int);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "character-LU_Mag   = %d", this->m_cLU_Mag);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "character-LU_Char  = %d", this->m_cLU_Char);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	 */
	wsprintf(cTxt, "character-EK-Count = %d", this->m_iEnemyKillCount);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-PK-Count = %d", this->m_iPKCount);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-reward-gold = %d", this->m_iRewardGold);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-downskillindex = %d", this->m_iDownSkillIndex);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-IDnum1 = %d", this->m_sCharIDnum1);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-IDnum2 = %d", this->m_sCharIDnum2);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-IDnum3 = %d", this->m_sCharIDnum3);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	/*
	wsprintf(cTxt, "party-rank = %d", this->m_iPartyRank);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "party-IDnum1 = %d", this->m_sPartyIDnum1);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "party-IDnum2 = %d", this->m_sPartyIDnum2);
	strcat(pData, cTxt);
	strcat(pData,"\n");
	wsprintf(cTxt, "party-IDnum3 = %d", this->m_sPartyIDnum3);
	strcat(pData, cTxt);
	strcat(pData,"\n\n");
	 */
	//
	strcat(pData, "sex-status       = ");
	itoa(this->m_cSex, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "skin-status      = ");
	itoa(this->m_cSkin, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "hairstyle-status = ");
	itoa(this->m_cHairStyle, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "haircolor-status = ");
	itoa(this->m_cHairColor, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "underwear-status = ");
	itoa(this->m_cUnderwear, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "hunger-status    = %d", this->m_iHungerStatus);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "timeleft-shutup  = %d", this->m_iTimeLeft_ShutUp);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "timeleft-rating  = %d", this->m_iTimeLeft_Rating);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "timeleft-force-recall  = %d", this->m_iTimeLeft_ForceRecall);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "timeleft-firm-staminar = %d", this->m_iTimeLeft_FirmStaminar);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "admin-user-level = %d", this->m_iAdminUserLevel);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "penalty-block-date = %d %d %d", this->m_iPenaltyBlockYear, this->m_iPenaltyBlockMonth, this->m_iPenaltyBlockDay);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-quest-number = %d", this->m_iQuest);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-quest-ID     = %d", this->m_iQuestID);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "current-quest-count    = %d", this->m_iCurQuestCount);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "quest-reward-type      = %d", this->m_iQuestRewardType);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "quest-reward-amount    = %d", this->m_iQuestRewardAmount);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-contribution = %d", this->m_iContribution);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-war-contribution = %d", this->m_iWarContribution);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-quest-completed = %d", (int) this->m_bIsQuestCompleted);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "special-event-id = %d", (int) this->m_iSpecialEventID);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "super-attack-left = %d", (int) this->m_iSuperAttackLeft);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "reserved-fightzone-id = %d %d %d", this->m_iFightzoneNumber, this->m_iReserveTime, this->m_iFightZoneTicketNumber);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "special-ability-time = %d", this->m_iSpecialAbilityTime);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "locked-map-name = %s", this->lockedMap_ ? this->lockedMap_->m_cName : "NONE");
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "locked-map-time = %d", this->m_iLockedMapTime);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "crusade-job = %d", this->m_iCrusadeDuty);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "crusade-GUID = %d", this->m_dwCrusadeGUID);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "construct-point = %d", this->m_iConstructionPoint);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "dead-penalty-time = %d", this->m_iDeadPenaltyTime);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "party-id = 0", this->m_iPartyID);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "character-ball-points = %d", this->m_iBallPoints);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	wsprintf(cTxt, "gizon-item-upgade-left = %d", this->m_iGizonItemUpgradeLeft);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	/*wsprintf(cTxt, "gizon-item-upgade-left = %d", this->m_iGizonItemUpgradeLeft);
	strcat(pData, cTxt);
	strcat(pData,"\n");*/
	strcat(pData, "\n\n");
	strcat(pData, "appr1 = ");
	itoa(this->m_sAppr1, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "appr2 = ");
	//this->m_sAppr2 = this->m_sAppr2 & 0x0FFF;
	itoa(this->m_sAppr2, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "appr3 = ");
	itoa(this->m_sAppr3, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "appr4 = ");
	itoa(this->m_sAppr4, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n");
	// v1.4 ApprColor
	strcat(pData, "appr-color = ");
	itoa(this->m_iApprColor, cTxt, 10);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");
	strcat(pData, "[ITEMLIST]\n\n");
	for (i = 0; i < DEF_MAXITEMS; i++) { // v1.4
		TempItemPosList[i].x = 40;
		TempItemPosList[i].y = 30;
	}
	iPos = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			TempItemPosList[iPos].x = this->m_ItemPosList[i].x;
			TempItemPosList[iPos].y = this->m_ItemPosList[i].y;
			iPos++;
			strcat(pData, "character-item = ");
			memset(cTmp, ' ', 21);
			strcpy(cTmp, this->m_pItemList[i]->m_cName);
			cTmp[strlen(this->m_pItemList[i]->m_cName)] = (char) ' ';
			cTmp[20] = 0;
			strcat(pData, cTmp);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_dwCount, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sTouchEffectType, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sTouchEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sTouchEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sTouchEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_cItemColor, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sItemSpecEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sItemSpecEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_sItemSpecEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_wCurLifeSpan, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemList[i]->m_dwAttribute, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}
	strcat(pData, "\n\n");
	for (i = 0; i < DEF_MAXITEMS; i++) {
		this->m_ItemPosList[i].x = TempItemPosList[i].x;
		this->m_ItemPosList[i].y = TempItemPosList[i].y;
	}
	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (this->m_pItemInBankList[i] != nullptr) {
			strcat(pData, "character-bank-item = ");
			memset(cTmp, ' ', 21);
			strcpy(cTmp, this->m_pItemInBankList[i]->m_cName);
			cTmp[strlen(this->m_pItemInBankList[i]->m_cName)] = (char) ' ';
			cTmp[20] = 0;
			strcat(pData, cTmp);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_dwCount, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sTouchEffectType, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sTouchEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sTouchEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sTouchEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_cItemColor, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sItemSpecEffectValue1, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sItemSpecEffectValue2, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_sItemSpecEffectValue3, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_wCurLifeSpan, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, " ");
			itoa(this->m_pItemInBankList[i]->m_dwAttribute, cTxt, 10);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}
	strcat(pData, "\n\n");
	strcat(pData, "[MAGIC-SKILL-MASTERY]\n\n");
	strcat(pData, "//------------------012345678901234567890123456789012345678901234567890");
	strcat(pData, "\n");
	strcat(pData, "magic-mastery     = ");
	for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
		wsprintf(cTxt, "%d", (int) this->m_cMagicMastery[i]);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n");
	strcat(pData, "skill-mastery     = ");
	for (i = 0; i < 60; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", this->m_cSkillMastery[i]);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n");
	strcat(pData, "skill-SSN     = ");
	for (i = 0; i < 60; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", this->m_iSkillSSN[i]);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n");
	strcat(pData, "[ITEM-EQUIP-STATUS]\n\n");
	strcat(pData, "item-equip-status = ");
	std::memset(cTxt, 0, sizeof (cTxt));
	strcpy(cTxt, "00000000000000000000000000000000000000000000000000");
	int iEP = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			if ((this->m_bIsItemEquipped[i] == true) &&
					  (this->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_EQUIP)) {
				cTxt[iEP] = '1';
			}
			iEP++;
		}
	strcat(pData, cTxt);
	strcat(pData, "\n");
	strcat(pData, "item-position-x = ");
	for (i = 0; i < DEF_MAXITEMS; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", this->m_ItemPosList[i].x);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n");
	strcat(pData, "item-position-y = ");
	for (i = 0; i < DEF_MAXITEMS; i++) {
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "%d ", this->m_ItemPosList[i].y);
		strcat(pData, cTxt);
	}
	strcat(pData, "\n\n");
	strcat(pData, "[EOF]");
	strcat(pData, "\n\n\n\n");
	return strlen(pData);
}

void CClient::OnClientRead() {
	char * pData, cKey;
	uint32_t dwMsgSize;
	pData = this->m_pXSock->pGetRcvDataPointer(&dwMsgSize, &cKey); // v1.4
	if (bPutMsgQuene(DEF_MSGFROM_CLIENT, pData, dwMsgSize, this->id_, cKey) == false) {
		PutLogList("@@@@@@ CRITICAL ERROR in MsgQuene!!! @@@@@@");
	}
}

void CClient::ClientMotionHandler(char * pData) {
	int iRet;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	this->m_dwLastActionTime = this->m_dwAFKCheckTime = timeGetTime();
	/*this->m_cConnectionCheck++;
	if (this->m_cConnectionCheck > 50) {
		wsprintf(G_cTxt, "Hex: (%s) Player: (%s) - removed 03203203h, vital to hack detection.", this->m_cIPaddress, this->m_cCharName);
		PutHackLogFileList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}*/
	uint16_t *wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	uint16_t wCommand = *wp;
	char *cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	short *sp = (short *) cp;
	short sX = *sp;
	cp += 2;
	sp = (short *) cp;
	short sY = *sp;
	cp += 2;
	char cDir = *cp;
	cp++;
	sp = (short *) cp;
	short dX = *sp;
	cp += 2;
	sp = (short *) cp;
	short dY = *sp;
	cp += 2;
	sp = (short *) cp;
	short wType = *sp;
	cp += 2;
	uint16_t wTargetObjectID = 0;
	if ((wCommand == DEF_OBJECTATTACK) || (wCommand == DEF_OBJECTATTACKMOVE)) {
		wp = (uint16_t *) cp;
		wTargetObjectID = *wp;
		cp += 2;
	}
	uint32_t *dwp = (uint32_t *) cp;
	uint32_t dwClientTime = *dwp;
	cp += 4;
	CheckDenialServiceAttack(client, dwClientTime);
	switch (wCommand) {
		case DEF_OBJECTSTOP:
			iRet = iClientMotion_Stop_Handler(client, sX, sY, cDir);
			if (iRet == 1) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTSTOP, 0, 0, 0);
			} else if (iRet == 2) {
				SendObjectMotionRejectMsg(client);
			}
			break;
		case DEF_OBJECTRUN:
			iRet = iClientMotion_Move_Handler(client, sX, sY, cDir, 1);
			if (iRet == 1) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTRUN, 0, 0, 0);
				if (this->map_->bGetIsTeleport(this->m_sX, this->m_sY)) {
					this->RequestTeleportHandler("3");
				}
			}
			if ((!this->markedForDeletion_) && (this->m_iHP <= 0)) {
				this->ClientKilledHandler(0, 0, 1);
			}
			bCheckClientMoveFrequency(client, dwClientTime);
			break;
		case DEF_OBJECTMOVE:
			iRet = iClientMotion_Move_Handler(client, sX, sY, cDir, 2);
			if (iRet == 1) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
				if (this->map_->bGetIsTeleport(this->m_sX, this->m_sY)) {
					this->RequestTeleportHandler("3");
				}
			}
			if ((!this->markedForDeletion_) && (this->m_iHP <= 0)) {
				this->ClientKilledHandler(0, 0, 1);
			}
			bCheckClientMoveFrequency(client, dwClientTime);
			break;
		case DEF_OBJECTDAMAGEMOVE:
			iRet = iClientMotion_Move_Handler(client, sX, sY, cDir, 0);
			if (iRet == 1) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGEMOVE, this->m_iLastDamage, 0, 0);
				if (this->map_->bGetIsTeleport(this->m_sX, this->m_sY)) {
					this->RequestTeleportHandler("3");
				}
			}
			if ((!this->markedForDeletion_) && (this->m_iHP <= 0)) {
				this->ClientKilledHandler(0, 0, 1);
			}
			break;
		case DEF_OBJECTATTACKMOVE:
			iRet = iClientMotion_Move_Handler(client, sX, sY, cDir, 0);
			if ((iRet == 1) && (!this->markedForDeletion_)) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACKMOVE, 0, 0, 0);
				iClientMotion_Attack_Handler(client, this->m_sX, this->m_sY, dX, dY, wType, cDir, wTargetObjectID, false, true);
				if (this->map_->bGetIsTeleport(this->m_sX, this->m_sY)) {
					this->RequestTeleportHandler("3");
				}
			}
			if ((!this->markedForDeletion_) && (this->m_iHP <= 0)) {
				this->ClientKilledHandler(0, 0, 1);
			}
			bCheckClientAttackFrequency(client, dwClientTime);
			break;
		case DEF_OBJECTATTACK:
			_CheckAttackType(client, &wType);
			iRet = iClientMotion_Attack_Handler(client, sX, sY, dX, dY, wType, cDir, wTargetObjectID);
			if (iRet == 1) {
				if (wType >= 20) {
					this->m_iSuperAttackLeft--;
					if (this->m_iSuperAttackLeft < 0) this->m_iSuperAttackLeft = 0;
				}
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, wType);
			} else if (iRet == 2) SendObjectMotionRejectMsg(client);
			bCheckClientAttackFrequency(client, dwClientTime);
			break;
		case DEF_OBJECTGETITEM:
			iRet = iClientMotion_GetItem_Handler(client, sX, sY, cDir);
			if (iRet == 1) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTGETITEM, 0, 0, 0);
			} else if (iRet == 2) SendObjectMotionRejectMsg(client);
			break;
		case DEF_OBJECTMAGIC:
			iRet = iClientMotion_Magic_Handler(client, sX, sY, cDir);
			//client hp recorded here ONLY if its less than
			if (iRet == 1) {
				if (this->m_bMagicPauseTime == false) {
					this->m_bMagicPauseTime = true;
					this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMAGIC, dX, 10, 0);
					this->m_iSpellCount++;
					bCheckClientMagicFrequency(client, dwClientTime);
				} else if (this->m_bMagicPauseTime == true) {
					wsprintf(G_cTxt, "Cast Delay Hack: (%s) Player: (%s) - player casting too fast.", this->m_cIPaddress, this->m_cCharName);
					PutHackLogFileList(G_cTxt);
					m_pClientList[this->id_]->DeleteClient(true, true);
				}
			} else if (iRet == 2) SendObjectMotionRejectMsg(client);
			break;
		default:
			break;
	}
}

int CClient::iClientMotion_Move_Handler(short sX, short sY, char cDir, char cMoveType) {
	char * cp, cData[3000];
	class CTile * pTile;
	uint32_t * dwp;
	uint16_t * wp, wObjectID;
	short * sp, dX, dY, sTemp, sTemp2, sDOtype, pTopItem;
	int * ip, iRet, iSize, iDamage;
	bool bRet;
	bool bIsBlocked = false;
	if (this->markedForDeletion_) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (this->m_bIsKilled == true) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if ((sX != this->m_sX) || (sY != this->m_sY)) return 2;
	/*dwTime = timeGetTime();
	this->m_dwLastActionTime = dwTime;
	if (cMoveType == 2) {
		if (this->m_iRecentWalkTime > dwTime) {
			this->m_iRecentWalkTime = dwTime;
			if (this->m_sV1 < 1) {
				if (this->m_iRecentWalkTime < dwTime) {
					this->m_sV1++;
				}
				else {
					bIsBlocked = true;
					this->m_sV1 = 0;
				}
			}
		this->m_iRecentWalkTime = dwTime;
		}
		if (bIsBlocked == false) this->m_iMoveMsgRecvCount++;
		if (this->m_iMoveMsgRecvCount >= 3) {
			if (this->m_dwMoveLAT != 0) {
				if ((dwTime - this->m_dwMoveLAT) < (590)) {
					//wsprintf(G_cTxt, "3.51 Walk Speeder: (%s) Player: (%s) walk difference: %d. Speed Hack?", this->m_cIPaddress, this->m_cCharName, dwTime - this->m_dwMoveLAT);
					//PutHackLogFileList(G_cTxt);
					bIsBlocked = true;
				}
			}
			this->m_dwMoveLAT = dwTime;
			this->m_iMoveMsgRecvCount = 0;
		}
	}
	else if (cMoveType == 1) {
		if (this->m_iRecentRunTime > dwTime) {
			this->m_iRecentRunTime = dwTime;
			if (this->m_sV1 < 1) {
				if (this->m_iRecentRunTime < dwTime) {
					this->m_sV1++;
				}
				else {
					bIsBlocked = true;
					this->m_sV1 = 0;
				}
			}
		this->m_iRecentRunTime = dwTime;
		}
		if (bIsBlocked == false) this->m_iRunMsgRecvCount++;
		if (this->m_iRunMsgRecvCount >= 3) {
			if (this->m_dwRunLAT != 0) {
				if ((dwTime - this->m_dwRunLAT) < (290)) {
					//wsprintf(G_cTxt, "3.51 Run Speeder: (%s) Player: (%s) run difference: %d. Speed Hack?", this->m_cIPaddress, this->m_cCharName, dwTime - this->m_dwRunLAT);
					//PutHackLogFileList(G_cTxt);
					bIsBlocked = true;
				}
			}
			this->m_dwRunLAT	= dwTime;
			this->m_iRunMsgRecvCount = 0;
		}
	}*/
	int iStX;
	int iStY;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (this->m_cSide) {
			case 0: this->map_->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: this->map_->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: this->map_->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}
	this->ClearSkillUsingStatus();
	dX = this->m_sX;
	dY = this->m_sY;
	switch (cDir) {
		case 1: dY--;
			break;
		case 2: dX++;
			dY--;
			break;
		case 3: dX++;
			break;
		case 4: dX++;
			dY++;
			break;
		case 5: dY++;
			break;
		case 6: dX--;
			dY++;
			break;
		case 7: dX--;
			break;
		case 8: dX--;
			dY--;
			break;
	}
	pTopItem = 0;
	bRet = this->map_->bGetMoveable(dX, dY, &sDOtype, &pTopItem);
	if (this->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
		bRet = false;
	if ((bRet == true) && (bIsBlocked == false)) {
		if (this->m_iQuest) this->_bCheckIsQuestCompleted();
		this->map_->ClearOwner(1, iClientH, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
		this->m_sX = dX;
		this->m_sY = dY;
		this->m_cDir = cDir;
		this->map_->SetOwner((short) iClientH,
				  DEF_OWNERTYPE_PLAYER,
				  dX, dY);
		if (sDOtype == DEF_DYNAMICOBJECT_SPIKE) {
			if ((this->m_bIsNeutral == true) && ((this->m_sAppr2 & 0xF000) == 0)) {
			} else {
				iDamage = iDice(2, 4);
				if (this->m_iAdminUserLevel == 0)
					this->m_iHP -= iDamage;
			}
		}
		if (this->m_iHP <= 0) this->m_iHP = 0;
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_MOTION;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_OBJECTMOVE_CONFIRM;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		sp = (short *) cp;
		*sp = (short) (dX - 10);
		cp += 2;
		sp = (short *) cp;
		*sp = (short) (dY - 7);
		cp += 2;
		*cp = cDir;
		cp++;
		if (cMoveType == 1) {
			if (this->m_iSP > 0) {
				*cp = 0;
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP--;
					*cp = 1;
				}
			} else {
				*cp = 0;
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP--;
					*cp = 1;
				}
				if (this->m_iSP < -10) {
					this->m_iSP = 0;
					this->DeleteClient(true, true);
					return 0;
				}
			}
		} else *cp = 0;
		cp++;
		pTile = (class CTile *)(this->map_->m_pTile + dX + dY * this->map_->m_sSizeY);
		*cp = (char) pTile->m_iOccupyStatus;
		cp++;
		ip = (int *) cp;
		*ip = this->m_iHP;
		cp += 4;
		iSize = this->iComposeMoveMapData((short) (dX - 10), (short) (dY - 7), cDir, cp);
		iRet = this->m_pXSock->iSendMsg(cData, iSize + 12 + 1 + 4);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				return 0;
		}
		/*if (this->map_->3CA18h == true) {
			.text:00406037                 mov     [ebp+var_C1C], 0
			.text:0040603E                 xor     edx, edx
			.text:00406040                 mov     [ebp+var_C1B], edx
			.text:00406046                 mov     [ebp+var_C17], edx
			.text:0040604C                 mov     [ebp+var_C13], dx
			bRet = this->map_->sub_4C0F20(dX, dY, cTemp, wV1, wV2);
			if (bRet == 1) {
				this->RequestTeleportHandler("2   ", cTemp, wV1, wV2);
			}
		}*/
	} else {
		this->m_bIsMoveBlocked = true;
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_MOTION;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_OBJECTMOVE_REJECT;
		if (bIsBlocked == true) {
			this->m_dwAttackLAT = 1050;
		}
		this->m_dwAttackLAT = 1010;
		wObjectID = (uint16_t) iClientH;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		wp = (uint16_t *) cp;
		*wp = wObjectID;
		cp += 2;
		sp = (short *) cp;
		sX = m_pClientList[wObjectID]->m_sX;
		*sp = sX;
		cp += 2;
		sp = (short *) cp;
		sY = m_pClientList[wObjectID]->m_sY;
		*sp = sY;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sType;
		cp += 2;
		*cp = m_pClientList[wObjectID]->m_cDir;
		cp++;
		memcpy(cp, m_pClientList[wObjectID]->m_cCharName, 10);
		cp += 10;
		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr1;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr2;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr3;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[wObjectID]->m_sAppr4;
		cp += 2;
		ip = (int *) cp;
		*ip = m_pClientList[wObjectID]->m_iApprColor;
		cp += 4;
		ip = (int *) cp;
		sTemp = m_pClientList[wObjectID]->m_iStatus;
		sTemp = 0x0FFFFFFF & sTemp;
		sTemp2 = iGetPlayerABSStatus(wObjectID, iClientH);
		sTemp = (sTemp | (sTemp2 << 28));
		*ip = sTemp;
		cp += 4;
		iRet = this->m_pXSock->iSendMsg(cData, 42);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				return 0;
		}
		return 0;
	}
	//this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	return 1;
}

void CClient::RequestInitPlayerHandler(char * pData, char cKey) {
	register int i;
	char * cp, cCharName[11], cAccountName[11], cAccountPassword[11], cTxt[120];
	bool bIsObserverMode;
	if (this->m_bIsInitComplete == true) return;
	std::memset(cCharName, 0, sizeof (cCharName));
	std::memset(cAccountName, 0, sizeof (cAccountName));
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));
	std::memset(this->m_cCharName, 0, sizeof (this->m_cCharName));
	std::memset(this->m_cAccountName, 0, sizeof (this->m_cAccountName));
	std::memset(this->m_cAccountPassword, 0, sizeof (this->m_cAccountPassword));
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	cp += 10;
	std::memset(cTxt, 0, sizeof (cTxt));
	memcpy(cTxt, cCharName, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cCharName, 0, sizeof (cCharName));
	memcpy(cCharName, cTxt, 10);
	if (strlen(cTxt) == 0) PutLogList("RIPH - cTxt: Char nullptr!");
	memcpy(cAccountName, cp, 10);
	cp += 10;
	std::memset(cTxt, 0, sizeof (cTxt));
	memcpy(cTxt, cAccountName, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cAccountName, 0, sizeof (cAccountName));
	memcpy(cAccountName, cTxt, 10);
	memcpy(cAccountPassword, cp, 10);
	cp += 10;
	std::memset(cTxt, 0, sizeof (cTxt));
	memcpy(cTxt, cAccountPassword, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));
	memcpy(cAccountPassword, cTxt, 10);
	bIsObserverMode = (bool) * cp;
	cp++;
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (this->id_ != i) && (memcmp(m_pClientList[i]->m_cAccountName, cAccountName, 10) == 0)) {
			if (memcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword, 10) == 0) {
				wsprintf(G_cTxt, "<%d> Duplicate account player! Deleted with data save : CharName(%s) AccntName(%s) IP(%s)", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cAccountName, m_pClientList[i]->m_cIPaddress);
				PutLogList(G_cTxt);
				m_pClientList[i]->DeleteClient(true, true, false);
			} else {
				memcpy(this->m_cCharName, cCharName, 10);
				memcpy(this->m_cAccountName, cAccountName, 10);
				memcpy(this->m_cAccountPassword, cAccountPassword, 10);
				m_pClientList[this->id_]->DeleteClient(false, false, false);
				return;
			}
		}
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (this->id_ != i) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)) {
			if (memcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword, 10) == 0) {
				wsprintf(G_cTxt, "<%d> Duplicate player! Deleted with data save : CharName(%s) IP(%s)", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cIPaddress);
				PutLogList(G_cTxt);
				//PutLogFileList(G_cTxt);
				m_pClientList[i]->DeleteClient(true, true, false);
			} else {
				memcpy(this->m_cCharName, cCharName, 10);
				memcpy(this->m_cAccountName, cAccountName, 10);
				memcpy(this->m_cAccountPassword, cAccountPassword, 10);
				m_pClientList[this->id_]->DeleteClient(false, false);
				return;
			}
		}
	}
	memcpy(this->m_cCharName, cCharName, 10);
	memcpy(this->m_cAccountName, cAccountName, 10);
	memcpy(this->m_cAccountPassword, cAccountPassword, 10);
	this->m_bIsObserverMode = bIsObserverMode;
	bSendMsgToLS(MSGID_REQUEST_PLAYERDATA, this->id_);
}

void CClient::RequestInitDataHandler(char * pData, char cKey) {
	bool bFlag;
	char * pBuffer = nullptr;
	short * sp;
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cPlayerName[11], cTxt[120];
	int sSummonPoints;
	int * ip, i, iTotalItemA, iTotalItemB, iSize, iRet, iStats;
	SYSTEMTIME SysTime;
	if (this->markedForDeletion_) return;
	pBuffer = new char [DEF_MSGBUFFERSIZE + 1];
	ZeroMemory(pBuffer, DEF_MSGBUFFERSIZE + 1);
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cPlayerName, 0, sizeof (cPlayerName));
	memcpy(cPlayerName, cp, 10);
	std::memset(cTxt, 0, sizeof (cTxt)); // v1.4
	memcpy(cTxt, cPlayerName, 10);
	m_Misc.bDecode(cKey, cTxt);
	std::memset(cPlayerName, 0, sizeof (cPlayerName));
	memcpy(cPlayerName, cTxt, 10);
	if (memcmp(this->m_cCharName, cPlayerName, 10) != 0) {
		this->DeleteClient(false, true);
		return;
	}
	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_PLAYERCHARACTERCONTENTS; // 0x0Fm_cHeldenianVictoryType000 = 262406144
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);
	ip = (int *) cp;
	*ip = this->m_iHP;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iMP;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iSP;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iDefenseRatio;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iHitRatio;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iLevel;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iStr;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iInt;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iVit;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iDex;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iMag;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iCharisma;
	cp += 4;
	iStats = (this->m_iStr + this->m_iDex + this->m_iVit +
			  this->m_iInt + this->m_iMag + this->m_iCharisma);
	this->m_iLU_Pool = this->m_iLevel * 3 - (iStats - 70);
	wp = (uint16_t *) cp;
	//*wp = this->m_iLevel*3 - (iStats - 70);
	*wp = this->m_iLU_Pool;
	cp += 2;
	*cp = this->m_cVar;
	cp++;
	*cp = 0;
	cp++;
	*cp = 0;
	cp++;
	*cp = 0;
	cp++;
	*cp = 0;
	cp++;
	ip = (int *) cp;
	*ip = this->m_iExp;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iEnemyKillCount;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iPKCount;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iRewardGold;
	cp += 4;
	memcpy(cp, this->m_cLocation, 10);
	cp += 10;
	memcpy(cp, this->m_cGuildName, 20);
	cp += 20;
	ip = (int *) cp;
	*ip = this->m_iGuildRank;
	cp += 4;
	// v1.4311
	*cp = (char) this->m_iSuperAttackLeft;
	cp++;
	ip = (int *) cp;
	*ip = this->m_iFightzoneNumber;
	cp += 4;
	//Syntax : ======HP==MP==SP==DRatHRatLVL=STR=INT=VIT=DEX=MAG=CHR=LUstatEXP=EK==PK==RewaLocation==GuildName=RankAF
	//Syntax : 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
	//Syntax : ......145212521152........376.200=200=200=200=200=195=......big.8...17......aresden...NONE......NONE30
	// 0x0Fm_cHeldenianVictoryType000 = 262406144
	//Debug Event
	//DbgWnd->AddEventMsg(MSG_SEND,pBuffer,180,0);
	iRet = this->m_pXSock->iSendMsg(pBuffer, 118); // Original : 115
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}
	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_PLAYERITEMLISTCONTENTS;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	bFlag = false; // Kaozures - Adaptado darmart123.
	while (bFlag == false) {
		bFlag = true;
		for (i = 0; i < DEF_MAXITEMS - 1; i++)
			if ((this->m_pItemList[i] == nullptr) && (this->m_pItemList[i + 1] != nullptr)) {
				std::swap(this->m_pItemList[i], this->m_pItemList[i + 1]);
				this->m_ItemPosList[i].x = this->m_ItemPosList[i + 1].x;
				this->m_ItemPosList[i].y = this->m_ItemPosList[i + 1].y;
				bFlag = false;
			}
	}
	iTotalItemA = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr)
			iTotalItemA++;
	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);
	*cp = iTotalItemA;
	cp++;
	for (i = 0; i < iTotalItemA; i++) {
		// ### ERROR POINT!!!
		if (this->m_pItemList[i] == nullptr) {
			wsprintf(G_cTxt, "RequestInitDataHandler error: Client(%s) Item(%d)", this->m_cCharName, i);
			PutLogFileList(G_cTxt);
			this->DeleteClient(false, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
		}
		memcpy(cp, this->m_pItemList[i]->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = this->m_pItemList[i]->m_dwCount;
		cp += 4;
		*cp = this->m_pItemList[i]->m_cItemType;
		cp++;
		*cp = this->m_pItemList[i]->m_cEquipPos;
		cp++;
		*cp = (char) this->m_bIsItemEquipped[i];
		cp++;
		sp = (short *) cp;
		*sp = this->m_pItemList[i]->m_sLevelLimit;
		cp += 2;
		*cp = this->m_pItemList[i]->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = this->m_pItemList[i]->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = this->m_pItemList[i]->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = this->m_pItemList[i]->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = this->m_pItemList[i]->m_sSpriteFrame;
		cp += 2;
		*cp = this->m_pItemList[i]->m_cItemColor; // v1.4
		cp++;
		*cp = (char) this->m_pItemList[i]->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = this->m_pItemList[i]->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(this->m_pItemList[i]->m_dwAttribute & 0x00000001);
		cp++;
		 */
	}
	iTotalItemB = 0;
	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (this->m_pItemInBankList[i] != nullptr)
			iTotalItemB++;
	*cp = iTotalItemB;
	cp++;
	for (i = 0; i < iTotalItemB; i++) {
		if (this->m_pItemInBankList[i] == nullptr) {
			wsprintf(G_cTxt, "RequestInitDataHandler error: Client(%s) Bank-Item(%d)", this->m_cCharName, i);
			PutLogFileList(G_cTxt);
			this->DeleteClient(false, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
		}
		memcpy(cp, this->m_pItemInBankList[i]->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = this->m_pItemInBankList[i]->m_dwCount;
		cp += 4;
		*cp = this->m_pItemInBankList[i]->m_cItemType;
		cp++;
		*cp = this->m_pItemInBankList[i]->m_cEquipPos;
		cp++;
		sp = (short *) cp;
		*sp = this->m_pItemInBankList[i]->m_sLevelLimit;
		cp += 2;
		*cp = this->m_pItemInBankList[i]->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = this->m_pItemInBankList[i]->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = this->m_pItemInBankList[i]->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = this->m_pItemInBankList[i]->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = this->m_pItemInBankList[i]->m_sSpriteFrame;
		cp += 2;
		*cp = this->m_pItemInBankList[i]->m_cItemColor; // v1.4
		cp++;
		*cp = (char) this->m_pItemInBankList[i]->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = this->m_pItemInBankList[i]->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(this->m_pItemInBankList[i]->m_dwAttribute & 0x00000001);
		cp++;
		 */
	}
	for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
		*cp = this->m_cMagicMastery[i];
		cp++;
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		*cp = this->m_cSkillMastery[i];
		cp++;
	}
	iRet = this->m_pXSock->iSendMsg(pBuffer, 6 + 1 + iTotalItemA * 44 + iTotalItemB * 43 + DEF_MAXMAGICTYPE + DEF_MAXSKILLTYPE);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}
	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITDATA;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);
	if (this->m_bIsObserverMode == false)
		this->map_->bGetEmptyPosition(&this->m_sX, &this->m_sY);
	else this->map_->GetMapInitialPoint(&this->m_sX, &this->m_sY);
	// ObjectID
	wp = (uint16_t *) cp;
	*wp = iClientH;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sX - 14 - 5;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sY - 12 - 5;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sType;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr1;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr2;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr3;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sAppr4;
	cp += 2;
	ip = (int *) cp; // v1.4
	*ip = this->m_iApprColor;
	cp += 4;
	ip = (int *) cp;
	*ip = this->m_iStatus;
	cp += 4; // Original : 2
	memcpy(cp, this->map_->m_cName, 10);
	cp += 10;
	memcpy(cp, this->map_->m_cLocationName, 10);
	cp += 10;
	if (this->map_->m_bIsFixedDayMode == true)
		*cp = 1;
	else *cp = m_cDayOrNight;
	cp++;
	if (this->map_->m_bIsFixedDayMode == true)
		*cp = 0;
	else *cp = this->map_->m_cWhetherStatus;
	cp++;
	// v1.4 Contribution
	ip = (int *) cp;
	*ip = this->m_iContribution;
	cp += 4;
	if (this->m_bIsObserverMode == false) {
		this->map_->this->SetOwner(DEF_OWNERTYPE_PLAYER,
				  this->m_sX,
				  this->m_sY);
	}
	// v1.41
	*cp = (char) this->m_bIsObserverMode;
	cp++;
	// catches debug on player load up
	// v1.41
	ip = (int *) cp;
	*ip = this->m_iRating;
	cp += 4;
	// v1.44
	ip = (int *) cp;
	*ip = this->m_iHP;
	cp += 4;
	//Unknown variable
	*cp = 0;
	cp++;
	iSize = this->iComposeInitMapData(this->m_sX - 10, this->m_sY - 7, cp);
	iRet = this->m_pXSock->iSendMsg(pBuffer, 46 + iSize + 4 + 4 + 1 + 4 + 4 + 3); // Zabuza fix
	//iRet = this->m_pXSock->iSendMsg(pBuffer, 59 + iSize +4 +4 +1 +4 +4); // v1.41
	//	iRet = this->m_pXSock->iSendMsg(pBuffer, 46 + iSize +4 +4 +1 +4 +4); // v1.41
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			if (pBuffer != nullptr) delete pBuffer;
			return;
	}
	if (pBuffer != nullptr) delete pBuffer;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);

	if ((memcmp(this->m_cLocation, "are", 3) == 0) &&
			  (memcmp(this->map_->m_cLocationName, "elvine", 6) == 0)
			  && (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;

		this->SetForceRecallTime();
	} else if ((memcmp(this->m_cLocation, "elv", 3) == 0) &&
			  (memcmp(this->map_->m_cLocationName, "aresden", 7) == 0)
			  && (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;

		this->SetForceRecallTime();
	} else if (((memcmp(this->map_->m_cLocationName, "arejail", 7) == 0) ||
			  (memcmp(this->map_->m_cLocationName, "elvjail", 7) == 0))
			  && (this->m_iAdminUserLevel == 0)) {
		this->m_bIsWarLocation = true;
		this->m_dwWarBeginTime = timeGetTime();
		// v2.17 2002-7-15
		if (this->m_iTimeLeft_ForceRecall == 0) {
			this->m_iTimeLeft_ForceRecall = 20 * 5;

		} else if (this->m_iTimeLeft_ForceRecall > 20 * 5) {
			this->m_iTimeLeft_ForceRecall = 20 * 5;
		}
	} else if ((this->map_->m_bIsFightZone == true) &&
			  (m_iFightzoneNoForceRecall == 0) && (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;


		GetLocalTime(&SysTime);
		this->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
	} else {
		this->m_bIsWarLocation = false;
		// v1.42
		this->m_iTimeLeft_ForceRecall = 0;
		// 06/11/2004
		this->SetForceRecallTime();
	}

	if ((this->m_iTimeLeft_ForceRecall > 0) && (this->m_bIsWarLocation == true)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_FORCERECALLTIME, this->m_iTimeLeft_ForceRecall, 0, 0, nullptr);
		wsprintf(G_cTxt, "(!) Game Server Force Recall Time  %d (%d)min", this->m_iTimeLeft_ForceRecall, this->m_iTimeLeft_ForceRecall / 20);
		PutLogList(G_cTxt);
	}
	// No entering enemy shops
	int iMapside;
	int iMapside2;
	iMapside = iGetMapLocationSide(this->map_->m_cName);
	if (iMapside > 3) iMapside2 = iMapside - 2;
	else iMapside2 = iMapside;
	this->m_bIsInsideOwnTown = false;
	if ((this->m_cSide != iMapside2) && (iMapside != 0)) {
		if ((iMapside <= 2) && (this->m_iAdminUserLevel < 1)) {
			if (this->m_cSide != 0) {
				this->m_dwWarBeginTime = timeGetTime();
				this->m_bIsWarLocation = true;
				this->m_iTimeLeft_ForceRecall = 1;
				this->m_bIsInsideOwnTown = true;
			}
		}
	} else {
		if (this->map_->m_bIsFightZone == true &&
				  m_iFightzoneNoForceRecall == false &&
				  this->m_iAdminUserLevel == 0) {
			this->m_dwWarBeginTime = timeGetTime();
			this->m_bIsWarLocation = true;
			GetLocalTime(&SysTime);
			this->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
		} else {
			if (memcmp(this->map_->m_cLocationName, "arejail", 7) == 0 ||
					  memcmp(this->map_->m_cLocationName, "elvjail", 7) == 0) {
				if (this->m_iAdminUserLevel == 0) {
					this->m_bIsWarLocation = true;
					this->m_dwWarBeginTime = timeGetTime();
					if (this->m_iTimeLeft_ForceRecall == 0)
						this->m_iTimeLeft_ForceRecall = 100;
					else if (this->m_iTimeLeft_ForceRecall > 100)
						this->m_iTimeLeft_ForceRecall = 100;
				}
			}
		}
	}
	/*if ((this->m_iTimeLeft_ForceRecall > 0) &&
		(this->m_bIsWarLocation == true)) {
		SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_FORCERECALLTIME, this->m_iTimeLeft_ForceRecall, nullptr, nullptr, nullptr);
	}*/
	if (this->m_iGizonItemUpgradeLeft < 0) {
		this->m_iGizonItemUpgradeLeft = 0;
	}
	this->SendNotifyMsg(0, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, nullptr);
	this->SendNotifyMsg(0, DEF_NOTIFY_DOWNSKILLINDEXSET, this->m_iDownSkillIndex, 0, 0, nullptr);
	this->SendNotifyMsg(0, DEF_NOTIFY_ITEMPOSLIST, 0, 0, 0, nullptr);
	this->_SendQuestContents();
	this->_CheckQuestEnvironment();
	// v1.432
	if (this->m_iSpecialAbilityTime == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, nullptr);
	}
	if (m_bIsCrusadeMode == true) {
		if (this->m_dwCrusadeGUID == 0) {
			this->m_iCrusadeDuty = 0;
			this->m_iConstructionPoint = 0;
			this->m_dwCrusadeGUID = m_dwCrusadeGUID;
		} else if (this->m_dwCrusadeGUID != m_dwCrusadeGUID) {
			this->m_iCrusadeDuty = 0;
			this->m_iConstructionPoint = 0;
			this->m_iWarContribution = 0;
			this->m_dwCrusadeGUID = m_dwCrusadeGUID;
			this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
		}
		this->m_cVar = 1;
		this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, this->m_iCrusadeDuty, 0, nullptr);
	} else if (m_bIsHeldenianMode == true) {
		sSummonPoints = this->m_iCharisma * 300;
		if (sSummonPoints > 12000) sSummonPoints = 12000;
		if (this->m_dwHeldenianGUID == 0) {
			this->m_dwCrusadeGUID = m_dwHeldenianGUID;
			this->m_iConstructionPoint = sSummonPoints;
		} else if (this->m_dwCrusadeGUID != m_dwHeldenianGUID) {
			this->m_iConstructionPoint = sSummonPoints;
			this->m_iWarContribution = 0;
			this->m_dwCrusadeGUID = m_dwHeldenianGUID;
		}
		this->m_cWarType = 2;
		if (m_bIsHeldenianMode == true) {
			this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
			if (m_bHeldenianInitiated == false) {
				this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, this->m_iConstructionPoint, this->m_iWarContribution, 0, nullptr);
			UpdateHeldenianStatus();
		}
	} else if ((this->m_cVar == 1) && (this->m_dwCrusadeGUID == m_dwCrusadeGUID)) {
		this->m_iCrusadeDuty = 0;
		this->m_iConstructionPoint = 0;
	} else {
		if (this->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (this->m_cVar == 1) {
				this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
			}
		} else {
			this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
			this->m_dwCrusadeGUID = 0;
			this->m_iWarContribution = 0;
			this->m_dwCrusadeGUID = 0;
		}
	}
	// v1.42
	if (memcmp(this->map_->m_cName, "fightzone", 9) == 0) {
		wsprintf(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", this->m_cCharName, this->map_->m_cName, this->m_bIsObserverMode);
		PutLogEventFileList(G_cTxt);
	}
	if (m_bIsHeldenianMode == true) this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr, 0);
	if (m_bHeldenianInitiated == true) this->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr, 0);
	// Crusade
	this->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, this->m_iConstructionPoint, this->m_iWarContribution, 1, nullptr);
	this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
	//Critical Count Fix.
	this->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
}

int CClient::iClientMotion_Attack_Handler(short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse, bool bIsDash) {
	char cData[100];
	uint32_t * dwp, dwTime;
	uint16_t * wp;
	int iRet;
	int iExp;
	int tdX = 0, tdY = 0, i;
	short sOwner;
	short sAbsX;
	short sAbsY;
	char cOwnerType;
	bool bNearAttack = false, var_AC = false;
	short sItemIndex;
	int tX;
	int tY;
	int iErr;
	int iStX;
	int iStY;
	if (this->markedForDeletion_) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if (this->m_bIsKilled == true) return 0;
	dwTime = timeGetTime();
	this->m_dwLastActionTime = dwTime;
	this->m_iAttackMsgRecvCount++;
	if (this->m_iAttackMsgRecvCount >= 7) {
		if (this->m_dwAttackLAT != 0) {
			if ((dwTime - this->m_dwAttackLAT) < (3500)) {
				this->DeleteClient(true, true, true);
				return 0;
			}
		}
		this->m_dwAttackLAT = dwTime;
		this->m_iAttackMsgRecvCount = 0;
	}
	if ((wTargetObjectID != 0) && (wType != 2)) {
		if (wTargetObjectID < DEF_MAXCLIENTS) {
			if (m_pClientList[wTargetObjectID] != nullptr) {
				tdX = m_pClientList[wTargetObjectID]->m_sX;
				tdY = m_pClientList[wTargetObjectID]->m_sY;
			}
		} else if ((wTargetObjectID > 10000) && (wTargetObjectID < (10000 + DEF_MAXNPCS))) {
			if (m_pNpcList[wTargetObjectID - 10000] != nullptr) {
				tdX = m_pNpcList[wTargetObjectID - 10000]->m_sX;
				tdY = m_pNpcList[wTargetObjectID - 10000]->m_sY;
			}
		}
		this->map_->GetOwner(&sOwner, &cOwnerType, dX, dY);
		if (sOwner == (wTargetObjectID - 10000)) {
			tdX = m_pNpcList[sOwner]->m_sX;
			dX = tdX;
			tdY = m_pNpcList[sOwner]->m_sY;
			dY = tdY;
			bNearAttack = false;
			var_AC = true;
		}
		if (var_AC != true) {
			if ((tdX == dX) && (tdY == dY)) {
				bNearAttack = false;
			} else if ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1)) {
				dX = tdX;
				dY = tdY;
				bNearAttack = true;
			}
		}
	}
	if ((dX < 0) || (dX >= this->map_->m_sSizeX) ||
			  (dY < 0) || (dY >= this->map_->m_sSizeY)) return 0;
	if ((sX != this->m_sX) || (sY != this->m_sY)) return 2;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (this->m_cSide) {
			case 0: this->map_->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: this->map_->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: this->map_->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}
	sAbsX = abs(sX - dX);
	sAbsY = abs(sY - dY);
	if ((wType != 2) && (wType < 20)) {
		if (var_AC == false) {
			sItemIndex = this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
			if (sItemIndex != -1) {
				if (this->m_pItemList[sItemIndex] == nullptr) return 0;
				if (this->m_pItemList[sItemIndex]->m_sIDnum == 845) {
					if ((sAbsX > 4) || (sAbsY > 4)) wType = 0;
				} else {
					if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
				}
			} else {
				if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
			}
		} else if (var_AC != false) {
			sItemIndex = this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ];
			if (sItemIndex != -1) {
				if (this->m_pItemList[sItemIndex] == nullptr) return 0;
				if (this->m_pItemList[sItemIndex]->m_sIDnum == 845) {
					cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
					if ((this->map_->bCheckFlySpaceAvailable(sX, sY, cDir, sOwner)) != false) {
						wType = 0;
					}
				} else {
					if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
				}
			} else {
				if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
			}
		} else {
			if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
		}
	}
	this->ClearSkillUsingStatus();
	this->map_->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	this->map_->this->SetOwner(DEF_OWNERTYPE_PLAYER, sX, sY);
	this->m_cDir = cDir;
	iExp = 0;
	this->map_->GetOwner(&sOwner, &cOwnerType, dX, dY);
	if (sOwner != 0) {
		if ((wType != 0) && ((dwTime - this->m_dwRecentAttackTime) > 100)) {
			if ((this->m_pIsProcessingAllowed == false) && (this->m_bIsInsideOwnTown == false)) {
				sItemIndex = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
				if (sItemIndex != -1 && this->m_pItemList[sItemIndex] != nullptr) {
					if (this->m_pItemList[sItemIndex]->m_sIDnum == 874) { // Directional bow
						iErr = 0;
						for (i = 1; i < 2; i++) {
							m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
							this->map_->GetOwner(&sOwner, &cOwnerType, tX, tY);
							iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, tX, tY, wType, bNearAttack, bIsDash, true); // 1
							if (((abs(tdX - dX) >= 1) && (abs(tdY - dY) >= 1)) || ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1))) {
								this->map_->GetOwner(&sOwner, &cOwnerType, dX, dY);
								iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
								while (((tdX - dX) >= 1) && ((tdY - dY) >= 1)) {
									dX += 4;
									dY += 4;
									tY += 4;
									tX += 4;
								}
							}
						}
					} else if (this->m_pItemList[sItemIndex]->m_sIDnum == 873) { // Firebow
						if ((strcmp(this->map_->m_cName, "aresden") == 0) ||
								  (strcmp(this->map_->m_cName, "elvine") == 0) ||
								  (strcmp(this->map_->m_cName, "arefarm") == 0) ||
								  (strcmp(this->map_->m_cName, "elvfarm") == 0)) {
							iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false);
						} else {
							if ((this->m_sAppr2 & 0xF000) != 0) {
								if (m_bHeldenianInitiated != 1) {
									dynamicObjects_.this->iAddDynamicObjectList(DEF_OWNERTYPE_PLAYER_INDIRECT, DEF_DYNAMICOBJECT_FIRE3, this->map_, dX, dY, (iDice(1, 7) + 3)*1000, 8);
								}
								iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
							}
						}
					} else {
						iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
					}
				} else {
					iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
				}
			} else {
				iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
			}
			if (this->markedForDeletion_) return 0;
			this->m_dwRecentAttackTime = dwTime;
		}
	} else this->_CheckMiningAction(dX, dY);
	if (iExp != 0) {
		this->GetExp(iExp, true);
	}
	if (bResponse == true) {
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_MOTION;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_OBJECTMOTION_ATTACK_CONFIRM;
		iRet = this->m_pXSock->iSendMsg(cData, 6);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				return 0;
		}
	}
	return 1;
}

void CClient::ChatMsgHandler(char * pData, uint32_t dwMsgSize) {
	register int i, iRet;
	uint16_t * wp;
	char * cp;
	char cBuffer[256], cTemp[256], cSendMode = 0;
	= this->id_;
	this->m_dwAFKCheckTime = timeGetTime();
	if (this->m_bIsInitComplete == false) return;
	if (dwMsgSize > 83 + 30) return;
	{
	}
	if (this->m_iTimeLeft_ShutUp > 0) return;
	if (memcmp((pData + 10), this->m_cCharName, strlen(this->m_cCharName)) != 0) return;
	if ((this->m_bIsObserverMode == true) && (this->m_iAdminUserLevel == 0)) return;
	int iStX;
	int iStY;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (this->m_cSide) {
			case 0: this->map_->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: this->map_->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: this->map_->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}
	cp = (char *) (pData + 21);
	switch (m_iLogChatOption) {
			// Chat Logs of only players
		case 1:
			if (this->m_iAdminUserLevel == 0) {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) PC(%s):\"%s\"", this->map_->m_cName, this->m_cIPaddress, this->m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			}
			break;
			// Chat Logs of only GM
		case 2:
			if (this->m_iAdminUserLevel > 0) {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) GM(%s):\"%s\"", this->map_->m_cName, this->m_cIPaddress, this->m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			}
			break;
			// Chat logs of all
		case 3:
			if (this->m_iAdminUserLevel > 0) {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) GM(%s):\"%s\"", this->map_->m_cName, this->m_cIPaddress, this->m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			} else {
				std::memset(cTemp, 0, sizeof (cTemp));
				pData[dwMsgSize - 1] = 0;
				wsprintf(cTemp, "Loc(%s) IP(%s) PC(%s):\"%s\"", this->map_->m_cName, this->m_cIPaddress, this->m_cCharName, cp);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
			}
			break;
			// Chat logs of none
		case 4:
			break;
	}
	/*	// New 08/05/2004
	// Log GM Chats
	if (this->m_iAdminUserLevel > 0){
		std::memset(cTemp, 0, sizeof(cTemp));
		pData[dwMsgSize-1] = 0;
		wsprintf(cTemp,"GM Chat(%s):\"%s\"",this->m_cCharName,cp);
		bSendMsgToLS(MSGID_GAMEMASTERLOG,iClientH, false,cTemp);
	}*/
	switch (*cp) {
		case '@':
			*cp = 32;
			if ((this->m_iTimeLeft_ShutUp == 0) && (this->m_iLevel > 1) &&
					  (this->m_iSP >= 3)) {
				//v1.42
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP -= 3;
					this->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 1;
			} else cSendMode = 0;
			if (this->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
			break;
			// New 08/05/2004
			// Party chat
		case '$':
			*cp = 32;
			if ((this->m_iTimeLeft_ShutUp == 0) && (this->m_iSP >= 3)) {
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP -= 3;
					this->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 4;
			} else {
				cSendMode = 0;
			}
			if (this->m_iTimeLeft_ShutUp > 0) {
				cSendMode = 0;
			}
			break;
		case '^':
			*cp = 32;
			if ((this->m_iTimeLeft_ShutUp == 0) && (this->m_iLevel > 10) &&
					  (this->m_iSP > 5) && this->m_iGuildRank != -1) {
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP -= 3;
					this->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 1;
			} else cSendMode = 0;
			if (this->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
			if (this->m_iHP < 0) cSendMode = 0;
			if (this->m_iAdminUserLevel > 0) cSendMode = 10;
			break;
		case '!':
			*cp = 32;
			if ((this->m_iTimeLeft_ShutUp == 0) && (this->m_iLevel > 10) &&
					  (this->m_iSP >= 5)) {
				//v1.42
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP -= 5;
					this->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 2;
			} else cSendMode = 0;
			if (this->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
			if (this->m_iHP <= 0) cSendMode = 0;
			if (this->m_iAdminUserLevel > 0) cSendMode = 35; // GM Chat By Carlos
			break;
		case '~':
			*cp = 32;
			if ((this->m_iTimeLeft_ShutUp == 0) && (this->m_iLevel > 1) &&
					  (this->m_iSP >= 3)) {
				//v1.42
				if (this->m_iTimeLeft_FirmStaminar == 0) {
					this->m_iSP -= 3;
					this->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
				cSendMode = 3;
			} else cSendMode = 0;
			if (this->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
			if (this->m_iHP <= 0) cSendMode = 0;
			break;
		case '/':
			std::memset(cBuffer, 0, sizeof (cBuffer));
			memcpy(cBuffer, cp, dwMsgSize - 21);
			cp = (char *) (cBuffer);
			// New 19/05/2004
			if (memcmp(cp, "/version", 8) == 0) {
				this->ShowVersion();
				return;
			}
			// New 10/05/2004
			if (memcmp(cp, "/begincrusadetotalwar", 21) == 0) {
				if (this->m_iAdminUserLevel > 3) {
					GlobalStartCrusadeMode();
					wsprintf(cTemp, "(%s) GM Order(%s): begincrusadetotalwar", this->m_cIPaddress, this->m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}
			if (memcmp(cp, "/endcrusadetotalwar", 19) == 0) {
				if (this->m_iAdminUserLevel > 3) {
					ManualEndCrusadeMode(0);
					wsprintf(cTemp, "(%s) GM Order(%s): endcrusadetotalwar", this->m_cIPaddress, this->m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}
			// New 08/05/2004
			if (memcmp(cp, "/goto ", 6) == 0) {
				this->AdminOrder_GoTo(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/unsummonboss", 13) == 0) {
				this->AdminOrder_UnsummonBoss();
				return;
			}
			//Showdmg
			if (memcmp(cp, "/showdmg", 8) == 0) {
				this->PlayerOrder_ShowDamage();
				return;
			}
			if (memcmp(cp, "/clearnpc", 9) == 0) {
				this->AdminOrder_ClearNpc();
				return;
			}
			if (memcmp(cp, "/clearmap", 9) == 0) {
				this->AdminOrder_CleanMap(cp, dwMsgSize);
				return;
			}
			if (memcmp(cp, "/setforcerecalltime ", 20) == 0) {
				this->AdminOrder_SetForceRecallTime(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/enableadmincommand ", 20) == 0) {
				AdminOrder_EnableAdminCommand(client, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/monstercount", 13) == 0) {
				this->AdminOrder_MonsterCount(cp, dwMsgSize - 21);
				return;
			}
			// New 07/05/2004
			if (memcmp(cp, "/createparty", 12) == 0) {
				this->RequestCreatePartyHandler();
				return;
			}
			if (memcmp(cp, "/joinparty ", 11) == 0) {
				this->RequestJoinPartyHandler(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/dismissparty", 13) == 0) {
				this->RequestDismissPartyHandler();
				return;
			}
			if (memcmp(cp, "/getpartyinfo", 13) == 0) {
				this->GetPartyInfoHandler();
				return;
			}
			if (memcmp(cp, "/deleteparty", 12) == 0) {
				this->RequestDeletePartyHandler();
				return;
			}
			if (memcmp(cp, "/who", 4) == 0) {
				if (this->m_iAdminUserLevel >= m_iAdminLevelWho) {
					this->SendNotifyMsg(0, DEF_NOTIFY_TOTALUSERS, 0, 0, 0, nullptr);
				}
				return;
			}
			//50Cent - Capture The Flag
			if (memcmp(cp, "/startctf", 9) == 0) {
				if (this->m_iAdminUserLevel >= 4) StartCaptureTheFlag();
				return;
			}
			if (memcmp(cp, "/endctf", 7) == 0) {
				if (this->m_iAdminUserLevel >= 4) EndCaptureTheFlag(0);
				return;
			}
			if (memcmp(cp, "/happyhour ", 11) == 0) { // MORLA 2.3 - Comando Happy Hour
				if (this->m_iAdminUserLevel == 0) return;
				this->ManualStartHappyHour(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/porpatas", 3) == 0) {
				if (this->m_iAdminUserLevel > 0)
					this->DeleteClient(true, true);
				return;
			}
			if (memcmp(cp, "/desbug ", 8) == 0) {
				this->DesbugCharacter(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/dkset", 6) == 0) {
				this->GetDkSet();
				return;
			}
			//criticals by MihD
			if (memcmp(cp, "/criticals", 10) == 0) {
				PlayerOrder_GetCrits(client);
				return;
			}
			if (memcmp(cp, "/addmaster ", 11) == 0) {
				this->PlayerCommandAddMaster(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/deletemaster ", 13) == 0) {
				this->PlayerOrder_DeleteGuildMaster(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/balltrade ", 7) == 0) {
				this->_TradeItem(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/ballpoints", 10) == 0) {
				char cInfoString[50];
				wsprintf(cInfoString, " You have got:(%d) Ball Points. ", this->m_iBallPoints);
				this->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
				return;
			}
			if (memcmp(cp, "/setcrit", 6) == 0) {
				this->AdminOrder_SetCrit(cp, dwMsgSize - 21);
				;
				return;
			}
			if (memcmp(cp, "/setek", 6) == 0) {
				this->AdminOrder_SetEK(cp, dwMsgSize - 21);
				;
				return;
			}
			if (memcmp(cp, "/setmaj", 7) == 0) {
				this->AdminOrder_SetMAJ(cp, dwMsgSize - 21);
				;
				return;
			}
			if (memcmp(cp, "/setrep", 7) == 0) {
				this->AdminOrder_SetRep(cp, dwMsgSize - 21);
				;
				return;
			}
			if (memcmp(cp, "/setpk", 6) == 0) {
				this->AdminOrder_SetPK(cp, dwMsgSize - 21);
				;
				return;
			}
			if (memcmp(cp, "/fi ", 4) == 0) {
				this->CheckAndNotifyPlayerConnection(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/to", 3) == 0) {
				this->ToggleWhisperPlayer(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/setpf ", 7) == 0) {
				this->SetPlayerProfile(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/weather", 8) == 0) {
				AdminOrder_Weather(client, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/pf ", 4) == 0) {
				this->GetPlayerProfile(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/shutup ", 8) == 0) {
				this->ShutUpPlayer(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/rep+ ", 6) == 0) {
				this->SetPlayerReputation(cp, 1, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/time ", 6) == 0) {
				this->AdminOrder_Time(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/checkrep", 9) == 0) {
				this->AdminOrder_CheckRep(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/checkstatus ", 13) == 0) {
				this->AdminOrder_CheckStats(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/send ", 5) == 0) {
				this->AdminOrder_Pushplayer(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/rep- ", 6) == 0) {
				this->SetPlayerReputation(cp, 0, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/hold", 5) == 0) {
				this->SetSummonMobAction(1, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/tgt ", 5) == 0) {
				this->SetSummonMobAction(2, dwMsgSize - 21, cp);
				return;
			}
			if (memcmp(cp, "/free", 5) == 0) {
				this->SetSummonMobAction(0, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/summonall ", 11) == 0) {
				this->AdminOrder_SummonAll(cp, dwMsgSize - 21);
				return;
			}
			/*		if (memcmp(cp, "/summonguild ", 13) == 0) {
						this->AdminOrder_SummonGuild(cp, dwMsgSize - 21);
						return;
					}*/
			if (memcmp(cp, "/summonplayer ", 14) == 0) {
				this->AdminOrder_SummonPlayer(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/storm ", 7) == 0) {
				AdminOrder_SummonStorm(client, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/summondeath ", 13) == 0) {
				this->AdminOrder_SummonDeath();
				return;
			}
			if (memcmp(cp, "/kill ", 6) == 0) {
				this->AdminOrder_Kill(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/revive ", 8) == 0) {
				this->AdminOrder_Revive(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/closeconn ", 11) == 0) {
				this->AdminOrder_CloseConn(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/ban", 4) == 0) {
				this->UserCommand_BanGuildsman(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/reservefightzone", 17) == 0) {
				this->AdminOrder_ReserveFightzone(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/dissmiss ", 9) == 0) {
				this->UserCommand_DissmissGuild(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/attack ", 8) == 0) {
				this->AdminOrder_CallGuard(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/createfish ", 12) == 0) {
				this->AdminOrder_CreateFish(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/teleport ", 10) == 0 || memcmp(cp, "/tp ", 4) == 0) {
				this->AdminOrder_Teleport(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/summondemon ", 13) == 0) {
				this->AdminOrder_SummonDemon();
				return;
			}
			if (memcmp(cp, "/unsummonall ", 13) == 0) {
				this->AdminOrder_UnsummonAll();
				return;
			}
			if (memcmp(cp, "/unsummondemon ", 15) == 0) {
				this->AdminOrder_UnsummonDemon();
				return;
			}
			if (memcmp(cp, "/checkip ", 9) == 0) {
				this->AdminOrder_CheckIP(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/polymorph ", 11) == 0) {
				this->AdminOrder_Polymorph(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/setinvi ", 9) == 0) {
				this->AdminOrder_SetInvi(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/gns ", 4) == 0) {
				this->AdminOrder_GetNpcStatus(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/setattackmode ", 15) == 0) {
				this->AdminOrder_SetAttackMode(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/summon ", 8) == 0) {
				this->AdminOrder_Summon(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/setzerk ", 9) == 0) {
				this->AdminOrder_SetZerk(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/setfreeze ", 11) == 0) {
				this->AdminOrder_SetFreeze(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/setstatus ", 11) == 0) {
				AdminOrder_SetStatus(client, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/disconnectall ", 15) == 0) {
				this->AdminOrder_DisconnectAll(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/createitem ", 12) == 0) {
				AdminOrder_CreateItem(client, cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/energysphere ", 14) == 0) {
				if (this->m_iAdminUserLevel >= m_iAdminLevelEnergySphere) EnergySphereProcessor(true, iClientH);
				return;
			}
			if (memcmp(cp, "/banip ", 7) == 0) {
				this->AdminOrder_BanIP(cp, dwMsgSize - 21);
				return;
			}
			if (memcmp(cp, "/roll", 5) == 0) {
				CStrTok * pStrTok = new class CStrTok(cp, " ");
				char * token;
				token = pStrTok->pGet();
				token = pStrTok->pGet();
				if (token == nullptr) {
					delete pStrTok;
					return;
				}
				delete pStrTok;
				return;
			}
			if ((memcmp(cp, "/shutdownthisserverrightnow ", 28) == 0) && (this->m_iAdminUserLevel >= m_iAdminLevelShutdown)) {
				m_cShutDownCode = 2;
				m_bOnExitProcess = true;
				m_dwExitProcessTime = timeGetTime();
				PutLogList("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Admin-Command)!!!");
				bSendMsgToLS(MSGID_GAMESERVERSHUTDOWNED, 0);
				auto ml = middlelandMap_.lock();
				if (ml) {
					// Crusade
					SaveOccupyFlagData();
					//bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, nullptr, nullptr);
					//bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, nullptr, nullptr);
				}
				return;
			}
			if ((memcmp(cp, "/setobservermode ", 17) == 0) && (this->m_iAdminUserLevel >= m_iAdminLevelObserver)) {
				AdminOrder_SetObserverMode(client);
				return;
			}
			if ((memcmp(cp, "/getticket ", 11) == 0) && (this->m_iAdminUserLevel >= 2)) {
				this->AdminOrder_GetFightzoneTicket();
				return;
			}
			if (memcmp(cp, "/beginapocalypse", 16) == 0) {
				if (this->m_iAdminUserLevel > 3) {
					if (this->m_bIsAdminCommandEnabled == false) return;
					GlobalStartApocalypseMode();
					wsprintf(cTemp, "GM Order(%s): beginapocalypse", this->m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}
			if (memcmp(cp, "/endapocalypse", 14) == 0) {
				if (this->m_iAdminUserLevel > 3) {
					if (this->m_bIsAdminCommandEnabled == false) return;
					GlobalEndApocalypseMode();
					wsprintf(cTemp, "GM Order(%s): endapocalypse", this->m_cCharName);
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
				}
				return;
			}
			if (memcmp(cp, "/beginheldenian ", 16) == 0) {
				if (this->m_iAdminUserLevel > 2) {
					this->ManualStartHeldenianMode(cp, dwMsgSize - 21);
				}
				return;
			}
			if (memcmp(cp, "/endheldenian ", 14) == 0) {
				if (this->m_iAdminUserLevel > 2) {
					this->ManualEndHeldenianMode(cp, dwMsgSize - 21);
				}
				return;
			}
			if (memcmp(cp, "/beginheldenianfight", 20) == 0) {
				if ((this->m_iAdminUserLevel > 2) && (m_bIsHeldenianMode == 1)) {
					NotifyStartHeldenianFightMode();
				}
				return;
			}
			return;
	}
	pData[dwMsgSize - 1] = 0;
	if ((this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] == 1) && (iDice(1, 3) != 2)) {
		cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 17);
		while (*cp != 0) {
			if ((cp[0] != 0) && (cp[0] != ' ') && (cp[1] != 0) && (cp[1] != ' ')) {
				switch (iDice(1, 3)) {
					case 1: memcpy(cp, "¿ö", 2);
						break;
					case 2: memcpy(cp, "¿ì", 2);
						break;
					case 3: memcpy(cp, "¿ù", 2);
						break;
				}
				cp += 2;
			} else cp++;
		}
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 17);
	if ((cSendMode == 0) && (this->whisperedPlayer_.lock())) {
		cSendMode = 20;
		if (*cp == '#') cSendMode = 0;
		if (this->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
	}
	wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	*wp = (uint16_t) iClientH;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 16);
	*cp = cSendMode;
	if (cSendMode != 20) {
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if (m_pClientList[i] != nullptr) {
				switch (cSendMode) {
					case 0:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;
						if ((m_pClientList[i]->map_ == this->map_) &&
								  (m_pClientList[i]->m_sX > this->m_sX - 10) &&
								  (m_pClientList[i]->m_sX < this->m_sX + 10) &&
								  (m_pClientList[i]->m_sY > this->m_sY - 7) &&
								  (m_pClientList[i]->m_sY < this->m_sY + 7)) {
							// Crusade
							if (m_bIsCrusadeMode == true) {
								if ((this->m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
										  (m_pClientList[i]->m_cSide != this->m_cSide)) {
								} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
							} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						}
						break;
					case 1:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;
						if ((memcmp(m_pClientList[i]->m_cGuildName, this->m_cGuildName, 20) == 0) &&
								  (memcmp(m_pClientList[i]->m_cGuildName, "NONE", 4) != 0)) {
							// Crusade
							if (m_bIsCrusadeMode == true) {
								if ((this->m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
										  (m_pClientList[i]->m_cSide != this->m_cSide)) {
								} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
							} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						}
						break;
					case 2:
					case 10:
					case 35: // GM Chat By Carlos
						// Crusade
						if (m_bIsCrusadeMode == true) {
							if ((this->m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
									  (m_pClientList[i]->m_cSide != this->m_cSide)) {
							} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						} else iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						break;
					case 3:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;
						if ((m_pClientList[i]->m_cSide == this->m_cSide))
							iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						break;
					case 4:
						if (m_pClientList[i]->m_bIsInitComplete == false) break;
						if ((m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == this->m_iPartyID))
							iRet = m_pClientList[i]->m_pXSock->iSendMsg(pData, dwMsgSize);
						break;
				}
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						//m_pClientList[i]->DeleteClient(true, true);
						break;
				}
			}
	} else {
		// New 16/05/2004
		iRet = this->m_pXSock->iSendMsg(pData, dwMsgSize);
		auto whispered = this->whisperedPlayer_.lock();
		if (whispered) {
			iRet = whispered->m_pXSock->iSendMsg(pData, dwMsgSize);
			switch (m_iLogChatOption) {
				case 1:
					if (whispered->m_iAdminUserLevel == 0) {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "GM Whisper   (%s):\"%s\"\tto Player(%s)", this->m_cCharName, pData + 21, whispered->m_cCharName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					}
					break;
				case 2:
					if (whispered->m_iAdminUserLevel > 0) {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "GM Whisper   (%s):\"%s\"\tto GM(%s)", this->m_cCharName, pData + 21, whispered->m_cCharName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					}
					break;
				case 3:
					if (whispered->m_iAdminUserLevel > 0) {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "GM Whisper   (%s):\"%s\"\tto GM(%s)", this->m_cCharName, pData + 21, whispered->m_cCharName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					} else {
						std::memset(cTemp, 0, sizeof (cTemp));
						wsprintf(cTemp, "Player Whisper   (%s):\"%s\"\tto Player(%s)", this->m_cCharName, pData + 21, whispered->m_cCharName);
						bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
					}
					break;
				case 4:
					break;
			}
		}
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				//m_pClientList[i]->DeleteClient(true, true);
				break;
		}
	}
}

void CClient::RequestPurchaseItemHandler(char * pItemName, int iNum) {
	class CItem * pItem;
	char * cp, cItemName[21], cData[100];
	short * sp;
	uint32_t * dwp, dwGoldCount, dwItemCount;
	uint16_t * wp, wTempPrice;
	int i;
	int iRet;
	int iEraseReq;
	int iCost2;
	int iCost;
	int iDiscountRatio;
	int iDiscountCost;
	double dTmp1;
	double dTmp2;
	double dTmp3;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	//if ( (memcmp(this->m_cLocation, "NONE", 4) != 0) &&
	//	 (memcmp(this->map_->m_cLocationName, this->m_cLocation, 10) != 0) ) return;
	if (memcmp(this->m_cLocation, "NONE", 4) != 0) {
		if (memcmp(this->m_cLocation, "are", 3) == 0) {
			if ((memcmp(this->map_->m_cLocationName, "aresden", 7) == 0) ||
					  (memcmp(this->map_->m_cLocationName, "arefarm", 7) == 0)) {
			} else return;
		}
		if (memcmp(this->m_cLocation, "elv", 3) == 0) {
			if ((memcmp(this->map_->m_cLocationName, "elvine", 6) == 0) ||
					  (memcmp(this->map_->m_cLocationName, "elvfarm", 7) == 0)) {
			} else return;
		}
	}
	std::memset(cData, 0, sizeof (cData));
	std::memset(cItemName, 0, sizeof (cItemName));
	// New 18/05/2004
	if (this->m_pIsProcessingAllowed == false) return;
	if (memcmp(pItemName, "10Arrows", 8) == 0) {
		strcpy(cItemName, "Arrow");
		dwItemCount = 10;
	} else if (memcmp(pItemName, "100Arrows", 9) == 0) {
		strcpy(cItemName, "Arrow");
		dwItemCount = 100;
	} else {
		memcpy(cItemName, pItemName, 20);
		dwItemCount = 1;
	}
	for (i = 1; i <= iNum; i++) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {
			delete pItem;
		} else {
			if (pItem->m_bIsForSale == false) {
				delete pItem;
				return;
			}
			pItem->m_dwCount = dwItemCount;
			if (this->m_cSide == m_iLastCrusadeWinner) {
				iCost = (int) ((float) (pItem->m_wPrice) * 0.9f + 0.5f);
				iCost = iCost * pItem->m_dwCount;
				iCost2 = pItem->m_wPrice * pItem->m_dwCount;
			} else {
				iCost2 = iCost = ((pItem->m_wPrice * pItem->m_dwCount)*2);
			}
			iCost = pItem->m_wPrice * pItem->m_dwCount;
			/* v1.3 <- ÀÌ°Ç _bAddClientItemList¿¡¼­ Ã¼Å©ÇÏ´Â ±â´ÉÀÌ¹Ç·Î µÎ¹øÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
			if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW))
				iItemWeight = (pItem->m_wWeight * pItem->m_dwCount);
			else iItemWeight = pItem->m_wWeight;
			if ( (iItemWeight + this->m_iCurWeightLoad) > this->_iCalcMaxLoad()) {


				dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
			 *dwp = MSGID_NOTIFY;
				wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
			 *wp  = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					this->DeleteClient(true, true);
					break;
				}
				delete pItem;
				return;
			}
			 */
			dwGoldCount = this->dwGetItemCount("Gold");
			iDiscountRatio = ((this->m_iCharisma - 10) / 4);
			// 2.03 Discount Method
			// iDiscountRatio = (this->m_iCharisma / 4) -1;
			// if (iDiscountRatio == 0) iDiscountRatio = 1;
			dTmp1 = (double) (iDiscountRatio);
			dTmp2 = dTmp1 / 100.0f;
			dTmp1 = (double) iCost;
			dTmp3 = dTmp1 * dTmp2;
			iDiscountCost = (int) dTmp3;
			if (iDiscountCost >= (iCost / 2)) iDiscountCost = (iCost / 2) - 1;
			if ((iCost - iDiscountCost) <= (iCost2 / 2)) {
				iDiscountCost = iCost - (iCost2 / 2) + 1;
			}
			if (dwGoldCount < (uint32_t) (iCost - iDiscountCost)) {
				delete pItem;
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_NOTENOUGHGOLD;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = -1;
				cp++;
				iRet = this->m_pXSock->iSendMsg(cData, 7);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
				return;
			}
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				if (this->m_iCurWeightLoad < 0) this->m_iCurWeightLoad = 0;
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMPURCHASED;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char) 0;
				cp++;
				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				wp = (uint16_t *) cp;
				*wp = (iCost - iDiscountCost);
				wTempPrice = (iCost - iDiscountCost);
				cp += 2;
				if (iEraseReq == 1) delete pItem;
				iRet = this->m_pXSock->iSendMsg(cData, 48);
				this->SetItemCount("Gold", dwGoldCount - wTempPrice);
				this->iCalcTotalWeight();
				m_stCityStatus[this->m_cSide].iFunds += wTempPrice;
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
			} else {
				delete pItem;
				this->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CClient::GiveItemHandler(short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName) {
	int iEraseReq;
	short * sp, sOwnerH;
	char * cp, cOwnerType, cData[100], cCharName[21];
	uint32_t * dwp;
	uint16_t * wp;
	class CItem * pItem;
	if (this->markedForDeletion_) return;
	if (this->m_bIsOnServerChange == true) return;
	if ((m_bAdminSecurity == true) && (this->m_iAdminUserLevel > 0)) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_pItemList[sItemIndex] == nullptr) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (iAmount <= 0) return;
	if (memcmp(this->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) {
		PutLogList("GiveItemHandler - Not matching Item name");
		return;
	}
	std::memset(cCharName, 0, sizeof (cCharName));
	if (((this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
			  (this->m_pItemList[sItemIndex]->m_dwCount > (uint32_t) iAmount)) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, this->m_pItemList[sItemIndex]->m_cName) == false) {
			delete pItem;
			return;
		} else {
			pItem->m_dwCount = iAmount;
		}
		this->m_pItemList[sItemIndex]->m_dwCount -= iAmount;
		this->SetItemCount(sItemIndex, this->m_pItemList[sItemIndex]->m_dwCount);
		this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
		if (wObjectID != 0) {
			if (wObjectID < 10000) {
				if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS)) {
					if (m_pClientList[wObjectID] != nullptr) {
						if ((uint16_t) sOwnerH != wObjectID) sOwnerH = 0;
					}
				}
			} else {
				// NPC
				if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS)) {
					if (m_pNpcList[wObjectID - 10000] != nullptr) {
						if ((uint16_t) sOwnerH != (wObjectID - 10000)) sOwnerH = 0;
					}
				}
			}
		}
		if (sOwnerH == 0) {
			this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
			// v1.411
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);
			this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
					  this->m_sX, this->m_sY,
					  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		} else {
			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
				memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);
				if (sOwnerH == iClientH) {
					delete pItem;
					return;
				}
				if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == true) {
					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_ITEMOBTAINED;
					cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
					*cp = 1;
					cp++;
					memcpy(cp, pItem->m_cName, 20);
					cp += 20;
					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwCount;
					cp += 4;
					*cp = pItem->m_cItemType;
					cp++;
					*cp = pItem->m_cEquipPos;
					cp++;
					*cp = (char) 0;
					cp++;
					sp = (short *) cp;
					*sp = pItem->m_sLevelLimit;
					cp += 2;
					*cp = pItem->m_cGenderLimit;
					cp++;
					wp = (uint16_t *) cp;
					*wp = pItem->m_wCurLifeSpan;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = pItem->m_wWeight;
					cp += 2;
					sp = (short *) cp;
					*sp = pItem->m_sSprite;
					cp += 2;
					sp = (short *) cp;
					*sp = pItem->m_sSpriteFrame;
					cp += 2;
					*cp = pItem->m_cItemColor;
					cp++;
					*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
					cp++;
					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwAttribute;
					cp += 4;
					/*
					 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
					cp++;
					 */
					if (iEraseReq == 1) delete pItem;
					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 53);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							m_pClientList[sOwnerH]->DeleteClient(true, true);
							break;
					}
					this->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0, cCharName);
				} else {
					this->map_->bSetItem(this->m_sX,
							  this->m_sY,
							  pItem);
					// v1.411
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  this->m_sX, this->m_sY,
							  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							m_pClientList[sOwnerH]->DeleteClient(true, true);
							break;
					}
					this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTGIVEITEM, sItemIndex, iAmount, 0, cCharName);
				}
			} else {
				memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);
				if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0) {
					if (this->bSetItemToBankItem(pItem) == false) {
						this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTITEMTOBANK, 0, 0, 0, nullptr);
						this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
						// v1.411
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);
						this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
								  this->m_sX, this->m_sY,
								  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
					}
				} else {
					this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
					// v1.411
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  this->m_sX, this->m_sY,
							  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
				}
			}
		}
	} else {
		this->ReleaseItemHandler(sItemIndex, true);
		if (this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)
			this->m_cArrowIndex = -1;
		this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
		if (wObjectID != 0) {
			if (wObjectID < 10000) {
				if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS)) {
					if (m_pClientList[wObjectID] != nullptr) {
						if ((uint16_t) sOwnerH != wObjectID) sOwnerH = 0;
					}
				}
			} else {
				// NPC
				if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS)) {
					if (m_pNpcList[wObjectID - 10000] != nullptr) {
						if ((uint16_t) sOwnerH != (wObjectID - 10000)) sOwnerH = 0;
					}
				}
			}
		}
		if (sOwnerH == 0) {
			this->map_->bSetItem(this->m_sX,
					  this->m_sY,
					  & * this->m_pItemList[sItemIndex]);
			// v1.411
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, & * this->m_pItemList[sItemIndex]);
			this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
					  this->m_sX, this->m_sY,
					  this->m_pItemList[sItemIndex]->m_sSprite,
					  this->m_pItemList[sItemIndex]->m_sSpriteFrame,
					  this->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
			this->SendNotifyMsg(0, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, nullptr);
		} else {
			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {

				memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);
				pItem = & * this->m_pItemList[sItemIndex];


				if (pItem->m_sIDnum == 88) {



					if ((this->m_iGuildRank == -1) &&
							  (memcmp(this->m_cLocation, "NONE", 4) != 0) &&
							  (memcmp(this->m_cLocation, m_pClientList[sOwnerH]->m_cLocation, 10) == 0) &&
							  (m_pClientList[sOwnerH]->m_iGuildRank == 0)) {

						m_pClientList[sOwnerH]->this->SendNotifyMsg(DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION, 0, 0, 0, nullptr);

						this->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, 0, cCharName);
						_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int) - 1, pItem);
						goto REMOVE_ITEM_PROCEDURE;
					}
				}

				if ((m_bIsCrusadeMode == false) && (this->m_pItemList[sItemIndex]->m_sIDnum == 89)) {



					if ((memcmp(this->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
							  (this->m_iGuildRank != -1) &&
							  (m_pClientList[sOwnerH]->m_iGuildRank == 0)) {

						m_pClientList[sOwnerH]->this->SendNotifyMsg(DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION, 0, 0, 0, nullptr);

						this->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, 0, cCharName);
						_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int) - 1, pItem);
						goto REMOVE_ITEM_PROCEDURE;
					}
				}
				if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == true) {
					_bItemLog(DEF_ITEMLOG_GIVE, iClientH, sOwnerH, pItem);
					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_ITEMOBTAINED;
					cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
					*cp = 1;
					cp++;
					memcpy(cp, pItem->m_cName, 20);
					cp += 20;
					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwCount;
					cp += 4;
					*cp = pItem->m_cItemType;
					cp++;
					*cp = pItem->m_cEquipPos;
					cp++;
					*cp = (char) 0;
					cp++;
					sp = (short *) cp;
					*sp = pItem->m_sLevelLimit;
					cp += 2;
					*cp = pItem->m_cGenderLimit;
					cp++;
					wp = (uint16_t *) cp;
					*wp = pItem->m_wCurLifeSpan;
					cp += 2;
					wp = (uint16_t *) cp;
					*wp = pItem->m_wWeight;
					cp += 2;
					sp = (short *) cp;
					*sp = pItem->m_sSprite;
					cp += 2;
					sp = (short *) cp;
					*sp = pItem->m_sSpriteFrame;
					cp += 2;
					*cp = pItem->m_cItemColor;
					cp++;
					*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
					cp++;
					dwp = (uint32_t *) cp;
					*dwp = pItem->m_dwAttribute;
					cp += 4;
					/*
					 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
					cp++;
					 */
					if (iEraseReq == 1) delete pItem;
					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 53);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							m_pClientList[sOwnerH]->DeleteClient(true, true);
							break;
					}
				} else {
					this->map_->bSetItem(this->m_sX,
							  this->m_sY,
							  & * this->m_pItemList[sItemIndex]);
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, & * this->m_pItemList[sItemIndex]);
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  this->m_sX, this->m_sY,
							  this->m_pItemList[sItemIndex]->m_sSprite,
							  this->m_pItemList[sItemIndex]->m_sSpriteFrame,
							  this->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
					dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
					int iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							m_pClientList[sOwnerH]->DeleteClient(true, true);
							break;
					}
					std::memset(cCharName, 0, sizeof (cCharName));
				}
			} else {
				memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);
				if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0) {
					if (this->bSetItemToBankItem(sItemIndex) == false) {
						this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTITEMTOBANK, 0, 0, 0, nullptr);
						this->map_->bSetItem(this->m_sX,
								  this->m_sY,
								  & * this->m_pItemList[sItemIndex]);
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, & * this->m_pItemList[sItemIndex]);
						this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
								  this->m_sX, this->m_sY,
								  this->m_pItemList[sItemIndex]->m_sSprite,
								  this->m_pItemList[sItemIndex]->m_sSpriteFrame,
								  this->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
					}
				} else if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Kennedy", 7) == 0) {
					if ((m_bIsCrusadeMode == false) && (this->m_pItemList[sItemIndex]->m_sIDnum == 89)) {
						if ((this->m_iGuildRank != 0) && (this->m_iGuildRank != -1)) {
							this->this->SendNotifyMsg(DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, nullptr);
							std::memset(this->m_cGuildName, 0, sizeof (this->m_cGuildName));
							memcpy(this->m_cGuildName, "NONE", 4);
							this->m_iGuildRank = -1;
							this->m_iGuildGUID = -1;
							this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							this->m_iExp -= 300;
							if (this->m_iExp < 0) this->m_iExp = 0;
							this->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
						}
						this->m_pItemList[sItemIndex].reset();
					} else {
						this->map_->bSetItem(this->m_sX,
								  this->m_sY,
								  & * this->m_pItemList[sItemIndex]);
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, & * this->m_pItemList[sItemIndex]);
						this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
								  this->m_sX, this->m_sY,
								  this->m_pItemList[sItemIndex]->m_sSprite,
								  this->m_pItemList[sItemIndex]->m_sSpriteFrame,
								  this->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
						std::memset(cCharName, 0, sizeof (cCharName));
					}
				} else {
					this->map_->bSetItem(this->m_sX,
							  this->m_sY,
							  & * this->m_pItemList[sItemIndex]);
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, & * this->m_pItemList[sItemIndex]);
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  this->m_sX, this->m_sY,
							  this->m_pItemList[sItemIndex]->m_sSprite,
							  this->m_pItemList[sItemIndex]->m_sSpriteFrame,
							  this->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
					std::memset(cCharName, 0, sizeof (cCharName));
				}
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, cCharName);
		}
REMOVE_ITEM_PROCEDURE:
		;
		if (this->markedForDeletion_) return;
		this->m_pItemList[sItemIndex] = nullptr;
		this->m_bIsItemEquipped[sItemIndex] = false;
		this->m_cArrowIndex = this->_iGetArrowItemIndex();
	}
	this->iCalcTotalWeight();
}

void CClient::JoinGuildApproveHandler(char * pName) {
	register int i;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {
			if (memcmp(m_pClientList[i]->m_cLocation, this->m_cLocation, 10) != 0) return;
			std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
			strcpy(m_pClientList[i]->m_cGuildName, this->m_cGuildName);
			m_pClientList[i]->m_iGuildGUID = this->m_iGuildGUID;
			std::memset(m_pClientList[i]->m_cLocation, 0, sizeof (m_pClientList[i]->m_cLocation));
			strcpy(m_pClientList[i]->m_cLocation, this->m_cLocation);
			m_pClientList[i]->m_iGuildRank = DEF_GUILDSTARTRANK;
			m_pClientList[i]->this->SendNotifyMsg(DEF_COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, nullptr);
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			SendGuildMsg(i, DEF_NOTIFY_NEWGUILDSMAN, 0, 0, nullptr);
			bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN, i);
			return;
		}
}

void CClient::JoinGuildRejectHandler(char * pName) {
	register int i;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {
			m_pClientList[i]->this->SendNotifyMsg(DEF_COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, nullptr);
			return;
		}
}

void CClient::DismissGuildApproveHandler(char * pName) {
	register int i;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {
			bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);
			//_bItemLog(DEF_ITEMLOG_BANGUILD,i,(char *)nullptr,nullptr) ;
			SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, nullptr);
			std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
			strcpy(m_pClientList[i]->m_cGuildName, "NONE");
			m_pClientList[i]->m_iGuildRank = -1;
			m_pClientList[i]->m_iGuildGUID = -1;
			m_pClientList[i]->this->SendNotifyMsg(DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, nullptr);
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			return;
		}
}

void CClient::DismissGuildRejectHandler(char * pName) {
	register int i;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0)) {
			m_pClientList[i]->this->SendNotifyMsg(DEF_COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, nullptr);
			return;
		}
}

uint32_t CClient::dwGetItemCount(const char * pName) {
	register int i;
	char cTmpName[21];
	if (this->markedForDeletion_) return 0;
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pName);
	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((this->m_pItemList[i] != nullptr) && (memcmp(this->m_pItemList[i]->m_cName, cTmpName, 20) == 0)) {
			return this->m_pItemList[i]->m_dwCount;
		}
	return 0;
}

int CClient::SetItemCount(const char * pItemName, uint32_t dwCount) {
	register int i;
	char cTmpName[21];
	uint16_t wWeight;
	if (this->markedForDeletion_) return -1;
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pItemName);
	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((this->m_pItemList[i] != nullptr) && (memcmp(this->m_pItemList[i]->m_cName, cTmpName, 20) == 0)) {
			wWeight = iGetItemWeight(*this->m_pItemList[i], 1); // this->m_pItemList[i]->m_wWeight;
			if (dwCount == 0) {
				this->ItemDepleteHandler(i, false, true);
			} else {
				this->m_pItemList[i]->m_dwCount = dwCount;
				this->SendNotifyMsg(0, DEF_NOTIFY_SETITEMCOUNT, i, dwCount, (char) true, nullptr);
			}
			return wWeight;
		}
	return -1;
}

int CClient::SetItemCount(int iItemIndex, uint32_t dwCount) {
	uint16_t wWeight;
	if (this->markedForDeletion_) return -1;
	if (this->m_pItemList[iItemIndex] == nullptr) return -1;
	wWeight = iGetItemWeight(*this->m_pItemList[iItemIndex], 1); //this->m_pItemList[iItemIndex]->m_wWeight;
	if (dwCount == 0) {
		this->ItemDepleteHandler(iItemIndex, false, true);
	} else {
		this->m_pItemList[iItemIndex]->m_dwCount = dwCount;
		this->SendNotifyMsg(0, DEF_NOTIFY_SETITEMCOUNT, iItemIndex, dwCount, (char) true, nullptr);
	}
	return wWeight;
}

void CClient::TimeManaPointsUp() {
	register int iMaxMP, iTotal;
	double dV1;
	double dV2;
	double dV3;
	if (this->markedForDeletion_) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iHungerStatus <= 0) return;
	if (this->m_bSkillUsingStatus[19] == true) return;
	iMaxMP = (2 * (this->m_iMag + this->m_iAngelicMag)) + (2 * this->m_iLevel) + ((this->m_iInt + this->m_iAngelicInt) / 2); // v1.4
	if (this->m_iMP < iMaxMP) {
		iTotal = iDice(1, ((this->m_iMag + this->m_iAngelicMag)));
		if (this->m_iAddMP != 0) {
			dV2 = (double) iTotal;
			dV3 = (double) this->m_iAddMP;
			dV1 = (dV3 / 100.0f) * dV2;
			iTotal += (int) dV1;
		}
		this->m_iMP += iTotal;
		if (this->m_iMP > iMaxMP)
			this->m_iMP = iMaxMP;
		this->SendNotifyMsg(0, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
	}
}

void CClient::TimeStaminarPointsUp() {
	register int iMaxSP, iTotal = 0;
	double dV1;
	double dV2;
	double dV3;
	if (this->markedForDeletion_) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iHungerStatus <= 0) return;
	if (this->m_bSkillUsingStatus[19] == true) return;
	iMaxSP = (2 * (this->m_iStr + this->m_iAngelicStr)) + (2 * this->m_iLevel);
	if (this->m_iSP < iMaxSP) {
		iTotal = iDice(1, (this->m_iVit / 3));
		if (this->m_iAddSP != 0) {
			dV2 = (double) iTotal;
			dV3 = (double) this->m_iAddSP;
			dV1 = (dV3 / 100.0f) * dV2;
			iTotal += (int) dV1;
		}
		if (this->m_iLevel <= 20) {
			iTotal += 15;
		} else if (this->m_iLevel <= 40) {
			iTotal += 10;
		} else if (this->m_iLevel <= 60) {
			iTotal += 5;
		}
		this->m_iSP += iTotal;
		if (this->m_iSP > iMaxSP)
			this->m_iSP = iMaxSP;
		this->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
	}
}

void CClient::SendGuildMsg(uint16_t wNotifyMsgType, short /*sV1*/, short /*sV2*/, char * pString) {
	char cData[500];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	register int i, iRet;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) &&
				  (memcmp(m_pClientList[i]->m_cGuildName, this->m_cGuildName, 20) == 0)) {
			std::memset(cData, 0, sizeof (cData));
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = wNotifyMsgType;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			switch (wNotifyMsgType) {
				case DEF_NOTIFY_GUILDDISBANDED:
					if (i == iClientH) break;
					memcpy(cp, this->m_cGuildName, 20);
					cp += 20;
					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 26);
					std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
					strcpy(m_pClientList[i]->m_cGuildName, "NONE");
					m_pClientList[i]->m_iGuildRank = -1;
					m_pClientList[i]->m_iGuildGUID = -1;
					break;
				case DEF_NOTIFY_EVENTMSGSTRING:
					strcpy(cp, pString);
					cp += strlen(pString);
					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + strlen(pString) + 1);
					break;
				case DEF_NOTIFY_NEWGUILDSMAN:
					memcpy(cp, this->m_cCharName, 10);
					cp += 10;
					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + 10 + 1);
					break;
				case DEF_NOTIFY_DISMISSGUILDSMAN:
					memcpy(cp, this->m_cCharName, 10);
					cp += 10;
					iRet = m_pClientList[i]->m_pXSock->iSendMsg(cData, 6 + 10 + 1);
					break;
			}
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					m_pClientList[i]->DeleteClient(true, true);
					return;
			}
		}
}

void CClient::ToggleCombatModeHandler() {
	short sAppr2;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_bSkillUsingStatus[19] == true) return;
	sAppr2 = (short) ((this->m_sAppr2 & 0xF000) >> 12);
	this->m_bIsAttackModeChange = true; // v2.172
	if (sAppr2 == 0) {
		this->m_sAppr2 = (0xF000 | this->m_sAppr2);
	} else {
		this->m_sAppr2 = (0x0FFF & this->m_sAppr2);
	}
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

int CClient::iClientMotion_Magic_Handler(short sX, short sY, char cDir) {
	char cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	if (this->markedForDeletion_) return 0;
	if (this->m_bIsKilled == true) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if ((sX != this->m_sX) || (sY != this->m_sY)) return 2;
	int iStX;
	int iStY;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (this->m_cSide) {
			case 0: this->map_->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: this->map_->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: this->map_->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}
	this->ClearSkillUsingStatus();
	this->map_->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	this->map_->this->SetOwner(DEF_OWNERTYPE_PLAYER, sX, sY);
	if ((this->m_iStatus & 0x10) != 0) {
		this->SetInvisibilityFlag(DEF_OWNERTYPE_PLAYER, false);
		delayEvents_.this->remove(DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		m_pClientList[ iClientH ]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
	}
	this->m_cDir = cDir;
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;
	iRet = this->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			return 0;
	}
	return 1;
}
int _tmp_iMCProb[] = {0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40};
int _tmp_iMLevelPenalty[] = {0, 5, 5, 8, 8, 10, 14, 28, 32, 36, 40};

void CClient::PlayerMagicHandler(int dX, int dY, short sType, bool bItemEffect, int iV1) {
	short * sp, sX, sY, sOwnerH, sMagicCircle, rx, ry, sRemainItemSprite, sRemainItemSpriteFrame, sLevelMagic, sTemp;
	char * cp, cData[120], cDir, cOwnerType, cName[11], cItemName[21], cNpcWaypoint[11], cName_Master[11], cNpcName[21], cRemainItemColor, cScanMessage[256];
	double dV1;
	double dV2;
	double dV3;
	double dV4;
	int i;
	int iErr;
	int iRet;
	int ix;
	int iy;
	int iResult;
	int iDiceRes;
	int iNamingValue;
	int iFollowersNum;
	int iEraseReq;
	int iWhetherBonus;
	int tX;
	int tY;
	int iManaCost;
	int iMagicAttr;
	class CItem * pItem;
	uint32_t * dwp, dwTime;
	uint16_t * wp, wWeaponType;
	short sEqStatus;
	dwTime = timeGetTime();
	this->m_bMagicConfirm = true;
	this->m_dwLastActionTime = this->m_dwAFKCheckTime = timeGetTime();
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if ((dX < 0) || (dX >= this->map_->m_sSizeX) ||
			  (dY < 0) || (dY >= this->map_->m_sSizeY)) return;
	if (((dwTime - this->m_dwRecentAttackTime) < 1000) && (bItemEffect == 0)) {
		wsprintf(G_cTxt, "3.51 Detection: (%s) Player: (%s) - Magic casting speed is too fast! Hack?", this->m_cIPaddress, this->m_cCharName);
		PutHackLogFileList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	this->m_dwRecentAttackTime = dwTime;
	this->m_dwLastActionTime = dwTime;
	if (this->map_ == nullptr) return;
	if ((sType < 0) || (sType >= 100)) return;
	if (m_pMagicConfigList[sType] == nullptr) return;
	if ((bItemEffect == false) && (this->m_cMagicMastery[sType] != 1)) return;
	if ((this->map_->m_bIsAttackEnabled == false) && (this->m_iAdminUserLevel == 0)) return;
	//if ((var_874 == true) && (this->map_->m_bIsHeldenianMap == true) && (m_pMagicConfigList[sType]->m_sType != 8)) return;
	if (((this->m_iStatus & 0x100000) != 0) && (bItemEffect != true)) {
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}
	if (this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
		wWeaponType = ((this->m_sAppr2 & 0x0FF0) >> 4);
		if ((wWeaponType >= 34) && (wWeaponType <= 39)) {
		} else return;
	}
	if ((this->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1) ||
			  (this->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)) return;
	if ((this->m_iSpellCount > 1) && (bItemEffect == false)) {
		wsprintf(G_cTxt, "TSearch Spell Hack: (%s) Player: (%s) - casting magic without precasting.", this->m_cIPaddress, this->m_cCharName);
		PutHackLogFileList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	if (this->m_bInhibition == true) {
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}
	/*if (((this->m_iUninteruptibleCheck - (this->iGetMaxHP()/10)) > (this->m_iHP)) && (this->m_bMagicItem == false)) {
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, nullptr,
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
		return;
	}*/
	if (m_pMagicConfigList[sType]->m_sType == 32) { // Invisiblity
		sEqStatus = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
		if ((sEqStatus != -1) && (this->m_pItemList[sEqStatus] != nullptr)) {
			if ((this->m_pItemList[sEqStatus]->m_sIDnum == 865) || (this->m_pItemList[sEqStatus]->m_sIDnum == 866)) {
				bItemEffect = true;
			}
		}
	}
	sX = this->m_sX;
	sY = this->m_sY;
	sMagicCircle = (sType / 10) + 1;
	if (this->m_cSkillMastery[4] == 0)
		dV1 = 1.0f;
	else dV1 = (double) this->m_cSkillMastery[4];
	if (bItemEffect == true) dV1 = (double) 100.0f;
	dV2 = (double) (dV1 / 100.0f);
	dV3 = (double) _tmp_iMCProb[sMagicCircle];
	dV1 = dV2 * dV3;
	iResult = (int) dV1;
	if ((this->m_iInt + this->m_iAngelicInt) > 50)
		iResult += ((this->m_iInt + this->m_iAngelicInt) - 50) / 2;
	sLevelMagic = (this->m_iLevel / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double) (this->m_iLevel - sLevelMagic * 10);
			dV2 = (double) abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double) abs(sMagicCircle - sLevelMagic)*10;
			dV4 = (dV1 / dV3) * dV2;
			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int) dV4);
		} else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}
	switch (this->map_->m_cWhetherStatus) {
		case 0: break;
		case 1: iResult = iResult - (iResult / 24);
			break;
		case 2: iResult = iResult - (iResult / 12);
			break;
		case 3: iResult = iResult - (iResult / 5);
			break;
	}
	if (this->m_iSpecialWeaponEffectType == 10) {
		dV1 = (double) iResult;
		dV2 = (double) (this->m_iSpecialWeaponEffectValue * 3);
		dV3 = dV1 + dV2;
		iResult = (int) dV3;
	}
	if (iResult <= 0) iResult = 1;
	iWhetherBonus = iGetWhetherMagicBonusEffect(sType, this->map_->m_cWhetherStatus);
	iManaCost = m_pMagicConfigList[sType]->m_sValue1;
	if ((this->m_bIsSafeAttackMode == true) &&
			  (this->map_->m_bIsFightZone == false)) {
		iManaCost += (iManaCost / 2) - (iManaCost / 10);
	}
	if (this->m_iManaSaveRatio > 0) {
		dV1 = (double) this->m_iManaSaveRatio;
		dV2 = (double) (dV1 / 100.0f);
		dV3 = (double) iManaCost;
		dV1 = dV2 * dV3;
		dV2 = dV3 - dV1;
		iManaCost = (int) dV2;
		if (iManaCost <= 0) iManaCost = 1;
	}
	wWeaponType = ((this->m_sAppr2 & 0x0FF0) >> 4);
	if (wWeaponType == 34) {
		iManaCost += 20;
	}
	if (iResult < 100) {
		iDiceRes = iDice(1, 100);
		if (iResult < iDiceRes) {
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
			return;
		}
	}
	if (((this->m_iHungerStatus <= 10) || (this->m_iSP <= 0)) && (iDice(1, 1000) <= 100)) {
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
		return;
	}
	if (this->m_iMP < iManaCost) {
		return;
	}
	iResult = this->m_cSkillMastery[4];
	if ((this->m_iMag + this->m_iAngelicMag) > 50) iResult += ((this->m_iMag + this->m_iAngelicMag) - 50);
	sLevelMagic = (this->m_iLevel / 10);
	if (sMagicCircle != sLevelMagic) {
		if (sMagicCircle > sLevelMagic) {
			dV1 = (double) (this->m_iLevel - sLevelMagic * 10);
			dV2 = (double) abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double) abs(sMagicCircle - sLevelMagic)*10;
			dV4 = (dV1 / dV3) * dV2;
			iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int) dV4);
		} else {
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}
	iResult += this->m_iAddAR;
	if (iResult <= 0) iResult = 1;
	if (sType >= 80) iResult += 10000;
	if (m_pMagicConfigList[sType]->m_sType == 28) {
		iResult += 10000;
	}
	if (m_pMagicConfigList[sType]->m_cCategory == 1) {
		if (this->map_->iGetAttribute(sX, sY, 0x00000005) != 0) return;
	}
	iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;
	if ((this->m_iStatus & 0x10) != 0) {
		this->SetInvisibilityFlag(DEF_OWNERTYPE_PLAYER, false);
		delayEvents_.this->remove(DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
		this->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
	}
	this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((m_bIsCrusadeMode == false) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
		if ((this->m_bIsPlayerCivil != true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true)) {
			if (this->m_cSide != m_pClientList[sOwnerH]->m_cSide) return;
		} else if ((this->m_bIsPlayerCivil == true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == false)) {
			switch (m_pMagicConfigList[sType]->m_sType) {
				case 1: // DEF_MAGICTYPE_DAMAGE_SPOT
				case 4: // DEF_MAGICTYPE_SPDOWN_SPOT 4
				case 8: // DEF_MAGICTYPE_TELEPORT 8
				case 10: // DEF_MAGICTYPE_CREATE 10
				case 11: // DEF_MAGICTYPE_PROTECT 11
				case 12: // DEF_MAGICTYPE_HOLDOBJECT 12
				case 16: // DEF_MAGICTYPE_CONFUSE
				case 17: // DEF_MAGICTYPE_POISON
				case 24: // DEF_MAGICTYPE_RESURRECTION
					return;
			}
		}
	}
	if (m_pMagicConfigList[sType]->m_dwDelayTime == 0) {
		switch (m_pMagicConfigList[sType]->m_sType) {
			case DEF_MAGICTYPE_DAMAGE_SPOT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}
				break;
			case DEF_MAGICTYPE_HPUP_SPOT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				this->Effect_HpUp_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				break;
			case DEF_MAGICTYPE_PURITYLIFE_SPOT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				this->PURITYLIFE(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				break;
			case DEF_MAGICTYPE_DAMAGE_AREA:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;
			case DEF_MAGICTYPE_SPDOWN_SPOT:
				break;
			case DEF_MAGICTYPE_SPDOWN_AREA:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
				break;
			case DEF_MAGICTYPE_POLYMORPH:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (1) { // bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] != 0) goto MAGIC_NOEFFECT;
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							m_pClientList[sOwnerH]->m_sOriginalType = m_pClientList[sOwnerH]->m_sType;
							m_pClientList[sOwnerH]->m_sType = 18;
							m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] != 0) goto MAGIC_NOEFFECT;
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_POLYMORPH ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							m_pNpcList[sOwnerH]->m_sOriginalType = m_pNpcList[sOwnerH]->m_sType;
							m_pNpcList[sOwnerH]->m_sType = 18;
							m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							break;
					}
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_POLYMORPH, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					if (cOwnerType == DEF_OWNERTYPE_PLAYER)
						m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POLYMORPH, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				}
				break;
				// 05/20/2004 - Hypnotoad - Cancellation
			case DEF_MAGICTYPE_CANCELLATION:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) && (m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)) {
					// Removes Invisibility Flag 0x0010
					SetInvisibilityFlag(sOwnerH, cOwnerType, false);
					// Removes Illusion Flag 0x01000000
					SetIllusionFlag(sOwnerH, cOwnerType, false);
					// Removes Defense Shield Flag 0x02000000
					// Removes Great Defense Shield Flag 0x02000000
					SetDefenseShieldFlag(sOwnerH, cOwnerType, false);
					// Removes Absolute Magic Protection Flag 0x04000000
					// Removes Protection From Magic Flag 0x04000000
					SetMagicProtectionFlag(sOwnerH, cOwnerType, false);
					// Removes Protection From Arrow Flag 0x08000000
					SetProtectionFromArrowFlag(sOwnerH, cOwnerType, false);
					// Removes Illusion Movement Flag 0x00200000
					SetIllusionMovementFlag(sOwnerH, cOwnerType, false);
					// Removes Berserk Flag 0x0020
					SetBerserkFlag(sOwnerH, cOwnerType, false);
					if (this->m_bInhibition == true)
						delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INHIBITION);
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					if (this->m_iStatus & 0x0010)
						delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					if (this->m_iStatus & 0x0020)
						delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_BERSERK);
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					if ((this->m_iStatus & 0x08000000) || (this->m_iStatus & 0x04000000) || (this->m_iStatus & 0x02000000))
						delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					if ((this->m_iStatus & 0x01000000) || (this->m_iStatus & 0x00200000))
						delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					// Update Client
					m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
				}
				break;
			case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:

				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {

						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
							this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
						}
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
								this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
						}
					}
				break;
			case DEF_MAGICTYPE_DAMAGE_LINEAR:
				sX = this->m_sX;
				sY = this->m_sY;
				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
					// tx, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx-1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx+1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx, ty-1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx, ty+1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				// dX, dY
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
				}
				break;
			case DEF_MAGICTYPE_ICE_LINEAR:
				sX = this->m_sX;
				sY = this->m_sY;
				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
					// tx, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_iHP < 0) goto MAGIC_NOEFFECT;
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}
					// tx-1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}
					// tx+1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}
					// tx, ty-1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}
					// tx, ty+1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
					}
					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
							//
						}
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
								switch (cOwnerType) {
									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
										if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
											if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
												m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
												SetIceFlag(sOwnerH, cOwnerType, true);
												delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
												m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
											}
										}
										break;
									case DEF_OWNERTYPE_NPC:
										if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
										if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
											if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
												m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
												SetIceFlag(sOwnerH, cOwnerType, true);
												delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											}
										}
										break;
								}
								//
							}
						}
					}
				// dX, dY
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
									m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
								}
							}
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
									SetIceFlag(sOwnerH, cOwnerType, true);
									delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
											  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
								}
							}
							break;
					}
					//
				}
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
						//
					}
				}
				break;
			case DEF_MAGICTYPE_INHIBITION:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if ((m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) || (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)) goto MAGIC_NOEFFECT; // juan - ic fix para que no pegue si tiene AMP o PFM .
						if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INHIBITION ] != 0) goto MAGIC_NOEFFECT;
						if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
						//if (m_pClientList[sOwnerH]->m_iAdminUserLevel != 0) goto MAGIC_NOEFFECT;
						m_pClientList[sOwnerH]->m_bInhibition = true;
						delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
						break;
				}
				break;
			case DEF_MAGICTYPE_TREMOR:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;
			case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;
			case DEF_MAGICTYPE_SPUP_AREA:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				this->Effect_SpUp_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						this->Effect_SpUp_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
					}
				break;
			case DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN:
				sX = this->m_sX;
				sY = this->m_sY;
				for (i = 2; i < 10; i++) {
					iErr = 0;
					m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
					// tx, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}
					// tx-1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}
					// tx+1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}
					// tx, ty-1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}
					// tx, ty+1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
					}
					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
										this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
									}
									break;
							}
							//
						}
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
								switch (cOwnerType) {
									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
										if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
											this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
											this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
										}
										break;
									case DEF_OWNERTYPE_NPC:
										if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
										if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
											this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
											this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
										}
										break;
								}
								//
							}
						}
					}
				// dX, dY
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
							}
							break;
					}
					//
				}
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
						  (m_pClientList[sOwnerH]->m_iHP > 0)) {
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
						this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
									this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
									this->Effect_SpDown_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
								}
								break;
						}
						//
					}
				}
				break;
			case DEF_MAGICTYPE_TELEPORT:
				//50Cent - Capture The Flag
				if (this->bCheckIfIsFlagCarrier()) {
					this->ShowClientMsg("You can not use that magic being a flag carrier.");
					goto MAGIC_NOEFFECT;
				}
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH == iClientH)) {
							if ((dwTime - this->m_dwLogoutHackCheck) > 10000) {
								this->RequestTeleportHandler("1 ");
							} else {
								char cCannotRecall[256];
								wsprintf(cCannotRecall, "Wait 10 seconds after get DMG to Recall.");
								this->ShowClientMsg(cCannotRecall);
								goto MAGIC_NOEFFECT;
							}
						}
						break;
				}
				break;
			case DEF_MAGICTYPE_SUMMON:
				if (this->map_->m_bIsFightZone == true) return;
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
					iFollowersNum = iGetFollowerNumber(sOwnerH, cOwnerType);
					if (iFollowersNum >= (this->m_cSkillMastery[4] / 20)) break;
					iNamingValue = this->map_->iGetEmptyNamingValue();
					if (iNamingValue != -1) {
						std::memset(cName, 0, sizeof (cName));
						wsprintf(cName, "XX%d", iNamingValue);
						cName[0] = '_';
						cName[1] = this->map_->id_ + 65;
						std::memset(cNpcName, 0, sizeof (cNpcName));
						switch (iV1) {
							case 0:
								iResult = iDice(1, this->m_cSkillMastery[4] / 10);
								if (iResult < this->m_cSkillMastery[4] / 20)
									iResult = this->m_cSkillMastery[4] / 20;
								switch (iResult) {
									case 1: strcpy(cNpcName, "Slime");
										break;
									case 2: strcpy(cNpcName, "Giant-Ant");
										break;
									case 3: strcpy(cNpcName, "Amphis");
										break;
									case 4: strcpy(cNpcName, "Orc");
										break;
									case 5: strcpy(cNpcName, "Skeleton");
										break;
									case 6: strcpy(cNpcName, "Clay-Golem");
										break;
									case 7: strcpy(cNpcName, "Stone-Golem");
										break;
									case 8: strcpy(cNpcName, "Orc-Mage");
										break;
									case 9: strcpy(cNpcName, "Hellbound");
										break;
									case 10:strcpy(cNpcName, "Cyclops");
										break;
								}
								break;
							case 1: strcpy(cNpcName, "Orc");
								break;
							case 2: strcpy(cNpcName, "Skeleton");
								break;
							case 3: strcpy(cNpcName, "Clay-Golem");
								break;
							case 4: strcpy(cNpcName, "Stone-Golem");
								break;
							case 5: strcpy(cNpcName, "Hellbound");
								break;
							case 6: strcpy(cNpcName, "Cyclops");
								break;
							case 7: strcpy(cNpcName, "Troll");
								break;
							case 8: strcpy(cNpcName, "Orge");
								break;
							case 9: strcpy(cNpcName, "Sor-Aresden");
								break;
							case 10: strcpy(cNpcName, "ATK-Elvine");
								break;
							case 11: strcpy(cNpcName, "Elf-Aresden");
								break;
							case 12: strcpy(cNpcName, "DSK-Elvine");
								break;
							case 13: strcpy(cNpcName, "HBT-Aresden");
								break;
							case 14: strcpy(cNpcName, "Bar-Elvine");
								break;
						}
						if (this->map_->bCreateNewNpc(cNpcName, cName, 0, 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypoint, nullptr, 0, this->m_cSide, false, true) == false) {
							this->map_->SetNamingValueEmpty(iNamingValue);
						} else {
							std::memset(cName_Master, 0, sizeof (cName_Master));
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									memcpy(cName_Master, m_pClientList[sOwnerH]->m_cCharName, 10);
									break;
								case DEF_OWNERTYPE_NPC:
									memcpy(cName_Master, m_pNpcList[sOwnerH]->m_cName, 5);
									break;
							}
							bSetNpcFollowMode(cName, cName_Master, cOwnerType);
						}
					}
				}
				break;
			case DEF_MAGICTYPE_CREATE:
				if (this->map_->bGetIsMoveAllowedTile(dX, dY) == false)
					goto MAGIC_NOEFFECT;
				pItem = new class CItem;
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						// Food
						if (iDice(1, 2) == 1)
							wsprintf(cItemName, "Meat");
						else wsprintf(cItemName, "Baguette");
						break;
				}
				_bInitItemAttr(*pItem, cItemName);
				pItem->m_sTouchEffectType = DEF_ITET_ID;
				pItem->m_sTouchEffectValue1 = iDice(1, 100000);
				pItem->m_sTouchEffectValue2 = iDice(1, 100000);
				pItem->m_sTouchEffectValue3 = (short) timeGetTime();
				this->map_->bSetItem(dX, dY, pItem);
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, pItem);
				this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
						  dX, dY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
				break;
			case DEF_MAGICTYPE_PROTECT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] != 0) goto MAGIC_NOEFFECT;
						if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
						m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
						switch (m_pMagicConfigList[sType]->m_sValue4) {
							case 1:
								SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
								break;
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
						if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] != 0) goto MAGIC_NOEFFECT;
						if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
						m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
						switch (m_pMagicConfigList[sType]->m_sValue4) {
							case 1:
								SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
								break;
						}
						break;
				}
				delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
						  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
				if (cOwnerType == DEF_OWNERTYPE_PLAYER)
					m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_PROTECT, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				break;
				/*					// Magictype:	Protection from Magic, Protection from Arrow, Defense Shield
						//				Absolue Magic Protection, Great Defense Shield
						case DEF_MAGICTYPE_PROTECT:
							// iClientH (the caster) gets the spell targets (sOwnerH) type (npc/pc) and coords (x,y)
							this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
							// switch cases for the owner type (DEF_OWNERTYPE_PLAYER, DEF_OWNERTYPE_NPC)
							switch (cOwnerType) {
							// if the owners is a player
							case DEF_OWNERTYPE_PLAYER:
								// DISABLED: if the caster side isnt the same as the targets side, no effect occurs
								// DISABLED: if (this->m_cSide != m_pClientList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;
								// if the player target doesnt exist no magic effect
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								// if the player target already has a protect magic on, no magic effect occurs
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != false) goto MAGIC_NOEFFECT;
								// if the caster is neutral go to no magic effect
								if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
								// switch cases for the effect4 of the spell
								switch (m_pMagicConfigList[sType]->m_sValue4){
									// Protection From Arrow
									case 1:
										// triggers the spell target's protection from arrow flag on
										SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
										break; // end case 1
									// Protection From Magic
									// Absolue Magic Protection
									case 2:
									case 5:
										// triggers the spell target's protection from magic flag on
										SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
										break; // end case 2, case 5
									// Defense Shield
									// Great Defense Shield
									case 3:
									case 4:
										// triggers the spells target's defense shield flag on
										SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
										break; // end case 3, case 4
								} // end switch (m_pMagicConfigList[sType]->m_sValue4){
								// the target's magic effect is set to the effect4 value of the spell cast on him
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char)m_pMagicConfigList[sType]->m_sValue4;
								break; // break; DEF_OWNERTYPE_PLAYER
							case DEF_OWNERTYPE_NPC:
								// if the npc target doesnt exist no magic effect
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								// if the npc target already has a protect magic on, no magic effect occurs
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] != 0) goto MAGIC_NOEFFECT;
								// if the npc's action limit is not 0, no magic effect occurs
								if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
								// switch cases for the effect4 of the spell
								switch (m_pMagicConfigList[sType]->m_sValue4){
									// Protection From Arrow
									case 1:
										// triggers the spell target's protection from arrow flag on
										SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
										break; // end case 1
									// Protection From Magic
									// Absolue Magic Protection
									case 2:
									case 5:
										// triggers the spell target's protection from magic flag on
										SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
										break; // end case 2, case 5
									// Defense Shield
									// Great Defense Shield
									case 3:
									case 4:
										// triggers the spells target's defense shield flag on
										SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
										break; // end case 3, case 4
								} // end switch (m_pMagicConfigList[sType]->m_sValue4){
								// the npc's magic effect is set to the effect4 value of the spell cast on it
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] = (char)m_pMagicConfigList[sType]->m_sValue4;
								break;
							} // end switch (cOwnerType) {
							// the DEF_MAGICTYPE_PROTECT effect leaves after the magic effects "Last" time (in seconds)
							delayEvents_.bRegisterDelayEvent(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime*1000),
													 sOwnerH, cOwnerType, nullptr, nullptr, nullptr, m_pMagicConfigList[sType]->m_sValue4, nullptr, nullptr);
							// if the owner is a player send a message
							if (cOwnerType == DEF_OWNERTYPE_PLAYER)
								SendNotifyMsg(nullptr, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_PROTECT, m_pMagicConfigList[sType]->m_sValue4, nullptr, nullptr);
							break; // end case DEF_MAGICTYPE_PROTECT:*/
			case DEF_MAGICTYPE_SCAN:
				std::memset(cScanMessage, 0, sizeof (cScanMessage));
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							wsprintf(cScanMessage, " Player: %s || Health: %d || Mana: %d || Stamina: %d || Criticals: %d", m_pClientList[sOwnerH]->m_cCharName, m_pClientList[sOwnerH]->m_iHP, m_pClientList[sOwnerH]->m_iMP, m_pClientList[sOwnerH]->m_iSP, m_pClientList[sOwnerH]->m_iSuperAttackLeft);
							//this->ShowClientMsg(cScanMessage);
							this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cScanMessage);
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							wsprintf(cScanMessage, " Monster: %s || Health: %d || Mana: %d", m_pNpcList[sOwnerH]->m_cNpcName, m_pNpcList[sOwnerH]->m_iHP, m_pNpcList[sOwnerH]->m_iMana);
							//   this->ShowClientMsg(cScanMessage);
							this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cScanMessage);
							break;
					}
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
							  this->m_sX, this->m_sY, dX, dY, 10, 10);
				}
				break;
			case DEF_MAGICTYPE_HOLDOBJECT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto MAGIC_NOEFFECT;
							if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
							if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
								if (m_pClientList[sOwnerH]->map_->iGetAttribute(sX, sY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->map_->iGetAttribute(dX, dY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
							}
							if (strcmp(this->map_->m_cName, "middleland") != 0 &&
									  m_bIsCrusadeMode == false &&
									  this->m_cSide == m_pClientList[sOwnerH]->m_cSide)
								goto MAGIC_NOEFFECT;
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto MAGIC_NOEFFECT;
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) m_pMagicConfigList[sType]->m_sValue4;
							break;
					}
					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
							  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
					if (cOwnerType == DEF_OWNERTYPE_PLAYER)
						m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
				}
				break;
			case DEF_MAGICTYPE_INVISIBILITY:
				//50Cent - Capture The flag
				if (this->bCheckIfIsFlagCarrier()) {
					this->ShowClientMsg("You can not use that magic being a flag carrier.");
					goto MAGIC_NOEFFECT;
				}
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto MAGIC_NOEFFECT;
								if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetInvisibilityFlag(sOwnerH, cOwnerType, true);
								RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cActionLimit == 0) {
									m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) m_pMagicConfigList[sType]->m_sValue4;
									SetInvisibilityFlag(sOwnerH, cOwnerType, true);
									RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
								}
								break;
						}
						delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
						if (cOwnerType == DEF_OWNERTYPE_PLAYER)
							m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
						break;
					case 2:
						if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
						for (ix = dX - 8; ix <= dX + 8; ix++)
							for (iy = dY - 8; iy <= dY + 8; iy++) {
								this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (sOwnerH != 0) {
									switch (cOwnerType) {
										case DEF_OWNERTYPE_PLAYER:
											if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
											if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
												SetInvisibilityFlag(sOwnerH, cOwnerType, false);
												delayEvents_.remove(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
											}
											break;
										case DEF_OWNERTYPE_NPC:
											if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
											if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
												SetInvisibilityFlag(sOwnerH, cOwnerType, false);
												delayEvents_.remove(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
											}
											break;
									}
								}
							}
						break;
				}
				break;
			case DEF_MAGICTYPE_CREATE_DYNAMIC:
				if (m_bIsCrusadeMode == false) {
					if (strcmp(this->map_->m_cName, "aresden") == 0) return;
					if (strcmp(this->map_->m_cName, "elvine") == 0) return;
					// v2.14
					if (strcmp(this->map_->m_cName, "aresden") == 0) return;
					if (strcmp(this->map_->m_cName, "elvine") == 0) return;
				}
				switch (m_pMagicConfigList[sType]->m_sValue10) {
					case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:
					case DEF_DYNAMICOBJECT_FIRE:
					case DEF_DYNAMICOBJECT_SPIKE: // Spike
#ifdef DEF_TAIWANLOG
						short sTemp_X;
						short sTemp_Y;
						sTemp_X = this->m_sX;
						sTemp_Y = this->m_sY;
						this->m_sX = dX;
						this->m_sY = dY;
						_bItemLog(DEF_ITEMLOG_SPELLFIELD, iClientH, m_pMagicConfigList[sType]->m_cName, nullptr);
						this->m_sX = sTemp_X;
						this->m_sY = sTemp_Y;
#endif
						switch (m_pMagicConfigList[sType]->m_sValue11) {
							case 1:
								// wall - type
								cDir = m_Misc.cGetNextMoveDir(this->m_sX, this->m_sY, dX, dY);
								switch (cDir) {
									case 1: rx = 1;
										ry = 0;
										break;
									case 2: rx = 1;
										ry = 1;
										break;
									case 3: rx = 0;
										ry = 1;
										break;
									case 4: rx = -1;
										ry = 1;
										break;
									case 5: rx = 1;
										ry = 0;
										break;
									case 6: rx = -1;
										ry = -1;
										break;
									case 7: rx = 0;
										ry = -1;
										break;
									case 8: rx = 1;
										ry = -1;
										break;
								}
								dynamicObjects_.this->iAddDynamicObjectList(DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, this->map_,
										  dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
								this->bAnalyzeCriminalAction(dX, dY);
								for (i = 1; i <= m_pMagicConfigList[sType]->m_sValue12; i++) {
									dynamicObjects_.this->iAddDynamicObjectList(DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, this->map_,
											  dX + i*rx, dY + i*ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
									this->bAnalyzeCriminalAction(dX + i*rx, dY + i * ry);
									dynamicObjects_.this->iAddDynamicObjectList(DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, this->map_,
											  dX - i*rx, dY - i*ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
									this->bAnalyzeCriminalAction(dX - i*rx, dY - i * ry);
								}
								break;
							case 2:
								// Field - Type
								bool bFlag = false;
								int cx;
								int cy;
								for (ix = dX - m_pMagicConfigList[sType]->m_sValue12; ix <= dX + m_pMagicConfigList[sType]->m_sValue12; ix++)
									for (iy = dY - m_pMagicConfigList[sType]->m_sValue12; iy <= dY + m_pMagicConfigList[sType]->m_sValue12; iy++) {
										dynamicObjects_.this->iAddDynamicObjectList(DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, this->map_,
												  ix, iy, m_pMagicConfigList[sType]->m_dwLastTime * 1000, m_pMagicConfigList[sType]->m_sValue5);
										if (this->bAnalyzeCriminalAction(ix, iy, true) == true) {
											bFlag = true;
											cx = ix;
											cy = iy;
										}
									}
								if (bFlag == true) this->bAnalyzeCriminalAction(cx, cy);
								break;
						}
						//
						break;
					case DEF_DYNAMICOBJECT_ICESTORM:
						// Ice-Storm Dynamic Object
						dynamicObjects_.this->iAddDynamicObjectList(DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, this->map_,
								  dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000,
								  this->m_cSkillMastery[4]);
						break;
					default:
						break;
				}
				break;
			case DEF_MAGICTYPE_POSSESSION:
				if (this->m_cSide == 0) goto MAGIC_NOEFFECT;
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (sOwnerH != 0) break;
				pItem = this->map_->pGetItem(dX, dY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
				if (pItem != nullptr) {
					if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
						_bItemLog(DEF_ITEMLOG_GET, iClientH, (int) - 1, pItem);
						dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
						*dwp = MSGID_NOTIFY;
						wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
						*wp = DEF_NOTIFY_ITEMOBTAINED;
						cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
						*cp = 1;
						cp++;
						memcpy(cp, pItem->m_cName, 20);
						cp += 20;
						dwp = (uint32_t *) cp;
						*dwp = pItem->m_dwCount;
						cp += 4;
						*cp = pItem->m_cItemType;
						cp++;
						*cp = pItem->m_cEquipPos;
						cp++;
						*cp = (char) 0;
						cp++;
						sp = (short *) cp;
						*sp = pItem->m_sLevelLimit;
						cp += 2;
						*cp = pItem->m_cGenderLimit;
						cp++;
						wp = (uint16_t *) cp;
						*wp = pItem->m_wCurLifeSpan;
						cp += 2;
						wp = (uint16_t *) cp;
						*wp = pItem->m_wWeight;
						cp += 2;
						sp = (short *) cp;
						*sp = pItem->m_sSprite;
						cp += 2;
						sp = (short *) cp;
						*sp = pItem->m_sSpriteFrame;
						cp += 2;
						*cp = pItem->m_cItemColor;
						cp++;
						*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
						cp++;
						dwp = (uint32_t *) cp;
						*dwp = pItem->m_dwAttribute;
						cp += 4;
						/*
						 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
						cp++;
						 */
						if (iEraseReq == 1) delete pItem;
						this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM,
								  dX, dY, sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor); // v1.4
						iRet = this->m_pXSock->iSendMsg(cData, 53);
						switch (iRet) {
							case DEF_XSOCKEVENT_QUENEFULL:
							case DEF_XSOCKEVENT_SOCKETERROR:
							case DEF_XSOCKEVENT_CRITICALERROR:
							case DEF_XSOCKEVENT_SOCKETCLOSED:
								this->DeleteClient(true, true);
								return;
						}
					} else {
						this->map_->bSetItem(dX, dY, pItem);
						dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
						*dwp = MSGID_NOTIFY;
						wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
						*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
						iRet = this->m_pXSock->iSendMsg(cData, 6);
						switch (iRet) {
							case DEF_XSOCKEVENT_QUENEFULL:
							case DEF_XSOCKEVENT_SOCKETERROR:
							case DEF_XSOCKEVENT_CRITICALERROR:
							case DEF_XSOCKEVENT_SOCKETCLOSED:
								this->DeleteClient(true, true);
								return;
						}
					}
				}
				//
				break;
			case DEF_MAGICTYPE_CONFUSE:
				// if the caster side is the same as the targets side, no effect occurs
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
					case 2: // Confusion, Mass Confusion
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
							for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
								this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] != 0) break;
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] = (char) m_pMagicConfigList[sType]->m_sValue4;
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
									}
								}
							}
						break;
					case 3: // Ilusion, Mass-Ilusion
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
							for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
								this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] != 0) break;
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] = (char) m_pMagicConfigList[sType]->m_sValue4;
										switch (m_pMagicConfigList[sType]->m_sValue4) {
											case 3:
												SetIllusionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
												break;
										}
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, nullptr);
									}
								}
							}
						break;
					case 4: // Ilusion Movement
						if (this->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) break;
						for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
							for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
								this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] != 0) break;
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ] = (char) m_pMagicConfigList[sType]->m_sValue4;
										switch (m_pMagicConfigList[sType]->m_sValue4) {
											case 4:
												//para que los del mismo pueblo no se den con illusion movement by KahBur
												if ((strcmp(m_pClientList[sOwnerH]->m_cLocation, this->m_cLocation)) == 0) goto MAGIC_NOEFFECT;
												if ((this->m_bIsPlayerCivil == false) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true)) goto MAGIC_NOEFFECT;
												SetIllusionMovementFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
												break;
										}
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, nullptr);
									}
								}
							}
						break;
				}
				break;
			case DEF_MAGICTYPE_POISON:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (m_pMagicConfigList[sType]->m_sValue4 == 1) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (memcmp(this->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
							this->bAnalyzeCriminalAction(dX, dY);
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false) {
									m_pClientList[sOwnerH]->m_bIsPoisoned = true;
									m_pClientList[sOwnerH]->m_iPoisonLevel = m_pMagicConfigList[sType]->m_sValue5;
									m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;
									// 05/06/2004 - Hypnotoad - poison aura appears when cast Poison
									SetPoisonFlag(sOwnerH, cOwnerType, true);
									m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pMagicConfigList[sType]->m_sValue5, 0, nullptr);
#ifdef DEF_TAIWANLOG
									_bItemLog(DEF_ITEMLOG_POISONED, sOwnerH, (char *) nullptr, nullptr);
#endif
								}
							}
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pNpcList[sOwnerH]->m_iHP > 0) goto MAGIC_NOEFFECT;
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false) {
								}
							}
							break;
					}
				} else if (m_pMagicConfigList[sType]->m_sValue4 == 0) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							if (m_pClientList[sOwnerH]->m_bIsPoisoned == true) {
								m_pClientList[sOwnerH]->m_bIsPoisoned = false;
								// 05/06/2004 - Hypnotoad - poison aura removed when cure cast
								SetPoisonFlag(sOwnerH, cOwnerType, false);
								m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, nullptr);
							}
							break;
						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
							break;
					}
				}
				break;
			case DEF_MAGICTYPE_BERSERK:
				switch (m_pMagicConfigList[sType]->m_sValue4) {
					case 1:
						this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] != 0) goto MAGIC_NOEFFECT;
								m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetBerserkFlag(sOwnerH, cOwnerType, true);
								break;
							case DEF_OWNERTYPE_NPC:
								if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] != 0) goto MAGIC_NOEFFECT;
								if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
								if (this->m_cSide != m_pNpcList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;
								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] = (char) m_pMagicConfigList[sType]->m_sValue4;
								SetBerserkFlag(sOwnerH, cOwnerType, true);
								break;
						}
						delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
						if (cOwnerType == DEF_OWNERTYPE_PLAYER)
							m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, m_pMagicConfigList[sType]->m_sValue4, 0, nullptr);
						break;
				}
				break;
			case DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK:
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							this->ArmorLifeDecrement(sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
						}
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
								this->ArmorLifeDecrement(sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
							}
						}
					}
				break;
				/*						// Resurrection Magic.
								case DEF_MAGICTYPE_RESURRECTION:
									// 10 Mins once
									if (this->m_iSpecialAbilityTime != 0) goto MAGIC_NOEFFECT;
									this->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC/2;
									// Get the ID of the dead Player/NPC on coords dX, dY.
									this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
									switch (cOwnerType) {
												// For Player.
										case DEF_OWNERTYPE_PLAYER:
											// The Player has to exist.
											if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
											// Resurrection is not for alive Players.
											if (m_pClientList[sOwnerH]->m_bIsKilled == false) goto MAGIC_NOEFFECT;
											// Set Deadflag to Alive.
											m_pClientList[sOwnerH]->m_bIsKilled = false;
											// Player's HP becomes half of the Max HP.
											m_pClientList[sOwnerH]->m_iHP = ((m_pClientList[sOwnerH]->m_iLevel * 2) + (m_pClientList[sOwnerH]->m_iVit * 3) + (m_pClientList[sOwnerH]->m_iStr / 2)) / 2;
											// Send new HP to Player.
											SendNotifyMsg(nullptr, sOwnerH, DEF_NOTIFY_HP, nullptr, nullptr, nullptr, nullptr);
											// Make Player stand up. (Currently, by a fake damage).
											m_pClientList[sOwnerH]->map_->ClearDeadOwner(dX, dY);
											m_pClientList[sOwnerH]->map_->SetOwner(sOwnerH, DEF_OWNERTYPE_PLAYER, dX, dY);
											m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, nullptr, nullptr, nullptr);
											SendNotifyMsg(nullptr, sOwnerH, DEF_NOTIFY_HP, nullptr, nullptr, nullptr, nullptr);
											break;
											// Resurrection is not for NPC's.
										case DEF_OWNERTYPE_NPC:
											goto MAGIC_NOEFFECT;
											break;
									}
									break;*/
			case DEF_MAGICTYPE_ICE:
				for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
					for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
							//this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							this->Effect_Damage_Spot_DamageMove(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
							switch (cOwnerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
										}
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sOwnerH] == nullptr) goto MAGIC_NOEFFECT;
									if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
										if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
											m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
											SetIceFlag(sOwnerH, cOwnerType, true);
											delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
													  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										}
									}
									break;
							}
						}
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
								//this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								this->Effect_Damage_Spot(DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
								if ((m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false)) {
									if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										SetIceFlag(sOwnerH, cOwnerType, true);
										delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
												  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
										m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
							}
						}
					}
				break;
			default:
				break;
		}
	} else {
		// Resurrection wand(MS.10) or Resurrection wand(MS.20)
		if (m_pMagicConfigList[sType]->m_sType == DEF_MAGICTYPE_RESURRECTION) {
			//Check if player has resurrection wand
			if (!this->markedForDeletion_ && this->m_iSpecialAbilityTime == 0 &&
					  this->m_bIsSpecialAbilityEnabled == false) {
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (m_pClientList[sOwnerH] != nullptr) {
					// GM's can ressurect ne1, and players must be on same side to ressurect
					/*if ((this->m_pItemList[this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]]->m_sIDnum != 865) ||
						(this->m_pItemList[this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]]->m_sIDnum != 866)) {
						wsprintf(G_cTxt, "(!!!) Hack: Player(%s) cast resurrect without wand!", this->m_cCharName);
						PutLogList(G_cTxt);
						this->DeleteClient(true, true, true, true);
						return;
					}*/
					if ((this->m_iAdminUserLevel < 1) &&
							  (m_pClientList[sOwnerH]->m_cSide != this->m_cSide)) {
						return;
					}
					if (cOwnerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sOwnerH] != nullptr &&
							  m_pClientList[sOwnerH]->m_iHP <= 0) {
						m_pClientList[sOwnerH]->m_bIsBeingResurrected = true;
						m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_RESURRECTPLAYER, 0, 0, 0, nullptr);
						if (this->m_iAdminUserLevel < 2) {
							this->m_bIsSpecialAbilityEnabled = true;
							this->m_dwSpecialAbilityStartTime = dwTime;
							this->m_iSpecialAbilityLastSec = 0;
							this->m_iSpecialAbilityTime = m_pMagicConfigList[sType]->m_dwDelayTime;
							sTemp = this->m_sAppr4;
							sTemp = 0xFF0F & sTemp;
							sTemp = sTemp | 0x40;
							this->m_sAppr4 = sTemp;
						}
						this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, this->m_iSpecialAbilityType, this->m_iSpecialAbilityLastSec, nullptr);
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
					}
				}
			}
		}
	}
MAGIC_NOEFFECT:
	;
	if (this->markedForDeletion_) return;
	//Mana Slate
	if ((this->m_iStatus & 0x800000) != 0) {
		iManaCost = 0;
	}
	this->m_iMP -= iManaCost;
	if (this->m_iMP < 0)
		this->m_iMP = 0;
	this->CalculateSSN_SkillIndex(4, 1);
	this->SendNotifyMsg(0, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
	this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
			  this->m_sX, this->m_sY, dX, dY, (sType + 100), this->m_sType);
}

void CClient::DropItemHandler(short sItemIndex, int iAmount, char * pItemName, bool bByPlayer) {
	class CItem * pItem;
	if (this->markedForDeletion_) return;
	if (this->m_bIsOnServerChange == true) return;
	if ((m_bAdminSecurity == true) && (this->m_iAdminUserLevel > 0)) return;
	if (this->m_bIsInitComplete == false) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (this->m_pItemList[sItemIndex] == nullptr) return;
	if ((iAmount != -1) && (iAmount < 0)) return;
	if (((this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
			  (iAmount == -1))
		iAmount = this->m_pItemList[sItemIndex]->m_dwCount;
	if (memcmp(this->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) return;
	if (((this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			  (this->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
			  (((int) this->m_pItemList[sItemIndex]->m_dwCount - iAmount) > 0)) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, this->m_pItemList[sItemIndex]->m_cName) == false) {
			delete pItem;
			return;
		} else {
			if (iAmount <= 0) {
				delete pItem;
				return;
			}
			pItem->m_dwCount = (uint32_t) iAmount;
		}
		if ((uint32_t) iAmount > this->m_pItemList[sItemIndex]->m_dwCount) {
			delete pItem;
			return;
		}
		this->m_pItemList[sItemIndex]->m_dwCount -= iAmount;
		// v1.41 !!!
		this->SetItemCount(sItemIndex, this->m_pItemList[sItemIndex]->m_dwCount);
		this->map_->bSetItem(this->m_sX,
				  this->m_sY, pItem);
		if (bByPlayer == true)
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, pItem);
		else
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, pItem, true);
		this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  this->m_sX, this->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
		this->SendNotifyMsg(0, DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0, nullptr);
	} else {
		this->ReleaseItemHandler(sItemIndex, true);
		if (this->m_bIsItemEquipped[sItemIndex] == true)
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
		// v1.432
		if ((this->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) &&
				  (this->m_pItemList[sItemIndex]->m_wCurLifeSpan == 0)) {
			this->m_pItemList[sItemIndex].reset();
		} else {
			this->map_->bSetItem(this->m_sX,
					  this->m_sY,
					  & * this->m_pItemList[sItemIndex]);
			if (bByPlayer == true)
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, & * this->m_pItemList[sItemIndex]);
			else
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int) - 1, & * this->m_pItemList[sItemIndex], true);
			this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
					  this->m_sX, this->m_sY,
					  this->m_pItemList[sItemIndex]->m_sSprite,
					  this->m_pItemList[sItemIndex]->m_sSpriteFrame,
					  this->m_pItemList[sItemIndex]->m_cItemColor); //v1.4 color
		}
		this->m_pItemList[sItemIndex] = nullptr;
		this->m_bIsItemEquipped[sItemIndex] = false;
		this->SendNotifyMsg(0, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, nullptr);
		this->m_cArrowIndex = this->_iGetArrowItemIndex();
	}
	this->iCalcTotalWeight();
}

int CClient::iClientMotion_GetItem_Handler(short sX, short sY, char cDir) {
	uint32_t * dwp;
	uint16_t * wp;
	char * cp;
	short * sp, sRemainItemSprite, sRemainItemSpriteFrame;
	char cRemainItemColor;
	char cData[100];
	int iRet;
	int iEraseReq;
	class CItem * pItem;
	if (this->markedForDeletion_) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (this->m_bIsKilled == true) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if ((sX != this->m_sX) || (sY != this->m_sY)) return 2;
	int iStX;
	int iStY;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (this->m_cSide) {
			case 0: this->map_->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: this->map_->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: this->map_->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}
	this->ClearSkillUsingStatus();
	this->map_->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	this->map_->this->SetOwner(DEF_OWNERTYPE_PLAYER, sX, sY);
	pItem = this->map_->pGetItem(sX, sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
	if (pItem != nullptr) {
		if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
			_bItemLog(DEF_ITEMLOG_GET, iClientH, 0, pItem);
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMOBTAINED;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char) 0;
			cp++;
			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor;
			cp++;
			*cp = (char) pItem->m_sItemSpecEffectValue2;
			cp++;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;
			if (iEraseReq == 1) delete pItem;
			this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM,
					  this->m_sX, this->m_sY,
					  sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);
			iRet = this->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					return 0;
			}
		} else {
			this->map_->bSetItem(sX, sY, pItem);
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					return 0;
			}
		}
	}
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;
	iRet = this->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			return 0;
	}
	return 1;
}

bool CClient::_bAddClientItemList(class CItem * pItem, int * pDelReq) {
	register int i;
	if (this->markedForDeletion_) return false;
	if (pItem == nullptr) return false;
	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {
		if ((this->m_iCurWeightLoad + iGetItemWeight(*pItem, pItem->m_dwCount)) > this->_iCalcMaxLoad())
			return false;
	} else {
		if ((this->m_iCurWeightLoad + iGetItemWeight(*pItem, 1)) > this->_iCalcMaxLoad())
			return false;
	}
	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {
		for (i = 0; i < DEF_MAXITEMS; i++)
			if ((this->m_pItemList[i] != nullptr) &&
					  (memcmp(this->m_pItemList[i]->m_cName, pItem->m_cName, 20) == 0)) {
				this->m_pItemList[i]->m_dwCount += pItem->m_dwCount;
				//delete pItem;
				*pDelReq = 1;
				this->iCalcTotalWeight();
				return true;
			}
	}
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] == nullptr) {
			this->m_pItemList[i].reset(pItem);
			this->m_ItemPosList[i].x = 40;
			this->m_ItemPosList[i].y = 30;
			*pDelReq = 0;
			if (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)
				this->m_cArrowIndex = this->_iGetArrowItemIndex();
			this->iCalcTotalWeight();
			return true;
		}
	return false;
}

bool CClient::bEquipItemHandler(short sItemIndex, bool bNotify) {
	char cEquipPos;
	char cHeroArmorType;
	short sSpeed;
	short sTemp;
	int iTemp;
	if (this->markedForDeletion_) return false;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
	if (this->m_pItemList[sItemIndex] == nullptr) return false;
	if (this->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return false;
	if (this->m_pItemList[sItemIndex]->m_wCurLifeSpan == 0) return false;
	if (((this->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) == 0) &&
			  (this->m_pItemList[sItemIndex]->m_sLevelLimit > this->m_iLevel)) return false;
	if (this->m_pItemList[sItemIndex]->m_cGenderLimit != 0) {
		switch (this->m_sType) {
			case 1:
			case 2:
			case 3:
				if (this->m_pItemList[sItemIndex]->m_cGenderLimit != 1) return false;
				break;
			case 4:
			case 5:
			case 6:
				if (this->m_pItemList[sItemIndex]->m_cGenderLimit != 2) return false;
				break;
		}
	}
	if (iGetItemWeight(*this->m_pItemList[sItemIndex], 1) > (this->m_iStr + this->m_iAngelicStr)*100) return false;
	cEquipPos = this->m_pItemList[sItemIndex]->m_cEquipPos;
	if ((cEquipPos == DEF_EQUIPPOS_BODY) || (cEquipPos == DEF_EQUIPPOS_LEGGINGS) ||
			  (cEquipPos == DEF_EQUIPPOS_ARMS) || (cEquipPos == DEF_EQUIPPOS_HEAD)) {
		switch (this->m_pItemList[sItemIndex]->m_sItemEffectValue4) {
			case 10:
				if ((this->m_iStr + this->m_iAngelicStr) < this->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
					this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 11: // Dex
				if ((this->m_iDex + this->m_iAngelicDex) < this->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
					this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 12: // Vit
				if (this->m_iVit < this->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
					this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 13: // Int
				if ((this->m_iInt + this->m_iAngelicInt) < this->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
					this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 14: // Mag
				if ((this->m_iMag + this->m_iAngelicMag) < this->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
					this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
			case 15: // Chr
				if (this->m_iCharisma < this->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, nullptr);
					this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], true);
					return false;
				}
				break;
		}
	}
	if (cEquipPos == DEF_EQUIPPOS_TWOHAND) {
		// Stormbringer
		if (this->m_pItemList[sItemIndex]->m_sIDnum == 845) {
			if ((this->m_iInt + this->m_iInt) < 65) {
				this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_iSpecialAbilityEquipPos, sItemIndex, 0, nullptr);
				this->ReleaseItemHandler(sItemIndex, true);
				return false;
			}
		}
	}
	if (cEquipPos == DEF_EQUIPPOS_RHAND) {
		// Resurrection wand(MS.10) or Resurrection wand(MS.20)
		if ((this->m_pItemList[sItemIndex]->m_sIDnum == 865) || (this->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
			if ((this->m_iInt + this->m_iAngelicInt) > 99 && (this->m_iMag + this->m_iAngelicMag) > 99 && this->m_iSpecialAbilityTime < 1) {
				this->m_cMagicMastery[94] = true;
				this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
			}
		}
	}
	if ((this->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
			  (this->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) {
		if ((this->m_iSpecialAbilityType != 0)) {
			if (this->m_pItemList[sItemIndex]->m_cEquipPos != this->m_iSpecialAbilityEquipPos) {
				this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_iSpecialAbilityEquipPos, this->m_sItemEquipmentStatus[ this->m_iSpecialAbilityEquipPos ], 0, nullptr);
				this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ this->m_iSpecialAbilityEquipPos ], true);
			}
		}
	}
	if (cEquipPos == DEF_EQUIPPOS_NONE) return false;
	if (cEquipPos == DEF_EQUIPPOS_TWOHAND) {
		if (this->m_sItemEquipmentStatus[ cEquipPos ] != -1)
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], false);
		else {
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ] != -1)
				this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RHAND ], false);
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LHAND ] != -1)
				this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LHAND ], false);
		}
	} else {
		if ((cEquipPos == DEF_EQUIPPOS_LHAND) || (cEquipPos == DEF_EQUIPPOS_RHAND)) {
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ] != -1)
				this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_TWOHAND ], false);
		}
		if (this->m_sItemEquipmentStatus[ cEquipPos ] != -1)
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], false);
	}
	if (cEquipPos == DEF_EQUIPPOS_RELEASEALL) {
		if (this->m_sItemEquipmentStatus[ cEquipPos ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], false);
		}
		if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_HEAD ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_HEAD ], false);
		}
		if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BODY ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BODY ], false);
		}
		if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_ARMS ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_ARMS ], false);
		}
		if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LEGGINGS ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_LEGGINGS ], false);
		}
		if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_PANTS ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_PANTS ], false);
		}
		if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BACK ] != -1) {
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_BACK ], false);
		}
	} else {
		if (cEquipPos == DEF_EQUIPPOS_HEAD || cEquipPos == DEF_EQUIPPOS_BODY || cEquipPos == DEF_EQUIPPOS_ARMS ||
				  cEquipPos == DEF_EQUIPPOS_LEGGINGS || cEquipPos == DEF_EQUIPPOS_PANTS || cEquipPos == DEF_EQUIPPOS_BACK) {
			if (this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RELEASEALL ] != -1) {
				this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ DEF_EQUIPPOS_RELEASEALL ], false);
			}
		}
		if (this->m_sItemEquipmentStatus[ cEquipPos ] != -1)
			this->ReleaseItemHandler(this->m_sItemEquipmentStatus[ cEquipPos ], false);
	}
	this->m_sItemEquipmentStatus[cEquipPos] = sItemIndex;
	this->m_bIsItemEquipped[sItemIndex] = true;
	switch (cEquipPos) {
		case DEF_EQUIPPOS_HEAD:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0xFF0F;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 4);
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF0;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor));
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_PANTS:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0xF0FF;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 8);
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFF0FF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 8);
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_LEGGINGS:
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0x0FFF;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 12);
			this->m_sAppr4 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFF0F;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 4);
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_BODY:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			if (this->m_pItemList[sItemIndex]->m_cApprValue < 100) {
				sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 12);
				this->m_sAppr3 = sTemp;
			} else {
				sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue - 100) << 12);
				this->m_sAppr3 = sTemp;
				sTemp = this->m_sAppr4;
				sTemp = sTemp | 0x080;
				this->m_sAppr4 = sTemp;
			}
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFF0FFFFF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 20);
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_ARMS:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0xFFF0;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue));
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFFF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 12);
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_LHAND:
			sTemp = this->m_sAppr2;
			sTemp = sTemp & 0xFFF0;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue));
			this->m_sAppr2 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xF0FFFFFF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 24);
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_RHAND:
			sTemp = this->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 4);
			this->m_sAppr2 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 28);
			this->m_iApprColor = iTemp;
			iTemp = this->m_iStatus;
			iTemp = iTemp & 0xFFFFFFF0;
			sSpeed = (this->m_pItemList[sItemIndex]->m_cSpeed);
			sSpeed -= ((this->m_iStr + this->m_iAngelicStr) / 13);
			if (sSpeed < 0) sSpeed = 0;
			iTemp = iTemp | (int) sSpeed;
			this->m_iStatus = iTemp;
			this->m_iComboAttackCount = 0;
			break;
		case DEF_EQUIPPOS_TWOHAND:
			sTemp = this->m_sAppr2;
			sTemp = sTemp & 0xF00F;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 4);
			this->m_sAppr2 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFFFFFF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 28);
			this->m_iApprColor = iTemp;
			iTemp = this->m_iStatus;
			iTemp = iTemp & 0xFFFFFFF0;
			sSpeed = (this->m_pItemList[sItemIndex]->m_cSpeed);
			sSpeed -= ((this->m_iStr + this->m_iAngelicStr) / 13);
			if (sSpeed < 0) sSpeed = 0;
			iTemp = iTemp | (int) sSpeed;
			this->m_iStatus = iTemp;
			this->m_iComboAttackCount = 0;
			break;
		case DEF_EQUIPPOS_BACK:
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0xF0FF;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 8);
			this->m_sAppr4 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			iTemp = iTemp | ((this->m_pItemList[sItemIndex]->m_cItemColor) << 16);
			this->m_iApprColor = iTemp;
			break;
		case DEF_EQUIPPOS_RELEASEALL:
			sTemp = this->m_sAppr3;
			sTemp = sTemp & 0x0FFF;
			sTemp = sTemp | ((this->m_pItemList[sItemIndex]->m_cApprValue) << 12);
			this->m_sAppr3 = sTemp;
			iTemp = this->m_iApprColor;
			iTemp = iTemp & 0xFFF0FFFF;
			this->m_iApprColor = iTemp;
			break;
	}
	if (this->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) {
		this->m_sAppr4 = this->m_sAppr4 & 0xFFF3;
		switch (this->m_pItemList[sItemIndex]->m_sSpecialEffect) {
			case 0: break;
			case 1:
				this->m_sAppr4 = this->m_sAppr4 | 0x0004;
				break;
			case 2:
				this->m_sAppr4 = this->m_sAppr4 | 0x000C;
				break;
			case 3:
				this->m_sAppr4 = this->m_sAppr4 | 0x0008;
				break;
		}
	}
	if (this->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY) {
		this->m_sAppr4 = this->m_sAppr4 & 0xFFFC;
		switch (this->m_pItemList[sItemIndex]->m_sSpecialEffect) {
			case 0:
				break;
			case 50:
			case 51:
			case 52:
				this->m_sAppr4 = this->m_sAppr4 | 0x0002;
				break;
			default:
				if (this->m_iAdminUserLevel > 0)
					this->m_sAppr4 = this->m_sAppr4 | 0x0001;
				// m_sAppr4
				// 0x0001 GM
				// 0x0002 Green
				// 0x0003 ice element
				// 0x0004 sparkle
				// 0x0005 sparkle green gm
				// 0x0006 sparkle green
				break;
		}
	}
	cHeroArmorType = this->_cCheckHeroItemEquipped();
	if (cHeroArmorType != 0x0FFFFFFFF) this->m_cHeroArmourBonus = cHeroArmorType;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	this->CalcTotalItemEffect(sItemIndex, bNotify);
	return true;
}

int CClient::iClientMotion_Stop_Handler(short sX, short sY, char cDir) {
	char cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	short sOwnerH;
	char cOwnerType;
	if (this->markedForDeletion_) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (this->m_bIsKilled == true) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if ((sX != this->m_sX) || (sY != this->m_sY)) return 2;
	if (this->m_bSkillUsingStatus[19] == true) {
		this->map_->GetOwner(&sOwnerH, &cOwnerType, sX, sY);
		if (sOwnerH != 0) {
			this->DeleteClient(true, true);
			return 0;
		}
	}
	this->ClearSkillUsingStatus();
	int iStX;
	int iStY;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;
		switch (this->m_cSide) {
			case 0: this->map_->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++;
				break;
			case 1: this->map_->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++;
				break;
			case 2: this->map_->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;
				break;
		}
	}
	this->m_cDir = cDir;
	this->map_->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	this->map_->this->SetOwner(DEF_OWNERTYPE_PLAYER, sX, sY);
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;
	iRet = this->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			return 0;
	}
	return 1;
}

void CClient::RequestCreateNewGuildHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cGuildName[21], cTxt[120], cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	SYSTEMTIME SysTime;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (m_bIsCrusadeMode == true) return;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	cp += 10;
	cp += 10;
	cp += 10;
	std::memset(cGuildName, 0, sizeof (cGuildName));
	memcpy(cGuildName, cp, 20);
	cp += 20;
	if (this->m_iGuildRank != -1) {
		wsprintf(cTxt, "(!)Cannot create guild! Already guild member.: CharName(%s)", this->m_cCharName);
		PutLogList(cTxt);
	} else {
		if ((this->m_iLevel < 20) || (this->m_iCharisma < 20) ||
				  (memcmp(this->m_cLocation, "NONE", 4) == 0) ||
				  (memcmp(this->m_cLocation, this->map_->m_cLocationName, 10) != 0)) { // v1.4
			std::memset(cData, 0, sizeof (cData));
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_CREATENEWGUILD;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_REJECT;
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					return;
			}
		} else {
			std::memset(this->m_cGuildName, 0, sizeof (this->m_cGuildName));
			strcpy(this->m_cGuildName, cGuildName);
			std::memset(this->m_cLocation, 0, sizeof (this->m_cLocation));
			strcpy(this->m_cLocation, this->map_->m_cLocationName);
			GetLocalTime(&SysTime);
			this->m_iGuildGUID = (int) (SysTime.wYear + SysTime.wMonth + SysTime.wDay + SysTime.wHour + SysTime.wMinute + timeGetTime());
			bSendMsgToLS(MSGID_REQUEST_CREATENEWGUILD, iClientH);
		}
	}
}

void CClient::RequestDisbandGuildHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cGuildName[21], cTxt[120];
	if (m_bIsCrusadeMode == true) return;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cGuildName, 0, sizeof (cGuildName));
	cp += 10;
	cp += 10;
	cp += 10;
	memcpy(cGuildName, cp, 20);
	cp += 20;
	if ((this->m_iGuildRank != 0) || (memcmp(this->m_cGuildName, cGuildName, 20) != 0)) {
		wsprintf(cTxt, "(!)Cannot Disband guild! Not guildmaster.: CharName(%s)", this->m_cCharName);
		PutLogList(cTxt);
	} else {
		bSendMsgToLS(MSGID_REQUEST_DISBANDGUILD, iClientH);
	}
}

void CClient::RequestStudyMagicHandler(char * pName, bool bIsPurchase) {
	char * cp, cMagicName[31], cData[100];
	uint32_t * dwp, dwGoldCount;
	uint16_t * wp;
	int * ip, iReqInt, iCost, iRet;
	bool bMagic = true;
	int iTempInt;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;

	std::memset(cData, 0, sizeof (cData));
	std::memset(cMagicName, 0, sizeof (cMagicName));
	memcpy(cMagicName, pName, 30);
	iRet = _iGetMagicNumber(cMagicName, &iReqInt, &iCost);
	if (iRet == -1) {
	} else {
		if (bIsPurchase == true) {
			if (m_pMagicConfigList[iRet]->m_iGoldCost < 0) bMagic = false;
			dwGoldCount = this->dwGetItemCount("Gold");
			if ((uint32_t) iCost > dwGoldCount) bMagic = false;
		}
		if (this->m_cMagicMastery[iRet] != 0) return;
		iTempInt = this->m_iInt + this->m_iAngelicInt;
		if ((iReqInt <= iTempInt) && (bMagic == true)) {

			if (bIsPurchase == true) this->SetItemCount("Gold", dwGoldCount - iCost);

			this->iCalcTotalWeight();

			this->m_cMagicMastery[iRet] = 1;

			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_MAGICSTUDYSUCCESS;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

			*cp = iRet;
			cp++;
			memcpy(cp, cMagicName, 30);
			cp += 30;

			iRet = this->m_pXSock->iSendMsg(cData, 37);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					this->DeleteClient(true, true);
					return;
			}
		} else {


			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_MAGICSTUDYFAIL;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

			*cp = 1;
			cp++;

			*cp = iRet;
			cp++;
			memcpy(cp, cMagicName, 30);
			cp += 30;
			ip = (int *) cp;
			*ip = iCost;
			cp += 4;
			ip = (int *) cp;
			*ip = iReqInt;
			cp += 4;

			iRet = this->m_pXSock->iSendMsg(cData, 46);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:

					this->DeleteClient(true, true);
					return;
			}
		}
	}
}

void CClient::TrainSkillResponse(bool bSuccess, int iSkillNum, int iSkillLevel) {
	char * cp, cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	int iRet;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if ((iSkillNum < 0) || (iSkillNum > 100)) return;
	if ((iSkillLevel < 0) || (iSkillLevel > 100)) return;
	if (bSuccess == true) {

		if (this->m_cSkillMastery[iSkillNum] != 0) return;
		this->m_cSkillMastery[iSkillNum] = iSkillLevel;

		this->bCheckTotalSkillMasteryPoints(iSkillNum);

		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_SKILLTRAINSUCCESS;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);

		*cp = iSkillNum;
		cp++;

		*cp = iSkillLevel;
		cp++;
		//wsprintf(G_cTxt, "Skill Improve: (%s) increased (%s) at %s (%d,%d).", this->m_cCharName, m_pSkillConfigList[iSkillNum]->m_cName, this->map_->m_cName, this->m_sX, this->m_sY);
		//PutLogFileList(G_cTxt);
		if (m_pSkillConfigList[iSkillNum]->m_cName != nullptr)
			_bItemLog(DEF_ITEMLOG_SKILLLEARN, iClientH, m_pSkillConfigList[iSkillNum]->m_cName, nullptr);

		iRet = this->m_pXSock->iSendMsg(cData, 8);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:

				this->DeleteClient(true, true);
				return;
		}
	} else {
	}
	/*

	std::memset(cData, 0, sizeof(cData));
	std::memset(cSkillName, 0, sizeof(cSkillName));
	memcpy(cSkillName, pName, 20);
	iRet = _iGetSkillNumber(cSkillName);
	if (iRet == 0) {

	}
	else {

		if (1) {

			this->m_cSkillMastery[iRet]++;

			dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
	 *dwp = MSGID_NOTIFY;
			wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
	 *wp  = DEF_NOTIFY_SKILLTRAINSUCCESS;
			cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

	 *cp = this->m_cSkillMastery[iRet];
			cp++;

	 *cp = iRet;
			cp++;
			memcpy(cp, cSkillName, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 28);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:

				this->DeleteClient(true, true);
				return;
			}
		}
		else {


			dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
	 *dwp = MSGID_NOTIFY;
			wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
	 *wp  = DEF_NOTIFY_SKILLTRAINFAIL;
			cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

	 *cp = 1;
			cp++;

	 *cp = iRet;
			cp++;
			memcpy(cp, cSkillName, 20);
			cp += 20;

			iRet = this->m_pXSock->iSendMsg(cData, 28);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:

				this->DeleteClient(true, true);
				return;
			}
		}
	}
	 */
}

bool CClient::bPlayerItemToBank(short sItemIndex) {
	register int i, iIndex;
	if (this->markedForDeletion_) return false;
	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (this->m_pItemInBankList[i] == nullptr) {
			iIndex = i;
			this->ReleaseItemHandler(sItemIndex, true);
			std::swap(this->m_pItemInBankList[iIndex], this->m_pItemList[sItemIndex]);
			this->m_bIsItemEquipped[sItemIndex] = false;
			for (i = 1; i < DEF_MAXITEMS; i++)
				if ((this->m_pItemList[i - 1] == nullptr) && (this->m_pItemList[i] != nullptr)) {
					std::swap(this->m_pItemList[i - 1], this->m_pItemList[i]);
					this->m_bIsItemEquipped[i - 1] = this->m_bIsItemEquipped[i];
					this->m_pItemList[i] = nullptr;
					this->m_bIsItemEquipped[i] = false;
				}
			return true;
		}
	return false;
}

bool CClient::bBankItemToPlayer(short sItemIndex) {
	if (this->markedForDeletion_) return false;
	if (this->m_pItemInBankList[sItemIndex] == nullptr) return false;
	for (int i = 0; i < DEF_MAXITEMS; i++) {
		if (this->m_pItemList[i] == nullptr) {
			std::swap(this->m_pItemList[i], this->m_pItemInBankList[sItemIndex]);
			return true;
		}
	}
	return false;
}

bool CClient::bCheckLevelUp() {
	if (this->markedForDeletion_) return false;
	if (this->m_iLevel >= m_iPlayerMaxLevel) {
		if (this->m_iExp >= m_iLevelExpTable[m_iPlayerMaxLevel + 1]) {
			this->m_iExp = m_iLevelExpTable[m_iPlayerMaxLevel];
			this->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
			this->m_iGizonItemUpgradeLeft++;
			if (this->m_iGizonItemUpgradeLeft > DEF_MAXGIZONPOINT) this->m_iGizonItemUpgradeLeft = DEF_MAXGIZONPOINT; // adamas
			this->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, this->m_iGizonItemUpgradeLeft, 1, 0, nullptr);
		}
		return false;
	}
	/*	if ((this->m_iLevel > DEF_LIMITHUNTERLEVEL - 1 ) && (this->m_bIsHunter == true ) )
		{
			this->SetNoHunterMode(true) ;
	 */
	if (this->m_iExp < this->m_iNextLevelExp) return false;
	while (this->m_iExp > this->m_iNextLevelExp
			  && this->m_iLevel < m_iPlayerMaxLevel) {
		if (this->m_iExp >= this->m_iNextLevelExp) {
			this->m_iLevel++;
			this->m_iLU_Pool += 3; // v2.23
			if (this->m_iStr > DEF_CHARPOINTLIMIT) this->m_iStr = DEF_CHARPOINTLIMIT;
			if (this->m_iDex > DEF_CHARPOINTLIMIT) this->m_iDex = DEF_CHARPOINTLIMIT;
			if (this->m_iVit > DEF_CHARPOINTLIMIT) this->m_iVit = DEF_CHARPOINTLIMIT;
			if (this->m_iInt > DEF_CHARPOINTLIMIT) this->m_iInt = DEF_CHARPOINTLIMIT;
			if (this->m_iMag > DEF_CHARPOINTLIMIT) this->m_iMag = DEF_CHARPOINTLIMIT;
			if (this->m_iCharisma > DEF_CHARPOINTLIMIT) this->m_iCharisma = DEF_CHARPOINTLIMIT;
			this->SendNotifyMsg(0, DEF_NOTIFY_LEVELUP, 0, 0, 0, nullptr);
			this->m_iNextLevelExp = m_iLevelExpTable[this->m_iLevel + 1]; //iGetLevelExp(this->m_iLevel + 1);
			this->CalcTotalItemEffect(-1, false);
			if (this->m_iLevel >= DEF_PLAYERMAXLEVEL) {
				wsprintf(G_cTxt, "(!) Character (%s) is over max level", this->m_cCharName);
				PutLogFileList(G_cTxt);
			}
		} else return true;
	}
	return false;
}

void CClient::StateChangeHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cStateChange1, cStateChange2, cStateChange3;
	char cStr;
	char cVit;
	char cDex;
	char cInt;
	char cMag;
	char cChar;
	int iOldStr;
	int iOldVit;
	int iOldDex;
	int iOldInt;
	int iOldMag;
	int iOldChar;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iGizonItemUpgradeLeft <= 0) return;
	cStr = cVit = cDex = cInt = cMag = cChar = 0;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	cStateChange1 = *cp;
	cp++;
	cStateChange2 = *cp;
	cp++;
	cStateChange3 = *cp;
	cp++;
	iOldStr = this->m_iStr;
	iOldVit = this->m_iVit;
	iOldDex = this->m_iDex;
	iOldInt = this->m_iInt;
	iOldMag = this->m_iMag;
	iOldChar = this->m_iCharisma;
	if (!bChangeState(cStateChange1, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (!bChangeState(cStateChange2, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (!bChangeState(cStateChange3, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (this->m_iGuildRank == 0) {
		if (this->m_iCharisma - cChar < 20) {
			this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
			return;
		}
	}
	if (iOldStr + iOldVit + iOldDex + iOldInt + iOldMag + iOldChar != (179 * 3 + 70)) {
		return;
	}
	if ((cStr < 0 || cVit < 0 || cDex < 0 || cInt < 0 || cMag < 0 || cChar < 0) ||
			  (cStr + cVit + cDex + cInt + cMag + cChar != 3)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if ((this->m_iStr - cStr > DEF_CHARPOINTLIMIT)
			  || (this->m_iStr - cStr < 10)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if ((this->m_iDex - cDex > DEF_CHARPOINTLIMIT)
			  || (this->m_iDex - cDex < 10)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if ((this->m_iInt - cInt > DEF_CHARPOINTLIMIT)
			  || (this->m_iInt - cInt < 10)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if ((this->m_iVit - cVit > DEF_CHARPOINTLIMIT)
			  || (this->m_iVit - cVit < 10)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if ((this->m_iMag - cMag > DEF_CHARPOINTLIMIT)
			  || (this->m_iMag - cMag < 10)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if ((this->m_iCharisma - cChar > DEF_CHARPOINTLIMIT)
			  || (this->m_iCharisma - cChar < 10)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (this->m_iLU_Pool < 3) this->m_iLU_Pool = 3;
	this->m_iGizonItemUpgradeLeft--;
	this->m_iStr -= cStr;
	this->m_iVit -= cVit;
	this->m_iDex -= cDex;
	this->m_iInt -= cInt;
	this->m_iMag -= cMag;
	this->m_iCharisma -= cChar;
	if (cInt > 0) {
		this->bCheckMagicInt();
	}
	//Re-Calculate HP,MP,SP
	if (this->m_iHP > this->iGetMaxHP()) this->m_iHP = this->iGetMaxHP();
	if (this->m_iMP > this->iGetMaxMP()) this->m_iMP = this->iGetMaxMP();
	if (this->m_iSP > this->iGetMaxSP()) this->m_iSP = this->iGetMaxSP();
	this->SendNotifyMsg(0, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, nullptr);
}

bool CClient::bCheckMagicInt() {
	for (int i = 0; i < DEF_MAXMAGICTYPE; i++) {
		if (m_pMagicConfigList[i] != nullptr)
			if (m_pMagicConfigList[i]->m_sIntLimit > (this->m_iInt + this->m_iAngelicInt)) {
				this->m_cMagicMastery[i] = 0;
			}
	}
	return true;
}

void CClient::LevelUpSettingsHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char * cp;
	int * ip;
	int iStr;
	int iVit;
	int iDex;
	int iInt;
	int iMag;
	int iChar;
	int iTotalSetting = 0;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iLU_Pool <= 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	ip = (int *) cp;
	iStr = (int) *ip;
	cp += 4;
	ip = (int *) cp;
	iVit = (int) *ip;
	cp += 4;
	ip = (int *) cp;
	iDex = (int) *ip;
	cp += 4;
	ip = (int *) cp;
	iInt = (int) *ip;
	cp += 4;
	ip = (int *) cp;
	iMag = (int) *ip;
	cp += 4;
	ip = (int *) cp;
	iChar = (int) *ip;
	cp += 4;
	if ((iStr + iVit + iDex + iInt + iMag + iChar) > this->m_iLU_Pool) { // -3
		this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}

	if ((this->m_iStr + iStr > DEF_CHARPOINTLIMIT) || (iStr < 0))
		return;
	if ((this->m_iDex + iDex > DEF_CHARPOINTLIMIT) || (iDex < 0))
		return;
	if ((this->m_iInt + iInt > DEF_CHARPOINTLIMIT) || (iInt < 0))
		return;
	if ((this->m_iVit + iVit > DEF_CHARPOINTLIMIT) || (iVit < 0))
		return;
	if ((this->m_iMag + iMag > DEF_CHARPOINTLIMIT) || (iMag < 0))
		return;
	if ((this->m_iCharisma + iChar > DEF_CHARPOINTLIMIT) || (iChar < 0))
		return;
	iTotalSetting = this->m_iStr + this->m_iDex + this->m_iVit +
			  this->m_iInt + this->m_iMag + this->m_iCharisma;
	if (iTotalSetting + this->m_iLU_Pool - 3 > ((this->m_iLevel - 1)*3 + 70)) {
		this->m_iLU_Pool = 3 + (this->m_iLevel - 1)*3 + 70 - iTotalSetting;
		if (this->m_iLU_Pool < 3)
			this->m_iLU_Pool = 3;
		this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}
	if (iTotalSetting + (iStr + iVit + iDex + iInt + iMag + iChar)
			  > ((this->m_iLevel - 1)*3 + 70)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, nullptr);
		return;
	}
	this->m_iLU_Pool = this->m_iLU_Pool - (iStr + iVit + iDex + iInt + iMag + iChar);
	this->m_iStr += iStr;
	this->m_iVit += iVit;
	this->m_iDex += iDex;
	this->m_iInt += iInt;
	this->m_iMag += iMag;
	this->m_iCharisma += iChar;
	this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
}

void CClient::FightzoneReserveHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char cData[100];
	int iFightzoneNum, * ip, iEnableReserveTime;
	uint32_t * dwp, dwGoldCount;
	uint16_t * wp, wResult;
	int iRet;
	int iResult = 1, iCannotReserveDay;
	SYSTEMTIME SysTime;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	GetLocalTime(&SysTime);
	iEnableReserveTime = 2 * 20 * 60 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 5 * 20;
	dwGoldCount = this->dwGetItemCount("Gold");
	ip = (int *) (pData + DEF_INDEX2_MSGTYPE + 2);
	iFightzoneNum = *ip;
	if ((iFightzoneNum < 1) || (iFightzoneNum > DEF_MAXFIGHTZONE)) return;
	iCannotReserveDay = (SysTime.wDay + this->m_cSide + iFightzoneNum) % 2;
	if (iEnableReserveTime <= 0) {
		wResult = DEF_MSGTYPE_REJECT;
		iResult = 0;
	} else if (m_iFightZoneReserve[iFightzoneNum - 1] != 0) {
		wResult = DEF_MSGTYPE_REJECT;
		iResult = -1;
	} else if (dwGoldCount < 1500) {
		wResult = DEF_MSGTYPE_REJECT;
		iResult = -2;
	} else if (iCannotReserveDay) {
		wResult = DEF_MSGTYPE_REJECT;
		iResult = -3;
	} else if (this-> m_iFightzoneNumber != 0) {
		wResult = DEF_MSGTYPE_REJECT;
		iResult = -4;
	} else {
		wResult = DEF_MSGTYPE_CONFIRM;
		this->SetItemCount("Gold", dwGoldCount - 1500);
		this->iCalcTotalWeight();
		m_iFightZoneReserve[iFightzoneNum - 1] = iClientH;
		this->m_iFightzoneNumber = iFightzoneNum;
		this->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + SysTime.wHour;
		if (SysTime.wHour % 2) this->m_iReserveTime += 1;
		else this->m_iReserveTime += 2;
		wsprintf(G_cTxt, "(*) Reserve FIGHTZONETICKET : Char(%s) TICKENUMBER (%d)", this->m_cCharName, this->m_iReserveTime);
		PutLogFileList(G_cTxt);
		PutLogList(G_cTxt);
		this->m_iFightZoneTicketNumber = 50;
		iResult = 1;
	}
	std::memset(cData, 0, sizeof (cData));
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_FIGHTZONE_RESERVE;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wResult;
	ip = (int *) (cData + DEF_INDEX2_MSGTYPE + 2);
	*ip = iResult;
	ip += 4;
	iRet = this->m_pXSock->iSendMsg(cData, 10);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			return;
	}
}

bool CClient::bCheckLimitedUser() {
	if (this->markedForDeletion_) return false;
	if ((memcmp(this->m_cLocation, "NONE", 4) == 0) &&
			  (this->m_iExp >= m_iLevelExp20)) {
		this->m_iExp = m_iLevelExp20 - 1;
		this->SendNotifyMsg(0, DEF_NOTIFY_TRAVELERLIMITEDLEVEL, 0, 0, 0, nullptr);
		return true;
	}
	return false;
}

void CClient::RequestCivilRightHandler(char */*pData*/) {
	if (this->m_bIsInitComplete == false) return;
	if ((m_bAdminSecurity == true) && (this->m_iAdminUserLevel > 0)) return;
	uint16_t wResult = 1;
	if (memcmp(this->m_cLocation, "NONE", 4) != 0) wResult = 0;
	if (this->m_iLevel < 5) wResult = 0;
	if (wResult == 1) {
		std::memset(this->m_cLocation, 0, sizeof (this->m_cLocation));
		strcpy(this->m_cLocation, this->map_->m_cLocationName);
	}
	if (memcmp(this->m_cLocation, "are", 3) == 0)
		this->m_cSide = 1;
	if (memcmp(this->m_cLocation, "elv", 3) == 0)
		this->m_cSide = 2;
	char cData[100];
	uint32_t *dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_CIVILRIGHT;
	uint16_t *wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wResult;
	char *cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cp, this->m_cLocation, 10);
	cp += 10;
	int iRet = this->m_pXSock->iSendMsg(cData, 16);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[this->id_]->DeleteClient(true, true);
			return;
	}
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	RequestChangePlayMode(client);
}

void CClient::RequestRetrieveItemHandler(char *pData) {
	char * cp, cBankItemIndex, cMsg[100];
	register int i, j, iRet, iItemWeight;
	uint32_t * dwp;
	uint16_t * wp;
	if (this->m_bIsInitComplete == false) return;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	cBankItemIndex = *cp;
	if ((cBankItemIndex < 0) || (cBankItemIndex >= DEF_MAXBANKITEMS)) return;
	if (this->m_pItemInBankList[cBankItemIndex] == nullptr) {
		std::memset(cMsg, 0, sizeof (cMsg));
		dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_RETRIEVEITEM;
		wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
		*wp = DEF_MSGTYPE_REJECT;
		iRet = this->m_pXSock->iSendMsg(cMsg, 8);
	} else {
		/*
		if ( (this->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			 (this->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) ) {
			//iItemWeight = this->m_pItemInBankList[cBankItemIndex]->m_wWeight * this->m_pItemInBankList[cBankItemIndex]->m_dwCount;
			iItemWeight = iGetItemWeight(this->m_pItemInBankList[cBankItemIndex], this->m_pItemInBankList[cBankItemIndex]->m_dwCount);
		}
		else iItemWeight = iGetItemWeight(this->m_pItemInBankList[cBankItemIndex], 1); //this->m_pItemInBankList[cBankItemIndex]->m_wWeight;
		 */
		iItemWeight = iGetItemWeight(*this->m_pItemInBankList[cBankItemIndex], this->m_pItemInBankList[cBankItemIndex]->m_dwCount);
		if ((iItemWeight + this->m_iCurWeightLoad) > this->_iCalcMaxLoad()) {
			std::memset(cMsg, 0, sizeof (cMsg));
			dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = this->m_pXSock->iSendMsg(cMsg, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					m_pClientList[this->id_]->DeleteClient(true, true);
					break;
			}
			return;
		}
		if ((this->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
				  (this->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
			for (i = 0; i < DEF_MAXITEMS; i++)
				if ((this->m_pItemList[i] != nullptr) &&
						  (this->m_pItemList[i]->m_cItemType == this->m_pItemInBankList[cBankItemIndex]->m_cItemType) &&
						  (memcmp(this->m_pItemList[i]->m_cName, this->m_pItemInBankList[cBankItemIndex]->m_cName, 20) == 0)) {
					SetItemCount(client, i, this->m_pItemList[i]->m_dwCount + this->m_pItemInBankList[cBankItemIndex]->m_dwCount);
					this->m_pItemInBankList[cBankItemIndex].reset();
					for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++) {
						if ((this->m_pItemInBankList[j + 1] != nullptr) && (this->m_pItemInBankList[j] == nullptr)) {
							std::swap(this->m_pItemInBankList[j], this->m_pItemInBankList[j + 1]);
						}
					}
					std::memset(cMsg, 0, sizeof (cMsg));
					dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
					*dwp = MSGID_RESPONSE_RETRIEVEITEM;
					wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
					*wp = DEF_MSGTYPE_CONFIRM;
					cp = (char *) (cMsg + DEF_INDEX2_MSGTYPE + 2);
					*cp = cBankItemIndex;
					cp++;
					*cp = i;
					cp++;
					this->iCalcTotalWeight();
					this->m_cArrowIndex = _iGetArrowItemIndex(client);
					iRet = this->m_pXSock->iSendMsg(cMsg, 8);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							m_pClientList[this->id_]->DeleteClient(true, true);
							return;
					}
					return;
				}
			goto RRIH_NOQUANTITY;
		} else {
RRIH_NOQUANTITY:
			;
			for (i = 0; i < DEF_MAXITEMS; i++) {
				if (this->m_pItemList[i] == nullptr) {
					std::swap(this->m_pItemList[i], this->m_pItemInBankList[cBankItemIndex]);
					this->m_ItemPosList[i].x = 40;
					this->m_ItemPosList[i].y = 30;
					this->m_bIsItemEquipped[i] = false;
					for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++) {
						if ((this->m_pItemInBankList[j + 1] != nullptr) && (this->m_pItemInBankList[j] == nullptr)) {
							std::swap(this->m_pItemInBankList[j], this->m_pItemInBankList[j + 1]);
						}
					}
					std::memset(cMsg, 0, sizeof (cMsg));
					dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
					*dwp = MSGID_RESPONSE_RETRIEVEITEM;
					wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
					*wp = DEF_MSGTYPE_CONFIRM;
					cp = (char *) (cMsg + DEF_INDEX2_MSGTYPE + 2);
					*cp = cBankItemIndex;
					cp++;
					*cp = i;
					cp++;
					this->iCalcTotalWeight();
					this->m_cArrowIndex = _iGetArrowItemIndex(client);
					iRet = this->m_pXSock->iSendMsg(cMsg, 8);
					switch (iRet) {
						case DEF_XSOCKEVENT_QUENEFULL:
						case DEF_XSOCKEVENT_SOCKETERROR:
						case DEF_XSOCKEVENT_CRITICALERROR:
						case DEF_XSOCKEVENT_SOCKETCLOSED:
							m_pClientList[this->id_]->DeleteClient(true, true);
							return;
					}
					return;
				}
			}
			std::memset(cMsg, 0, sizeof (cMsg));
			dwp = (uint32_t *) (cMsg + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_RETRIEVEITEM;
			wp = (uint16_t *) (cMsg + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_REJECT;
			iRet = this->m_pXSock->iSendMsg(cMsg, 8);
		}
	}
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[this->id_]->DeleteClient(true, true);
			return;
	}
}

void CClient::EnemyKillRewardHandler(int iAttackerH,) {
	// enemy-kill-mode = 1 | 0
	// if m_bEnemyKillMode is true than death match mode
	// DEATHMATCH MODE:
	// Aresden kills Elvine in Aresden and gets EK
	// Elvine kills Aresden in Aresden and gets an EK
	// Elvine kills Aresden in Elvine and gets an EK
	// Aresden kills Elvine in Elvine and gets an EK
	// CLASSIC MODE:
	// Aresden kills Elvine in Aresden and gets EK
	// Elvine kills Aresden in Aresden and doesnt get an EK
	// Elvine kills Aresden in Elvine and gets an EK
	// Aresden kills Elvine in Elvine and doesnt get an EK
	int iRewardExp;
	int iEK_Level;
	// if the attacker doesnt exist no result
	if (m_pClientList[iAttackerH] == nullptr) return;
	// if the target doesnt exist no result
	if (this->markedForDeletion_) return;
	// Log the EK (killed by enemy)
	_bPKLog(DEF_PKLOG_BYENERMY, iAttackerH, iClientH, nullptr);
	// set EK level to 30
	iEK_Level = 30;
	// if attacker's level is greater than 80, set ek level to 80
	if (m_pClientList[iAttackerH]->m_iLevel >= 80) iEK_Level = 80;
	// check if attacker level is less than or equal to max level
	if (m_pClientList[iAttackerH]->m_iLevel >= m_iPlayerMaxLevel) {
		// if the
		if (iGetExpLevel(this->m_iExp) >= iEK_Level) {
			if ((memcmp(this->m_cLocation, this->map_->m_cName, 10) != 0)
					  && (m_bEnemyKillMode == false)) {
				//Happy Hour para vos
				if (m_bHappyHour == true) {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
				} else {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
				}
			}
			if (m_bEnemyKillMode == true) {
				//Happy Hour para vos
				if (m_bHappyHour == true) {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
				} else {
					m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
				}
			}
		}
		m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(this->m_iExp)));
		if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
			m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
		if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
			m_pClientList[iAttackerH]->m_iRewardGold = 0;
		m_pClientList[iAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, nullptr);
		return;
	}
	if (m_pClientList[iAttackerH]->m_iPKCount != 0) {
	} else {
		if (this->m_iGuildRank == -1) {
			iRewardExp = (iDice(3, (3 * iGetExpLevel(this->m_iExp))) + iGetExpLevel(this->m_iExp)) / 3;
			if (m_bIsCrusadeMode == true) {
				m_pClientList[iAttackerH]->m_iExp += (iRewardExp / 3)*4;
				m_pClientList[iAttackerH]->m_iWarContribution += (iRewardExp - (iRewardExp / 3))*12;
				if (m_pClientList[iAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
					m_pClientList[iAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
				m_pClientList[iAttackerH]->m_iConstructionPoint += this->m_iLevel / 2;
				if (m_pClientList[iAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
					m_pClientList[iAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
				//testcode
				wsprintf(G_cTxt, "Enemy Player Killed by Player! Construction: +%d WarContribution +%d", this->m_iLevel / 2, (iRewardExp - (iRewardExp / 3))*6);
				PutLogList(G_cTxt);
				m_pClientList[iAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iAttackerH]->m_iConstructionPoint, m_pClientList[iAttackerH]->m_iWarContribution, 0, nullptr);
				if (iGetExpLevel(this->m_iExp) >= iEK_Level) {
					if (memcmp(this->m_cLocation, this->map_->m_cName, 10) != 0) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
					if (m_bEnemyKillMode == true) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}
				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(this->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			} else {
				m_pClientList[iAttackerH]->m_iExp += iRewardExp;
				if (iGetExpLevel(this->m_iExp) >= iEK_Level) {
					if ((memcmp(this->m_cLocation, this->map_->m_cName, 10) != 0)
							  && (m_bEnemyKillMode == false)) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
					if (m_bEnemyKillMode == true) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}
				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(this->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			}
		} else {
			iRewardExp = (iDice(3, (3 * iGetExpLevel(this->m_iExp))) + iGetExpLevel(this->m_iExp)) / 3;
			if (m_bIsCrusadeMode == true) {
				m_pClientList[iAttackerH]->m_iExp += (iRewardExp / 3)*4;
				m_pClientList[iAttackerH]->m_iWarContribution += (iRewardExp - (iRewardExp / 3))*12;
				if (m_pClientList[iAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
					m_pClientList[iAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
				m_pClientList[iAttackerH]->m_iConstructionPoint += this->m_iLevel / 2;
				if (m_pClientList[iAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
					m_pClientList[iAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
				//testcode
				wsprintf(G_cTxt, "Enemy Player Killed by Player! Construction: +%d WarContribution +%d", this->m_iLevel / 2, (iRewardExp - (iRewardExp / 3))*6);
				PutLogList(G_cTxt);
				m_pClientList[iAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iAttackerH]->m_iConstructionPoint, m_pClientList[iAttackerH]->m_iWarContribution, 0, nullptr);
				if (iGetExpLevel(this->m_iExp) >= iEK_Level) {
					if ((memcmp(this->m_cLocation, this->map_->m_cName, 10) != 0)
							  && (m_bEnemyKillMode == false)) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
					if (m_bEnemyKillMode == true) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}
				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(this->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			} else {
				m_pClientList[iAttackerH]->m_iExp += iRewardExp;
				if (iGetExpLevel(this->m_iExp) >= iEK_Level) {
					if ((memcmp(this->m_cLocation, this->map_->m_cName, 10) != 0)
							  && (m_bEnemyKillMode == false)) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
					if (m_bEnemyKillMode == true) {
						//Happy Hour para vos
						if (m_bHappyHour == true) {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust * 2;
						} else {
							m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
						}
					}
				}
				m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(this->m_iExp)));
				if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
					m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
				if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
					m_pClientList[iAttackerH]->m_iRewardGold = 0;
			}
		}
		m_pClientList[iAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, nullptr);
		if (bCheckLimitedUser(iAttackerH) == false) {
			m_pClientList[iAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
		}
		bCheckLevelUp(iAttackerH);
		m_stCityStatus[m_pClientList[iAttackerH]->m_cSide].iWins++;
	}
}

void CClient::ApplyCombatKilledPenalty(int cPenaltyLevel, bool bIsSAattacked) {
	int iExp;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	// Crusade
	if (m_bIsCrusadeMode == true) {
		if (this->m_iPKCount > 0) {
			this->m_iPKCount--;
			this->SendNotifyMsg(0, DEF_NOTIFY_PKPENALTY, 0, 0, 0, nullptr);
			// v2.15
			_bPKLog(DEF_PKLOG_REDUCECRIMINAL, 0, iClientH, nullptr);
		}
		return;
	} else {
		if (this->m_iPKCount > 0) {
			this->m_iPKCount--;
			this->SendNotifyMsg(0, DEF_NOTIFY_PKPENALTY, 0, 0, 0, nullptr);
			// v2.15
			_bPKLog(DEF_PKLOG_REDUCECRIMINAL, 0, iClientH, nullptr);
		}
		iExp = iDice(1, (5 * cPenaltyLevel * this->m_iLevel));
		if (this->m_bIsNeutral == true) iExp = iExp / 3;
		// if (this->m_iLevel == DEF_PLAYERMAXLEVEL) iExp = 0;
		this->m_iExp -= iExp;
		if (this->m_iExp < 0) this->m_iExp = 0;
		this->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
		if (this->m_bIsNeutral != true) {
			if (this->m_iLevel < 80) {
				// v2.03 60 -> 80
				cPenaltyLevel--;
				if (cPenaltyLevel <= 0) cPenaltyLevel = 1;
				this->_PenaltyItemDrop(cPenaltyLevel, bIsSAattacked);
			} else this->_PenaltyItemDrop(cPenaltyLevel, bIsSAattacked);
		}
	}
}

void CClient::_PenaltyItemDrop(int iTotal, bool bIsSAattacked) {
	register int i, j, iRemainItem;
	char cItemIndexList[DEF_MAXITEMS], cItemIndex;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if ((this->m_iAlterItemDropIndex != -1) && (this->m_pItemList[this->m_iAlterItemDropIndex] != nullptr)) {
		// Testcode
		if (this->m_pItemList[this->m_iAlterItemDropIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) {
			if (this->m_pItemList[this->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
				this->m_pItemList[this->m_iAlterItemDropIndex]->m_wCurLifeSpan--;
			this->DropItemHandler(this->m_iAlterItemDropIndex, -1, this->m_pItemList[this->m_iAlterItemDropIndex]->m_cName);
			this->m_iAlterItemDropIndex = -1;
		} else {
			// v2.04 testcode
			PutLogFileList("Alter Drop Item Index Error1");
			for (i = 0; i < DEF_MAXITEMS; i++)
				if ((this->m_pItemList[i] != nullptr) && (this->m_pItemList[i]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {
					this->m_iAlterItemDropIndex = i;
					if (this->m_pItemList[this->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
						this->m_pItemList[this->m_iAlterItemDropIndex]->m_wCurLifeSpan--;
					this->DropItemHandler(this->m_iAlterItemDropIndex, -1, this->m_pItemList[this->m_iAlterItemDropIndex]->m_cName);
					this->m_iAlterItemDropIndex = -1;
					return;
				}
			goto PID_DROP;
		}
		return;
	}
PID_DROP:
	;
	for (i = 1; i <= iTotal; i++) {
		iRemainItem = 0;
		std::memset(cItemIndexList, 0, sizeof (cItemIndexList));
		for (j = 0; j < DEF_MAXITEMS; j++)
			if (this->m_pItemList[j] != nullptr) {
				cItemIndexList[iRemainItem] = j;
				iRemainItem++;
			}
		if (iRemainItem == 0) return;
		cItemIndex = cItemIndexList[ iDice(1, iRemainItem) - 1 ];
		if ((this->m_pItemList[cItemIndex]->m_sTouchEffectType != 0) &&
				  (this->m_pItemList[cItemIndex]->m_sTouchEffectValue1 == this->m_sCharIDnum1) &&
				  (this->m_pItemList[cItemIndex]->m_sTouchEffectValue2 == this->m_sCharIDnum2) &&
				  (this->m_pItemList[cItemIndex]->m_sTouchEffectValue3 == this->m_sCharIDnum3)) {
		} else if (
				  (this->m_pItemList[cItemIndex]->m_sIDnum >= 400) &&
				  (this->m_pItemList[cItemIndex]->m_sIDnum != 402) &&
				  (this->m_pItemList[cItemIndex]->m_sIDnum <= 428)) {
		} else if (((this->m_pItemList[cItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
				  (this->m_pItemList[cItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) &&
				  (bIsSAattacked == false)) {
		} else if ((this->m_bIsLuckyEffect == true) && (iDice(1, 10) == 5)) {
		} else this->DropItemHandler(cItemIndex, -1, this->m_pItemList[cItemIndex]->m_cName);
	}
}

void CClient::GetRewardMoneyHandler() {
	int iRet;
	int iEraseReq;
	int iWeightLeft;
	int iRewardGoldLeft;
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cData[100], cItemName[21];
	class CItem * pItem;
	short * sp;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	iWeightLeft = this->_iCalcMaxLoad() - this->iCalcTotalWeight();
	if (iWeightLeft <= 0) return;
	iWeightLeft = iWeightLeft / 2;
	if (iWeightLeft <= 0) return;
	pItem = new class CItem;
	std::memset(cItemName, 0, sizeof (cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(*pItem, cItemName);
	//pItem->m_dwCount = this->m_iRewardGold;
	if ((iWeightLeft / iGetItemWeight(*pItem, 1)) >= this->m_iRewardGold) {
		pItem->m_dwCount = this->m_iRewardGold;
		iRewardGoldLeft = 0;
	} else {
		pItem->m_dwCount = (iWeightLeft / iGetItemWeight(*pItem, 1));
		iRewardGoldLeft = this->m_iRewardGold - (iWeightLeft / iGetItemWeight(*pItem, 1));
	}
	if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
		this->m_iRewardGold = iRewardGoldLeft;
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 1;
		cp++;
		memcpy(cp, pItem->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;
		*cp = pItem->m_cItemType;
		cp++;
		*cp = pItem->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;
		*cp = pItem->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;
		*cp = pItem->m_cItemColor;
		cp++;
		*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
		cp++;
		 */
		if (iEraseReq == 1) delete pItem;
		iRet = this->m_pXSock->iSendMsg(cData, 53);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
				this->DeleteClient(true, true);
				return;
		}
		this->SendNotifyMsg(0, DEF_NOTIFY_REWARDGOLD, 0, 0, 0, nullptr);
	} else {
	}
}

void CClient::RequestFullObjectData(char *pData) {
	uint32_t * dwp;
	uint16_t wObjectID;
	char * cp, cData[100];
	short * sp, sX, sY;
	int iTemp;
	int iTemp2;
	int * ip, iRet;
	if (this->m_bIsInitComplete == false) return;
	uint16_t *wp = (uint16_t *) (pData + DEF_INDEX2_MSGTYPE);
	wObjectID = *wp;
	std::memset(cData, 0, sizeof (cData));
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_EVENT_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTSTOP;
	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	if (wObjectID < 10000) {
		if ((wObjectID == 0) || (wObjectID >= DEF_MAXCLIENTS)) return;
		auto &targetClientPtr = m_pClientList[wObjectID];
		if (!targetClientPtr) return;
		auto &targetClient = *targetClientPtr;
		wp = (uint16_t *) cp;
		*wp = wObjectID;
		cp += 2;
		sp = (short *) cp;
		sX = targetClient.m_sX;
		*sp = sX;
		cp += 2;
		sp = (short *) cp;
		sY = targetClient.m_sY;
		*sp = sY;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sType;
		cp += 2;
		*cp = targetClient.m_cDir;
		cp++;
		memcpy(cp, targetClient.m_cCharName, 10);
		cp += 10;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr1;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr2;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr3;
		cp += 2;
		sp = (short *) cp;
		*sp = targetClient.m_sAppr4;
		cp += 2;
		ip = (int *) cp;
		*ip = targetClient.m_iApprColor;
		cp += 4;
		ip = (int *) cp;
		iTemp = targetClient.m_iStatus;
		iTemp = 0x0FFFFFFF & iTemp;
		iTemp2 = iGetPlayerABSStatus(wObjectID, this->id_);
		iTemp = (iTemp | (iTemp2 << 28));
		*ip = iTemp;
		cp += 4;
		*cp = targetClient.m_bIsKilled;
		cp++;
		iRet = this->m_pXSock->iSendMsg(cData, 41);
	} else {
		wObjectID -= 10000;
		if (((wObjectID) == 0) || ((wObjectID) >= DEF_MAXNPCS)) return;
		auto &targetNpcPtr = m_pNpcList[wObjectID];
		if (!targetNpcPtr) return;
		auto &targetNpc = *targetNpcPtr;
		wp = (uint16_t *) cp;
		*wp = wObjectID;
		cp += 2;
		sp = (short *) cp;
		sX = targetNpc.m_sX;
		*sp = sX;
		cp += 2;
		sp = (short *) cp;
		sY = targetNpc.m_sY;
		*sp = sY;
		cp += 2;
		sp = (short *) cp;
		*sp = targetNpc.m_sType;
		cp += 2;
		*cp = targetNpc.m_cDir;
		cp++;
		memcpy(cp, targetNpc.m_cName, 5);
		cp += 5;
		sp = (short *) cp;
		*sp = targetNpc.m_sAppr2;
		cp += 2;
		ip = (int *) cp;
		iTemp = targetNpc.m_iStatus;
		iTemp = 0x0FFFFFFF & iTemp;
		iTemp2 = iGetNpcRelationship(wObjectID, this->id_);
		iTemp = (iTemp | (iTemp2 << 28)); //Original : 12
		*ip = iTemp;
		//*sp = DEF_TEST;
		cp += 4; //Original 2
		*cp = targetNpc.m_bIsKilled;
		cp++;
		iRet = this->m_pXSock->iSendMsg(cData, 27); // v1.4 //Original : 25
	}
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[this->id_]->DeleteClient(true, true);
			return;
	}
}

int CClient::_iGetArrowItemIndex() {
	register int i;
	if (this->markedForDeletion_) return -1;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			if ((this->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_ARROW) &&
					  (this->m_pItemList[i]->m_dwCount > 0))
				return i;
		}
	return -1;
}

void CClient::UseItemHandler(short sItemIndex, short dX, short dY, short sDestItemID) {
	int iTemp;
	int iMax;
	int iV1;
	int iV2;
	int iV3;
	int iSEV1;
	int iEffectResult = 0;
	uint32_t dwTime;
	short sTemp;
	short sTmpType;
	short sTmpAppr1;
	char cSlateType[20];
	dwTime = timeGetTime();
	std::memset(cSlateType, 0, sizeof (cSlateType));
	//testcode
	//wsprintf(G_cTxt, "%d", sDestItemID);
	//PutLogList(G_cTxt);
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsInitComplete == false) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	auto &itemPtr = this->m_pItemList[sItemIndex];
	if (!itemPtr) return;
	CItem &item = *itemPtr;
	if ((item.m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
			  (item.m_cItemType == DEF_ITEMTYPE_USE_PERM) ||
			  (item.m_cItemType == DEF_ITEMTYPE_ARROW) ||
			  (item.m_cItemType == DEF_ITEMTYPE_EAT) ||
			  (item.m_cItemType == DEF_ITEMTYPE_USE_SKILL) ||
			  (item.m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST)) {
	} else return;
	if ((item.m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
			  (item.m_cItemType == DEF_ITEMTYPE_EAT)) {
		switch (item.m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_WARM:

				if (this->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 1) {
					//	this->SetIceFlag(DEF_OWNERTYPE_PLAYER, false);
					delayEvents_.remove(this->id_, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_ICE);

					delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (1 * 1000),
							  this->id_, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
					//				SendNotifyMsg(nullptr, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_ICE, nullptr, nullptr, nullptr);
				}
				this->m_dwWarmEffectTime = dwTime;
				break;
			case DEF_ITEMEFFECTTYPE_LOTTERY:
				iTemp = iDice(1, item.m_sItemSpecEffectValue1);
				if (iTemp == iDice(1, item.m_sItemSpecEffectValue1)) {
				} else {
					// ²Î!
				}
				break;
			case DEF_ITEMEFFECTTYPE_SLATES:
			{
				// Full Ancient Slate ??
				if (item.m_sIDnum == 867) {
					// Slates dont work on Heldenian Map
					switch (item.m_sItemSpecEffectValue2) {
						case 2: // Bezerk slate
							this->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] = true;
							SetBerserkFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
							delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (1000 * 600),
									  this->id_, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, 1, 0, nullptr);
							strcpy(cSlateType, "Berserk");
							break;
						case 1: // Invincible slate
							if (strlen(cSlateType) == 0) {
								strcpy(cSlateType, "Invincible");
							}
						case 3: // Mana slate
							if (strlen(cSlateType) == 0) {
								strcpy(cSlateType, "Mana");
							}
						case 4: // Exp slate
							if (strlen(cSlateType) == 0) {
								strcpy(cSlateType, "Exp");
							}
							SetSlateFlag(client, item.m_sItemSpecEffectValue2, true);
							delayEvents_.add(DelayEventType::ANCIENT_TABLET, item.m_sItemSpecEffectValue2,
									  dwTime + (1000 * 600), this->id_, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
							switch (item.m_sItemSpecEffectValue2) {
								case 1:
									iEffectResult = 4;
									break;
								case 3:
									iEffectResult = 5;
									break;
								case 4:
									iEffectResult = 6;
									break;
							}
					}
					if (strlen(cSlateType) > 0)
						_bItemLog(DEF_ITEMLOG_USE, client, strlen(cSlateType), &item);
				}
			}
				break;
			case DEF_ITEMEFFECTTYPE_HP:
				iMax = this->iGetMaxHP();
				if (this->m_iHP < iMax) {
					if (item.m_sItemSpecEffectValue1 == 0) {
						iV1 = item.m_sItemEffectValue1;
						iV2 = item.m_sItemEffectValue2;
						iV3 = item.m_sItemEffectValue3;
					} else {
						iV1 = item.m_sItemSpecEffectValue1;
						iV2 = item.m_sItemSpecEffectValue2;
						iV3 = item.m_sItemSpecEffectValue3;
					}
					this->m_iHP += (iDice(iV1, iV2) + iV3);
					if (this->m_iHP > iMax) this->m_iHP = iMax;
					if (this->m_iHP <= 0) this->m_iHP = 1;
					iEffectResult = 1;
				}
				break;
			case DEF_ITEMEFFECTTYPE_MP:
				iMax = this->iGetMaxMP();
				if (this->m_iMP < iMax) {
					if (item.m_sItemSpecEffectValue1 == 0) {
						iV1 = item.m_sItemEffectValue1;
						iV2 = item.m_sItemEffectValue2;
						iV3 = item.m_sItemEffectValue3;
					} else {
						iV1 = item.m_sItemSpecEffectValue1;
						iV2 = item.m_sItemSpecEffectValue2;
						iV3 = item.m_sItemSpecEffectValue3;
					}
					this->m_iMP += (iDice(iV1, iV2) + iV3);
					if (this->m_iMP > iMax)
						this->m_iMP = iMax;
					iEffectResult = 2;
				}
				break;
			case DEF_ITEMEFFECTTYPE_SP:
				iMax = this->iGetMaxSP();
				if (this->m_iSP < iMax) {
					if (item.m_sItemSpecEffectValue1 == 0) {
						iV1 = item.m_sItemEffectValue1;
						iV2 = item.m_sItemEffectValue2;
						iV3 = item.m_sItemEffectValue3;
					} else {
						iV1 = item.m_sItemSpecEffectValue1;
						iV2 = item.m_sItemSpecEffectValue2;
						iV3 = item.m_sItemSpecEffectValue3;
					}
					this->m_iSP += (iDice(iV1, iV2) + iV3);
					if (this->m_iSP > iMax)
						this->m_iSP = iMax;
					iEffectResult = 3;
				}
				if (this->m_bIsPoisoned == true) {
					this->m_bIsPoisoned = false;
					SetPoisonFlag(this->id_, DEF_OWNERTYPE_PLAYER, false); // removes poison aura when using a revitalizing potion
					m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, nullptr);
				}
				break;
			case DEF_ITEMEFFECTTYPE_HPSTOCK:
				iV1 = item.m_sItemEffectValue1;
				iV2 = item.m_sItemEffectValue2;
				iV3 = item.m_sItemEffectValue3;
				this->m_iHPstock += iDice(iV1, iV2) + iV3;
				if (this->m_iHPstock < 0) this->m_iHPstock = 0;
				if (this->m_iHPstock > 500) this->m_iHPstock = 500;
				this->m_iHungerStatus += iDice(iV1, iV2) + iV3;
				if (this->m_iHungerStatus > 100) this->m_iHungerStatus = 100;
				if (this->m_iHungerStatus < 0) this->m_iHungerStatus = 0;
				break;
			case DEF_ITEMEFFECTTYPE_REPPLUS:
				iMax = 10000;
				if (this->m_iRating < iMax) this->m_iRating += 1;
				iEffectResult = 7;
				break;
			case DEF_ITEMEFFECTTYPE_STUDYSKILL:
				iV1 = item.m_sItemEffectValue1;
				iV2 = item.m_sItemEffectValue2;
				iSEV1 = item.m_sItemSpecEffectValue1;
				if (iSEV1 == 0) {
					TrainSkillResponse(true, client, iV1, iV2);
				} else {
					TrainSkillResponse(true, client, iV1, iSEV1);
				}
				break;
			case DEF_ITEMEFFECTTYPE_STUDYMAGIC:
				iV1 = item.m_sItemEffectValue1;
				if (m_pMagicConfigList[iV1] != nullptr)
					RequestStudyMagicHandler(client, m_pMagicConfigList[iV1]->m_cName, false);
				break;
			case DEF_ITEMEFFECTTYPE_ADDBALLPOINTS:
				char cInfoString[56];
				iV1 = item.m_sItemEffectValue1;
				this->m_iBallPoints += iV1;
				wsprintf(cInfoString, "%d Ball Points added. Total Amount: %d ", iV1, this->m_iBallPoints);
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
				break;
				/*case DEF_ITEMEFFECTTYPE_LOTTERY:
					iLottery = iDice(1, item.
					break;*/
				// New 15/05/2004 Changed
			case DEF_ITEMEFFECTTYPE_MAGIC:
				if ((this->m_iStatus & 0x10) != 0) {
					if (this->m_iAdminUserLevel == 0) {
						SetInvisibilityFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
						delayEvents_.remove(this->id_, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
						this->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
					}
				}
				switch (item.m_sItemEffectValue1) {
					case 1:
						// testcode
						if (bCheckIfIsFlagCarrier(client)) ShowClientMsg(client, "You can not use that item being a flag carrier.");
						else this->RequestTeleportHandler("1   ");
						break;
					case 2:
						if (bCheckIfIsFlagCarrier(client)) ShowClientMsg(client, "You can not use that item being a flag carrier.");
						else PlayerMagicHandler(client, this->m_sX, this->m_sY, 32, true);
						break;
					case 3:
						if (this->map_->m_bIsFightZone == false)
							PlayerMagicHandler(client, this->m_sX, this->m_sY, 34, true);
						break;
					case 4:
						switch (item.m_sItemEffectValue2) {
							case 1:
								if (bCheckIfIsFlagCarrier(client)) ShowClientMsg(client, "You can not use that item being a flag carrier.");
								else {
									if (memcmp(this->map_->m_cName, "bisle", 5) != 0) {
										this->ItemDepleteHandler(sItemIndex, true, true);
										this->RequestTeleportHandler("2   ", "bisle", -1, -1);
									}
								}
								break;
							case 11:
							case 12:
							case 13:
							case 14:
							case 15:
							case 16:
							case 17:
							case 18:
							case 19:
								SYSTEMTIME SysTime;
								GetLocalTime(&SysTime);
								if ((item.m_sTouchEffectValue1 != SysTime.wMonth) ||
										  (item.m_sTouchEffectValue2 != SysTime.wDay) ||
										  (item.m_sTouchEffectValue3 <= SysTime.wHour)) {
								} else {
									char cDestMapName[11];
									std::memset(cDestMapName, 0, sizeof (cDestMapName));
									wsprintf(cDestMapName, "fightzone%d", item.m_sItemEffectValue2 - 10);
									if (memcmp(this->map_->m_cName, cDestMapName, 10) != 0) {
										//v1.42
										this->ItemDepleteHandler(sItemIndex, true, true);
										this->RequestTeleportHandler("2   ", cDestMapName, -1, -1);
									}
								}
								break;
						}
						break;
					case 5:
						// new
						if (this->map_ == 0) break;
						if (memcmp(this->map_->m_cName, "GodH", 4) == 0) break;
						if (item.m_sItemEffectValue2 > 8) {
							if ((m_bHeldenianInitiated == true) && (this->map_->m_bIsHeldenianMap == true)) {
								PlayerMagicHandler(client, this->m_sX, this->m_sY, 31, true,
										  item.m_sItemEffectValue2);
							}
						}
						PlayerMagicHandler(client, this->m_sX, this->m_sY, 31, true,
								  item.m_sItemEffectValue2);
						break;
				}
				break;
			case DEF_ITEMEFFECTTYPE_FIRMSTAMINAR:
				this->m_iTimeLeft_FirmStaminar += item.m_sItemEffectValue1;
				if (this->m_iTimeLeft_FirmStaminar > 20 * 30) this->m_iTimeLeft_FirmStaminar = 20 * 30;
				break;
			case DEF_ITEMEFFECTTYPE_CHANGEATTR:
				switch (item.m_sItemEffectValue1) {
					case 1:
						this->m_cHairColor++;
						if (this->m_cHairColor > 15) this->m_cHairColor = 0;
						sTemp = (this->m_cHairStyle << 8) | (this->m_cHairColor << 4) | (this->m_cUnderwear);
						this->m_sAppr1 = sTemp;
						break;
					case 2:
						this->m_cHairStyle++;
						if (this->m_cHairStyle > 7) this->m_cHairStyle = 0;
						sTemp = (this->m_cHairStyle << 8) | (this->m_cHairColor << 4) | (this->m_cUnderwear);
						this->m_sAppr1 = sTemp;
						break;
					case 3:
						this->m_cSkin++;
						if (this->m_cSkin > 3)
							this->m_cSkin = 1;
						if (this->m_cSex == 1) sTemp = 1;
						else if (this->m_cSex == 2) sTemp = 4;
						switch (this->m_cSkin) {
							case 2: sTemp += 1;
								break;
							case 3: sTemp += 2;
								break;
						}
						this->m_sType = sTemp;
						break;
					case 4:
						sTemp = this->m_sAppr3 & 0xFF0F;
						if (sTemp == 0) {
							if (this->m_cSex == 1)
								this->m_cSex = 2;
							else this->m_cSex = 1;
							if (this->m_cSex == 1) {
								sTmpType = 1;
							} else if (this->m_cSex == 2) {
								sTmpType = 4;
							}
							switch (this->m_cSkin) {
								case 1:
									break;
								case 2:
									sTmpType += 1;
									break;
								case 3:
									sTmpType += 2;
									break;
							}
							sTmpAppr1 = (this->m_cHairStyle << 8) | (this->m_cHairColor << 4) | (this->m_cUnderwear);
							this->m_sType = sTmpType;
							this->m_sAppr1 = sTmpAppr1;
							//
						}
						break;
				}
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
				break;
		}
		this->ItemDepleteHandler(sItemIndex, true, true);
		switch (iEffectResult) {
			case 1:
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
				break;
			case 2:
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
				break;
			case 3:
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				break;
			case 4: // Invincible
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SLATE_INVINCIBLE, 0, 0, 0, nullptr);
				break;
			case 5: // Mana
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SLATE_MANA, 0, 0, 0, nullptr);
				break;
			case 6: // EXP
				m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SLATE_EXP, 0, 0, 0, nullptr);
				break;
			default:
				break;
			case 7: //Rep
				char cRepMessage[60];
				wsprintf(cRepMessage, "You have Earned 1 Rep Point.");
				ShowClientMsg(client, cRepMessage);
				break;
		}
	} else if (item.m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) {
		if (_bDepleteDestTypeItemUseEffect(client, dX, dY, sItemIndex, sDestItemID) == true)
			this->ItemDepleteHandler(sItemIndex, true, true);
	} else if (item.m_cItemType == DEF_ITEMTYPE_ARROW) {
		this->m_cArrowIndex = _iGetArrowItemIndex(client);
	} else if (item.m_cItemType == DEF_ITEMTYPE_USE_PERM) {
		switch (item.m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_SHOWLOCATION:
				iV1 = item.m_sItemEffectValue1;
				switch (iV1) {
					case 1:
						if (strcmp(this->map_->m_cName, "aresden") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 1, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "elvine") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 2, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "middleland") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 3, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "default") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 4, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "huntzone2") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 5, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "huntzone1") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 6, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "huntzone4") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 7, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "huntzone3") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 8, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "arefarm") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 9, 0, nullptr);
						else if (strcmp(this->map_->m_cName, "elvfarm") == 0)
							m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 10, 0, nullptr);
						else m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_SHOWMAP, iV1, 0, 0, nullptr);
						break;
				}
				break;
		}
	} else if (item.m_cItemType == DEF_ITEMTYPE_USE_SKILL) {
		if ((this->m_pItemList[sItemIndex] == nullptr) ||
				  (item.m_wCurLifeSpan <= 0) ||
				  (this->m_bSkillUsingStatus[ item.m_sRelatedSkill ] == true)) {
			return;
		} else {
			if (item.m_wMaxLifeSpan != 0) {
				item.m_wCurLifeSpan--;
				if (item.m_wCurLifeSpan <= 0) {
					m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_ITEMLIFESPANEND, DEF_EQUIPPOS_NONE, sItemIndex, 0, nullptr);
				} else {
					int iSkillUsingTimeID = (int) timeGetTime();
					delayEvents_.add(DelayEventType::USEITEM_SKILL, item.m_sRelatedSkill,
							  dwTime + m_pSkillConfigList[ item.m_sRelatedSkill ]->m_sValue2 * 1000,
							  this->id_, DEF_OWNERTYPE_PLAYER, this->map_, dX, dY,
							  this->m_cSkillMastery[ item.m_sRelatedSkill ], iSkillUsingTimeID, 0);
					this->m_bSkillUsingStatus[ item.m_sRelatedSkill ] = true;
					this->m_iSkillUsingTimeID[ item.m_sRelatedSkill ] = iSkillUsingTimeID; //v1.12
				}
			}
		}
	}
}

bool CClient::bCheckTotalSkillMasteryPoints(int iSkill) {
	register int i;
	int iRemainPoint;
	int iTotalPoints;
	int iWeaponIndex;
	int iDownSkillSSN;
	int iDownPoint;
	short sDownSkillIndex;
	iTotalPoints = 0;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		iTotalPoints += this->m_cSkillMastery[i];
	iRemainPoint = iTotalPoints - DEF_MAXSKILLPOINTS;
	if (iRemainPoint > 0) {
		while (iRemainPoint > 0) {
			sDownSkillIndex = -1; // v1.4
			if (this->m_iDownSkillIndex != -1) {
				switch (this->m_iDownSkillIndex) {
					case 3:
						/* ÀÌ ½ºÅ³µµ 0À¸·Î µÈ´Ù.
						case 4:
						case 5:
						case 7:

							if (this->m_cSkillMastery[this->m_iDownSkillIndex] > 20) {
								sDownSkillIndex = this->m_iDownSkillIndex;
							}
							else {

								iDownSkillSSN = 99999999;
								for (i = 0; i < DEF_MAXSKILLTYPE; i++)
								if ((this->m_cSkillMastery[i] >= 21) && (i != iSkill) &&
									(this->m_iSkillSSN[i] <= iDownSkillSSN)) {

									iDownSkillSSN = this->m_iSkillSSN[i];
									sDownSkillIndex = i;
								}
							}
							break;
						 */
					default:
						if (this->m_cSkillMastery[this->m_iDownSkillIndex] > 0) {
							sDownSkillIndex = this->m_iDownSkillIndex;
						} else {
							iDownSkillSSN = 99999999;
							for (i = 0; i < DEF_MAXSKILLTYPE; i++)
								if ((this->m_cSkillMastery[i] >= 21) && (i != iSkill) &&
										  (this->m_iSkillSSN[i] <= iDownSkillSSN)) {
									iDownSkillSSN = this->m_iSkillSSN[i];
									sDownSkillIndex = i;
								}
						}
						break;
				}
			}
			if (sDownSkillIndex != -1) {
				if (this->m_cSkillMastery[sDownSkillIndex] <= 20) // v1.4
					iDownPoint = this->m_cSkillMastery[sDownSkillIndex];
				else iDownPoint = 1;
				this->m_cSkillMastery[sDownSkillIndex] -= iDownPoint; // v1.4
				this->m_iSkillSSN[sDownSkillIndex] = m_iSkillSSNpoint[this->m_cSkillMastery[sDownSkillIndex] + 1] - 1;
				iRemainPoint -= iDownPoint; // v1.4
				if (this->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1) {
					iWeaponIndex = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
					if (this->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex) {
						this->m_iHitRatio -= iDownPoint; // v1.4
						if (this->m_iHitRatio < 0) this->m_iHitRatio = 0;
					}
				}
				if (this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
					iWeaponIndex = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
					if (this->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex) {
						this->m_iHitRatio -= iDownPoint; // v1.4
						if (this->m_iHitRatio < 0) this->m_iHitRatio = 0;
					}
				}
				this->SendNotifyMsg(0, DEF_NOTIFY_SKILL, sDownSkillIndex, this->m_cSkillMastery[sDownSkillIndex], 0, nullptr);
			} else {
				return false;
			}
		}
		return true;
	}
	return false;
}

void CClient::SendObjectMotionRejectMsg() {
	char * cp, cData[30];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	int iRet;
	this->m_bIsMoveBlocked = true; // v2.171
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_REJECT;
	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	*sp = this->m_sX;
	cp += 2;
	sp = (short *) cp;
	*sp = this->m_sY;
	cp += 2;
	iRet = this->m_pXSock->iSendMsg(cData, 10);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			return;
	}
	return;
}
constexpr char _tmp_cCorpseX[] = {0, 1, 1, 1, 0, -1, -1, -1, 0, 0, 0, 0};
constexpr char _tmp_cCorpseY[] = {-1, -1, 0, 1, 1, 1, 0, -1, 0, 0, 0};

void CClient::ClearSkillUsingStatus() {
	register int i;
	short tX;
	short fX;
	short tY;
	short fY;
	if (this->m_bSkillUsingStatus[19] == true) {
		tX = this->m_sX;
		tY = this->m_sY;
		if (this->map_->bGetMoveable(tX, tY, nullptr) == false) {
			fX = this->m_sX + _tmp_cCorpseX[this->m_cDir];
			fY = this->m_sY + _tmp_cCorpseY[this->m_cDir];
			if (this->map_->bGetMoveable(fX, fY, nullptr) == false) {
				this->m_cDir = iDice(1, 8);
				fX = this->m_sX + _tmp_cCorpseX[this->m_cDir];
				fY = this->m_sY + _tmp_cCorpseY[this->m_cDir];
				if (this->map_->bGetMoveable(fX, fY, nullptr) == false) {
					return;
				}
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_DAMAGEMOVE, this->m_cDir, 0, 0, nullptr);
		}
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		this->m_bSkillUsingStatus[i] = false;
		this->m_iSkillUsingTimeID[i] = 0;
	}
	if (this->m_iAllocatedFish != 0) {
		if (m_pFish[this->m_iAllocatedFish] != nullptr)
			m_pFish[this->m_iAllocatedFish]->m_sEngagingCount--;
		this->m_iAllocatedFish = 0;
		this->SendNotifyMsg(0, DEF_NOTIFY_FISHCANCELED, 0, 0, 0, nullptr);
	}
}

void CClient::UseSkillHandler(int iV1, int /*iV2*/, int /*iV3*/) {
	char cOwnerType;
	short sAttackerWeapon;
	short sOwnerH;
	int iResult;
	int iPlayerSkillLevel;
	if (this->m_bIsInitComplete == false) return;
	if ((iV1 < 0) || (iV1 >= DEF_MAXSKILLTYPE)) return;
	if (m_pSkillConfigList[iV1] == nullptr) return;
	if (this->m_bSkillUsingStatus[iV1] == true) return;
	/*
	if (iV1 != 19) {
		this->m_iAbuseCount++;
		if ((this->m_iAbuseCount % 30) == 0) {
			wsprintf(G_cTxt, "(!) ÇØÅ· ¿ëÀÇÀÚ(%s) Skill(%d) Tries(%d)",this->m_cCharName,
																						 iV1, this->m_iAbuseCount);
			PutLogFileList(G_cTxt);
		}
	}
	 */
	iPlayerSkillLevel = this->m_cSkillMastery[iV1];
	iResult = iDice(1, 100);
	if (iResult > iPlayerSkillLevel) {
		this->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
		return;
	}
	switch (m_pSkillConfigList[iV1]->m_sType) {
		case DEF_SKILLEFFECTTYPE_PRETEND:
			switch (m_pSkillConfigList[iV1]->m_sValue1) {
				case 1:
					if (this->map_->m_bIsFightZone == true) {
						this->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
						return;
					}
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, this->m_sX, this->m_sY);
					if (sOwnerH != 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
						return;
					}
					iResult = 0;
					if (this->m_iAdminUserLevel <= 0) {
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX, this->m_sY - 1);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX, this->m_sY + 1);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX - 1, this->m_sY);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX + 1, this->m_sY);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX - 1, this->m_sY - 1);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX + 1, this->m_sY - 1);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX - 1, this->m_sY + 1);
						iResult += sOwnerH;
						this->map_->GetOwner(&sOwnerH, &cOwnerType, this->m_sX + 1, this->m_sY + 1);
						iResult += sOwnerH;
					}
					if (iResult != 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
						return;
					}
					this->CalculateSSN_SkillIndex(iV1, 1);
					sAttackerWeapon = 1;
					this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, sAttackerWeapon, 0);
					this->map_->ClearOwner(14, iClientH, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
					this->map_->this->SetDeadOwner(DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
					break;
			}
			break;
	}
	this->m_bSkillUsingStatus[iV1] = true;
}

void CClient::ReqSellItemHandler(char cItemID, char cSellToWhom, int iNum, char * /*pItemName*/) {
	char cItemCategory;
	char cItemName[21];
	short sRemainLife;
	int iPrice;
	double d1;
	double d2;
	double d3;
	bool bNeutral;
	uint32_t dwSWEType;
	uint32_t dwSWEValue;
	uint32_t dwAddPrice1;
	uint32_t dwAddPrice2;
	uint32_t dwMul1;
	uint32_t dwMul2;
	if (this->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (this->m_pItemList[cItemID] == nullptr) return;
	if (iNum <= 0) return;
	if (this->m_pItemList[cItemID]->m_dwCount < iNum) return;
	this->iCalcTotalWeight();
	CItem gold;
	std::memset(cItemName, 0, sizeof (cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(gold, cItemName);
	// v1.42
	bNeutral = false;
	if (memcmp(this->m_cLocation, "NONE", 4) == 0) bNeutral = true;
	switch (cSellToWhom) {
		case 15:
		case 24:
			cItemCategory = this->m_pItemList[cItemID]->m_cCategory;
			if ((cItemCategory >= 11) && (cItemCategory <= 50)) {
				iPrice = (this->m_pItemList[cItemID]->m_wPrice / 2) * iNum;
				sRemainLife = this->m_pItemList[cItemID]->m_wCurLifeSpan;
				if (bNeutral) iPrice = iPrice / 2;
				if (iPrice <= 0) iPrice = 1;
				if (iPrice > 1000000) iPrice = 1000000;
				if (this->m_iCurWeightLoad + iGetItemWeight(gold, iPrice) > this->_iCalcMaxLoad()) {
					this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, 0, this->m_pItemList[cItemID]->m_cName);
				} else this->SendNotifyMsg(0, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, this->m_pItemList[cItemID]->m_cName, iNum);
			} else if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
				sRemainLife = this->m_pItemList[cItemID]->m_wCurLifeSpan;
				if (sRemainLife == 0) {
					this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 2, 0, this->m_pItemList[cItemID]->m_cName);
				} else {
					d1 = (double) sRemainLife;
					if (this->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
						d2 = (double) this->m_pItemList[cItemID]->m_wMaxLifeSpan;
					else d2 = 1.0f;
					d3 = (d1 / d2) * 0.5f;
					d2 = (double) this->m_pItemList[cItemID]->m_wPrice;
					d3 = d3 * d2;
					iPrice = (int) d3;
					iPrice = iPrice*iNum;
					dwAddPrice1 = 0;
					dwAddPrice2 = 0;
					if ((this->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != 0) {
						dwSWEType = (this->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
						dwSWEValue = (this->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
						switch (dwSWEType) {
							case 6: dwMul1 = 2;
								break;
							case 8: dwMul1 = 2;
								break;
							case 5: dwMul1 = 3;
								break;
							case 1: dwMul1 = 4;
								break;
							case 7: dwMul1 = 5;
								break;
							case 2: dwMul1 = 6;
								break;
							case 3: dwMul1 = 15;
								break;
							case 9: dwMul1 = 20;
								break;
							default: dwMul1 = 1;
								break;
						}
						d1 = (double) iPrice*dwMul1;
						switch (dwSWEValue) {
							case 1: d2 = 10.0f;
								break;
							case 2: d2 = 20.0f;
								break;
							case 3: d2 = 30.0f;
								break;
							case 4: d2 = 35.0f;
								break;
							case 5: d2 = 40.0f;
								break;
							case 6: d2 = 50.0f;
								break;
							case 7: d2 = 100.0f;
								break;
							case 8: d2 = 200.0f;
								break;
							case 9: d2 = 300.0f;
								break;
							case 10: d2 = 400.0f;
								break;
							case 11: d2 = 500.0f;
								break;
							case 12: d2 = 700.0f;
								break;
							case 13: d2 = 900.0f;
								break;
							default: d2 = 0.0f;
								break;
						}
						d3 = d1 * (d2 / 100.0f);
						dwAddPrice1 = (int) (d1 + d3);
					}
					if ((this->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != 0) {
						dwSWEType = (this->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
						dwSWEValue = (this->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;
						switch (dwSWEType) {
							case 1:
							case 12: dwMul2 = 2;
								break;
							case 2:
							case 3:
							case 4:
							case 5:
							case 6:
							case 7: dwMul2 = 4;
								break;
							case 8:
							case 9:
							case 10:
							case 11: dwMul2 = 6;
								break;
						}
						d1 = (double) iPrice*dwMul2;
						switch (dwSWEValue) {
							case 1: d2 = 10.0f;
								break;
							case 2: d2 = 20.0f;
								break;
							case 3: d2 = 30.0f;
								break;
							case 4: d2 = 35.0f;
								break;
							case 5: d2 = 40.0f;
								break;
							case 6: d2 = 50.0f;
								break;
							case 7: d2 = 100.0f;
								break;
							case 8: d2 = 200.0f;
								break;
							case 9: d2 = 300.0f;
								break;
							case 10: d2 = 400.0f;
								break;
							case 11: d2 = 500.0f;
								break;
							case 12: d2 = 700.0f;
								break;
							case 13: d2 = 900.0f;
								break;
							default: d2 = 0.0f;
								break;
						}
						d3 = d1 * (d2 / 100.0f);
						dwAddPrice2 = (int) (d1 + d3);
					}
					iPrice = iPrice + (dwAddPrice1 - (dwAddPrice1 / 3)) + (dwAddPrice2 - (dwAddPrice2 / 3));
					if (bNeutral) iPrice = iPrice / 2;
					if (iPrice <= 0) iPrice = 1;
					if (iPrice > 1000000) iPrice = 1000000;
					if (this->m_iCurWeightLoad + iGetItemWeight(gold, iPrice) > this->_iCalcMaxLoad()) {
						this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, 0, this->m_pItemList[cItemID]->m_cName);
					} else this->SendNotifyMsg(0, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, this->m_pItemList[cItemID]->m_cName, iNum);
				}
			} else this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 1, 0, this->m_pItemList[cItemID]->m_cName);
			break;
		default:
			break;
	}
}

void CClient::ReqSellItemConfirmHandler(char cItemID, int iNum, char * /*pString*/) {
	short sRemainLife;
	int iPrice;
	double d1;
	double d2;
	double d3;
	char * cp, cItemName[21], cData[120], cItemCategory;
	uint32_t * dwp, dwMul1, dwMul2, dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2;
	uint16_t * wp;
	int iEraseReq;
	int iRet;
	short * sp;
	bool bNeutral;
	if (this->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (this->m_pItemList[cItemID] == nullptr) return;
	if (iNum <= 0) return;
	if (this->m_pItemList[cItemID]->m_dwCount < iNum) return;
	// New 18/05/2004
	if (this->m_pIsProcessingAllowed == false) return;
	this->iCalcTotalWeight();
	cItemCategory = this->m_pItemList[cItemID]->m_cCategory;
	// v1.42
	bNeutral = false;
	if (memcmp(this->m_cLocation, "NONE", 4) == 0) bNeutral = true;
	iPrice = 0;
	if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
		sRemainLife = this->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife <= 0) {
			return;
		} else {
			d1 = (double) sRemainLife;
			if (this->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) this->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) this->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			iPrice = (short) d3;
			iPrice = iPrice*iNum;
			dwAddPrice1 = 0;
			dwAddPrice2 = 0;
			if ((this->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != 0) {
				dwSWEType = (this->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
				dwSWEValue = (this->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
				switch (dwSWEType) {
					case 6: dwMul1 = 2;
						break;
					case 8: dwMul1 = 2;
						break;
					case 5: dwMul1 = 3;
						break;
					case 1: dwMul1 = 4;
						break;
					case 7: dwMul1 = 5;
						break;
					case 2: dwMul1 = 6;
						break;
					case 3: dwMul1 = 15;
						break;
					case 9: dwMul1 = 20;
						break;
					default: dwMul1 = 1;
						break;
				}
				d1 = (double) iPrice*dwMul1;
				switch (dwSWEValue) {
					case 1: d2 = 10.0f;
						break;
					case 2: d2 = 20.0f;
						break;
					case 3: d2 = 30.0f;
						break;
					case 4: d2 = 35.0f;
						break;
					case 5: d2 = 40.0f;
						break;
					case 6: d2 = 50.0f;
						break;
					case 7: d2 = 100.0f;
						break;
					case 8: d2 = 200.0f;
						break;
					case 9: d2 = 300.0f;
						break;
					case 10: d2 = 400.0f;
						break;
					case 11: d2 = 500.0f;
						break;
					case 12: d2 = 700.0f;
						break;
					case 13: d2 = 900.0f;
						break;
					default: d2 = 0.0f;
						break;
				}
				d3 = d1 * (d2 / 100.0f);
				dwAddPrice1 = (int) (d1 + d3);
			}
			if ((this->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != 0) {
				dwSWEType = (this->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
				dwSWEValue = (this->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;
				switch (dwSWEType) {
					case 1:
					case 12: dwMul2 = 2;
						break;
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7: dwMul2 = 4;
						break;
					case 8:
					case 9:
					case 10:
					case 11: dwMul2 = 6;
						break;
				}
				d1 = (double) iPrice*dwMul2;
				switch (dwSWEValue) {
					case 1: d2 = 10.0f;
						break;
					case 2: d2 = 20.0f;
						break;
					case 3: d2 = 30.0f;
						break;
					case 4: d2 = 35.0f;
						break;
					case 5: d2 = 40.0f;
						break;
					case 6: d2 = 50.0f;
						break;
					case 7: d2 = 100.0f;
						break;
					case 8: d2 = 200.0f;
						break;
					case 9: d2 = 300.0f;
						break;
					case 10: d2 = 400.0f;
						break;
					case 11: d2 = 500.0f;
						break;
					case 12: d2 = 700.0f;
						break;
					case 13: d2 = 900.0f;
						break;
					default: d2 = 0.0f;
						break;
				}
				d3 = d1 * (d2 / 100.0f);
				dwAddPrice2 = (int) (d1 + d3);
			}
			iPrice = iPrice + (dwAddPrice1 - (dwAddPrice1 / 3)) + (dwAddPrice2 - (dwAddPrice2 / 3));
			if (bNeutral) iPrice = iPrice / 2;
			if (iPrice <= 0) iPrice = 1;
			if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMSOLD, cItemID, 0, 0, nullptr);
			_bItemLog(DEF_ITEMLOG_SELL, iClientH, (int) - 1, & * this->m_pItemList[cItemID]);
			if ((this->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
					  (this->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
				// v1.41 !!!
				this->SetItemCount(cItemID, this->m_pItemList[cItemID]->m_dwCount - iNum);
			} else this->ItemDepleteHandler(cItemID, false, false);
		}
	} else
		if ((cItemCategory >= 11) && (cItemCategory <= 50)) {
		iPrice = this->m_pItemList[cItemID]->m_wPrice / 2;
		iPrice = iPrice*iNum;
		if (bNeutral) iPrice = iPrice / 2;
		if (iPrice <= 0) iPrice = 1;
		if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004
		this->SendNotifyMsg(0, DEF_NOTIFY_ITEMSOLD, cItemID, 0, 0, nullptr);
		_bItemLog(DEF_ITEMLOG_SELL, iClientH, (int) - 1, & * this->m_pItemList[cItemID]);
		if ((this->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
				  (this->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
			// v1.41 !!!
			this->SetItemCount(cItemID, this->m_pItemList[cItemID]->m_dwCount - iNum);
		} else this->ItemDepleteHandler(cItemID, false, true);
	}
	if (iPrice <= 0) return;
	CItem *gold = new CItem();
	std::memset(cItemName, 0, sizeof (cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(*gold, cItemName);
	gold->m_dwCount = iPrice;
	if (this->_bAddClientItemList(gold, &iEraseReq) == true) {
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 1;
		cp++;
		memcpy(cp, gold->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = gold->m_dwCount;
		cp += 4;
		*cp = gold->m_cItemType;
		cp++;
		*cp = gold->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = gold->m_sLevelLimit;
		cp += 2;
		*cp = gold->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = gold->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = gold->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = gold->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = gold->m_sSpriteFrame;
		cp += 2;
		*cp = gold->m_cItemColor;
		cp++;
		*cp = (char) gold->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = gold->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(pItemGold->m_dwAttribute & 0x00000001);
		cp++;
		 */
		if (iEraseReq == 1)
			delete gold;
		iRet = this->m_pXSock->iSendMsg(cData, 53);
		this->iCalcTotalWeight();
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				break;
		}
	} else {
		this->map_->bSetItem(this->m_sX,
				  this->m_sY, gold);
		this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  this->m_sX, this->m_sY,
				  gold->m_sSprite, gold->m_sSpriteFrame, gold->m_cItemColor); // v1.4 color
		this->iCalcTotalWeight();
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
		iRet = this->m_pXSock->iSendMsg(cData, 6);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				return;
		}
	}
}

void CClient::ReqRepairItemHandler(char cItemID, char cRepairWhom, char * /*pString*/) {
	char cItemCategory;
	short sRemainLife;
	short sPrice;
	double d1;
	double d2;
	double d3;
	if (this->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (this->m_pItemList[cItemID] == nullptr) return;
	cItemCategory = this->m_pItemList[cItemID]->m_cCategory;
	if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
		if (cRepairWhom != 24) {
			this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, 0, this->m_pItemList[cItemID]->m_cName);
			return;
		}
		sRemainLife = this->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			sPrice = this->m_pItemList[cItemID]->m_wPrice / 2;
		} else {
			d1 = (double) sRemainLife;
			if (this->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) this->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) this->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			sPrice = (this->m_pItemList[cItemID]->m_wPrice / 2) - (short) d3;
		}
		this->SendNotifyMsg(0, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, this->m_pItemList[cItemID]->m_cName);
	} else if (((cItemCategory >= 43) && (cItemCategory <= 50)) || ((cItemCategory >= 11) && (cItemCategory <= 12))) {
		if (cRepairWhom != 15) {
			this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, 0, this->m_pItemList[cItemID]->m_cName);
			return;
		}
		sRemainLife = this->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			sPrice = this->m_pItemList[cItemID]->m_wPrice / 2;
		} else {
			d1 = (double) sRemainLife;
			if (this->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) this->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) this->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			sPrice = (this->m_pItemList[cItemID]->m_wPrice / 2) - (short) d3;
		}
		this->SendNotifyMsg(0, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, this->m_pItemList[cItemID]->m_cName);
	} else {
		this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 1, 0, this->m_pItemList[cItemID]->m_cName);
	}
}

void CClient::ReqRepairItemCofirmHandler(char cItemID, char * /*pString*/) {
	short sRemainLife;
	short sPrice;
	char * cp, cItemCategory, cData[120];
	double d1;
	double d2;
	double d3;
	uint32_t * dwp, dwGoldCount;
	uint16_t * wp;
	int iRet;
	if (this->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (this->m_pItemList[cItemID] == nullptr) return;
	// New 18/05/2004
	if (this->m_pIsProcessingAllowed == false) return;
	//testcode
	//PutLogList("Repair!");
	cItemCategory = this->m_pItemList[cItemID]->m_cCategory;
	if (((cItemCategory >= 1) && (cItemCategory <= 10)) || ((cItemCategory >= 43) && (cItemCategory <= 50)) ||
			  ((cItemCategory >= 11) && (cItemCategory <= 12))) {
		sRemainLife = this->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			sPrice = this->m_pItemList[cItemID]->m_wPrice / 2;
		} else {
			d1 = (double) abs(sRemainLife);
			if (this->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) abs(this->m_pItemList[cItemID]->m_wMaxLifeSpan);
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) this->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			sPrice = (this->m_pItemList[cItemID]->m_wPrice / 2) - (short) d3;
		}
		dwGoldCount = this->dwGetItemCount("Gold");
		if (dwGoldCount < (uint32_t) sPrice) {
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_NOTENOUGHGOLD;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = cItemID;
			cp++;
			iRet = this->m_pXSock->iSendMsg(cData, 7);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					return;
			}
			return;
		} else {
			this->m_pItemList[cItemID]->m_wCurLifeSpan = this->m_pItemList[cItemID]->m_wMaxLifeSpan;
			this->SendNotifyMsg(0, DEF_NOTIFY_ITEMREPAIRED, cItemID, this->m_pItemList[cItemID]->m_wCurLifeSpan, 0, nullptr);
			this->iCalcTotalWeight();
			m_stCityStatus[this->m_cSide].iFunds += sPrice;
		}
	} else {
	}
}

void CClient::CheckAndNotifyPlayerConnection(char * pMsg, uint32_t dwSize) {
	char seps[] = "= \t\n";
	char * token, * cp, cName[11], cBuff[256], cPlayerLocation[120];
	class CStrTok * pStrTok;
	register int i;
	uint16_t * wp;
	if (dwSize <= 0) return;
	std::memset(cPlayerLocation, 0, sizeof (cPlayerLocation));
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pMsg, dwSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	if (strlen(token) > 10)
		memcpy(cName, token, 10);
	else memcpy(cName, token, strlen(token));
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(cName, m_pClientList[i]->m_cCharName, 10) == 0)) {
			if (this->m_iAdminUserLevel > 0) { // GM's get more info
				cp = (char *) cPlayerLocation;
				memcpy(cp, m_pClientList[i]->map_->m_cName, 10);
				cp += 10;
				wp = (uint16_t *) cp;
				*wp = m_pClientList[i]->m_sX;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = m_pClientList[i]->m_sY;
				cp += 2;
			}
			this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERONGAME, 0, 0, 0, m_pClientList[i]->m_cCharName, 0, 0, 0, 0, 0, 0, cPlayerLocation);
			delete pStrTok;
			return;
		}
	delete pStrTok;
}

void CClient::ToggleWhisperPlayer(char * pMsg, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256];
	if (dwMsgSize <= 0) return;
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pMsg, dwMsgSize);
	CStrTok pStrTok(cBuff, seps);
	token = pStrTok.pGet();
	token = pStrTok.pGet();
	if (token == nullptr) {
		this->whisperedPlayer_.reset();
		this->SendNotifyMsg(0, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, cName);
	} else {
		if (strlen(token) > 10)
			memcpy(cName, token, 10);
		else memcpy(cName, token, strlen(token));
		this->whisperedPlayer_.reset();
		for (auto &iterClient : m_pClientList) {
			if (memcmp(iterClient.m_cCharName, cName, 10) == 0) {
				if (iterClient.id_ == iClientH) {
					this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You shouldnt talk to yourself!");
					return;
				}
				this->whisperedPlayer_ = iterClient.shared_from_this();
				this->SendNotifyMsg(0, DEF_NOTIFY_WHISPERMODEON, 0, 0, 0, iterClient.m_cCharName);
				break;
			}
		}
		if (!this->whisperedPlayer_.lock()) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "This player is not online!");
		}
	}
}

void CClient::SetPlayerProfile(char * pMsg, uint32_t dwMsgSize) {
	char cTemp[256];
	register int i;
	if ((dwMsgSize - 7) <= 0) return;
	std::memset(cTemp, 0, sizeof (cTemp));
	memcpy(cTemp, (pMsg + 7), dwMsgSize - 7);
	for (i = 0; i < 256; i++)
		if (cTemp[i] == ' ') cTemp[i] = '_';
	cTemp[255] = 0;
	std::memset(this->m_cProfile, 0, sizeof (this->m_cProfile));
	strcpy(this->m_cProfile, cTemp);
}

void CClient::GetPlayerProfile(char * pMsg, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256], cBuff2[500];
	class CStrTok * pStrTok;
	register int i;
	if ((dwMsgSize) <= 0) return;
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pMsg, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cName, token, 10);
		else memcpy(cName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0)) {
				std::memset(cBuff2, 0, sizeof (cBuff2));
				wsprintf(cBuff2, "%s Profile: %s", cName, m_pClientList[i]->m_cProfile);
				this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERPROFILE, 0, 0, 0, cBuff2);
				delete pStrTok;
				return;
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
	}
	delete pStrTok;
	return;
}

void CClient::___RestorePlayerCharacteristics() {
	int iStr;
	int iDex;
	int iInt;
	int iVit;
	int iMag;
	int iCharisma;
	int iOriginalPoint;
	int iCurPoint;
	int iVerifyPoint;
	int iToBeRestoredPoint;
	int iMax;
	int iA;
	int iB;
	bool bFlag;
	char cTxt[120];
	return;
	iStr = this->m_iStr;
	iDex = this->m_iDex;
	iInt = this->m_iInt;
	iVit = this->m_iVit;
	iMag = this->m_iMag;
	iCharisma = this->m_iCharisma;
	iCurPoint = this->m_iStr + this->m_iInt +
			  this->m_iVit + this->m_iDex +
			  this->m_iMag + this->m_iCharisma;
	iOriginalPoint = (this->m_iLevel - 1)*3 + 70;
	iToBeRestoredPoint = iOriginalPoint - iCurPoint;
	if (iToBeRestoredPoint == 0) return;
	if (iToBeRestoredPoint > 0) {
		while (1) {
			bFlag = false;
			if ((iToBeRestoredPoint > 0) && (this->m_iStr < 10)) {
				this->m_iStr++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (this->m_iMag < 10)) {
				this->m_iMag++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (this->m_iInt < 10)) {
				this->m_iInt++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (this->m_iDex < 10)) {
				this->m_iDex++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (this->m_iVit < 10)) {
				this->m_iVit++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (this->m_iCharisma < 10)) {
				this->m_iCharisma++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if (!bFlag) break;
			if (iToBeRestoredPoint <= 0) break;
		}
		iMax = this->m_cSkillMastery[5];
		if (this->m_iStr < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (this->m_iStr < (iMax / 2))) {
					this->m_iStr++;
					iToBeRestoredPoint--;
				}
				if (this->m_iStr == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		iA = this->m_cSkillMastery[7];
		iB = this->m_cSkillMastery[8];
		if (iA > iB)
			iMax = iA;
		else iMax = iB;
		iA = this->m_cSkillMastery[9];
		if (iA > iMax) iMax = iA;
		iA = this->m_cSkillMastery[6];
		if (iA > iMax) iMax = iA;
		if (this->m_iDex < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (this->m_iDex < (iMax / 2))) {
					this->m_iDex++;
					iToBeRestoredPoint--;
				}
				if (this->m_iDex == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		iMax = this->m_cSkillMastery[19];
		if (this->m_iInt < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (this->m_iInt < (iMax / 2))) {
					this->m_iInt++;
					iToBeRestoredPoint--;
				}
				if (this->m_iInt == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		iA = this->m_cSkillMastery[3];
		iB = this->m_cSkillMastery[4];
		if (iA > iB)
			iMax = iA;
		else iMax = iB;
		if (this->m_iMag < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (this->m_iMag < (iMax / 2))) {
					this->m_iMag++;
					iToBeRestoredPoint--;
				}
				if (this->m_iMag == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		while (iToBeRestoredPoint != 0) {
			switch (iDice(1, 6)) {
				case 1:
					if (this->m_iStr < DEF_CHARPOINTLIMIT) {
						this->m_iStr++;
						iToBeRestoredPoint--;
					}
					break;
				case 2:
					if (this->m_iVit < DEF_CHARPOINTLIMIT) {
						this->m_iVit++;
						iToBeRestoredPoint--;
					}
					break;
				case 3:
					if (this->m_iDex < DEF_CHARPOINTLIMIT) {
						this->m_iDex++;
						iToBeRestoredPoint--;
					}
					break;
				case 4:
					if (this->m_iMag < DEF_CHARPOINTLIMIT) {
						this->m_iMag++;
						iToBeRestoredPoint--;
					}
					break;
				case 5:
					if (this->m_iInt < DEF_CHARPOINTLIMIT) {
						this->m_iInt++;
						iToBeRestoredPoint--;
					}
					break;
				case 6:
					if (this->m_iCharisma < DEF_CHARPOINTLIMIT) {
						this->m_iCharisma++;
						iToBeRestoredPoint--;
					}
					break;
			}
		}
		iVerifyPoint = this->m_iStr + this->m_iInt +
				  this->m_iVit + this->m_iDex +
				  this->m_iMag + this->m_iCharisma;
		if (iVerifyPoint != iOriginalPoint) {
			wsprintf(cTxt, "(T_T) RestorePlayerCharacteristics(Minor) FAIL! Player(%s)-(%d/%d)", this->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
			this->m_iStr = iStr;
			this->m_iDex = iDex;
			this->m_iInt = iInt;
			this->m_iVit = iVit;
			this->m_iMag = iMag;
			this->m_iCharisma = iCharisma;
		} else {
			wsprintf(cTxt, "(^o^) RestorePlayerCharacteristics(Minor) SUCCESS! Player(%s)-(%d/%d)", this->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
		}
	} else {
		while (1) {
			bFlag = false;
			if (this->m_iStr > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				this->m_iStr--;
				iToBeRestoredPoint++;
			}
			if (this->m_iDex > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				this->m_iDex--;
				iToBeRestoredPoint++;
			}
			if (this->m_iVit > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				this->m_iVit--;
				iToBeRestoredPoint++;
			}
			if (this->m_iInt > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				this->m_iInt--;
				iToBeRestoredPoint++;
			}
			if (this->m_iMag > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				this->m_iMag--;
				iToBeRestoredPoint++;
			}
			if (this->m_iCharisma > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				this->m_iCharisma--;
				iToBeRestoredPoint++;
			}
			if (!bFlag) break;
			if (iToBeRestoredPoint >= 0) break;
		}
		if (iToBeRestoredPoint < 0) {
			while (iToBeRestoredPoint != 0) {
				switch (iDice(1, 6)) {
					case 1:
						if (this->m_iStr > 10) {
							this->m_iStr--;
							iToBeRestoredPoint++;
						}
						break;
					case 2:
						if (this->m_iVit > 10) {
							this->m_iVit--;
							iToBeRestoredPoint++;
						}
						break;
					case 3:
						if (this->m_iDex > 10) {
							this->m_iDex--;
							iToBeRestoredPoint++;
						}
						break;
					case 4:
						if (this->m_iMag > 10) {
							this->m_iMag--;
							iToBeRestoredPoint++;
						}
						break;
					case 5:
						if (this->m_iInt > 10) {
							this->m_iInt--;
							iToBeRestoredPoint++;
						}
						break;
					case 6:
						if (this->m_iCharisma > 10) {
							this->m_iCharisma--;
							iToBeRestoredPoint++;
						}
						break;
				}
			}
		} else {
			while (iToBeRestoredPoint != 0) {
				switch (iDice(1, 6)) {
					case 1:
						if (this->m_iStr < DEF_CHARPOINTLIMIT) {
							this->m_iStr++;
							iToBeRestoredPoint--;
						}
						break;
					case 2:
						if (this->m_iVit < DEF_CHARPOINTLIMIT) {
							this->m_iVit++;
							iToBeRestoredPoint--;
						}
						break;
					case 3:
						if (this->m_iDex < DEF_CHARPOINTLIMIT) {
							this->m_iDex++;
							iToBeRestoredPoint--;
						}
						break;
					case 4:
						if (this->m_iMag < DEF_CHARPOINTLIMIT) {
							this->m_iMag++;
							iToBeRestoredPoint--;
						}
						break;
					case 5:
						if (this->m_iInt < DEF_CHARPOINTLIMIT) {
							this->m_iInt++;
							iToBeRestoredPoint--;
						}
						break;
					case 6:
						if (this->m_iCharisma < DEF_CHARPOINTLIMIT) {
							this->m_iCharisma++;
							iToBeRestoredPoint--;
						}
						break;
				}
			}
		}
		iVerifyPoint = this->m_iStr + this->m_iInt +
				  this->m_iVit + this->m_iDex +
				  this->m_iMag + this->m_iCharisma;
		if (iVerifyPoint != iOriginalPoint) {
			wsprintf(cTxt, "(T_T) RestorePlayerCharacteristics(Over) FAIL! Player(%s)-(%d/%d)", this->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
			/*
			this->m_iStr = iStr;
			this->m_iDex = iDex;
			this->m_iInt = iInt;
			this->m_iVit = iVit;
			this->m_iMag = iMag;
			this->m_iCharisma = iCharisma;
			 */
		} else {
			wsprintf(cTxt, "(^o^) RestorePlayerCharacteristics(Over) SUCCESS! Player(%s)-(%d/%d)", this->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
		}
	}
}

bool CClient::bAnalyzeCriminalAction(short dX, short dY, bool bIsCheck) {
	int iNamingValue;
	int tX;
	int tY;
	short sOwnerH;
	char cOwnerType;
	char cName[11], cNpcName[21];
	char cNpcWaypoint[11];
	if (this->m_bIsInitComplete == false) return false;
	if (m_bIsCrusadeMode) return false;
	this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr)) {
		if (this->_bGetIsPlayerHostile(sOwnerH) != true) {
			if (bIsCheck) return true;
			std::memset(cNpcName, 0, sizeof (cNpcName));
			if (strcmp(this->map_->m_cName, "aresden") == 0)
				strcpy(cNpcName, "Guard-Aresden");
			else if (strcmp(this->map_->m_cName, "elvine") == 0)
				strcpy(cNpcName, "Guard-Elvine");
			else if (strcmp(this->map_->m_cName, "default") == 0)
				strcpy(cNpcName, "Guard-Neutral");
			iNamingValue = this->map_->iGetEmptyNamingValue();
			if (iNamingValue == -1) {
			} else {
				std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
				std::memset(cName, 0, sizeof (cName));
				wsprintf(cName, "XX%d", iNamingValue);
				cName[0] = '_';
				cName[1] = this->map_->id_ + 65;
				tX = (int) this->m_sX;
				tY = (int) this->m_sY;
				if (this->map_->bCreateNewNpc(cNpcName, cName, 0, 0, DEF_MOVETYPE_RANDOM,
						  &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, true) == false) {
					this->map_->SetNamingValueEmpty(iNamingValue);
				} else {
					bSetNpcAttackMode(cName, iClientH, DEF_OWNERTYPE_PLAYER, true);
				}
			}
		}
	}
	return false;
}

bool CClient::_bGetIsPlayerHostile(int sOwnerH) {
	if (m_pClientList[sOwnerH] == nullptr) return false;
	if (iClientH == sOwnerH) return true;
	if (this->m_cSide == 0) {
		if (m_pClientList[sOwnerH]->m_iPKCount != 0)
			return true;
		else return false;
	} else {
		if (this->m_cSide != m_pClientList[sOwnerH]->m_cSide) {
			if (m_pClientList[sOwnerH]->m_cSide == 0) {
				if (m_pClientList[sOwnerH]->m_iPKCount != 0)
					return true;
				else return false;
			} else return true;
		} else {
			if (m_pClientList[sOwnerH]->m_iPKCount != 0)
				return true;
			else return false;
		}
	}
	return false;
}

void CClient::PoisonEffect(int /*iV1*/) {
	int iPoisonLevel;
	int iDamage;
	int iPrevHP;
	int iProb;
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsInitComplete == false) return;
	iPoisonLevel = this->m_iPoisonLevel;
	iDamage = iDice(1, iPoisonLevel);
	iPrevHP = this->m_iHP;
	this->m_iHP -= iDamage;
	if (this->m_iHP <= 0) this->m_iHP = 1;
	if (iPrevHP != this->m_iHP)
		this->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
	iProb = this->m_cSkillMastery[23] - 10 + this->m_iAddPR;
	if (iProb <= 10) iProb = 10;
	if (iDice(1, 100) <= iProb) {
		this->m_bIsPoisoned = false;
		this->SetPoisonFlag(DEF_OWNERTYPE_PLAYER, false); // remove poison aura after effect complete
		this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, nullptr);
	}
}

void CClient::ShutUpPlayer(char * pMsg, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i, iTime;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelShutup) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pMsg, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cName, token, 10);
		else memcpy(cName, token, strlen(token));
		token = pStrTok->pGet();
		if (token == nullptr)
			iTime = 0;
		else iTime = atoi(token);
		if (iTime < 0) iTime = 0;
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0)) {
				m_pClientList[i]->m_iTimeLeft_ShutUp = iTime * 20;
				this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERSHUTUP, iTime, 0, 0, cName);
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERSHUTUP, iTime, 0, 0, cName);
				// Admin Log
				wsprintf(G_cTxt, "GM Order(%s): Shutup PC(%s) (%d)Min", this->m_cCharName,
						  m_pClientList[i]->m_cCharName, iTime);
				PutAdminLogFileList(G_cTxt);
				delete pStrTok;
				return;
			}
	}
	delete pStrTok;
	return;
}

void CClient::SetPlayerReputation(char * pMsg, char cValue, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iLevel < 40) return;
	if ((this->m_iTimeLeft_Rating != 0) || (this->m_iPKCount != 0)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTRATING, this->m_iTimeLeft_Rating, 0, 0, nullptr);
		return;
	} else if (memcmp(this->m_cLocation, "NONE", 4) == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTRATING, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pMsg, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cName, token, 10);
		else memcpy(cName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0)) {
				if (i != iClientH) {
					if (cValue == 0)
						m_pClientList[i]->m_iRating--;
					else if (cValue == 1)
						m_pClientList[i]->m_iRating++;
					if (m_pClientList[i]->m_iRating > 10000) m_pClientList[i]->m_iRating = 10000;
					if (m_pClientList[i]->m_iRating < -10000) m_pClientList[i]->m_iRating = -10000;
					this->m_iTimeLeft_Rating = 20 * 60;
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_RATINGPLAYER, cValue, 0, 0, cName);
					this->SendNotifyMsg(0, DEF_NOTIFY_RATINGPLAYER, cValue, 0, 0, cName);
					delete pStrTok;
					return;
				}
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
	}
	delete pStrTok;
	return;
}

void CClient::CalcExpStock() {
	bool bIsLevelUp;
	class CItem * pItem;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iExpStock <= 0) return;
	// !!!!
	//if ((this->m_iLevel >= m_iPlayerMaxLevel) && (this->m_iExp >= m_iLevelExpTable[m_iPlayerMaxLevel])) return;
	if (this->map_->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) {
		this->m_iExpStock = 0;
		return;
	}
	this->m_iExp += this->m_iExpStock;
	this->m_iAutoExpAmount += this->m_iExpStock;
	this->m_iExpStock = 0;
	if (this->bCheckLimitedUser() == false) {
		this->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
	}
	bIsLevelUp = this->bCheckLevelUp();
	if ((bIsLevelUp) && (this->m_iLevel <= 5)) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, "Gold") == false) {
			delete pItem;
			return;
		} else pItem->m_dwCount = (uint32_t) 100;
		this->bAddItem(pItem, 0);
	}
	if ((bIsLevelUp) && (this->m_iLevel > 5) && (this->m_iLevel <= 20)) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, "Gold") == false) {
			delete pItem;
			return;
		} else pItem->m_dwCount = (uint32_t) 300;
		this->bAddItem(pItem, 0);
	}
}

void CClient::___RestorePlayerRating() {
	if (this->m_iRating < -10000) this->m_iRating = 0;
	if (this->m_iRating > 10000) this->m_iRating = 0;
}

void CClient::AdminOrder_CallGuard(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	int iNamingValue;
	int tX;
	int tY;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelCallGaurd) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cName, 0, sizeof (cName));
	std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				if (memcmp(m_pClientList[i]->map_->m_cName, "aresden", 7) == 0)
					strcpy(cNpcName, "Guard-Aresden");
				else if (memcmp(m_pClientList[i]->map_->m_cName, "elvine", 6) == 0)
					strcpy(cNpcName, "Guard-Elvine");
				else strcpy(cNpcName, "Guard-Neutral");
				iNamingValue = m_pClientList[i]->map_->iGetEmptyNamingValue();
				if (iNamingValue == -1) {
				} else {
					wsprintf(cName, "XX%d", iNamingValue);
					cName[0] = '_';
					cName[1] = m_pClientList[i]->map_->id_ + 65;
					tX = (int) m_pClientList[i]->m_sX;
					tY = (int) m_pClientList[i]->m_sY;
					if (m_pClientList[i]->map_->bCreateNewNpc(cNpcName, cName, 0, 0, DEF_MOVETYPE_RANDOM,
							  &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, true) == false) {
						m_pClientList[i]->map_->SetNamingValueEmpty(iNamingValue);
					} else {
						bSetNpcAttackMode(cName, i, DEF_OWNERTYPE_PLAYER, true);
					}
				}
				delete pStrTok;
				return;
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CClient::AdminOrder_Kill(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	const char * token;
	char cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	int sAttackerWeapon;
	int sDamage;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelGMKill) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	//testcode
	//wsprintf(G_cTxt, "%s", token);
	//PutLogList(G_cTxt);
	//
	token = pStrTok->pGet();
	//testcode
	//wsprintf(G_cTxt, "%s", token);
	//PutLogList(G_cTxt);
	//
	if (token != nullptr) {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, token);
	} else {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, "null");
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		sDamage = atoi(token);
	} else {
		sDamage = 1;
	}
	//testcode
	//wsprintf(G_cTxt, "%s", token);
	//PutLogList(G_cTxt);
	//
	if (token == nullptr) {
		token = "null";
	}
	if (cName != nullptr) {
		token = cName;
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				m_pClientList[i]->m_iHP = 0;
				//if (iGetMaxHP(i) < m_pClientList[i]->m_iHP) m_pClientList[i]->m_iHP = iGetMaxHP(i);
				m_pClientList[i]->m_bIsKilled = true;
				if (m_pClientList[i]->m_bIsExchangeMode == true) {
					auto with = m_pClientList[i]->exchangingWith_.lock();
					if (with) {
						_ClearExchangeStatus(with->id_);
					}
					_ClearExchangeStatus(i);
				}
				RemoveFromTarget(i, DEF_OWNERTYPE_PLAYER);
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_KILLED, 0, 0, 0, this->m_cCharName);
				sAttackerWeapon = 1;
				m_pClientList[i]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
				m_pClientList[i]->map_->ClearOwner(12, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				m_pClientList[i]->map_->SetDeadOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				delete pStrTok;
				return;
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CClient::AdminOrder_Revive(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	int sAttackerWeapon;
	int sDamage;
	int sHP;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelGMRevive) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, token);
	} else {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, "null");
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		sDamage = atoi(token);
	} else {
		sDamage = 1;
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		sHP = atoi(token);
	} else {
		sHP = 1;
	}
	token = cName;
	//if (token == nullptr) { token = "null"; }
	if (cName != nullptr) {
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				m_pClientList[i]->m_iHP = sHP;
				if (m_pClientList[i]->iGetMaxHP() < m_pClientList[i]->m_iHP) m_pClientList[i]->m_iHP = m_pClientList[i]->iGetMaxHP();
				m_pClientList[i]->m_bIsKilled = false;
				m_pClientList[i]->m_iLastDamage = sDamage;
				//			m_pClientList[i]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGEMOVE, m_pClientList[i]->m_iLastDamage, 0, 0);
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
				sAttackerWeapon = 1;
				m_pClientList[i]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, sDamage, sAttackerWeapon, 0);
				m_pClientList[i]->map_->ClearOwner(14, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				m_pClientList[i]->map_->SetDeadOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				delete pStrTok;
				return;
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CClient::AdminOrder_SummonDemon() {
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	register int iNamingValue;
	int tX;
	int tY;
	if (this->m_bIsAdminCommandEnabled == false) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSummonDemon) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	iNamingValue = this->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		std::memset(cNpcName, 0, sizeof (cNpcName));
		strcpy(cNpcName, "Demon");
		std::memset(cName, 0, sizeof (cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = this->map_->id_ + 65;
		std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
		tX = (int) this->m_sX;
		tY = (int) this->m_sY;
		if (this->map_->bCreateNewNpc(cNpcName, cName, 0, (rand() % 9),
				  DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
			this->map_->SetNamingValueEmpty(iNamingValue);
		} else {
			// Admin Log
			wsprintf(G_cTxt, "Admin Order(%s): Summon Demon", this->m_cCharName);
			PutAdminLogFileList(G_cTxt);
		}
	}
}

void CClient::AdminOrder_SummonDeath() {
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	register int iNamingValue;
	int tX;
	int tY;
	if (this->markedForDeletion_) return;
	if (this->m_bIsAdminCommandEnabled == false) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSummonDeath) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	iNamingValue = this->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		std::memset(cNpcName, 0, sizeof (cNpcName));
		strcpy(cNpcName, "Wyvern");
		std::memset(cName, 0, sizeof (cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = this->map_->id_ + 65;
		std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
		tX = (int) this->m_sX;
		tY = (int) this->m_sY;
		if (this->map_->bCreateNewNpc(cNpcName, cName, 0, (rand() % 9),
				  DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
			this->map_->SetNamingValueEmpty(iNamingValue);
		} else {
			// Admin Log
			wsprintf(G_cTxt, "Admin Order(%s): Summon Death", this->m_cCharName);
			PutAdminLogFileList(G_cTxt);
		}
	}
}

void CClient::AdminOrder_ReserveFightzone(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	int iNum;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelReserveFightzone) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (m_iFightzoneNoForceRecall == 0) {
		m_iFightzoneNoForceRecall = 1;
		this->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -5, 0, 0, nullptr);
	} else {
		m_iFightzoneNoForceRecall = 0;
		this->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -4, 0, 0, nullptr);
	}
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	iNum = atoi(token) - 1;
	if (m_iFightZoneReserve[iNum] != -1) {
		// Admin Log
		wsprintf(G_cTxt, "Admin Order(%s):  %d FightzoneReserved", this->m_cCharName, iNum);
		PutAdminLogFileList(G_cTxt);
		m_iFightZoneReserve[iNum] = -1;
		this->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, iNum + 1, 0, 0, nullptr);
	} else {
		wsprintf(G_cTxt, "Admin Order(%s):  %d Cancel FightzoneReserved", this->m_cCharName, iNum);
		PutAdminLogFileList(G_cTxt);
		m_iFightZoneReserve[iNum] = 0;
		this->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -3, 0, 0, nullptr);
	}
	return;
}

void CClient::AdminOrder_CloseConn(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cTargetName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	/*

	if ((memcmp(this->map_->m_cName, "fightzone", 9) == 0) && (this->m_iSkillSSN[59] == 1)) {
		goto AOCC_PASS;
	}
	 */
	if (this->m_iAdminUserLevel < m_iAdminLevelGMCloseconn) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				if (m_pClientList[i]->m_bIsInitComplete == true) {
					m_pClientList[i]->DeleteClient(true, true, true, true);
				}
				// Admin Log
				wsprintf(G_cTxt, "Admin Order(%s): Close Conn", this->m_cCharName);
				PutAdminLogFileList(G_cTxt);
				delete pStrTok;
				return;
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CClient::UserCommand_BanGuildsman(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cTargetName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iGuildRank != 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				if (memcmp(this->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0) {
					this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTBANGUILDMAN, 0, 0, 0, nullptr);
					delete pStrTok;
					return;
				}
				bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);
				SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, nullptr);
				std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
				strcpy(m_pClientList[i]->m_cGuildName, "NONE");
				m_pClientList[i]->m_iGuildRank = -1;
				m_pClientList[i]->m_iGuildGUID = -1;
				this->SendNotifyMsg(0, DEF_NOTIFY_SUCCESSBANGUILDMAN, 0, 0, 0, nullptr);
				m_pClientList[i]->this->SendNotifyMsg(DEF_COMMONTYPE_BANGUILD, 0, 0, 0, nullptr);
				m_pClientList[i]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
				delete pStrTok;
				return;
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CClient::UserCommand_DissmissGuild(char * pData, uint32_t dwMsgSize) {
}

int CClient::iCheckFish(MapPtr map, short dX, short dY) {
	register int i;
	short sDistX;
	short sDistY;
	if (this->markedForDeletion_) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if (!map) return 0;
	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		auto &dynObjPtr = dynamicObjects_[i];
		if (dynObjPtr) {
			auto &dynObj = *dynObjPtr;
			sDistX = abs(dynObj.m_sX - dX);
			sDistY = abs(dynObj.m_sY - dY);
			if ((dynObj.map_ == map) &&
					  ((dynObj.m_sType == DEF_DYNAMICOBJECT_FISH) || (dynObj.m_sType == DEF_DYNAMICOBJECT_FISHOBJECT)) &&
					  (sDistX <= 2) && (sDistY <= 2)) {
				if (m_pFish[dynObj.m_sOwner] == nullptr) return 0;
				if (m_pFish[dynObj.m_sOwner]->m_sEngagingCount >= DEF_MAXENGAGINGFISH) return 0;
				if (this->m_iAllocatedFish != 0) return 0;
				if (this->map_ != map) return 0;
				this->m_iAllocatedFish = dynObj.m_sOwner;
				this->m_iFishChance = 1;
				this->m_bSkillUsingStatus[1] = true;
				this->SendNotifyMsg(0, DEF_NOTIFY_EVENTFISHMODE, (m_pFish[dynObj.m_sOwner]->m_pItem->m_wPrice / 2), m_pFish[dynObj.m_sOwner]->m_pItem->m_sSprite,
						  m_pFish[dynObj.m_sOwner]->m_pItem->m_sSpriteFrame, m_pFish[dynObj.m_sOwner]->m_pItem->m_cName);
				m_pFish[dynObj.m_sOwner]->m_sEngagingCount++;
				return i;
			}
		}
	}
	return 0;
}

void CClient::AdminOrder_CreateFish(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cItemName[21];
	class CStrTok * pStrTok;
	class CItem * pItem;
	register int tX, tY, iType;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelCreateFish) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	tX = tY = iType = 0;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		tX = atoi(token);
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		tY = atoi(token);
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		iType = atoi(token);
	}
	if ((tX != 0) && (tY != 0) && (iType != 0)) {
		pItem = new class CItem;
		if (pItem == nullptr) {
			delete pStrTok;
			return;
		}
		std::memset(cItemName, 0, sizeof (cItemName));
		strcpy(cItemName, "¹°°í±â");
		if (_bInitItemAttr(*pItem, cItemName) == true) {
			iCreateFish(this->map_, tX, tY, iType, pItem, 1, 60000 * 20);
		} else delete pItem;
	}
	delete pStrTok;
	return;
}

void CClient::AdminOrder_Teleport(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cMapName[256];
	class CStrTok * pStrTok;
	register int dX, dY;
	bool bFlag;
	dX = dY = -1;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelTeleport) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cMapName, 0, sizeof (cMapName));
		strcpy(cMapName, token);
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		dX = atoi(token);
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		dY = atoi(token);
	}
	bFlag = false;
	if (strcmp("2ndmiddle", cMapName) == 0) bFlag = true;
	if (strcmp("abaddon", cMapName) == 0) bFlag = true;
	if (strcmp("arebrk11", cMapName) == 0) bFlag = true;
	if (strcmp("arebrk12", cMapName) == 0) bFlag = true;
	if (strcmp("arebrk21", cMapName) == 0) bFlag = true;
	if (strcmp("arebrk22", cMapName) == 0) bFlag = true;
	if (strcmp("arefarm", cMapName) == 0) bFlag = true;
	if (strcmp("arejail", cMapName) == 0) bFlag = true;
	if (strcmp("aremidl", cMapName) == 0) bFlag = true;
	if (strcmp("aremidr", cMapName) == 0) bFlag = true;
	if (strcmp("aresden", cMapName) == 0) bFlag = true;
	if (strcmp("aresdend1", cMapName) == 0) bFlag = true;
	if (strcmp("areuni", cMapName) == 0) bFlag = true;
	if (strcmp("arewrhus", cMapName) == 0) bFlag = true;
	if (strcmp("bisle", cMapName) == 0) bFlag = true;
	if (strcmp("bsmith_1", cMapName) == 0) bFlag = true;
	if (strcmp("bsmith_1f", cMapName) == 0) bFlag = true;
	if (strcmp("bsmith_2", cMapName) == 0) bFlag = true;
	if (strcmp("bsmith_2f", cMapName) == 0) bFlag = true;
	if (strcmp("BtField", cMapName) == 0) bFlag = true;
	if (strcmp("cath_1", cMapName) == 0) bFlag = true;
	if (strcmp("cath_2", cMapName) == 0) bFlag = true;
	if (strcmp("cityhall_1", cMapName) == 0) bFlag = true;
	if (strcmp("cityhall_2", cMapName) == 0) bFlag = true;
	if (strcmp("CmdHall_1", cMapName) == 0) bFlag = true;
	if (strcmp("CmdHall_2", cMapName) == 0) bFlag = true;
	if (strcmp("default", cMapName) == 0) bFlag = true;
	if (strcmp("dglv2", cMapName) == 0) bFlag = true;
	if (strcmp("dglv3", cMapName) == 0) bFlag = true;
	if (strcmp("dglv4", cMapName) == 0) bFlag = true;
	if (strcmp("druncncity", cMapName) == 0) bFlag = true;
	if (strcmp("elvbrk11", cMapName) == 0) bFlag = true;
	if (strcmp("elvbrk12", cMapName) == 0) bFlag = true;
	if (strcmp("elvbrk21", cMapName) == 0) bFlag = true;
	if (strcmp("elvbrk22", cMapName) == 0) bFlag = true;
	if (strcmp("elvfarm", cMapName) == 0) bFlag = true;
	if (strcmp("elvine", cMapName) == 0) bFlag = true;
	if (strcmp("elvined1", cMapName) == 0) bFlag = true;
	if (strcmp("elvjail", cMapName) == 0) bFlag = true;
	if (strcmp("elvmidl", cMapName) == 0) bFlag = true;
	if (strcmp("elvmidr", cMapName) == 0) bFlag = true;
	if (strcmp("elvuni", cMapName) == 0) bFlag = true;
	if (strcmp("elvwrhus", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone1", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone2", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone3", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone4", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone5", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone6", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone7", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone8", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone9", cMapName) == 0) bFlag = true;
	if (strcmp("fightzone10", cMapName) == 0) bFlag = true;
	if (strcmp("gldhall_1", cMapName) == 0) bFlag = true;
	if (strcmp("gldhall_2", cMapName) == 0) bFlag = true;
	if (strcmp("GodH", cMapName) == 0) bFlag = true;
	if (strcmp("gshop_1", cMapName) == 0) bFlag = true;
	if (strcmp("gshop_1f", cMapName) == 0) bFlag = true;
	if (strcmp("gshop_2", cMapName) == 0) bFlag = true;
	if (strcmp("gshop_2f", cMapName) == 0) bFlag = true;
	if (strcmp("HRampart", cMapName) == 0) bFlag = true;
	if (strcmp("huntzone1", cMapName) == 0) bFlag = true;
	if (strcmp("huntzone2", cMapName) == 0) bFlag = true;
	if (strcmp("huntzone3", cMapName) == 0) bFlag = true;
	if (strcmp("huntzone4", cMapName) == 0) bFlag = true;
	if (strcmp("icebound", cMapName) == 0) bFlag = true;
	if (strcmp("inferniaA", cMapName) == 0) bFlag = true;
	if (strcmp("inferniaB", cMapName) == 0) bFlag = true;
	if (strcmp("maze", cMapName) == 0) bFlag = true;
	if (strcmp("middled1n", cMapName) == 0) bFlag = true;
	if (strcmp("middled1x", cMapName) == 0) bFlag = true;
	if (strcmp("middleland", cMapName) == 0) bFlag = true;
	if (strcmp("penalty", cMapName) == 0) bFlag = true;
	if (strcmp("procella", cMapName) == 0) bFlag = true;
	if (strcmp("resurr1", cMapName) == 0) bFlag = true;
	if (strcmp("resurr2", cMapName) == 0) bFlag = true;
	if (strcmp("toh1", cMapName) == 0) bFlag = true;
	if (strcmp("toh2", cMapName) == 0) bFlag = true;
	if (strcmp("toh3", cMapName) == 0) bFlag = true;
	if (strcmp("wrhus_1", cMapName) == 0) bFlag = true;
	if (strcmp("wrhus_1f", cMapName) == 0) bFlag = true;
	if (strcmp("wrhus_2", cMapName) == 0) bFlag = true;
	if (strcmp("wrhus_2f", cMapName) == 0) bFlag = true;
	if (strcmp("wzdtwr_1", cMapName) == 0) bFlag = true;
	if (strcmp("wzdtwr_2", cMapName) == 0) bFlag = true;
	if (strcmp("Test", cMapName) == 0) bFlag = true;
	if (strcmp("GMMap", cMapName) == 0) bFlag = true;
	if (strcmp("dv", cMapName) == 0) bFlag = true;
	if (strcmp("HBX", cMapName) == 0) bFlag = true;
	if (bFlag)
		this->RequestTeleportHandler("2   ", cMapName, dX, dY);
	delete pStrTok;
	return;
}

void CClient::ReqGetFishThisTimeHandler() {
	register int iResult, iFishH;
	class CItem * pItem;
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iAllocatedFish == 0) return;
	if (m_pFish[this->m_iAllocatedFish] == nullptr) return;
	this->m_bSkillUsingStatus[1] = false;
	iResult = iDice(1, 100);
	if (this->m_iFishChance >= iResult) {
		this->GetExp(iDice(m_pFish[this->m_iAllocatedFish]->m_iDifficulty, 5)); //this->m_iExpStock += iDice(m_pFish[this->m_iAllocatedFish]->m_iDifficulty, 5);
		this->CalculateSSN_SkillIndex(1, m_pFish[this->m_iAllocatedFish]->m_iDifficulty);
		pItem = m_pFish[this->m_iAllocatedFish]->m_pItem;
		m_pFish[this->m_iAllocatedFish]->m_pItem = nullptr;
		this->map_->bSetItem(this->m_sX,
				  this->m_sY,
				  pItem);
		this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  this->m_sX, this->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
		this->SendNotifyMsg(0, DEF_NOTIFY_FISHSUCCESS, 0, 0, 0, nullptr);
		iFishH = this->m_iAllocatedFish;
		this->m_iAllocatedFish = 0;
		bDeleteFish(iFishH, 1);
		return;
	}
	m_pFish[this->m_iAllocatedFish]->m_sEngagingCount--;
	this->SendNotifyMsg(0, DEF_NOTIFY_FISHFAIL, 0, 0, 0, nullptr);
	this->m_iAllocatedFish = 0;
}

int CClient::iGetPlayerRelationship(int iOpponentH) {
	int iRet;
	if (this->markedForDeletion_) return 0;
	if (this->m_bIsInitComplete == false) return 0;
	if (m_pClientList[iOpponentH] == nullptr) return 0;
	if (m_pClientList[iOpponentH]->m_bIsInitComplete == false) return 0;
	iRet = 0;
	if (this->m_iPKCount != 0) {
		if ((memcmp(this->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
				  (memcmp(this->m_cLocation, "NONE", 4) != 0) && (memcmp(m_pClientList[iOpponentH]->m_cLocation, "NONE", 4) != 0)) {
			iRet = 7;
		} else iRet = 2;
	} else if (m_pClientList[iOpponentH]->m_iPKCount != 0) {
		if ((memcmp(this->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
				  (memcmp(this->m_cLocation, "NONE", 4) != 0))
			iRet = 6;
		else iRet = 2;
	} else {
		if (this->m_cSide != m_pClientList[iOpponentH]->m_cSide) {
			if ((this->m_cSide != 0) && (m_pClientList[iOpponentH]->m_cSide != 0)) {
				iRet = 2;
			} else {
				iRet = 0;
			}
		} else {
			if ((memcmp(this->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) == 0) &&
					  (memcmp(this->m_cGuildName, "NONE", 4) != 0)) {
				if (m_pClientList[iOpponentH]->m_iGuildRank == 0)
					iRet = 5;
				else iRet = 3;
			} else
				if ((memcmp(this->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
					  (memcmp(this->m_cGuildName, "NONE", 4) != 0) &&
					  (memcmp(m_pClientList[iOpponentH]->m_cGuildName, "NONE", 4) != 0) &&
					  (memcmp(this->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) != 0)) {
				iRet = 4;
			} else iRet = 1;
		}
	}
	return iRet;
}

void CClient::AdminOrder_CheckIP(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cIP[21], cInfoString[500];
	class CStrTok * pStrTok;
	register int i;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelCheckIP) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cIP, 0, sizeof (cIP));
		strcpy(cIP, token);
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cIPaddress, cIP, strlen(cIP)) == 0)) {
				std::memset(cInfoString, 0, sizeof (cInfoString));
				wsprintf(cInfoString, "Name(%s/%s) Loc(%s: %d %d) Level(%d:%d) Init(%d) IP(%s)",
						  m_pClientList[i]->m_cAccountName, m_pClientList[i]->m_cCharName, m_pClientList[i]->map_->m_cName,
						  m_pClientList[i]->m_sX, m_pClientList[i]->m_sY,
						  m_pClientList[i]->m_iLevel, nullptr/*m_pClientList[i]->m_cAccountStatus*/,
						  m_pClientList[i]->m_bIsInitComplete, m_pClientList[i]->m_cIPaddress);
				this->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
			}
	}
	delete pStrTok;
}

void CClient::ToggleSafeAttackModeHandler() {
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsSafeAttackMode == true)
		this->m_bIsSafeAttackMode = false;
	else this->m_bIsSafeAttackMode = true;
	this->SendNotifyMsg(0, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, nullptr);
}

void CClient::AdminOrder_Polymorph(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelPolymorph) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (memcmp(token, "off", 3) == 0)
			this->m_sType = this->m_sOriginalType;
		else if (memcmp(token, "Slime", 5) == 0)
			this->m_sType = 10;
		else if (memcmp(token, "Skeleton", 8) == 0)
			this->m_sType = 11;
		else if (memcmp(token, "Stone-Golem", 11) == 0)
			this->m_sType = 12;
		else if (memcmp(token, "Cyclops", 7) == 0)
			this->m_sType = 13;
		else if (memcmp(token, "Orc", 3) == 0)
			this->m_sType = 14;
		else if (memcmp(token, "ShopKeeper", 10) == 0)
			this->m_sType = 15;
		else if (memcmp(token, "Giant-Ant", 9) == 0)
			this->m_sType = 16;
		else if (memcmp(token, "Scorpion", 8) == 0)
			this->m_sType = 17;
		else if (memcmp(token, "Zombie", 6) == 0)
			this->m_sType = 18;
		else if (memcmp(token, "Gandlf", 6) == 0)
			this->m_sType = 19;
		else if (memcmp(token, "Howard", 6) == 0)
			this->m_sType = 20;
		else if (memcmp(token, "Gaurd", 5) == 0)
			this->m_sType = 21;
		else if (memcmp(token, "Amphis", 6) == 0)
			this->m_sType = 22;
		else if (memcmp(token, "Clay-Golem", 10) == 0)
			this->m_sType = 23;
		else if (memcmp(token, "Tom", 3) == 0)
			this->m_sType = 24;
		else if (memcmp(token, "William", 7) == 0)
			this->m_sType = 25;
		else if (memcmp(token, "Kennedy", 7) == 0)
			this->m_sType = 26;
		else if (memcmp(token, "Hellbound", 9) == 0)
			this->m_sType = 27;
		else if (memcmp(token, "Troll", 5) == 0)
			this->m_sType = 28;
		else if (memcmp(token, "Orge", 4) == 0)
			this->m_sType = 29;
		else if (memcmp(token, "Liche", 5) == 0)
			this->m_sType = 30;
		else if (memcmp(token, "Demon", 5) == 0)
			this->m_sType = 31;
		else if (memcmp(token, "Unicorn", 7) == 0)
			this->m_sType = 32;
		else if (memcmp(token, "WereWolf", 8) == 0)
			this->m_sType = 33;
		else if (memcmp(token, "Dummy", 5) == 0)
			this->m_sType = 34;
		else if (memcmp(token, "Energy-Sphere", 13) == 0)
			this->m_sType = 35;
		else if (memcmp(token, "AGT", 3) == 0)
			this->m_sType = 36;
		else if (memcmp(token, "CGT", 3) == 0)
			this->m_sType = 37;
		else if (memcmp(token, "MS", 2) == 0)
			this->m_sType = 38;
		else if (memcmp(token, "DT", 2) == 0)
			this->m_sType = 39;
		else if (memcmp(token, "ESG", 3) == 0)
			this->m_sType = 40;
		else if (memcmp(token, "GMG", 3) == 0)
			this->m_sType = 41;
		else if (memcmp(token, "ManaStone", 9) == 0)
			this->m_sType = 42;
		else if (memcmp(token, "LWB", 3) == 0)
			this->m_sType = 43;
		else if (memcmp(token, "GHK", 3) == 0)
			this->m_sType = 44;
		else if (memcmp(token, "GHC", 6) == 0)
			this->m_sType = 45;
		else if (memcmp(token, "TK", 2) == 0)
			this->m_sType = 46;
		else if (memcmp(token, "BG", 2) == 0)
			this->m_sType = 47;
		else if (memcmp(token, "Stalker", 7) == 0)
			this->m_sType = 48;
		else if (memcmp(token, "Hellclaw", 8) == 0)
			this->m_sType = 49;
		else if (memcmp(token, "Tigerworm", 8) == 0)
			this->m_sType = 50;
		else if (memcmp(token, "CP", 2) == 0)
			this->m_sType = 51;
		else if (memcmp(token, "Gagoyle", 7) == 0)
			this->m_sType = 52;
		else if (memcmp(token, "Beholder", 8) == 0)
			this->m_sType = 53;
		else if (memcmp(token, "Dark-Elf", 8) == 0)
			this->m_sType = 54;
		else if (memcmp(token, "Rabbit", 6) == 0)
			this->m_sType = 55;
		else if (memcmp(token, "Cat", 3) == 0)
			this->m_sType = 56;
		else if (memcmp(token, "Giant-Frog", 10) == 0)
			this->m_sType = 57;
		else if (memcmp(token, "Mountain-Giant", 14) == 0)
			this->m_sType = 58;
		else if (memcmp(token, "Ettin", 5) == 0)
			this->m_sType = 59;
		else if (memcmp(token, "Cannibal-Plant", 13) == 0)
			this->m_sType = 60;
		else if (memcmp(token, "Rudolph", 7) == 0)
			this->m_sType = 61;
		else if (memcmp(token, "DireBoar", 8) == 0)
			this->m_sType = 62;
		else if (memcmp(token, "Frost", 5) == 0)
			this->m_sType = 63;
		else if (memcmp(token, "Crops", 5) == 0)
			this->m_sType = 64;
		else if (memcmp(token, "Ice-Golem", 9) == 0)
			this->m_sType = 65;
			//else if (memcmp(token, "Wyvern", 6) == 0)
			//	this->m_sType = 66;
		else if (memcmp(token, "McGaffin", 8) == 0)
			this->m_sType = 67;
		else if (memcmp(token, "Perry", 5) == 0)
			this->m_sType = 68;
		else if (memcmp(token, "Devlin", 6) == 0)
			this->m_sType = 69;
		else if (memcmp(token, "Barlog", 6) == 0)
			this->m_sType = 70;
		else if (memcmp(token, "Centaurus", 9) == 0)
			this->m_sType = 71;
		else if (memcmp(token, "Claw-Turtle", 11) == 0)
			this->m_sType = 72;
			//else if (memcmp(token, "Fire-Wyvern", 11) == 0)
			//	this->m_sType = 73;
		else if (memcmp(token, "Giant-Crayfish", 14) == 0)
			this->m_sType = 74;
		else if (memcmp(token, "Giant-Lizard", 12) == 0)
			this->m_sType = 75;
		else if (memcmp(token, "Giant-Plant", 11) == 0)
			this->m_sType = 76;
		else if (memcmp(token, "MasterMage-Orc", 14) == 0)
			this->m_sType = 77;
		else if (memcmp(token, "Minotaurs", 9) == 0)
			this->m_sType = 78;
		else if (memcmp(token, "Nizie", 5) == 0)
			this->m_sType = 79;
		else if (memcmp(token, "Tentocle", 8) == 0)
			this->m_sType = 80;
			//else if (memcmp(token, "Abaddon", 7) == 0)
			//	this->m_sType = 81;
		else if (memcmp(token, "Sor", 3) == 0)
			this->m_sType = 82;
		else if (memcmp(token, "ATK", 3) == 0)
			this->m_sType = 83;
		else if (memcmp(token, "Elf", 3) == 0)
			this->m_sType = 84;
		else if (memcmp(token, "DSK", 3) == 0)
			this->m_sType = 85;
		else if (memcmp(token, "HBT", 3) == 0)
			this->m_sType = 86;
		else if (memcmp(token, "CT", 2) == 0)
			this->m_sType = 87;
		else if (memcmp(token, "Bar", 3) == 0)
			this->m_sType = 88;
		else if (memcmp(token, "AGC", 3) == 0)
			this->m_sType = 89;
		else if (memcmp(token, "Gail", 4) == 0)
			this->m_sType = 90;
		else if (memcmp(token, "gate", 4) == 0)
			this->m_sType = 91;
	}
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	delete pStrTok;
}

void CClient::AdminOrder_SetInvi(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSetInvis) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '1') this->SetInvisibilityFlag(DEF_OWNERTYPE_PLAYER, true);
		else
			if (token[0] == '0') this->SetInvisibilityFlag(DEF_OWNERTYPE_PLAYER, false);
	}
	delete pStrTok;
}

void CClient::AdminOrder_SetZerk(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSetZerk) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '1') {
			this->SetBerserkFlag(DEF_OWNERTYPE_PLAYER, true);
			this->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = (char) m_pMagicConfigList[32]->m_sValue4;
		} else
			if (token[0] == '0') this->SetBerserkFlag(DEF_OWNERTYPE_PLAYER, false);
	}
	delete pStrTok;
}

void CClient::AdminOrder_SetFreeze(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSetIce) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '1') this->SetIceFlag(DEF_OWNERTYPE_PLAYER, true);
		else
			if (token[0] == '0') this->SetIceFlag(DEF_OWNERTYPE_PLAYER, false);
	}
	delete pStrTok;
}

void CClient::_CheckAttackType(short *spType) {
	uint16_t wType;
	if (this->markedForDeletion_) return;
	wType = ((this->m_sAppr2 & 0x0FF0) >> 4);
	switch (*spType) {
		case 2:
			if (this->m_cArrowIndex == -1) *spType = 0;
			if (wType < 40) *spType = 1;
			break;
		case 20:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[5] < 100) *spType = 1;
			break;
		case 21:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[7] < 100) *spType = 1;
			break;
		case 22:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[9] < 100) *spType = 1;
			break;
		case 23:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[8] < 100) *spType = 1;
			break;
		case 24:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[10] < 100) *spType = 1;
			break;
		case 25:
			if (this->m_iSuperAttackLeft <= 0) *spType = 2;
			if (this->m_cSkillMastery[6] < 100) *spType = 2;
			if (this->m_cArrowIndex == -1) *spType = 0;
			if (wType < 40) *spType = 1;
			break;
		case 26:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[14] < 100) *spType = 1;
			break;
		case 27:
			if (this->m_iSuperAttackLeft <= 0) *spType = 1;
			if (this->m_cSkillMastery[21] < 100) *spType = 1;
			break;
	}
}

void CClient::ReqCreatePortionHandler(char *pData) {
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cI[6], cPortionName[21], cData[120];
	int iRet;
	int i;
	int j;
	int iEraseReq;
	int iSkillLimit;
	int iSkillLevel;
	int iResult;
	int iDifficulty;
	short * sp, sItemIndex[6], sTemp;
	short sItemNumber[6], sItemArray[12];
	bool bDup;
	bool bFlag;
	class CItem * pItem;
	if (this->markedForDeletion_) return;
	this->m_iSkillMsgRecvCount++;
	for (i = 0; i < 6; i++) {
		cI[i] = -1;
		sItemIndex[i] = -1;
		sItemNumber[i] = 0;
	}
	cp = (char *) (pData + 11);
	cI[0] = *cp;
	cp++;
	cI[1] = *cp;
	cp++;
	cI[2] = *cp;
	cp++;
	cI[3] = *cp;
	cp++;
	cI[4] = *cp;
	cp++;
	cI[5] = *cp;
	cp++;
	for (i = 0; i < 6; i++) {
		if (cI[i] >= DEF_MAXITEMS) return;
		if ((cI[i] >= 0) && (this->m_pItemList[cI[i]] == nullptr)) return;
	}
	for (i = 0; i < 6; i++)
		if (cI[i] >= 0) {
			bDup = false;
			for (j = 0; j < 6; j++)
				if (sItemIndex[j] == cI[i]) {
					sItemNumber[j]++;
					bDup = true;
				}
			if (!bDup) {
				for (j = 0; j < 6; j++)
					if (sItemIndex[j] == -1) {
						sItemIndex[j] = cI[i];
						sItemNumber[j]++;
						goto RCPH_LOOPBREAK;
					}
RCPH_LOOPBREAK:
				;
			}
		}
	for (i = 0; i < 6; i++)
		if (sItemIndex[i] != -1) {
			if (sItemIndex[i] < 0) return;
			if ((sItemIndex[i] >= 0) && (sItemIndex[i] >= DEF_MAXITEMS)) return;
			if (this->m_pItemList[sItemIndex[i]] == nullptr) return;
			if (this->m_pItemList[sItemIndex[i]]->m_dwCount < sItemNumber[i]) return;
		}
	bFlag = true;
	while (bFlag) {
		bFlag = false;
		for (i = 0; i < 5; i++)
			if ((sItemIndex[i] != -1) && (sItemIndex[i + 1] != -1)) {
				if ((this->m_pItemList[sItemIndex[i]]->m_sIDnum) <
						  (this->m_pItemList[sItemIndex[i + 1]]->m_sIDnum)) {
					sTemp = sItemIndex[i + 1];
					sItemIndex[i + 1] = sItemIndex[i];
					sItemIndex[i] = sTemp;
					sTemp = sItemNumber[i + 1];
					sItemNumber[i + 1] = sItemNumber[i];
					sItemNumber[i] = sTemp;
					bFlag = true;
				}
			}
	}
	//testcode
	/*
	char cTemp[120];
	char cTxt1[120];
	char cTxt2[120];
	std::memset(cTxt1, 0, sizeof(cTxt1));
	std::memset(cTxt2, 0, sizeof(cTxt2));
	for (i = 0; i < 6; i++) {
		std::memset(cTemp, 0, sizeof(cTemp));
		if (sItemIndex[i] != -1)
			 wsprintf(cTemp, "(%d) ", this->m_pItemList[sItemIndex[i]]->m_sIDnum);
		else strcpy(cTemp, "(*)");
		strcat(cTxt1, cTemp);
		std::memset(cTemp, 0, sizeof(cTemp));
		wsprintf(cTemp, "(%d) ", sItemNumber[i]);
		strcat(cTxt2, cTemp);
	}
	PutLogList(cTxt1);
	PutLogList(cTxt2);
	 */
	j = 0;
	for (i = 0; i < 6; i++) {
		if (sItemIndex[i] != -1)
			sItemArray[j] = this->m_pItemList[sItemIndex[i]]->m_sIDnum;
		else sItemArray[j] = sItemIndex[i];
		sItemArray[j + 1] = sItemNumber[i];
		j += 2;
	}
	//testcode
	/*
	char cTemp[120], cTxt1[120];
	std::memset(cTxt1, 0, sizeof(cTxt1));
	for (i = 0; i < 12; i++) {
		std::memset(cTemp, 0, sizeof(cTemp));
		wsprintf(cTemp, "%d", sItemArray[i]);
		strcat(cTxt1, cTemp);
	}
	PutLogList(cTxt1);
	 */
	std::memset(cPortionName, 0, sizeof (cPortionName));
	for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
		if (m_pPortionConfigList[i] != nullptr) {
			bFlag = false;
			for (j = 0; j < 12; j++)
				if (m_pPortionConfigList[i]->m_sArray[j] != sItemArray[j]) bFlag = true;
			if (!bFlag) {
				std::memset(cPortionName, 0, sizeof (cPortionName));
				memcpy(cPortionName, m_pPortionConfigList[i]->m_cName, 20);
				iSkillLimit = m_pPortionConfigList[i]->m_iSkillLimit;
				iDifficulty = m_pPortionConfigList[i]->m_iDifficulty;
			}
		}
	if (strlen(cPortionName) == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_NOMATCHINGPORTION, 0, 0, 0, nullptr);
		return;
	}
	iSkillLevel = this->m_cSkillMastery[12];
	if (iSkillLimit > iSkillLevel) {
		this->SendNotifyMsg(0, DEF_NOTIFY_LOWPORTIONSKILL, 0, 0, 0, cPortionName);
		return;
	}
	iSkillLevel -= iDifficulty;
	if (iSkillLevel <= 0) iSkillLevel = 1;
	iResult = iDice(1, 100);
	if (iResult > iSkillLevel) {
		this->SendNotifyMsg(0, DEF_NOTIFY_PORTIONFAIL, 0, 0, 0, cPortionName);
		return;
	}
	this->CalculateSSN_SkillIndex(12, 1);
	if (strlen(cPortionName) != 0) {
		pItem = nullptr;
		pItem = new class CItem;
		if (pItem == nullptr) return;
		for (i = 0; i < 6; i++)
			if (sItemIndex[i] != -1) {
				if (this->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME)
					// v1.41 !!!
					this->SetItemCount(sItemIndex[i], //     this->m_pItemList[sItemIndex[i]]->m_cName,
						  this->m_pItemList[sItemIndex[i]]->m_dwCount - sItemNumber[i]);
				else this->ItemDepleteHandler(sItemIndex[i], false, true);
			}
		this->SendNotifyMsg(0, DEF_NOTIFY_PORTIONSUCCESS, 0, 0, 0, cPortionName);
		this->m_iExpStock += iDice(1, (iDifficulty / 3));
		if ((_bInitItemAttr(*pItem, cPortionName) == true)) {
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				std::memset(cData, 0, sizeof (cData));
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char) 0;
				cp++;
				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
				cp++;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwAttribute;
				cp += 4;
				/*
				 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
				cp++;
				 */
				if (iEraseReq == 1) delete pItem;
				iRet = this->m_pXSock->iSendMsg(cData, 53);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						break;
				}
				//if ((pItem->m_wPrice * pItem->m_dwCount) > 1000)
				//	SendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, iClientH);
			} else {
				this->map_->bSetItem(this->m_sX,
						  this->m_sY, pItem);
				this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
						  this->m_sX, this->m_sY,
						  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						break;
				}
			}
		} else {
			delete pItem;
			pItem = nullptr;
		}
	}
}

void CClient::LocalSavePlayerData() {
	char * pData, * cp, cFn[256], cDir[256], cTxt[256], cCharDir[256];
	int iSize;
	FILE * pFile;
	SYSTEMTIME SysTime;
	if (this->markedForDeletion_) return;
	pData = new char[30000];
	if (pData == nullptr) return;
	ZeroMemory(pData, 30000);
	cp = (char *) (pData);
	iSize = this->_iComposePlayerDataFileContents(cp);
	GetLocalTime(&SysTime);
	ZeroMemory(cCharDir, sizeof (cDir));
	wsprintf(cCharDir, "Character_%d_%d_%d_%d", SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	std::memset(cDir, 0, sizeof (cDir));
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, cCharDir);
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	wsprintf(cTxt, "AscII%d", (unsigned char) this->m_cCharName[0]);
	strcat(cFn, cTxt);
	strcpy(cDir, cFn);
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, this->m_cCharName);
	strcat(cFn, ".txt");
	_mkdir(cCharDir);
	_mkdir(cDir);
	if (iSize == 0) {
		PutLogList("(!) Character data body empty: Cannot create & save temporal player data file.");
		delete pData;
		return;
	}
	pFile = fopen(cFn, "wt");
	if (pFile == nullptr) {
		wsprintf(cTxt, "(!) Cannot create temporal player data file : Name(%s)", cFn);
		PutLogList(cTxt);
	} else {
		wsprintf(cTxt, "(!) temporal player data file saved : Name(%s)", cFn);
		PutLogList(cTxt);
		fwrite(cp, iSize, 1, pFile);
	}
	if (pFile != nullptr) fclose(pFile);
	delete pData;
}

void CClient::_CheckMiningAction(int dX, int dY) {
	short sType;
	uint32_t dwRegisterTime;
	int iDynamicIndex;
	int iSkillLevel;
	int iResult;
	int iItemID;
	class CItem * pItem;
	uint16_t wWeaponType;
	if (this->markedForDeletion_) return;
	this->map_->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);
	switch (sType) {
		case DEF_DYNAMICOBJECT_MINERAL1:
		case DEF_DYNAMICOBJECT_MINERAL2:
			wWeaponType = ((this->m_sAppr2 & 0x0FF0) >> 4);
			if (wWeaponType == 25) {
			} else return;
			if ((this->m_sAppr2 & 0xF000) == 0) return;
			iSkillLevel = this->m_cSkillMastery[0];
			if (iSkillLevel == 0) break;
			if (dynamicObjects_[iDynamicIndex] == nullptr) break;
			iSkillLevel -= m_pMineral[dynamicObjects_[iDynamicIndex]->m_iV1]->m_iDifficulty;
			if (iSkillLevel <= 0) iSkillLevel = 1;
			iResult = iDice(1, 100);
			if (iResult <= iSkillLevel) {
				this->CalculateSSN_SkillIndex(0, 1);
				switch (m_pMineral[dynamicObjects_[iDynamicIndex]->m_iV1]->m_cType) {
					case 1:
						switch (iDice(1, 5)) {
							case 1:
							case 2:
							case 3:
								iItemID = 355; // Coal
								this->m_iExpStock += iDice(1, 3);
								break;
							case 4:
								iItemID = 357; // IronOre
								this->m_iExpStock += iDice(1, 3);
								break;
							case 5:
								iItemID = 507; // BlondeStone
								this->m_iExpStock += iDice(1, 3);
								break;
						}
						break;
					case 2:
						switch (iDice(1, 5)) {
							case 1:
							case 2:
								iItemID = 355; // Coal
								this->m_iExpStock += iDice(1, 3);
								break;
							case 3:
							case 4:
								iItemID = 357; // IronOre
								this->m_iExpStock += iDice(1, 3);
								break;
							case 5:
								if (iDice(1, 3) == 2) {
									iItemID = 356; // SilverNugget
									this->m_iExpStock += iDice(1, 4);
								} else {
									iItemID = 507; // BlondeStone
									this->m_iExpStock += iDice(1, 3);
								}
								break;
						}
						break;
					case 3:
						switch (iDice(1, 6)) {
							case 1:
								iItemID = 355; // Coal
								this->m_iExpStock += iDice(1, 3);
								break;
							case 2:
							case 3:
							case 4:
							case 5:
								iItemID = 357; // IronOre
								this->m_iExpStock += iDice(1, 3);
								break;
							case 6:
								if (iDice(1, 8) == 3) {
									if (iDice(1, 2) == 1) {
										iItemID = 356; // SilverNugget
										this->m_iExpStock += iDice(1, 4);
									} else {
										iItemID = 357; // IronOre
										this->m_iExpStock += iDice(1, 3);
									}
									break;
								} else {
									iItemID = 357; // IronOre
									this->m_iExpStock += iDice(1, 3);
								}
								break;
						}
						break;
					case 4:
						switch (iDice(1, 6)) {
							case 1:
								iItemID = 355; // Coal
								this->m_iExpStock += iDice(1, 3);
								break;
							case 2:
								if (iDice(1, 3) == 2) {
									iItemID = 356; // SilverNugget
									this->m_iExpStock += iDice(1, 4);
								}
								break;
							case 3:
							case 4:
							case 5:
								iItemID = 357; // IronOre
								this->m_iExpStock += iDice(1, 3);
								break;
							case 6:
								if (iDice(1, 8) == 3) {
									if (iDice(1, 4) == 3) {
										if (iDice(1, 4) < 3) {
											iItemID = 508; // Mithral
											this->m_iExpStock += iDice(1, 6);
										} else {
											iItemID = 354; // GoldNugget
											this->m_iExpStock += iDice(1, 5);
										}
										break;
									} else {
										iItemID = 356; // SilverNugget
										this->m_iExpStock += iDice(1, 4);
									}
									break;
								} else {
									if (iDice(1, 2) == 1) {
										iItemID = 354; // GoldNugget
										this->m_iExpStock += iDice(1, 5);
									} else {
										iItemID = 357; // IronOre
										this->m_iExpStock += iDice(1, 3);
									}
									break;
								}
								break;
						}
						break;
					case 5:
						switch (iDice(1, 19)) {
							case 3:
								iItemID = 352; // Sapphire
								this->m_iExpStock += iDice(2, 3);
								break;
							default:
								iItemID = 358; // Crystal
								this->m_iExpStock += iDice(2, 3);
								break;
						}
						break;
					case 6:
						switch (iDice(1, 5)) {
							case 1:
								if (iDice(1, 6) == 3) {
									iItemID = 353; // Emerald
									this->m_iExpStock += iDice(2, 4);
								} else {
									iItemID = 358; // Crystal
									this->m_iExpStock += iDice(2, 3);
								}
								break;
							case 2:
								if (iDice(1, 6) == 3) {
									iItemID = 352; // Saphire
									this->m_iExpStock += iDice(2, 4);
								} else {
									iItemID = 358; // Crystal
									this->m_iExpStock += iDice(2, 3);
								}
								break;
							case 3:
								if (iDice(1, 6) == 3) {
									iItemID = 351; // Ruby
									this->m_iExpStock += iDice(2, 4);
								} else {
									iItemID = 358; // Crystal
									this->m_iExpStock += iDice(2, 3);
								}
								break;
							case 4:
								iItemID = 358; // Crystal
								this->m_iExpStock += iDice(2, 3);
								break;
							case 5:
								if (iDice(1, 12) == 3) {
									iItemID = 350; // Diamond
									this->m_iExpStock += iDice(2, 5);
								} else {
									iItemID = 358; // Crystal
									this->m_iExpStock += iDice(2, 3);
								}
								break;
						}
						break;
				}
				pItem = new class CItem;
				if (_bInitItemAttr(*pItem, iItemID) == false) {
					delete pItem;
				} else {
					this->map_->bSetItem(this->m_sX,
							  this->m_sY, pItem);
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  this->m_sX, this->m_sY,
							  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4
					// testcode
					// wsprintf(G_cTxt, "Mining Success: %d", iItemID);
					// PutLogList(G_cTxt);
				}
				m_pMineral[dynamicObjects_[iDynamicIndex]->m_iV1]->m_iRemain--;
				if (m_pMineral[dynamicObjects_[iDynamicIndex]->m_iV1]->m_iRemain <= 0) {
					bDeleteMineral(dynamicObjects_[iDynamicIndex]->m_iV1);
					dynamicObjects_[iDynamicIndex].reset();
				}
			}
			break;
		default:
			break;
	}
}

void CClient::NpcTalkHandler(int iWho) {
	char cRewardName[21], cTargetName[21];
	int iResMode;
	int iQuestNum;
	int iQuestType;
	int iRewardType;
	int iRewardAmount;
	int iContribution;
	int iX;
	int iY;
	int iRange;
	int iTargetType;
	int iTargetCount;
	iQuestNum = 0;
	std::memset(cTargetName, 0, sizeof (cTargetName));
	if (this->markedForDeletion_) return;
	switch (iWho) {
		case 1: break;
		case 2: break;
		case 3: break;
		case 4:
			iQuestNum = this->_iTalkToNpcResult_Cityhall(&iQuestType, &iResMode, &iRewardType, &iRewardAmount, &iContribution, cTargetName, &iTargetType, &iTargetCount, &iX, &iY, &iRange);
			break;
		case 5: break;
		case 6: break;
		case 32: break;
		case 21:
			iQuestNum = this->_iTalkToNpcResult_Guard(&iQuestType, &iResMode, &iRewardType, &iRewardAmount, &iContribution, cTargetName, &iTargetType, &iTargetCount, &iX, &iY, &iRange);
			if (iQuestNum >= 1000) return;
			break;
	}
	std::memset(cRewardName, 0, sizeof (cRewardName));
	if (iQuestNum > 0) {
		if (iRewardType > 1) {
			strcpy(cRewardName, m_pItemConfigList[iRewardType]->m_cName);
		} else {
			switch (iRewardType) {
				case -10: strcpy(cRewardName, "���F-�");
					break;
			}
		}
		this->m_iAskedQuest = iQuestNum;
		this->m_iQuestRewardType = iRewardType;
		this->m_iQuestRewardAmount = iRewardAmount;
		this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, iQuestType, iResMode, iRewardAmount, cRewardName, iContribution,
				  iTargetType, iTargetCount, iX, iY, iRange, cTargetName);
	} else {
		switch (iQuestNum) {
			case 0: this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (iWho + 130), 0, 0, nullptr, 0);
				break;
			case -1:
			case -2:
			case -3:
			case -4: this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, abs(iQuestNum) + 100, 0, 0, nullptr, 0);
				break;
			case -5: break;
		}
	}
}

void CClient::AdminOrder_GetNpcStatus(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cNpcName[256];
	class CStrTok * pStrTok;
	register int i;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelGetNpcStatus) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cNpcName, 0, sizeof (cNpcName));
		strcpy(cNpcName, token);
	}
	for (i = 0; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {
			if (memcmp(m_pNpcList[i]->m_cName, cNpcName, 5) == 0) {
				this->SendNotifyMsg(0, DEF_NOTIFY_ADMINIFO, 1, i, 0, nullptr);
			}
		}
	delete pStrTok;
	return;
}

int CClient::_iGetWeaponSkillType() {
	uint16_t wWeaponType;
	if (this->markedForDeletion_) return 0;
	wWeaponType = ((this->m_sAppr2 & 0x0FF0) >> 4);
	if (wWeaponType == 0) {
		return 5;
	} else if ((wWeaponType >= 1) && (wWeaponType <= 2)) {
		return 7;
	} else if ((wWeaponType > 2) && (wWeaponType < 20)) {
		if (wWeaponType == 7)
			return 9;
		else return 8;
	} else if ((wWeaponType >= 20) && (wWeaponType < 30)) {
		return 10;
	} else if ((wWeaponType >= 30) && (wWeaponType < 35)) {
		return 14;
	} else if ((wWeaponType >= 35) && (wWeaponType < 40)) {
		return 21;
	} else if (wWeaponType >= 40) {
		return 6;
	}
	return 1;
}

void CClient::SetDownSkillIndexHandler(int iSkillIndex) {
	if (this->markedForDeletion_) return;
	if ((iSkillIndex < 0) || (iSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (this->m_cSkillMastery[iSkillIndex] > 0)
		this->m_iDownSkillIndex = iSkillIndex;
	this->SendNotifyMsg(0, DEF_NOTIFY_DOWNSKILLINDEXSET, this->m_iDownSkillIndex, 0, 0, nullptr);
}

bool CClient::_bDepleteDestTypeItemUseEffect(int dX, int dY, short sItemIndex, short sDestItemID) {
	bool bRet;
	if (this->markedForDeletion_) return false;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
	if (this->m_pItemList[sItemIndex] == nullptr) return false;
	switch (this->m_pItemList[sItemIndex]->m_sItemEffectType) {
		case DEF_ITEMEFFECTTYPE_OCCUPYFLAG:
			bRet = this->map_->__bSetOccupyFlag(dX, dY,
					  this->m_pItemList[sItemIndex]->m_sItemEffectValue1,
					  0, iClientH, 0);
			if (bRet) {
				this->GetExp((iDice(this->m_iLevel, 10)));
			} else {
				this->SendNotifyMsg(0, DEF_NOTIFY_NOTFLAGSPOT, 0, 0, 0, nullptr);
			}
			return bRet;
			// crusade
		case DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT:
			bRet = __bSetConstructionKit(dX, dY, //
					  this->m_pItemList[sItemIndex]->m_sItemEffectValue1, //
					  this->m_pItemList[sItemIndex]->m_sItemEffectValue2, //
					  *m_pClientList[iClientH]);
			if (bRet) {
			} else {
			}
			return bRet;
		case DEF_ITEMEFFECTTYPE_DYE:
			if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
				if (this->m_pItemList[sDestItemID] != nullptr) {
					if ((this->m_pItemList[sDestItemID]->m_cCategory == 11) ||
							  (this->m_pItemList[sDestItemID]->m_cCategory == 12)) {
						this->m_pItemList[sDestItemID]->m_cItemColor = this->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, this->m_pItemList[sDestItemID]->m_cItemColor, 0, nullptr);
						return true;
					} else {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, nullptr);
						return false;
					}
				}
			}
			break;
		case DEF_ITEMEFFECTTYPE_ARMORDYE:
			if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
				if (this->m_pItemList[sDestItemID] != nullptr) {
					if (this->m_pItemList[sDestItemID]->m_cCategory == 6) {
						this->m_pItemList[sDestItemID]->m_cItemColor = this->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, this->m_pItemList[sDestItemID]->m_cItemColor, 0, nullptr);
						return true;
					} else {
						this->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, nullptr);
						return false;
					}
				}
			}
			break;
		case DEF_ITEMEFFECTTYPE_FARMING:
			bRet = bPlantSeedBag(dX, dY,
					  this->m_pItemList[sItemIndex]->m_sItemEffectValue1,
					  this->m_pItemList[sItemIndex]->m_sItemEffectValue2,
					  iClientH);
			return bRet;
		default:
			break;
	}
	return true;
}

void CClient::SetSummonMobAction(int iMode, uint32_t dwMsgSize, char *pData) {
	register int i, iTargetIndex;
	char seps[] = "= \t\n";
	char * token, cTargetName[11], cBuff[256];
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if (this->m_cSide == 0) return;
	switch (iMode) {
		case 0:
		case 1:
			for (i = 0; i < DEF_MAXNPCS; i++)
				if (m_pNpcList[i] != nullptr) {
					if ((m_pNpcList[i]->m_bIsSummoned == true) &&
							  (m_pNpcList[i]->m_iFollowOwnerIndex == iClientH) &&
							  (m_pNpcList[i]->m_cFollowOwnerType == DEF_OWNERTYPE_PLAYER)) {
						m_pNpcList[i]->m_iSummonControlMode = iMode;
						m_pNpcList[i]->m_bIsPermAttackMode = false;
						m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
						m_pNpcList[i]->m_sBehaviorTurnCount = 0;
						m_pNpcList[i]->m_iTargetIndex = 0;
					}
				}
			break;
		case 2:
			if ((dwMsgSize) <= 0) return;
			memcpy(cBuff, pData, dwMsgSize);
			pStrTok = new class CStrTok(cBuff, seps);
			token = pStrTok->pGet();
			token = pStrTok->pGet();
			iTargetIndex = 0;
			if (token != nullptr) {
				if (strlen(token) > 10)
					memcpy(cTargetName, token, 10);
				else memcpy(cTargetName, token, strlen(token));
				for (i = 1; i < DEF_MAXCLIENTS; i++) {
					// if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) { // original
					if ((m_pClientList[i] != nullptr) &&
							  (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0) &&
							  (strcmp(this->map_->m_cName, m_pClientList[i]->map_->m_cName) == 0)) {
						iTargetIndex = i;
						goto SSMA_SKIPSEARCH;
					}
				}
			}
SSMA_SKIPSEARCH:
			;
			if ((iTargetIndex != 0) && (m_pClientList[iTargetIndex]->m_cSide != 0) &&
					  (m_pClientList[iTargetIndex]->m_cSide != this->m_cSide)) {
				for (i = 0; i < DEF_MAXNPCS; i++)
					if (m_pNpcList[i] != nullptr) {
						if ((m_pNpcList[i]->m_bIsSummoned == true) &&
								  (m_pNpcList[i]->m_iFollowOwnerIndex == iClientH) &&
								  (m_pNpcList[i]->m_cFollowOwnerType == DEF_OWNERTYPE_PLAYER)) {
							m_pNpcList[i]->m_iSummonControlMode = iMode;
							m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
							m_pNpcList[i]->m_sBehaviorTurnCount = 0;
							m_pNpcList[i]->m_iTargetIndex = iTargetIndex;
							m_pNpcList[i]->m_cTargetType = DEF_OWNERTYPE_PLAYER;
							m_pNpcList[i]->m_bIsPermAttackMode = true;
						}
					}
			}
			break;
	}
}

void CClient::GetOccupyFlagHandler() {
	int i;
	int iNum;
	int iRet;
	int iEraseReq;
	int iEKNum;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (this->markedForDeletion_) return;
	if (this->m_iEnemyKillCount < 3) return;
	if (this->m_cSide == 0) return;
	std::memset(cItemName, 0, sizeof (cItemName));
	switch (this->m_cSide) {
		case 1: strcpy(cItemName, "AresdenFlag");
			break;
		case 2: strcpy(cItemName, "ElvineFlag");
			break;
	}
	iNum = 1;
	for (i = 1; i <= iNum; i++) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {
			delete pItem;
		} else {
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				if (this->m_iCurWeightLoad < 0) this->m_iCurWeightLoad = 0;
				if (this->m_iEnemyKillCount >= 10) {
					iEKNum = 10;
					this->m_iEnemyKillCount -= 10;
				} else {
					iEKNum = this->m_iEnemyKillCount;
					this->m_iEnemyKillCount = iEKNum;
				}
				pItem->m_sItemSpecEffectValue1 = iEKNum;
				wsprintf(G_cTxt, "PC(%s) obtained a Flag (%d).   %s(%d %d)", this->m_cCharName, this->m_cSide, this->map_->m_cName, this->m_sX, this->m_sY);
				PutLogList(G_cTxt);
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char) 0;
				cp++;
				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				*cp = (char) pItem->m_sItemSpecEffectValue2;
				cp++;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwAttribute;
				cp += 4;
				if (iEraseReq == 1) delete pItem;
				iRet = this->m_pXSock->iSendMsg(cData, 53);
				this->iCalcTotalWeight();
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
				this->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, this->m_iEnemyKillCount, 0, 0, nullptr);
			} else {
				delete pItem;
				this->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CClient::GetFightzoneTicketHandler() {
	int iRet;
	int iEraseReq;
	int iMonth;
	int iDay;
	int iHour;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (this->markedForDeletion_) return;
	if (this->m_iFightZoneTicketNumber <= 0) {
		this->m_iFightzoneNumber *= -1;
		this->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -1, 0, 0, nullptr);
		return;
	}
	std::memset(cItemName, 0, sizeof (cItemName));
	if (this->m_iFightzoneNumber == 1)
		strcpy(cItemName, "ArenaTicket");
	else wsprintf(cItemName, "ArenaTicket(%d)", this->m_iFightzoneNumber);
	pItem = new class CItem;
	if (_bInitItemAttr(*pItem, cItemName) == false) {
		delete pItem;
		return;
	}
	if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
		if (this->m_iCurWeightLoad < 0) this->m_iCurWeightLoad = 0;
		this->m_iFightZoneTicketNumber = this->m_iFightZoneTicketNumber - 1;
		pItem->m_sTouchEffectType = DEF_ITET_DATE;
		iMonth = this->m_iReserveTime / 10000;
		iDay = (this->m_iReserveTime - iMonth * 10000) / 100;
		iHour = this->m_iReserveTime - iMonth * 10000 - iDay * 100;
		pItem->m_sTouchEffectValue1 = iMonth;
		pItem->m_sTouchEffectValue2 = iDay;
		pItem->m_sTouchEffectValue3 = iHour;
		wsprintf(G_cTxt, "(*) Get FIGHTZONETICKET : Char(%s) TICKENUMBER (%d)(%d)(%d)", this->m_cCharName, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
		PutLogFileList(G_cTxt);
		PutLogList(G_cTxt);
		std::memset(cData, 0, sizeof (cData));
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 1;
		cp++;
		memcpy(cp, pItem->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;
		*cp = pItem->m_cItemType;
		cp++;
		*cp = pItem->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;
		*cp = pItem->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;
		*cp = pItem->m_cItemColor;
		cp++;
		*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		if (iEraseReq == 1) delete pItem;
		iRet = this->m_pXSock->iSendMsg(cData, 53);
		this->iCalcTotalWeight();
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				return;
		}
	} else {
		delete pItem;
		this->iCalcTotalWeight();
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
		iRet = this->m_pXSock->iSendMsg(cData, 6);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				this->DeleteClient(true, true);
				return;
		}
	}
}

void CClient::GetHeroMantleHandler(int iItemID, char */*pString*/) {
	int i;
	int iNum;
	int iRet;
	int iEraseReq;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (this->markedForDeletion_) return;
	if (this->m_iEnemyKillCount < 100) return;
	if (this->m_cSide == 0) return;
	if (this->_iGetItemSpaceLeft() == 0) {
		this->SendItemNotifyMsg(DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	//Prevents a crash if item dosent exist
	if (m_pItemConfigList[iItemID] == nullptr) return;
	switch (iItemID) {
			// Hero Cape
		case 400: //Aresden HeroCape
		case 401: //Elvine HeroCape
			if (this->m_iEnemyKillCount < 300) return;
			this->m_iEnemyKillCount -= 300;
			break;
			// Hero Helm
		case 403: //Aresden HeroHelm(M)
		case 404: //Aresden HeroHelm(W)
		case 405: //Elvine HeroHelm(M)
		case 406: //Elvine HeroHelm(W)
			if (this->m_iEnemyKillCount < 150) return;
			this->m_iEnemyKillCount -= 150;
			if (this->m_iContribution < 20) return;
			this->m_iContribution -= 20;
			break;
			// Hero Cap
		case 407: //Aresden HeroCap(M)
		case 408: //Aresden HeroCap(W)
		case 409: //Elvine HeroHelm(M)
		case 410: //Elvine HeroHelm(W)
			if (this->m_iEnemyKillCount < 100) return;
			this->m_iEnemyKillCount -= 100;
			if (this->m_iContribution < 20) return;
			this->m_iContribution -= 20;
			break;
			// Hero Armour
		case 411: //Aresden HeroArmour(M)
		case 412: //Aresden HeroArmour(W)
		case 413: //Elvine HeroArmour(M)
		case 414: //Elvine HeroArmour(W)
			if (this->m_iEnemyKillCount < 300) return;
			this->m_iEnemyKillCount -= 300;
			if (this->m_iContribution < 30) return;
			this->m_iContribution -= 30;
			break;
			// Hero Robe
		case 415: //Aresden HeroRobe(M)
		case 416: //Aresden HeroRobe(W)
		case 417: //Elvine HeroRobe(M)
		case 418: //Elvine HeroRobe(W)
			if (this->m_iEnemyKillCount < 200) return;
			this->m_iEnemyKillCount -= 200;
			if (this->m_iContribution < 20) return;
			this->m_iContribution -= 20;
			break;
			// Hero Hauberk
		case 419: //Aresden HeroHauberk(M)
		case 420: //Aresden HeroHauberk(W)
		case 421: //Elvine HeroHauberk(M)
		case 422: //Elvine HeroHauberk(W)
			if (this->m_iEnemyKillCount < 100) return;
			this->m_iEnemyKillCount -= 100;
			if (this->m_iContribution < 10) return;
			this->m_iContribution -= 10;
			break;
			// Hero Leggings
		case 423: //Aresden HeroLeggings(M)
		case 424: //Aresden HeroLeggings(W)
		case 425: //Elvine HeroLeggings(M)
		case 426: //Elvine HeroLeggings(W)
			if (this->m_iEnemyKillCount < 150) return;
			this->m_iEnemyKillCount -= 150;
			if (this->m_iContribution < 15) return;
			this->m_iContribution -= 15;
			break;
		default:
			return;
			break;
	}
	std::memset(cItemName, 0, sizeof (cItemName));
	memcpy(cItemName, m_pItemConfigList[iItemID]->m_cName, 20);
	// ReqPurchaseItemHandler
	iNum = 1;
	for (i = 1; i <= iNum; i++) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {
			delete pItem;
		} else {
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				if (this->m_iCurWeightLoad < 0) this->m_iCurWeightLoad = 0;
				wsprintf(G_cTxt, "(*) Get HeroItem : Char(%s) Player-EK(%d) Player-Contr(%d) Hero Obtained(%s)", this->m_cCharName, this->m_iEnemyKillCount, this->m_iContribution, cItemName);
				PutLogFileList(G_cTxt);
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = this->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = this->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = this->m_sCharIDnum3;
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char) 0;
				cp++;
				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
				cp++;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwAttribute;
				cp += 4;
				/*
				 *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-Item??? ??
				cp++;
				 */
				if (iEraseReq == 1) delete pItem;
				iRet = this->m_pXSock->iSendMsg(cData, 53);
				this->iCalcTotalWeight();
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
				this->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, this->m_iEnemyKillCount, 0, 0, nullptr);
			} else {
				delete pItem;
				this->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CClient::_SetItemPos(char *pData) {
	char *cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	char cItemIndex = *cp;
	cp++;
	short *sp = (short *) cp;
	short sX = *sp;
	cp += 2;
	sp = (short *) cp;
	short sY = *sp;
	cp += 2;
	if (sY < -10) sY = -10;
	if ((cItemIndex < 0) || (cItemIndex >= DEF_MAXITEMS)) return;
	if (this->m_pItemList[cItemIndex] != nullptr) {
		this->m_ItemPosList[cItemIndex].x = sX;
		this->m_ItemPosList[cItemIndex].y = sY;
	}
}

void CClient::CheckUniqueItemEquipment() {
	register int i, iDamage;
	if (this->markedForDeletion_) return;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			if ((this->m_pItemList[i]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER) &&
					  (this->m_bIsItemEquipped[i] == true)) {
				if ((this->m_pItemList[i]->m_sTouchEffectValue1 == this->m_sCharIDnum1) &&
						  (this->m_pItemList[i]->m_sTouchEffectValue2 == this->m_sCharIDnum2) &&
						  (this->m_pItemList[i]->m_sTouchEffectValue3 == this->m_sCharIDnum3)) {
				} else {
					this->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, this->m_pItemList[i]->m_cEquipPos, i, 0, nullptr);
					this->ReleaseItemHandler(i, true);
					iDamage = iDice(10, 10);
					this->m_iHP -= iDamage;
					if (this->m_iHP <= 0) {
						this->ClientKilledHandler(0, 0, iDamage);
					}
				}
			}
		}
}

void CClient::_BWM_Init(char */*pData*/) {
	this->m_bIsBWMonitor = true;
	wsprintf(G_cTxt, "(*) BWMonitor(%d) registered.", this->id_);
	PutLogList(G_cTxt);
}

void CClient::ExchangeItemHandler(short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char */*pItemName*/) {
	short sOwnerH;
	char cOwnerType;
	if (this->markedForDeletion_) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if ((m_bAdminSecurity) && (this->m_iAdminUserLevel > 0)) return;
	if (this->m_pItemList[sItemIndex] == nullptr) return;
	if (this->m_pItemList[sItemIndex]->m_dwCount < iAmount) return;
	if (this->m_bIsOnServerChange == true) return;
	if (this->m_bIsExchangeMode == true) return;
	if (wObjectID >= DEF_MAXCLIENTS) return;
	this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
		if ((m_bAdminSecurity) && (m_pClientList[sOwnerH]->m_iAdminUserLevel > 0)) {
			return;
		}
		if (wObjectID != 0) {
			if (wObjectID < 10000) {
				if (m_pClientList[wObjectID] != nullptr) {
					if ((uint16_t) sOwnerH != wObjectID) sOwnerH = 0;
				}
			} else sOwnerH = 0;
		}
		if ((sOwnerH == 0) || (m_pClientList[sOwnerH] == nullptr)) {
			this->_ClearExchangeStatus();
		} else {
			if ((m_pClientList[sOwnerH]->m_bIsExchangeMode == true) || (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) ||
					  (m_pClientList[sOwnerH]->map_->m_bIsFightZone == true)) {
				this->_ClearExchangeStatus();
			} else {
				this->m_bIsExchangeMode = true;
				this->exchangingWith_ = m_pClientList[sOwnerH]->shared_from_this();
				//Clear items in the list
				this->iExchangeCount = 0;
				m_pClientList[sOwnerH]->iExchangeCount = 0;
				for (int i = 0; i < 4; i++) {
					//Clear the trader
					std::memset(this->m_cExchangeItemName[i], 0, sizeof (this->m_cExchangeItemName[i]));
					this->m_cExchangeItemIndex[i] = -1;
					this->m_iExchangeItemAmount[i] = 0;
					//Clear the guy we're trading with
					std::memset(m_pClientList[sOwnerH]->m_cExchangeItemName[i], 0, sizeof (m_pClientList[sOwnerH]->m_cExchangeItemName[i]));
					m_pClientList[sOwnerH]->m_cExchangeItemIndex[i] = -1;
					m_pClientList[sOwnerH]->m_iExchangeItemAmount[i] = 0;
				}
				this->m_cExchangeItemIndex[this->iExchangeCount] = (char) sItemIndex;
				this->m_iExchangeItemAmount[this->iExchangeCount] = iAmount;
				//std::memset(this->m_cExchangeItemName, 0, sizeof(this->m_cExchangeItemName));
				memcpy(this->m_cExchangeItemName[this->iExchangeCount], this->m_pItemList[sItemIndex]->m_cName, 20);
				m_pClientList[sOwnerH]->m_bIsExchangeMode = true;
				m_pClientList[sOwnerH]->exchangingWith_ = this->shared_from_this();
				this->iExchangeCount++;
				this->this->SendNotifyMsg(DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex + 1000, this->m_pItemList[sItemIndex]->m_sSprite,
						  this->m_pItemList[sItemIndex]->m_sSpriteFrame, this->m_pItemList[sItemIndex]->m_cName, iAmount, this->m_pItemList[sItemIndex]->m_cItemColor,
						  this->m_pItemList[sItemIndex]->m_wCurLifeSpan,
						  this->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
						  this->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
						  this->m_pItemList[sItemIndex]->m_dwAttribute);
				m_pClientList[sOwnerH]->this->SendNotifyMsg(DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex, this->m_pItemList[sItemIndex]->m_sSprite,
						  this->m_pItemList[sItemIndex]->m_sSpriteFrame, this->m_pItemList[sItemIndex]->m_cName, iAmount, this->m_pItemList[sItemIndex]->m_cItemColor,
						  this->m_pItemList[sItemIndex]->m_wCurLifeSpan,
						  this->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
						  this->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
						  this->m_pItemList[sItemIndex]->m_dwAttribute);
			}
		}
	} else {
		this->_ClearExchangeStatus();
	}
}

void CClient::SetExchangeItem(int iItemIndex, int iAmount) {
	if (this->markedForDeletion_) return;
	if (this->m_bIsOnServerChange == true) return;
	if (this->iExchangeCount > 4) return; //only 4 items trade
	//no admin trade
	if ((m_bAdminSecurity) && (this->m_iAdminUserLevel > 0)) {
		auto with = this->exchangingWith_.lock();
		if (with) {
			_ClearExchangeStatus(with->id_);
		}
		this->_ClearExchangeStatus();
	}
	if ((this->m_bIsExchangeMode == true)) {
		auto with = this->exchangingWith_.lock();
		if (with) {
			if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
			if (this->m_pItemList[iItemIndex] == nullptr) return;
			if (this->m_pItemList[iItemIndex]->m_dwCount < iAmount) return;
			//No Duplicate items
			for (int i = 0; i < this->iExchangeCount; i++) {
				if (this->m_cExchangeItemIndex[i] == (char) iItemIndex) {
					_ClearExchangeStatus(with->id_);
					this->_ClearExchangeStatus();
					return;
				}
			}
			this->m_cExchangeItemIndex[this->iExchangeCount] = (char) iItemIndex;
			this->m_iExchangeItemAmount[this->iExchangeCount] = iAmount;
			//std::memset(this->m_cExchangeItemName, 0, sizeof(this->m_cExchangeItemName));
			memcpy(this->m_cExchangeItemName[this->iExchangeCount], this->m_pItemList[iItemIndex]->m_cName, 20);
			//this->m_cExchangeItemIndex  = iItemIndex;
			//this->m_iExchangeItemAmount = iAmount;
			//std::memset(this->m_cExchangeItemName, 0, sizeof(this->m_cExchangeItemName));
			//memcpy(this->m_cExchangeItemName, this->m_pItemList[iItemIndex]->m_cName, 20);
			this->iExchangeCount++;
			this->this->SendNotifyMsg(DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex + 1000, this->m_pItemList[iItemIndex]->m_sSprite,
					  this->m_pItemList[iItemIndex]->m_sSpriteFrame, this->m_pItemList[iItemIndex]->m_cName, iAmount, this->m_pItemList[iItemIndex]->m_cItemColor,
					  this->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					  this->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
					  this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
					  this->m_pItemList[iItemIndex]->m_dwAttribute);
			with->this->SendNotifyMsg(DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex, this->m_pItemList[iItemIndex]->m_sSprite,
					  this->m_pItemList[iItemIndex]->m_sSpriteFrame, this->m_pItemList[iItemIndex]->m_cName, iAmount, this->m_pItemList[iItemIndex]->m_cItemColor,
					  this->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					  this->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
					  this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
					  this->m_pItemList[iItemIndex]->m_dwAttribute);
		}
	}
}

void CClient::ConfirmExchangeItem() {
	int iItemWeightA;
	int iItemWeightB;
	int iWeightLeftA;
	int iWeightLeftB;
	int iAmountLeft;
	class CItem * pItemA[4], * pItemB[4], * pItemAcopy[4], * pItemBcopy[4];
	if (this->markedForDeletion_) return;
	if (this->m_bIsOnServerChange == true) return;
	if ((m_bAdminSecurity) && (this->m_iAdminUserLevel > 0)) return;
	if ((this->m_bIsExchangeMode == true)) {
		auto with = this->exchangingWith_.lock();
		if (with) {
			if (iClientH == with->id_) return;
			if ((with->m_bIsExchangeMode != true) || !with->exchangingWith_.lock() || with->exchangingWith_.lock()->id_ != iClientH) {
				this->_ClearExchangeStatus();
				_ClearExchangeStatus(with->id_);
				return;
			} else {
				this->m_bIsExchangeConfirm = true;
				if (with->m_bIsExchangeConfirm == true) {
					//Check all items
					for (int i = 0; i < this->iExchangeCount; i++) {
						if ((this->m_pItemList[this->m_cExchangeItemIndex[i]] == nullptr) ||
								  (memcmp(this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cName, this->m_cExchangeItemName[i], 20) != 0)) {
							this->_ClearExchangeStatus();
							_ClearExchangeStatus(with->id_);
							return;
						}
					}
					for (int i = 0; i < with->iExchangeCount; i++) {
						if ((with->m_pItemList[with->m_cExchangeItemIndex[i]] == nullptr) ||
								  (memcmp(with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cName, with->m_cExchangeItemName[i], 20) != 0)) {
							this->_ClearExchangeStatus();
							_ClearExchangeStatus(with->id_);
							return;
						}
					}
					iWeightLeftA = this->_iCalcMaxLoad() - this->iCalcTotalWeight();
					iWeightLeftB = with->_iCalcMaxLoad() - with->iCalcTotalWeight();
					//Calculate weight for items
					iItemWeightA = 0;
					for (int i = 0; i < this->iExchangeCount; i++) {
						iItemWeightA = iGetItemWeight(*this->m_pItemList[this->m_cExchangeItemIndex[i]],
								  this->m_iExchangeItemAmount[i]);
					}
					iItemWeightB = 0;
					for (int i = 0; i < with->iExchangeCount; i++) {
						iItemWeightB = iGetItemWeight(*with->m_pItemList[with->m_cExchangeItemIndex[i]],
								  with->m_iExchangeItemAmount[i]);
					}
					//See if the other person can take the item weightload
					if ((iWeightLeftA < iItemWeightB) || (iWeightLeftB < iItemWeightA)) {
						this->_ClearExchangeStatus();
						_ClearExchangeStatus(with->id_);
						return;
					}
					for (int i = 0; i < this->iExchangeCount; i++) {
						if ((this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							if (this->m_iExchangeItemAmount[i] > this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_dwCount) {
								this->_ClearExchangeStatus();
								_ClearExchangeStatus(with->id_);
								return;
							}
							pItemA[i] = new class CItem;
							_bInitItemAttr(*pItemA[i], this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cName);
							pItemA[i]->m_dwCount = this->m_iExchangeItemAmount[i];
							pItemAcopy[i] = new class CItem;
							_bInitItemAttr(*pItemAcopy[i], this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemAcopy[i], pItemA[i]);
							pItemAcopy[i]->m_dwCount = this->m_iExchangeItemAmount[i];
						} else {
							pItemA[i] = (class CItem *)& * this->m_pItemList[this->m_cExchangeItemIndex[i]];
							pItemA[i]->m_dwCount = this->m_iExchangeItemAmount[i];
							pItemAcopy[i] = new class CItem;
							_bInitItemAttr(*pItemAcopy[i], this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemAcopy[i], pItemA[i]);
							pItemAcopy[i]->m_dwCount = this->m_iExchangeItemAmount[i];
						}
					}
					for (int i = 0; i < with->iExchangeCount; i++) {
						if ((with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							if (with->m_iExchangeItemAmount[i] > with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_dwCount) {
								this->_ClearExchangeStatus();
								_ClearExchangeStatus(with->id_);
								return;
							}
							pItemB[i] = new class CItem;
							_bInitItemAttr(*pItemB[i], with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cName);
							pItemB[i]->m_dwCount = with->m_iExchangeItemAmount[i];
							pItemBcopy[i] = new class CItem;
							_bInitItemAttr(*pItemBcopy[i], with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemBcopy[i], pItemB[i]);
							pItemBcopy[i]->m_dwCount = with->m_iExchangeItemAmount[i];
						} else {
							pItemB[i] = (class CItem *)&*with->m_pItemList[with->m_cExchangeItemIndex[i]];
							pItemB[i]->m_dwCount = with->m_iExchangeItemAmount[i];
							pItemBcopy[i] = new class CItem;
							_bInitItemAttr(*pItemBcopy[i], with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemBcopy[i], pItemB[i]);
							pItemBcopy[i]->m_dwCount = with->m_iExchangeItemAmount[i];
						}
					}
					for (int i = 0; i < with->iExchangeCount; i++) {
						this->bAddItem(pItemB[i], 0);
						_bItemLog(DEF_ITEMLOG_EXCHANGE, with->id_, iClientH, pItemBcopy[i]);
						delete pItemBcopy[i];
						pItemBcopy[i] = nullptr;
						if ((with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							//
							iAmountLeft = (int) with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_dwCount - with->m_iExchangeItemAmount[i];
							if (iAmountLeft < 0) iAmountLeft = 0;
							// v1.41 !!!
							SetItemCount(with->id_, with->m_cExchangeItemIndex[i], iAmountLeft);
							// with->m_pItemList[with->m_cExchangeItemIndex]->m_cName, iAmountLeft);
							//
						} else {
							with->ReleaseItemHandler(with->m_cExchangeItemIndex[i], true);
							with->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, with->m_cExchangeItemIndex[i], with->m_iExchangeItemAmount[i], 0, this->m_cCharName);
							with->m_pItemList[with->m_cExchangeItemIndex[i]] = nullptr;
						}
					}
					for (int i = 0; i < this->iExchangeCount; i++) {
						bAddItem(with->id_, pItemA[i], 0);
						_bItemLog(DEF_ITEMLOG_EXCHANGE, iClientH, with->id_, pItemAcopy[i]);
						delete pItemAcopy[i];
						pItemAcopy[i] = nullptr;
						if ((this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							//
							iAmountLeft = (int) this->m_pItemList[this->m_cExchangeItemIndex[i]]->m_dwCount - this->m_iExchangeItemAmount[i];
							if (iAmountLeft < 0) iAmountLeft = 0;
							// v1.41 !!!
							this->SetItemCount(this->m_cExchangeItemIndex[i], iAmountLeft);
							// this->m_pItemList[this->m_cExchangeItemIndex]->m_cName, iAmountLeft);
							//
						} else {
							this->ReleaseItemHandler(this->m_cExchangeItemIndex[i], true);
							this->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, this->m_cExchangeItemIndex[i], this->m_iExchangeItemAmount[i], 0, with->m_cCharName);
							this->m_pItemList[this->m_cExchangeItemIndex[i]] = nullptr;
						}
					}
					this->m_bIsExchangeMode = false;
					this->m_bIsExchangeConfirm = false;
					this->exchangingWith_.reset();
					this->iExchangeCount = 0;
					with->m_bIsExchangeMode = false;
					with->m_bIsExchangeConfirm = false;
					with->exchangingWith_.reset();
					with->iExchangeCount = 0;
					for (int i = 0; i < 4; i++) {
						this->m_cExchangeItemIndex[i] = -1;
						with->m_cExchangeItemIndex[i] = -1;
					}
					this->SendNotifyMsg(0, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, 0, 0, 0, nullptr);
					with->SendNotifyMsg(0, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, 0, 0, 0, nullptr);
					this->iCalcTotalWeight();
					with->iCalcTotalWeight();
					return;
				}
			}
		}
	}
}

int CClient::_iGetItemSpaceLeft() {
	int i;
	int iTotalItem;
	iTotalItem = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) iTotalItem++;
	return (DEF_MAXITEMS - iTotalItem);
}

bool CClient::bAddItem(CItem * pItem, char /*cMode*/) {
	char * cp, cData[256];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	int iEraseReq;
	std::memset(cData, 0, sizeof (cData));
	if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 1;
		cp++;
		memcpy(cp, pItem->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;
		*cp = pItem->m_cItemType;
		cp++;
		*cp = pItem->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;
		*cp = pItem->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;
		*cp = pItem->m_cItemColor;
		cp++;
		*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		/*
		 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
		cp++;
		 */
		if (iEraseReq == 1) {
			//testcode
			//wsprintf(G_cTxt, "AddItem: Delete (%s)", pItem->m_cName);
			//PutLogFileList(G_cTxt);
			delete pItem;
			pItem = nullptr;
		}
		this->m_pXSock->iSendMsg(cData, 53);
		return true;
	} else {
		this->map_->bSetItem(this->m_sX,
				  this->m_sY,
				  pItem);
		this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  this->m_sX, this->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
		this->m_pXSock->iSendMsg(cData, 6);
		return true;
	}
	return false;
}

int CClient::_iTalkToNpcResult_Cityhall(int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange) {
	int iQuest;
	int iEraseReq;
	int iExp;
	class CItem * pItem;
	// Return Code
	if (this->markedForDeletion_) return 0;
	/*if (((memcmp(this->map_->m_cName, "cityhall_1", 10) != 1) ||
		(memcmp(this->map_->m_cName, "cityhall_2", 10) != 1)) &&
		(memcmp(m_pClientList[i]->m_cLocation, "NONE",4) != 0)) {
			wsprintf(G_cTxt, "Traveller hack: (%s) Player: (%s) - is talking to William from a remote map.", this->m_cIPaddress, this->m_cCharName);
			PutHackLogFileList(G_cTxt);
			this->DeleteClient(true, true);
			return 0;
	}*/
	if (this->m_iQuest != 0) {
		if (m_pQuestConfigList[this->m_iQuest] == nullptr) return -4;
		else if (m_pQuestConfigList[this->m_iQuest]->m_iFrom == 4) {
			if (this->m_bIsQuestCompleted == true) {
				if ((this->m_iQuestRewardType > 0) &&
						  (m_pItemConfigList[this->m_iQuestRewardType] != nullptr)) {
					pItem = new class CItem;
					_bInitItemAttr(*pItem, m_pItemConfigList[this->m_iQuestRewardType]->m_cName);
					pItem->m_dwCount = this->m_iQuestRewardAmount;
					if (this->_bCheckItemReceiveCondition(pItem) == true) {
						this->_bAddClientItemList(pItem, &iEraseReq);
						this->SendItemNotifyMsg(DEF_NOTIFY_ITEMOBTAINED, pItem, 0);
						if (iEraseReq == 1) delete pItem;
						this->m_iContribution += m_pQuestConfigList[this->m_iQuest]->m_iContribution;
						this->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, this->m_iQuestRewardAmount,
								  m_pItemConfigList[this->m_iQuestRewardType]->m_cName, this->m_iContribution);
						this->_ClearQuestStatus();
						return -5;
					} else {
						delete pItem;
						this->SendItemNotifyMsg(DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
						this->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 0, this->m_iQuestRewardAmount,
								  m_pItemConfigList[this->m_iQuestRewardType]->m_cName, this->m_iContribution);
						return -5;
					}
				} else if (this->m_iQuestRewardType == -1) {
					this->m_iExpStock += this->m_iQuestRewardAmount;
					this->m_iContribution += m_pQuestConfigList[this->m_iQuest]->m_iContribution;
					this->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, this->m_iQuestRewardAmount,
							  "°æÇèÄ¡              ", this->m_iContribution);
					this->_ClearQuestStatus();
					return -5;
				} else if (this->m_iQuestRewardType == -2) {
					//***
					iExp = iDice(1, (10 * this->m_iLevel));
					//***
					iExp = iExp * this->m_iQuestRewardAmount;
					this->m_iExpStock += iExp;
					this->m_iContribution += m_pQuestConfigList[this->m_iQuest]->m_iContribution;
					this->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, iExp,
							  "°æÇèÄ¡              ", this->m_iContribution);
					this->_ClearQuestStatus();
					return -5;
				} else {
					this->m_iContribution += m_pQuestConfigList[this->m_iQuest]->m_iContribution;
					this->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, 0,
							  "                     ", this->m_iContribution);
					this->_ClearQuestStatus();
					return -5;
				}
			} else return -1;
		}
		return -4;
	}
	if (memcmp(this->m_cLocation, this->map_->m_cLocationName, 10) == 0) {
		if (this->m_iPKCount > 0) return -3;
		iQuest = this->__iSearchForQuest(4, pQuestType, pMode, pRewardType, pRewardAmount, pContribution, pTargetName, pTargetType, pTargetCount, pX, pY, pRange);
		if (iQuest <= 0) return -4;
		return iQuest;
	} else return -2;
	return -4;
}

int CClient::_iTalkToNpcResult_Guard(int * /*pQuestType*/, int * /*pMode*/, int * /*pRewardType*/, int * /*pRewardAmount*/, int * /*pContribution*/, char * /*pTargetName*/, int * /*pTargetType*/, int * /*pTargetCount*/, int * /*pX*/, int * /*pY*/, int * /*pRange*/) {
	if (this->markedForDeletion_) return 0;
	if (memcmp(this->m_cLocation, "are", 3) == 0) {
		if (memcmp(this->map_->m_cName, "aresden", 7) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (200), 0, 0, nullptr, 0);
			return 1000;
		} else
			if (memcmp(this->map_->m_cName, "elv", 3) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (201), 0, 0, nullptr, 0);
			return 1001;
		}
	} else
		if (memcmp(this->m_cLocation, "elv", 3) == 0) {
		if (memcmp(this->map_->m_cName, "aresden", 7) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (202), 0, 0, nullptr, 0);
			return 1002;
		} else
			if (memcmp(this->map_->m_cName, "elv", 3) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (203), 0, 0, nullptr, 0);
			return 1003;
		}
	} else
		if (memcmp(this->m_cLocation, "NONE", 4) == 0) {
		if (memcmp(this->map_->m_cName, "aresden", 7) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (204), 0, 0, nullptr, 0);
			return 1004;
		} else
			if (memcmp(this->map_->m_cName, "elvine", 6) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (205), 0, 0, nullptr, 0);
			return 1005;
		} else
			if (memcmp(this->map_->m_cName, "default", 7) == 0) {
			this->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (206), 0, 0, nullptr, 0);
			return 1006;
		}
	}
	return 0;
}

int CClient::__iSearchForQuest(int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange) {
	int i;
	int iQuestList[DEF_MAXQUESTTYPE], iIndex, iQuest, iReward, iQuestIndex;
	if (this->markedForDeletion_) return -1;
	iIndex = 0;
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		iQuestList[i] = -1;
	for (i = 1; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != nullptr) {
			if (m_pQuestConfigList[i]->m_iFrom != iWho) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_cSide != this->m_cSide) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMinLevel > this->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMaxLevel < this->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iReqContribution > this->m_iContribution) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iRequiredSkillNum != -1) {
				if (this->m_cSkillMastery[m_pQuestConfigList[i]->m_iRequiredSkillNum] <
						  m_pQuestConfigList[i]->m_iRequiredSkillLevel) goto SFQ_SKIP;
			}
			if ((m_bIsCrusadeMode) && (m_pQuestConfigList[i]->m_iAssignType != 1)) goto SFQ_SKIP;
			if ((!m_bIsCrusadeMode) && (m_pQuestConfigList[i]->m_iAssignType == 1)) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iContributionLimit < this->m_iContribution) goto SFQ_SKIP;
			iQuestList[iIndex] = i;
			iIndex++;
SFQ_SKIP:
			;
		}
	if (iIndex == 0) return -1;
	iQuest = (iDice(1, iIndex)) - 1;
	iQuestIndex = iQuestList[iQuest];
	iReward = iDice(1, 3);
	*pMode = m_pQuestConfigList[iQuestIndex]->m_iResponseMode;
	*pRewardType = m_pQuestConfigList[iQuestIndex]->m_iRewardType[iReward];
	*pRewardAmount = m_pQuestConfigList[iQuestIndex]->m_iRewardAmount[iReward];
	*pContribution = m_pQuestConfigList[iQuestIndex]->m_iContribution;
	strcpy(pTargetName, m_pQuestConfigList[iQuestIndex]->m_cTargetName);
	*pX = m_pQuestConfigList[iQuestIndex]->m_sX;
	*pY = m_pQuestConfigList[iQuestIndex]->m_sY;
	*pRange = m_pQuestConfigList[iQuestIndex]->m_iRange;
	*pTargetType = m_pQuestConfigList[iQuestIndex]->m_iTargetType;
	*pTargetCount = m_pQuestConfigList[iQuestIndex]->m_iMaxCount;
	*pQuestType = m_pQuestConfigList[iQuestIndex]->m_iType;
	return iQuestIndex;
}

void CClient::QuestAcceptedHandler() {
	int iIndex;
	if (this->markedForDeletion_) return;
	// Does the quest exist ??
	if (m_pQuestConfigList[this->m_iAskedQuest] == nullptr) return;
	if (m_pQuestConfigList[this->m_iAskedQuest]->m_iAssignType == 1) {
		switch (m_pQuestConfigList[this->m_iAskedQuest]->m_iType) {
			case 10:
				this->_ClearQuestStatus();
				this->RequestTeleportHandler("2   ", m_pQuestConfigList[this->m_iAskedQuest]->m_cTargetName,
						  m_pQuestConfigList[this->m_iAskedQuest]->m_sX, m_pQuestConfigList[this->m_iAskedQuest]->m_sY);
				return;
		}
	}
	this->m_iQuest = this->m_iAskedQuest;
	iIndex = this->m_iQuest;
	this->m_iQuestID = m_pQuestConfigList[iIndex]->m_iQuestID;
	this->m_iCurQuestCount = 0;
	this->m_bIsQuestCompleted = false;
	this->_CheckQuestEnvironment();
	this->_SendQuestContents();
}

void CClient::_SendQuestContents() {
	int iWho;
	int iIndex;
	int iQuestType;
	int iContribution;
	int iTargetType;
	int iTargetCount;
	int iX;
	int iY;
	int iRange;
	int iQuestCompleted;
	char cTargetName[21];
	if (this->markedForDeletion_) return;
	iIndex = this->m_iQuest;
	if (iIndex == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_QUESTCONTENTS, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
	} else {
		iWho = m_pQuestConfigList[iIndex]->m_iFrom;
		iQuestType = m_pQuestConfigList[iIndex]->m_iType;
		iContribution = m_pQuestConfigList[iIndex]->m_iContribution;
		iTargetType = m_pQuestConfigList[iIndex]->m_iTargetType;
		iTargetCount = m_pQuestConfigList[iIndex]->m_iMaxCount;
		iX = m_pQuestConfigList[iIndex]->m_sX;
		iY = m_pQuestConfigList[iIndex]->m_sY;
		iRange = m_pQuestConfigList[iIndex]->m_iRange;
		std::memset(cTargetName, 0, sizeof (cTargetName));
		memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
		iQuestCompleted = (int) this->m_bIsQuestCompleted;
		this->SendNotifyMsg(0, DEF_NOTIFY_QUESTCONTENTS, iWho, iQuestType, iContribution, nullptr,
				  iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted, cTargetName);
	}
}

void CClient::_CheckQuestEnvironment() {
	int iIndex;
	char cTargetName[21];
	if (this->markedForDeletion_) return;
	iIndex = this->m_iQuest;
	if (iIndex == 0) return;
	if (m_pQuestConfigList[iIndex] == nullptr) return;
	if (iIndex >= 35 && iIndex <= 40) {
		this->m_iQuest = 0;
		this->m_iQuestID = 0;
		this->m_iQuestRewardAmount = 0;
		this->m_iQuestRewardType = 0;
		this->SendNotifyMsg(0, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, nullptr);
		return;
	}
	if (m_pQuestConfigList[iIndex]->m_iQuestID != this->m_iQuestID) {
		this->m_iQuest = 0;
		this->m_iQuestID = 0;
		this->m_iQuestRewardAmount = 0;
		this->m_iQuestRewardType = 0;
		this->SendNotifyMsg(0, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, nullptr);
		return;
	}
	switch (m_pQuestConfigList[iIndex]->m_iType) {
		case DEF_QUESTTYPE_MONSTERHUNT:
		case DEF_QUESTTYPE_GOPLACE:
			std::memset(cTargetName, 0, sizeof (cTargetName));
			memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
			if (memcmp(this->map_->m_cName, cTargetName, 10) == 0)
				this->m_bQuestMatchFlag_Loc = true;
			else this->m_bQuestMatchFlag_Loc = false;
			break;
	}
}

bool CClient::_bCheckIsQuestCompleted() {
	int iQuestIndex;
	if (this->markedForDeletion_) return false;
	if (this->m_bIsQuestCompleted == true) return false;
	iQuestIndex = this->m_iQuest;
	if (iQuestIndex == 0) return false;
	if (m_pQuestConfigList[iQuestIndex] != nullptr) {
		switch (m_pQuestConfigList[iQuestIndex]->m_iType) {
			case DEF_QUESTTYPE_MONSTERHUNT:
				if ((this->m_bQuestMatchFlag_Loc == true) &&
						  (this->m_iCurQuestCount >= m_pQuestConfigList[iQuestIndex]->m_iMaxCount)) {
					this->m_bIsQuestCompleted = true;
					this->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOMPLETED, 0, 0, 0, nullptr);
					return true;
				}
				break;
			case DEF_QUESTTYPE_GOPLACE:
				if ((this->m_bQuestMatchFlag_Loc == true) &&
						  (this->m_sX >= m_pQuestConfigList[iQuestIndex]->m_sX - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						  (this->m_sX <= m_pQuestConfigList[iQuestIndex]->m_sX + m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						  (this->m_sY >= m_pQuestConfigList[iQuestIndex]->m_sY - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						  (this->m_sY <= m_pQuestConfigList[iQuestIndex]->m_sY + m_pQuestConfigList[iQuestIndex]->m_iRange)) {
					this->m_bIsQuestCompleted = true;
					this->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOMPLETED, 0, 0, 0, nullptr);
					return true;
				}
				break;
		}
	}
	return false;
}

void CClient::SendItemNotifyMsg(uint16_t wMsgType, CItem *pItem, int iV1) {
	char * cp, cData[512];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	if (this->markedForDeletion_) return;
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_NOTIFY;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;
	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	switch (wMsgType) {
		case DEF_NOTIFY_ITEMOBTAINED:
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char) 0;
			cp++;
			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor; // v1.4
			cp++;
			*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
			cp++;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;
			/*
			 *cp = (char)(pItem->m_dwAttribute & 0x00000001);
			cp++;
			 */
			this->m_pXSock->iSendMsg(cData, 53);
			break;
		case DEF_NOTIFY_ITEMPURCHASED:
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char) 0;
			cp++;
			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor;
			cp++;
			wp = (uint16_t *) cp;
			*wp = iV1; // (iCost - iDiscountCost);
			this->m_pXSock->iSendMsg(cData, 48);
			break;
		case DEF_NOTIFY_CANNOTCARRYMOREITEM:
			this->m_pXSock->iSendMsg(cData, 6);
			break;
	}
}

bool CClient::_bCheckItemReceiveCondition(CItem *pItem) {
	int i;
	if (this->markedForDeletion_) return false;
	/*
	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {

		if ((this->m_iCurWeightLoad + (pItem->m_wWeight * pItem->m_dwCount)) > (uint32_t)this->_iCalcMaxLoad())
			return false;
	}
	else {

		if ((this->m_iCurWeightLoad + pItem->m_wWeight) > this->_iCalcMaxLoad())
			return false;
	}
	 */
	if (this->m_iCurWeightLoad + iGetItemWeight(*pItem, pItem->m_dwCount) > (uint32_t) this->_iCalcMaxLoad())
		return false;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] == nullptr) return true;
	return false;
}

void CClient::BuildItemHandler(char *pData) {
	char * cp, cName[21], cElementItemID[6], cColor;
	int i;
	int x;
	int z;
	int iMatch;
	int iCount;
	int iPlayerSkillLevel;
	int iResult;
	int iTotalValue;
	int iResultValue;
	int iTemp;
	int iItemCount[DEF_MAXITEMS];
	class CItem * pItem;
	bool bFlag;
	bool bItemFlag[6];
	double dV1;
	double dV2;
	double dV3;
	uint32_t dwTemp;
	uint32_t dwTemp2;
	uint32_t dwType;
	uint32_t dwValue;
	uint16_t wTemp;
	if (this->markedForDeletion_) return;
	this->m_iSkillMsgRecvCount++;
	cp = (char *) (pData + 11);
	std::memset(cName, 0, sizeof (cName));
	memcpy(cName, cp, 20);
	cp += 20;
	//testcode
	//PutLogList(cName);
	std::memset(cElementItemID, 0, sizeof (cElementItemID));
	cElementItemID[0] = *cp;
	cp++;
	cElementItemID[1] = *cp;
	cp++;
	cElementItemID[2] = *cp;
	cp++;
	cElementItemID[3] = *cp;
	cp++;
	cElementItemID[4] = *cp;
	cp++;
	cElementItemID[5] = *cp;
	cp++;
	bFlag = true;
	while (bFlag) {
		bFlag = false;
		for (i = 0; i <= 4; i++)
			if ((cElementItemID[i] == -1) && (cElementItemID[i + 1] != -1)) {
				cElementItemID[i] = cElementItemID[i + 1];
				cElementItemID[i + 1] = -1;
				bFlag = true;
			}
	}
	for (i = 0; i < 6; i++) bItemFlag[i] = false;
	//testcode
	//wsprintf(G_cTxt, "%d %d %d %d %d %d", cElementItemID[0], cElementItemID[1], cElementItemID[2],
	//        cElementItemID[3], cElementItemID[4], cElementItemID[5]);
	//PutLogList(G_cTxt);
	iPlayerSkillLevel = this->m_cSkillMastery[13];
	iResult = iDice(1, 100);
	if (iResult > iPlayerSkillLevel) {
		this->SendNotifyMsg(0, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, nullptr);
		return;
	}
	for (i = 0; i < 6; i++)
		if (cElementItemID[i] != -1) {
			if ((cElementItemID[i] < 0) || (cElementItemID[i] > DEF_MAXITEMS)) return;
			if (this->m_pItemList[cElementItemID[i]] == nullptr) return;
		}
	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		if (m_pBuildItemList[i] != nullptr) {
			if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0) {
				if (m_pBuildItemList[i]->m_iSkillLimit > this->m_cSkillMastery[13]) return;
				for (x = 0; x < DEF_MAXITEMS; x++)
					if (this->m_pItemList[x] != nullptr)
						iItemCount[x] = this->m_pItemList[x]->m_dwCount;
					else iItemCount[x] = 0;
				iMatch = 0;
				iTotalValue = 0;
				for (x = 0; x < 6; x++) {
					if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0) {
						iMatch++;
					} else {
						for (z = 0; z < 6; z++)
							if ((cElementItemID[z] != -1) && (!bItemFlag[z])) {
								if ((this->m_pItemList[cElementItemID[z]]->m_sIDnum == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
										  (this->m_pItemList[cElementItemID[z]]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
										  (iItemCount[cElementItemID[z]] > 0)) {
									iTemp = this->m_pItemList[cElementItemID[z]]->m_sItemSpecEffectValue2;
									if (iTemp > this->m_cSkillMastery[13]) {
										iTemp = iTemp - (iTemp - this->m_cSkillMastery[13]) / 2;
									}
									iTotalValue += (iTemp * m_pBuildItemList[i]->m_iMaterialItemValue[x]);
									iItemCount[cElementItemID[z]] -= m_pBuildItemList[i]->m_iMaterialItemCount[x];
									iMatch++;
									bItemFlag[z] = true;
									goto BIH_LOOPBREAK;
								}
							}
BIH_LOOPBREAK:
						;
					}
				}
				if (iMatch != 6) {
					this->SendNotifyMsg(0, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, nullptr);
					return;
				}
				dV2 = (double) m_pBuildItemList[i]->m_iMaxValue;
				if (iTotalValue <= 0)
					dV3 = 1.0f;
				else dV3 = (double) iTotalValue;
				dV1 = (double) (dV3 / dV2)*100.0f;
				iTotalValue = (int) dV1;
				pItem = new class CItem;
				if (_bInitItemAttr(*pItem, m_pBuildItemList[i]->m_cName) == false) {
					delete pItem;
					return;
				}
				dwTemp = pItem->m_dwAttribute;
				dwTemp = dwTemp & 0xFFFFFFFE;
				dwTemp = dwTemp | 0x00000001;
				pItem->m_dwAttribute = dwTemp;
				if (pItem->m_cItemType == DEF_ITEMTYPE_MATERIAL) {
					iTemp = iDice(1, (iPlayerSkillLevel / 2) + 1) - 1;
					pItem->m_sItemSpecEffectValue2 = (iPlayerSkillLevel / 2) + iTemp;
					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = timeGetTime();
				} else {
					dwTemp = pItem->m_dwAttribute;
					dwTemp = dwTemp & 0x0000FFFF;
					dwTemp2 = (uint16_t) m_pBuildItemList[i]->m_wAttribute;
					dwTemp2 = dwTemp2 << 16;
					dwTemp = dwTemp | dwTemp2;
					pItem->m_dwAttribute = dwTemp;
					iResultValue = (iTotalValue - m_pBuildItemList[i]->m_iAverageValue);
					if (iResultValue > 0) {
						dV2 = (double) iResultValue;
						dV3 = (double) (100 - m_pBuildItemList[i]->m_iAverageValue);
						dV1 = (dV2 / dV3)*100.0f;
						pItem->m_sItemSpecEffectValue2 = (int) dV1;
					} else if (iResultValue < 0) {
						dV2 = (double) (iResultValue);
						dV3 = (double) (m_pBuildItemList[i]->m_iAverageValue);
						dV1 = (dV2 / dV3)*100.0f;
						pItem->m_sItemSpecEffectValue2 = (int) dV1;
					} else pItem->m_sItemSpecEffectValue2 = 0;
					dV2 = (double) pItem->m_sItemSpecEffectValue2;
					dV3 = (double) pItem->m_wMaxLifeSpan;
					dV1 = (dV2 / 100.0f) * dV3;
					iTemp = (int) pItem->m_wMaxLifeSpan;
					iTemp += (int) dV1;
					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = timeGetTime();
					if (iTemp <= 0)
						wTemp = 1;
					else wTemp = (uint16_t) iTemp;
					if (wTemp <= pItem->m_wMaxLifeSpan * 2) {
						pItem->m_wMaxLifeSpan = wTemp;
						pItem->m_sItemSpecEffectValue1 = (short) wTemp;
						pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
					} else pItem->m_sItemSpecEffectValue1 = (short) pItem->m_wMaxLifeSpan;
					if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
						iResult = iDice(1, 10000);
						if ((iResult >= 1) && (iResult <= 299)) {
							dwType = 10;
							cColor = 5;
						} else if ((iResult >= 300) && (iResult <= 999)) {
							dwType = 12;
							cColor = 5;
						} else if ((iResult >= 1000) && (iResult <= 2499)) {
							dwType = 7;
							cColor = 6;
						} else if ((iResult >= 2500) && (iResult <= 4499)) {
							dwType = 7;
							cColor = 6;
						} else if ((iResult >= 4500) && (iResult <= 6499)) {
							dwType = 7;
							cColor = 6;
						} else if ((iResult >= 6500) && (iResult <= 8099)) {
							dwType = 7;
							cColor = 6;
						} else if ((iResult >= 8100) && (iResult <= 9699)) {
							dwType = 9;
							cColor = 8;
						} else if ((iResult >= 9700) && (iResult <= 10000)) {
							dwType = 9;
							cColor = 8;
						}
						pItem->m_cItemColor = cColor;
						iResult = iDice(1, 30000);
						if ((iResult >= 1) && (iResult < 10000)) dwValue = 2; // 10000/29348 = 34%
						else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
						else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 2; // 4356/29348 = 14.8%
						else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
						else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 4; // 1897/29348 = 6.4%
						else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 4; // 1252/29348 = 4.2%
						else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 8; // 826/29348 = 2.8%
						else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
						else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 8; // 360/29348 = 1.2%
						else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 10; // 237/29348 = 0.8%
						else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 11; // 156/29348 = 0.5%
						else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 12; // 103/29348 = 0.3%
						else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 13; // 68/29348 = 0.1%
						else dwValue = 1; // v2.03 906
						switch (dwType) {
							case 1:
								if (dwValue <= 5) dwValue = 5;
								break;
							case 2:
								if (dwValue <= 4) dwValue = 4;
								break;
							case 6:
								if (dwValue <= 4) dwValue = 4;
								break;
							case 8:
								if (dwValue <= 2) dwValue = 2;
								break;
						}
						if (dwValue > 7) dwValue = 7;
						pItem->m_dwAttribute = 0;
						dwType = dwType << 20;
						dwValue = dwValue << 16;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					}
					if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE) {
						iResult = iDice(1, 10000); //AQUI OGEID
						if ((iResult >= 1) && (iResult <= 5999)) dwType = 8;
						else if ((iResult >= 8100) && (iResult <= 8999)) dwType = 6;
						else if ((iResult >= 9000) && (iResult <= 9554)) dwType = 11; //dwType = 11;
						else if ((iResult >= 9555) && (iResult <= 10000)) dwType = 12; //dwType = 12;
						iResult = iDice(1, 30000);
						if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
						else if ((iResult >= 12000) && (iResult < 17400)) dwValue = 1; // 6600/29348 = 22.4%
						else if ((iResult >= 18400) && (iResult < 22400)) dwValue = 1; // 4356/29348 = 14.8%
						else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 2; // 2874/29348 = 9.7%
						else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 2; // 1897/29348 = 6.4%
						else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 2; // 1252/29348 = 4.2%
						else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 4; // 826/29348 = 2.8%
						else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 4; // 545/29348 = 1.85%
						else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 4; // 360/29348 = 1.2%
						else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 6; // 237/29348 = 0.8%
						else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 6; // 156/29348 = 0.5%
						else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 10; // 103/29348 = 0.3%
						else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 10; // 68/29348 = 0.1%
						else dwValue = 1; // v2.03 906
						switch (dwType) {
							case 6:
								if (dwValue <= 4) dwValue = 4;
								break;
							case 8:
								if (dwValue <= 2) dwValue = 2;
								break;
							case 11:
							case 12:
								// v2.04
								dwValue = (dwValue + 1) / 2;
								if (dwValue < 1) dwValue = 1;
								if (dwValue > 2) dwValue = 2;
								break;
						}
						if (dwValue > 7) dwValue = 7;
						pItem->m_dwAttribute = 0;
						dwType = dwType << 20;
						dwValue = dwValue << 16;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
						if (iDice(1, 10000) >= 3000) {
							iResult = iDice(1, 10000);
							if ((iResult >= 1) && (iResult <= 999)) {
								dwType = 8;
								cColor = 15;
							} else if ((iResult >= 1000) && (iResult <= 3999)) {
								dwType = 8;
								cColor = 15;
							} else if ((iResult >= 4000) && (iResult <= 5499)) {
								dwType = 8;
								cColor = 15;
							} else if ((iResult >= 5500) && (iResult <= 6499)) {
								dwType = 9;
								cColor = 10;
							} else if ((iResult >= 6500) && (iResult <= 7499)) {
								dwType = 9;
								cColor = 10;
							} else if ((iResult >= 7500) && (iResult <= 9399)) {
								dwType = 9;
								cColor = 10;
							} else if ((iResult >= 9400) && (iResult <= 9799)) {
								dwType = 8;
								cColor = 15;
							} else if ((iResult >= 9800) && (iResult <= 10000)) {
								dwType = 9;
								cColor = 10;
							}
							pItem->m_cItemColor = cColor;
							iResult = iDice(1, 30000);
							if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
							else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 1; // 6600/29348 = 22.4%
							else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 1; // 4356/29348 = 14.8%
							else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 2; // 2874/29348 = 9.7%
							else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 2; // 1897/29348 = 6.4%
							else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 2; // 1252/29348 = 4.2%
							else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 2; // 826/29348 = 2.8%
							else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 4; // 545/29348 = 1.85%
							else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 6; // 360/29348 = 1.2%
							else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 6; // 237/29348 = 0.8%
							else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 8; // 156/29348 = 0.5%
							else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 8; // 103/29348 = 0.3%
							else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 8; // 68/29348 = 0.1%
							else dwValue = 1; // v2.03 906
							switch (dwType) {
								case 1:
								case 3:
								case 7:
								case 8:
								case 9:
									if (dwValue <= 3) dwValue = 3;
									break;
							}
							if (dwValue > 7) dwValue = 7;
							dwType = dwType << 12;
							dwValue = dwValue << 8;
							pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
						}
					}
				}
				//testcode
				wsprintf(G_cTxt, "Custom-Item(%s) Value(%d) Life(%d/%d)", pItem->m_cName, pItem->m_sItemSpecEffectValue2, pItem->m_wCurLifeSpan, pItem->m_wMaxLifeSpan);
				PutLogList(G_cTxt);
				this->bAddItem(pItem, 0);
				this->SendNotifyMsg(0, DEF_NOTIFY_BUILDITEMSUCCESS, pItem->m_sItemSpecEffectValue2, pItem->m_cItemType, 0, nullptr);
#ifdef DEF_TAIWANLOG
				_bItemLog(DEF_ITEMLOG_MAKE, iClientH, (int) - 1, pItem);
#endif
				for (x = 0; x < 6; x++)
					if (cElementItemID[x] != -1) {
						if (this->m_pItemList[cElementItemID[x]] == nullptr) {
							// ### BUG POINT!!!
							wsprintf(G_cTxt, "(?) Char(%s) ElementItemID(%d)", this->m_cCharName, cElementItemID[x]);
							PutLogFileList(G_cTxt);
						} else {
							iCount = this->m_pItemList[cElementItemID[x]]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
							if (iCount < 0) iCount = 0;
							this->SetItemCount(cElementItemID[x], iCount);
						}
					}
				if (m_pBuildItemList[i]->m_iMaxSkill > this->m_cSkillMastery[13])
					this->CalculateSSN_SkillIndex(13, 1);
				this->GetExp(iDice(1, (m_pBuildItemList[i]->m_iSkillLimit / 4))); //this->m_iExpStock += iDice(1, (m_pBuildItemList[i]->m_iSkillLimit/4));
				return;
			}
		}
	/*

	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
	if (m_pBuildItemList[i] != nullptr) {
		if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0) {
			if (m_pBuildItemList[i]->m_iSkillLimit > this->m_cSkillMastery[13]) return;

			iMatch = 0;
			for (x = 0; x < 6; x++) m_pBuildItemList[i]->m_iIndex[x] = -1;
			for (x = 0; x < DEF_MAXITEMS; x++) bItemFlag[x] = false;
			iTotalValue = 0;
			for (x = 0; x < 6; x++)
			if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0) iMatch++;
			else {
				bFlag = false;
				for (z = 0; z < DEF_MAXITEMS; z++)
				if ((this->m_pItemList[z] != nullptr)) {
					if ((this->m_pItemList[z]->m_sIDnum  == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
						(this->m_pItemList[z]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
						(!bItemFlag[z])) {

						m_pBuildItemList[i]->m_iIndex[x] = z;
						bItemFlag[z] = true;
						iTotalValue += (this->m_pItemList[z]->m_sItemSpecEffectValue1 * m_pBuildItemList[i]->m_iMaterialItemValue[x]);

						bFlag = true;
						goto BIH_LOOPBREAK;
					}
				}

BIH_LOOPBREAK:;
				if (bFlag) iMatch++;
			}

			if (iMatch != 6) {
				this->SendNotifyMsg(0,DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, nullptr);
				return;
			}
			//testcode
			wsprintf(G_cTxt, "iTotalValue(%d) MaxValue(%d)", iTotalValue, m_pBuildItemList[i]->m_iMaxValue);
			PutLogList(G_cTxt);
			dV2 = (double)m_pBuildItemList[i]->m_iMaxValue;
			if (iTotalValue <= 0)
				 dV3 = 1.0f;
			else dV3 = (double)iTotalValue;
			dV1 = (double)(dV3/dV2)*100.0f;
			iTotalValue = (int)dV1;
			//testcode
			wsprintf(G_cTxt, "iTotalValue(%d)", iTotalValue);
			PutLogList(G_cTxt);
			for (x = 0; x < 6; x++)
			if (m_pBuildItemList[i]->m_iIndex[x] != -1) {
				iIndex = m_pBuildItemList[i]->m_iIndex[x];
				iCount = this->m_pItemList[iIndex]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
				if (iCount < 0) iCount = 0;
				this->SetItemCount(iIndex, iCount);
			}
			pItem = new class CItem;
			if (_bInitItemAttr(pItem, m_pBuildItemList[i]->m_cName) == false) {
				delete pItem;
				return;
			}
			if (pItem->m_cItemType == DEF_ITEMTYPE_MATERIAL) {
				pItem->m_sItemSpecEffectValue1 = iDice(1, iPlayerSkillLevel);
				if (pItem->m_sItemSpecEffectValue1 < (iPlayerSkillLevel/2))
					pItem->m_sItemSpecEffectValue1 = (iPlayerSkillLevel/2);
			}
			else {
				dwTemp = pItem->m_dwAttribute;
				dwTemp = dwTemp | 0x00000001;
				pItem->m_dwAttribute = dwTemp;
				iResultValue = (iTotalValue - m_pBuildItemList[i]->m_iAverageValue);
				//testcode
				wsprintf(G_cTxt, "iResultValue(%d) %d %d", iResultValue, iTotalValue, m_pBuildItemList[i]->m_iAverageValue);
				PutLogList(G_cTxt);
				if (iResultValue == 0)
					 dV2 = 1.0f;
				else dV2 = (double)iResultValue;
				dV3 = (double)pItem->m_wMaxLifeSpan;
				dV1 = (dV2/100.0f)*dV3;
				iTemp  = (int)pItem->m_wMaxLifeSpan;
				iTemp += (int)dV1;
				if (iTemp <= 0)
					 wTemp = 1;
				else wTemp = (uint16_t)iTemp;
				if (wTemp <= pItem->m_wMaxLifeSpan*2) {
					pItem->m_wMaxLifeSpan = wTemp;
					pItem->m_wCurLifeSpan = wTemp;
					pItem->m_sItemSpecEffectValue1 = (short)wTemp;
				}
				else pItem->m_sItemSpecEffectValue1 = (short)pItem->m_wMaxLifeSpan;
				if (iResultValue > 0) {
					dV2 = (double)iResultValue;
					dV3 = (double)(100 - m_pBuildItemList[i]->m_iAverageValue);
					dV1 = (dV2/dV3)*100.0f;
					pItem->m_sItemSpecEffectValue2 = (int)dV1;
				}
				else if (iResultValue < 0) {
					dV2 = (double)abs(iResultValue);
					dV3 = (double)(m_pBuildItemList[i]->m_iAverageValue);
					dV1 = (dV2/dV3)*100.0f;
					pItem->m_sItemSpecEffectValue2 = -1*abs(100 - (int)dV1);
				}
				else pItem->m_sItemSpecEffectValue2 = 0;
			}
			//testcode
			wsprintf(G_cTxt, "Custom-Item(%s) Value(%d)", pItem->m_cName, pItem->m_sItemSpecEffectValue2);
			PutLogList(G_cTxt);
			this->bAddItem(pItem, nullptr);
			this->SendNotifyMsg(0,DEF_NOTIFY_BUILDITEMSUCCESS, 0, 0, 0, nullptr);
		}
	}
	 */
}

void CClient::AdminOrder_SetAttackMode(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	int i;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	/*

	if ((memcmp(this->map_->m_cName, "fightzone", 9) == 0) && (this->m_iSkillSSN[59] == 1)) {
		goto AOCC_SAM;
	}
	 */
	if (this->m_iAdminUserLevel < m_iAdminLevelSetAttackMode) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	//testcode
	//wsprintf(G_cTxt, "%s", token);
	//PutLogList(G_cTxt);
	if (token != nullptr) {
		if (token[0] == '1') {
			this->map_->m_bIsAttackEnabled = true;
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) && (m_pClientList[i]->map_ == this->map_))
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_GLOBALATTACKMODE, (int) this->map_->m_bIsAttackEnabled, 0, 0, nullptr);
		} else if (token[0] == '0') {
			this->map_->m_bIsAttackEnabled = false;
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) && (m_pClientList[i]->map_ == this->map_))
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_GLOBALATTACKMODE, (int) this->map_->m_bIsAttackEnabled, 0, 0, nullptr);
		}
	}
	delete pStrTok;
}

void CClient::AdminOrder_UnsummonAll() {
	int i;
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelUnsummonAll) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	for (i = 1; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {
			if ((m_pNpcList[i]->m_bIsSummoned == true) && (m_pNpcList[i]->m_bIsKilled == false))
				m_pNpcList[i]->this->NpcKilledHandler(DEF_OWNERTYPE_PLAYER, 0);
		}
}

void CClient::AdminOrder_UnsummonDemon() {
	int i;
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelUnsummonDemon) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	for (i = 1; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {
			if ((m_pNpcList[i]->m_sType == 31) && (m_pNpcList[i]->m_bIsKilled == false))
				m_pNpcList[i]->this->NpcKilledHandler(DEF_OWNERTYPE_PLAYER, 0);
		}
}

void CClient::AdminOrder_Summon(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cName_Master[10], cName_Slave[10], cNpcName[256], cWaypoint[11], cSA;
	int pX;
	int pY;
	int j;
	int iNum;
	int iNamingValue;
	class CStrTok * pStrTok;
	bool bMaster;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_bIsAdminCommandEnabled == false) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSummon) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	std::memset(cNpcName, 0, sizeof (cNpcName));
	strcpy(cNpcName, token);
	token = pStrTok->pGet();
	if (token != nullptr)
		iNum = atoi(token);
	else iNum = 1;
	if (iNum <= 0) iNum = 1;
	if (iNum >= 50) iNum = 50;
	cSA = 0;
	pX = this->m_sX;
	pY = this->m_sY;
	wsprintf(G_cTxt, "(!) Admin Order: Summon(%s)-(%d)", cNpcName, iNum);
	PutLogList(G_cTxt);
	iNamingValue = this->map_->iGetEmptyNamingValue();
	if (iNamingValue != -1) {
		std::memset(cName_Master, 0, sizeof (cName_Master));
		wsprintf(cName_Master, "XX%d", iNamingValue);
		cName_Master[0] = '_';
		cName_Master[1] = this->map_->id_ + 65;
		if ((bMaster = this->map_->bCreateNewNpc(cNpcName, cName_Master, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false, true)) == false) {
			this->map_->SetNamingValueEmpty(iNamingValue);
		}
	}
	for (j = 0; j < (iNum - 1); j++) {
		iNamingValue = this->map_->iGetEmptyNamingValue();
		if (iNamingValue != -1) {
			std::memset(cName_Slave, 0, sizeof (cName_Slave));
			wsprintf(cName_Slave, "XX%d", iNamingValue);
			cName_Slave[0] = '_';
			cName_Slave[1] = this->map_->id_ + 65;
			if (this->map_->bCreateNewNpc(cNpcName, cName_Slave, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false) == false) {
				this->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				bSetNpcFollowMode(cName_Slave, cName_Master, DEF_OWNERTYPE_NPC);
			}
		}
	}
	delete pStrTok;
}

void CClient::AdminOrder_SummonAll(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cLocation[11], cMapName[11];
	int pX;
	int pY;
	int i;
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_bIsAdminCommandEnabled == false) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSummonAll) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	pX = this->m_sX;
	pY = this->m_sY;
	memcpy(cMapName, this->map_->m_cName, 11);
	if (strlen(token) > 10)
		memcpy(cLocation, token, 10);
	else memcpy(cLocation, token, strlen(token));
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cLocation, token) == 0)) {
			m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, pX, pY);
		}
	wsprintf(G_cTxt, "GM Order(%s): PC(%s) Summoned to (%s)", this->m_cLocation,
			  cLocation, cMapName);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	delete pStrTok;
}

void CClient::AdminOrder_SummonPlayer(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cMapName[11], cName[11];
	int pX;
	int pY;
	int i;
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSummonPlayer) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	std::memset(cName, 0, sizeof (cName));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	pX = this->m_sX;
	pY = this->m_sY;
	memcpy(cMapName, this->map_->m_cName, 11);
	if (strlen(token) > 10)
		memcpy(cName, token, 10);
	else memcpy(cName, token, strlen(token));
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0)) {
			if (i == iClientH) {
				delete pStrTok;
				return;
			}
			m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, pX, pY);
			delete pStrTok;
			return;
		}//m_pClientList[i]->m_cCharName
	// find char on other hg's
	wsprintf(G_cTxt, "GM Order(%s): PC(%s) Summoned to (%s)", this->m_cCharName,
			  cName, cMapName);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	delete pStrTok;
}

void CClient::CheckSpecialEvent() {
	class CItem * pItem;
	char cItemName[21];
	int iEraseReq;
	if (this->markedForDeletion_) return;
	if (this->m_iSpecialEventID == 200081) {
		if (this->m_iLevel < 11) {
			this->m_iSpecialEventID = 0;
			return;
		}
		std::memset(cItemName, 0, sizeof (cItemName));
		strcpy(cItemName, "MemorialRing");
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {
			delete pItem;
		} else {
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				if (this->m_iCurWeightLoad < 0) this->m_iCurWeightLoad = 0;
				wsprintf(G_cTxt, "(*) Get MemorialRing  : Char(%s)", this->m_cCharName);
				PutLogFileList(G_cTxt);
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = this->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = this->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = this->m_sCharIDnum3;
				pItem->m_cItemColor = 9;
				this->m_iSpecialEventID = 0;
			}
		}
	}
}

void CClient::AdminOrder_DisconnectAll(char */*pData*/, uint32_t dwMsgSize) {
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_bIsAdminCommandEnabled == false) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelDisconnectAll) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	_iForcePlayerDisconect(DEF_MAXCLIENTS);
}

void CClient::RequestNoticementHandler(char * pData) {
	if (m_dwNoticementDataSize < 10) return;
	int * ip = (int *) (pData + DEF_INDEX2_MSGTYPE + 2);
	int iClientSize = *ip;
	if (iClientSize != m_dwNoticementDataSize) {
		char *cp = new char[m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2];
		ZeroMemory(cp, m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2);
		memcpy((cp + DEF_INDEX2_MSGTYPE + 2), m_pNoticementData, m_dwNoticementDataSize);
		uint32_t *dwp = (uint32_t *) (cp + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_NOTICEMENT;
		uint16_t *wp = (uint16_t *) (cp + DEF_INDEX2_MSGTYPE);
		*wp = DEF_MSGTYPE_REJECT;
		this->m_pXSock->iSendMsg(cp, m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2);
		delete cp;
	} else {
		char cData[120];
		std::memset(cData, 0, sizeof (cData));
		uint32_t *dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_NOTICEMENT;
		uint16_t *wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_MSGTYPE_CONFIRM;
		this->m_pXSock->iSendMsg(cData, 6);
	}
}

void CClient::_TamingHandler(int iSkillNum, MapPtr map, int dX, int dY) {
	int iSkillLevel;
	int iRange;
	int iTamingLevel;
	int iResult;
	int iX;
	int iY;
	short sOwnerH;
	char cOwnerType;
	if (this->markedForDeletion_) return;
	if (!map) return;
	iSkillLevel = (int) this->m_cSkillMastery[iSkillNum];
	iRange = iSkillLevel / 12;
	for (iX = dX - iRange; iX <= dX + iRange; iX++)
		for (iY = dY - iRange; iY <= dY + iRange; iY++) {
			sOwnerH = 0;
			if ((iX > 0) && (iY > 0) && (iX < map->m_sSizeX) && (iY < map->m_sSizeY))
				map->GetOwner(&sOwnerH, &cOwnerType, iX, iY);
			if (sOwnerH != 0) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH] == nullptr) break;
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH] == nullptr) break;
						iTamingLevel = 10;
						switch (m_pNpcList[sOwnerH]->m_sType) {
							case 10:
							case 16: iTamingLevel = 1;
								break;
							case 22: iTamingLevel = 2;
								break; // ¹ì
							case 17:
							case 14: iTamingLevel = 3;
								break;
							case 18: iTamingLevel = 4;
								break;
							case 11: iTamingLevel = 5;
								break;
							case 23:
							case 12: iTamingLevel = 6;
								break;
							case 28: iTamingLevel = 7;
								break; // Æ®·Ñ
							case 13:
							case 27: iTamingLevel = 8;
								break;
							case 29: iTamingLevel = 9;
								break;
							case 33: iTamingLevel = 9;
								break;
							case 30: iTamingLevel = 9;
								break;
							case 31:
							case 32: iTamingLevel = 10;
								break;
						}
						iResult = (iSkillLevel / 10);
						if (iResult < iTamingLevel) break;
						break;
				}
			}
		}
}

void CClient::GetMagicAbilityHandler() {
	if (this->markedForDeletion_) return;
	if (this->m_cSkillMastery[4] != 0) return;
	this->m_cSkillMastery[4] = 20;
	this->SendNotifyMsg(0, DEF_NOTIFY_SKILL, 4, this->m_cSkillMastery[4], 0, nullptr);
	this->bCheckTotalSkillMasteryPoints(4);
}

int CClient::iRequestPanningMapDataRequest(char * pData) {
	char * cp, cDir, cData[3000];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp, dX, dY;
	int iRet;
	int iSize;
	if (!this->m_bIsObserverMode) return 0;
	if (this->m_bIsKilled) return 0;
	if (!this->m_bIsInitComplete) return 0;
	dX = this->m_sX;
	dY = this->m_sY;
	cDir = *(pData + DEF_INDEX2_MSGTYPE + 2);
	if ((cDir <= 0) || (cDir > 8)) return 0;
	switch (cDir) {
		case 1: dY--;
			break; // responding when mouse is placed north
		case 2: dX++;
			dY--;
			break;
		case 3: dX++;
			break;
		case 4: dX++;
			dY++;
			break;
		case 5: dY++;
			break;
		case 6: dX--;
			dY++;
			break;
		case 7: dX--;
			break; // responding when mouse placed at west side of screen
		case 8: dX--;
			dY--;
			break; // responding when mouse is placed north west
			/*
			player is in the center, and is trying to pan,
			directions not responding or causing a break will be kept as X,
			others are the cDir case...
					8	1	X
					7	_	X
					X	X	X
			 */
	}
	this->m_sX = dX;
	this->m_sY = dY;
	this->m_cDir = cDir;
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_PANNING;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOVE_CONFIRM;
	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	*sp = (short) (dX - 10);
	cp += 2;
	sp = (short *) cp;
	*sp = (short) (dY - 7);
	cp += 2;
	*cp = cDir;
	cp++;
	iSize = this->iComposeMoveMapData((short) (dX - 10), (short) (dY - 7), cDir, cp);
	iRet = this->m_pXSock->iSendMsg(cData, iSize + 12 + 1 + 4);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[this->id_]->DeleteClient(true, true);
			return 0;
	}
	return 1;
}

void CClient::AdminOrder_SetObserverMode() {
	if (this->m_bIsObserverMode == true) {
		this->map_->SetOwner(this->id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
		this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
		this->SendNotifyMsg(0, DEF_NOTIFY_OBSERVERMODE, 0, 0, 0, nullptr);
		this->m_bIsObserverMode = false;
	} else {
		this->map_->ClearOwner(1, this->id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
		this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
		//this->iRequestPanningMapDataRequest(pData)
		this->SendNotifyMsg(0, DEF_NOTIFY_OBSERVERMODE, 1, 0, 0, nullptr);
		this->m_bIsObserverMode = true;
	}
}

void CClient::AdminOrder_EnableAdminCommand(char *pData, uint32_t dwMsgSize) {
	char * token, cBuff[256], len;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	if ((dwMsgSize) <= 0) return;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		len = strlen(token);
		if (len > 10) len = 10;
		if (memcmp(token, m_cSecurityNumber, len) == 0) {
			this->m_bIsAdminCommandEnabled = true;
		} else {
			wsprintf(G_cTxt, "(%s) Player(%s) attempts to access /enableadmincommand with %s", this->m_cIPaddress, this->m_cCharName, token);
			PutHackLogFileList(G_cTxt);
			return;
		}
	}
}

void CClient::AdminOrder_CreateItem(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * cp, * token, cBuff[256], cItemName[256], cData[256], cTemp[256], cAttribute[256], cValue[256];
	SYSTEMTIME SysTime;
	class CStrTok * pStrTok;
	class CItem * pItem;
	short * sp;
	int iTemp;
	int iEraseReq;
	int iValue;
	uint32_t * dwp;
	uint16_t * wp, wTemp;
	double dV1;
	double dV2;
	double dV3;
	// if the client doesnt exist than no effect.
	if (this->m_bIsAdminCommandEnabled == false) return;
	// if the command is just "/createitem " than no effect.
	if ((dwMsgSize) <= 0) return;
	// if client's admin level is less than 4 no effect.
	if (this->m_iAdminUserLevel < m_iAdminLevelCreateItem) {
		// if the admin user level is not 0 and less than 4 send message
		if (this->m_iAdminUserLevel != 0)
			this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	} // close if (this->m_iAdminUserLevel < 4) {
	// IP address not able to create item
	// DISABLED: if (memcmp(this->m_cIPaddress, "192.168.1.", 12) != 0) return;
	// check if the client has typed "/enableadmincreateitem"
	//if (this->m_bIsAdminCreateItemEnabled == false) return;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	// if the token is something it is an item name
	if (token != nullptr) {
		if (strlen(token) > 21) return;
		std::memset(cItemName, 0, sizeof (cItemName));
		strcpy(cItemName, token);
	} // close if (token != nullptr) {
	// if the 2nd token is true it is the item attribute
	std::memset(cAttribute, 0, sizeof (cAttribute));
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 256) return;
		std::memset(cAttribute, 0, sizeof (cAttribute));
		strcpy(cAttribute, token);
	} // close if (token != nullptr) {
	// if the 3rd token is true it is the item value
	std::memset(cValue, 0, sizeof (cValue));
	iValue = 0;
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 256) return;
		std::memset(cValue, 0, sizeof (cValue));
		strcpy(cValue, token);
		iValue = atoi(cValue);
	} // close if (token != nullptr) {
	pItem = new class CItem;
	// if the given itemname doesnt exist delete item
	if (_bInitItemAttr(*pItem, cItemName) == false) {
		delete pItem;
		return;
	} //close if (_bInitItemAttr(pItem, cItemName) == false) {
	if (strlen(cAttribute) != 0) {
		pItem->m_dwAttribute = atoi(cAttribute);
		if (pItem->m_dwAttribute == 1) {
			if ((iValue >= 1) && (iValue <= 200)) {
				pItem->m_cItemColor = 2;
				pItem->m_sItemSpecEffectValue2 = iValue - 100;
				dV2 = (double) pItem->m_sItemSpecEffectValue2;
				dV3 = (double) pItem->m_wMaxLifeSpan;
				dV1 = (dV2 / 100.0f) * dV3;
				iTemp = (int) pItem->m_wMaxLifeSpan;
				iTemp += (int) dV1;
				if (iTemp <= 0)
					wTemp = 1;
				else wTemp = (uint16_t) iTemp;
				if (wTemp <= pItem->m_wMaxLifeSpan * 2) {
					pItem->m_wMaxLifeSpan = wTemp;
					pItem->m_sItemSpecEffectValue1 = (short) wTemp;
					pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
				}// close if (wTemp <= pItem->m_wMaxLifeSpan*2) {
				else pItem->m_sItemSpecEffectValue1 = (short) pItem->m_wMaxLifeSpan;
			}// close if ((iValue >= 1) && (iValue <= 200)) {
			else pItem->m_dwAttribute = 0;
		}// close if (pItem->m_dwAttribute == 1) {
		else {
			if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
				switch ((pItem->m_dwAttribute & 0xF00000) >> 20) {
					case 6: pItem->m_cItemColor = 2;
						break;
					case 8: pItem->m_cItemColor = 3;
						break;
					case 1: pItem->m_cItemColor = 5;
						break;
					case 5: pItem->m_cItemColor = 1;
						break;
					case 3: pItem->m_cItemColor = 7;
						break;
					case 2: pItem->m_cItemColor = 4;
						break;
					case 7: pItem->m_cItemColor = 6;
						break;
					case 9: pItem->m_cItemColor = 8;
						break;
				} // close switch ((pItem->m_dwAttribute & 0xF00000) >> 20) {
			}// close if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
			else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE) {
				switch ((pItem->m_dwAttribute & 0xF00000) >> 20) {
					case 10: pItem->m_cItemColor = 5;
						break;
				} // close switch ((pItem->m_dwAttribute & 0xF00000) >> 20) {
			} // close else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE) {
		} // close else {
	} // close if (strlen(cAttribute) != 0) {
	// if the item id is
	switch (pItem->m_sIDnum) {
		case 511: // ArenaTicket
		case 513: // ArenaTicket(2)
		case 515: // ArenaTicket(3)
		case 517: // ArenaTicket(4)
		case 530: // ArenaTicket(5)
		case 531: // ArenaTicket(6)
		case 532: // ArenaTicket(7)
		case 533: // ArenaTicket(8)
		case 534: // ArenaTicket(9)
			GetLocalTime(&SysTime);
			pItem->m_sTouchEffectType = DEF_ITET_DATE;
			pItem->m_sTouchEffectValue1 = (short) SysTime.wMonth;
			pItem->m_sTouchEffectValue2 = (short) SysTime.wDay;
			pItem->m_sTouchEffectValue3 = 24;
			break;
		default:
			GetLocalTime(&SysTime);
			pItem->m_sTouchEffectType = DEF_ITET_ID;
			pItem->m_sTouchEffectValue1 = iDice(1, 100000);
			pItem->m_sTouchEffectValue2 = iDice(1, 100000);
			std::memset(cTemp, 0, sizeof (cTemp));
			wsprintf(cTemp, "%d%2d", (short) SysTime.wMonth, (short) SysTime.wDay);
			pItem->m_sTouchEffectValue3 = atoi(cTemp);
			break;
	}
	std::memset(cData, 0, sizeof (cData));
	if (_bAddClientItemList(client, pItem, &iEraseReq) == true) {
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 1;
		cp++;
		memcpy(cp, pItem->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;
		*cp = pItem->m_cItemType;
		cp++;
		*cp = pItem->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;
		*cp = pItem->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;
		*cp = pItem->m_cItemColor;
		cp++;
		*cp = (char) pItem->m_sItemSpecEffectValue2; // v1.41
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		if (iEraseReq == 1) {
			delete pItem;
			pItem = nullptr;
		}
		this->m_pXSock->iSendMsg(cData, 53);
		// v2.14 Admin Log
		wsprintf(G_cTxt, "(%s) GM Order(%s): Create ItemName(%s)", this->m_cIPaddress, this->m_cCharName, cItemName);
		bSendMsgToLS(MSGID_GAMEMASTERLOG, client, 0, G_cTxt);
		return;
	} else {
		delete pItem;
		return;
	}
}

void CClient::RequestSellItemListHandler(char * pData) {
	int i, * ip, iAmount;
	char * cp, cIndex;

	struct {
		char cIndex;
		int iAmount;
	} stTemp[12];
	cp = (char *) (pData + 6);
	for (i = 0; i < 12; i++) {
		stTemp[i].cIndex = *cp;
		cp++;
		ip = (int *) cp;
		stTemp[i].iAmount = *ip;
		cp += 4;
	}
	int id = this->id_;
	for (i = 0; i < 12; i++) {
		cIndex = stTemp[i].cIndex;
		iAmount = stTemp[i].iAmount;
		if ((cIndex == -1) || (cIndex < 0) || (cIndex >= DEF_MAXITEMS)) return;
		if (this->m_pItemList[cIndex] == nullptr) return;
		ReqSellItemConfirmHandler(id, cIndex, iAmount, nullptr);
		if (m_pClientList[id] == nullptr) return;
	}
}

void CClient::CreateNewPartyHandler() {
	bool bFlag;
	bFlag = this->bCreateNewParty();
	this->SendNotifyMsg(0, DEF_NOTIFY_RESPONSE_CREATENEWPARTY, (int) bFlag, 0, 0, nullptr);
}

void CClient::JoinPartyHandler(int iV1, char *pMemberName) {
	short sAppr2;
	int i;
	if ((m_bAdminSecurity) && (this->m_iAdminUserLevel > 0)) return;
	switch (iV1) {
		case 0:
			RequestDeletePartyHandler(client);
			break;
		case 1:
			//testcode
			wsprintf(G_cTxt, "Join Party Req: %s(%d) ID(%d) Stat(%d) ReqJoinH(%d) ReqJoinName(%s)", this->m_cCharName, this->id_,
					  this->m_iPartyID, this->m_iPartyStatus, this->m_iReqJoinPartyClientH,
					  this->m_cReqJoinPartyName);
			PutLogList(G_cTxt);
			if ((this->m_iPartyID != 0) || (this->m_iPartyStatus != DEF_PARTYSTATUS_NULL)) {
				this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
				this->m_iReqJoinPartyClientH = 0;
				std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
				this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				//testcode
				PutLogList("Join Party Reject (1)");
				return;
			}
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cCharName, pMemberName) == 0)) {
					sAppr2 = (short) ((m_pClientList[i]->m_sAppr2 & 0xF000) >> 12);
					if (sAppr2 != 0) {
						this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
						//testcode
						PutLogList("Join Party Reject (2)");
					} else if (m_pClientList[i]->m_cSide != this->m_cSide) {
						this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
						//testcode
						PutLogList("Join Party Reject (3)");
					} else if (m_pClientList[i]->m_iPartyStatus == DEF_PARTYSTATUS_PROCESSING) {
						this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
						//testcode
						PutLogList("Join Party Reject (4)");
						//testcode
						wsprintf(G_cTxt, "Party join reject(2) ClientH:%d ID:%d JoinName:%d", i, m_pClientList[i]->m_iPartyID, m_pClientList[i]->m_cReqJoinPartyName);
						PutLogList(G_cTxt);
						this->m_iReqJoinPartyClientH = 0;
						std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
						this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
					} else {
						m_pClientList[i]->m_iReqJoinPartyClientH = this->id_;
						std::memset(m_pClientList[i]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[i]->m_cReqJoinPartyName));
						strcpy(m_pClientList[i]->m_cReqJoinPartyName, this->m_cCharName);
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_QUERY_JOINPARTY, 0, 0, 0, this->m_cCharName);
						this->m_iReqJoinPartyClientH = i;
						std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
						strcpy(this->m_cReqJoinPartyName, m_pClientList[i]->m_cCharName);
						this->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
					}
					return;
				}
			break;
		case 2:
			if (this->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) {
			}
			break;
	}
}

void CClient::ActivateSpecialAbilityHandler() {
	uint32_t dwTime = timeGetTime();
	short sTemp;
	if (this->m_iSpecialAbilityTime != 0) return;
	if (this->m_iSpecialAbilityType == 0) return;
	if (this->m_bIsSpecialAbilityEnabled == true) return;
	this->m_bIsSpecialAbilityEnabled = true;
	this->m_dwSpecialAbilityStartTime = dwTime;
	this->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
	sTemp = this->m_sAppr4;
	sTemp = sTemp & 0xFF0F;
	switch (this->m_iSpecialAbilityType) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			sTemp = sTemp | 0x0010;
			break;
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
			sTemp = sTemp | 0x0020;
			break;
	}
	this->m_sAppr4 = sTemp;
	this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, this->m_iSpecialAbilityType, this->m_iSpecialAbilityLastSec, nullptr);
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CClient::CancelQuestHandler() {
	this->_ClearQuestStatus();
	this->SendNotifyMsg(0, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, nullptr);
}

bool CClient::__bSetConstructionKit(int dX, int dY, int iType, int /*iTimeCost*/,) {
	int iNamingValue;
	int ix;
	int iy;
	int tX;
	int tY;
	char cNpcName[21], cName[21], cNpcWaypoint[11], cOwnerType;
	short sOwnerH;
	if ((!m_bIsCrusadeMode) || (this->m_iCrusadeDuty != 2)) return false;
	if (this->map_->m_iTotalCrusadeStructures >= DEF_MAXCRUSADESTRUCTURES) {
		this->SendNotifyMsg(0, DEF_NOTIFY_NOMORECRUSADESTRUCTURE, 0, 0, 0, nullptr);
		return false;
	}
	iNamingValue = this->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		for (ix = dX - 3; ix <= dX + 5; ix++)
			for (iy = dY - 3; iy <= dX + 5; iy++) {
				this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
				if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sOwnerH]->m_cActionLimit == 5)) return false;
			}
		std::memset(cNpcName, 0, sizeof (cNpcName));
		if (this->m_cSide == 1) {
			switch (iType) {
				case 1: strcpy(cNpcName, "AGT-Aresden");
					break;
				case 2: strcpy(cNpcName, "CGT-Aresen");
					break;
				case 3: strcpy(cNpcName, "MS-Aresden");
					break;
				case 4: strcpy(cNpcName, "DT-Aresden");
					break;
			}
		} else if (this->m_cSide == 2) {
			switch (iType) {
				case 1: strcpy(cNpcName, "AGT-Elvine");
					break;
				case 2: strcpy(cNpcName, "CGT-Elvine");
					break;
				case 3: strcpy(cNpcName, "MS-Elvine");
					break;
				case 4: strcpy(cNpcName, "DT-Elvine");
					break;
			}
		} else return false;
		std::memset(cName, 0, sizeof (cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = this->map_->id_ + 65;
		std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
		tX = (int) dX;
		tY = (int) dY;
		if (this->map_->bCreateNewNpc(cNpcName, cName, 0, (rand() % 9), DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
			this->map_->SetNamingValueEmpty(iNamingValue);
		} else {
			wsprintf(G_cTxt, "Structure(%s) construction begin(%d,%d)!", cNpcName, tX, tY);
			PutLogList(G_cTxt);
			return true;
		}
	}
	return false;
}

void CClient::RequestSetGuildConstructLocHandler(int dX, int dY, int iGuildGUID, char * pMapName) {
	register int i;
	int iIndex;
	uint32_t dwTemp;
	uint32_t dwTime;
	if (this->m_bIsOnServerChange == true) return;
	dwTime = timeGetTime();
	wsprintf(G_cTxt, "SetGuildConstructLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
	PutLogList(G_cTxt);
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID) {
			if ((m_pGuildTeleportLoc[i].m_sDestX2 == dX) && (m_pGuildTeleportLoc[i].m_sDestY2 == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName) == 0)) {
				m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
				return;
			} else {
				m_pGuildTeleportLoc[i].m_sDestX2 = dX;
				m_pGuildTeleportLoc[i].m_sDestY2 = dY;
				std::memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName2));
				strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
				m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
				return;
			}
		}
	dwTemp = 0;
	iIndex = -1;
	for (i = 0; i < DEF_MAXGUILDS; i++) {
		if (m_pGuildTeleportLoc[i].m_iV1 == 0) {
			m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
			m_pGuildTeleportLoc[i].m_sDestX2 = dX;
			m_pGuildTeleportLoc[i].m_sDestY2 = dY;
			std::memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName2));
			strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
			m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
			return;
		} else {
			if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime2)) {
				dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime2);
				iIndex = i;
			}
		}
	}
	if (iIndex == -1) return;
	PutLogList("(X) No more GuildConstructLoc Space! Replaced.");
	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX2 = dX;
	m_pGuildTeleportLoc[i].m_sDestY2 = dY;
	std::memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName2));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
}

void CClient::RequestSummonWarUnitHandler(int dX, int dY, char cType, char cNum, char cMode) {
	char cName[6], cNpcName[21], cMapName[11], cNpcWayPoint[11], cOwnerType;
	register int i, x;
	int iNamingValue;
	int tX;
	int tY;
	int ix;
	int iy;
	bool bRet;
	short sOwnerH;
	uint32_t dwTime = timeGetTime();
	if (this->m_bIsInitComplete == false) return;
	std::memset(cNpcWayPoint, 0, sizeof (cNpcWayPoint));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cMapName, 0, sizeof (cMapName));
	if (cType < 0) return;
	if (cNum > 10) return;
	if (this->m_iConstructionPoint < m_iNpcConstructionPoint[cType]) return;
	if ((this->map_ != nullptr) && (this->map_->m_bIsFixedDayMode == true)) return;
	cNum = 1;
	for (x = 1; x <= cNum; x++) {
		iNamingValue = this->map_->iGetEmptyNamingValue();
		if (iNamingValue == -1) {
		} else {
			std::memset(cName, 0, sizeof (cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = this->map_->id_ + 65;
			switch (cType) {
				case 43:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "LWB-Aresden");
							break;
						case 2: strcpy(cNpcName, "LWB-Elvine");
							break;
					}
					break;
				case 36:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "AGT-Aresden");
							break;
						case 2: strcpy(cNpcName, "AGT-Elvine");
							break;
					}
					break;
				case 37:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "CGT-Aresden");
							break;
						case 2: strcpy(cNpcName, "CGT-Elvine");
							break;
					}
					break;
				case 38:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "MS-Aresden");
							break;
						case 2: strcpy(cNpcName, "MS-Elvine");
							break;
					}
					break;
				case 39:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "DT-Aresden");
							break;
						case 2: strcpy(cNpcName, "DT-Elvine");
							break;
					}
					break;
				case 51:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "CP-Aresden");
							break;
						case 2: strcpy(cNpcName, "CP-Elvine");
							break;
					}
					break;
				case 44:
					strcpy(cNpcName, "GHK");
					break;
				case 45:
					strcpy(cNpcName, "GHKABS");
					break;
				case 46:
					strcpy(cNpcName, "TK");
					break;
				case 47:
					strcpy(cNpcName, "BG");
					break;
				case 82:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "Sor-Aresden");
							break;
						case 2: strcpy(cNpcName, "Sor-Elvine");
							break;
					}
					break;
				case 83:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "ATK-Aresden");
							break;
						case 2: strcpy(cNpcName, "ATK-Elvine");
							break;
					}
					break;
				case 84:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "Elf-Aresden");
							break;
						case 2: strcpy(cNpcName, "Elf-Elvine");
							break;
					}
					break;
				case 85:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "DSK-Aresden");
							break;
						case 2: strcpy(cNpcName, "DSK-Elvine");
							break;
					}
					break;
				case 86:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "HBT-Aresden");
							break;
						case 2: strcpy(cNpcName, "HBT-Elvine");
							break;
					}
					break;
				case 87:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "CT-Aresden");
							break;
						case 2: strcpy(cNpcName, "CT-Elvine");
							break;
					}
					break;
				case 88:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "Bar-Aresden");
							break;
						case 2: strcpy(cNpcName, "Bar-Elvine");
							break;
					}
					break;
				case 89:
					switch (this->m_cSide) {
						case 1: strcpy(cNpcName, "AGC-Aresden");
							break;
						case 2: strcpy(cNpcName, "AGC-Elvine");
							break;
					}
					break;
			}
			wsprintf(G_cTxt, "(!) Request Summon War Unit (%d) (%s)", cType, cNpcName);
			PutLogList(G_cTxt);
			tX = (int) dX;
			tY = (int) dY;
			bRet = false;
			switch (cType) {
				case 36:
				case 37:
				case 38:
				case 39:
					if (strcmp(this->m_cConstructMapName, this->map_->m_cName) != 0) bRet = true;
					if (abs(this->m_sX - this->m_iConstructLocX) > 10) bRet = true;
					if (abs(this->m_sY - this->m_iConstructLocY) > 10) bRet = true;
					if (bRet) {
						this->map_->SetNamingValueEmpty(iNamingValue);
						this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 2, 0, 0, nullptr);
						return;
					}
					for (i = 0; i < DEF_MAXGUILDS; i++)
						if (m_pGuildTeleportLoc[i].m_iV1 == this->m_iGuildGUID) {
							m_pGuildTeleportLoc[i].m_dwTime = dwTime;
							if (m_pGuildTeleportLoc[i].m_iV2 >= DEF_MAXCONSTRUCTNUM) {
								this->map_->SetNamingValueEmpty(iNamingValue);
								this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 3, 0, 0, nullptr);
								return;
							} else {
								m_pGuildTeleportLoc[i].m_iV2++;
								goto RSWU_LOOPBREAK;
							}
						}
					this->map_->SetNamingValueEmpty(iNamingValue);
					this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 3, 0, 0, nullptr);
					return;
					break;
				case 43:
				case 44:
				case 45:
				case 46:
				case 47:
				case 51:
					break;
				case 40:
				case 41:
				case 42:
				case 48:
				case 49:
				case 50:
					break;
			}
RSWU_LOOPBREAK:
			;
			bRet = false;
			switch (cType) {
				case 36:
				case 37:
					for (ix = tX - 2; ix <= tX + 2; ix++)
						for (iy = tY - 2; iy <= tY + 2; iy++) {
							this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_NPC)) {
								switch (m_pNpcList[sOwnerH]->m_sType) {
									case 36:
									case 37:
										bRet = true;
										break;
								}
							}
						}
					if ((dY <= 32) || (dY >= 783)) bRet = true;
					break;
			}
			if (bRet) {
				this->map_->SetNamingValueEmpty(iNamingValue);
				this->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 1, 0, 0, nullptr);
				return;
			}
			if (cMode == 0) {
				bRet = this->map_->bCreateNewNpc(cNpcName, cName, 0, 0, DEF_MOVETYPE_FOLLOW, &tX, &tY, cNpcWayPoint, nullptr, 0, -1, false, false, false, false, this->m_iGuildGUID);
				bSetNpcFollowMode(cName, this->m_cCharName, DEF_OWNERTYPE_PLAYER);
			} else bRet = this->map_->bCreateNewNpc(cNpcName, cName, 0, 0, DEF_MOVETYPE_GUARD, &tX, &tY, cNpcWayPoint, nullptr, 0, -1, false, false, false, false, this->m_iGuildGUID);
			if (!bRet) {
				this->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				this->m_iConstructionPoint -= m_iNpcConstructionPoint[cType];
				if (this->m_iConstructionPoint < 0) this->m_iConstructionPoint = 0;
				this->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, this->m_iConstructionPoint, this->m_iWarContribution, 0, nullptr);
			}
		}
	}
}

void CClient::CheckConnectionHandler(char *pData) {
	char * cp;
	uint32_t * dwp, dwTimeRcv, dwTime, dwTimeGapClient, dwTimeGapServer;
	//this->m_cConnectionCheck = 0;
	dwTime = timeGetTime();
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *) cp;
	dwTimeRcv = *dwp;
	CheckDenialServiceAttack(client, dwTimeRcv);
	if (this->m_dwInitCCTimeRcv == 0) {
		this->m_dwInitCCTimeRcv = dwTimeRcv;
		this->m_dwInitCCTime = dwTime;
	} else {
		dwTimeGapClient = (dwTimeRcv - this->m_dwInitCCTimeRcv);
		dwTimeGapServer = (dwTime - this->m_dwInitCCTime);
		if (dwTimeGapClient < dwTimeGapServer) return;
		if ((abs(dwTimeGapClient - dwTimeGapServer)) >= (DEF_CLIENTTIMEOUT)) {
			this->DeleteClient(true, true);
			return;
		}
	}
}

void CClient::SelectCrusadeDutyHandler(int iDuty) {
	if ((this->m_iGuildRank != 0) && (iDuty == 3)) return;
	if (m_iLastCrusadeWinner == this->m_cSide &&
			  this->m_dwCrusadeGUID == 0 && iDuty == 3) {
		this->m_iConstructionPoint = 3000;
	}
	this->m_iCrusadeDuty = iDuty;
	this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, this->m_iCrusadeDuty, 0, nullptr);
	if (iDuty == 1)
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, client, 0, "Fighter");
	else if (iDuty == 2)
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, client, 0, "Constructor");
	else
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, client, 0, "Commander");
}

void CClient::MapStatusHandler(int iMode, char * pMapName) {
	register int i;
	switch (iMode) {
		case 1:
			if (this->m_iCrusadeDuty == 0) return;
			for (i = 0; i < DEF_MAXGUILDS; i++)
				if ((m_pGuildTeleportLoc[i].m_iV1 != 0) && (m_pGuildTeleportLoc[i].m_iV1 == this->m_iGuildGUID)) {
					this->SendNotifyMsg(0, DEF_NOTIFY_TCLOC, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY, 0, m_pGuildTeleportLoc[i].m_cDestMapName, m_pGuildTeleportLoc[i].m_sDestX2, m_pGuildTeleportLoc[i].m_sDestY2, 0, 0, 0, 0, m_pGuildTeleportLoc[i].m_cDestMapName2);
					std::memset(this->m_cConstructMapName, 0, sizeof (this->m_cConstructMapName));
					memcpy(this->m_cConstructMapName, m_pGuildTeleportLoc[i].m_cDestMapName2, 10);
					this->m_iConstructLocX = m_pGuildTeleportLoc[i].m_sDestX2;
					this->m_iConstructLocY = m_pGuildTeleportLoc[i].m_sDestY2;
					return;
				}
			break;
		case 3:
			for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
				this->m_stCrusadeStructureInfo[i].cType = 0;
				this->m_stCrusadeStructureInfo[i].cSide = 0;
				this->m_stCrusadeStructureInfo[i].sX = 0;
				this->m_stCrusadeStructureInfo[i].sY = 0;
			}
			this->m_iCSIsendPoint = 0;
			std::memset(this->m_cSendingMapName, 0, sizeof (this->m_cSendingMapName));
			if (strcmp(pMapName, this->map_->m_cName) == 0) {
				for (i = 0; i < this->map_->m_iTotalCrusadeStructures; i++) {
					if (this->m_iCrusadeDuty == 3) {
						this->m_stCrusadeStructureInfo[i].cType = this->map_->m_stCrusadeStructureInfo[i].cType;
						this->m_stCrusadeStructureInfo[i].cSide = this->map_->m_stCrusadeStructureInfo[i].cSide;
						this->m_stCrusadeStructureInfo[i].sX = this->map_->m_stCrusadeStructureInfo[i].sX;
						this->m_stCrusadeStructureInfo[i].sY = this->map_->m_stCrusadeStructureInfo[i].sY;
					} else if (this->map_->m_stCrusadeStructureInfo[i].cType == 42) {
						this->m_stCrusadeStructureInfo[i].cType = this->map_->m_stCrusadeStructureInfo[i].cType;
						this->m_stCrusadeStructureInfo[i].cSide = this->map_->m_stCrusadeStructureInfo[i].cSide;
						this->m_stCrusadeStructureInfo[i].sX = this->map_->m_stCrusadeStructureInfo[i].sX;
						this->m_stCrusadeStructureInfo[i].sY = this->map_->m_stCrusadeStructureInfo[i].sY;
					}
				}
				memcpy(this->m_cSendingMapName, this->map_->m_cName, 10);
			} else {
				if (strcmp(pMapName, "middleland") == 0) {
					for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++) {
						if (this->m_iCrusadeDuty == 3) {
							this->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
							this->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
							this->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
							this->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
						} else if (m_stMiddleCrusadeStructureInfo[i].cType == 42) {
							this->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
							this->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
							this->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
							this->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
						}
					}
					strcpy(this->m_cSendingMapName, "middleland");
				} else {
				}
			}
			_SendMapStatus(client);
			break;
	}
}

void CClient::_SendMapStatus() {
	register int i, iDataSize;
	char *cp, cData[DEF_MAXCRUSADESTRUCTURES * 6];
	short * sp;
	std::memset(cData, 0, sizeof (cData));
	cp = (char *) (cData);
	memcpy(cp, this->m_cSendingMapName, 10);
	cp += 10;
	sp = (short *) cp;
	*sp = (short) this->m_iCSIsendPoint;
	cp += 2;
	cp++;
	if (this->m_iCSIsendPoint == 0)
		this->m_bIsSendingMapStatus = true;
	iDataSize = 0;
	for (i = 0; i < 100; i++) {
		if (this->m_iCSIsendPoint >= DEF_MAXCRUSADESTRUCTURES) goto SMS_ENDOFDATA;
		if (this->m_stCrusadeStructureInfo[this->m_iCSIsendPoint].cType == 0) goto SMS_ENDOFDATA;
		*cp = this->m_stCrusadeStructureInfo[this->m_iCSIsendPoint].cType;
		cp++;
		sp = (short *) cp;
		*sp = this->m_stCrusadeStructureInfo[this->m_iCSIsendPoint].sX;
		cp += 2;
		sp = (short *) cp;
		*sp = this->m_stCrusadeStructureInfo[this->m_iCSIsendPoint].sY;
		cp += 2;
		*cp = this->m_stCrusadeStructureInfo[this->m_iCSIsendPoint].cSide;
		cp++;
		iDataSize += 6;
		this->m_iCSIsendPoint++;
	}
	cp = (char *) (cData + 12);
	*cp = (iDataSize / 6);
	this->SendNotifyMsg(0, DEF_NOTIFY_MAPSTATUSNEXT, iDataSize + 13, 0, 0, cData);
	return;
SMS_ENDOFDATA:
	;
	cp = (char *) (cData + 12);
	*cp = (iDataSize / 6);
	this->SendNotifyMsg(0, DEF_NOTIFY_MAPSTATUSLAST, iDataSize + 13, 0, 0, cData);
	this->m_bIsSendingMapStatus = false;
	return;
}

void CClient::RequestHelpHandler() {
	register int i;
	if (this->m_iGuildRank == -1) return;
	if (this->m_iCrusadeDuty != 1) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iGuildRank == 0) &&
				  (m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == this->m_iGuildGUID)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HELP, this->m_sX, this->m_sY, this->m_iHP, this->m_cCharName);
			return;
		}
	this->SendNotifyMsg(0, DEF_NOTIFY_HELPFAILED, 0, 0, 0, nullptr);
}

void CClient::CheckCommanderConstructionPoint() {
	int i;
	if (!m_bIsCrusadeMode) return;
	if (this->m_iConstructionPoint <= 0) return;
	switch (this->m_iCrusadeDuty) {
		case 1:
		case 2:
			for (i = 0; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == this->m_iGuildGUID)) {
					m_pClientList[i]->m_iConstructionPoint += this->m_iConstructionPoint;
					m_pClientList[i]->m_iWarContribution += (this->m_iConstructionPoint / 10);
					if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
						m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
					if (m_pClientList[i]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
						m_pClientList[i]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, nullptr);
					this->m_iConstructionPoint = 0;
					return;
				}
			this->m_iConstructionPoint = 0;
			break;
		case 3:
			break;
	}
}

void CClient::AdminOrder_SummonStorm(char* pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cOwnerType;
	int i;
	class CStrTok * pStrTok;
	short sOwnerH;
	short sType;
	short sX;
	short sY;
	int dX1;
	int dY1;
	int iWhetherBonus;
	int iResult = 0, tX, tY, iErr, ix, iy;
	int iStormCount;
	int iMagicAttr;
	if ((dwMsgSize) <= 0) return;
	if (this->m_bIsAdminCommandEnabled == false) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelStorm) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	//No Storm Specified
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	sType = atoi(token);
	//Only 100 Magic Spells
	if (sType <= 0) sType = 1;
	if (sType > 100) sType = 100;
	//No Count Specified
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	iStormCount = atoi(token);
	if (iStormCount < 0)
		iStormCount = 0;
	if (iStormCount > 30)
		iStormCount = 30; //Original : 30
	try {
		//Whether Bouns
		iWhetherBonus = iGetWhetherMagicBonusEffect(sType, this->map_->m_cWhetherStatus);
		//Magic Attribute
		iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;
		//Calculate Area Coords
		//Gives Right
		dX1 = this->m_sX;
		dX1 -= 10;
		//Gives Top
		dY1 = this->m_sY;
		dY1 -= 7;
		while (iStormCount != 0) {
			int randX;
			int randY;
			randX = (rand() % 20) + dX1;
			randY = (rand() % 13) + dY1;
			switch (m_pMagicConfigList[sType]->m_sType) {
				case DEF_MAGICTYPE_DAMAGE_SPOT:
					//Calculate Damage
					this->map_->GetOwner(&sOwnerH, &cOwnerType, randX, randY);
					Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, randX, randY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					}
					//Show effects
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
							  this->m_sX, this->m_sY, randX, randY, (sType + 100), this->m_sType);
					break;
				case DEF_MAGICTYPE_DAMAGE_LINEAR:
					sX = this->m_sX;
					sY = this->m_sY;
					for (i = 2; i < 10; i++) {
						iErr = 0;
						m_Misc.GetPoint2(sX, sY, randX, randY, &tX, &tY, &iErr, i);
						// tx, ty
						this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx-1, ty
						this->map_->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx+1, ty
						this->map_->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx, ty-1
						this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx, ty+1
						this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						if ((abs(tX - randX) <= 1) && (abs(tY - randY) <= 1)) break;
					}
					for (iy = randY - m_pMagicConfigList[sType]->m_sValue3; iy <= randY + m_pMagicConfigList[sType]->m_sValue3; iy++)
						for (ix = randX - m_pMagicConfigList[sType]->m_sValue2; ix <= randX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
							this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
									Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							}
						}
					// dX, dY
					this->map_->GetOwner(&sOwnerH, &cOwnerType, randX, randY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr); // v1.41 false
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, randX, randY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr); // v1.41 false
					}
					//Show effects
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
							  this->m_sX, this->m_sY, randX, randY, (sType + 100), this->m_sType);
					break;
				case DEF_MAGICTYPE_DAMAGE_AREA:
					this->map_->GetOwner(&sOwnerH, &cOwnerType, randX, randY);
					if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, randX, randY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					}
					for (iy = randY - m_pMagicConfigList[sType]->m_sValue3; iy <= randY + m_pMagicConfigList[sType]->m_sValue3; iy++)
						for (ix = randX - m_pMagicConfigList[sType]->m_sValue2; ix <= randX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
							this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, randX, randY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {
								if (bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
									Effect_Damage_Spot_DamageMove(this->id_, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, randX, randY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							}
						}
					//Show effects
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
							  this->m_sX, this->m_sY, randX, randY, (sType + 100), this->m_sType);
					break;
				default:
					goto Skip_Storm;
			}
			Sleep(rand() % 10);
			iStormCount--;
		}
	} catch (...) {
	}
Skip_Storm:
	delete pStrTok;
}

void CClient::AdminOrder_Weather(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char cPrevMode;
	int i;
	int j;
	int sType;
	int sTime;
	uint32_t dwTime;
	dwTime = timeGetTime();
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelWeather) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	sType = atoi(token);
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	sTime = atoi(token);
	delete pStrTok;
	for (i = 0; i < DEF_MAXMAPS; i++) {
		if ((m_pMapList[i] != nullptr) && (m_pMapList[i]->m_bIsFixedDayMode == false)) {
			cPrevMode = m_pMapList[i]->m_cWhetherStatus;
			if (m_pMapList[i]->m_cWhetherStatus != 0) {
				if ((dwTime - m_pMapList[i]->m_dwWhetherStartTime) > m_pMapList[i]->m_dwWhetherLastTime)
					m_pMapList[i]->m_cWhetherStatus = 0;
			} else {
				sTime *= 1000;
				m_pMapList[i]->m_cWhetherStatus = sType;
				m_pMapList[i]->m_dwWhetherStartTime = dwTime;
				m_pMapList[i]->m_dwWhetherLastTime = sTime;
			}
			if (cPrevMode != m_pMapList[i]->m_cWhetherStatus) {
				for (j = 1; j < DEF_MAXCLIENTS; j++)
					if ((m_pClientList[j] != nullptr) && (m_pClientList[j]->m_bIsInitComplete == true) && (m_pClientList[j]->map_ == m_pMapList[i]))
						m_pClientList[j]->SendNotifyMsg(0, DEF_NOTIFY_WHETHERCHANGE, m_pMapList[i]->m_cWhetherStatus, 0, 0, nullptr);
			}
		}
	}
}

void CClient::RequestChangePlayMode() {
	if (this->m_iPKCount > 0) return;
	if (this->m_bIsInitComplete == false) return;
	if (memcmp(this->map_->m_cName, "cityhall", 8) != 0) return;
	if (this->m_iLevel < 100 ||
			  this->m_bIsPlayerCivil == true) {
		if (memcmp(this->m_cLocation, "aresden", 7) == 0) strcpy(this->m_cLocation, "arehunter");
		else if (memcmp(this->m_cLocation, "elvine", 6) == 0) strcpy(this->m_cLocation, "elvhunter");
		else if (memcmp(this->m_cLocation, "arehunter", 9) == 0) strcpy(this->m_cLocation, "aresden");
		else if (memcmp(this->m_cLocation, "elvhunter", 9) == 0) strcpy(this->m_cLocation, "elvine");
		if (this->m_bIsPlayerCivil == true)
			this->m_bIsPlayerCivil = false;
		else this->m_bIsPlayerCivil = true;
		this->SendNotifyMsg(0, DEF_NOTIFY_CHANGEPLAYMODE, 0, 0, 0, this->m_cLocation);
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, 100, 0, 0, 0);
	}
}

void CClient::AdminOrder_SetStatus(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSetStatus) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	/*auto dummy = */pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '0') {
			SetPoisonFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetIllusionFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetDefenseShieldFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetMagicProtectionFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetProtectionFromArrowFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetIllusionMovementFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetInhibitionCastingFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
			SetHeroFlag(this->id_, DEF_OWNERTYPE_PLAYER, false);
		}
		if (token[0] == '1') {
			SetPoisonFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '2') {
			SetIllusionFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '3') {
			SetDefenseShieldFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '4') {
			SetMagicProtectionFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '5') {
			SetProtectionFromArrowFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '6') {
			SetIllusionMovementFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '7') {
			SetInhibitionCastingFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '8') {
			SetHeroFlag(this->id_, DEF_OWNERTYPE_PLAYER, true);
		}
	}
	delete pStrTok;
}

void CClient::RequestGuildNameHandler(int iObjectID, int iIndex) {
	if (this->markedForDeletion_) return;
	if ((iObjectID <= 0) || (iObjectID >= DEF_MAXCLIENTS)) return;
	if (m_pClientList[iObjectID] == nullptr) {
	} else {
		this->SendNotifyMsg(0, DEF_NOTIFY_REQGUILDNAMEANSWER, m_pClientList[iObjectID]->m_iGuildRank, iIndex, 0, m_pClientList[iObjectID]->m_cGuildName);
	}
}

bool CClient::_bItemLog(int iAction, char * cName, class CItem * pItem) {
	if (pItem == nullptr) return false;
	if (_bCheckGoodItem(pItem) == false) return false;
	if (iAction != DEF_ITEMLOG_NEWGENDROP) {
		if (this->markedForDeletion_) return false;
	}
	char cTxt[200], cTemp1[120];
	std::memset(cTxt, 0, sizeof (cTxt));
	std::memset(cTemp1, 0, sizeof (cTemp1));
	if (!this->markedForDeletion_) this->m_pXSock->iGetPeerAddress(cTemp1);
	switch (iAction) {
		case DEF_ITEMLOG_NEWGENDROP:
			if (pItem == nullptr) return false;
			wsprintf(cTxt, "NPC(%s)\tDrop\t%s(%d %d %d %d)", cName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
			break;
		case DEF_ITEMLOG_SKILLLEARN:
		case DEF_ITEMLOG_MAGICLEARN:
			if (cName == nullptr) return false;
			if (this->markedForDeletion_) return false;
			wsprintf(cTxt, "PC(%s)\tLearn\t(%s)\t%s(%d %d)\t \tIP(%s)", this->m_cCharName, cName,
					  this->map_->m_cName, this->m_sX, this->m_sY, cTemp1);
			break;
		case DEF_ITEMLOG_SUMMONMONSTER:
			if (cName == nullptr) return false;
			if (this->markedForDeletion_) return false;
			wsprintf(cTxt, "PC(%s)\tSummon\t(%s)\t%s(%d %d)\t \tIP(%s)", this->m_cCharName, cName,
					  this->map_->m_cName, this->m_sX, this->m_sY, cTemp1);
			break;
		case DEF_ITEMLOG_POISONED:
			if (this->markedForDeletion_) return false;
			wsprintf(cTxt, "PC(%s)\tBe Poisoned\t \t%s(%d %d)\t \tIP(%s)", this->m_cCharName,
					  this->map_->m_cName, this->m_sX, this->m_sY, cTemp1);
			break;
		case DEF_ITEMLOG_REPAIR:
			if (cName == nullptr) return false;
			if (this->markedForDeletion_) return false;
			wsprintf(cTxt, "PC(%s)\tRepair\t(%s)\t%s(%d %d)\t \tIP(%s)", this->m_cCharName, cName,
					  this->map_->m_cName, this->m_sX, this->m_sY, cTemp1);
			break;
		default:
			return false;
	}
	bSendMsgToLS(MSGID_GAMEITEMLOG, iClientH, 0, cTxt);
	return true;
}

void CClient::GetExp(int iExp, bool /*bIsAttackerOwn*/) {
	double dV1;
	double dV2;
	double dV3;
	int i;
	int iH;
	int iUnitValue;
	int iTotalPartyMembers;
	if (this->markedForDeletion_) return;
	if (iExp <= 0) return;
	if (this->m_iLevel <= 80) {
		dV1 = (double) (80 - this->m_iLevel);
		dV2 = dV1 * 0.025f;
		dV3 = (double) iExp;
		dV1 = (dV2 + 1.025f) * dV3;
		iExp = (int) dV1;
	} else { //Lower exp
		if ((this->m_iLevel >= 100) &&((strcmp(this->map_->m_cName, "aresden") == 0)
				  || (strcmp(this->map_->m_cName, "elvine") == 0))) {
			iExp = (iExp / 10);
		} else if ((strcmp(this->map_->m_cName, "aresden") == 0)
				  || (strcmp(this->map_->m_cName, "elvine") == 0)) {
			iExp = (iExp * 1 / 4);
		}
	}
	//Check for party status, else give exp to player
	//if ((this->m_iPartyID != nullptr) && (this->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) &&
	//	((dwTime - this->m_dwLastActionTime) < 1000*60*5)) {
	if ((this->m_iPartyID != 0) && (this->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)) {
		//Only divide exp if > 1 person and exp > 10
		if (iExp >= 10 && m_stPartyInfo[this->m_iPartyID].iTotalMembers > 0) {
			//Calc total ppl in party
			iTotalPartyMembers = 0;
			for (i = 0; i < m_stPartyInfo[this->m_iPartyID].iTotalMembers; i++) {
				iH = m_stPartyInfo[this->m_iPartyID].iIndex[i];
				if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_iHP > 0)) {
					//Newly added, Only players on same map get exp :}
					//if(m_pClientList[iH]->m_cMapIndex == this->m_cMapIndex)
					if (m_pClientList[iH]->map_ == this->map_) {
						iTotalPartyMembers++;
					}
				}
			}
			//Check for party bug
			if (iTotalPartyMembers > 8) {
				wsprintf(G_cTxt, "(X) Party Bug !! partyMember %d XXXXXXXXXX", iTotalPartyMembers);
				PutLogFileList(G_cTxt);
				iTotalPartyMembers = 8;
			}
			//Figure out how much exp a player gets
			dV1 = (double) iExp;
			switch (iTotalPartyMembers) {
				case 1:
					dV2 = dV1;
					break;
				case 2:
					dV2 = (dV1 + (dV1 * 2.0e-2)) / 2.0;
					break;
				case 3:
					dV2 = (dV1 + (dV1 * 5.0e-2)) / 3.0;
					break;
				case 4:
					dV2 = (dV1 + (dV1 * 7.000000000000001e-2)) / 4.0;
					break;
				case 5:
					dV2 = (dV1 + (dV1 * 1.0e-1)) / 5.0;
					break;
				case 6:
					dV2 = (dV1 + (dV1 * 1.4e-1)) / 6.0;
					break;
				case 7:
					dV2 = (dV1 + (dV1 * 1.7e-1)) / 7.0;
					break;
				case 8:
					dV2 = (dV1 + (dV1 * 2.0e-1)) / 8.0;
					break;
			}
			dV3 = dV2 + 5.0e-1;
			iUnitValue = (int) dV3;
			//Divide exp among party members
			for (i = 0; i < iTotalPartyMembers; i++) {
				iH = m_stPartyInfo[this->m_iPartyID].iIndex[i];
				//Player who kills monster gets more exp
				if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_bSkillUsingStatus[19] != 1) && (m_pClientList[iH]->m_iHP > 0)) { // Is player alive ??
					//Exp gain based on lvl
					if ((m_pClientList[iH]->m_iStatus & 0x10000) != 0)
						iUnitValue *= 3;
					m_pClientList[iH]->m_iExpStock += iUnitValue;
					iUnitValue = (int) dV3;
				}
			}
		} else {
			if ((this->m_iStatus & 0x10000) != 0)
				iExp *= 3;
			this->m_iExpStock += iExp;
		}
	} else {
		if ((this->m_iStatus & 0x10000) != 0)
			iExp *= 3;
		//Add exp to player
		this->m_iExpStock += iExp;
	}
}

void CClient::AdminOrder_GoTo(char* pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cPlayerName[11], cMapName[32];
	class CStrTok * pStrTok;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelGoto) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cPlayerName, 0, sizeof (cPlayerName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	if (strlen(token) > 10) {
		memcpy(cPlayerName, token, 10);
	} else {
		memcpy(cPlayerName, token, strlen(token));
	}
	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {
			if (memcmp(cPlayerName, m_pClientList[i]->m_cCharName, 10) == 0) {
				if (this->m_iAdminUserLevel > 0) {
					if (m_pClientList[i]->m_sX == -1 && m_pClientList[i]->m_sX == -1) {
						wsprintf(cBuff, "GM Order(%s): GoTo MapName(%s)", this->m_cCharName, m_pClientList[i]->map_->m_cName);
					} else {
						wsprintf(cBuff, "GM Order(%s): GoTo MapName(%s)(%d %d)", this->m_cCharName,
								  m_pClientList[i]->map_->m_cName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					}
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cBuff);
					std::memset(cMapName, 0, sizeof (cMapName));
					strcpy(cMapName, m_pClientList[i]->map_->m_cName);
					this->RequestTeleportHandler("2   ", cMapName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				}
				delete pStrTok;
				return;
			}
		}
	}
	this->m_bIsAdminOrderGoto = true;
	delete pStrTok;
}

void CClient::AdminOrder_MonsterCount(char* /*pData*/, uint32_t dwMsgSize) {
	int iMonsterCount;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelMonsterCount) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	iMonsterCount = this->map_->m_iTotalAliveObject;
	this->SendNotifyMsg(0, DEF_NOTIFY_MONSTERCOUNT, iMonsterCount, 0, 0, nullptr);
}

void CClient::AdminOrder_SetForceRecallTime(char *pData, uint32_t dwMsgSize) {
	class CStrTok * pStrTok;
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	int iTime;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelSetRecallTime) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token == nullptr)
			iTime = 0;
		else iTime = atoi(token);
		if (iTime < 0) iTime = 0;
		m_sForceRecallTime = iTime;
		wsprintf(G_cTxt, "(!) Game Server Force Recall Time (%d)min", m_sForceRecallTime);
		PutLogList(G_cTxt);
	}
	delete pStrTok;
	return;
}

void CClient::AdminOrder_UnsummonBoss() {
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelUnsummonBoss) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	for (int i = 1; i < DEF_MAXNPCS; i++) {
		if (m_pNpcList[i] != nullptr && m_pNpcList[i]->m_bIsSummoned == false) {
			switch (m_pNpcList[i]->m_sType) {
				case 31: //Demon
				case 49: //Hellclaw
				case 45: //GHKABS
				case 47: //BG
				case 50: //Tigerworm
				case 52: //Gagoyle
				case 81: //Abaddon
				case 66: //Wyvern
				case 73: //Fire-Wyvern
					m_pNpcList[i]->m_bIsUnsummoned = true;
					m_pNpcList[i]->this->NpcKilledHandler(DEF_OWNERTYPE_PLAYER, 0);
					break;
			}
		}
	}
}

void CClient::AdminOrder_ClearNpc() {
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelClearNpc) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	for (int i = 1; i < DEF_MAXNPCS; i++) {
		if (m_pNpcList[i] != nullptr && m_pNpcList[i]->m_bIsSummoned == false) {
			switch (m_pNpcList[i]->m_sType) {
				case 15:
				case 19:
				case 20:
				case 24:
				case 25:
				case 26:
				case 67:
				case 68:
				case 69:
				case 90:
				case 91:
					break;
				default:
					m_pNpcList[i]->m_bIsUnsummoned = true;
					m_pNpcList[i]->this->NpcKilledHandler(DEF_OWNERTYPE_PLAYER, 0);
					break;
			}
		}
	}
}

bool CClient::_bCrusadeLog(int iAction, int iData, const char * cName) {
	char cTxt[200];
	std::memset(cTxt, 0, sizeof (cTxt));
	switch (iAction) {
		case DEF_CRUSADELOG_ENDCRUSADE:
			if (cName == nullptr) return false;
			wsprintf(cTxt, "\tEnd Crusade\t%s", cName);
			break;
		case DEF_CRUSADELOG_SELECTDUTY:
			if (cName == nullptr) return false;
			if (this->markedForDeletion_) return false;
			wsprintf(cTxt, "PC(%s)\tSelect Duty\t(%s)\t \t(%s)", this->m_cCharName, cName, this->m_cGuildName);
			break;
		case DEF_CRUSADELOG_GETEXP:
			if (this->markedForDeletion_) return false;
			wsprintf(cTxt, "PC(%s)\tGet Exp\t(%d)\t \t(%s)", this->m_cCharName, iData, this->m_cGuildName);
			break;
		case DEF_CRUSADELOG_STARTCRUSADE:
			wsprintf(cTxt, "\tStart Crusade");
			break;
		default:
			return false;
	}
	bSendMsgToLS(MSGID_GAMECRUSADELOG, iClientH, 0, cTxt);
	return true;
}

int CClient::iGetPlayerABSStatus() {
	int iRet;
	if (this->markedForDeletion_) return 0;
	iRet = 0;
	if (this->m_iPKCount != 0) {
		iRet = 8;
	}
	if (this->m_cSide != 0) {
		iRet = iRet | 4;
	}
	if (this->m_cSide == 1) {
		iRet = iRet | 2;
	}
	if (this->m_bIsPlayerCivil == true)
		iRet = iRet | 1;
	return iRet;
}

void CClient::SetSlateFlag(short sType, bool bFlag) {
	if (this->markedForDeletion_) return;
	if (sType == DEF_NOTIFY_SLATECLEAR) {
		this->m_iStatus &= 0xFFBFFFFF;
		this->m_iStatus &= 0xFF7FFFFF;
		this->m_iStatus &= 0xFFFEFFFF;
		return;
	}
	if (bFlag) {
		if (sType == 1) { // Invincible slate
			this->m_iStatus |= 0x400000;
		} else if (sType == 3) { // Mana slate
			this->m_iStatus |= 0x800000;
		} else if (sType == 4) { // Exp slate
			this->m_iStatus |= 0x10000;
		}
	} else {
		if ((this->m_iStatus & 0x400000) != 0) {
			this->m_iStatus &= 0xFFBFFFFF;
		} else if ((this->m_iStatus & 0x800000) != 0) {
			this->m_iStatus &= 0xFF7FFFFF;
		} else if ((this->m_iStatus & 0x10000) != 0) {
			this->m_iStatus &= 0xFFFEFFFF;
		}
	}
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CClient::SetPlayingStatus() {
	char cMapName[11], cLocation[11];
	if (this->markedForDeletion_) return;
	std::memset(cMapName, 0, sizeof (cMapName));
	std::memset(cLocation, 0, sizeof (cLocation));
	strcpy(cLocation, this->m_cLocation);
	strcpy(cMapName, this->map_->m_cName);
	this->m_cSide = 0;
	this->m_bIsOwnLocation = false;
	this->m_bIsPlayerCivil = false;
	if (memcmp(cLocation, cMapName, 3) == 0) {
		this->m_bIsOwnLocation = true;
	}
	if (memcmp(cLocation, "are", 3) == 0)
		this->m_cSide = 1;
	else if (memcmp(cLocation, "elv", 3) == 0)
		this->m_cSide = 2;
	else {
		if (strcmp(cMapName, "elvine") == 0 || strcmp(cMapName, "aresden") == 0) {
			this->m_bIsOwnLocation = true;
		}
		this->m_bIsNeutral = true;
	}
	if (memcmp(cLocation, "arehunter", 9) == 0 || memcmp(cLocation, "elvhunter", 9) == 0) {
		this->m_bIsPlayerCivil = true;
	}
	if (memcmp(this->map_->m_cName, "bisle", 5) == 0) {
		this->m_bIsPlayerCivil = false;
	}
	if (memcmp(this->map_->m_cName, "bsmith", 6) == 0 ||
			  memcmp(this->map_->m_cName, "gldhall", 7) == 0 ||
			  memcmp(this->map_->m_cName, "gshop", 5) == 0)
		this->m_pIsProcessingAllowed = true;
	else
		this->m_pIsProcessingAllowed = false;
}

void CClient::ForceChangePlayMode(bool bNotify) {
	if (this->markedForDeletion_) return;
	if (memcmp(this->m_cLocation, "arehunter", 9) == 0)
		strcpy(this->m_cLocation, "aresden");
	else if (memcmp(this->m_cLocation, "elvhunter", 9) == 0)
		strcpy(this->m_cLocation, "elvine");
	if (this->m_bIsPlayerCivil == true)
		this->m_bIsPlayerCivil = false;
	if (bNotify) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CHANGEPLAYMODE, 0, 0, 0, this->m_cLocation);
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	}
}

void CClient::ShowVersion() {
	char cVerMessage[256];
	std::memset(cVerMessage, 0, sizeof (cVerMessage));
	wsprintf(cVerMessage, "Helbreath Sources %s.%s - www.xtremehb.com", DEF_UPPERVERSION, DEF_LOWERVERSION);
	this->ShowClientMsg(cVerMessage);
}

bool CClient::bCheckClientAttackFrequency(uint32_t dwClientTime) {
	uint32_t dwTimeGap;
	if (this->markedForDeletion_) return false;
	if (this->m_iAdminUserLevel > 0) return false;
	if (this->m_dwAttackFreqTime == 0)
		this->m_dwAttackFreqTime = dwClientTime;
	else {
		dwTimeGap = dwClientTime - this->m_dwAttackFreqTime;
		this->m_dwAttackFreqTime = dwClientTime;
		if (dwTimeGap < 450) {
			wsprintf(G_cTxt, "Swing Hack: (%s) Player: (%s) - attacking with weapon at irregular rates.", this->m_cIPaddress, this->m_cCharName);
			PutHackLogFileList(G_cTxt);
			this->DeleteClient(true, true);
			return false;
		}
		//testcode
		//wsprintf(G_cTxt, "Attack: %d", dwTimeGap);
		//PutLogList(G_cTxt);
	}
	return false;
}

bool CClient::bCheckClientMagicFrequency(uint32_t dwClientTime) {
	uint32_t dwTimeGap;
	if (this->markedForDeletion_) return false;
	if (this->m_dwMagicFreqTime == 0)
		this->m_dwMagicFreqTime = dwClientTime;
	else {
		dwTimeGap = dwClientTime - this->m_dwMagicFreqTime;
		this->m_dwMagicFreqTime = dwClientTime;
		if ((dwTimeGap < 1500) && (this->m_bMagicConfirm == true)) {
			wsprintf(G_cTxt, "Speed Cast: (%s) Player: (%s) - casting magic at irregular rates. ", this->m_cIPaddress, this->m_cCharName);
			PutHackLogFileList(G_cTxt);
			this->DeleteClient(true, true);
			return false;
		}
		this->m_iSpellCount--;
		this->m_bMagicConfirm = false;
		this->m_bMagicPauseTime = false;
		//testcode
		//wsprintf(G_cTxt, "Magic: %d", dwTimeGap);
		//PutLogList(G_cTxt);
	}
	return false;
}

bool CClient::bCheckClientMoveFrequency(uint32_t dwClientTime) {
	uint32_t dwTimeGap;
	if (this->markedForDeletion_) return false;
	if (this->m_iAdminUserLevel > 0) return false;
	if (this->m_dwMoveFreqTime == 0)
		this->m_dwMoveFreqTime = dwClientTime;
	else {
		if (this->m_bIsMoveBlocked == true) {
			this->m_dwMoveFreqTime = 0;
			this->m_bIsMoveBlocked = false;
			return false;
		}
		if (this->m_bIsAttackModeChange == true) {
			this->m_dwMoveFreqTime = 0;
			this->m_bIsAttackModeChange = false;
			return false;
		}
		dwTimeGap = dwClientTime - this->m_dwMoveFreqTime;
		this->m_dwMoveFreqTime = dwClientTime;
		if ((dwTimeGap < 200)) {
			wsprintf(G_cTxt, "Speed Hack: (%s) Player: (%s) - running too fast.", this->m_cIPaddress, this->m_cCharName);
			PutHackLogFileList(G_cTxt);
			this->DeleteClient(true, true);
			return false;
		}
		// testcode
		// wsprintf(G_cTxt, "Move: %d", dwTimeGap);
		// PutLogList(G_cTxt);
	}
	return false;
}

void CClient::RequestGuildTeleportHandler() {
	register int i;
	char cMapName[11];
	if (this->markedForDeletion_) return;
	if (this->m_iLockedMapTime != 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_LOCKEDMAP, this->m_iLockedMapTime, 0, 0, this->lockedMap_->m_cName);
		return;
	}
	if (!m_bIsCrusadeMode) {
		wsprintf(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - setting teleport location when crusade is disabled.", this->m_cIPaddress, this->m_cCharName);
		PutLogList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	if (this->m_iCrusadeDuty == 0) {
		wsprintf(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - teleporting when not in a guild", this->m_cIPaddress, this->m_cCharName);
		PutLogList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	if (this->map_ == middlelandMap_.lock()) return;
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == this->m_iGuildGUID) {
			std::memset(cMapName, 0, sizeof (cMapName));
			strcpy(cMapName, m_pGuildTeleportLoc[i].m_cDestMapName);
			wsprintf(G_cTxt, "ReqGuildTeleport: %d %d %d %s", this->m_iGuildGUID, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY, cMapName);
			PutLogList(G_cTxt);
			this->RequestTeleportHandler("2   ", cMapName, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY);
			return;
		}
	switch (this->m_cSide) {
		case 1:
			break;
		case 2:
			break;
	}
}

void CClient::RequestSetGuildTeleportLocHandler(int dX, int dY, int iGuildGUID, const char * pMapName) {
	register int i;
	int iIndex;
	uint32_t dwTemp;
	uint32_t dwTime;
	if (this->markedForDeletion_) return;
	if (this->m_bIsOnServerChange == true) return;
	if (!m_bIsCrusadeMode) {
		wsprintf(G_cTxt, "Accessing Crusade Set Teleport: (%s) Player: (%s) - setting point when not a crusade.", this->m_cIPaddress, this->m_cCharName);
		PutLogList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	if (this->m_iCrusadeDuty != 3) {
		wsprintf(G_cTxt, "Accessing Crusade Set Teleport: (%s) Player: (%s) - setting point when not a guildmaster.", this->m_cIPaddress, this->m_cCharName);
		PutLogList(G_cTxt);
		this->DeleteClient(true, true);
		return;
	}
	if (dY < 100) dY = 100;
	if (dY > 600) dY = 600;
	dwTime = timeGetTime();
	wsprintf(G_cTxt, "SetGuildTeleportLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
	PutLogList(G_cTxt);
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID) {
			if ((m_pGuildTeleportLoc[i].m_sDestX == dX) && (m_pGuildTeleportLoc[i].m_sDestY == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName) == 0)) {
				m_pGuildTeleportLoc[i].m_dwTime = dwTime;
				return;
			} else {
				m_pGuildTeleportLoc[i].m_sDestX = dX;
				m_pGuildTeleportLoc[i].m_sDestY = dY;
				std::memset(m_pGuildTeleportLoc[i].m_cDestMapName, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName));
				strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
				m_pGuildTeleportLoc[i].m_dwTime = dwTime;
				return;
			}
		}
	dwTemp = 0;
	iIndex = -1;
	for (i = 0; i < DEF_MAXGUILDS; i++) {
		if (m_pGuildTeleportLoc[i].m_iV1 == 0) {
			m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
			m_pGuildTeleportLoc[i].m_sDestX = dX;
			m_pGuildTeleportLoc[i].m_sDestY = dY;
			std::memset(m_pGuildTeleportLoc[i].m_cDestMapName, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName));
			strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
			m_pGuildTeleportLoc[i].m_dwTime = dwTime;
			return;
		} else {
			if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime)) {
				dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime);
				iIndex = i;
			}
		}
	}
	if (iIndex == -1) return;
	PutLogList("(X) No more GuildTeleportLoc Space! Replaced.");
	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX = dX;
	m_pGuildTeleportLoc[i].m_sDestY = dY;
	std::memset(m_pGuildTeleportLoc[i].m_cDestMapName, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime = dwTime;
}

void CClient::CancelExchangeItem() {
	auto with = this->exchangingWith_.lock();
	if (with) {
		_ClearExchangeStatus(with->id_);
	}
	this->_ClearExchangeStatus();
}

void CClient::_ClearExchangeStatus() {
	if (this->markedForDeletion_) return;
	if (this->exchangingWith_.lock()) {
		this->SendNotifyMsg(0, DEF_NOTIFY_CANCELEXCHANGEITEM, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
	}
	this->m_dwInitCCTime = false;
	this->m_iAlterItemDropIndex = 0;
	this->exchangingWith_.reset();
	this->m_bIsExchangeMode = false;
	this->m_bIsExchangeConfirm = false;
}

void CClient::SetForceRecallTime() {
	int iTL_ = 0;
	SYSTEMTIME SysTime;
	if (this->markedForDeletion_) return;
	if (this->m_bIsWarLocation == false) return;
	//PutLogProcessList("PROCESS #353");
	if (this->m_iTimeLeft_ForceRecall == 0) {
		if (m_sForceRecallTime > 0) {
			this->m_iTimeLeft_ForceRecall = 60 * m_sForceRecallTime;
		} else {
			GetLocalTime(&SysTime);
			switch (SysTime.wDayOfWeek) {
				case 1: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeMonday;
					break;
				case 2: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeTuesday;
					break;
				case 3: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeWednesday;
					break;
				case 4: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeThursday;
					break;
				case 5: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeFriday;
					break;
				case 6: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSaturday;
					break;
				case 0: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSunday;
					break;
			}
		}
	} else { // if (this->m_iTimeLeft_ForceRecall == 0)
		if (m_sForceRecallTime > 0) {
			iTL_ = 60 * m_sForceRecallTime;
		} else {
			GetLocalTime(&SysTime);
			switch (SysTime.wDayOfWeek) {
				case 1: iTL_ = 60 * m_sRaidTimeMonday;
					break;
				case 2: iTL_ = 60 * m_sRaidTimeTuesday;
					break;
				case 3: iTL_ = 60 * m_sRaidTimeWednesday;
					break;
				case 4: iTL_ = 60 * m_sRaidTimeThursday;
					break;
				case 5: iTL_ = 60 * m_sRaidTimeFriday;
					break;
				case 6: iTL_ = 60 * m_sRaidTimeSaturday;
					break;
				case 0: iTL_ = 60 * m_sRaidTimeSunday;
					break;
				default: iTL_ = 60 * 1;
					break;
			}
			if (this->m_iTimeLeft_ForceRecall > iTL_)
				this->m_iTimeLeft_ForceRecall = iTL_;
		}
	}
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	char cMsg[120];
	std::memset(cMsg, 0, sizeof (cMsg));
	if (this->m_iTimeLeft_ForceRecall >= 60) {
		int azucar = (this->m_iTimeLeft_ForceRecall / 60);
		wsprintf(cMsg, "Time left force recall %d Min", azucar);
	}// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	else {
		wsprintf(cMsg, "Time left force recall %d Seconds", this->m_iTimeLeft_ForceRecall);
	}
	this->ShowClientMsg(cMsg);
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

void CClient::CheckForceRecallTime() {
	SYSTEMTIME SysTime;
	int iTL_;
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel > 0) return;
	//PutLogProcessList("PROCESS #354");
	if (this->m_iTimeLeft_ForceRecall == 0) {
		// has admin set a recall time ??
		if (m_sForceRecallTime > 0) {
			this->m_iTimeLeft_ForceRecall = m_sForceRecallTime * 60;
		}// use standard recall time calculations
		else {
			GetLocalTime(&SysTime);
			switch (SysTime.wDayOfWeek) {
				case 1: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeMonday;
					break;
				case 2: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeTuesday;
					break;
				case 3: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeWednesday;
					break;
				case 4: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeThursday;
					break;
				case 5: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeFriday;
					break;
				case 6: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSaturday;
					break;
				case 0: this->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSunday;
					break;
				default: this->m_iTimeLeft_ForceRecall = 60 * 1;
					break; //
			}
		}
	} else {
		// has admin set a recall time ??
		if (m_sForceRecallTime > 0) {
			iTL_ = m_sForceRecallTime * 60;
		}// use standard recall time calculations
		else {
			GetLocalTime(&SysTime);
			switch (SysTime.wDayOfWeek) {
				case 1: iTL_ = 60 * m_sRaidTimeMonday;
					break;
				case 2: iTL_ = 60 * m_sRaidTimeTuesday;
					break;
				case 3: iTL_ = 60 * m_sRaidTimeWednesday;
					break;
				case 4: iTL_ = 60 * m_sRaidTimeThursday;
					break;
				case 5: iTL_ = 60 * m_sRaidTimeFriday;
					break;
				case 6: iTL_ = 60 * m_sRaidTimeSaturday;
					break;
				case 0: iTL_ = 60 * m_sRaidTimeSunday;
					break;
				default: iTL_ = 60 * 1;
					break;
			}
			if (this->m_iTimeLeft_ForceRecall > iTL_)
				this->m_iTimeLeft_ForceRecall = iTL_;
		}
	}
	this->m_bIsWarLocation = true;
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	char cMsg[120];
	std::memset(cMsg, 0, sizeof (cMsg));
	if (this->m_iTimeLeft_ForceRecall >= 60) {
		int azucar = (this->m_iTimeLeft_ForceRecall / 60);
		wsprintf(cMsg, "Time left force recall %d Min", azucar);
	}// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	else {
		wsprintf(cMsg, "Time left force recall %d Seconds", this->m_iTimeLeft_ForceRecall);
	}
	this->ShowClientMsg(cMsg);
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

void CClient::AdminOrder_Time(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	int i;
	if ((dwMsgSize) <= 0) return;
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelTime) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '2') {
			m_cDayOrNight = 2;
			wsprintf(G_cTxt, "(%s) Admin(%s) Order: Forces night mode", this->m_cIPaddress, this->m_cCharName);
			bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
		} else if (token[0] == '1') {
			m_cDayOrNight = 1;
			wsprintf(G_cTxt, "(%s) Admin(%s) Order: Forces day mode", this->m_cIPaddress, this->m_cCharName);
			bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
		} else if (token[0] == '0') {
			wsprintf(G_cTxt, "(%s) Admin(%s) Order: Disables force time", this->m_cIPaddress, this->m_cCharName);
			bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
		}
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				if ((m_pClientList[i]->map_ != nullptr) && (m_pClientList[i]->map_->m_bIsFixedDayMode == false))
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_TIMECHANGE, m_cDayOrNight, 0, 0, nullptr);
			}
	}
	delete pStrTok;
}

void CClient::AdminOrder_CheckRep(char *pData, uint32_t dwMsgSize) {
	class CStrTok * pStrTok;
	const char * token;
	char cName[11], cTargetName[11], cRepMessage[256], cTemp[256], seps[] = "= \t\n", cBuff[256];
	register int i;
	if (this->markedForDeletion_) return;
	std::memset(cTemp, 0, sizeof (cTemp));
	std::memset(cRepMessage, 0, sizeof (cRepMessage));
	if (this->m_iAdminUserLevel < 6) {
		wsprintf(cRepMessage, "You have %d reputation points.", this->m_iRating);
		//this->ShowClientMsg(cRepMessage);
		this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cRepMessage);
	} else {
		if ((dwMsgSize) <= 0) return;
		std::memset(cTargetName, 0, sizeof (cTargetName));
		std::memset(cBuff, 0, sizeof (cBuff));
		memcpy(cBuff, pData, dwMsgSize);
		pStrTok = new class CStrTok(cBuff, seps);
		token = pStrTok->pGet();
		token = pStrTok->pGet();
		if (token != nullptr) {
			std::memset(cName, 0, sizeof (cName));
			strcpy(cName, token);
		} else {
			std::memset(cName, 0, sizeof (cName));
			strcpy(cName, "null");
		}
		token = pStrTok->pGet();
		if (token != nullptr) {
			atoi(token);
		}
		if (token == nullptr) {
			token = "null";
		}
		if (cName != nullptr) {
			token = cName;
			if (strlen(token) > 10)
				memcpy(cTargetName, token, 10);
			else memcpy(cTargetName, token, strlen(token));
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
					wsprintf(cRepMessage, " %s has %d reputation points.", m_pClientList[i]->m_cCharName, m_pClientList[i]->m_iRating);
					this->ShowClientMsg(cRepMessage);
				}
		}
	}
}

void CClient::AdminOrder_Pushplayer(char * pData, uint32_t dwMsgSize) {
	//Push Player- By:ACiDx  Monday , September 2004
	//Teleports a Defined Player to Defined Destination
	// /push playername mapname sX dX
	char seps[] = "= \t\n";
	const char * token;
	char cBuff[256], cMapName[256], cName[11], cTargetName[11];
	class CStrTok * pStrTok;
	register int dX, dY;
	register int i;
	bool bFlag;
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	dX = dY = -1;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelPushPlayer) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, token);
	} else {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, "null");
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cMapName, 0, sizeof (cMapName));
		strcpy(cMapName, token);
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		dX = atoi(token);
	}
	token = pStrTok->pGet();
	if (token != nullptr) {
		dY = atoi(token);
	}
	if (token == nullptr) {
		token = "null";
	}
	if (cName != nullptr) {
		token = cName;
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				bFlag = false;
				//Asks For Flag on maps
				if (strcmp("2ndmiddle", cMapName) == 0) bFlag = true;
				if (strcmp("abaddon", cMapName) == 0) bFlag = true;
				if (strcmp("arebrk11", cMapName) == 0) bFlag = true;
				if (strcmp("arebrk12", cMapName) == 0) bFlag = true;
				if (strcmp("arebrk21", cMapName) == 0) bFlag = true;
				if (strcmp("arebrk22", cMapName) == 0) bFlag = true;
				if (strcmp("arefarm", cMapName) == 0) bFlag = true;
				if (strcmp("arejail", cMapName) == 0) bFlag = true;
				if (strcmp("aremidl", cMapName) == 0) bFlag = true;
				if (strcmp("aremidr", cMapName) == 0) bFlag = true;
				if (strcmp("aresden", cMapName) == 0) bFlag = true;
				if (strcmp("aresdend1", cMapName) == 0) bFlag = true;
				if (strcmp("areuni", cMapName) == 0) bFlag = true;
				if (strcmp("arewrhus", cMapName) == 0) bFlag = true;
				if (strcmp("bisle", cMapName) == 0) bFlag = true;
				if (strcmp("bsmith_1", cMapName) == 0) bFlag = true;
				if (strcmp("bsmith_1f", cMapName) == 0) bFlag = true;
				if (strcmp("bsmith_2", cMapName) == 0) bFlag = true;
				if (strcmp("bsmith_2f", cMapName) == 0) bFlag = true;
				if (strcmp("BtField", cMapName) == 0) bFlag = true;
				if (strcmp("cath_1", cMapName) == 0) bFlag = true;
				if (strcmp("cath_2", cMapName) == 0) bFlag = true;
				if (strcmp("cityhall_1", cMapName) == 0) bFlag = true;
				if (strcmp("cityhall_2", cMapName) == 0) bFlag = true;
				if (strcmp("CmdHall_1", cMapName) == 0) bFlag = true;
				if (strcmp("CmdHall_2", cMapName) == 0) bFlag = true;
				if (strcmp("default", cMapName) == 0) bFlag = true;
				if (strcmp("dglv2", cMapName) == 0) bFlag = true;
				if (strcmp("dglv3", cMapName) == 0) bFlag = true;
				if (strcmp("dglv4", cMapName) == 0) bFlag = true;
				if (strcmp("druncncity", cMapName) == 0) bFlag = true;
				if (strcmp("elvbrk11", cMapName) == 0) bFlag = true;
				if (strcmp("elvbrk12", cMapName) == 0) bFlag = true;
				if (strcmp("elvbrk21", cMapName) == 0) bFlag = true;
				if (strcmp("elvbrk22", cMapName) == 0) bFlag = true;
				if (strcmp("elvfarm", cMapName) == 0) bFlag = true;
				if (strcmp("elvine", cMapName) == 0) bFlag = true;
				if (strcmp("elvined1", cMapName) == 0) bFlag = true;
				if (strcmp("elvjail", cMapName) == 0) bFlag = true;
				if (strcmp("elvmidl", cMapName) == 0) bFlag = true;
				if (strcmp("elvmidr", cMapName) == 0) bFlag = true;
				if (strcmp("elvuni", cMapName) == 0) bFlag = true;
				if (strcmp("elvwrhus", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone1", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone2", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone3", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone4", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone5", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone6", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone7", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone8", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone9", cMapName) == 0) bFlag = true;
				if (strcmp("fightzone10", cMapName) == 0) bFlag = true;
				if (strcmp("gldhall_1", cMapName) == 0) bFlag = true;
				if (strcmp("gldhall_2", cMapName) == 0) bFlag = true;
				if (strcmp("GodH", cMapName) == 0) bFlag = true;
				if (strcmp("gshop_1", cMapName) == 0) bFlag = true;
				if (strcmp("gshop_1f", cMapName) == 0) bFlag = true;
				if (strcmp("gshop_2", cMapName) == 0) bFlag = true;
				if (strcmp("gshop_2f", cMapName) == 0) bFlag = true;
				if (strcmp("HRampart", cMapName) == 0) bFlag = true;
				if (strcmp("huntzone1", cMapName) == 0) bFlag = true;
				if (strcmp("huntzone2", cMapName) == 0) bFlag = true;
				if (strcmp("huntzone3", cMapName) == 0) bFlag = true;
				if (strcmp("huntzone4", cMapName) == 0) bFlag = true;
				if (strcmp("icebound", cMapName) == 0) bFlag = true;
				if (strcmp("inferniaA", cMapName) == 0) bFlag = true;
				if (strcmp("inferniaB", cMapName) == 0) bFlag = true;
				if (strcmp("maze", cMapName) == 0) bFlag = true;
				if (strcmp("middled1n", cMapName) == 0) bFlag = true;
				if (strcmp("middled1x", cMapName) == 0) bFlag = true;
				if (strcmp("middleland", cMapName) == 0) bFlag = true;
				if (strcmp("penalty", cMapName) == 0) bFlag = true;
				if (strcmp("procella", cMapName) == 0) bFlag = true;
				if (strcmp("resurr1", cMapName) == 0) bFlag = true;
				if (strcmp("resurr2", cMapName) == 0) bFlag = true;
				if (strcmp("toh1", cMapName) == 0) bFlag = true;
				if (strcmp("toh2", cMapName) == 0) bFlag = true;
				if (strcmp("toh3", cMapName) == 0) bFlag = true;
				if (strcmp("wrhus_1", cMapName) == 0) bFlag = true;
				if (strcmp("wrhus_1f", cMapName) == 0) bFlag = true;
				if (strcmp("wrhus_2", cMapName) == 0) bFlag = true;
				if (strcmp("wrhus_2f", cMapName) == 0) bFlag = true;
				if (strcmp("wzdtwr_1", cMapName) == 0) bFlag = true;
				if (strcmp("wzdtwr_2", cMapName) == 0) bFlag = true;
				if (strcmp("Test", cMapName) == 0) bFlag = true;
				if (strcmp("GMMap", cMapName) == 0) bFlag = true;
				//Defines I as Max clients
				if (bFlag)
					//Reqeust the Push/Teleport
					m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, dX, dY);
				wsprintf(G_cTxt, "(%s) GM(%s) sends (%s) Player(%s) to [%s](%d,%d)", this->m_cIPaddress,
						  this->m_cCharName, m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName, cMapName, dX, dY);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
				delete pStrTok;
				return;
			}
	}
}

void CClient::AdminOrder_CheckStats(char *pData, uint32_t dwMsgSize) {
	char cStatMessage[256];
	char seps[] = "= \t\n";
	char cBuff[256];
	class CStrTok * pStrTok;
	const char * token;
	char cName[11], cTargetName[11];
	register int i;
	if (this->markedForDeletion_) return;
	if (this->m_iAdminUserLevel < 1) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	if ((dwMsgSize) <= 0) return;
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, token);
	} else {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, "null");
	}
	token = pStrTok->pGet();
	if (token == nullptr) {
		token = "null";
	}
	if (cName != nullptr) {
		token = cName;
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				// m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma
				std::memset(cStatMessage, 0, sizeof (cStatMessage));
				wsprintf(cStatMessage, "Str:%d Dex:%d Vit:%d Int:%d Mag:%d Chr:%d", m_pClientList[i]->m_iStr, m_pClientList[i]->m_iDex, m_pClientList[i]->m_iVit, m_pClientList[i]->m_iInt, m_pClientList[i]->m_iMag, m_pClientList[i]->m_iCharisma);
				this->ShowClientMsg(cStatMessage);
			}
	}
}

bool CClient::bCheckIsItemUpgradeSuccess(int iItemIndex, int iSomH, bool bBonus) {
	int iValue;
	int iProb;
	int iResult;
	if (this->m_pItemList[iSomH] == nullptr) return false;
	iValue = (this->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
	switch (iValue) {
		case 0: iProb = 90;
			break; // %80 de hacerla +1
		case 1: iProb = 65;
			break; // %65 de hacerla +2
		case 2: iProb = 40;
			break; // %40 de hacerla +3
		case 3: iProb = 0;
			break; // %0 de hacerla +4
		case 4: iProb = 0;
			break; // %0 de hacerla +5
		case 5: iProb = 5;
			break; // %5 de hacerla +6
		case 6: iProb = 0;
			break; // %0 de hacerla +7
		case 7: iProb = 0;
			break; // %0 de hacerla +8
		case 8: iProb = 0;
			break; // %0 de hacerla +9
		case 9: iProb = 0;
			break; // %0 de hacerla +10
		default: iProb = 0;
			break;
	}
	if (((this->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0) && (this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 > 100)) {
		if (iProb > 20)
			iProb += (this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 10);
		else if (iProb > 7)
			iProb += (this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 20);
		else
			iProb += (this->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 40);
	}
	if (bBonus) iProb *= 2;
	iProb *= 100;
	iResult = iDice(1, 10000);
	if (iProb >= iResult) {
		_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int) - 1, & * this->m_pItemList[iItemIndex]);
		return true;
	}
	_bItemLog(DEF_ITEMLOG_UPGRADEFAIL, iClientH, (int) - 1, & * this->m_pItemList[iItemIndex]);
	return false;
}

void CClient::AdminOrder_CleanMap(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	char * token, cMapName[11], cBuff[256];
	bool bFlag = false; //Used to check if we are on the map we wanna clear
	int i;
	CItem *pItem;
	short sRemainItemSprite;
	short sRemainItemSpriteFrame;
	short dX;
	short dY;
	char cRemainItemColor;
	char len;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < m_iAdminLevelCleanMap) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cMapName, 0, sizeof (cMapName));
		strcpy(cMapName, token);
		for (i = 0; i < DEF_MAXMAPS; i++) //Enum all maps
			if (m_pMapList[i] != nullptr) { //Is allocated map
				if (memcmp(m_pMapList[i]->m_cName, cMapName, 10) == 0) { //is map same name
					bFlag = true; //Set flag
					//Get X and Y coords
					int m_x = m_pMapList[i]->m_sSizeX;
					int m_y = m_pMapList[i]->m_sSizeY;
					for (int j = 1; j < m_x; j++)
						for (int k = 1; k < m_y; k++) {
							do { //Delete all items on current tile
								pItem = m_pMapList[i]->pGetItem(j, k, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor); // v1.4
								if (pItem != nullptr) {
									delete pItem; //Delete item;
								}
							} while (pItem != nullptr);
						}
					break; //Break outo f loop
				}
			}
		if (!bFlag) { //Notify GM he has to be on the map he clears
		} else { //Notify GM that all items have been cleared
			for (int i = 1; i < DEF_MAXCLIENTS; i++) {
				if (m_pClientList[i] != nullptr) {
					len = strlen(cMapName);
					if (len > 10) len = 10;
					if (memcmp(this->map_->m_cName, cMapName, len) != 0) return;
					dX = m_pClientList[i]->m_sX;
					dY = m_pClientList[i]->m_sY;
					std::memset(cMapName, 0, sizeof (cMapName));
					strcpy(cMapName, m_pClientList[i]->map_->m_cName);
					m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, dX, dY);
				}
			}
		}
	}
	delete pStrTok;
	return;
}

void CClient::ShowClientMsg(const char* pMsg) {
	char * cp, cTemp[256];
	uint32_t * dwp, dwMsgSize;
	uint16_t * wp;
	short * sp;
	std::memset(cTemp, 0, sizeof (cTemp));
	dwp = (uint32_t *) cTemp;
	*dwp = MSGID_COMMAND_CHATMSG;
	wp = (uint16_t *) (cTemp + DEF_INDEX2_MSGTYPE);
	*wp = 0;
	cp = (char *) (cTemp + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	*sp = 0;
	cp += 2;
	sp = (short *) cp;
	*sp = 0;
	cp += 2;
	memcpy(cp, "HGServer", 8); // Player name :P
	cp += 10;
	*cp = 10; // chat type
	cp++;
	dwMsgSize = strlen(pMsg);
	if (dwMsgSize > 50) dwMsgSize = 50;
	memcpy(cp, pMsg, dwMsgSize);
	cp += dwMsgSize;
	this->m_pXSock->iSendMsg(cTemp, dwMsgSize + 22);
}

void CClient::Command_YellowBall(char* pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cPlayerName[11], cMapName[32];
	class CStrTok * pStrTok;
	int iSoxH;
	int iSoX;
	int i;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			switch (this->m_pItemList[i]->m_sIDnum) {
				case 653: iSoX++;
					iSoxH = i;
					break;
			}
		}
	if (iSoX > 0) {
		std::memset(cPlayerName, 0, sizeof (cPlayerName));
		std::memset(cBuff, 0, sizeof (cBuff));
		memcpy(cBuff, pData, dwMsgSize);
		pStrTok = new class CStrTok(cBuff, seps);
		token = pStrTok->pGet();
		token = pStrTok->pGet();
		if (token == nullptr) {
			delete pStrTok;
			return;
		}
		if (strlen(token) > 10) {
			memcpy(cPlayerName, token, 10);
		} else {
			memcpy(cPlayerName, token, strlen(token));
		}
		for (int i = 1; i < DEF_MAXCLIENTS; i++) {
			if (m_pClientList[i] != nullptr) {
				if (memcmp(cPlayerName, m_pClientList[i]->m_cCharName, 10) == 0) {
					if ((this->m_cLocation) != (m_pClientList[i]->m_cLocation)) return;
					std::memset(cMapName, 0, sizeof (cMapName));
					strcpy(cMapName, m_pClientList[i]->map_->m_cName);
					wsprintf(G_cTxt, "(%s) Player: (%s) - YellowBall MapName(%s)(%d %d)", m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName, m_pClientList[i]->map_->m_cName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					PutItemLogFileList(G_cTxt);
					this->ItemDepleteHandler(iSoxH, true, true);
					this->RequestTeleportHandler("2   ", cMapName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					delete pStrTok;
					return;
				}
			}
		}
		this->m_bIsAdminOrderGoto = true;
		delete pStrTok;
	}
}

void CClient::Command_RedBall(char */*pData*/, uint32_t /*dwMsgSize*/) {
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	register int iNamingValue;
	int tX;
	int tY;
	int i;
	int x;
	int iNpcID;
	int iSoxH;
	int iSoX;
	if (this->markedForDeletion_) return;
	if ((memcmp(this->map_->m_cName, "huntzone1", 9) != 0) &&
			  (memcmp(this->map_->m_cName, "huntzone2", 9) != 0) &&
			  (memcmp(this->map_->m_cName, "huntzone3", 9) != 0) &&
			  (memcmp(this->map_->m_cName, "huntzone4", 9) != 0)) return;
	iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			switch (this->m_pItemList[i]->m_sIDnum) {
				case 652: iSoX++;
					iSoxH = i;
					break;
			}
		}
	if (iSoX > 0) {
		iNamingValue = this->map_->iGetEmptyNamingValue();
		if (iNamingValue == -1) {
		} else {
			std::memset(cNpcName, 0, sizeof (cNpcName));
			switch (iDice(1, 5)) {
				case 1: strcpy(cNpcName, "Wyvern");
					iNpcID = 66;
					break;
				case 2: strcpy(cNpcName, "Hellclaw");
					iNpcID = 49;
					break;
				case 3: strcpy(cNpcName, "Demon");
					iNpcID = 31;
					break;
				case 4: strcpy(cNpcName, "Tigerworm");
					iNpcID = 50;
					break;
				case 5: strcpy(cNpcName, "Gagoyle");
					iNpcID = 52;
					break;
			}
			std::memset(cName, 0, sizeof (cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = this->map_->id_ + 65;
			std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
			tX = (int) this->m_sX;
			tY = (int) this->m_sY;
			if (this->map_->bCreateNewNpc(cNpcName, cName, 0, (rand() % 9),
					  DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
				this->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				wsprintf(G_cTxt, "(%s) Player: (%s) - RedBall used %s [%s(%d, %d)] ", this->m_cIPaddress, this->m_cCharName, cNpcName, this->map_->m_cName, tX, tY);
				PutItemLogFileList(G_cTxt);
			}
		}
		for (x = 1; x < DEF_MAXCLIENTS; x++)
			if ((m_pClientList[x] != nullptr) && (m_pClientList[x]->m_bIsInitComplete == true)) {
				m_pClientList[x]->SendNotifyMsg(0, DEF_NOTIFY_SPAWNEVENT, tX, tY, iNpcID, nullptr, 0, 0);
			}
		this->ItemDepleteHandler(iSoxH, true, true);
	}
}

void CClient::Command_BlueBall(char */*pData*/, uint32_t /*dwMsgSize*/) {
	char cName_Master[10], cName_Slave[10], cNpcName[256], cWaypoint[11], cSA;
	int pX;
	int pY;
	int j;
	int iNum;
	int iNamingValue;
	int iNpcID;
	register int i, x;
	bool bMaster;
	int iSoxH;
	int iSoX;
	if (this->markedForDeletion_) return;
	if ((memcmp(this->map_->m_cName, "huntzone1", 9) != 0) &&
			  (memcmp(this->map_->m_cName, "huntzone2", 9) != 0) &&
			  (memcmp(this->map_->m_cName, "huntzone3", 9) != 0) &&
			  (memcmp(this->map_->m_cName, "huntzone4", 9) != 0)) return;
	iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (this->m_pItemList[i] != nullptr) {
			switch (this->m_pItemList[i]->m_sIDnum) {
				case 654: iSoX++;
					iSoxH = i;
					break;
			}
		}
	if (iSoX > 0) {
		iNamingValue = this->map_->iGetEmptyNamingValue();
		if (iNamingValue == -1) {
		} else {
			std::memset(cNpcName, 0, sizeof (cNpcName));
			switch (iDice(1, 38)) {
				case 1: strcpy(cNpcName, "Slime");
					iNpcID = 10;
					break;
				case 2: strcpy(cNpcName, "Giant-Ant");
					iNpcID = 15;
					break;
				case 3: strcpy(cNpcName, "Zombie");
					iNpcID = 17;
					break;
				case 4: strcpy(cNpcName, "Scorpion");
					iNpcID = 16;
					break;
				case 5: strcpy(cNpcName, "Skeleton");
					iNpcID = 11;
					break;
				case 6: strcpy(cNpcName, "Orc-Mage");
					iNpcID = 14;
					break;
				case 7: strcpy(cNpcName, "Clay-Golem");
					iNpcID = 23;
					break;
				case 8: strcpy(cNpcName, "Stone-Golem");
					iNpcID = 12;
					break;
				case 9: strcpy(cNpcName, "Hellbound");
					iNpcID = 27;
					break;
				case 10: strcpy(cNpcName, "Giant-Frog");
					iNpcID = 57;
					break;
				case 11: strcpy(cNpcName, "Troll");
					iNpcID = 28;
					break;
				case 12: strcpy(cNpcName, "Cyclops");
					iNpcID = 13;
					break;
				case 13: strcpy(cNpcName, "Ice-Golem");
					iNpcID = 65;
					break;
				case 14: strcpy(cNpcName, "Beholder");
					iNpcID = 53;
					break;
				case 15: strcpy(cNpcName, "Cannibal-Plant");
					iNpcID = 60;
					break;
				case 16: strcpy(cNpcName, "Orge");
					iNpcID = 29;
					break;
				case 17: strcpy(cNpcName, "Mountain-Giant");
					iNpcID = 58;
					break;
				case 18: strcpy(cNpcName, "DireBoar");
					iNpcID = 62;
					break;
				case 19: strcpy(cNpcName, "Liche");
					iNpcID = 30;
					break;
				case 20: strcpy(cNpcName, "Stalker");
					iNpcID = 48;
					break;
				case 21: strcpy(cNpcName, "WereWolf");
					iNpcID = 33;
					break;
				case 22: strcpy(cNpcName, "Dark-Elf");
					iNpcID = 54;
					break;
				case 23: strcpy(cNpcName, "Frost");
					iNpcID = 63;
					break;
				case 24: strcpy(cNpcName, "Orc");
					iNpcID = 14;
					break;
				case 25: strcpy(cNpcName, "Ettin");
					iNpcID = 59;
					break;
				case 26: strcpy(cNpcName, "Tentocle");
					iNpcID = -1;
					break;
				case 27: strcpy(cNpcName, "Giant-Crayfish");
					iNpcID = -1;
					break;
				case 28: strcpy(cNpcName, "Giant-Plant");
					iNpcID = -1;
					break;
				case 29: strcpy(cNpcName, "Rudolph");
					iNpcID = -1;
					break;
				case 30: strcpy(cNpcName, "Claw-Turtle");
					iNpcID = -1;
					break;
				case 31: strcpy(cNpcName, "Centaurus");
					iNpcID = -1;
					break;
				case 32: strcpy(cNpcName, "Barlog");
					iNpcID = -1;
					break;
				case 33: strcpy(cNpcName, "Giant-Lizard");
					iNpcID = -1;
					break;
				case 34: strcpy(cNpcName, "MasterMage-Orc");
					iNpcID = -1;
					break;
				case 35: strcpy(cNpcName, "Minotaurs");
					iNpcID = -1;
					break;
				case 36: strcpy(cNpcName, "Unicorn");
					iNpcID = -1;
					break;
				case 37: strcpy(cNpcName, "Nizie");
					iNpcID = -1;
					break;
			}
			iNum = 10;
			cSA = 0;
			pX = this->m_sX;
			pY = this->m_sY;
			/*std::memset(cBallMessage, 0, sizeof(cBallMessage));
			wsprintf(cBallMessage, "BlueBall event-%s %s(%d,%d)", cNpcName, this->map_->m_cName, pX, pY);
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((this->m_cLocation) == (m_pClientList[i]->m_cLocation))
					ShowClientMsg(i, cBallMessage);*/
			wsprintf(G_cTxt, "(!) BlueBallEvent: SummonMob (%s)-(%d)", cNpcName, iNum);
			PutLogList(G_cTxt);
			iNamingValue = this->map_->iGetEmptyNamingValue();
			if (iNamingValue != -1) {
				std::memset(cName_Master, 0, sizeof (cName_Master));
				wsprintf(cName_Master, "XX%d", iNamingValue);
				cName_Master[0] = '_';
				cName_Master[1] = this->map_->id_ + 65;
				if ((bMaster = this->map_->bCreateNewNpc(cNpcName, cName_Master, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false, true)) == false) {
					this->map_->SetNamingValueEmpty(iNamingValue);
				}
			}
			for (j = 0; j < (iNum - 1); j++) {
				iNamingValue = this->map_->iGetEmptyNamingValue();
				if (iNamingValue != -1) {
					// Slave Mob
					std::memset(cName_Slave, 0, sizeof (cName_Slave));
					wsprintf(cName_Slave, "XX%d", iNamingValue);
					cName_Slave[0] = '_';
					cName_Slave[1] = this->map_->id_ + 65;
					if (this->map_->bCreateNewNpc(cNpcName, cName_Slave, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false) == false) {
						this->map_->SetNamingValueEmpty(iNamingValue);
					} else {
						// Slave
						bSetNpcFollowMode(cName_Slave, cName_Master, DEF_OWNERTYPE_NPC);
					}
				}
			}
			//delete pStrTok;
		}
	}
	for (x = 1; x < DEF_MAXCLIENTS; x++)
		if ((m_pClientList[x] != nullptr) && (m_pClientList[x]->m_bIsInitComplete == true)) {
			m_pClientList[x]->SendNotifyMsg(0, DEF_NOTIFY_SPAWNEVENT, pX, pY, iNpcID, nullptr, 0, 0);
		}
	this->ItemDepleteHandler(iSoxH, true, true);
}

void CClient::RequestCreatePartyHandler() {
	if (this->markedForDeletion_) return;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_iAdminUserLevel > 0 && this->m_iAdminUserLevel < 3) return; // v2.23 22/04/06 15:24 AdminSecurity
	if (this->m_iPartyStatus != DEF_PARTYSTATUS_NULL) {
		return;
	}
	this->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
}

void CClient::PartyOperationResult_Create(char *pName, int iResult, int iPartyID) {
	int i;
	if (this->markedForDeletion_) return;
	if (strcmp(this->m_cCharName, pName) != 0) return;
	switch (iResult) {
		case 0:
			if (this->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(this->m_cCharName, pName) != 0) return;
			this->m_iPartyID = 0;
			this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			this->m_iReqJoinPartyClientH = 0;
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 1, 0, 0, nullptr);
			break;
		case 1:
			if (this->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(this->m_cCharName, pName) != 0) return;
			this->m_iPartyID = iPartyID;
			this->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 1, 1, 0, nullptr);
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[this->m_iPartyID].iIndex[i] == 0) {
					m_stPartyInfo[this->m_iPartyID].iIndex[i] = iClientH;
					m_stPartyInfo[this->m_iPartyID].iTotalMembers++;
					goto PORC_LOOPBREAK1;
				}
PORC_LOOPBREAK1:
			;
			if ((this->m_iReqJoinPartyClientH != 0) && (strlen(this->m_cReqJoinPartyName) != 0)) {
				this->m_iReqJoinPartyClientH = 0;
				std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
			}
			break;
	}
}

void CClient::PartyOperationResult_Join(char *pName, int iResult, int iPartyID) {
	int i;
	if (this->markedForDeletion_) return;
	switch (iResult) {
		case 0:
			if (this->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(this->m_cCharName, pName) != 0) return;
			this->m_iPartyID = 0;
			this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 4, 0, 0, pName);
			this->m_iReqJoinPartyClientH = 0;
			std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
			break;
		case 1:
			if (this->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(this->m_cCharName, pName) != 0) return;
			this->m_iPartyID = iPartyID;
			this->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 4, 1, 0, pName);
			this->m_iReqJoinPartyClientH = 0;
			std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[this->m_iPartyID].iIndex[i] == 0) {
					m_stPartyInfo[this->m_iPartyID].iIndex[i] = iClientH;
					m_stPartyInfo[this->m_iPartyID].iTotalMembers++;
					goto PORC_LOOPBREAK1;
				}
PORC_LOOPBREAK1:
			;
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((i != iClientH) && (m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 4, 1, 0, pName);
				}
			break;
	}
}

void CClient::PartyOperationResult_Dismiss(char *pName, int iResult, int iPartyID) {
	int i;
	switch (iResult) {
		case 0:
			break;
		case 1:
			if (iClientH == 0) {
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cCharName, pName) == 0)) {
						iClientH = i;
						goto PORD_LOOPBREAK;
					}
PORD_LOOPBREAK:
				;
				for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
					if (m_stPartyInfo[iPartyID].iIndex[i] == iClientH) {
						m_stPartyInfo[iPartyID].iIndex[i] = 0;
						m_stPartyInfo[iPartyID].iTotalMembers--;
						//testcode
						wsprintf(G_cTxt, "PartyID:%d member:%d Out Total:%d", iPartyID, iClientH, m_stPartyInfo[iPartyID].iTotalMembers);
						PutLogList(G_cTxt);
						goto PORC_LOOPBREAK1;
					}
PORC_LOOPBREAK1:
				;
				for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
					if ((m_stPartyInfo[iPartyID].iIndex[i] == 0) && (m_stPartyInfo[iPartyID].iIndex[i + 1] != 0)) {
						m_stPartyInfo[iPartyID].iIndex[i] = m_stPartyInfo[iPartyID].iIndex[i + 1];
						m_stPartyInfo[iPartyID].iIndex[i + 1] = 0;
					}
				if (!this->markedForDeletion_) {
					this->m_iPartyID = 0;
					this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
					this->m_iReqJoinPartyClientH = 0;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 6, 1, 0, pName);
					}
				return;
			}
			if ((!this->markedForDeletion_) && (this->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)) return;
			if ((!this->markedForDeletion_) && (strcmp(this->m_cCharName, pName) != 0)) return;
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 6, 1, 0, pName);
				}
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[iPartyID].iIndex[i] == iClientH) {
					m_stPartyInfo[iPartyID].iIndex[i] = 0;
					m_stPartyInfo[iPartyID].iTotalMembers--;
					//testcode
					wsprintf(G_cTxt, "PartyID:%d member:%d Out Total:%d", iPartyID, iClientH, m_stPartyInfo[iPartyID].iTotalMembers);
					PutLogList(G_cTxt);
					goto PORC_LOOPBREAK2;
				}
PORC_LOOPBREAK2:
			;
			for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
				if ((m_stPartyInfo[iPartyID].iIndex[i] == 0) && (m_stPartyInfo[iPartyID].iIndex[i + 1] != 0)) {
					m_stPartyInfo[iPartyID].iIndex[i] = m_stPartyInfo[iPartyID].iIndex[i + 1];
					m_stPartyInfo[iPartyID].iIndex[i + 1] = 0;
				}
			if (!this->markedForDeletion_) {
				this->m_iPartyID = 0;
				this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				this->m_iReqJoinPartyClientH = 0;
			}
			break;
	}
}

void CClient::RequestJoinPartyHandler(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cName[12];
	class CStrTok * pStrTok;
	int i;
	if (this->markedForDeletion_) return;
	if (this->m_iPartyStatus != DEF_PARTYSTATUS_NULL) return;
	if (this->m_iAdminUserLevel > 0 && this->m_iAdminUserLevel < 3) return; // v2.23 22/04/06 15:24 AdminSecurity
	if ((dwMsgSize) <= 0) return;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, token);
	} else {
		delete pStrTok;
		return;
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cCharName, cName) == 0)) {
			if ((m_pClientList[i]->m_iPartyID == 0) || (m_pClientList[i]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM)) {
				delete pStrTok;
				return;
			}
			delete pStrTok;
			return;
		}
	this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
	delete pStrTok;
}

void CClient::RequestDismissPartyHandler() {
	if (this->markedForDeletion_) return;
	if (this->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;
	this->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
}

void CClient::GetPartyInfoHandler() {
	if (this->markedForDeletion_) return;
	if (this->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;
}

void CClient::PartyOperationResult_Info(char * pName, int iTotal, char *pNameList) {
	if (this->markedForDeletion_) return;
	if (strcmp(this->m_cCharName, pName) != 0) return;
	if (this->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;
	this->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 5, 1, iTotal, pNameList);
}

void CClient::RequestDeletePartyHandler() {
	if (this->markedForDeletion_) return;
	if (this->m_iPartyID != 0) {
		this->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
	}
}

void CClient::RequestAcceptJoinPartyHandler(int iResult) {
	int iH;
	if (this->markedForDeletion_) return;
	switch (iResult) {
		case 0:
			iH = this->m_iReqJoinPartyClientH;
			if (m_pClientList[iH] == nullptr) {
				return;
			}
			if (strcmp(m_pClientList[iH]->m_cCharName, this->m_cReqJoinPartyName) != 0) {
				return;
			}
			if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
				return;
			}
			if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, this->m_cCharName) != 0)) {
				return;
			}
			m_pClientList[iH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
			m_pClientList[iH]->m_iPartyID = 0;
			m_pClientList[iH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iH]->m_iReqJoinPartyClientH = 0;
			std::memset(m_pClientList[iH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iH]->m_cReqJoinPartyName));
			this->m_iReqJoinPartyClientH = 0;
			std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
			break;
		case 1:
			if ((this->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) && (this->m_iPartyID != 0)) {
				iH = this->m_iReqJoinPartyClientH;
				if (m_pClientList[iH] == nullptr) {
					return;
				}
				if (strcmp(m_pClientList[iH]->m_cCharName, this->m_cReqJoinPartyName) != 0) {
					return;
				}
				if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
					return;
				}
				if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, this->m_cCharName) != 0)) {
					return;
				}
			} else {
				iH = this->m_iReqJoinPartyClientH;
				if (m_pClientList[iH] == nullptr) {
					return;
				}
				if (strcmp(m_pClientList[iH]->m_cCharName, this->m_cReqJoinPartyName) != 0) {
					return;
				}
				if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
					return;
				}
				if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, this->m_cCharName) != 0)) {
					return;
				}
				if (this->m_iPartyStatus == DEF_PARTYSTATUS_NULL) {
					this->RequestCreatePartyHandler();
				} else {
				}
			}
			break;
		case 2:
			if ((this->m_iPartyID != 0) && (this->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)) {
				this->RequestDismissPartyHandler();
			} else {
				iH = this->m_iReqJoinPartyClientH;
				if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_iReqJoinPartyClientH == iClientH) &&
						  (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, this->m_cCharName) == 0)) {
					m_pClientList[iH]->m_iReqJoinPartyClientH = 0;
					std::memset(m_pClientList[iH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iH]->m_cReqJoinPartyName));
				}
				this->m_iPartyID = 0;
				this->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				this->m_iReqJoinPartyClientH = 0;
				std::memset(this->m_cReqJoinPartyName, 0, sizeof (this->m_cReqJoinPartyName));
			}
			break;
	}
}

void CClient::TimeHitPointsUp() {
	register int iMaxHP, iTemp, iTotal;
	double dV1;
	double dV2;
	double dV3;
	/*
	.text:0043A679                 mov     [ebp+m_pClientList], ecx
	.text:0043A67C                 mov     eax, [ebp+iClientH]
	.text:0043A67F                 mov     ecx, [ebp+m_pClientList]
	.text:0043A682                 cmp     dword ptr [ecx+eax*4+16Ch], 0 ; m_pClientList[iClientH] == 0
	.text:0043A68A                 jnz     short loc_43A691;
	.text:0043A68C                 jmp     loc_43A8F4;
	 */
	if (this->markedForDeletion_) return;
	/*
	.text:0043A691                 mov     edx, [ebp+iClientH]
	.text:0043A694                 mov     eax, [ebp+m_pClientList]
	.text:0043A697                 mov     ecx, [eax+edx*4+16Ch] ; this->
	.text:0043A69E                 cmp     dword ptr [ecx+2Ch], 0 ; m_bIsInitComplete == 0
	.text:0043A6A2                 jnz     short loc_43A6A9;
	.text:0043A6A4                 jmp     break loc_43A8F4;
	 */
	if (this->m_bIsInitComplete == false) return;
	/*
	.text:0043A6A9                 mov     edx, [ebp+iClientH]
	.text:0043A6AC                 mov     eax, [ebp+m_pClientList]
	.text:0043A6AF                 mov     ecx, [eax+edx*4+16Ch] ; this->
	.text:0043A6B6                 cmp     dword ptr [ecx+0B6Ch], 0 ; m_iHungerStatus <= 0
	.text:0043A6BD                 jg      short loc_43A6C4
	.text:0043A6BF                 jmp     loc_43A8F4
	 */
	if (this->m_iHungerStatus <= 0) return;
	/*
	.text:0043A6C4                 mov     edx, [ebp+iClientH]
	.text:0043A6C7                 mov     eax, [ebp+m_pClientList]
	.text:0043A6CA                 mov     ecx, [eax+edx*4+16Ch] ; this->
	.text:0043A6D1                 cmp     dword ptr [ecx+0C8h], 1 ; m_bIsKilled == true
	.text:0043A6D8                 jnz     short loc_43A6DF
	.text:0043A6DA                 jmp     loc_43A8F4
	 */
	if (this->m_bIsKilled == true) return;
	/*
	.text:0043A6DF                 mov     edx, [ebp+iClientH]
	.text:0043A6E2                 mov     eax, [ebp+m_pClientList]
	.text:0043A6E5                 mov     ecx, [eax+edx*4+16Ch] ; this->
	.text:0043A6EC                 cmp     dword ptr [ecx+864h], 1 ; m_pClientList[i]->m_bSkillUsingStatus[19]
	.text:0043A6F3                 jnz     short loc_43A6FA
	.text:0043A6F5                 jmp     loc_43A8F4
	 */
	if (this->m_bSkillUsingStatus[19] == true) return;
	/*
	.text:0043A6FA                 mov     edx, [ebp+iClientH]
	.text:0043A6FD                 push    edx
	.text:0043A6FE                 mov     ecx, [ebp+m_pClientList]
	.text:0043A701                 call    CGame__iGetMaxHP
	.text:0043A706                 mov     [ebp+iMaxHP], eax
	 */
	iMaxHP = this->iGetMaxHP();
	/*
	.text:0043A709                 mov     eax, [ebp+iClientH]
	.text:0043A70C                 mov     ecx, [ebp+m_pClientList]
	.text:0043A70F                 mov     edx, [ecx+eax*4+16Ch]
	.text:0043A716                 mov     eax, [edx+0ACh] ; m_iHP < iMaxHP
	.text:0043A71C                 cmp     eax, [ebp+iMaxHP]
	.text:0043A71F                 jge     loc_43A8DD
	 */
	if (this->m_iHP < iMaxHP) {
		/*
		.text:0043A725                 mov     ecx, [ebp+iClientH]
		.text:0043A728                 mov     edx, [ebp+m_pClientList]
		.text:0043A72B                 mov     eax, [edx+ecx*4+16Ch] ; this->
		.text:0043A732                 mov     ecx, [eax+120h] ; m_iVit
		.text:0043A738                 push    ecx
		.text:0043A739                 push    1
		.text:0043A73B                 mov     ecx, [ebp+m_pClientList]
		.text:0043A73E                 call    CGame__iDice
		.text:0043A743                 mov     [ebp+iTemp], eax
		 */
		iTemp = iDice(1, (this->m_iVit));
		/*
		.text:0043A746                 mov     edx, [ebp+iClientH]
		.text:0043A749                 mov     eax, [ebp+m_pClientList]
		.text:0043A74C                 mov     ecx, [eax+edx*4+16Ch] ; this->
		.text:0043A753                 mov     eax, [ecx+120h] ; m_iVit
		.text:0043A759                 cdq
		.text:0043A75A                 sub     eax, edx
		.text:0043A75C                 sar     eax, 1
		.text:0043A75E                 cmp     [ebp+iTemp], eax
		.text:0043A761                 jge     short loc_43A77E
		.text:0043A763                 mov     edx, [ebp+iClientH]
		.text:0043A766                 mov     eax, [ebp+m_pClientList]
		.text:0043A769                 mov     ecx, [eax+edx*4+16Ch] ; this->
		.text:0043A770                 mov     eax, [ecx+120h] ; m_iVit
		.text:0043A776                 cdq
		.text:0043A777                 sub     eax, edx
		.text:0043A779                 sar     eax, 1
		.text:0043A77B                 mov     [ebp+iTemp], eax
		 */
		if (iTemp < (this->m_iVit / 2)) iTemp = (this->m_iVit / 2);
		/*
		.text:0043A77E                 mov     edx, [ebp+iClientH]
		.text:0043A781                 mov     eax, [ebp+m_pClientList]
		.text:0043A784                 mov     ecx, [eax+edx*4+16Ch] ; this->
		.text:0043A78B                 cmp     dword ptr [ecx+0C1Ch], 0 ; m_iSideEffect_MaxHPdown != 0
		.text:0043A792                 jz      short loc_43A7B3
		 */
		if (this->m_iSideEffect_MaxHPdown != 0)
			/*
			.text:0043A7A5                 idiv    dword ptr [ecx+0C1Ch] ; m_iSideEffect_MaxHPdown
			.text:0043A7AB                 mov     edx, [ebp+iTemp] ; (iTemp / m_iSideEffect_MaxHPdown)
			.text:0043A7AE                 sub     edx, eax
			.text:0043A7B0                 mov     [ebp+iTemp], edx ; iTemp =
			 */
			iTemp -= (iTemp / this->m_iSideEffect_MaxHPdown);
		/*
		.text:0043A7B3                 mov     eax, [ebp+iClientH]
		.text:0043A7B6                 mov     ecx, [ebp+m_pClientList]
		.text:0043A7B9                 mov     edx, [ecx+eax*4+16Ch]
		.text:0043A7C0                 mov     eax, [ebp+iTemp]
		.text:0043A7C3                 add     eax, [edx+0B0h]
		.text:0043A7C9                 mov     [ebp+iTotal], eax
		 */
		iTotal = iTemp + this->m_iHPstock;
		/*
		.text:0043A7CC                 mov     ecx, [ebp+iClientH]
		.text:0043A7CF                 mov     edx, [ebp+m_pClientList]
		.text:0043A7D2                 mov     eax, [edx+ecx*4+16Ch] ; this->
		.text:0043A7D9                 mov     ecx, [ebp+iTotal]
		.text:0043A7DC                 add     ecx, [eax+0B4h]
		.text:0043A7E2                 mov     [ebp+iTotal], ecx ; iTotal =
		.text:0043A7E5                 mov     edx, [ebp+iClientH]
		.text:0043A7E8                 mov     eax, [ebp+m_pClientList]
		.text:0043A7EB                 mov     ecx, [eax+edx*4+16Ch]
		.text:0043A7F2                 cmp     dword ptr [ecx+0E60h], 0 ; m_iAddHP == 0
		.text:0043A7F9                 jz      short loc_43A830
		.text:0043A7FB                 fild    [ebp+iTotal]
		.text:0043A7FE                 fstp    [ebp+var_20]
		.text:0043A801                 mov     edx, [ebp+iClientH]
		.text:0043A804                 mov     eax, [ebp+m_pClientList]
		.text:0043A807                 mov     ecx, [eax+edx*4+16Ch]
		.text:0043A80E                 fild    dword ptr [ecx+0E60h] ; m_iAddHP
		.text:0043A814                 fst     [ebp+var_28]
		.text:0043A817                 fdiv    ds:dbl_4ECBB8
		.text:0043A81D                 fmul    [ebp+var_20]
		.text:0043A820                 fst     [ebp+var_18]
		.text:0043A823                 call    __ftol
		.text:0043A828                 mov     edx, [ebp+iTotal]
		.text:0043A82B                 add     edx, eax
		.text:0043A82D                 mov     [ebp+iTotal], edx
		 */
		if (this->m_iAddHP != 0) {
			dV2 = (double) iTotal;
			dV3 = (double) this->m_iAddHP;
			dV1 = (dV3 / 100.0f) * dV2;
			iTotal += (int) dV1;
		}
		/*
		.text:0043A830                 mov     eax, [ebp+iClientH]
		.text:0043A833                 mov     ecx, [ebp+m_pClientList]
		.text:0043A836                 mov     edx, [ecx+eax*4+16Ch] ; this->
		.text:0043A83D                 mov     eax, [edx+0ACh] ; m_iHP
		.text:0043A843                 add     eax, [ebp+iTotal]
		.text:0043A846                 mov     ecx, [ebp+iClientH]
		.text:0043A849                 mov     edx, [ebp+m_pClientList]
		.text:0043A84C                 mov     ecx, [edx+ecx*4+16Ch] ; this->
		.text:0043A853                 mov     [ecx+0ACh], eax ; m_iHP
		 */
		this->m_iHP += iTotal;
		/*
		.text:0043A859                 mov     edx, [ebp+iClientH]
		.text:0043A85C                 mov     eax, [ebp+m_pClientList]
		.text:0043A85F                 mov     ecx, [eax+edx*4+16Ch] ; this->
		.text:0043A866                 mov     edx, [ecx+0ACh] ; m_iHP
		.text:0043A86C                 cmp     edx, [ebp+iMaxHP]
		.text:0043A86F                 jle     short loc_43A887
		.text:0043A871                 mov     eax, [ebp+iClientH]
		.text:0043A874                 mov     ecx, [ebp+m_pClientList]
		.text:0043A877                 mov     edx, [ecx+eax*4+16Ch] ; this->
		.text:0043A87E                 mov     eax, [ebp+iMaxHP]
		.text:0043A881                 mov     [edx+0ACh], eax ; m_iHP
		 */
		if (this->m_iHP > iMaxHP) this->m_iHP = iMaxHP;
		/*
		.text:0043A887                 mov     ecx, [ebp+iClientH]
		.text:0043A88A                 mov     edx, [ebp+m_pClientList]
		.text:0043A88D                 mov     eax, [edx+ecx*4+16Ch] ; this->
		.text:0043A894                 cmp     dword ptr [eax+m_iHP], 0
		.text:0043A89B                 jg      short loc_43A8B4
		.text:0043A89D                 mov     ecx, [ebp+iClientH]
		.text:0043A8A0                 mov     edx, [ebp+m_pClientList]
		.text:0043A8A3                 mov     eax, [edx+ecx*4+16Ch] ; this->
		.text:0043A8AA                 mov     dword ptr [eax+m_iHP], 0
		 */
		if (this->m_iHP <= 0) this->m_iHP = 0;
		/*
		.text:0043A8B4                 push    0               ; pString2
		.text:0043A8B6                 push    0               ; sV9
		.text:0043A8B8                 push    0               ; sV8
		.text:0043A8BA                 push    0               ; sV7
		.text:0043A8BC                 push    0               ; sV6
		.text:0043A8BE                 push    0               ; sV5
		.text:0043A8C0                 push    0               ; sV4
		.text:0043A8C2                 push    0               ; pString
		.text:0043A8C4                 push    0               ; sV3
		.text:0043A8C6                 push    0               ; sV2
		.text:0043A8C8                 push    0               ; sV1
		.text:0043A8CA                 push    0B07h           ; wMsgType DEF_NOTIFY_HP
		.text:0043A8CF                 mov     ecx, [ebp+iClientH]
		.text:0043A8D2                 push    ecx             ; iToH
		.text:0043A8D3                 push    0               ; iFromH
		.text:0043A8D5                 mov     ecx, [ebp+m_pClientList]
		.text:0043A8D8                 call    CGame__SendNotifyMsg
		 */
		this->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
	}
	/*
	.text:0043A8DD                 mov     edx, [ebp+iClientH]
	.text:0043A8E0                 mov     eax, [ebp+m_pClientList]
	.text:0043A8E3                 mov     ecx, [eax+edx*4+16Ch]
	.text:0043A8EA                 mov     dword ptr [ecx+0B0h], 0 ; m_iHPstock = 0
	 */
	this->m_iHPstock = 0;
}

char CClient::_cCheckHeroItemEquipped() {
	short sHeroLeggings;
	short sHeroHauberk;
	short sHeroArmor;
	short sHeroHelm;
	if (this->markedForDeletion_) return 0;
	sHeroHelm = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
	sHeroArmor = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
	sHeroHauberk = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
	sHeroLeggings = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
	if ((sHeroHelm < 0) || (sHeroLeggings < 0) || (sHeroArmor < 0) || (sHeroHauberk < 0)) return 0;
	if (this->m_pItemList[sHeroHelm] == nullptr) return 0;
	if (this->m_pItemList[sHeroLeggings] == nullptr) return 0;
	if (this->m_pItemList[sHeroArmor] == nullptr) return 0;
	if (this->m_pItemList[sHeroHauberk] == nullptr) return 0;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 403) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 411) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 1;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 407) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 415) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 2;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 404) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 412) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 1;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 408) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 416) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 2;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 405) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 413) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 1;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 409) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 417) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 2;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 406) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 414) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 1;
	if ((this->m_pItemList[sHeroHelm]->m_sIDnum == 410) &&
			  (this->m_pItemList[sHeroArmor]->m_sIDnum == 418) &&
			  (this->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
			  (this->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 2;
	return 0;
}

bool CClient::bPlantSeedBag(int dX, int dY, int iItemEffectValue1, int iItemEffectValue2,) {
	int iNamingValue;
	int tX;
	int tY;
	short sOwnerH;
	char cOwnerType;
	char cNpcName[21], cName[21], cNpcWaypointIndex[11];
	bool bRet;
	if (this->map_->m_iTotalAgriculture >= 200) {
		this->SendNotifyMsg(0, DEF_NOTIFY_NOMOREAGRICULTURE, 0, 0, 0, nullptr);
		return false;
	}
	if (iItemEffectValue2 > this->m_cSkillMastery[2]) {
		this->SendNotifyMsg(0, DEF_NOTIFY_AGRICULTURESKILLLIMIT, 0, 0, 0, nullptr);
		return false;
	}
	iNamingValue = this->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
		if (sOwnerH != 0 && sOwnerH == DEF_OWNERTYPE_NPC && m_pNpcList[sOwnerH]->m_cActionLimit == 5) {
			this->SendNotifyMsg(0, DEF_NOTIFY_AGRICULTURENOAREA, 0, 0, 0, nullptr);
			return false;
		} else {
			if (this->map_->bGetIsFarm(dX, dY) == false) {
				this->SendNotifyMsg(0, DEF_NOTIFY_AGRICULTURENOAREA, 0, 0, 0, nullptr);
				return false;
			}
			std::memset(cNpcName, 0, sizeof (cNpcName));
			strcpy(cNpcName, "Crops");
			std::memset(cName, 0, sizeof (cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = this->map_->id_ + 65;
			std::memset(cNpcWaypointIndex, 0, sizeof (cNpcWaypointIndex));
			tX = dX;
			tY = dY;
			bRet = this->map_->bCreateNewNpc(cNpcName, cName, 0, 0, DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypointIndex, nullptr, 0, 0, false, true);
			if (!bRet) {
				this->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (m_pNpcList[sOwnerH] == nullptr) return 0;
				m_pNpcList[sOwnerH]->m_cCropType = iItemEffectValue1;
				switch (iItemEffectValue1) {
					case 1: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 2: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 3: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 4: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 5: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 6: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 7: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 8: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 9: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 10: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 11: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 12: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					case 13: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2;
						break;
					default: m_pNpcList[sOwnerH]->m_cCropSkill = 100;
						break;
				}
				m_pNpcList[sOwnerH]->m_sAppr2 = 1;
				m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
				wsprintf(G_cTxt, "(skill:%d type:%d)plant(%s) Agriculture begin(%d,%d) sum(%d)!", m_pNpcList[sOwnerH]->m_cCropSkill, m_pNpcList[sOwnerH]->m_cCropType, cNpcName, tX, tY, this->map_->m_iTotalAgriculture);
				PutLogList(G_cTxt);
				return true;
			}
		}
	}
	return false;
}

void CClient::StormBringer(short dX, short dY) {
	char cOwnerType;
	short sOwner;
	short sAppr2;
	short sAttackerWeapon;
	int iDamage;
	int iTemp;
	int iV1;
	int iV2;
	int iV3;
	//Stormbringer
	if (this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
		this->map_->GetOwner(&sOwner, &cOwnerType, dX, dY);
		iTemp = this->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
		sAppr2 = (short) ((this->m_sAppr2 & 0xF000) >> 12);
		if (memcmp(this->m_pItemList[iTemp]->m_cName, "StormBringer", 12) == 0) {
			switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwner]->m_iAdminUserLevel < 3) {
						if (sAppr2 != 0) {
							iV1 = this->m_cAttackDiceThrow_L;
							iV2 = this->m_cAttackDiceRange_L;
							iV3 = this->m_cAttackBonus_L;
							if (this->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0) {
								iDamage = iDice(iV1 * 2, iV2 * 2) + iV3;
							} else {
								iDamage = iDice(iV1, iV2) + iV3;
							}
							m_pClientList[sOwner]->m_iHP -= iDamage;
							if (m_pClientList[sOwner]->m_iHP <= 0) {
								sAttackerWeapon = 1;
								m_pClientList[sOwner]->m_iHP = 0;
								m_pClientList[sOwner]->m_bIsKilled = true;
								m_pClientList[sOwner]->m_iLastDamage = iDamage;
								m_pClientList[sOwner]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
								m_pClientList[sOwner]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDYING, iDamage, sAttackerWeapon, 0);
								m_pClientList[sOwner]->map_->ClearOwner(14, sOwner, DEF_OWNERTYPE_PLAYER, m_pClientList[sOwner]->m_sX, m_pClientList[sOwner]->m_sY);
								m_pClientList[sOwner]->map_->SetDeadOwner(sOwner, DEF_OWNERTYPE_PLAYER, m_pClientList[sOwner]->m_sX, m_pClientList[sOwner]->m_sY);
							} else {
								m_pClientList[sOwner]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
								m_pClientList[sOwner]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
							}
						}
					}
					break;
			}
		}
	}
}

bool CClient::_bCheckCharacterData() {
	register int i;
	if ((this->m_iStr > m_sCharStatLimit) || (this->m_iVit > m_sCharStatLimit) || (this->m_iDex > m_sCharStatLimit) ||
			  (this->m_iMag > m_sCharStatLimit) || (this->m_iInt > m_sCharStatLimit) || (this->m_iCharisma > m_sCharStatLimit)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) stat points are greater then server accepts.", this->m_cIPaddress, this->m_cCharName);
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((this->m_iLevel > m_sMaxPlayerLevel) && (this->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) level above max server level.", this->m_cIPaddress, this->m_cCharName);
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if (this->m_iExp < 0) {
		this->m_iExp = DEF_PLAYER_MAX_EXPERIENCE; // juan249 - Exp negativa Fix.
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) experience is below 0 - (Exp:%d). - FIXED", this->m_cIPaddress, this->m_cCharName, this->m_iExp);
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((this->m_iHP > this->iGetMaxHP()) && (this->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) HP: current/maximum (%d/%d).", this->m_cIPaddress, this->m_cCharName, this->m_iHP, this->iGetMaxHP());
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((this->m_iMP > this->iGetMaxMP()) && (this->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) MP: current/maximum (%d/%d).", this->m_cIPaddress, this->m_cCharName, this->m_iMP, this->iGetMaxMP());
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((this->m_iSP > this->iGetMaxSP()) && (this->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) SP: current/maximum (%d/%d).", this->m_cIPaddress, this->m_cCharName, this->m_iSP, this->iGetMaxSP());
		PutHackLogFileList(G_cTxt);
		return false;
	}
	/*if (this->m_iAdminUserLevel != 0) {
		for (i = 0; i < DEF_MAXADMINS; i++) {
			if(strlen(m_stAdminList[i].m_cGMName) == 0) break; //No more GM's on list
			if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(this->m_cCharName))) {
				if(memcmp(m_stAdminList[i].m_cGMName, this->m_cCharName, strlen(this->m_cCharName)) != 0){
				}
				else {
					wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) is an unaccepted admin.", this->m_cIPaddress, this->m_cCharName);
					PutHackLogFileList(G_cTxt);
					return false;
				}
			}
		}
	}*/
	for (i = 0; i < DEF_MAXBANNED; i++) {
		if (strlen(m_stBannedList[i].m_cBannedIPaddress) == 0) break; //No more GM's on list
		if ((strlen(m_stBannedList[i].m_cBannedIPaddress)) == (strlen(this->m_cIPaddress))) {
			if (memcmp(m_stBannedList[i].m_cBannedIPaddress, this->m_cIPaddress, strlen(this->m_cIPaddress)) == 0) {
				wsprintf(G_cTxt, "Client Rejected: Banned: (%s)", this->m_cIPaddress);
				PutLogList(G_cTxt);
				return false;
			} else {
			}
		}
	}
	return true;
}

void CClient::RequestHeldenianTeleport(char * pData, uint32_t /*dwMsgSize*/) {
	char cTmpName[21], * cp, cData[512], cMapName[11];
	int iRet;
	int iWhyReturn;
	int iProcessed;
	int tX;
	int tY;
	int cLoc, * ip;
	uint32_t *dwp;
	iProcessed = 1;
	if (this->markedForDeletion_) return;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, cp);
	if (strcmp(cTmpName, "Gail") == 0) {
		std::memset(cData, 0, sizeof (cData));
		dwp = (uint32_t *) (cData);
		*dwp = MSGID_REQUEST_GAILTELEPORT;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		if ((m_bIsHeldenianMode == 1) &&
				  (this->m_bIsPlayerCivil != true) &&
				  ((this->m_cSide == 2 || (this->m_cSide == 1)))) {
			if (m_cHeldenianModeType == 1) {
				std::memset(cMapName, 0, sizeof (cMapName));
				memcpy(cMapName, "BtField", 10);
				if (this->m_cSide == 1) {
					tX = 68;
					tY = 225;
					cLoc = 1;
				} else if (this->m_cSide == 2) {
					tX = 202;
					tY = 70;
					cLoc = 2;
				}
			} else if (m_cHeldenianModeType == 2) {
				std::memset(cMapName, 0, sizeof (cMapName));
				memcpy(cMapName, "HRampart", 10);
				if (this->m_cSide == m_sLastHeldenianWinner) {
					tX = 81;
					tY = 42;
					cLoc = 3;
				} else {
					tX = 156;
					tY = 153;
					cLoc = 4;
				}
			}
			iProcessed = 1;
			iWhyReturn = 0;
			goto SENDPLAYER;
		}
		iProcessed = 0;
		iWhyReturn = 0;
	}
SENDPLAYER:
	;
	ip = (int *) cp;
	*ip = iProcessed;
	cp += 4;
	ip = (int *) cp;
	*ip = cLoc;
	cp += 4;
	memcpy((char *) cp, cMapName, 10);
	cp += 10;
	ip = (int *) cp;
	*ip = tX;
	cp += 4;
	ip = (int *) cp;
	*ip = tY;
	cp += 4;
	ip = (int *) cp;
	*ip = iWhyReturn;
	cp += 4;
	iRet = this->m_pXSock->iSendMsg(cData, 36);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			this->DeleteClient(true, true);
			break;
	}
}

void CClient::RequestNpcSpeakTeleport(char * pData, uint32_t /*dwMsgSize*/) {
	char cMapName[11], *cp;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	cp += 4;
	std::memset(cMapName, 0, sizeof (cMapName));
	switch (iDice(1, 2)) {
		case 1:
			switch (this->m_cSide) {
				case 0: this->RequestTeleportHandler("2   ", "BtField", 68, 225);
					break;
				case 1: this->RequestTeleportHandler("2   ", "BtField", 68, 225);
					break;
				case 2: this->RequestTeleportHandler("2   ", "BtField", 202, 70);
					break;
			}
			break;
	}
}

void CClient::ManualStartHeldenianMode(char *pData, uint32_t dwMsgSize) {
	char cHeldenianType;
	char cBuff[256], * token, seps[] = "= \t\n";
	SYSTEMTIME SysTime;
	class CStrTok * pStrTok;
	int iV1;
	if (this->m_iAdminUserLevel < 3)
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
	if (m_bIsHeldenianMode) return;
	if (m_bIsApocalypseMode) return;
	if (m_bIsCrusadeMode) return;
	if ((dwMsgSize != 0) && (pData != nullptr)) {
		m_bHeldenianRunning = true;
		GetLocalTime(&SysTime);
		std::memset(cBuff, 0, sizeof (cBuff));
		memcpy(cBuff, pData, dwMsgSize);
		pStrTok = new class CStrTok(cBuff, seps);
		token = pStrTok->pGet();
		token = pStrTok->pGet();
		if (token != nullptr) {
			iV1 = atoi(token);
			iV1 += (SysTime.wHour * 24 + SysTime.wMinute * 60);
			m_dwHeldenianStartHour = (iV1 / 24);
			m_dwHeldenianStartMinute = (iV1 / 60);
		}
		token = pStrTok->pGet();
		if (token != nullptr) {
			cHeldenianType = atoi(token);
			if ((cHeldenianType == 1) || (cHeldenianType == 2)) {
				m_cHeldenianModeType = cHeldenianType;
			}
		}
		delete pStrTok;
	}
	GlobalStartHeldenianMode();
	wsprintf(G_cTxt, "GM Order(%s): begin Heldenian", this->m_cCharName);
	PutLogFileList(G_cTxt);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
}

void CClient::ManualEndHeldenianMode(char */*pData*/, uint32_t /*dwMsgSize*/) {
	if (this->m_iAdminUserLevel < 3) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
	}
	if (m_bIsHeldenianMode) {
		GlobalEndHeldenianMode();
		m_bHeldenianRunning = false;
		wsprintf(G_cTxt, "GM Order(%s): end Heldenian", this->m_cCharName);
		bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	}
}

void CClient::CheckCrusadeResultCalculation() {
	if (this->markedForDeletion_) return;
	if (this->m_cVar == 1) return;
	if ((!m_bIsCrusadeMode) && (this->m_dwCrusadeGUID != 0)) {
		if (this->m_iWarContribution > DEF_MAXWARCONTRIBUTION) this->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
		if (this->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (m_iCrusadeWinnerSide == 0) {
				this->m_iExpStock += (this->m_iWarContribution / 6);
				this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, this->m_iWarContribution, nullptr);
				_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (this->m_iWarContribution / 6), nullptr);
			} else {
				if (m_iCrusadeWinnerSide == this->m_cSide) {
					if (this->m_iLevel <= 80) {
						this->m_iWarContribution += this->m_iLevel * 100;
					} else if (this->m_iLevel <= 100) {
						this->m_iWarContribution += this->m_iLevel * 40;
					} else this->m_iWarContribution += this->m_iLevel;
					this->m_iExpStock += this->m_iWarContribution;
					this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, this->m_iWarContribution, nullptr);
					_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, this->m_iWarContribution, nullptr);
				} else if (m_iCrusadeWinnerSide != this->m_cSide) {
					if (this->m_iLevel <= 80) {
						this->m_iWarContribution += this->m_iLevel * 100;
					} else if (this->m_iLevel <= 100) {
						this->m_iWarContribution += this->m_iLevel * 40;
					} else this->m_iWarContribution += this->m_iLevel;
					this->m_iExpStock += this->m_iWarContribution / 10;
					this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, -1 * this->m_iWarContribution, nullptr);
					_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (this->m_iWarContribution / 10), nullptr);
				}
			}
		} else {
			this->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
		}
		this->m_iCrusadeDuty = 0;
		this->m_iWarContribution = 0;
		this->m_dwCrusadeGUID = 0;
		this->m_dwSpeedHackCheckTime = timeGetTime();
		this->m_iSpeedHackCheckExp = this->m_iExp;
	}
}

void CClient::CheckHeldenianResultCalculation() {
	double dV1;
	double dV2;
	double dV3;
	if (this->markedForDeletion_) return;
	if (this->m_cWarType != 2) return;
	if ((m_cHeldenianModeType == 0) || (this->m_dwCrusadeGUID == 0)) return;
	if (this->m_iWarContribution > DEF_MAXWARCONTRIBUTION) this->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
	if (this->m_dwCrusadeGUID == m_dwHeldenianGUID) {
		if (m_sLastHeldenianWinner == 0) {
			this->m_iExpStock += this->m_iWarContribution / 6;
		} else if (this->m_cSide == m_sLastHeldenianWinner) {
			if (this->m_iLevel <= 80) {
				this->m_iWarContribution += this->m_iLevel * 200;
			} else if (this->m_iLevel > 100) {
				this->m_iWarContribution += this->m_iLevel * 100;
			} else {
				this->m_iWarContribution += this->m_iLevel * 30;
			}
			dV2 = (double) this->m_iExpStock;
			dV3 = (double) this->m_iWarContribution * 1.2;
			dV1 = dV2 + dV3;
			this->m_iExpStock += dV1;
		} else if (this->m_cSide != m_sLastHeldenianWinner) {
			if (this->m_iLevel <= 80) {
				this->m_iWarContribution += this->m_iLevel * 200;
			} else if (this->m_iLevel > 100) {
				this->m_iWarContribution += this->m_iLevel * 100;
			} else {
				this->m_iWarContribution += this->m_iLevel * 30;
			}
			this->m_iExpStock += this->m_iWarContribution / 5;
		}
		this->m_iWarContribution = 0;
		this->m_dwCrusadeGUID = 0;
		this->m_dwSpeedHackCheckTime = timeGetTime();
		this->m_iSpeedHackCheckExp = this->m_iExp;
	}
}

void CClient::RequestHeldenianScroll(char * pData, uint32_t /*dwMsgSize*/) {
	char *cp, cData[256], cTmpName[21];
	int iScroll;
	class CItem * pItem;
	int iRet;
	int iEraseReq;
	int iRequiredConstruction;
	char cItemName[21];
	short * sp;
	uint16_t * wp;
	int * ip;
	uint32_t * dwp;
	if (this->m_bIsInitComplete == false) return;
	if (this->_iGetItemSpaceLeft() == 0) {
		this->SendItemNotifyMsg(DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, cp);
	cp += 20;
	ip = (int *) cp;
	iScroll = (int) *ip;
	cp += 2;
	wsprintf(G_cTxt, "PC(%s) obtained a Scroll (%d).   %s(%d %d)", this->m_cCharName, iScroll, this->map_->m_cName, this->m_sX, this->m_sY);
	PutLogList(G_cTxt);
	switch (iScroll) {
		case 1:
			wsprintf(cItemName, "SummonScroll(SOR)");
			iRequiredConstruction = 2000;
			break;
		case 2:
			wsprintf(cItemName, "SummonScroll(ATK)");
			iRequiredConstruction = 3000;
			break;
		case 3:
			wsprintf(cItemName, "SummonScroll(ELF)");
			iRequiredConstruction = 1500;
			break;
		case 4:
			wsprintf(cItemName, "SummonScroll(DSK)");
			iRequiredConstruction = 3000;
			break;
		case 5:
			wsprintf(cItemName, "SummonScroll(HBT)");
			iRequiredConstruction = 4000;
			break;
		case 6:
			wsprintf(cItemName, "SummonScroll(BAR)");
			iRequiredConstruction = 3000;
			break;
		default:
			PutLogList("Gail asked to create a wrong item!");
			break;
	}
	pItem = nullptr;
	pItem = new class CItem;
	if (pItem == nullptr) return;
	if ((_bInitItemAttr(*pItem, cItemName) == true)) {
		pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
		pItem->m_sTouchEffectValue1 = this->m_sCharIDnum1;
		pItem->m_sTouchEffectValue2 = this->m_sCharIDnum2;
		pItem->m_sTouchEffectValue3 = this->m_sCharIDnum3;
		if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
			std::memset(cData, 0, sizeof (cData));
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMOBTAINED;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char) 0;
			cp++;
			sp = (short *) cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (uint16_t *) cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short *) cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor;
			cp++;
			*cp = (char) pItem->m_sItemSpecEffectValue2;
			cp++;
			dwp = (uint32_t *) cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;
			if (iEraseReq == 1) delete pItem;
			iRet = this->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					break;
			}
			this->m_iConstructionPoint -= iRequiredConstruction;
			this->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, this->m_iConstructionPoint, this->m_iWarContribution, 0, nullptr);
		} else {
			this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
			this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, this->m_sX, this->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = this->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					this->DeleteClient(true, true);
					break;
			}
		}
	} else {
		delete pItem;
		pItem = nullptr;
	}
}

void CClient::GetDkSet() {
	class CItem * pItem;
	int dkM[] = {706, 707, 708, 709, 710, 714, 915, -1};
	int dkW[] = {724, 725, 726, 728, 714, 709, 915, -1};
	int dmM[] = {711, 712, 713, 853, 717, 913, 915, -1};
	int dmW[] = {729, 730, 731, 853, 717, 914, 915, -1};
	int weapon;
	int iItemID;
	int i;
	int iEraseReq;
	if (this->m_iLevel < 200) return;
	for (i = 0; i <= 7; i++) {
		pItem = new class CItem;
		iItemID = -1;
		if (this->m_cSex == 1) {
			if (this->m_iStr > 100) {
				iItemID = dkM[i];
			} else {
				iItemID = dmM[i];
			}
		} else {
			if (this->m_iStr > 100) {
				iItemID = dkW[i];
			} else {
				iItemID = dmW[i];
			}
		}
		if (i == 7) {
			if (this->m_iStr > this->m_iInt) {
				if (this->m_cSex == 1) {
					weapon = 709;
					weapon = 2000;
				} else if (this->m_cSex == 2) {
					weapon = 709;
					weapon = 2000;
				}
			} else {
				if (this->m_cSex == 1) {
					weapon = 714;
				} else if (this->m_cSex == 2) {
					weapon = 714;
				}
			}
			iItemID = weapon;
		}
		if (iItemID != -1) //if any error occures, dont crash character
		{
			_bInitItemAttr(*pItem, iItemID);
			pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
			pItem->m_sTouchEffectValue1 = this->m_sCharIDnum1;
			pItem->m_sTouchEffectValue2 = this->m_sCharIDnum2;
			pItem->m_sTouchEffectValue3 = this->m_sCharIDnum3;
			this->_bAddClientItemList(pItem, &iEraseReq);
			this->SendItemNotifyMsg(DEF_NOTIFY_ITEMOBTAINED, pItem, 0);
		}
	}
}

void CClient::AdminOrder_BanIP(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char cMsg[100], cNick[20], cIPtoBan[15];
	int i;
	FILE * pFile;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel <= 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	if (this->m_bIsAdminCommandEnabled == false) return;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cNick, token);
	pStrTok->pGet();
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cNick, strlen(cNick)) == 0)) {
			strcpy(cIPtoBan, m_pClientList[i]->m_cIPaddress);
			//opens cfg file
			pFile = fopen("..\\GameConfigs\\BannedList.cfg", "a");
			//notifys to player banned
			wsprintf(cMsg, "You will be banned from the server right now!");
			ShowClientMsg(i, cMsg);
			//notifys to game master the ban is success
			wsprintf(cMsg, "%s was banned ip.", this->m_cCharName);
			this->ShowClientMsg(cMsg);
			//shows log
			wsprintf(G_cTxt, "<%d> Client IP Banned: (%s)", i, cIPtoBan);
			PutLogList(G_cTxt);
			//modifys cfg file
			fprintf(pFile, "\nbanned-ip = %s", cIPtoBan);
			fclose(pFile);
			//updates BannedList.cfg on the server
			LocalUpdateConfigs(3);
			//disconnects player, and he can't connect again.. :)
			m_pClientList[i]->DeleteClient(true, true);
			return;
		}
	}
	delete pStrTok;
}

void CClient::PlayerOrder_ShowDamage() {/*ShowDamage - Coded by EvilHit*/
	char cDamageMod[256];
	if (this->iDmgShowon == 1) {
		this->iDmgShowon = 0;
		std::memset(cDamageMod, 0, sizeof (cDamageMod));
		wsprintf(cDamageMod, "Show Damage is now disabled");
		this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
	} else {
		this->iDmgShowon = 1;
		std::memset(cDamageMod, 0, sizeof (cDamageMod));
		wsprintf(cDamageMod, "Show Damage is now enabled");
		this->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
	}
}

void CClient::PlayerOrder_GetCrits() {
	if (this->m_iSuperAttackLeft >= 300) {
		m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You can't buy more Criticals!.");
		return;
	}
	if (this->m_iAdminUserLevel > 0) {
		this->m_iSuperAttackLeft += 300;
		m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You bought Criticals");
		return;
	}
	uint32_t dwGoldCount;
	if (this->m_iLevel < 202) {
		dwGoldCount = dwGetItemCount(client, "Gold");
		if (dwGoldCount >= 5000) {
			SetItemCount(client, "Gold", dwGoldCount - 5000);
			this->m_iSuperAttackLeft += 300;
			m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You bought Criticals!.");
			return;
		}
	}
	m_pClientList[this->id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You haven't got the needed requirement to use this command!.");
}

void CClient::RequestRango(int iObjectID) {
	if ((iObjectID <= 0) || (iObjectID >= DEF_MAXCLIENTS)) return;
	if (m_pClientList[iObjectID] == nullptr) {
	} else {
		this->SendNotifyMsg(0, DEF_NOTIFY_REQRANGO, m_pClientList[iObjectID]->m_iEnemyKillCount, m_pClientList[iObjectID]->m_iRating, 0, nullptr);
	}
}

void CClient::GetTradeEKMantleHandler(int iItemID, char */*pString*/) {
	int i;
	int iNum;
	int iRet;
	int iEraseReq;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (this->_iGetItemSpaceLeft() == 0) {
		this->SendItemNotifyMsg(DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	if (m_pItemConfigList[iItemID] == nullptr) return;
	switch (iItemID) {
		case 848://Lighning Blade
			if (this->m_iEnemyKillCount <= 7799) return;
			this->m_iEnemyKillCount -= 7800;
			break;
		case 616:// Demon Slayer
			if (this->m_iEnemyKillCount <= 6999) return;
			this->m_iEnemyKillCount -= 7000;
			break;
		case 861://Berzerk Wand MS 20
			if (this->m_iEnemyKillCount <= 6199) return;
			this->m_iEnemyKillCount -= 6200;
			break;
		case 863://Kloness Wand MS 20
			if (this->m_iEnemyKillCount <= 6999) return;
			this->m_iEnemyKillCount -= 6000;
			break;
		case 3043://Strike of the Ghost
			if (this->m_iEnemyKillCount <= 5999) return;
			this->m_iEnemyKillCount -= 6000;
			break;
		case 850://Kloness Axe
			if (this->m_iEnemyKillCount <= 8999) return;
			this->m_iEnemyKillCount -= 9000;
			break;
	}
	std::memset(cItemName, 0, sizeof (cItemName));
	memcpy(cItemName, m_pItemConfigList[iItemID]->m_cName, 20);
	iNum = 1;
	for (i = 1; i <= iNum; i++) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {
			delete pItem;
		} else {
			if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
				if (this->m_iCurWeightLoad < 0) this->m_iCurWeightLoad = 0;
				wsprintf(G_cTxt, "(*) Get Trade EK Item : Char(%s) Player-EK(%d) Item Obtained(%s)", this->m_cCharName, this->m_iEnemyKillCount, cItemName);
				PutItemLogFileList(G_cTxt);
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;
				cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char) 0;
				cp++;
				sp = (short *) cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (uint16_t *) cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short *) cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				*cp = (char) pItem->m_sItemSpecEffectValue2;
				cp++;
				dwp = (uint32_t *) cp;
				*dwp = pItem->m_dwAttribute;
				cp += 4;
				if (iEraseReq == 1) delete pItem;
				iRet = this->m_pXSock->iSendMsg(cData, 53);
				this->iCalcTotalWeight();
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
				this->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, this->m_iEnemyKillCount, 0, 0, nullptr);
			} else {
				delete pItem;
				this->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = this->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						this->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CClient::PlayerCommandAddMaster(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iGuildRank != 0) {
		// ???? ??????? ?????* ?? ?????? ??????? ????.
		this->SendNotifyMsg(0, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cName, 0, sizeof (cName));
	std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				if (memcmp(this->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0) {
					// ????? ???????? ???? ?????? ????????.
					this->this->SendNotifyMsg(DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot Add Master...");
					delete pStrTok;
					return;
				}
				m_pClientList[i]->m_iGuildRank = 0;
				this->this->SendNotifyMsg(DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "New Guild Master Added");
				m_pClientList[i]->this->SendNotifyMsg(DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You are Now a GuildMaster");
			}
		delete pStrTok;
		return;
	}
	this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	delete pStrTok;
	return;
}

void CClient::PlayerOrder_DeleteGuildMaster(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iGuildRank != 0) {
		// ???? ??????? ?????* ?? ?????? ??????? ????.
		this->SendNotifyMsg(0, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cName, 0, sizeof (cName));
	std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
	std::memset(cTargetName, 0, sizeof (cTargetName));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) {
				if (memcmp(this->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0) {
					// ????? ???????? ???? ?????? ????????.
					this->this->SendNotifyMsg(DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: No se puede borrar el GuildMaster.");
					delete pStrTok;
					return;
				}
				m_pClientList[i]->m_iGuildRank = 12;
				this->this->SendNotifyMsg(DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "El GuildMaster fue borrado.");
				m_pClientList[i]->this->SendNotifyMsg(DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Ya no eres GuildMaster.");
			}
		delete pStrTok;
		return;
	}
	this->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	delete pStrTok;
	return;
}

void CClient::AdminOrder_SetCrit(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char *token, cBuff[256];
	class CStrTok *pStrTok;
	char *pk;
	char cNick[20];
	char cPK[5];
	char notice[100];
	int oldpk;
	int i;
	if ((dwMsgSize) <= 0)
		return;
	if (this->m_iAdminUserLevel == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cNick, token);
	pk = pStrTok->pGet();
	if (pk == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cPK, pk);
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr)
				  && (memcmp(m_pClientList[i]->m_cCharName, cNick, strlen(cNick)) == 0)) {
			oldpk = m_pClientList[i]->m_iSuperAttackLeft;
			m_pClientList[i]->m_iSuperAttackLeft = atoi(cPK);
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, m_pClientList[i]->m_iSuperAttackLeft, 0, 0, nullptr);
			wsprintf(notice, "Crit Count has been changed for player %s from %d to %d.", this->m_cCharName, oldpk, atoi(cPK));
		}
	}
	delete pStrTok;
}

void CClient::AdminOrder_SetEK(char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char * ek;
	char cNick[20];
	char cEK[5];
	char notice[100];
	int oldek;
	int i;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel <= 3) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cNick, token);
	ek = pStrTok->pGet();
	if (ek == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cEK, ek);
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cNick, strlen(cNick)) == 0)) {
			oldek = m_pClientList[i]->m_iEnemyKillCount;
			m_pClientList[i]->m_iEnemyKillCount = atoi(cEK);
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, m_pClientList[i]->m_iEnemyKillCount, 0, 0, nullptr);
			std::memset(notice, 0, sizeof (notice));
			wsprintf(notice, "Your EK Counts has been changed %d to %d.", oldek, atoi(cEK));
			this->ShowClientMsg(notice);
		}
	}
	delete pStrTok;
}

void CClient::AdminOrder_SetMAJ(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char *token, cBuff[256];
	class CStrTok *pStrTok;
	char *pk;
	char cNick[20];
	char cPK[5];
	char notice[100];
	int oldpk;
	int i;
	if ((dwMsgSize) <= 0)
		return;
	if (this->m_iAdminUserLevel == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cNick, token);
	pk = pStrTok->pGet();
	if (pk == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cPK, pk);
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cNick, strlen(cNick)) == 0)) {
			oldpk = m_pClientList[i]->m_iGizonItemUpgradeLeft;
			m_pClientList[i]->m_iGizonItemUpgradeLeft = atoi(cPK);
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[i]->m_iGizonItemUpgradeLeft, 0, 0, nullptr);
			wsprintf(notice, "Majestic Count has been changed for player %s from %d to %d.", this->m_cCharName, oldpk, atoi(cPK));
		}
	}
	delete pStrTok;
}

void CClient::AdminOrder_SetPK(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char *token, cBuff[256];
	class CStrTok *pStrTok;
	char *pk;
	char cNick[20];
	char cPK[5];
	char notice[100];
	int oldpk;
	int i;
	if ((dwMsgSize) <= 0)
		return;
	if (this->m_iAdminUserLevel == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cNick, token);
	pk = pStrTok->pGet();
	if (pk == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cPK, pk);
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cNick, strlen(cNick)) == 0)) {
			oldpk = m_pClientList[i]->m_iPKCount;
			m_pClientList[i]->m_iPKCount = atoi(cPK);
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PKPENALTY, m_pClientList[i]->m_iPKCount, 0, 0, nullptr);
			wsprintf(notice, "EK Count has been changed for player %s from %d to %d.", this->m_cCharName, oldpk, atoi(cPK));
		}
	}
	delete pStrTok;
}

void CClient::AdminOrder_SetRep(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char * rep;
	char cNick[20];
	char cRep[5];
	char notice[100];
	int oldrep;
	int i;
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel == 0) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cNick, token);
	rep = pStrTok->pGet();
	if (rep == nullptr) {
		delete pStrTok;
		return;
	}
	strcpy(cRep, rep);
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cCharName, cNick, strlen(cNick)) == 0)) {
			oldrep = m_pClientList[i]->m_iRating;
			m_pClientList[i]->m_iRating = atoi(cRep);
			wsprintf(notice, "Your Reputation Points change %d to %d.", oldrep, atoi(cRep));
			wsprintf(notice, "REP Count has been changed for player %s from %d to %d.", this->m_cCharName, oldrep, atoi(cRep));
		}
	}
	delete pStrTok;
}

void CClient::RequestBallItem(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * cp, * token, cBuff[256], cWantedItemName[256], cData[256], cInfoString[56];
	class CStrTok * pStrTok;
	class CItem * pItem;
	short * sp, sCounter;
	int i;
	int iEraseReq;
	uint32_t * dwp;
	uint16_t * wp;
	if ((dwMsgSize) <= 0) return;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cWantedItemName, 0, sizeof (cWantedItemName));
		strcpy(cWantedItemName, token);
	}
	sCounter = 1;
	for (i = 0; i < DEF_MAXBALLITEMS; i++) {
		if (sCounter == 1) {
			if (m_pBallItemConfigList[i] != nullptr) {
				if (strcmp(cWantedItemName, m_pBallItemConfigList[i]->m_cName) == 0) {
					if (this->m_iBallPoints < m_pBallItemConfigList[i]->m_iReqPoints) {
						wsprintf(cInfoString, "Item (%s) cannot be traded, not enough points. ", cWantedItemName);
						this->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
						return;
					}
					this->m_iBallPoints -= m_pBallItemConfigList[i]->m_iReqPoints;
					pItem = new class CItem;
					if (_bInitItemAttr(*pItem, cWantedItemName) == false) {
						delete pItem;
						return;
					}
					std::memset(cData, 0, sizeof (cData));
					if (this->_bAddClientItemList(pItem, &iEraseReq) == true) {
						dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
						*dwp = MSGID_NOTIFY;
						wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
						*wp = DEF_NOTIFY_ITEMOBTAINED;
						cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
						*cp = 1;
						cp++;
						memcpy(cp, pItem->m_cName, 20);
						cp += 20;
						dwp = (uint32_t *) cp;
						*dwp = pItem->m_dwCount;
						cp += 4;
						*cp = pItem->m_cItemType;
						cp++;
						*cp = pItem->m_cEquipPos;
						cp++;
						*cp = (char) 0;
						cp++;
						sp = (short *) cp;
						*sp = pItem->m_sLevelLimit;
						cp += 2;
						*cp = pItem->m_cGenderLimit;
						cp++;
						wp = (uint16_t *) cp;
						*wp = pItem->m_wCurLifeSpan;
						cp += 2;
						wp = (uint16_t *) cp;
						*wp = pItem->m_wWeight;
						cp += 2;
						sp = (short *) cp;
						*sp = pItem->m_sSprite;
						cp += 2;
						sp = (short *) cp;
						*sp = pItem->m_sSpriteFrame;
						cp += 2;
						*cp = pItem->m_cItemColor;
						cp++;
						*cp = (char) pItem->m_sItemSpecEffectValue2;
						cp++;
						dwp = (uint32_t *) cp;
						*dwp = pItem->m_dwAttribute;
						cp += 4;
						if (iEraseReq == 1) {
							delete pItem;
							pItem = nullptr;
						}
						this->m_pXSock->iSendMsg(cData, 53);
						sCounter = 0;
						return;
					} else {
						sCounter = 0;
						delete pItem;
						return;
					}
				}
			}
		}
	}
}

void CClient::_TradeItem(char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * cp, * token, cBuff[256], cItemName[256], cData[256];
	class CStrTok * pStrTok;
	class CItem * pItem;
	short * sp;
	int iEraseReq;
	uint32_t * dwp;
	uint16_t * wp;
	if (this->markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) { //Centuu : Comprueba el nombre del item se?alado con el ID correspondiente.
		std::memset(cItemName, 0, sizeof (cItemName));
		strcpy(cItemName, token);
	}
	if (this->_iGetItemSpaceLeft() == 0) { //Centuu : Dice, que si no tiene espacio en el bag, no le d? el item.
		this->SendItemNotifyMsg(DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	pItem = new class CItem;
	if (_bInitItemAttr(*pItem, cItemName) == false) { //Centuu : No acepta cualquier item.
		delete pItem;
		return;
	}
	switch (pItem->m_sIDnum) { //Centuu : Comprueba el ID correspondiente con el nombre del item se?alado.
		case 952: //Armor Dyes.
		case 953: //Armor Dyes.
		case 954: //Armor Dyes.
		case 955: //Armor Dyes.
		case 956: //Armor Dyes.
		case 957: //Armor Dyes.
		case 958: //Armor Dyes.
		case 959: //Armor Dyes
		case 960: //Armor Dyes.
		case 961: //Armor Dyes.
		case 962: //Armor Dyes.
		case 963: //Armor Dyes.
			if (this->m_iBallPoints < 20) return;
			this->m_iBallPoints -= 20;
			break;
		case 621: //Merien Plates
		case 622:
			if (this->m_iBallPoints < 850) return;
			this->m_iBallPoints -= 850;
			break;
		case 977: //Merien Chains
		case 978:
			if (this->m_iBallPoints < 820) return;
			this->m_iBallPoints -= 820;
			break;
		case 650: //Zem
			if (this->m_iBallPoints < 10) return;
			this->m_iBallPoints -= 10;
			break;
		case 847: //Dark Executor.
			if (this->m_iBallPoints < 1100) return;
			this->m_iBallPoints -= 1100;
			break;
		case 858: //Neck Of Merien
		case 859: //Neck Of Kloness
		case 860: //Neck Of Xelima
			if (this->m_iBallPoints < 700) return;
			this->m_iBallPoints -= 700;
			break;
		case 630: //Ring of xelima
		case 631: //Ring of Abaddon
			if (this->m_iBallPoints < 560) return;
			this->m_iBallPoints -= 560;
		case 618: //Dark Elf Bow
			if (this->m_iBallPoints < 750) return;
			this->m_iBallPoints -= 750;
			break;
		case 620: //Merien Shield
			if (this->m_iBallPoints < 850) return;
			this->m_iBallPoints -= 850;
			break;
		case 979: //Sex Change Potion
			if (this->m_iBallPoints < 35) return;
			this->m_iBallPoints -= 35;
			break;
		case 970: //Xelima Esterk
		case 973: //Xelima B Hammer
		case 976: //Xelima Bow
			if (this->m_iBallPoints < 500) return;
			this->m_iBallPoints -= 500;
			break;
		default:
			return;
			break;
	}
	std::memset(cData, 0, sizeof (cData));
	if (this->_bAddClientItemList(pItem, &iEraseReq) == true) { //Centuu : Dice que obtuvo el item.
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 1;
		cp++;
		memcpy(cp, pItem->m_cName, 20);
		cp += 20;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwCount;
		cp += 4;
		*cp = pItem->m_cItemType;
		cp++;
		*cp = pItem->m_cEquipPos;
		cp++;
		*cp = (char) 0;
		cp++;
		sp = (short *) cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;
		*cp = pItem->m_cGenderLimit;
		cp++;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;
		wp = (uint16_t *) cp;
		*wp = pItem->m_wWeight;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSprite;
		cp += 2;
		sp = (short *) cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;
		*cp = pItem->m_cItemColor;
		cp++;
		*cp = (char) pItem->m_sItemSpecEffectValue2;
		cp++;
		dwp = (uint32_t *) cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;
		if (iEraseReq == 1) {
			delete pItem;
			pItem = nullptr;
		}
		//Centuu : Dice que le quit? los EK's
		this->SendNotifyMsg(0, DEF_NOTIFY_BALLPOINTS, this->m_iBallPoints, 0, 0, nullptr);
		this->m_pXSock->iSendMsg(cData, 53);
		return;
	} else {
		delete pItem;
		return;
	}
}

void CClient::DesbugCharacter(char * pData, uint32_t dwMsgSize) {
	CStrTok * pStrTok;
	char cBuff[256], cName[11], cTargetName[11], cFileNamex[255], cFileName[255], cDir[63];
	const char * token;
	char seps[] = "= \t\n";
	std::memset(cTargetName, 0, sizeof (cTargetName));
	if ((dwMsgSize) <= 0) return;
	if (this->m_iAdminUserLevel < 2) {
		this->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, token);
	} else {
		std::memset(cName, 0, sizeof (cName));
		strcpy(cName, "null");
	}
	if (token == nullptr) {
		token = "null";
	}
	if (cName != nullptr) {
		token = cName;
		if (strlen(token) > 10)
			memcpy(cTargetName, token, 10);
		else memcpy(cTargetName, token, strlen(token));
		std::memset(cFileName, 0, sizeof (cFileName));
		std::memset(cDir, 0, sizeof (cDir));
		wsprintf(cDir, "..\\..\\Character\\AscII%d\\%s.bak", *cTargetName, cTargetName);
		strcat(cFileName, cDir);
		std::memset(cFileNamex, 0, sizeof (cFileNamex));
		std::memset(cDir, 0, sizeof (cDir));
		wsprintf(cDir, "..\\..\\Character\\AscII%d\\%s.txt", *cTargetName, cTargetName);
		strcat(cFileNamex, cDir);
		DeleteFile(cFileNamex);
		CopyFile(cFileName, cFileNamex, false);
		delete pStrTok;
		return;
	}
}

void CClient::ManualStartHappyHour(char *pData, uint32_t dwMsgSize) {
	char cBuff[256], * token, seps[] = "= \t\n";
	class CStrTok * pStrTok;
	int i;
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		for (i = 1; i < DEF_MAXCLIENTS; i++) // Check all clients
		{
			if ((m_pClientList[i] != nullptr)) {
				if (token[0] == '1') {
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HAPPYHOURSTAR, 0, 0, 0, nullptr);
					m_bHappyHour = true;
				} else if (token[0] == '0') {
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HAPPYHOUREND, 0, 0, 0, nullptr);
					m_bHappyHour = false;
				}
			}
		}
	}
	delete pStrTok;
	wsprintf(G_cTxt, "Admin Order(%s): begin Happy-Hour", this->m_cCharName);
	PutAdminLogFileList(G_cTxt);
	PutLogList(G_cTxt);
	PutLogEventFileList(G_cTxt);
}

void CClient::RequestCheckFlag() {
	if (!m_bIsCTFMode) return;
	if (this->m_cSide == 0) this->RequestTeleportHandler("2   ", "default", -1, -1);
	if ((this->m_sX == 151) && (this->m_sY == 132))//Elvine Flag Cords
	{
		if (this->m_iLevel >= 120) {
			if (this->m_cSide == 1) //Aresden Side
			{
				if ((!m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured)) //Aresden Capture Elvine Flag
				{
					PutLogList("Capture The Flag: Aresden Captured Elvine Flag!");
					this->SetFlagCarrierFlag(true);
				}
			} else //Elvine Side
			{
				if ((m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured)) {
					PutLogList("Capture The Flag: Elvine wins this round!");
					m_sElvineFlagCount++;
					m_bIsAresdenFlagCaptured = false;
					for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ELVINEWINSROUND, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
					if (m_sElvineFlagCount == 5) {
						EndCaptureTheFlag(2);
					}
				}
			}
		} else this->RequestTeleportHandler("2   ", "elvine", -1, -1);
	} else if ((this->m_sX == 151) && (this->m_sY == 128))//Aresden Flag Cords
	{
		if (this->m_iLevel >= 120) {
			if (this->m_cSide == 2) //Elvine Side
			{
				if ((!m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured)) //Elvine Capture Aresden Flag
				{
					PutLogList("Capture The Flag: Elvine Captured Aresden Flag!");
					this->SetFlagCarrierFlag(true);
				}
			} else //Aresden Side
			{
				if ((m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured)) {
					PutLogList("Capture The Flag: Aresden wins this round!");
					m_sAresdenFlagCount++;
					m_bIsElvineFlagCaptured = false;
					for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ARESDENWINSROUND, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
					if (m_sAresdenFlagCount == 5) {
						EndCaptureTheFlag(1);
					}
				}
			}
		} else this->RequestTeleportHandler("2   ", "aresden", -1, -1);
	}
}

bool CClient::bCheckIfIsFlagCarrier() {
	if (!m_bIsCTFMode) return false;
	if (m_iFlagCarrierIndex == -1) return false;
	if (this->markedForDeletion_) return false;
	if (iClientH == m_iFlagCarrierIndex) return true;
	else return false;
}

void CClient::SetFlagCarrierFlag(bool bFlagMode) {
	if (!m_bIsCTFMode) return;
	if (bFlagMode) {
		PutLogList("Capture The Flag: New Flag Carrier!");
		m_iFlagCarrierIndex = iClientH;
		if (this->m_cSide == 1) {
			m_bIsElvineFlagCaptured = true;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ARESDENCAPTUREDELVINEFLAG, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		} else {
			m_bIsAresdenFlagCaptured = true;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ELVINEFLAGBACKTOCH, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		}
		this->m_iEnemyKillCount += 20;
		this->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, nullptr);
		if (this->m_cSide == 1) this->m_iStatus = this->m_iStatus | 0x80000; //Aresden Flag
		else if (this->m_cSide == 2) this->m_iStatus = this->m_iStatus | 0x40000; //Elvine Flag
	} else {
		PutLogList("Capture The Flag: Flag Carrier Reset.");
		m_iFlagCarrierIndex = -1;
		if (this->m_cSide == 1) {
			m_bIsElvineFlagCaptured = false;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ELVINECAPTUREDARESDENFLAG, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		} else {
			m_bIsAresdenFlagCaptured = false;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ARESDENFLAGBACKTOCH, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		}
		this->m_iStatus = this->m_iStatus | 0x00001000;
	}
}

void CClient::CheckDenialServiceAttack(uint32_t dwClientTime) {
	uint32_t dwTime = timeGetTime();
	// Find a Denial of service attack by packet sent time
	if (this->m_dwDSLAT == 0)
		// Start with 1st msg
	{
		this->m_dwDSLAT = dwClientTime;
		this->m_dwDSLATOld = dwClientTime;
		this->m_dwDSLATS = dwTime;
		this->m_iDSCount = 0;
	} else {
		if (dwClientTime >= this->m_dwDSLAT)
			// current message was sent later than previous (normal case)
		{
			this->m_dwDSLAT = dwClientTime;
		} else
			// current message was sent before previous
		{
			if (this->m_dwDSLATOld == dwClientTime)
				// If we receive more late msg with same time
			{
				this->m_iDSCount++;
				if (((dwTime - this->m_dwDSLATS) > 10 * 1000)
						  && (this->m_iDSCount > 5))
					// Receiving a "late" msg more than 10 sec after !
					// This is an attack!
				{
					wsprintf(G_cTxt, "DS check: PC(%s) - Denial of service attack! (Disc.) \tIP(%s)"
							  , this->m_cCharName
							  , this->m_cIPaddress);
					PutHackLogFileList(G_cTxt);
					PutLogList(G_cTxt);
					return;
				}
			} else
				// else this message become late msg
			{
				this->m_dwDSLATOld = dwClientTime;
				this->m_iDSCount = 1;
				this->m_dwDSLATS = dwTime;
			}
		}
	}
}

void CClient::PlayerOrder_GetSkills() {
	int i;
	for (i = 0; i < 23; i++) {
		this->m_cSkillMastery[i] = 100;
		this->SendNotifyMsg(0, DEF_NOTIFY_SKILL, i, this->m_cSkillMastery[i], 0, nullptr);
	}
	this->ShowClientMsg("Enjoy your new skills!");
}

void CClient::RemoveFromTarget() {
	game_.RemoveFromTarget(id_, DEF_OWNERTYPE_PLAYER);
}

void CClient::SetAngelFlag(int iStatus, int iTemp) {
	game_.SetAngelFlag(id_, DEF_OWNERTYPE_PLAYER, iStatus, iTemp);
}

void CClient::SetIceFlag(bool bStatus) {
	game_.SetIceFlag(id_, DEF_OWNERTYPE_PLAYER, bStatus);
}
void CClient::SetHeroFlag(bool bStatus) {
	game_.SetHeroFlag(id_, DEF_OWNERTYPE_PLAYER, bStatus);
}
void CClient::SetInvisibilityFlag(bool bStatus) {
	game_.SetInvisibilityFlag(id_, DEF_OWNERTYPE_PLAYER, bStatus);
}