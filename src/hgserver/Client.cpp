#include "Client.h"
#include "Game.h"
#include <vector>
#include <algorithm>
#include "WINMAIN.H"
#include <cstring>
extern char G_cTxt[512];

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
	std::memset(m_cWhisperPlayerName, 0, sizeof (m_cWhisperPlayerName));
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
	m_iExchangeH = 0;
	std::memset(m_cExchangeName, 0, sizeof (m_cExchangeName));
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
	std::memset(m_cLockedMapName, 0, sizeof (m_cLockedMapName));
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
	int i, iExH;
	bool bIsSAattacked = false;
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	
	
	if (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cName, "fight", 5) == 0) {
		this->m_dwFightzoneDeadTime = timeGetTime();
		wsprintf(G_cTxt, "Fightzone Dead Time: %d", this->m_dwFightzoneDeadTime);
		PutLogList(G_cTxt);
	}
	this->m_bIsKilled = true;
	
	this->m_iHP = 0;
	
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
						
						game_.EnemyKillRewardHandler(iAttackerH, id_);
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
						
						game_.EnemyKillRewardHandler(iAttackerH, id_);
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
		
		if (game_.m_pNpcList[iAttackerH]->m_iGuildGUID != 0) {
			if (game_.m_pNpcList[iAttackerH]->m_cSide != this->m_cSide) {
				for(auto &iterClient: game_.m_pClientList) {
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
	wsprintf(cEKMsg, msgs[iDice(1, 10)-1], cAttackerName, this->m_cCharName);
	for(auto &iterClient: game_.m_pClientList) {
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
			*wp = (uint16_t) game_.m_iTotalClients; //_iGetTotalClients();
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
	int iAfterItemID, iRequiredEnemyKills, iRequiredContribution, iStoneNumber, i;
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
	int i, iItemX, iItemY, iSoM, iSoX, iSomH, iSoxH, iCheckHeroCape; // v2.172
	uint32_t dwTemp, dwSWEType, iValue;
	double dV1, dV2, dV3;
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
						if (game_.bCheckIsItemUpgradeSuccess(this->id_, iItemIndex, iSoxH) == false) {
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
				if (game_.bCheckIsItemUpgradeSuccess(this->id_, iItemIndex, iSoxH) == false) {
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
				if (game_.bCheckIsItemUpgradeSuccess(this->id_, iItemIndex, iSomH, true) == false) {
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
						if (game_.bCheckIsItemUpgradeSuccess(this->id_, iItemIndex, iSomH, true) == false) {
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
						if (game_.bCheckIsItemUpgradeSuccess(this->id_, iItemIndex, iSoxH) == false) {
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
	game_.ReleaseItemHandler(id_, sItemIndex, true);
	this->SendNotifyMsg(0, DEF_NOTIFY_ITEMDEPLETED_ERASEITEM, sItemIndex, (int) bIsUseItemResult, 0, nullptr);
	itemPtr.reset();
	this->m_bIsItemEquipped[sItemIndex] = false;
	this->m_cArrowIndex = game_._iGetArrowItemIndex(id_);
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
			iWeight += game_.iGetItemWeight(*this->m_pItemList[i], this->m_pItemList[i]->m_dwCount);
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
	int iOldSSN, iSSNpoint, iWeaponIndex;
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
			
			game_.bCheckTotalSkillMasteryPoints(id_, sSkillIndex);
			this->SendNotifyMsg(0, DEF_NOTIFY_SKILL, sSkillIndex, this->m_cSkillMastery[sSkillIndex], 0, nullptr);
		}
	}
}

void CClient::CalculateSSN_SkillIndex(short sSkillIndex, int iValue) {
	int iOldSSN, iSSNpoint, iWeaponIndex;
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
			game_.bCheckTotalSkillMasteryPoints(id_, sSkillIndex);
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
	int iReserveTime, iFightzoneTN, iFightzoneN;
	char cTemp[21];
	SYSTEMTIME SysTime;
	if (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cName, "fightzone", 9) == 0) {
		iReserveTime = this->m_iReserveTime;
		GetLocalTime(&SysTime);
		this->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + (SysTime.wHour + 3);
		std::memset(cTemp, 0, sizeof (cTemp));
		strcpy(cTemp, (game_.m_pMapList[this->m_cMapIndex]->m_cName + 9));
		iFightzoneN = this->m_iFightzoneNumber;
		iFightzoneTN = this->m_iFightZoneTicketNumber;
		this->m_iFightZoneTicketNumber = 10;
		this->m_iFightzoneNumber = atoi(cTemp);
		game_.GetFightzoneTicketHandler(id_);
		game_.GetFightzoneTicketHandler(id_);
		game_.GetFightzoneTicketHandler(id_);
		this->m_iFightzoneNumber = iFightzoneN;
		this->m_iFightZoneTicketNumber = iFightzoneTN;
		this->m_iReserveTime = iReserveTime;
	} else {
		iReserveTime = this->m_iReserveTime;
		GetLocalTime(&SysTime);
		this->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + (SysTime.wHour + 2);
		std::memset(cTemp, 0, sizeof (cTemp));
		strcpy(cTemp, (game_.m_pMapList[this->m_cMapIndex]->m_cName + 9));
		iFightzoneN = this->m_iFightzoneNumber;
		iFightzoneTN = this->m_iFightZoneTicketNumber;
		this->m_iFightZoneTicketNumber = 10;
		this->m_iFightzoneNumber = 1;
		game_.GetFightzoneTicketHandler(id_);
		game_.GetFightzoneTicketHandler(id_);
		game_.GetFightzoneTicketHandler(id_);
		this->m_iFightzoneNumber = iFightzoneN;
		this->m_iFightZoneTicketNumber = iFightzoneTN;
		this->m_iReserveTime = iReserveTime;
	}
}

void CClient::GetAngelHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char *cp, cData[256], cTmpName[21];
	int iAngel;
	int iRet, iEraseReq, iRequiredMagesty;
	char cItemName[21];
	short * sp;
	uint16_t * wp;
	int * ip;
	uint32_t * dwp;
	if (this->m_bIsInitComplete == false) return;
	if (game_._iGetItemSpaceLeft(this->id_) == 0) {
		game_.SendItemNotifyMsg(this->id_, DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, cp);
	cp += 20;
	ip = (int *) cp;
	iAngel = (int) *ip;
	cp += 2;
	wsprintf(G_cTxt, "PC(%s) obtained an Angel (%d).   %s(%d %d)", this->m_cCharName, iAngel, this->m_cMapName, this->m_sX, this->m_sY);
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
		if (game_._bAddClientItemList(this->id_, pItem, &iEraseReq)) {
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
			game_.m_pMapList[this->m_cMapIndex]->bSetItem(this->m_sX, this->m_sY, pItem);
			game_.SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, this->m_cMapIndex, this->m_sX, this->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
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
			game_.PlayerOrder_GetCrits(*this);
			break;
		case DEF_REQUEST_SHOWBALLPOINTS:
			this->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, nullptr);
			break;
		case DEF_REQUEST_ANGEL:
			this->GetAngelHandler(pData, dwMsgSize);
			break;
		case MSGID_REQUEST_SELLITEMLIST:
			game_.RequestSellItemListHandler(*this, pData);
			break;
		case MSGID_REQUEST_RESTART:
			game_.RequestRestartHandler(*this);
			this->SendNotifyMsg(0, DEF_NOTIFY_PARTYMEMBERSTATUS, 0, 0, 0, nullptr);
			break;
		case MSGID_REQUEST_PANNING:
			game_.iRequestPanningMapDataRequest(*this, pData);
			break;
		case MSGID_REQUEST_NOTICEMENT:
			game_.RequestNoticementHandler(*this, pData);
			break;
		case MSGID_BWM_COMMAND_SHUTUP:
			game_._BWM_Command_Shutup(pData);
			break;
		case MSGID_BWM_INIT:
			game_._BWM_Init(*this, pData);
			break;
		case MSGID_REQUEST_SETITEMPOS:
			game_._SetItemPos(*this, pData);
			break;
		case MSGID_REQUEST_FULLOBJECTDATA:
			game_.RequestFullObjectData(*this, pData);
			break;
		case MSGID_REQUEST_RETRIEVEITEM:
			game_.RequestRetrieveItemHandler(*this, pData);
			break;
		case MSGID_REQUEST_CIVILRIGHT:
			game_.RequestCivilRightHandler(*this, pData);
			break;
		case MSGID_REQUEST_TELEPORT:
			this->RequestTeleportHandler("1");
			break;
		case MSGID_REQUEST_INITPLAYER:
			game_.RequestInitPlayerHandler(*this, pData, cKey);
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
				game_.m_pMapList[this->m_cMapIndex]->ClearOwner(2, id_, DEF_OWNERTYPE_PLAYER, this->m_sX, this->m_sY);
				game_.delayEvents_.remove(id_, DEF_OWNERTYPE_PLAYER, 0);
				game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, id_, false);
				if ((dwTime - game_.m_dwGameTime2) > 3000) {
					this->m_bIsClientConnected = false;
					game_.m_iTotalClients--;
					this->m_bMarkedForDeletion = true;
				}
				break;
			} else {
				this->m_bIsClientConnected = true;
				game_.RequestInitDataHandler(id_, pData, cKey);
			}
		}
			break;
		case MSGID_COMMAND_COMMON:
			ClientCommonHandler(pData);
			break;
		case MSGID_COMMAND_MOTION:
			game_.ClientMotionHandler(*this, pData);
			break;
		case MSGID_COMMAND_CHECKCONNECTION:
			game_.CheckConnectionHandler(id_, pData);
			break;
		case MSGID_COMMAND_CHATMSG:
			game_.ChatMsgHandler(*this, pData, dwMsgSize);
			break;
		case MSGID_REQUEST_CREATENEWGUILD:
			game_.RequestCreateNewGuildHandler(id_, pData, dwMsgSize);
			break;
		case MSGID_REQUEST_DISBANDGUILD:
			game_.RequestDisbandGuildHandler(id_, pData, dwMsgSize);
			break;
		case MSGID_REQUEST_FIGHTZONE_RESERVE:
			game_.FightzoneReserveHandler(id_, pData, dwMsgSize);
			break;
		case MSGID_LEVELUPSETTINGS:
			game_.LevelUpSettingsHandler(id_, pData, dwMsgSize);
			break;
		case MSGID_STATECHANGEPOINT:
			game_.StateChangeHandler(id_, pData, dwMsgSize);
			break;
		case MSGID_REQUEST_HELDENIANTELEPORT:
			game_.RequestHeldenianTeleport(id_, pData, dwMsgSize);
			break;
		case MSGID_REQUEST_NPCSPEAKTELEPORT:
			game_.RequestNpcSpeakTeleport(id_, pData, dwMsgSize);
			break;
		case MSGID_REQUEST_HELDENIAN_SCROLL:
			game_.RequestHeldenianScroll(id_, pData, dwMsgSize);
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
	this->RequestTeleportHandler("2   ", this->m_cMapName, this->m_sX, this->m_sY);
}

void CClient::RequestTeleportHandler(const char * pData, const char * cMapName, int dX, int dY) {
	char * pBuffer, cTempMapName[21];
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cDestMapName[11], cDir, cMapIndex, cQuestRemain;
	short * sp, sX, sY, sSummonPoints;
	int * ip, i, iRet, iSize, iDestX, iDestY, iExH, iMapSide;
	bool bRet, bIsLockedMapNotify;
	SYSTEMTIME SysTime;
	this->m_dwLastActionTime = this->m_dwAFKCheckTime = timeGetTime();
	if (this->m_bIsInitComplete == false) return;
	if (this->m_bIsKilled == true) return;
	if (this->m_bIsOnWaitingProcess == true) return;
	if ((game_.m_pMapList[this->m_cMapIndex]->m_bIsRecallImpossible == true) && (this->m_iAdminUserLevel == 0) &&
			  (this->m_bIsKilled == false) && (this->m_iHP > 0)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_NORECALL, 0, 0, 0, nullptr);
		return;
	}
	if ((memcmp(this->m_cLocation, "elvine", 6) == 0)
			  && (this->m_iTimeLeft_ForceRecall > 0)
			  && (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)
			  && ((pData[0] == '1'))
			  && (this->m_iAdminUserLevel == 0)
			  && (game_.m_bIsCrusadeMode == false)) return;
	if ((memcmp(this->m_cLocation, "aresden", 7) == 0)
			  && (this->m_iTimeLeft_ForceRecall > 0)
			  && (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)
			  && ((pData[0] == '1'))
			  && (this->m_iAdminUserLevel == 0)
			  && (game_.m_bIsCrusadeMode == false)) return;
	bIsLockedMapNotify = false;
	if (this->m_bIsExchangeMode == true) {
		iExH = this->m_iExchangeH;
		game_._ClearExchangeStatus(iExH);
		game_._ClearExchangeStatus(id_);
	}
	if ((memcmp(this->m_cLocation, "NONE", 4) == 0) && (pData[0] == '1'))
		return;
	game_.RemoveFromTarget(id_, DEF_OWNERTYPE_PLAYER);
	game_.m_pMapList[this->m_cMapIndex]->ClearOwner(13, id_, DEF_OWNERTYPE_PLAYER,
			  this->m_sX,
			  this->m_sY);
	game_.SendEventToNearClient_TypeA(id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
	sX = this->m_sX;
	sY = this->m_sY;
	std::memset(cDestMapName, 0, sizeof (cDestMapName));
	bRet = game_.m_pMapList[this->m_cMapIndex]->bSearchTeleportDest(sX, sY, cDestMapName, &iDestX, &iDestY, &cDir);
	// Crusade
	if ((strcmp(this->m_cLockedMapName, "NONE") != 0) && (this->m_iLockedMapTime > 0)) {
		iMapSide = game_.iGetMapLocationSide(cDestMapName);
		if (iMapSide > 3) iMapSide -= 2; // New 18/05/2004
		if ((iMapSide != 0) && (this->m_cSide == iMapSide)) {
		} else {
			iDestX = -1;
			iDestY = -1;
			bIsLockedMapNotify = true;
			std::memset(cDestMapName, 0, sizeof (cDestMapName));
			strcpy(cDestMapName, this->m_cLockedMapName);
		}
	}
	if ((bRet == true) && (cMapName == nullptr)) {
		for (i = 0; i < DEF_MAXMAPS; i++)
			if (game_.m_pMapList[i] != nullptr) {
				if (memcmp(game_.m_pMapList[i]->m_cName, cDestMapName, 10) == 0) {
					if (pData[0] == '3' && this->m_iAdminUserLevel == 0 && (this->m_iTimeLeft_ForceRecall > 0)) {
						std::vector<std::string> whitelist;
						if ((memcmp(this->m_cLocation, "elvine", 6) == 0)
								  && (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)) {
							// Elv in ares
							whitelist = {"middleland", "huntzone2", "aresdend1", "arefarm"};
						}
						if ((memcmp(this->m_cLocation, "aresden", 7) == 0)
								  && (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)) {
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
					this->m_cMapIndex = i;
					std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
					memcpy(this->m_cMapName, game_.m_pMapList[i]->m_cName, 10);
					goto RTH_NEXTSTEP;
				}
			}
		this->m_sX = iDestX;
		this->m_sY = iDestY;
		this->m_cDir = cDir;
		std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
		memcpy(this->m_cMapName, cDestMapName, 10);
		// New 18/05/2004
		this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
				  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
		game_.SetSlateFlag(id_, DEF_NOTIFY_SLATECLEAR, false);
		game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, id_, false);
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
				if ((strcmp(this->m_cLockedMapName, "NONE") != 0) && (this->m_iLockedMapTime > 0)) {
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, this->m_cLockedMapName);
				}
				for (i = 0; i < DEF_MAXMAPS; i++)
					if (game_.m_pMapList[i] != nullptr) {
						if (memcmp(game_.m_pMapList[i]->m_cName, cTempMapName, 10) == 0) {
							game_.GetMapInitialPoint(i, &this->m_sX, &this->m_sY, this->m_cLocation);
							this->m_cMapIndex = i;
							std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
							memcpy(this->m_cMapName, cTempMapName, 10);
							goto RTH_NEXTSTEP;
						}
					}
				this->m_sX = -1;
				this->m_sY = -1;
				std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
				memcpy(this->m_cMapName, cTempMapName, 10);
				// New 18/05/2004
				this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
						  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
				game_.SetSlateFlag(id_, DEF_NOTIFY_SLATECLEAR, false);
				game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, id_, false);
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
				if ((strcmp(this->m_cLockedMapName, "NONE") != 0) && (this->m_iLockedMapTime > 0)) {
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, this->m_cLockedMapName);
				}
				for (i = 0; i < DEF_MAXMAPS; i++)
					if (game_.m_pMapList[i] != nullptr) {
						if (memcmp(game_.m_pMapList[i]->m_cName, cTempMapName, 10) == 0) {
							game_.GetMapInitialPoint(i, &this->m_sX, &this->m_sY, this->m_cLocation);
							this->m_cMapIndex = i;
							std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
							memcpy(this->m_cMapName, game_.m_pMapList[i]->m_cName, 10);
							goto RTH_NEXTSTEP;
						}
					}
				this->m_sX = -1;
				this->m_sY = -1;
				std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
				memcpy(this->m_cMapName, cTempMapName, 10);
				// New 18/05/2004
				this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
						  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
				game_.SetSlateFlag(id_, DEF_NOTIFY_SLATECLEAR, false);
				game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, id_, false);
				// !!!
				this->m_bIsOnServerChange = true;
				this->m_bIsOnWaitingProcess = true;
				return;
			case '2':
				// Crusade
				if ((strcmp(this->m_cLockedMapName, "NONE") != 0) && (this->m_iLockedMapTime > 0)) {
					dX = -1;
					dY = -1;
					bIsLockedMapNotify = true;
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, this->m_cLockedMapName);
				} else {
					std::memset(cTempMapName, 0, sizeof (cTempMapName));
					strcpy(cTempMapName, cMapName);
				}
				cMapIndex = game_.iGetMapIndex(cTempMapName);
				if (cMapIndex == -1) {
					this->m_sX = dX;
					this->m_sY = dY;
					std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
					memcpy(this->m_cMapName, cTempMapName, 10);
					// New 18/05/2004
					this->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
							  this->m_cMagicEffectStatus[ DEF_MAGICTYPE_CONFUSE ], 0, nullptr);
					game_.SetSlateFlag(id_, DEF_NOTIFY_SLATECLEAR, false);
					game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, id_, false);
					// !!!
					this->m_bIsOnServerChange = true;
					this->m_bIsOnWaitingProcess = true;
					return;
				}
				this->m_sX = dX;
				this->m_sY = dY;
				this->m_cMapIndex = cMapIndex;
				std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
				memcpy(this->m_cMapName, game_.m_pMapList[cMapIndex]->m_cName, 10);
				break;
		}
	}
RTH_NEXTSTEP:
	;
	// New 17/05/2004
	game_.SetPlayingStatus(id_);
	int iTemp, iTemp2;
	iTemp = this->m_iStatus;
	iTemp = 0x0FFFFFFF & iTemp;
	iTemp2 = game_.iGetPlayerABSStatus(id_);
	iTemp = iTemp | (iTemp2 << 28);
	this->m_iStatus = iTemp;
	// Crusade
	if (bIsLockedMapNotify == true) this->SendNotifyMsg(0, DEF_NOTIFY_LOCKEDMAP, this->m_iLockedMapTime, 0, 0, this->m_cLockedMapName);
	pBuffer = new char [DEF_MSGBUFFERSIZE + 1];
	ZeroMemory(pBuffer, DEF_MSGBUFFERSIZE + 1);
	dwp = (uint32_t *) (pBuffer + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_INITDATA;
	wp = (uint16_t *) (pBuffer + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	cp = (char *) (pBuffer + DEF_INDEX2_MSGTYPE + 2);
	if (this->m_bIsObserverMode == false)
		game_.bGetEmptyPosition(&this->m_sX, &this->m_sY, this->m_cMapIndex);
	else game_.GetMapInitialPoint(this->m_cMapIndex, &this->m_sX, &this->m_sY);
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
	memcpy(cp, this->m_cMapName, 10);
	cp += 10;
	memcpy(cp, game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, 10);
	cp += 10;
	if (game_.m_pMapList[this->m_cMapIndex]->m_bIsFixedDayMode == true)
		*cp = 1;
	else *cp = game_.m_cDayOrNight;
	cp++;
	if (game_.m_pMapList[this->m_cMapIndex]->m_bIsFixedDayMode == true)
		*cp = 0;
	else *cp = game_.m_pMapList[this->m_cMapIndex]->m_cWhetherStatus;
	cp++;
	// v1.4 Contribution
	ip = (int *) cp;
	*ip = this->m_iContribution;
	cp += 4;
	if (this->m_bIsObserverMode == false) {
		game_.m_pMapList[this->m_cMapIndex]->SetOwner(id_,
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
	iSize = game_.iComposeInitMapData(this->m_sX - 10, this->m_sY - 7, id_, cp);
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
	game_.SendEventToNearClient_TypeA(id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
	if ((memcmp(this->m_cLocation, "are", 3) == 0) &&
			  (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0) &&
			  (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;
		// New 17/05/2004
		game_.CheckForceRecallTime(id_);
	} else if ((memcmp(this->m_cLocation, "elv", 3) == 0) &&
			  (memcmp(game_.m_pMapList[this->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0) &&
			  (this->m_iAdminUserLevel == 0)) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;
		// New 17/05/2004
		game_.CheckForceRecallTime(id_);
	} else if (game_.m_pMapList[this->m_cMapIndex]->m_bIsFightZone == true) {
		this->m_dwWarBeginTime = timeGetTime();
		this->m_bIsWarLocation = true;
		game_.SetForceRecallTime(id_);
		GetLocalTime(&SysTime);
		this->m_iTimeLeft_ForceRecall = 2 * 20 * 60 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
	} else {
		this->m_bIsWarLocation = false;
		this->m_iTimeLeft_ForceRecall = 0;
		game_.SetForceRecallTime(id_);
	}
	// No entering enemy shops
	int iMapside, iMapside2;
	iMapside = game_.iGetMapLocationSide(game_.m_pMapList[this->m_cMapIndex]->m_cName);
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
		if (game_.m_pMapList[ this->m_cMapIndex ]->m_bIsFightZone == true &&
				  game_.m_iFightzoneNoForceRecall == false &&
				  this->m_iAdminUserLevel == 0) {
			this->m_dwWarBeginTime = timeGetTime();
			this->m_bIsWarLocation = true;
			GetLocalTime(&SysTime);
			this->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2)*20 * 60 + SysTime.wMinute * 20) - 2 * 20;
		} else {
			if (memcmp(game_.m_pMapList[ this->m_cMapIndex ]->m_cLocationName, "arejail", 7) == 0 ||
					  memcmp(game_.m_pMapList[ this->m_cMapIndex ]->m_cLocationName, "elvjail", 7) == 0) {
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
	game_._SendQuestContents(id_);
	game_._CheckQuestEnvironment(id_);
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
	if (memcmp(this->m_cMapName, "fight", 5) == 0) {
		wsprintf(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", this->m_cCharName, this->m_cMapName, this->m_bIsObserverMode);
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
		game_._bCheckIsQuestCompleted(id_);
	}
}

void CClient::CalcTotalItemEffect(int iEquipItemID, bool bNotify) {
	int i, iTemp;
	char cEquipPos;
	double dV1, dV2, dV3;
	uint32_t dwSWEType, dwSWEValue;
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
	game_.SetAngelFlag(id_, DEF_OWNERTYPE_PLAYER, 0, 0);
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
							game_.SetAngelFlag(id_, DEF_OWNERTYPE_PLAYER, 1, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
						case 17: // Angel DEX //AngelicPandent(DEX)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicDex = iTemp;
							game_.SetAngelFlag(id_, DEF_OWNERTYPE_PLAYER, 2, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
						case 18: // Angel INT//AngelicPandent(INT)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicInt = iTemp;
							game_.SetAngelFlag(id_, DEF_OWNERTYPE_PLAYER, 3, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
						case 19: // Angel MAG//AngelicPandent(MAG)
							iTemp = (item.m_dwAttribute & 0xF0000000) >> 28;
							this->m_iAngelicMag = iTemp;
							game_.SetAngelFlag(id_, DEF_OWNERTYPE_PLAYER, 4, iTemp);
							this->SendNotifyMsg(0, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, nullptr);
							break;
					}
					break;
				case DEF_ITEMEFFECTTYPE_ATTACK_ARROW:
					if ((this->m_cArrowIndex != -1) &&
							  (this->m_pItemList[this->m_cArrowIndex] == nullptr)) {
						this->m_cArrowIndex = game_._iGetArrowItemIndex(id_);
					} else if (this->m_cArrowIndex == -1)
						this->m_cArrowIndex = game_._iGetArrowItemIndex(id_);
					if (this->m_cArrowIndex == -1) {
						this->m_cAttackDiceThrow_SM = 0;
						this->m_cAttackDiceRange_SM = 0;
						this->m_cAttackBonus_SM = 0;
						this->m_cAttackDiceThrow_L = 0;
						this->m_cAttackDiceRange_L = 0;
						this->m_cAttackBonus_L = 0;
					} else {
						/*
						this->m_cAttackDiceThrow_SM = this->m_pItemList[iArrowIndex]->m_sItemEffectValue1;
						this->m_cAttackDiceRange_SM = this->m_pItemList[iArrowIndex]->m_sItemEffectValue2;
						this->m_cAttackBonus_SM     = this->m_pItemList[iArrowIndex]->m_sItemEffectValue3;
						this->m_cAttackDiceThrow_L  = this->m_pItemList[iArrowIndex]->m_sItemEffectValue4;
						this->m_cAttackDiceRange_L  = this->m_pItemList[iArrowIndex]->m_sItemEffectValue5;
						this->m_cAttackBonus_L      = this->m_pItemList[iArrowIndex]->m_sItemEffectValue6;
						 */
						this->m_cAttackDiceThrow_SM = item.m_sItemEffectValue1;
						this->m_cAttackDiceRange_SM = item.m_sItemEffectValue2;
						this->m_cAttackBonus_SM = item.m_sItemEffectValue3;
						this->m_cAttackDiceThrow_L = item.m_sItemEffectValue4;
						this->m_cAttackDiceRange_L = item.m_sItemEffectValue5;
						this->m_cAttackBonus_L = item.m_sItemEffectValue6;
					}
					this->m_iHitRatio += this->m_cSkillMastery[item.m_sRelatedSkill];
					// this->m_iHitRatio_ItemEffect_SM += item.m_sSM_HitRatio;
					// this->m_iHitRatio_ItemEffect_L  += item.m_sL_HitRatio;
					break;
				case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
				case DEF_ITEMEFFECTTYPE_DEFENSE:
					this->m_iDefenseRatio += item.m_sItemEffectValue1;
					//this->m_iHitRatio_ItemEffect_SM += item.m_sSM_HitRatio;
					//this->m_iHitRatio_ItemEffect_L  += item.m_sL_HitRatio;
					if ((item.m_dwAttribute & 0x00000001) != 0) {
						this->m_iCustomItemValue_Defense += item.m_sItemSpecEffectValue2;
						dV2 = (double) item.m_sItemSpecEffectValue2;
						dV3 = (double) item.m_sItemEffectValue1;
						dV1 = (double) (dV2 / 100.0f) * dV3;
						dV1 = dV1 / 2.0f;
						this->m_iDefenseRatio += (int) dV1;
						if (this->m_iDefenseRatio <= 0) this->m_iDefenseRatio = 1;
						//testcode
						//wsprintf(G_cTxt, "Custom-Defense: %d", (int)dV1);
						//PutLogList(G_cTxt);
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
								// v2.04
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
	//v1.432
	if ((iPrevSAType != 0) && (this->m_iSpecialAbilityType == 0) && (bNotify == true)) {
		this->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 4, 0, 0, nullptr);
		if (this->m_bIsSpecialAbilityEnabled == true) {
			this->m_bIsSpecialAbilityEnabled = false;
			this->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
			sTemp = this->m_sAppr4;
			sTemp = sTemp & 0xFF0F;
			this->m_sAppr4 = sTemp;
			game_.SendEventToNearClient_TypeA(id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			game_.SendEventToNearClient_TypeA(id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
	int iRet, i, j, iEraseReq, iRiskLevel, iDifficulty, iNeededContrib = 0;
	short * sp, sTemp;
	short sItemIndex[6], sItemPurity[6], sItemNumber[6], sItemArray[12];
	bool bDup, bFlag, bNeedLog;
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
			/*wsprintf(G_cTxt, "Crafting: %d x %s (%d)"
				, sItemNumber[i]
				, this->m_pItemList[sItemIndex[i]]->m_cName
				, this->m_pItemList[sItemIndex[i]]->m_sIDnum);
			PutLogList(G_cTxt);*/
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
	int iPurity, iTot = 0, iCount = 0;
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
					game_.SetItemCount(id_, sItemIndex[i],
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
			if (game_._bAddClientItemList(id_, pItem, &iEraseReq) == true) {
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
				game_.m_pMapList[this->m_cMapIndex]->bSetItem(this->m_sX,
						  this->m_sY, pItem);
				game_.SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, this->m_cMapIndex,
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
			game_.RequestChangePlayMode(id_);
			break;
			//
		case DEF_COMMONTYPE_SETGUILDTELEPORTLOC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDTELEPORTLOC");
			game_.RequestSetGuildTeleportLocHandler(id_, iV1, iV2, this->m_iGuildGUID, "middleland");
			break;
		case DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC");
			game_.RequestSetGuildConstructLocHandler(id_, iV1, iV2, this->m_iGuildGUID, pString);
			break;
		case DEF_COMMONTYPE_GUILDTELEPORT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GUILDTELEPORT");
			game_.RequestGuildTeleportHandler(id_);
			break;
		case DEF_COMMONTYPE_SUMMONWARUNIT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SUMMONWARUNIT");
			game_.RequestSummonWarUnitHandler(id_, sX, sY, iV1, iV2, iV3);
			break;
		case DEF_COMMONTYPE_REQUEST_HELP:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_HELP");
			game_.RequestHelpHandler(id_);
			break;
		case DEF_COMMONTYPE_REQUEST_MAPSTATUS:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_MAPSTATUS");
			game_.MapStatusHandler(id_, iV1, pString);
			break;
		case DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY");
			game_.SelectCrusadeDutyHandler(id_, iV1);
			break;
		case DEF_COMMONTYPE_REQUEST_CANCELQUEST:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_CANCELQUEST");
			game_.CancelQuestHandler(id_);
			break;
		case DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY");
			game_.ActivateSpecialAbilityHandler(id_);
			break;
		case DEF_COMMONTYPE_REQUEST_JOINPARTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_JOINPARTY");
			game_.JoinPartyHandler(id_, iV1, pString);
			break;
		case DEF_COMMONTYPE_GETMAGICABILITY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GETMAGICABILITY");
			game_.GetMagicAbilityHandler(id_);
			break;
		case DEF_COMMONTYPE_BUILDITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_BUILDITEM");
			game_.BuildItemHandler(id_, pData);
			break;
		case DEF_COMMONTYPE_QUESTACCEPTED:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_QUESTACCEPTED");
			game_.QuestAcceptedHandler(id_);
			break;
		case DEF_COMMONTYPE_CANCELEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CANCELEXCHANGEITEM");
			game_.CancelExchangeItem(id_);
			break;
		case DEF_COMMONTYPE_CONFIRMEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CONFIRMEXCHANGEITEM");
			game_.ConfirmExchangeItem(id_);
			break;
		case DEF_COMMONTYPE_SETEXCHANGEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETEXCHANGEITEM");
			game_.SetExchangeItem(id_, iV1, iV2);
			break;
		case DEF_COMMONTYPE_REQ_GETHEROMANTLE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETHEROMANTLE");
			game_.GetHeroMantleHandler(id_, iV1, pString);
			break;
		case DEF_COMMONTYPE_REQ_GETTRADEEK: // By Luqah
			game_.GetTradeEKMantleHandler(id_, iV1, pString);
			break;
		case DEF_COMMONTYPE_REQ_GETOCCUPYFLAG:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFLAG");
			game_.GetOccupyFlagHandler(id_);
			break;
		case DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX");
			game_.SetDownSkillIndexHandler(id_, iV1);
			break;
		case DEF_COMMONTYPE_TALKTONPC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TALKTONPC");
			// works for client, but for npc it returns middleland
			// if ((m_pMapList[m_pNpcList[iV1]->m_cMapIndex]->m_cName) != (m_pMapList[this->m_cMapIndex]->m_cName)) break;
			game_.NpcTalkHandler(id_, iV1);
			break;
		case DEF_COMMONTYPE_REQ_CREATEPORTION:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_CREATEPORTION");
			game_.ReqCreatePortionHandler(id_, pData);
			break;
		case DEF_COMMONTYPE_REQ_GETFISHTHISTIME:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETFISHTHISTIME");
			game_.ReqGetFishThisTimeHandler(id_);
			break;
		case DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM");
			game_.ReqRepairItemCofirmHandler(id_, iV1, pString);
			break;
		case DEF_COMMONTYPE_REQ_REPAIRITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEM");
			game_.ReqRepairItemHandler(id_, iV1, iV2, pString);
			break;
		case DEF_COMMONTYPE_REQ_SELLITEMCONFIRM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEMCONFIRM");
			game_.ReqSellItemConfirmHandler(id_, iV1, iV2, pString);
			break;
		case DEF_COMMONTYPE_REQ_SELLITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEM");
			game_.ReqSellItemHandler(id_, iV1, iV2, iV3, pString);
			break;
		case DEF_COMMONTYPE_REQ_USESKILL:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USESKILL");
			game_.UseSkillHandler(id_, iV1, iV2, iV3);
			break;
		case DEF_COMMONTYPE_REQ_USEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USEITEM");
			game_.UseItemHandler(*this, iV1, iV2, iV3, iV4);
			break;
		case DEF_COMMONTYPE_REQ_GETREWARDMONEY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETREWARDMONEY");
			game_.GetRewardMoneyHandler(id_);
			break;
		case DEF_COMMONTYPE_ITEMDROP:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_ITEMDROP");
			game_.DropItemHandler(id_, iV1, iV2, pString, true);
			break;
		case DEF_COMMONTYPE_EQUIPITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EQUIPITEM");
			game_.bEquipItemHandler(id_, iV1);
			// .....
			break;
		case DEF_COMMONTYPE_REQ_PURCHASEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_PURCHASEITEM");
			game_.RequestPurchaseItemHandler(id_, pString, iV1);
			break;
		case DEF_COMMONTYPE_REQ_STUDYMAGIC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_STUDYMAGIC");
			game_.RequestStudyMagicHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_REQ_TRAINSKILL:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_TRAINSKILL");
			//RequestTrainSkillHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_GIVEITEMTOCHAR:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GIVEITEMTOCHAR");
			game_.GiveItemHandler(id_, cDir, iV1, iV2, iV3, iV4, pString);
			break;
		case DEF_COMMONTYPE_EXCHANGEITEMTOCHAR:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EXCHANGEITEMTOCHAR");
			game_.ExchangeItemHandler(id_, cDir, iV1, iV2, iV3, iV4, pString);
			break;
		case DEF_COMMONTYPE_JOINGUILDAPPROVE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDAPPROVE");
			game_.JoinGuildApproveHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_JOINGUILDREJECT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDREJECT");
			game_.JoinGuildRejectHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDAPPROVE");
			game_.DismissGuildApproveHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_DISMISSGUILDREJECT:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDREJECT");
			game_.DismissGuildRejectHandler(id_, pString);
			break;
		case DEF_COMMONTYPE_RELEASEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_RELEASEITEM");
			game_.ReleaseItemHandler(id_, iV1, true);
			break;
		case DEF_COMMONTYPE_TOGGLECOMBATMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLECOMBATMODE");
			game_.ToggleCombatModeHandler(id_);
			break;
		case DEF_COMMONTYPE_MAGIC:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_MAGIC");
			game_.PlayerMagicHandler(id_, iV1, iV2, (iV3 - 100));
			break;
		case DEF_COMMONTYPE_TOGGLESAFEATTACKMODE:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLESAFEATTACKMODE");
			game_.ToggleSafeAttackModeHandler(id_);
			break;
		case DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET");
			game_.GetFightzoneTicketHandler(id_);
			break;
			// Upgrade Item
		case DEF_COMMONTYPE_UPGRADEITEM:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_UPGRADEITEM");
			this->RequestItemUpgradeHandler(iV1);
			break;
		case DEF_COMMONTYPE_REQGUILDNAME:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
			game_.RequestGuildNameHandler(id_, iV1, iV2);
			break;
		case DEF_COMMONTYPE_REQRANGO: // MORLA 2.2 - Llama al rango del pj
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
			game_.RequestRango(id_, iV1);
			break;
		case DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY:
			//DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY");
			game_.RequestAcceptJoinPartyHandler(id_, iV1);
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
	int i, iRet;
	short *sp;
	char cItemID[4], ctr[4];
	char *cp, cSlateColour, cData[120];
	bool bIsSlatePresent = false;
	CItem * pItem;
	int iSlateType, iEraseReq;
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
		if (game_._bAddClientItemList(id_, pItem, &iEraseReq) == true) {
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
			game_.m_pMapList[this->m_cMapIndex]->bSetItem(this->m_sX, this->m_sY, pItem);
			game_.SendEventToNearClient_TypeB(MSGID_MAGICCONFIGURATIONCONTENTS, DEF_COMMONTYPE_ITEMDROP, this->m_cMapIndex,
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
	int i, iExH;
	char cTmpMap[30];
	if (this->m_bIsInitComplete == true) {
		if (memcmp(this->m_cMapName, "fight", 5) == 0) {
			wsprintf(G_cTxt, "Char(%s)-Exit(%s)", this->m_cCharName, this->m_cMapName);
			PutLogEventFileList(G_cTxt);
		}
		if (this->m_bIsExchangeMode == true) {
			iExH = this->m_iExchangeH;
			game_._ClearExchangeStatus(iExH);
			game_._ClearExchangeStatus(id_);
		}
		if ((this->m_iAllocatedFish != 0) && (game_.m_pFish[this->m_iAllocatedFish] != nullptr))
			game_.m_pFish[this->m_iAllocatedFish]->m_sEngagingCount--;
		if (bNotify == true)
			game_.SendEventToNearClient_TypeA(id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
		game_.RemoveFromTarget(id_, DEF_OWNERTYPE_PLAYER);
		for(auto &iterClient: game_.m_pClientList) {
			if((iterClient.m_iWhisperPlayerIndex == id_)) {
				iterClient.m_iWhisperPlayerIndex = -1;
				iterClient.SendNotifyMsg(0, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, this->m_cCharName);
			}
		}
		game_.m_pMapList[this->m_cMapIndex]->ClearOwner(2, id_, DEF_OWNERTYPE_PLAYER,
				  this->m_sX,
				  this->m_sY);
		game_.delayEvents_.remove(id_, DEF_OWNERTYPE_PLAYER, 0);
	}
	if ((bSave == true) && (this->m_bIsOnServerChange == false)) {
		if (this->m_bIsKilled == true) {
			this->m_sX = -1;
			this->m_sY = -1;
			strcpy(cTmpMap, this->m_cMapName);
			std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
			if (this->m_cSide == 0) {
				strcpy(this->m_cMapName, "default");
			} else {
				if (memcmp(this->m_cLocation, "are", 3) == 0) {
					if (game_.m_bIsCrusadeMode == true) {
						if (this->m_iDeadPenaltyTime > 0) {
							std::memset(this->m_cLockedMapName, 0, sizeof (this->m_cLockedMapName));
							strcpy(this->m_cLockedMapName, "aresden");
							this->m_iLockedMapTime = 60 * 5;
							this->m_iDeadPenaltyTime = 60 * 10;
						} else {
							this->m_iDeadPenaltyTime = 60 * 10;
						}
					}
					if (strcmp(cTmpMap, "elvine") == 0) {
						strcpy(this->m_cLockedMapName, "elvjail");
						this->m_iLockedMapTime = 10 * 2;
						memcpy(this->m_cMapName, "elvjail", 7);
					} else if (this->m_iLevel > 80)
						memcpy(this->m_cMapName, "resurr1", 7);
					else memcpy(this->m_cMapName, "resurr1", 7);
				} else {
					if (game_.m_bIsCrusadeMode == true) {
						if (this->m_iDeadPenaltyTime > 0) {
							std::memset(this->m_cLockedMapName, 0, sizeof (this->m_cLockedMapName));
							strcpy(this->m_cLockedMapName, "elvine");
							this->m_iLockedMapTime = 60 * 5;
							this->m_iDeadPenaltyTime = 60 * 10;
						} else {
							this->m_iDeadPenaltyTime = 60 * 10;
						}
					}
					if (strcmp(cTmpMap, "aresden") == 0) {
						strcpy(this->m_cLockedMapName, "arejail");
						this->m_iLockedMapTime = 10 * 2;
						memcpy(this->m_cMapName, "arejail", 7);
					} else if (this->m_iLevel > 80)
						memcpy(this->m_cMapName, "resurr2", 7);
					else memcpy(this->m_cMapName, "resurr2", 7);
				}
			}
		} else if (bForceCloseConn == true) {
			std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
			memcpy(this->m_cMapName, "bisle", 5);
			this->m_sX = -1;
			this->m_sY = -1;
			std::memset(this->m_cLockedMapName, 0, sizeof (this->m_cLockedMapName));
			strcpy(this->m_cLockedMapName, "bisle");
			this->m_iLockedMapTime = 10 * 60;
		}
		if (this->m_bIsObserverMode == true) {
			std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
			if (this->m_cSide == 0) {
				switch (iDice(1, 2)) {
					case 1:
						memcpy(this->m_cMapName, "aresden", 7);
						break;
					case 2:
						memcpy(this->m_cMapName, "elvine", 6);
						break;
				}
			} else {
				memcpy(this->m_cMapName, this->m_cLocation, 10);
			}
			this->m_sX = -1;
			this->m_sY = -1;
		}
		if (memcmp(this->m_cMapName, "fight", 5) == 0) {
			std::memset(this->m_cMapName, 0, sizeof (this->m_cMapName));
			if (this->m_cSide == 0) {
				switch (iDice(1, 2)) {
					case 1:
						memcpy(this->m_cMapName, "aresden", 7);
						break;
					case 2:
						memcpy(this->m_cMapName, "elvine", 6);
						break;
				}
			} else {
				memcpy(this->m_cMapName, this->m_cLocation, 10);
			}
			this->m_sX = -1;
			this->m_sY = -1;
		}
		if (this->m_bIsInitComplete == true) {
			if (game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, id_, bCountLogout) == false) game_.LocalSavePlayerData(id_);
		} else game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, id_, bCountLogout);
	} else {
		if (this->m_bIsOnServerChange == false) {
			game_.bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, id_, bCountLogout);
		} else {
			game_.bSendMsgToLS(MSGID_REQUEST_SETACCOUNTWAITSTATUS, id_, false);
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
	game_.m_iTotalClients--;
	//50Cent - Capture The Flag
	if (game_.bCheckIfIsFlagCarrier(id_)) {
		game_.SetFlagCarrierFlag(id_, false);
		game_.SetIceFlag(id_, DEF_OWNERTYPE_PLAYER, false);
	}
	this->m_bMarkedForDeletion = true;
}

