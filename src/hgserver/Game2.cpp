#include "Game.h"
#include <cstring>
#include "NpcArchetype.h"
extern char G_cTxt[512];
extern HWND G_hWnd;

void CGame::Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr) {
	int iPartyID;
	int iDamage;
	int iSideCondition;
	int iIndex;
	int iRemainLife;
	int iTemp;
	int iExp;
	int iMaxSuperAttack;
	int iRepDamage;
	char cAttackerSide;
	char cDamageMoveDir;
	char cDamageMinimum;
	uint32_t dwTime;
	register double dTmp1, dTmp2, dTmp3;
	short sTgtX;
	short sTgtY;
	short sItemIndex;
	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] == nullptr)) return;
	if ((cAttackerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sAttackerH] == nullptr)) return;
	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->map_ != 0) &&
			  (m_pClientList[sAttackerH]->map_->m_bIsHeldenianMap == 1) && (m_bHeldenianInitiated)) return;
	dwTime = timeGetTime();
	sTgtX = 0;
	sTgtY = 0;
	iDamage = iDice(sV1, sV2) + sV3;
	if (iDamage <= 0) iDamage = 0;
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER:
			if ((m_bAdminSecurity) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return;
			if (m_pClientList[sAttackerH]->m_cHeroArmourBonus == 2) iDamage += 4;
			if ((m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::LHAND] == -1) || (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND] == -1)) {
				sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::RHAND];
				if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
					if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 861 || m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 862) {
						iDamage *= (int) 1.3;
					}
					if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 863 || m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 864) {
						if (m_pClientList[sAttackerH]->m_iRating > 0) {
							iRepDamage = m_pClientList[sAttackerH]->m_iRating / 100;
							if (iRepDamage < 5) iRepDamage = 5;
							iDamage += iRepDamage;
						}
						if (cTargetType == DEF_OWNERTYPE_PLAYER) {
							if (m_pClientList[sTargetH] != nullptr) {
								if (m_pClientList[sTargetH]->m_iRating < 0) {
									iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 10);
									if (iRepDamage > 10) iRepDamage = 10;
									iDamage += iRepDamage;
								}
							}
						}
					}
				}
				sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::NECK];
				if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
					if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 859) { // NecklaceOfKloness
						if (cTargetType == DEF_OWNERTYPE_PLAYER) {
							if (m_pClientList[sTargetH] != nullptr) {
								iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 20);
								if (iRepDamage > 5) iRepDamage = 5;
								iDamage += iRepDamage;
							}
						}
					}
				}
			}
			if ((!m_bIsCrusadeMode) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true) && (cTargetType == DEF_OWNERTYPE_PLAYER)) return;
			dTmp1 = (double) iDamage;
			if (m_pClientList[sAttackerH]->m_iMag <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double) m_pClientList[sAttackerH]->m_iMag;
			dTmp2 = dTmp2 / 3.3f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iDamage = (int) (dTmp3 + 0.5f);
			if (iDamage <= 0) iDamage = 0;
			// order switched with above
			iDamage += m_pClientList[sAttackerH]->m_iAddMagicalDamage;
			if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true)
				iDamage += iDamage / 3;
			if (m_pClientList[sAttackerH]->bCheckHeldenianMap() == 1) {
				iDamage += iDamage / 3;
			}
			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
				if (m_pClientList[sAttackerH]->m_iLevel <= 80) {
					iDamage += (iDamage * 7) / 10;
				} else if (m_pClientList[sAttackerH]->m_iLevel <= 100) {
					iDamage += iDamage / 2;
				} else
					iDamage += iDamage / 3;
			}
			cAttackerSide = m_pClientList[sAttackerH]->m_cSide;
			iPartyID = m_pClientList[sAttackerH]->m_iPartyID;
			break;
		case DEF_OWNERTYPE_NPC:
			cAttackerSide = m_pNpcList[sAttackerH]->m_cSide;
			break;
	}
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsInitComplete == false) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;
			if ((dwTime - m_pClientList[sTargetH]->m_dwTime) > DEF_RAGPROTECTIONTIME) return;
			if (m_pClientList[sTargetH]->map_ == nullptr) return;
			if ((m_pClientList[sTargetH]->map_->m_bIsAttackEnabled == false) && (m_pClientList[sTargetH]->m_iAdminUserLevel == 0)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) && (m_pClientList[sTargetH]->m_iPKCount == 0)) return;
			if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (!m_bIsCrusadeMode) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsOwnLocation == true)) return;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				if (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) {
					iSideCondition = m_pClientList[sAttackerH]->iGetPlayerRelationship(sTargetH);
					if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {
					} else {
						if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true) {
							if (m_pClientList[sAttackerH]->m_iGuildGUID != m_pClientList[sTargetH]->m_iGuildGUID) {
							} else return;
						} else return;
					}
				}
				if (m_pClientList[sTargetH]->map_->iGetAttribute(m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY, 0x00000005) != 0) return;
			}
			m_pClientList[sTargetH]->ClearSkillUsingStatus();
			switch (iAttr) {
				case 1:
					if (m_pClientList[sTargetH]->m_iAddAbsEarth != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsEarth;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				case 2:
					if (m_pClientList[sTargetH]->m_iAddAbsAir != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsAir;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				case 3:
					if (m_pClientList[sTargetH]->m_iAddAbsFire != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsFire;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				case 4:
					if (m_pClientList[sTargetH]->m_iAddAbsWater != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsWater;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				default: break;
			}
			iIndex = m_pClientList[sTargetH]->m_iMagicDamageSaveItemIndex;
			if ((iIndex != -1) && (iIndex >= 0) && (iIndex < DEF_MAXITEMS)) {
				switch (m_pClientList[sTargetH]->m_pItemList[iIndex]->m_sIDnum) {
					case 335:
						dTmp1 = (double) iDamage;
						dTmp2 = dTmp1 * 0.2f;
						dTmp3 = dTmp1 - dTmp2;
						iDamage = (int) (dTmp3 + 0.5f);
						break;
					case 337:
						dTmp1 = (double) iDamage;
						dTmp2 = dTmp1 * 0.1f;
						dTmp3 = dTmp1 - dTmp2;
						iDamage = (int) (dTmp3 + 0.5f);
						break;
				}
				if (iDamage <= 0) iDamage = 0;
				iRemainLife = m_pClientList[sTargetH]->m_pItemList[iIndex]->m_wCurLifeSpan;
				if (iRemainLife <= iDamage) {
					m_pClientList[sTargetH]->ItemDepleteHandler(iIndex, true, true);
				} else {
					m_pClientList[sTargetH]->m_pItemList[iIndex]->m_wCurLifeSpan -= iDamage;
				}
			}
			if (m_pClientList[sTargetH]->m_iAddAbsMD != 0) {
				dTmp1 = (double) iDamage;
				dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsMD;
				dTmp3 = (dTmp2 / 100.0f) * dTmp1;
				iDamage = iDamage - (int) dTmp3;
			}
			if (cTargetType == DEF_OWNERTYPE_PLAYER) {
				iDamage -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
				if (iDamage <= 0) iDamage = 0;
			}
			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] == 2)
				iDamage = iDamage / 2;
			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] == 5) break;
			if ((m_pClientList[sTargetH]->m_bIsLuckyEffect == true) &&
					  (iDice(1, 10) == 5) && (m_pClientList[sTargetH]->m_iHP <= iDamage)) {
				iDamage = m_pClientList[sTargetH]->m_iHP - 1;
			}
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true)) {
				switch (m_pClientList[sTargetH]->m_iSpecialAbilityType) {
					case 51:
					case 52:
						return;
				}
			}
			m_pClientList[sTargetH]->m_iHP -= iDamage;
			if (m_pClientList[sTargetH]->m_iHP <= 0) {
				m_pClientList[sTargetH]->ClientKilledHandler(sAttackerH, cAttackerType, iDamage);
			} else {
				if (iDamage > 0) {
					if (m_pClientList[sTargetH]->m_iAddTransMana > 0) {
						dTmp1 = (double) m_pClientList[sTargetH]->m_iAddTransMana;
						dTmp2 = (double) iDamage;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2 + 1.0f;
						iTemp = (2 * m_pClientList[sTargetH]->m_iMag) + (2 * m_pClientList[sTargetH]->m_iLevel) + (m_pClientList[sTargetH]->m_iInt / 2);
						m_pClientList[sTargetH]->m_iMP += (int) dTmp3;
						if (m_pClientList[sTargetH]->m_iMP > iTemp) m_pClientList[sTargetH]->m_iMP = iTemp;
					}
					if (m_pClientList[sTargetH]->m_iAddChargeCritical > 0) {
						if (iDice(1, 100) < (m_pClientList[sTargetH]->m_iAddChargeCritical)) {
							iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
							if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
							m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
						}
					}
					if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true)) {
						cDamageMinimum = 80;
					} else {
						cDamageMinimum = 50;
					}
					if (iDamage >= cDamageMinimum) {
						sTgtX = m_pClientList[sTargetH]->m_sX;
						sTgtY = m_pClientList[sTargetH]->m_sY;
						if (sTgtX == sAtkX) {
							if (sTgtY == sAtkY) return;
							else if (sTgtY > sAtkY) cDamageMoveDir = 5;
							else if (sTgtY < sAtkY) cDamageMoveDir = 1;
						} else if (sTgtX > sAtkX) {
							if (sTgtY == sAtkY) cDamageMoveDir = 3;
							else if (sTgtY > sAtkY) cDamageMoveDir = 4;
							else if (sTgtY < sAtkY) cDamageMoveDir = 2;
						} else if (sTgtX < sAtkX) {
							if (sTgtY == sAtkY) cDamageMoveDir = 7;
							else if (sTgtY > sAtkY) cDamageMoveDir = 6;
							else if (sTgtY < sAtkY) cDamageMoveDir = 8;
						}
						m_pClientList[sTargetH]->m_iLastDamage = iDamage;
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_DAMAGEMOVE, cDamageMoveDir, iDamage, 0, nullptr);
					}
					m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
					m_pClientList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
					if (m_pClientList[sTargetH]->m_bSkillUsingStatus[19] != true) {
						m_pClientList[sTargetH]->map_->ClearOwner(0, sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						m_pClientList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
					}
					if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT], 0, nullptr);
						m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, MagicType::HOLDOBJECT);
					}
				}
			}
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if ((m_bIsCrusadeMode) && (cAttackerSide == m_pNpcList[sTargetH]->m_cSide)) return;
			switch (m_pNpcList[sTargetH]->m_cActionLimit) {
				case 1:
				case 2:
				case 4:
				case 6:
					return;
				case 3:
				case 5:
					if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
						switch (m_pNpcList[sTargetH]->m_sType) {
							case 40:
							case 41:
								if ((m_pClientList[sAttackerH]->m_cSide == 0) || (m_pNpcList[sTargetH]->m_cSide == m_pClientList[sAttackerH]->m_cSide)) return;
								break;
						}
					}
			}
			if (m_pNpcList[sTargetH]->m_iAbsDamage > 0) {
				dTmp1 = (double) iDamage;
				dTmp2 = (double) (m_pNpcList[sTargetH]->m_iAbsDamage) / 100.0f;
				dTmp3 = dTmp1 * dTmp2;
				dTmp2 = dTmp1 - dTmp3;
				iDamage = (int) dTmp2;
				if (iDamage < 0) iDamage = 1;
			}
			if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] == 2)
				iDamage = iDamage / 2;
			m_pNpcList[sTargetH]->m_iHP -= iDamage;
			if (m_pNpcList[sTargetH]->m_iHP < 0) {
				m_pNpcList[sTargetH]->NpcKilledHandler(sAttackerH, cAttackerType, iDamage);
			} else {
				switch (cAttackerType) {
					case DEF_OWNERTYPE_PLAYER:
						if ((m_pNpcList[sTargetH]->m_sType != 21) && (m_pNpcList[sTargetH]->m_sType != 55) && (m_pNpcList[sTargetH]->m_sType != 56)
								  && (m_pNpcList[sTargetH]->m_cSide == cAttackerSide)) return;
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide) return;
						break;
				}
				m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
				if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0)) {
					if ((cAttackerType == DEF_OWNERTYPE_NPC) &&
							  (m_pNpcList[sAttackerH]->m_sType == m_pNpcList[sTargetH]->m_sType) &&
							  (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide)) return;
					m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
					m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
					m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
					m_pNpcList[sTargetH]->m_dwTime = dwTime;
					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, MagicType::HOLDOBJECT);
					}
					if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
						if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
							iExp = iDamage;
							if ((m_bIsCrusadeMode) && (iExp > 10)) iExp = 10;
							if (m_pClientList[sAttackerH]->m_iAddExp > 0) {
								dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double) iExp;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iExp += (int) dTmp3;
							}
							if (m_pClientList[sAttackerH]->m_iLevel > 100) {
								switch (m_pNpcList[sTargetH]->m_sType) {
									case 55:
									case 56:
										iExp = 0;
										break;
									default: break;
								}
							}
							if (bExp)
								m_pClientList[sAttackerH]->GetExp(iExp, true);
							else m_pClientList[sAttackerH]->GetExp((iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
						} else {
							iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							if ((m_bIsCrusadeMode) && (iExp > 10)) iExp = 10;
							if (m_pClientList[sAttackerH]->m_iAddExp > 0) {
								dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double) iExp;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iExp += (int) dTmp3;
							}
							if (m_pClientList[sAttackerH]->m_iLevel > 100) {
								switch (m_pNpcList[sTargetH]->m_sType) {
									case 55:
									case 56:
										iExp = 0;
										break;
									default: break;
								}
							}
							if (bExp)
								m_pClientList[sAttackerH]->GetExp(iExp, true);
							else m_pClientList[sAttackerH]->GetExp((iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
					}
				}
			}
			break;
	}
}

void CGame::Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr) {
	int iDamage;
	int iSideCondition;
	int iIndex;
	int iRemainLife;
	int iTemp;
	int iMaxSuperAttack;
	uint32_t dwTime;
	uint32_t wWeaponType;
	char cDamageMod[256];
	char cAttackerSide;
	char cDamageMoveDir;
	register double dTmp1, dTmp2, dTmp3;
	int iPartyID;
	int iMoveDamage;
	short sTgtX;
	short sTgtY;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;
	if (cAttackerType == DEF_OWNERTYPE_NPC)
		if (m_pNpcList[sAttackerH] == nullptr) return;
	dwTime = timeGetTime();
	sTgtX = 0;
	sTgtY = 0;
	iDamage = iDice(sV1, sV2) + sV3;
	if (iDamage <= 0) iDamage = 0;
	iPartyID = 0;
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER:
			if ((m_bAdminSecurity) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return;
			dTmp1 = (double) iDamage;
			if ((m_pClientList[sAttackerH]->m_iMag + m_pClientList[sAttackerH]->m_iAngelicMag) <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double) (m_pClientList[sAttackerH]->m_iMag + m_pClientList[sAttackerH]->m_iAngelicMag);
			dTmp2 = dTmp2 / 3.3f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iDamage = (int) (dTmp3 + 0.5f);
			if (iDamage <= 0) iDamage = 0;
			// v1.432 2001 4 7 13 7
			iDamage += m_pClientList[sAttackerH]->m_iAddMagicalDamage;
			if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true)
				iDamage += iDamage / 3;
			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
				if (m_pClientList[sAttackerH]->m_iLevel <= 80) {
					iDamage += (iDamage * 7) / 10;
				} else if (m_pClientList[sAttackerH]->m_iLevel <= 100) {
					iDamage += iDamage / 2;
				} else iDamage += iDamage / 3;
			}
			if (m_pClientList[sAttackerH]->m_cHeroArmourBonus == 2) {
				iDamage += 4;
			}
			wWeaponType = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
			if (wWeaponType == 34) {
				iDamage += iDamage / 3;
			}
			if (m_pClientList[sAttackerH]->bCheckHeldenianMap() == 1) {
				iDamage += iDamage / 3;
			}
			cAttackerSide = m_pClientList[sAttackerH]->m_cSide;
			iPartyID = m_pClientList[sAttackerH]->m_iPartyID;
			break;
		case DEF_OWNERTYPE_NPC:
			cAttackerSide = m_pNpcList[sAttackerH]->m_cSide;
			break;
	}
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsInitComplete == false) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;
			if ((dwTime - m_pClientList[sTargetH]->m_dwTime) > DEF_RAGPROTECTIONTIME) return;
			if (m_pClientList[sTargetH]->map_ == nullptr) return;
			if ((m_pClientList[sTargetH]->map_->m_bIsAttackEnabled == false) && (m_pClientList[sTargetH]->m_iAdminUserLevel == 0)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsNeutral == true) && (m_pClientList[sTargetH]->m_iPKCount == 0)) return;
			if ((!m_bIsCrusadeMode) && (m_pClientList[sTargetH]->m_iPKCount == 0) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;
			if ((!m_bIsCrusadeMode) && (m_pClientList[sTargetH]->m_iPKCount == 0) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) && (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				if (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) {
					iSideCondition = m_pClientList[sAttackerH]->iGetPlayerRelationship(sTargetH);
					if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {
					} else {
						if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true) {
							if (m_pClientList[sAttackerH]->m_iGuildGUID != m_pClientList[sTargetH]->m_iGuildGUID) {
							} else return;
						} else return;
					}
				}
				if (m_pClientList[sTargetH]->map_->iGetAttribute(m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY, 0x00000005) != 0) return;
			}
			m_pClientList[sTargetH]->ClearSkillUsingStatus();
			switch (iAttr) {
				case 1:
					if (m_pClientList[sTargetH]->m_iAddAbsEarth != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsEarth;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				case 2:
					if (m_pClientList[sTargetH]->m_iAddAbsAir != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsAir;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				case 3:
					if (m_pClientList[sTargetH]->m_iAddAbsFire != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsFire;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				case 4:
					if (m_pClientList[sTargetH]->m_iAddAbsWater != 0) {
						dTmp1 = (double) iDamage;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsWater;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iDamage = iDamage - (int) (dTmp3);
						if (iDamage < 0) iDamage = 0;
					}
					break;
				default: break;
			}
			iIndex = m_pClientList[sTargetH]->m_iMagicDamageSaveItemIndex;
			if ((iIndex != -1) && (iIndex >= 0) && (iIndex < DEF_MAXITEMS)) {
				switch (m_pClientList[sTargetH]->m_pItemList[iIndex]->m_sIDnum) {
					case 335:
						dTmp1 = (double) iDamage;
						dTmp2 = dTmp1 * 0.2f;
						dTmp3 = dTmp1 - dTmp2;
						iDamage = (int) (dTmp3 + 0.5f);
						break;
					case 337:
						dTmp1 = (double) iDamage;
						dTmp2 = dTmp1 * 0.1f;
						dTmp3 = dTmp1 - dTmp2;
						iDamage = (int) (dTmp3 + 0.5f);
						break;
				}
				if (iDamage <= 0) iDamage = 0;
				iRemainLife = m_pClientList[sTargetH]->m_pItemList[iIndex]->m_wCurLifeSpan;
				if (iRemainLife <= iDamage) {
					m_pClientList[sTargetH]->ItemDepleteHandler(iIndex, true, true);
				} else {
					m_pClientList[sTargetH]->m_pItemList[iIndex]->m_wCurLifeSpan -= iDamage;
				}
			}
			if (m_pClientList[sTargetH]->m_iAddAbsMD != 0) {
				dTmp1 = (double) iDamage;
				dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsMD;
				dTmp3 = (dTmp2 / 100.0f) * dTmp1;
				iDamage = iDamage - (int) dTmp3;
			}
			if (cTargetType == DEF_OWNERTYPE_PLAYER) {
				iDamage -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
				if (iDamage <= 0) iDamage = 0;
			}
			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] == 2)
				iDamage = iDamage / 2;
			if ((m_pClientList[sTargetH]->m_bIsLuckyEffect == true) &&
					  (iDice(1, 10) == 5) && (m_pClientList[sTargetH]->m_iHP <= iDamage)) {
				iDamage = m_pClientList[sTargetH]->m_iHP - 1;
			}
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true)) {
				switch (m_pClientList[sTargetH]->m_iSpecialAbilityType) {
					case 51:
					case 52:
						//
						return;
				}
			}
			m_pClientList[sTargetH]->m_iHP -= iDamage;
			if (m_pClientList[sAttackerH] != nullptr) {
				if (m_pClientList[sAttackerH]->iDmgShowon == 1) {
					std::memset(cDamageMod, 0, sizeof (cDamageMod));
					wsprintf(cDamageMod, "You did [%d] Damage to Character [%s] Remaining Life [%d]", iDamage, m_pClientList[sTargetH]->m_cCharName, m_pClientList[sTargetH]->m_iHP);
					m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
				}
			}
			if (m_pClientList[sTargetH]->m_iHP <= 0) {
				m_pClientList[sTargetH]->ClientKilledHandler(sAttackerH, cAttackerType, iDamage);
			} else {
				if (iDamage > 0) {
					if (m_pClientList[sTargetH]->m_iAddTransMana > 0) {
						dTmp1 = (double) m_pClientList[sTargetH]->m_iAddTransMana;
						dTmp2 = (double) iDamage;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2 + 1.0f;
						iTemp = (2 * (m_pClientList[sTargetH]->m_iMag + m_pClientList[sTargetH]->m_iAngelicMag)) + (2 * m_pClientList[sTargetH]->m_iLevel) + ((m_pClientList[sTargetH]->m_iInt + m_pClientList[sTargetH]->m_iAngelicInt) / 2);
						m_pClientList[sTargetH]->m_iMP += (int) dTmp3;
						if (m_pClientList[sTargetH]->m_iMP > iTemp) m_pClientList[sTargetH]->m_iMP = iTemp;
					}
					if (m_pClientList[sTargetH]->m_iAddChargeCritical > 0) {
						if (iDice(1, 100) < (m_pClientList[sTargetH]->m_iAddChargeCritical)) {
							iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
							if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
							m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
						}
					}
					if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true))
						iMoveDamage = 80;
					else iMoveDamage = 50;
					if (iDamage >= iMoveDamage) {
						///		char cDamageMoveDir;
						sTgtX = m_pClientList[sTargetH]->m_sX;
						sTgtY = m_pClientList[sTargetH]->m_sY;
						if (sTgtX == sAtkX) {
							if (sTgtY == sAtkY) goto EDSD_SKIPDAMAGEMOVE;
							else if (sTgtY > sAtkY) cDamageMoveDir = 5;
							else if (sTgtY < sAtkY) cDamageMoveDir = 1;
						} else if (sTgtX > sAtkX) {
							if (sTgtY == sAtkY) cDamageMoveDir = 3;
							else if (sTgtY > sAtkY) cDamageMoveDir = 4;
							else if (sTgtY < sAtkY) cDamageMoveDir = 2;
						} else if (sTgtX < sAtkX) {
							if (sTgtY == sAtkY) cDamageMoveDir = 7;
							else if (sTgtY > sAtkY) cDamageMoveDir = 6;
							else if (sTgtY < sAtkY) cDamageMoveDir = 8;
						}
						m_pClientList[sTargetH]->m_iLastDamage = iDamage;
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_DAMAGEMOVE, cDamageMoveDir, iDamage, 0, nullptr);
					} else {
EDSD_SKIPDAMAGEMOVE:
						;
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
						m_pClientList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
					}
					if (m_pClientList[sTargetH]->m_bSkillUsingStatus[19] != true) {
						m_pClientList[sTargetH]->map_->ClearOwner(0, sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						m_pClientList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
					}
					if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {
						// 1: Hold-Person
						// 2: Paralize
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT], 0, nullptr);
						m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, MagicType::HOLDOBJECT);
					}
				}
			}
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if ((m_bIsCrusadeMode) && (cAttackerSide == m_pNpcList[sTargetH]->m_cSide)) return;
			switch (m_pNpcList[sTargetH]->m_cActionLimit) {
				case 1:
				case 2:
				case 4:
					return;
			}
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				switch (m_pNpcList[sTargetH]->m_sType) {
					case 40:
					case 41:
						if ((m_pClientList[sAttackerH]->m_cSide == 0) || (m_pNpcList[sTargetH]->m_cSide == m_pClientList[sAttackerH]->m_cSide)) return;
						break;
				}
			}
			switch (m_pNpcList[sTargetH]->m_sType) {
				case 67: // McGaffin
				case 68: // Perry
				case 69: // Devlin
					iDamage = 0;
					break;
			}
			if (m_pNpcList[sTargetH]->m_iAbsDamage > 0) {
				dTmp1 = (double) iDamage;
				dTmp2 = (double) (m_pNpcList[sTargetH]->m_iAbsDamage) / 100.0f;
				dTmp3 = dTmp1 * dTmp2;
				dTmp2 = dTmp1 - dTmp3;
				iDamage = (int) dTmp2;
				if (iDamage < 0) iDamage = 1;
			}
			if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] == 2)
				iDamage = iDamage / 2;
			m_pNpcList[sTargetH]->m_iHP -= iDamage;
			if (m_pClientList[sAttackerH] != nullptr) {
				if (m_pClientList[sAttackerH]->iDmgShowon == 1) {
					std::memset(cDamageMod, 0, sizeof (cDamageMod));
					wsprintf(cDamageMod, "You did [%d] Damage to Npc [%s] Remaining Life [%d]", iDamage, m_pNpcList[sTargetH]->m_cNpcName, m_pNpcList[sTargetH]->m_iHP);
					m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
				}
			}
			if (m_pNpcList[sTargetH]->m_iHP <= 0) {
				m_pNpcList[sTargetH]->NpcKilledHandler(sAttackerH, cAttackerType, iDamage);
			} else {
				switch (cAttackerType) {
					case DEF_OWNERTYPE_PLAYER:
						if ((m_pNpcList[sTargetH]->m_sType != 21) && (m_pNpcList[sTargetH]->m_sType != 55) && (m_pNpcList[sTargetH]->m_sType != 56)
								  && (m_pNpcList[sTargetH]->m_cSide == cAttackerSide)) return;
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide) return;
						break;
				}
				m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
				if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0)) {
					if ((cAttackerType == DEF_OWNERTYPE_NPC) &&
							  (m_pNpcList[sAttackerH]->m_sType == m_pNpcList[sTargetH]->m_sType) &&
							  (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide)) return;
					m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
					m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
					m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
					m_pNpcList[sTargetH]->m_dwTime = dwTime;
					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, MagicType::HOLDOBJECT);
					}
					//Crusade
					int iExp;
					if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) &&
							  (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
						if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
							// Crusade
							iExp = iDamage;
							if ((m_bIsCrusadeMode) && (iExp > 10)) iExp = 10;
							if (m_pClientList[sAttackerH]->m_iAddExp > 0) {
								dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double) iExp;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iExp += (int) dTmp3;
							}
							if (m_pClientList[sAttackerH]->m_iLevel > 100) {
								switch (m_pNpcList[sTargetH]->m_sType) {
									case 55:
									case 56:
										iExp = 0;
										break;
									default: break;
								}
							}
							if (bExp)
								m_pClientList[sAttackerH]->GetExp(iExp); //m_pClientList[sAttackerH]->m_iExpStock += iExp;     //iDamage;
							else m_pClientList[sAttackerH]->GetExp((iExp / 2)); //m_pClientList[sAttackerH]->m_iExpStock += (iExp/2); //(iDamage/2);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
						} else {
							// Crusade
							iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							if ((m_bIsCrusadeMode) && (iExp > 10)) iExp = 10;
							if (m_pClientList[sAttackerH]->m_iAddExp > 0) {
								dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double) iExp;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iExp += (int) dTmp3;
							}
							if (m_pClientList[sAttackerH]->m_iLevel > 100) {
								switch (m_pNpcList[sTargetH]->m_sType) {
									case 55:
									case 56:
										iExp = 0;
										break;
									default: break;
								}
							}
							if (bExp)
								m_pClientList[sAttackerH]->GetExp(iExp); //m_pClientList[sAttackerH]->m_iExpStock += iExp;     //m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							else m_pClientList[sAttackerH]->GetExp((iExp / 2)); //m_pClientList[sAttackerH]->m_iExpStock += (iExp/2); //(m_pNpcList[sTargetH]->m_iNoDieRemainExp/2);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
					}
				}
			}
			break;
	}
}

void CGame::Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3) {
	int iHP;
	int iMaxHP;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;
	iHP = iDice(sV1, sV2) + sV3;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;
			iMaxHP = (3 * m_pClientList[sTargetH]->m_iVit) + (2 * m_pClientList[sTargetH]->m_iLevel) + ((m_pClientList[sTargetH]->m_iStr + m_pClientList[sTargetH]->m_iAngelicStr) / 2);
			if (m_pClientList[sTargetH]->m_iSideEffect_MaxHPdown != 0)
				iMaxHP = iMaxHP - (iMaxHP / m_pClientList[sTargetH]->m_iSideEffect_MaxHPdown);
			if (m_pClientList[sTargetH]->m_iHP < iMaxHP) {
				m_pClientList[sTargetH]->m_iHP += iHP;
				if (m_pClientList[sTargetH]->m_iHP > iMaxHP) m_pClientList[sTargetH]->m_iHP = iMaxHP;
				if (m_pClientList[sTargetH]->m_iHP <= 0) m_pClientList[sTargetH]->m_iHP = 1;
				m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
			}
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if (m_pNpcList[sTargetH]->m_bIsKilled == true) return;
			iMaxHP = m_pNpcList[sTargetH]->m_iHitDice * 4;
			if (m_pNpcList[sTargetH]->m_iHP < iMaxHP) {
				m_pNpcList[sTargetH]->m_iHP += iHP;
				if (m_pNpcList[sTargetH]->m_iHP > iMaxHP) m_pNpcList[sTargetH]->m_iHP = iMaxHP;
				if (m_pNpcList[sTargetH]->m_iHP <= 0) m_pNpcList[sTargetH]->m_iHP = 1;
			}
			break;
	}
}

void CGame::Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3) {
	int iSP;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;
	iSP = iDice(sV1, sV2) + sV3;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;
			// New 19/05/2004
			// Is the user having an invincibility slate
			if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return;
			if (m_pClientList[sTargetH]->m_iSP > 0) {
				//v1.42
				if (m_pClientList[sTargetH]->m_iTimeLeft_FirmStaminar == 0) {
					m_pClientList[sTargetH]->m_iSP -= iSP;
					if (m_pClientList[sTargetH]->m_iSP < 0) m_pClientList[sTargetH]->m_iSP = 0;
					m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
				}
			}
			break;
		case DEF_OWNERTYPE_NPC:
			break;
	}
}

void CGame::Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3) {
	int iSP;
	int iMaxSP;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;
	iSP = iDice(sV1, sV2) + sV3;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;
			iMaxSP = (2 * (m_pClientList[sTargetH]->m_iStr + m_pClientList[sTargetH]->m_iAngelicStr)) + (2 * m_pClientList[sTargetH]->m_iLevel);
			if (m_pClientList[sTargetH]->m_iSP < iMaxSP) {
				m_pClientList[sTargetH]->m_iSP += iSP;
				if (m_pClientList[sTargetH]->m_iSP > iMaxSP)
					m_pClientList[sTargetH]->m_iSP = iMaxSP;
				m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
			}
			break;
		case DEF_OWNERTYPE_NPC:
			break;
	}
}

/*********************************************************************************************************************
 **  int bool CGame::bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio) **
 **  description			:: calculates if a player resists magic														**
 **  last updated		:: November 20, 2004; 8:42 PM; Hypnotoad													**
 **	return value		:: bool																						**
 **  commentary			::	-	hero armor for target mages adds 50 magic resistance								**
 **							-	10000 or more it ratio will deduct 10000 hit ratio									**
 **							-	invincible tablet is 100% magic resistance											**
 **********************************************************************************************************************/
bool CGame::bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio) {
	double dTmp1;
	double dTmp2;
	double dTmp3;
	int iTargetMagicResistRatio;
	int iDestHitRatio;
	int iResult;
	char cProtect;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return false;
			if (m_pClientList[sTargetH]->map_->m_bIsAttackEnabled == false) return false;
			if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
			if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return true;
			iTargetMagicResistRatio = m_pClientList[sTargetH]->m_cSkillMastery[3] + m_pClientList[sTargetH]->m_iAddMR;
			if ((m_pClientList[sTargetH]->m_iMag + m_pClientList[sTargetH]->m_iAngelicMag) > 50)
				iTargetMagicResistRatio += ((m_pClientList[sTargetH]->m_iMag + m_pClientList[sTargetH]->m_iAngelicMag) - 50);
			iTargetMagicResistRatio += m_pClientList[sTargetH]->m_iAddResistMagic;
			cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT];
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return false;
			iTargetMagicResistRatio = m_pNpcList[sTargetH]->m_cResistMagic;
			cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT];
			break;
	}
	if (cProtect == 5) return true;
	if ((iHitRatio < 1000) && (cProtect == 2)) return true;
	if (iHitRatio >= 10000) iHitRatio -= 10000;
	if (iTargetMagicResistRatio < 1) iTargetMagicResistRatio = 1;
	if ((cAttackerDir != 0) && (m_pClientList[sTargetH] != nullptr) && (m_pClientList[sTargetH]->m_cHeroArmourBonus == 2)) {
		iHitRatio += 50;
	}
	dTmp1 = (double) (iHitRatio);
	dTmp2 = (double) (iTargetMagicResistRatio);
	dTmp3 = (dTmp1 / dTmp2)*50.0f;
	iDestHitRatio = (int) (dTmp3);
	if (iDestHitRatio < DEF_MINIMUMHITRATIO) iDestHitRatio = DEF_MINIMUMHITRATIO;
	if (iDestHitRatio > DEF_MAXIMUMHITRATIO) iDestHitRatio = DEF_MAXIMUMHITRATIO;
	if (iDestHitRatio >= 100) return false;
	iResult = iDice(1, 100);
	if (iResult <= iDestHitRatio) return false;
	if (cTargetType == DEF_OWNERTYPE_PLAYER)
		m_pClientList[sTargetH]->CalculateSSN_SkillIndex(3, 1);
	return true;
}

UnitPtr CGame::getUnit(short sHandle, char cType) {
	UnitPtr unitPtr;
	switch (cType) {
		case DEF_OWNERTYPE_PLAYER:
			unitPtr = m_pClientList[sHandle];
			break;
		case DEF_OWNERTYPE_NPC:
			unitPtr = m_pNpcList[sHandle];
			break;
	}
	return unitPtr;
}
void CGame::OnKeyDown(WPARAM wParam, LPARAM /*lParam*/) {
	switch (wParam) {
		case VK_F1:
			m_bF1pressed = true;
			break;
		case VK_F4:
			m_bF4pressed = true;
			break;
		case VK_F5:
			m_bF5pressed = true;
			break;
		case VK_F12:
			m_bF12pressed = true;
			break;
	}
}

void CGame::OnKeyUp(WPARAM wParam, LPARAM /*lParam*/) {
	int i;
	switch (wParam) {
		case VK_F2:
			/*
			char cTxt[120];
			for (i = 1; i <= 200; i++){
				wsprintf(cTxt, "Level %d:  Exp %d", i, iGetLevelExp(i));
				PutLogFileList(cTxt);
			}

			if (!m_bOnExitProcess) {
				m_cShutDownCode      = 3;
				m_bOnExitProcess     = true;
				m_dwExitProcessTime  = timeGetTime();

				PutLogList("(!) GAME SERVER SHUTDOWN PROCESS STARTED(by Log-server connection Lost Emulation)!!!");
			}
			 */
			break;
		case VK_F1:
			m_bF1pressed = false;
			break;
		case VK_F4:
			m_bF4pressed = false;
			break;
		case VK_F5:
			m_bF5pressed = false;
			break;
		case VK_F12:
			m_bF12pressed = false;
			break;
		case VK_F6:
			if (m_bF1pressed) {
				PutLogList("(!) Send server shutdown announcement1...");
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_SERVERSHUTDOWN, 1, 0, 0, nullptr);
					}
			}
			break;
		case VK_F7:
			if (m_bF1pressed) {
				PutLogList("(!) Send server shutdown announcement2...");
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_SERVERSHUTDOWN, 2, 0, 0, nullptr);
					}
			}
			break;
		case VK_F9:
			if ((m_bF1pressed)) {
				PutLogList("(!!!) Resume Crusade Mode...");
				LocalStartCrusadeMode(0);
			}
			break;
		case VK_F11:
			if ((m_bF1pressed)) {
				PutLogList("(!!!) ManualEndCrusadeMode: side 0");
				ManualEndCrusadeMode(0);
			}
			break;
#define VK_1 0x31
		case VK_1:
			if ((m_bF1pressed)) {
				GlobalUpdateConfigs(1);
			}
			break;
#define VK_2 0x32
		case VK_2:
			if ((m_bF1pressed)) {
				GlobalUpdateConfigs(2);
			}
			break;
#define VK_3 0x33
		case VK_3:
			if ((m_bF1pressed)) {
				GlobalUpdateConfigs(3);
			}
			break;
#define VK_4 0x34
		case VK_4:
			if ((m_bF1pressed)) {
				GlobalUpdateConfigs(1);
			}
			break;
			/*#define VK_H 0x49 // H key
			case VK_H:
				if ((m_bF1pressed)) {
					GlobalStartHeldenianMode();
				}
				break;*/
			//Crusade Testcode
		case VK_HOME:
			if ((m_bF1pressed)) {
				GlobalStartCrusadeMode();
			}
			break;
		case VK_INSERT:
			MeteorStrikeMsgHandler(1);
			break;
		case VK_DELETE:
			MeteorStrikeMsgHandler(2);
			break;
	}
}

int CGame::iGetFollowerNumber(short sOwnerH, char cOwnerType) {
	register int i, iTotal;
	iTotal = 0;
	for (i = 1; i < DEF_MAXNPCS; i++) {
		if ((m_pNpcList[i] != nullptr) && (m_pNpcList[i]->m_cMoveType == DEF_MOVETYPE_FOLLOW)) {
			if ((m_pNpcList[i]->m_iFollowOwnerIndex == sOwnerH) && (m_pNpcList[i]->m_cFollowOwnerType == cOwnerType))
				iTotal++;
		}
	}
	return iTotal;
}

int CGame::_iGetTotalClients() {
	register int i, iTotal;
	iTotal = 0;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) iTotal++;
	return iTotal;
}

int CGame::iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, MapPtr map, int dX, int dY) {
	class CItem * pItem;
	char cItemName[21];
	short lX;
	short lY;
	int iResult;
	int iFish;
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[iOwnerH] == nullptr) return 0;
			if (m_pClientList[iOwnerH]->map_ != map) return 0;
			lX = m_pClientList[iOwnerH]->m_sX;
			lY = m_pClientList[iOwnerH]->m_sY;
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[iOwnerH] == nullptr) return 0;
			if (m_pNpcList[iOwnerH]->map_ != map) return 0;
			lX = m_pNpcList[iOwnerH]->m_sX;
			lY = m_pNpcList[iOwnerH]->m_sY;
			break;
	}
	if (cOwnerSkill == 0) return 0;
	iResult = iDice(1, 105);
	if (cOwnerSkill <= iResult) return 0;
	if (map->bGetIsWater(dX, dY) == false) return 0;
	if (cOwnerType == DEF_OWNERTYPE_PLAYER)
		m_pClientList[iOwnerH]->CalculateSSN_SkillIndex(iSkillNum, 1);
	switch (m_pSkillConfigList[iSkillNum]->m_sType) {
		case DEF_SKILLEFFECTTYPE_TAMING:
			m_pClientList[iOwnerH]->_TamingHandler(iSkillNum, map, dX, dY);
			break;
		case DEF_SKILLEFFECTTYPE_GET:
			std::memset(cItemName, 0, sizeof (cItemName));
			switch (m_pSkillConfigList[iSkillNum]->m_sValue1) {
				case 1:
					// Â±Â¤Â¹Â°
					wsprintf(cItemName, "Meat");
					break;
				case 2:
					//if (map->bGetIsWater(dX, dY) == false) return 0;
					if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
						iFish = m_pClientList[iOwnerH]->iCheckFish(map, dX, dY);
						if (iFish == 0) wsprintf(cItemName, "Fish");
					} else wsprintf(cItemName, "Fish");
					break;
			}
			if (strlen(cItemName) != 0) {
				if (memcmp(cItemName, "Fish", 6) == 0) {
					m_pClientList[iOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_FISHSUCCESS, 0, 0, 0, nullptr);
					m_pClientList[iOwnerH]->m_iExpStock += iDice(1, 2);
				}
				pItem = new class CItem;
				if (pItem == nullptr) return 0;
				if (_bInitItemAttr(*pItem, cItemName) == true) {
					map->bSetItem(lX, lY, pItem);
					map->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  lX, lY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4
				}
			}
			break;
	}
	return 1;
}

void CGame::bSetNpcAttackMode(char * cName, int iTargetH, char cTargetType, bool bIsPermAttack) {
	register int i, iIndex;
	for (i = 1; i < DEF_MAXNPCS; i++)
		if ((m_pNpcList[i] != nullptr) && (memcmp(m_pNpcList[i]->m_cName, cName, 5) == 0)) {
			iIndex = i;
			goto NEXT_STEP_SNAM1;
			//testcode
			//PutLogList("bSetNpcAttackMode - Npc found");
		}
	return;
NEXT_STEP_SNAM1:
	;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[iTargetH] == nullptr) return;
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[iTargetH] == nullptr) return;
			break;
	}
	m_pNpcList[iIndex]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
	m_pNpcList[iIndex]->m_sBehaviorTurnCount = 0;
	m_pNpcList[iIndex]->m_iTargetIndex = iTargetH;
	m_pNpcList[iIndex]->m_cTargetType = cTargetType;
	m_pNpcList[iIndex]->m_bIsPermAttackMode = bIsPermAttack;
	//testcode
	//PutLogList("bSetNpcAttackMode - complete");
}

bool CGame::bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType) {
	int iResist;
	int iResult;
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return false;
			iResist = m_pClientList[sOwnerH]->m_cSkillMastery[23] + m_pClientList[sOwnerH]->m_iAddPR;
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return false;
			iResist = 0;
			break;
	}
	iResult = iDice(1, 100);
	if (iResult >= iResist)
		return false;
	if (cOwnerType == DEF_OWNERTYPE_PLAYER)
		m_pClientList[sOwnerH]->CalculateSSN_SkillIndex(23, 1);
	return true;
}

bool CGame::bCheckBadWord(char * pString) {
	char * cp;
	cp = pString;
	while (*cp != 0) {
		cp++;
	}
	return false;
}

void CGame::CheckDayOrNightMode() {
	SYSTEMTIME SysTime;
	char cPrevMode;
	int i;
	if (m_bManualTime) return;
	cPrevMode = m_cDayOrNight;
	GetLocalTime(&SysTime);
	if (SysTime.wMinute >= DEF_NIGHTTIME)
		m_cDayOrNight = 2;
	else m_cDayOrNight = 1;
	if (cPrevMode != m_cDayOrNight) {
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				if ((m_pClientList[i]->map_ != nullptr) &&
						  (m_pClientList[i]->map_->m_bIsFixedDayMode == false))
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_TIMECHANGE, m_cDayOrNight, 0, 0, nullptr);
			}
	}
}

bool CGame::bReadNotifyMsgListFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode;
	char seps[] = "=\t\n;";
	class CStrTok * pStrTok;
	int i;
	cReadMode = 0;
	m_iTotalNoticeMsg = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Notify Message list file not found!...");
		return false;
	} else {
		PutLogList("(!) Reading Notify Message list file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		//token = strtok( cp, seps );
		while (token != nullptr) {
			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
							if (m_pNoticeMsgList[i] == nullptr) {
								m_pNoticeMsgList[i] = new class CMsg;
								m_pNoticeMsgList[i]->bPut(0, token, strlen(token), 0, 0);
								m_iTotalNoticeMsg++;
								goto LNML_NEXTSTEP1;
							}
LNML_NEXTSTEP1:
						;
						cReadMode = 0;
						break;
				}
			} else {
				if (memcmp(token, "notify_msg", 10) == 0) cReadMode = 1;
			}
			token = pStrTok->pGet();
			//token = strtok( nullptr, seps );
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

void CGame::NoticeHandler() {
	char cTemp;
	char cBuffer[1000], cKey;
	uint32_t dwSize;
	uint32_t dwTime = timeGetTime();
	register int i;
	int iMsgIndex;
	int iTemp;
	if (m_iTotalNoticeMsg <= 1) return;
	if ((dwTime - m_dwNoticeTime) > DEF_NOTICETIME) {
		m_dwNoticeTime = dwTime;
		do {
			iMsgIndex = iDice(1, m_iTotalNoticeMsg) - 1;
		} while (iMsgIndex == m_iPrevSendNoticeMsg);
		m_iPrevSendNoticeMsg = iMsgIndex;
		std::memset(cBuffer, 0, sizeof (cBuffer));
		if (m_pNoticeMsgList[iMsgIndex] != nullptr) {
			m_pNoticeMsgList[iMsgIndex]->Get(&cTemp, cBuffer, &dwSize, &iTemp, &cKey);
		}
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if (m_pClientList[i] != nullptr) {
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cBuffer);
			}
	}
}

void CGame::ResponseSavePlayerDataReplyHandler(char * pData, uint32_t /*dwMsgSize*/) {
	char * cp, cCharName[11];
	register int i;
	std::memset(cCharName, 0, sizeof (cCharName));
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	memcpy(cCharName, cp, 10);
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) {
			if (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0) {
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_SERVERCHANGE, 0, 0, 0, nullptr);
			}
		}
}

int CGame::iGetExpLevel(int iExp) {
	register int i;
	for (i = 1; i < 1001; i++)
		if ((m_iLevelExpTable[i] <= iExp) && (m_iLevelExpTable[i + 1] > iExp)) return i;
	return 0;
}

int CGame::iCreateFish(MapPtr map, short sX, short sY, short sType, class CItem * pItem, int iDifficulty, uint32_t dwLastTime) {
	register int i, iDynamicHandle;
	if (map == nullptr) return 0;
	if (map->bGetIsWater(sX, sY) == false) return 0;
	for (i = 1; i < DEF_MAXFISHS; i++)
		if (m_pFish[i] == nullptr) {
			m_pFish[i] = new class CFish(map, sX, sY, sType, pItem, iDifficulty);
			if (m_pFish[i] == nullptr) return 0;
			switch (pItem->m_sIDnum) {
				case 101:
				case 102:
				case 103:
				case 570:
				case 571:
				case 572:
				case 573:
				case 574:
				case 575:
				case 576:
				case 577:
					iDynamicHandle = dynamicObjects_.iAddDynamicObjectList(i, 0, DynamicObjectType::FISH, map, sX, sY, dwLastTime);
					break;
				default:
					iDynamicHandle = dynamicObjects_.iAddDynamicObjectList(i, 0, DynamicObjectType::FISHOBJECT, map, sX, sY, dwLastTime);
					break;
			}
			if (iDynamicHandle == 0) {
				delete m_pFish[i];
				m_pFish[i] = nullptr;
				return 0;
			}
			m_pFish[i]->m_sDynamicObjectHandle = iDynamicHandle;
			map->m_iCurFish++;
			return i;
		}
	return 0;
}

bool CGame::bDeleteFish(int iHandle, int iDelMode) {
	register int i, iH;
	uint32_t dwTime;
	if (m_pFish[iHandle] == nullptr) return false;
	dwTime = timeGetTime();
	iH = m_pFish[iHandle]->m_sDynamicObjectHandle;
	auto &dynObjPtr = dynamicObjects_[iH];
	if (dynObjPtr) {
		auto &dynObj = *dynObjPtr;
		dynObj.map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, dynObj.m_sX, dynObj.m_sY, (short) dynObj.m_sType, iH, 0);
		dynObj.map_->SetDynamicObject(0, DynamicObjectType::_0, dynObj.m_sX, dynObj.m_sY, dwTime);
		dynObj.map_->m_iCurFish--;
		dynObjPtr.reset();
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) &&
				  (m_pClientList[i]->m_iAllocatedFish == iHandle)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_FISHCANCELED, iDelMode, 0, 0, nullptr);
			m_pClientList[i]->ClearSkillUsingStatus();
		}
	}
	delete m_pFish[iHandle];
	m_pFish[iHandle] = nullptr;
	return true;
}

void CGame::FishProcessor() {
	register int i, iSkillLevel, iResult, iChangeValue;
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) &&
				  (m_pClientList[i]->m_iAllocatedFish != 0)) {
			if (m_pFish[m_pClientList[i]->m_iAllocatedFish] == nullptr) break;
			iSkillLevel = m_pClientList[i]->m_cSkillMastery[1];
			iSkillLevel -= m_pFish[m_pClientList[i]->m_iAllocatedFish]->m_iDifficulty;
			if (iSkillLevel <= 0) iSkillLevel = 1;
			iChangeValue = iSkillLevel / 10;
			if (iChangeValue <= 0) iChangeValue = 1;
			iChangeValue = iDice(1, iChangeValue);
			iResult = iDice(1, 100);
			if (iSkillLevel > iResult) {
				m_pClientList[i]->m_iFishChance += iChangeValue;
				if (m_pClientList[i]->m_iFishChance > 99) m_pClientList[i]->m_iFishChance = 99;
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, 0, 0, nullptr);
			} else if (iSkillLevel < iResult) {
				m_pClientList[i]->m_iFishChance -= iChangeValue;
				if (m_pClientList[i]->m_iFishChance < 1) m_pClientList[i]->m_iFishChance = 1;
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, 0, 0, nullptr);
			}
		}
	}
}

void CGame::FishGenerator() {
	register int i;
	int iP;
	int tX;
	int tY;
	char cItemName[21];
	short sDifficulty;
	uint32_t dwLastTime;
	class CItem * pItem;
	for (i = 0; i < DEF_MAXMAPS; i++) {
		auto &mapPtr = m_pMapList[i];
		if ((iDice(1, 10) == 5) && (mapPtr != nullptr) &&
				  (mapPtr->m_iCurFish < mapPtr->m_iMaxFish)) {
			iP = iDice(1, mapPtr->m_iTotalFishPoint) - 1;
			if ((mapPtr->m_FishPointList[iP].x == -1) || (mapPtr->m_FishPointList[iP].y == -1)) break;
			tX = mapPtr->m_FishPointList[iP].x + (iDice(1, 3) - 2);
			tY = mapPtr->m_FishPointList[iP].y + (iDice(1, 3) - 2);
			pItem = new class CItem;
			if (pItem == nullptr) break;
			std::memset(cItemName, 0, sizeof (cItemName));
			switch (iDice(1, 9)) {
				case 1: strcpy(cItemName, "RedCarp");
					sDifficulty = iDice(1, 10) + 20;
					break;
				case 2: strcpy(cItemName, "GreenCarp");
					sDifficulty = iDice(1, 5) + 10;
					break;
				case 3: strcpy(cItemName, "GoldCarp");
					sDifficulty = iDice(1, 10) + 1;
					break;
				case 4: strcpy(cItemName, "CrucianCarp");
					sDifficulty = 1;
					break;
				case 5: strcpy(cItemName, "BlueSeaBream");
					sDifficulty = iDice(1, 15) + 1;
					break;
				case 6: strcpy(cItemName, "RedSeaBream");
					sDifficulty = iDice(1, 18) + 1;
					break;
				case 7: strcpy(cItemName, "Salmon");
					sDifficulty = iDice(1, 12) + 1;
					break;
				case 8: strcpy(cItemName, "GrayMullet");
					sDifficulty = iDice(1, 10) + 1;
					break;
				case 9:
					switch (iDice(1, 150)) {
						case 1:
						case 2:
						case 3:
							strcpy(cItemName, "PowerGreenPotion");
							sDifficulty = iDice(5, 4) + 30;
							break;
						case 10:
						case 11:
							strcpy(cItemName, "SuperPowerGreenPotion");
							sDifficulty = iDice(5, 4) + 50;
							break;
						case 20:
							strcpy(cItemName, "Dagger+2");
							sDifficulty = iDice(5, 4) + 30;
							break;
						case 30:
							strcpy(cItemName, "LongSword+2");
							sDifficulty = iDice(5, 4) + 40;
							break;
						case 40:
							strcpy(cItemName, "Scimitar+2");
							sDifficulty = iDice(5, 4) + 50;
							break;
						case 50:
							strcpy(cItemName, "Rapier+2");
							sDifficulty = iDice(5, 4) + 60;
							break;
						case 60:
							strcpy(cItemName, "Flameberge+2");
							sDifficulty = iDice(5, 4) + 60;
							break;
						case 70:
							strcpy(cItemName, "WarAxe+2");
							sDifficulty = iDice(5, 4) + 50;
							break;
						case 90:
							strcpy(cItemName, "Ruby");
							sDifficulty = iDice(5, 4) + 40;
							break;
						case 95:
							strcpy(cItemName, "Diamond");
							sDifficulty = iDice(5, 4) + 40;
							break;
					}
					break;
			}
			dwLastTime = (60000 * 10) + (iDice(1, 3) - 1)*(60000 * 10);
			if (_bInitItemAttr(*pItem, cItemName) == true) {
				iCreateFish(mapPtr, tX, tY, 1, pItem, sDifficulty, dwLastTime);
			} else {
				delete pItem;
				pItem = nullptr;
			}
		}
	}
}

void CGame::WhetherProcessor() {
	char cPrevMode;
	int i;
	int j;
	uint32_t dwTime;
	dwTime = timeGetTime();
	for (i = 0; i < DEF_MAXMAPS; i++) {
		auto &map = m_pMapList[i];
		if ((map != nullptr) && (map->m_bIsFixedDayMode == false)) {
			cPrevMode = map->m_cWhetherStatus;
			if (map->m_cWhetherStatus != 0) {
				if ((dwTime - map->m_dwWhetherStartTime) > map->m_dwWhetherLastTime)
					map->m_cWhetherStatus = 0;
			} else {
				if (iDice(1, 300) == 13) {
					map->m_cWhetherStatus = iDice(1, 3); //This looks better or else we only get snow :(
					//map->m_cWhetherStatus = iDice(1,3)+3; <- This original code looks fucked
					map->m_dwWhetherStartTime = dwTime;
					map->m_dwWhetherLastTime = 60000 * 3 + 60000 * iDice(1, 7);
				}
			}
			if (map->m_bIsSnowEnabled == true) {
				map->m_cWhetherStatus = iDice(1, 3) + 3;
				map->m_dwWhetherStartTime = dwTime;
				map->m_dwWhetherLastTime = 60000 * 3 + 60000 * iDice(1, 7);
			}
			if (cPrevMode != map->m_cWhetherStatus) {
				for (j = 1; j < DEF_MAXCLIENTS; j++)
					if ((m_pClientList[j] != nullptr) && (m_pClientList[j]->m_bIsInitComplete == true) && (m_pClientList[j]->map_ == map))
						m_pClientList[j]->SendNotifyMsg(0, DEF_NOTIFY_WHETHERCHANGE, map->m_cWhetherStatus, 0, 0, nullptr);
			}
		} //If
	} //for Loop
}

void CGame::FightzoneReserveProcessor() {/*
 register int i;
	PutLogList("CAN FIGHTZONE RESERVE!!");

	for (i=0 ; i < DEF_MAXFIGHTZONE ; i++ ) {
		if (m_iFightZoneReserve[i] == -1 ) continue ;
		m_iFightZoneReserve[i] = 0 ;
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++) {


		if((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iFightzoneNumber != 0))
				m_pClientList[i]->SendNotifyMsg(0,DEF_NOTIFY_FIGHTZONERESERVE, -2, 0, 0, nullptr);
	}*/
}

/*********************************************************************************************************************
 **  int CGame::iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus)										**
 **  description			:: checks for a weather bonus when magic is cast											**
 **  last updated		:: November 20, 2004; 10:34 PM; Hypnotoad													**
 **	return value		:: int																						**
 *********************************************************************************************************************/
int CGame::iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus) {
	int iWheatherBonus;
	iWheatherBonus = 0;
	switch (cWheatherStatus) {
		case 0: break;
		case 1:
		case 2:
		case 3:
			switch (sType) {
				case 10:
				case 37:
				case 43:
				case 51:
					iWheatherBonus = 1;
					break;
				case 20:
				case 30:
					iWheatherBonus = -1;
					break;
			}
			break;
	}
	return iWheatherBonus;
}

int CGame::iGetPlayerABSStatus(int iWhatH, int iRecvH) {
	int iRet;
	if (m_pClientList[iWhatH] == nullptr) return 0;
	if (m_pClientList[iRecvH] == nullptr) return 0;
	iRet = 0;
	/*if (m_pMapList[m_pClientList[iRecvH]->m_cMapIndex]->m_bIsFightZone == true &&
		m_pClientList[iWhatH]->m_iGuildRank != -1 && m_pClientList[iRecvH]->m_iGuildRank != -1 &&
		memcmp(m_pClientList[iWhatH]->m_cGuildName,m_pClientList[iRecvH]->m_cGuildName,20) != 0) {
			iRet = 8;
		}*/
	if (m_pClientList[iWhatH]->m_iPKCount != 0) {
		iRet = 8;
	}
	if (m_pClientList[iWhatH]->m_cSide != 0) {
		iRet = iRet | 4;
	}
	if (m_pClientList[iWhatH]->m_cSide == 1) {
		iRet = iRet | 2;
	}
	if (m_pClientList[iWhatH]->m_bIsPlayerCivil == true)
		iRet = iRet | 1;
	return iRet;
}

int CGame::iGetNpcRelationship(int iWhatH, int iRecvH) {
	int iRet;
	if (m_pClientList[iRecvH] == nullptr) return 0;
	if (m_pNpcList[iWhatH] == nullptr) return 0;
	iRet = 0;
	switch (m_pNpcList[iWhatH]->m_cSide) {
		case 10: iRet |= 8; //Fixed, Original 15
		case 1: iRet = (iRet | 4) | 2; //Fixed, Original 1
		case 2: iRet |= 4; //Fixed, Orignal 8
	}
	return iRet;
}

int CGame::iGetMapIndex(char * pMapName) {
	register int i, iMapIndex;
	char cTmpName[256];
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pMapName);
	iMapIndex = -1;
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) {
			if (memcmp(m_pMapList[i]->m_cName, pMapName, 10) == 0)
				iMapIndex = i;
		}
	return iMapIndex;
}

int CGame::_iForcePlayerDisconect(int iNum) {
	register int i, iCnt;
	iCnt = 0;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) {
			if (m_pClientList[i]->m_bIsInitComplete == true)
				m_pClientList[i]->DeleteClient(true, true);
			else m_pClientList[i]->DeleteClient(false, false);
			iCnt++;
			if (iCnt >= iNum) break;
		}
	return iCnt;
}

void CGame::SpecialEventHandler() {
	uint32_t dwTime;
	dwTime = timeGetTime();
	if ((dwTime - m_dwSpecialEventTime) < DEF_SPECIALEVENTTIME) return; // DEF_SPECIALEVENTTIME
	m_dwSpecialEventTime = dwTime;
	m_bIsSpecialEventTime = true;
	switch (iDice(1, 180)) {
		case 98: m_cSpecialEventType = 2;
			break;
		default: m_cSpecialEventType = 1;
			break;
	}
}

void CGame::ForceDisconnectAccount(char *pAccountName, uint16_t wCount) {
	register int i;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (memcmp(m_pClientList[i]->m_cAccountName, pAccountName, 10) == 0)) {
			wsprintf(G_cTxt, "<%d> Force disconnect account: CharName(%s) AccntName(%s) Count(%d)", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cAccountName, wCount);
			PutLogList(G_cTxt);
			//m_pClientList[i]->DeleteClient(true, true);
			//v1.4312
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_FORCEDISCONN, wCount, 0, 0, nullptr);
		}
}

bool CGame::bOnClose() {
	if (!m_bIsServerShutdowned)
		if (MessageBox(nullptr, "Player data not saved! Shutdown server now?", m_cServerName, MB_ICONEXCLAMATION | MB_YESNO) == IDYES) return true;
		else return false;
	else return true;
	return false;
}

bool CGame::_bDecodePortionConfigFileContents(char *pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iPortionConfigListIndex = 0;
	int iCraftingConfigListIndex = 0;
	class CStrTok * pStrTok;
	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1: // Potion's index
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format(1).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							if (m_pPortionConfigList[atoi(token)] != nullptr) {
								PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Duplicate portion number.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pPortionConfigList[atoi(token)] = new class CPortion;
							iPortionConfigListIndex = atoi(token);
							cReadModeB = 2;
							break;
						case 2: // Potion's name
							std::memset(m_pPortionConfigList[iPortionConfigListIndex]->m_cName, 0, sizeof (m_pPortionConfigList[iPortionConfigListIndex]->m_cName));
							memcpy(m_pPortionConfigList[iPortionConfigListIndex]->m_cName, token, strlen(token));
							cReadModeB = 3;
							break;
						default: // m_sArray[0~10]
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pPortionConfigList[iPortionConfigListIndex]->m_sArray[cReadModeB - 3] = atoi(token);
							cReadModeB++;
							break;
						case 14: // m_sArray[11]
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pPortionConfigList[iPortionConfigListIndex]->m_sArray[11] = atoi(token);
							cReadModeB = 15;
							break;
						case 15: // Mini Skill
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pPortionConfigList[iPortionConfigListIndex]->m_iSkillLimit = atoi(token);
							cReadModeB = 16;
							break;
						case 16:// Difficulty
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pPortionConfigList[iPortionConfigListIndex]->m_iDifficulty = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 2: // Crafting
					switch (cReadModeB) {
						case 1: //
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format(1).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							if (m_pCraftingConfigList[atoi(token)] != nullptr) {
								PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Duplicate crafting number.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pCraftingConfigList[atoi(token)] = new class CPortion;
							iCraftingConfigListIndex = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							std::memset(m_pCraftingConfigList[iCraftingConfigListIndex]->m_cName, 0, sizeof (m_pCraftingConfigList[iCraftingConfigListIndex]->m_cName));
							memcpy(m_pCraftingConfigList[iCraftingConfigListIndex]->m_cName, token, strlen(token));
							cReadModeB = 3;
							break;
						default: // m_sArray[0~10]
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pCraftingConfigList[iCraftingConfigListIndex]->m_sArray[cReadModeB - 3] = atoi(token);
							cReadModeB++;
							break;
						case 14: // m_sArray[11]
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pCraftingConfigList[iCraftingConfigListIndex]->m_sArray[11] = atoi(token);
							cReadModeB = 15;
							break;
						case 15: //
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pCraftingConfigList[iCraftingConfigListIndex]->m_iSkillLimit = atoi(token);
							cReadModeB = 16;
							break;
						case 16://
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pCraftingConfigList[iCraftingConfigListIndex]->m_iDifficulty = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				default:
					break;
			}
		} else {
			if (memcmp(token, "potion", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
			if (memcmp(token, "crafting", 8) == 0) {
				cReadModeA = 2;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! POTION configuration file contents error!");
		return false;
	}
	wsprintf(cTxt, "(!) POTION(Total:%d) configuration - success!", iPortionConfigListIndex);
	PutLogList(cTxt);
	return true;
}

bool CGame::bDeleteMineral(int iIndex) {
	int iDynamicIndex;
	uint32_t dwTime;
	dwTime = timeGetTime();
	if (m_pMineral[iIndex] == nullptr) return false;
	iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
	if (dynamicObjects_[iDynamicIndex] == nullptr) return false;
	dynamicObjects_[iDynamicIndex]->map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT,
			  dynamicObjects_[iDynamicIndex]->m_sX, dynamicObjects_[iDynamicIndex]->m_sY,
			  (short) dynamicObjects_[iDynamicIndex]->m_sType, iDynamicIndex, 0);
	dynamicObjects_[iDynamicIndex]->map_->SetDynamicObject(0, DynamicObjectType::_0, dynamicObjects_[iDynamicIndex]->m_sX, dynamicObjects_[iDynamicIndex]->m_sY, dwTime);
	m_pMineral[iIndex]->map_->SetTempMoveAllowedFlag(dynamicObjects_[iDynamicIndex]->m_sX, dynamicObjects_[iDynamicIndex]->m_sY, true);
	m_pMineral[iIndex]->map_->m_iCurMineral--;
	delete m_pMineral[iIndex];
	m_pMineral[iIndex] = nullptr;
	return true;
}

void CGame::MineralGenerator() {
	register int i, iP, tX, tY;
	for (i = 0; i < DEF_MAXMAPS; i++) {
		auto &map = m_pMapList[i];
		if ((iDice(1, 4) == 1) && (map != nullptr) &&
				  (map->m_bMineralGenerator == true) &&
				  (map->m_iCurMineral < map->m_iMaxMineral)) {
			iP = iDice(1, map->m_iTotalMineralPoint) - 1;
			if ((map->m_MineralPointList[iP].x == -1) || (map->m_MineralPointList[iP].y == -1)) break;
			tX = map->m_MineralPointList[iP].x;
			tY = map->m_MineralPointList[iP].y;
			map->iCreateMineral(tX, tY, map->m_cMineralGeneratorLevel);
		}
	}
}

static int ___iCAB5[] = {0, 0, 0, 1, 2};
static int ___iCAB6[] = {0, 0, 0, 0, 0};
static int ___iCAB7[] = {0, 0, 1, 2, 3};
static int ___iCAB8[] = {0, 0, 1, 3, 5};
static int ___iCAB9[] = {0, 0, 2, 4, 8};
static int ___iCAB10[] = {0, 0, 1, 2, 3};

int CGame::iGetComboAttackBonus(int iSkill, int iComboCount) {
	if (iComboCount <= 1) return 0;
	if (iComboCount > 6) return 0;
	switch (iSkill) {
		case 5:
			return ___iCAB5[iComboCount];
			break;
		case 6:
			return ___iCAB6[iComboCount];
			break;
		case 7:
			return ___iCAB7[iComboCount];
			break;
		case 8:
			return ___iCAB8[iComboCount];
			break;
		case 9:
			return ___iCAB9[iComboCount];
			break;
		case 10:
			return ___iCAB10[iComboCount];
			break;
		case 14:
			return ___iCAB6[iComboCount];
			break;
		case 21:
			return ___iCAB10[iComboCount];
			break;
	}
	return 0;
}

int CGame::_iComposeFlagStatusContents(char * pData) {
	SYSTEMTIME SysTime;
	char cTxt[120];
	int i;
	auto ml = middlelandMap_.lock();
	if (!ml) return 0;
	GetLocalTime(&SysTime);
	strcat(pData, "[FILE-DATE]\n\n");
	wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");
	//
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (ml->m_pOccupyFlag[i] != nullptr) {
			wsprintf(cTxt, "flag = %d %d %d %d", ml->m_pOccupyFlag[i]->m_cSide,
					  ml->m_pOccupyFlag[i]->m_sX,
					  ml->m_pOccupyFlag[i]->m_sY,
					  ml->m_pOccupyFlag[i]->m_iEKCount);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}
	strcat(pData, "\n\n");
	return strlen(pData);
}

bool CGame::_bDecodeOccupyFlagSaveFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token;
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iSide;
	class CStrTok * pStrTok;
	int dX;
	int dY;
	int iEKNum;
	int iTotalFlags;
	auto ml = middlelandMap_.lock();
	if (!ml) {
		PutLogList("(!) This game server does not managing Middleland Map. OcuupyFlag data ignored.");
		return false;
	}
	iTotalFlags = 0;
	pContents = new char[dwMsgSize + 10];
	ZeroMemory(pContents, dwMsgSize + 10);
	memcpy(pContents, pData, dwMsgSize - 9);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							// Side
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iSide = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							dX = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							dY = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
						{
							// EKNum
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iEKNum = atoi(token);
							if (ml->__bSetOccupyFlag(dX, dY, iSide, iEKNum, -1, true) == true) {
								iTotalFlags++;
							}
							cReadModeA = 0;
							cReadModeB = 0;
						}
							break;
					}
				default:
					break;
			}
		} else {
			if (memcmp(token, "flag", 4) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file contents error!");
		return false;
	}
	wsprintf(G_cTxt, "(!) OccupyFlag save file decoding success! Total(%d)", iTotalFlags);
	PutLogList(G_cTxt);
	return true;
}
//Hero Code by Zabuza

void CGame::_BWM_Command_Shutup(char *pData) {
	char *cp = (char *) (pData + 16);
	char cName[11];
	std::memset(cName, 0, sizeof (cName));
	memcpy(cName, cp, 10);
	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {
			if (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0) {
				m_pClientList[i]->m_iTimeLeft_ShutUp = 20 * 3 * 10;
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERSHUTUP, 20 * 3 * 10, 0, 0, cName);
				// Admin Log
				wsprintf(G_cTxt, "BWordMonitor shutup(%s %d)", cName, 20 * 3 * 10);
				PutAdminLogFileList(G_cTxt);
				return;
			}
		}
	}
}

bool CGame::_bDecodeQuestConfigFileContents(char * pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iQuestConfigListIndex = 0;
	class CStrTok * pStrTok;
	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							if (m_pQuestConfigList[atoi(token)] != nullptr) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Duplicate quest number.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[atoi(token)] = new class CQuest;
							iQuestConfigListIndex = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_cSide = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iType = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// TargetType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iTargetType = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// MaxCount
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxCount = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iFrom = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iMinLevel = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxLevel = atoi(token);
							cReadModeB = 9;
							break;
						case 9:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillNum = atoi(token);
							cReadModeB = 10;
							break;
						case 10:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillLevel = atoi(token);
							cReadModeB = 11;
							break;
						case 11:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iTimeLimit = atoi(token);
							cReadModeB = 12;
							break;
						case 12:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iAssignType = atoi(token);
							cReadModeB = 13;
							break;
						case 13:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[1] = atoi(token);
							cReadModeB = 14;
							break;
						case 14:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[1] = atoi(token);
							cReadModeB = 15;
							break;
						case 15:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[2] = atoi(token);
							cReadModeB = 16;
							break;
						case 16:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[2] = atoi(token);
							cReadModeB = 17;
							break;
						case 17:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[3] = atoi(token);
							cReadModeB = 18;
							break;
						case 18:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[3] = atoi(token);
							cReadModeB = 19;
							break;
						case 19:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iContribution = atoi(token);
							cReadModeB = 20;
							break;
						case 20:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iContributionLimit = atoi(token);
							cReadModeB = 21;
							break;
						case 21:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iResponseMode = atoi(token);
							cReadModeB = 22;
							break;
						case 22:
							std::memset(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, 0, sizeof (m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName));
							strcpy(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, token);
							cReadModeB = 23;
							break;
						case 23:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_sX = atoi(token);
							cReadModeB = 24;
							break;
						case 24:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_sY = atoi(token);
							cReadModeB = 25;
							break;
						case 25:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iRange = atoi(token);
							cReadModeB = 26;
							break;
						case 26:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iQuestID = atoi(token);
							cReadModeB = 27;
							break;
						case 27:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pQuestConfigList[iQuestConfigListIndex]->m_iReqContribution = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				default:
					break;
			}
		} else {
			if (memcmp(token, "quest", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file contents error!");
		return false;
	}
	wsprintf(cTxt, "(!) QUEST(Total:%d) configuration - success!", iQuestConfigListIndex);
	PutLogList(cTxt);
	return true;
}

void CGame::OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	UINT iTmp;
	register int iLogSockH, iRet;
	iTmp = (WM_ONLOGSOCKETEVENT + 1);
	iLogSockH = message - iTmp;
	if (m_pSubLogSock[iLogSockH] == nullptr) return;
	iRet = m_pSubLogSock[iLogSockH]->iOnSocketEvent(wParam, lParam);
	switch (iRet) {
		case DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE:
			/*
					//GayDave
					pDisplay = Display;
					wdap = (char *)pDisplay; memcpy(wdap, "D", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "a", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "v", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "e", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "i", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "s", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "a", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "f", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "u", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "c", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "k", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "i", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "n", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "g", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "t", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "h", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "i", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "e", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "f", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "-", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "S", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "e", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "r", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "v", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "e", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "r", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "b", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "y", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "l", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "i", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "l", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "m", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "a", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "g", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "e", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "/", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "Z", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "e", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "r", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "o", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "o", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "f", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, " ", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "H", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "B", 1); pDisplay++;
					wdap = (char *)pDisplay; memcpy(wdap, "x", 1); pDisplay++;
			 */
			//Dave is a fucking thief - Server by lilmage/Zero of HBx
			wsprintf(G_cTxt, "(!!!) Log Socket Connection Established Log#(%d) Address:%s  Port:%d", iLogSockH, m_cLogServerAddr, m_iLogServerPort);
			PutLogList(G_cTxt);
			//		wsprintf(G_cTxt, "(!!!) %s", Display);
			//		PutLogList(G_cTxt);
			//		wsprintf(G_cTxt, "(!!!) And the random person who did nothing at all: Axe");
			//		PutLogList(G_cTxt);
			m_bIsSubLogSockAvailable[iLogSockH] = true;
			m_iSubLogSockActiveCount++;
		case DEF_XSOCKEVENT_CONNECTIONESTABLISH:
			//		wsprintf(G_cTxt, "(!!!) Sub-log-socket(%d) connected.", iLogSockH);
			//		PutLogList(G_cTxt);
			break;
		case DEF_XSOCKEVENT_READCOMPLETE:
			OnSubLogRead(iLogSockH);
			break;
		case DEF_XSOCKEVENT_BLOCK:
			wsprintf(G_cTxt, "(!!!) Sub-log-socket(%d) BLOCKED!", iLogSockH);
			PutLogList(G_cTxt);
			break;
		case DEF_XSOCKEVENT_CONFIRMCODENOTMATCH:
		case DEF_XSOCKEVENT_MSGSIZETOOLARGE:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			delete m_pSubLogSock[iLogSockH];
			m_pSubLogSock[iLogSockH] = nullptr;
			m_bIsSubLogSockAvailable[iLogSockH] = false;
			m_iSubLogSockFailCount++;
			m_iSubLogSockActiveCount--;
			wsprintf(G_cTxt, "(!!!) Sub-log-socket(%d) connection lost!", iLogSockH);
			PutLogList(G_cTxt);
			PutLogFileList(G_cTxt);
			m_pSubLogSock[iLogSockH] = new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
			m_pSubLogSock[iLogSockH]->bConnect(m_cLogServerAddr, m_iLogServerPort, (WM_ONLOGSOCKETEVENT + iLogSockH + 1));
			m_pSubLogSock[iLogSockH]->bInitBufferSize(DEF_MSGBUFFERSIZE);
			wsprintf(G_cTxt, "(!!!) Try to reconnect sub-log-socket(%d)... Addr:%s  Port:%d", iLogSockH, m_cLogServerAddr, m_iLogServerPort);
			PutLogList(G_cTxt);
			break;
	}
}

void CGame::OnSubLogRead(int iIndex) {
	uint32_t dwMsgSize;
	char * pData, cKey;
	pData = m_pSubLogSock[iIndex]->pGetRcvDataPointer(&dwMsgSize, &cKey);
	if (bPutMsgQuene(DEF_MSGFROM_LOGSERVER, pData, dwMsgSize, 0, cKey) == false) {
		PutLogList("@@@@@@ CRITICAL ERROR in MsgQuene!!! @@@@@@");
	}
}

bool CGame::_bCheckSubLogSocketIndex() {
	int iCnt;
	bool bLoopFlag;
	m_iCurSubLogSockIndex++;
	if (m_iCurSubLogSockIndex >= DEF_MAXSUBLOGSOCK) m_iCurSubLogSockIndex = 0;
	iCnt = 0;
	bLoopFlag = false;
	while (!bLoopFlag) {
		if ((m_pSubLogSock[m_iCurSubLogSockIndex] != nullptr) && (m_bIsSubLogSockAvailable[m_iCurSubLogSockIndex]))
			bLoopFlag = true;
		else m_iCurSubLogSockIndex++;
		iCnt++;
		if (iCnt >= DEF_MAXSUBLOGSOCK) {
			if (!m_bOnExitProcess) {
				m_cShutDownCode = 3;
				m_bOnExitProcess = true;
				m_dwExitProcessTime = timeGetTime();
				PutLogList("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Log-server connection Lost)!!!");
			}
			return false;
		}
	}
	return true;
}

bool CGame::_bDecodeBuildItemConfigFileContents(char *pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int i;
	int iIndex = 0;
	class CStrTok * pStrTok;
	class CItem * pItem;
	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							std::memset(m_pBuildItemList[iIndex]->m_cName, 0, sizeof (m_pBuildItemList[iIndex]->m_cName));
							memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format(1).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemID[0] = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemCount[0] = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemValue[0] = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemID[1] = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemCount[1] = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemValue[1] = atoi(token);
							cReadModeB = 9;
							break;
						case 9:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemID[2] = atoi(token);
							cReadModeB = 10;
							break;
						case 10:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemCount[2] = atoi(token);
							cReadModeB = 11;
							break;
						case 11:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemValue[2] = atoi(token);
							cReadModeB = 12;
							break;
						case 12:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemID[3] = atoi(token);
							cReadModeB = 13;
							break;
						case 13:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemCount[3] = atoi(token);
							cReadModeB = 14;
							break;
						case 14:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemValue[3] = atoi(token);
							cReadModeB = 15;
							break;
						case 15:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemID[4] = atoi(token);
							cReadModeB = 16;
							break;
						case 16:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemCount[4] = atoi(token);
							cReadModeB = 17;
							break;
						case 17:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemValue[4] = atoi(token);
							cReadModeB = 18;
							break;
						case 18:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemID[5] = atoi(token);
							cReadModeB = 19;
							break;
						case 19:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemCount[5] = atoi(token);
							cReadModeB = 20;
							break;
						case 20:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaterialItemValue[5] = atoi(token);
							cReadModeB = 21;
							break;
						case 21:
							// m_iAverageValue
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iAverageValue = atoi(token);
							cReadModeB = 22;
							break;
						case 22:
							// m_iMaxSkill
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);
							cReadModeB = 23;
							break;
						case 23:
							// m_wAttribute
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pBuildItemList[iIndex]->m_wAttribute = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							pItem = new class CItem;
							if (_bInitItemAttr(*pItem, m_pBuildItemList[iIndex]->m_cName) == true) {
								m_pBuildItemList[iIndex]->m_sItemID = pItem->m_sIDnum;
								for (i = 0; i < 6; i++)
									m_pBuildItemList[iIndex]->m_iMaxValue += (m_pBuildItemList[iIndex]->m_iMaterialItemValue[i]*100);
								iIndex++;
							} else {
								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! BuildItem configuration file error - Not Existing Item(%s)", m_pBuildItemList[iIndex]->m_cName);
								PutLogList(G_cTxt);
								delete m_pBuildItemList[iIndex];
								m_pBuildItemList[iIndex] = nullptr;
								delete pContents;
								delete pStrTok;
								return false;
							}
							delete pItem;
							break;
					}
					break;
				default:
					break;
			}
		} else {
			if (memcmp(token, "BuildItem", 9) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
				m_pBuildItemList[iIndex] = new class CBuildItem;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file contents error!");
		return false;
	}
	wsprintf(cTxt, "(!) BuildItem(Total:%d) configuration - success!", iIndex);
	PutLogList(cTxt);
	return true;
}

SpecialAbility CGame::_cGetSpecialAbility(int iKindSA) {
	SpecialAbility cSA = SpecialAbility::NONE;
	switch (iKindSA) {
		case 1:
			// Slime, Orc, Orge, WereWolf, YB-, Rabbit, Mountain-Giant, Stalker, Hellclaw,
			// Wyvern, Fire-Wyvern, Barlog, Tentocle, Centaurus, Giant-Lizard, Minotaurus,
			// Abaddon, Claw-Turtle, Giant-Cray-Fish, Giant-Plant, MasterMage-Orc, Nizie,
			// Tigerworm
			switch (iDice(1, 2)) {
				case 1: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 2: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
			}
			break;
		case 2:
			// Giant-Ant, Cat, Giant-Frog,
			switch (iDice(1, 3)) {
				case 1: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 2: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
				case 3: cSA = SpecialAbility::POISONOUS;
					break; // Poisonous
			}
			break;
		case 3:
			// Zombie, Scorpion, Amphis, Troll, Dark-Elf
			switch (iDice(1, 4)) {
				case 1: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 2: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
				case 3: cSA = SpecialAbility::POISONOUS;
					break; // Poisonous
				case 4: cSA = SpecialAbility::EXTREMELY_POISONOUS;
					break; // Critical Poisonous
			}
			break;
		case 4:
			// no linked Npc
			switch (iDice(1, 3)) {
				case 1: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 2: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
				case 3: cSA = SpecialAbility::EXPLOSIVE;
					break; // Explosive
			}
			break;
		case 5:
			// Stone-Golem, Clay-Golem, Beholder, Cannibal-Plant, Rudolph, DireBoar
			switch (iDice(1, 4)) {
				case 1: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 2: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
				case 3: cSA = SpecialAbility::EXPLOSIVE;
					break; // Explosive
				case 4: cSA = SpecialAbility::HIGHLY_EXPLOSIVE;
					break; // Critical-Explosive
			}
			break;
		case 6:
			// no linked Npc
			switch (iDice(1, 3)) {
				case 1: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 2: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
				case 3: cSA = SpecialAbility::POISONOUS;
					break; // Poisonous
			}
			break;
		case 7:
			// Orc-Mage, Unicorn
			switch (iDice(1, 3)) {
				case 1: cSA = SpecialAbility::CLAIRVOYANT;
					break; // Clairvoyant
				case 2: cSA = SpecialAbility::ANTI_MAGIC_PROT;
					break; // Distruction of Magic Protection
				case 3: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
			}
			break;
		case 8:
			// Frost, Ice-Golem, Ettin, Gagoyle, Demon, Liche, Hellbound, Cyclops,
			// Skeleton
			switch (iDice(1, 5)) {
				case 1: cSA = SpecialAbility::CLAIRVOYANT;
					break; // Clairvoyant
				case 2: cSA = SpecialAbility::ANTI_MAGIC_PROT;
					break; // Distruction of Magic Protection
				case 3: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break; // Anti-Magic Damage
				case 4: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break; // Anti-Physical Damage
				case 5: cSA = SpecialAbility::HIGHLY_EXPLOSIVE;
					break; // Critical-Explosive
			}
			break;
		case 9:
			switch (iDice(1, 8)) {
				case 1: cSA = SpecialAbility::CLAIRVOYANT;
					break;
				case 2: cSA = SpecialAbility::ANTI_MAGIC_PROT;
					break;
				case 3: cSA = SpecialAbility::RESISTANT_PHYS_DMG;
					break;
				case 4: cSA = SpecialAbility::RESISTANT_MAG_DMG;
					break;
				case 5: cSA = SpecialAbility::POISONOUS;
					break;
				case 6: cSA = SpecialAbility::EXTREMELY_POISONOUS;
					break;
				case 7: cSA = SpecialAbility::EXPLOSIVE;
					break;
				case 8: cSA = SpecialAbility::HIGHLY_EXPLOSIVE;
					break;
			}
			break;
	}
	return cSA;
}

bool CGame::_bDecodeDupItemIDFileContents(char *pData, uint32_t dwMsgSize) {
	char * pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iIndex = 0;
	class CStrTok * pStrTok;
	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							if (m_pDupItemIDList[atoi(token)] != nullptr) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Duplicate magic number.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pDupItemIDList[atoi(token)] = new class CItem;
							iIndex = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							// m_sTouchEffectType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pDupItemIDList[iIndex]->m_sTouchEffectType = (TouchEffectType) atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							// m_sTouchEffectValue1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pDupItemIDList[iIndex]->m_sTouchEffectValue1 = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// m_sTouchEffectValue2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pDupItemIDList[iIndex]->m_sTouchEffectValue2 = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// m_sTouchEffectValue3
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pDupItemIDList[iIndex]->m_sTouchEffectValue3 = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							// m_wPrice
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							m_pDupItemIDList[iIndex]->m_wPrice = (uint16_t) atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							//testcode
							//wsprintf(G_cTxt, "(%d) %d %d %d %d", iIndex, m_pDupItemIDList[iIndex]->m_sTouchEffectType, m_pDupItemIDList[iIndex]->m_sTouchEffectValue1, m_pDupItemIDList[iIndex]->m_sTouchEffectValue2, m_pDupItemIDList[iIndex]->m_sTouchEffectValue3);
							//PutLogList(G_cTxt);
							break;
					}
					break;
				default:
					break;
			}
		} else {
			if (memcmp(token, "DupItemID", 9) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) ERROR! DupItemID configuration file contents error!");
		return false;
	}
	wsprintf(cTxt, "(!) DupItemID(Total:%d) configuration - success!", iIndex);
	PutLogList(cTxt);
	return true;
}

bool CGame::_bCheckDupItemID(CItem *pItem) {
	int i;
	for (i = 0; i < DEF_MAXDUPITEMID; i++)
		if (m_pDupItemIDList[i] != nullptr) {
			if ((pItem->m_sTouchEffectType == m_pDupItemIDList[i]->m_sTouchEffectType) &&
					  (pItem->m_sTouchEffectValue1 == m_pDupItemIDList[i]->m_sTouchEffectValue1) &&
					  (pItem->m_sTouchEffectValue2 == m_pDupItemIDList[i]->m_sTouchEffectValue2) &&
					  (pItem->m_sTouchEffectValue3 == m_pDupItemIDList[i]->m_sTouchEffectValue3)) {
				pItem->m_wPrice = m_pDupItemIDList[i]->m_wPrice;
				return true;
			}
		}
	return false;
}

void CGame::_AdjustRareItemValue(CItem &item) {
	uint32_t dwSWEType;
	uint32_t dwSWEValue;
	double dV1;
	double dV2;
	double dV3;
	if ((item.m_dwAttribute & 0x00F00000) != 0) {
		dwSWEType = (item.m_dwAttribute & 0x00F00000) >> 20;
		dwSWEValue = (item.m_dwAttribute & 0x000F0000) >> 16;
		switch (dwSWEType) {
			case 0: break;
			case 5:
				item.m_cSpeed--;
				if (item.m_cSpeed < 0) item.m_cSpeed = 0;
				break;
			case 6:
				dV2 = (double) item.m_wWeight;
				dV3 = (double) (dwSWEValue * 4);
				dV1 = (dV3 / 100.0f) * dV2;
				item.m_wWeight -= (int) dV1;
				if (item.m_wWeight < 1) item.m_wWeight = 1;
				break;
			case 8:
			case 9:
				dV2 = (double) item.m_wMaxLifeSpan;
				dV3 = (double) (dwSWEValue * 7);
				dV1 = (dV3 / 100.0f) * dV2;
				item.m_wMaxLifeSpan += (int) dV1;
				break;
		}
	}
}

void CGame::_bDecodeNoticementFileContents(char *pData, uint32_t /*dwMsgSize*/) {
	char * cp;
	cp = (pData);
	if (m_pNoticementData != nullptr) {
		delete m_pNoticementData;
		m_pNoticementData = nullptr;
	}
	m_pNoticementData = new char[strlen(cp) + 2];
	ZeroMemory(m_pNoticementData, strlen(cp) + 2);
	memcpy(m_pNoticementData, cp, strlen(cp));
	m_dwNoticementDataSize = strlen(cp);
	wsprintf(G_cTxt, "(!) Noticement Data Size: %d", m_dwNoticementDataSize);
	PutLogList(G_cTxt);
}

void CGame::RequestCheckAccountPasswordHandler(char *pData, uint32_t /*dwMsgSize*/) {
	int * ip, i, iLevel;
	char * cp, cAccountName[11], cAccountPassword[11];
	cp = (char *) (pData + 6);
	std::memset(cAccountName, 0, sizeof (cAccountName));
	std::memset(cAccountPassword, 0, sizeof (cAccountPassword));
	memcpy(cAccountName, cp, 10);
	cp += 10;
	memcpy(cAccountPassword, cp, 10);
	cp += 10;
	ip = (int *) cp;
	iLevel = *ip;
	cp += 4;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cAccountName, cAccountName) == 0)) {
			if ((strcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword) != 0) || (m_pClientList[i]->m_iLevel != iLevel)) {
				wsprintf(G_cTxt, "(TestLog) Error! Account(%s)-Level(%d) password(or level) mismatch! Disconnect.", cAccountName, iLevel);
				PutLogList(G_cTxt);
				m_pClientList[i]->DeleteClient(false, true);
				return;
			}
		}
}

void CGame::EnergySphereProcessor(bool bIsAdminCreate, CClient *clientPtr) {
	int i;
	int iNamingValue;
	int iCIndex;
	int iTemp;
	int pX;
	int pY;
	char cName_Internal[31], cWaypoint[31];
	auto ml = middlelandMap_.lock();
	if (!bIsAdminCreate) {
		if (ml == nullptr) return;
		if (iDice(1, 2000) != 123) return;
		if (m_pClientList.getTotalClients() < 500) return;
		if (ml->m_iCurEnergySphereGoalPointIndex >= 0) return;
		iCIndex = iDice(1, ml->m_iTotalEnergySphereCreationPoint);
		if (ml->m_stEnergySphereCreationList[iCIndex].cType == 0) return;
		SpecialAbility cSA = SpecialAbility::NONE;
		pX = ml->m_stEnergySphereCreationList[iCIndex].sX;
		pY = ml->m_stEnergySphereCreationList[iCIndex].sY;
		std::memset(cWaypoint, 0, sizeof (cWaypoint));
		iNamingValue = ml->iGetEmptyNamingValue();
		if (iNamingValue != -1) {
			//
			std::memset(cName_Internal, 0, sizeof (cName_Internal));
			wsprintf(cName_Internal, "XX%d", iNamingValue);
			cName_Internal[0] = '_';
			cName_Internal[1] = ml->id_ + 65;
			if ((ml->bCreateNewNpc("Energy-Sphere", cName_Internal, (rand() % 5), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false)) == false) {
				ml->SetNamingValueEmpty(iNamingValue);
				return;
			}
		}
		iTemp = iDice(1, ml->m_iTotalEnergySphereGoalPoint);
		if (ml->m_stEnergySphereGoalList[iTemp].cResult == 0) return;
		ml->m_iCurEnergySphereGoalPointIndex = iTemp;
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHERECREATED, pX, pY, 0, nullptr);
			}
		//testcode
		wsprintf(G_cTxt, "(!) Energy Sphere Created! (%d, %d)", pX, pY);
		PutLogList(G_cTxt);
		PutLogFileList(G_cTxt);
	} else {
		CClient &client = *clientPtr;
		if (client.map_->m_iCurEnergySphereGoalPointIndex >= 0) return;
		iCIndex = iDice(1, client.map_->m_iTotalEnergySphereCreationPoint);
		if (client.map_->m_stEnergySphereCreationList[iCIndex].cType == 0) return;
		SpecialAbility cSA = SpecialAbility::NONE;
		pX = client.map_->m_stEnergySphereCreationList[iCIndex].sX;
		pY = client.map_->m_stEnergySphereCreationList[iCIndex].sY;
		std::memset(cWaypoint, 0, sizeof (cWaypoint));
		iNamingValue = client.map_->iGetEmptyNamingValue();
		if (iNamingValue != -1) {
			//
			std::memset(cName_Internal, 0, sizeof (cName_Internal));
			wsprintf(cName_Internal, "XX%d", iNamingValue);
			cName_Internal[0] = '_';
			cName_Internal[1] = client.map_->id_ + 65;
			if ((client.map_->bCreateNewNpc("Energy-Sphere", cName_Internal, (rand() % 5), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false)) == false) {
				client.map_->SetNamingValueEmpty(iNamingValue);
				return;
			}
		}
		iTemp = iDice(1, client.map_->m_iTotalEnergySphereGoalPoint);
		if (client.map_->m_stEnergySphereGoalList[iTemp].cResult == 0) return;
		client.map_->m_iCurEnergySphereGoalPointIndex = iTemp;
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHERECREATED, pX, pY, 0, nullptr);
			}
		//testcode
		wsprintf(G_cTxt, "(!) Admin Energy Sphere Created! (%d, %d)", pX, pY);
		PutLogList(G_cTxt);
		PutLogFileList(G_cTxt);
	}
}

void CGame::UpdateMapSectorInfo() {
	int i;
	int ix;
	int iy;
	int iMaxNeutralActivity;
	int iMaxAresdenActivity;
	int iMaxElvineActivity;
	int iMaxMonsterActivity;
	int iMaxPlayerActivity;
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) {
			iMaxNeutralActivity = iMaxAresdenActivity = iMaxElvineActivity = iMaxMonsterActivity = iMaxPlayerActivity = 0;
			m_pMapList[i]->m_iMaxNx = m_pMapList[i]->m_iMaxNy = m_pMapList[i]->m_iMaxAx = m_pMapList[i]->m_iMaxAy = 0;
			m_pMapList[i]->m_iMaxEx = m_pMapList[i]->m_iMaxEy = m_pMapList[i]->m_iMaxMx = m_pMapList[i]->m_iMaxMy = 0;
			m_pMapList[i]->m_iMaxPx = m_pMapList[i]->m_iMaxPy = 0;
			for (ix = 0; ix < DEF_MAXSECTORS; ix++)
				for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
					if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iNeutralActivity > iMaxNeutralActivity) {
						iMaxNeutralActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iNeutralActivity;
						m_pMapList[i]->m_iMaxNx = ix;
						m_pMapList[i]->m_iMaxNy = iy;
					}
					if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iAresdenActivity > iMaxAresdenActivity) {
						iMaxAresdenActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iAresdenActivity;
						m_pMapList[i]->m_iMaxAx = ix;
						m_pMapList[i]->m_iMaxAy = iy;
					}
					if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iElvineActivity > iMaxElvineActivity) {
						iMaxElvineActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iElvineActivity;
						m_pMapList[i]->m_iMaxEx = ix;
						m_pMapList[i]->m_iMaxEy = iy;
					}
					if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iMonsterActivity > iMaxMonsterActivity) {
						iMaxMonsterActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iMonsterActivity;
						m_pMapList[i]->m_iMaxMx = ix;
						m_pMapList[i]->m_iMaxMy = iy;
					}
					if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iPlayerActivity > iMaxPlayerActivity) {
						iMaxPlayerActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iPlayerActivity;
						m_pMapList[i]->m_iMaxPx = ix;
						m_pMapList[i]->m_iMaxPy = iy;
					}
				}
			m_pMapList[i]->ClearTempSectorInfo();
			if (m_pMapList[i]->m_iMaxNx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxNx][m_pMapList[i]->m_iMaxNy].iNeutralActivity++;
			if (m_pMapList[i]->m_iMaxAx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxAx][m_pMapList[i]->m_iMaxAy].iAresdenActivity++;
			if (m_pMapList[i]->m_iMaxEx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxEx][m_pMapList[i]->m_iMaxEy].iElvineActivity++;
			if (m_pMapList[i]->m_iMaxMx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxMx][m_pMapList[i]->m_iMaxMy].iMonsterActivity++;
			if (m_pMapList[i]->m_iMaxPx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxPx][m_pMapList[i]->m_iMaxPy].iPlayerActivity++;
		}
}

void CGame::AgingMapSectorInfo() {
	int i;
	int ix;
	int iy;
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) {
			for (ix = 0; ix < DEF_MAXSECTORS; ix++)
				for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
					m_pMapList[i]->m_stSectorInfo[ix][iy].iNeutralActivity--;
					m_pMapList[i]->m_stSectorInfo[ix][iy].iAresdenActivity--;
					m_pMapList[i]->m_stSectorInfo[ix][iy].iElvineActivity--;
					m_pMapList[i]->m_stSectorInfo[ix][iy].iMonsterActivity--;
					m_pMapList[i]->m_stSectorInfo[ix][iy].iPlayerActivity--;
					if (m_pMapList[i]->m_stSectorInfo[ix][iy].iNeutralActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iNeutralActivity = 0;
					if (m_pMapList[i]->m_stSectorInfo[ix][iy].iAresdenActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iAresdenActivity = 0;
					if (m_pMapList[i]->m_stSectorInfo[ix][iy].iElvineActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iElvineActivity = 0;
					if (m_pMapList[i]->m_stSectorInfo[ix][iy].iMonsterActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iMonsterActivity = 0;
					if (m_pMapList[i]->m_stSectorInfo[ix][iy].iPlayerActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iPlayerActivity = 0;
				}
		}
}

void CGame::LocalStartCrusadeMode(uint32_t dwCrusadeGUID) {
	int i;
	if (m_bIsCrusadeMode) return;
	m_bIsCrusadeMode = true;
	m_iCrusadeWinnerSide = 0;
	if (dwCrusadeGUID != 0) {
		_CreateCrusadeGUID(dwCrusadeGUID, 0);
		m_dwCrusadeGUID = dwCrusadeGUID;
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
			m_pClientList[i]->m_iCrusadeDuty = 0;
			m_pClientList[i]->m_iConstructionPoint = 0;
			m_pClientList[i]->m_dwCrusadeGUID = m_dwCrusadeGUID;
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, m_pClientList[i]->m_iCrusadeDuty, 0, nullptr);
		}
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) m_pMapList[i]->RestoreStrikePoints();
	CreateCrusadeStructures();
	PutLogList("(!)Crusade Mode ON.");
	_bCrusadeLog(DEF_CRUSADELOG_STARTCRUSADE, 0, nullptr);
}

void CGame::LocalEndCrusadeMode(int iWinnerSide) {
	int i;
	wsprintf(G_cTxt, "LocalEndCrusadeMode(%d)", iWinnerSide);
	PutLogList(G_cTxt);
	if (!m_bIsCrusadeMode) return;
	m_bIsCrusadeMode = false;
	PutLogList("(!)Crusade Mode OFF.");
	RemoveCrusadeStructures();
	RemoveCrusadeNpcs();
	_CreateCrusadeGUID(m_dwCrusadeGUID, iWinnerSide);
	m_iCrusadeWinnerSide = iWinnerSide;
	m_iLastCrusadeWinner = iWinnerSide;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
			m_pClientList[i]->m_iCrusadeDuty = 0;
			m_pClientList[i]->m_iConstructionPoint = 0;
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, m_iCrusadeWinnerSide);
		}
	RemoveCrusadeRecallTime();
	if (iWinnerSide == 2) {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, 0, "Elvine Win!");
	} else if (iWinnerSide == 1) {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, 0, "Aresden Win!");
	} else {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, 0, "Drawn!");
	}
}

void CGame::SaveOccupyFlagData() {
	char * pData;
	int iSize;
	FILE * pFile;
	PutLogList("(!) Middleland OccupyFlag data saved.");
	pData = new char[1000000 + 1];
	if (pData == nullptr) return;
	ZeroMemory(pData, 1000000);
	iSize = _iComposeFlagStatusContents(pData);
	_mkdir("GameData");
	pFile = fopen("GameData\\OccupyFlag.txt", "wt");
	if (pFile == nullptr) return;
	fwrite(pData, 1, iSize, pFile);
	delete pData;
	fclose(pFile);
}

void CGame::CreateCrusadeStructures() {
	int i;
	int z;
	int tX;
	int tY;
	int iNamingValue;
	char cName[6], cNpcName[21], cNpcWayPoint[11];
	std::memset(cName, 0, sizeof (cName));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cNpcWayPoint, 0, sizeof (cNpcWayPoint));
	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
		if (m_stCrusadeStructures[i].cType != 0) {
			for (z = 0; z < DEF_MAXMAPS; z++)
				if ((m_pMapList[z] != nullptr) && (strcmp(m_pMapList[z]->m_cName, m_stCrusadeStructures[i].cMapName) == 0)) {
					iNamingValue = m_pMapList[z]->iGetEmptyNamingValue();
					if (iNamingValue == -1) {
					} else {
						wsprintf(cName, "XX%d", iNamingValue);
						cName[0] = '_';
						cName[1] = z + 65;
						switch (m_stCrusadeStructures[i].cType) {
							case 36:
								if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
									strcpy(cNpcName, "AGT-Aresden");
								else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
									strcpy(cNpcName, "AGT-Elvine");
								break;
							case 37:
								if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
									strcpy(cNpcName, "CGT-Aresden");
								else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
									strcpy(cNpcName, "CGT-Elvine");
								break;
							case 40:
								if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
									strcpy(cNpcName, "ESG-Aresden");
								else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
									strcpy(cNpcName, "ESG-Elvine");
								break;
							case 41:
								if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
									strcpy(cNpcName, "GMG-Aresden");
								else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
									strcpy(cNpcName, "GMG-Elvine");
								break;
							case 42:
								strcpy(cNpcName, "ManaStone");
								break;
							default:
								strcpy(cNpcName, m_pNpcArchetypes[m_stCrusadeStructures[i].cType]->m_cNpcName);
								break;
						}
						tX = (int) m_stCrusadeStructures[i].dX;
						tY = (int) m_stCrusadeStructures[i].dY;
						if (m_pMapList[z]->bCreateNewNpc(cNpcName, cName, 0, SpecialAbility::NONE, DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWayPoint, nullptr, 0, -1, false) == false) {
							m_pMapList[z]->SetNamingValueEmpty(iNamingValue);
						} else {
							wsprintf(G_cTxt, "(!) Creating Crusade Structure(%s) at %s(%d, %d)", cNpcName, m_stCrusadeStructures[i].cMapName, tX, tY);
							PutLogList(G_cTxt);
						}
					}
				}
		}
}

void CGame::RemoveCrusadeStructures() {
	register int i;
	for (i = 0; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {
			switch (m_pNpcList[i]->m_sType) {
				case 36:
				case 37:
				case 38:
				case 39:
				case 40:
				case 41:
				case 42:
					m_pNpcList[i]->DeleteNpc();
					break;
			}
		}
}


void CGame::SyncMiddlelandMapInfo() {
	int i;
	auto ml = middlelandMap_.lock();
	if (ml) {
		for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
			m_stMiddleCrusadeStructureInfo[i].cType = 0;
			m_stMiddleCrusadeStructureInfo[i].cSide = 0;
			m_stMiddleCrusadeStructureInfo[i].sX = 0;
			m_stMiddleCrusadeStructureInfo[i].sY = 0;
		}
		m_iTotalMiddleCrusadeStructures = ml->m_iTotalCrusadeStructures;
		for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++) {
			m_stMiddleCrusadeStructureInfo[i].cType = ml->m_stCrusadeStructureInfo[i].cType;
			m_stMiddleCrusadeStructureInfo[i].cSide = ml->m_stCrusadeStructureInfo[i].cSide;
			m_stMiddleCrusadeStructureInfo[i].sX = ml->m_stCrusadeStructureInfo[i].sX;
			m_stMiddleCrusadeStructureInfo[i].sY = ml->m_stCrusadeStructureInfo[i].sY;
		}
	}
}

void CGame::GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char * pMapName) {
	int i;
	int iIndex;
	uint32_t dwTemp;
	uint32_t dwTime;
	wsprintf(G_cTxt, "SetGuildConstructLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
	PutLogList(G_cTxt);
	dwTime = timeGetTime();
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
			ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName));
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
	PutLogList("(X) No more GuildConstLocuct Space! Replaced.");
	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX2 = dX;
	m_pGuildTeleportLoc[i].m_sDestY2 = dY;
	std::memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof (m_pGuildTeleportLoc[i].m_cDestMapName2));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
}

void CGame::GSM_ConstructionPoint(int iGuildGUID, int iPoint) {
	int i;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == iGuildGUID)) {
			m_pClientList[i]->m_iConstructionPoint += iPoint;
			m_pClientList[i]->m_iWarContribution += iPoint / 10;
			if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
				m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
			if (m_pClientList[i]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
				m_pClientList[i]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, nullptr);
			wsprintf(G_cTxt, "GSM_ConstructionPoint: %d %d", iGuildGUID, iPoint);
			PutLogList(G_cTxt);
			return;
		}
}

void CGame::_CreateCrusadeGUID(uint32_t dwCrusadeGUID, int iWinnerSide) {
	char * cp, cTxt[256], cFn[256], cTemp[1024];
	FILE * pFile;
	_mkdir("GameData");
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, "GameData");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, "CrusadeGUID.Txt");
	pFile = fopen(cFn, "wt");
	if (pFile == nullptr) {
		wsprintf(cTxt, "(!) Cannot create CrusadeGUID(%d) file", dwCrusadeGUID);
		PutLogList(cTxt);
	} else {
		std::memset(cTemp, 0, sizeof (cTemp));
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "CrusadeGUID = %d\n", dwCrusadeGUID);
		strcat(cTemp, cTxt);
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
		strcat(cTemp, cTxt);
		cp = (char *) cTemp;
		fwrite(cp, strlen(cp), 1, pFile);
		wsprintf(cTxt, "(O) CrusadeGUID(%d) file created", dwCrusadeGUID);
		PutLogList(cTxt);
	}
	if (pFile != nullptr) fclose(pFile);
}

bool CGame::bReadCrusadeGUIDFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	cReadMode = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open CrusadeGUID file.");
		return false;
	} else {
		PutLogList("(!) Reading CrusadeGUID file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						m_dwCrusadeGUID = atoi(token);
						wsprintf(G_cTxt, "CrusadeGUID = %d", m_dwCrusadeGUID);
						PutLogList(G_cTxt);
						cReadMode = 0;
						break;
					case 2:
						m_iLastCrusadeWinner = atoi(token);
						wsprintf(G_cTxt, "CrusadeWinnerSide = %d", m_iLastCrusadeWinner);
						PutLogList(G_cTxt);
						cReadMode = 0;
						break;
				}
			} else {
				if (memcmp(token, "CrusadeGUID", 11) == 0) cReadMode = 1;
				if (memcmp(token, "winner-side", 11) == 0) cReadMode = 2;
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

void CGame::ManualEndCrusadeMode(int iWinnerSide) {
	if (!m_bIsCrusadeMode) return;
	LocalEndCrusadeMode(iWinnerSide);
}

int CGame::iGetMapLocationSide(char *pMapName) {
	if (strcmp(pMapName, "aresden") == 0) return 3;
	if (strcmp(pMapName, "elvine") == 0) return 4;
	if (strcmp(pMapName, "arebrk11") == 0) return 3;
	if (strcmp(pMapName, "elvbrk11") == 0) return 4;
	if (strcmp(pMapName, "cityhall_1") == 0) return 1;
	if (strcmp(pMapName, "cityhall_2") == 0) return 2;
	if (strcmp(pMapName, "cath_1") == 0) return 1;
	if (strcmp(pMapName, "cath_2") == 0) return 2;
	if (strcmp(pMapName, "gshop_1") == 0) return 1;
	if (strcmp(pMapName, "gshop_2") == 0) return 2;
	if (strcmp(pMapName, "bsmith_1") == 0) return 1;
	if (strcmp(pMapName, "bsmith_2") == 0) return 2;
	if (strcmp(pMapName, "wrhus_1") == 0) return 1;
	if (strcmp(pMapName, "wrhus_2") == 0) return 2;
	if (strcmp(pMapName, "gldhall_1") == 0) return 1;
	if (strcmp(pMapName, "gldhall_2") == 0) return 2;
	if (strcmp(pMapName, "wzdtwr_1") == 0) return 1;
	if (strcmp(pMapName, "wzdtwr_2") == 0) return 2;
	if (strcmp(pMapName, "arefarm") == 0) return 1;
	if (strcmp(pMapName, "elvfarm") == 0) return 2;
	if (strcmp(pMapName, "arewrhus") == 0) return 1;
	if (strcmp(pMapName, "elvwrhus") == 0) return 2;
	if (strcmp(pMapName, "cmdhall_1") == 0) return 1;
	if (strcmp(pMapName, "Cmdhall_2") == 0) return 2;
	return 0;
}

bool CGame::bCopyItemContents(CItem * pCopy, CItem *pOriginal) {
	if (pOriginal == nullptr) return false;
	if (pCopy == nullptr) return false;
	pCopy->m_sIDnum = pOriginal->m_sIDnum;
	pCopy->m_cItemType = pOriginal->m_cItemType;
	pCopy->m_cEquipPos = pOriginal->m_cEquipPos;
	pCopy->m_sItemEffectType = pOriginal->m_sItemEffectType;
	pCopy->m_sItemEffectValue1 = pOriginal->m_sItemEffectValue1;
	pCopy->m_sItemEffectValue2 = pOriginal->m_sItemEffectValue2;
	pCopy->m_sItemEffectValue3 = pOriginal->m_sItemEffectValue3;
	pCopy->m_sItemEffectValue4 = pOriginal->m_sItemEffectValue4;
	pCopy->m_sItemEffectValue5 = pOriginal->m_sItemEffectValue5;
	pCopy->m_sItemEffectValue6 = pOriginal->m_sItemEffectValue6;
	pCopy->m_wMaxLifeSpan = pOriginal->m_wMaxLifeSpan;
	pCopy->m_sSpecialEffect = pOriginal->m_sSpecialEffect;
	//short m_sSM_HitRatio, m_sL_HitRatio;
	pCopy->m_sSpecialEffectValue1 = pOriginal->m_sSpecialEffectValue1;
	pCopy->m_sSpecialEffectValue2 = pOriginal->m_sSpecialEffectValue2;
	pCopy->m_sSprite = pOriginal->m_sSprite;
	pCopy->m_sSpriteFrame = pOriginal->m_sSpriteFrame;
	pCopy->m_cApprValue = pOriginal->m_cApprValue;
	pCopy->m_cSpeed = pOriginal->m_cSpeed;
	pCopy->m_wPrice = pOriginal->m_wPrice;
	pCopy->m_wWeight = pOriginal->m_wWeight;
	pCopy->m_sLevelLimit = pOriginal->m_sLevelLimit;
	pCopy->m_cGenderLimit = pOriginal->m_cGenderLimit;
	pCopy->m_sRelatedSkill = pOriginal->m_sRelatedSkill;
	pCopy->m_cCategory = pOriginal->m_cCategory;
	pCopy->m_bIsForSale = pOriginal->m_bIsForSale;
	//
	pCopy->m_dwCount = pOriginal->m_dwCount;
	pCopy->m_sTouchEffectType = pOriginal->m_sTouchEffectType;
	pCopy->m_sTouchEffectValue1 = pOriginal->m_sTouchEffectValue1;
	pCopy->m_sTouchEffectValue2 = pOriginal->m_sTouchEffectValue2;
	pCopy->m_sTouchEffectValue3 = pOriginal->m_sTouchEffectValue3;
	pCopy->m_cItemColor = pOriginal->m_cItemColor;
	pCopy->m_sItemSpecEffectValue1 = pOriginal->m_sItemSpecEffectValue1;
	pCopy->m_sItemSpecEffectValue2 = pOriginal->m_sItemSpecEffectValue2;
	pCopy->m_sItemSpecEffectValue3 = pOriginal->m_sItemSpecEffectValue3;
	pCopy->m_wCurLifeSpan = pOriginal->m_wCurLifeSpan;
	pCopy->m_dwAttribute = pOriginal->m_dwAttribute;
	return true;
}

void CGame::SendMsg(short sOwnerH, char /*cOwnerType*/, bool /*bStatus*/, long lPass) {
	if (m_pClientList[sOwnerH] == nullptr) return;
	//m_pClientList[sOwnerH]->SendNotifyMsg(0,DEF_NOTIFY_LEVELUP, 0, 0, 0, nullptr);
	m_pClientList[sOwnerH]->SendNotifyMsg(0, lPass, 0, 0, 0, nullptr);
}

/*********************************************************************************************************************
 **  void CGame::SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
 **  description			:: changes the status of the player to show invisibility aura								**
 **  last updated		:: November 20, 2004; 9:30 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000010;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000010;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void CGame::SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus)								**
 **  description			:: changes the status of the player to show inhibit casting aura							**
 **  last updated		:: November 20, 2004; 9:33 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00100000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00100000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
 **  description			:: changes the status of the player to show berserk aura									**
 **  last updated		:: November 20, 2004; 9:35 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000020;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000020;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus)										**
 **  description			:: changes the status of the player to show frozen aura										**
 **  last updated		:: November 20, 2004; 9:35 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000040;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000040;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
 **  description			:: changes the status of the player to show poison aura										**
 **  last updated		:: November 20, 2004; 9:36 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000080;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000080;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
 **  description			:: changes the status of the player to show illusion aura									**
 **  last updated		:: November 20, 2004; 9:36 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x01000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x01000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus)										**
 **  description			:: changes the status of the player to show hero item aura									**
 **  last updated		:: November 20, 2004; 9:37 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00020000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00020000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus)								**
 **  description			:: changes the status of the player to show defense aura									**
 **  last updated		:: November 20, 2004; 9:37 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x02000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x02000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus)							**
 **  description			:: changes the status of the player to show magic protect aura								**
 **  last updated		:: November 20, 2004; 9:38 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x04000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x04000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus)						**
 **  description			:: changes the status of the player to show arrow protect aura								**
 **  last updated		:: November 20, 2004; 9:39 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x08000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x08000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
			m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}

/*********************************************************************************************************************
 **  void void CGame::SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus)							**
 **  description			:: changes the status of the player to show illusion movement aura							**
 **  last updated		:: November 20, 2004; 9:39 PM; Hypnotoad													**
 **	return value		:: void																						**
 *********************************************************************************************************************/
void CGame::SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus) {
	switch (cOwnerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sOwnerH] == nullptr) return;
			if (bStatus)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00200000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFDFFFFF;
			m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}
// New 07/05/2004


bool CGame::_bItemLog(int iAction, int iGiveH, int iRecvH, class CItem * pItem, bool /*bForceItemLog*/) {
	char cTxt[1024], cTemp1[120], cTemp2[120];
	int iItemCount;
	if (pItem == nullptr) return false;
	if (m_pClientList[iGiveH]->m_cCharName == nullptr) return false;
	if (iAction == DEF_ITEMLOG_DUPITEMID) {
		if (m_pClientList[iGiveH] == nullptr) return false;
		if (m_pClientList[iGiveH]->m_cCharName == nullptr) return false;
		wsprintf(G_cTxt, "(!) Delete-DupItem(%s %d %d %d %d) Owner(%s)", pItem->m_cName, pItem->m_dwCount, pItem->m_sTouchEffectValue1,
				  pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3,
				  m_pClientList[iGiveH]->m_cCharName);
		//	PutItemLogFileList(iGiveH,G_cTxt);
		m_pClientList[iGiveH]->bSendMsgToLS(MSGID_GAMEITEMLOG, 0, G_cTxt);
		return true;
	}
	std::memset(cTxt, 0, sizeof (cTxt));
	std::memset(cTemp1, 0, sizeof (cTemp1));
	std::memset(cTemp2, 0, sizeof (cTemp2));
	switch (iAction) {
		case DEF_ITEMLOG_EXCHANGE:
			if (m_pClientList[iRecvH]->m_cCharName == nullptr) return false;
			wsprintf(cTxt, "(%s) PC(%s)\tExchange\t%s(%d %d %d %d %x)\t%s(%d %d)\tPC(%s)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY, m_pClientList[iRecvH]->m_cCharName);
			break;
		case DEF_ITEMLOG_GIVE:
			if (m_pClientList[iRecvH]->m_cCharName == nullptr) return false;
			wsprintf(cTxt, "(%s) PC(%s)\tGive\t%s(%d %d %d %d %x)\t%s(%d %d)\tPC(%s)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY, m_pClientList[iRecvH]->m_cCharName);
			break;
		case DEF_ITEMLOG_BUY:
			iItemCount = iRecvH;
			wsprintf(cTxt, "(%s) PC(%s)\tBuy\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, iItemCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		default:
			return false;
	}
	m_pClientList[iGiveH]->bSendMsgToLS(MSGID_GAMEITEMLOG, 0, cTxt);
	return true;
}

bool CGame::_bCheckGoodItem(class CItem * pItem) {
	if (pItem == nullptr) return false;
	if (pItem->m_sIDnum == 90) {
		if (pItem->m_dwCount > 10000) return true;
		else return false;
	}
	switch (pItem->m_sIDnum) {
		case 259:
		case 290:
		case 291:
		case 292:
		case 300:
		case 305:
		case 308:
		case 311:
		case 334:
		case 335:
		case 336:
		case 338:
		case 380:
		case 381:
		case 382:
		case 391:
		case 400:
		case 401:
		case 490:
		case 491:
		case 492:
		case 508:
		case 581:
		case 610:
		case 611:
		case 612:
		case 613:
		case 614:
		case 616:
		case 618:
		case 620:
		case 621:
		case 622:
		case 623:
		case 630:
		case 631:
		case 632:
		case 633:
		case 634:
		case 635:
		case 636:
		case 637:
		case 638:
		case 639:
		case 640:
		case 641:
		case 642:
		case 643:
		case 644:
		case 645:
		case 646:
		case 647:
		case 650:
		case 654:
		case 655:
		case 656:
		case 657:
		case 700:
		case 701:
		case 702:
		case 703:
		case 704:
		case 705:
		case 706:
		case 707:
		case 708:
		case 709:
		case 710:
		case 711:
		case 712:
		case 713:
		case 714:
		case 715:
		case 720:
		case 721:
		case 722:
		case 723:
		case 724:
		case 725:
		case 726:
		case 727:
		case 728:
		case 729:
		case 730:
		case 731:
		case 732:
		case 733:
		case 734:
		case 735:
		case 736:
		case 737:
		case 738:
			return true;
			break;
		default:
			if ((pItem->m_dwAttribute & 0xF0F0F001) == 0) return false;
			else if (pItem->m_sIDnum > 30) return true;
			else return false;
	}
}

void CGame::RemoveCrusadeNpcs(void) {
	for (int i = 0; i < DEF_MAXNPCS; i++) {
		if (m_pNpcList[i] != nullptr) {
			if ((m_pNpcList[i]->m_sType >= 43 && m_pNpcList[i]->m_sType <= 47) || m_pNpcList[i]->m_sType == 51) {
				m_pNpcList[i]->NpcKilledHandler(0, 0, 0);
			}
		}
	}
}

void CGame::RemoveCrusadeRecallTime(void) {
	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {
			if (m_pClientList[i]->m_bIsWarLocation == true &&
					  m_pClientList[i]->m_bIsPlayerCivil == true &&
					  m_pClientList[i]->m_bIsInitComplete == true) {
				m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
			}
		}
	}
}
// New 13/05/2004

bool CGame::_bInitItemAttr(CItem &item, int iItemID) {
	register int i;
	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != nullptr) {
			if (m_pItemConfigList[i]->m_sIDnum == iItemID) {
				std::memset(item.m_cName, 0, sizeof (item.m_cName));
				strcpy(item.m_cName, m_pItemConfigList[i]->m_cName);
				item.m_cItemType = m_pItemConfigList[i]->m_cItemType;
				item.m_cEquipPos = m_pItemConfigList[i]->m_cEquipPos;
				item.m_sItemEffectType = m_pItemConfigList[i]->m_sItemEffectType;
				item.m_sItemEffectValue1 = m_pItemConfigList[i]->m_sItemEffectValue1;
				item.m_sItemEffectValue2 = m_pItemConfigList[i]->m_sItemEffectValue2;
				item.m_sItemEffectValue3 = m_pItemConfigList[i]->m_sItemEffectValue3;
				item.m_sItemEffectValue4 = m_pItemConfigList[i]->m_sItemEffectValue4;
				item.m_sItemEffectValue5 = m_pItemConfigList[i]->m_sItemEffectValue5;
				item.m_sItemEffectValue6 = m_pItemConfigList[i]->m_sItemEffectValue6;
				item.m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
				item.m_wCurLifeSpan = item.m_wMaxLifeSpan;
				item.m_sSpecialEffect = m_pItemConfigList[i]->m_sSpecialEffect;
				item.m_sSprite = m_pItemConfigList[i]->m_sSprite;
				item.m_sSpriteFrame = m_pItemConfigList[i]->m_sSpriteFrame;
				item.m_wPrice = m_pItemConfigList[i]->m_wPrice;
				item.m_wWeight = m_pItemConfigList[i]->m_wWeight;
				item.m_cApprValue = m_pItemConfigList[i]->m_cApprValue;
				item.m_cSpeed = m_pItemConfigList[i]->m_cSpeed;
				item.m_sLevelLimit = m_pItemConfigList[i]->m_sLevelLimit;
				item.m_cGenderLimit = m_pItemConfigList[i]->m_cGenderLimit;
				item.m_sSpecialEffectValue1 = m_pItemConfigList[i]->m_sSpecialEffectValue1;
				item.m_sSpecialEffectValue2 = m_pItemConfigList[i]->m_sSpecialEffectValue2;
				item.m_sRelatedSkill = m_pItemConfigList[i]->m_sRelatedSkill;
				item.m_cCategory = m_pItemConfigList[i]->m_cCategory;
				item.m_sIDnum = m_pItemConfigList[i]->m_sIDnum;
				item.m_bIsForSale = m_pItemConfigList[i]->m_bIsForSale;
				item.m_cItemColor = m_pItemConfigList[i]->m_cItemColor;
				return true;
			}
		}
	return false;
}
// New 16/05/2004

// v2.15 2002-5-21

void CGame::GSM_RequestShutupPlayer(char * /*pGMName*/, uint16_t /*wReqServerID*/, uint16_t /*wReqClientH*/, uint16_t wTime, char * pPlayer) {
	register int i;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cCharName, pPlayer) == 0)) {
			m_pClientList[i]->m_iTimeLeft_ShutUp = wTime * 20;
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERSHUTUP, wTime, 0, 0, pPlayer);
			return;
		}
}
// v2.14 05/22/2004 - Hypnotoad - adds pk log

bool CGame::_bPKLog(int iAction, int iAttackerH, int iVictumH, char * pNPC) {
	char cTxt[1024], cTemp1[120], cTemp2[120];
	std::memset(cTxt, 0, sizeof (cTxt));
	std::memset(cTemp1, 0, sizeof (cTemp1));
	std::memset(cTemp2, 0, sizeof (cTemp2));
	if (m_pClientList[iVictumH] == nullptr) return false;
	switch (iAction) {
		case DEF_PKLOG_REDUCECRIMINAL:
			wsprintf(cTxt, "(%s) PC(%s)\tReduce\tCC(%d)\t%s(%d %d)\t", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName, m_pClientList[iVictumH]->m_iPKCount,
					  m_pClientList[iVictumH]->map_->m_cName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY);
			break;
		case DEF_PKLOG_BYPLAYER:
			if (m_pClientList[iAttackerH] == nullptr) return false;
			wsprintf(cTxt, "(%s) PC(%s)\tKilled by PC\t \t%s(%d %d)\t(%s) PC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
					  m_pClientList[iVictumH]->map_->m_cName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, m_pClientList[iAttackerH]->m_cIPaddress, m_pClientList[iAttackerH]->m_cCharName);
			break;
		case DEF_PKLOG_BYPK:
			if (m_pClientList[iAttackerH] == nullptr) return false;
			wsprintf(cTxt, "(%s) PC(%s)\tKilled by PK\tCC(%d)\t%s(%d %d)\t(%s) PC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName, m_pClientList[iAttackerH]->m_iPKCount,
					  m_pClientList[iVictumH]->map_->m_cName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, m_pClientList[iAttackerH]->m_cIPaddress, m_pClientList[iAttackerH]->m_cCharName);
			break;
		case DEF_PKLOG_BYENERMY:
			if (m_pClientList[iAttackerH] == nullptr) return false;
			wsprintf(cTxt, "(%s) PC(%s)\tKilled by Enemy\t \t%s(%d %d)\t(%s) PC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
					  m_pClientList[iVictumH]->map_->m_cName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, m_pClientList[iAttackerH]->m_cIPaddress, m_pClientList[iAttackerH]->m_cCharName);
			break;
		case DEF_PKLOG_BYNPC:
			if (pNPC == nullptr) return false;
			wsprintf(cTxt, "(%s) PC(%s)\tKilled by NPC\t \t%s(%d %d)\tNPC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
					  m_pClientList[iVictumH]->map_->m_cName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, pNPC);
			break;
		case DEF_PKLOG_BYOTHER:
			wsprintf(cTxt, "(%s) PC(%s)\tKilled by Other\t \t%s(%d %d)\tUnknown", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
					  m_pClientList[iVictumH]->map_->m_cName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY);
			break;
		default:
			return false;
	}
	PutPvPLogFileList(cTxt);
	return true;
}

/*
bool CGame::bCheckClientInvisibility(short iClientH)
{
	bool bFlag;
	int iShortCutIndex;
	int i;
	short sRange;
	if(client.markedForDeletion_) return false;
	if (client.m_iAdminUserLevel > 0) return false;
	bFlag = true;
	iShortCutIndex = 0;
		while(bFlag){
Raven-HBRND(www.hbrnd.com) says:
what i suggest
is .. if its requesting
to see the other side whyle invsie
check the bag
for the neckie
			i = m_iClientShortCut[iShortCutIndex];
			iShortCutIndex++;
			if (i == 0) bFlag = false;
			if ((bFlag) && (m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				if ( (m_pClientList[i]->m_cMapIndex == client.m_cMapIndex) &&
					  (m_pClientList[i]->m_sX >= client.m_sX - 10 - sRange) &&
					 (m_pClientList[i]->m_sX <= client.m_sX + 10 + sRange) &&
					 (m_pClientList[i]->m_sY >= client.m_sY - 8 - sRange) &&
					 (m_pClientList[i]->m_sY <= client.m_sY + 8 + sRange) ) {
			// iClient = center screen character
			// i = visable characters on screen
					if ( (client.m_cSide != m_pClientList[i]->m_cSide)
								&& (i != iClientH)
								&& (client.m_sAppr4 == client.m_sAppr4 & 0x0001)
								) {
						//wsprintf(G_cTxt, "(!) Invisibility Hack Suspect: (%s)", client.m_cCharName);
						//PutLogList(G_cTxt);
						//client.DeleteClient(true, true);
						}
					}
			}
		}
	return false;
}*/
void CGame::CrusadeWarStarter() {
	SYSTEMTIME SysTime;
	int i;
	if (m_bIsCrusadeMode) return;
	if (!m_bIsCrusadeWarStarter) return;
	GetLocalTime(&SysTime);
	for (i = 0; i < DEF_MAXSCHEDULE; i++)
		if ((m_stCrusadeWarSchedule[i].iDay == SysTime.wDayOfWeek) &&
				  (m_stCrusadeWarSchedule[i].iHour == SysTime.wHour) &&
				  (m_stCrusadeWarSchedule[i].iMinute == SysTime.wMinute)) {
			PutLogList("(!) Automated crusade is being initiated!");
			GlobalStartCrusadeMode();
			return;
		}
}

void CGame::OnTimer(char /*cType*/) {
	int q; //added for "advertiser"
	char cAdvert[1000]; //holds the text for the "advert"
	std::memset(cAdvert, 0, sizeof (cAdvert));
	m_pNpcList.cull();
	m_pClientList.cull();
	uint32_t dwTime = timeGetTime();
	//if ((dwTime - m_dwGameTime1) > 200) {
	GameProcess();
	// m_dwGameTime1 = dwTime;
	//}
	if ((dwTime - m_dwGameTime2) > 3000) {
		CheckClientResponseTime();
		CheckDayOrNightMode();
		InvalidateRect(G_hWnd, nullptr, true);
		m_dwGameTime2 = dwTime;
		// v1.41
		if ((!m_bIsGameStarted) && (m_bIsItemAvailable) &&
				  (m_bIsNpcAvailable) &&
				  (m_bIsLogSockAvailable) && (m_bIsMagicAvailable) &&
				  (m_bIsSkillAvailable) && (m_bIsPortionAvailable) &&
				  (m_bIsQuestAvailable) && (m_bIsBuildItemAvailable) &&
				  (m_iSubLogSockActiveCount == DEF_MAXSUBLOGSOCK)) {
			PutLogList("Sending start message...");
			SendMessage(m_hWnd, WM_USER_STARTGAMESIGNAL, 0, 0);
			m_bIsGameStarted = true;
		}
	}
	if ((dwTime - m_dwGameTime6) > 1000) {
		delayEvents_.process(std::bind(&CGame::processDelayedEvent, this, std::placeholders::_1));
		m_dwGameTime6 = dwTime;
		// v2.05
		if (m_iFinalShutdownCount != 0) {
			m_iFinalShutdownCount--;
			wsprintf(G_cTxt, "Final Shutdown...%d", m_iFinalShutdownCount);
			PutLogList(G_cTxt);
			if (m_iFinalShutdownCount <= 1) {
				SendMessage(m_hWnd, WM_CLOSE, 0, 0);
				return;
			}
		}
	}
	if (m_bHappyHour) {
		if ((dwTime - dwHappyHourTime) == 3600000) {
			EndHappyHour();
		}
	}
	if ((dwTime - m_dwGameTime3) > 3000) {
		SyncMiddlelandMapInfo();
		dynamicObjects_.CheckDynamicObjectList();
		dynamicObjects_.DynamicObjectEffectProcessor();
		NoticeHandler();
		SpecialEventHandler();
		EnergySphereProcessor(false, nullptr);
		m_dwGameTime3 = dwTime;
	}
	if ((dwTime - m_dwGameTime7) > 100000) { //Timer
		switch (iDice(1, 19)) { //Pick a random advert
				//Change these to whatever you want to display
			case 1: wsprintf(cAdvert, "Server-Info: Criticals: Ctrl + C (300 Criticals = 5000 Gold).");
				break;
			case 2: wsprintf(cAdvert, "Server-Info: MIDDLELAND NO TIENE PITS (SOLO SE USA EN CRUSADE).");
				break;
			case 3: wsprintf(cAdvert, "Server-Info: Lista de Drops y demï¿½s guias en la Web!.");
				break;
			case 4: wsprintf(cAdvert, "Server-Info: Max Barrack lvl 200 || PL: No Max lvl!.");
				break;
			case 5: wsprintf(cAdvert, "Server-Info: Druncian PITS: HC,TW,Dark-Elf,Liche,Uni,Barlog & G.Lizard.");
				break;
			case 6: wsprintf(cAdvert, "Server-Info: MIDDLELAND DON'T HAVE PITS!. (ONLY USED IN CRUSADE).");
				break;
			case 7: wsprintf(cAdvert, "Server-Info: PL PITS: Ettin,Demon,Gargoyle,Orges,Ice-Golems,Wyverns & Abbaddon.");
				break;
			case 8: wsprintf(cAdvert, "Server-Info: City PITS: Cyclops, WereWolf, Scorp, Clay Golems & Hellhound.");
				break;
			case 9: wsprintf(cAdvert, "Server-Info: Drop List and other Guides in the Website!.");
				break;
			case 10: wsprintf(cAdvert, "SERVER-Info: Pedimos disculpas a todos los afectados por la caida del primer dï¿½a!.");
				break;
		}
		for (q = 1; q < DEF_MAXCLIENTS; q++) // Check all clients
		{
			if ((m_pClientList[q] != nullptr)) // Check if client is active
			{ //Send message to server.
				m_pClientList[q]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cAdvert);
			}
		}
		m_dwGameTime7 = dwTime; //Reset timer so it doesn't spam :D
	}
	if ((dwTime - m_dwGameTime4) > 600) {
		MobGenerator();
		if (m_iSubLogSockInitIndex < DEF_MAXSUBLOGSOCK) {
			m_pSubLogSock[m_iSubLogSockInitIndex] = new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
			m_pSubLogSock[m_iSubLogSockInitIndex]->bConnect(m_cLogServerAddr, m_iLogServerPort, (WM_ONLOGSOCKETEVENT + m_iSubLogSockInitIndex + 1));
			m_pSubLogSock[m_iSubLogSockInitIndex]->bInitBufferSize(DEF_MSGBUFFERSIZE);
			wsprintf(G_cTxt, "(!) Try to connect sub-log-socket(%d)... Addr:%s  Port:%d", m_iSubLogSockInitIndex, m_cLogServerAddr, m_iLogServerPort);
			PutLogList(G_cTxt);
			m_iSubLogSockInitIndex++;
		}
		m_dwGameTime4 = dwTime;
	}
	if ((dwTime - m_dwGameTime5) > 1000 * 60 * 3) {
		if (middlelandMap_.lock()) {
			// Crusade
			SaveOccupyFlagData();
			//bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, nullptr, nullptr);
			//bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, nullptr, nullptr);
		}
		m_dwGameTime5 = dwTime;
		srand((unsigned) time(nullptr));
	}
	if ((dwTime - m_dwFishTime) > 4000) {
		FishProcessor();
		FishGenerator();
		SendCollectedMana();
		CrusadeWarStarter();
		m_dwFishTime = dwTime;
		AFKChecker();
		if ((m_bIsHeldenianReady) && (!m_bIsHeldenianMode)) {
			HeldenianWarStarter();
		}
		if ((m_bIsHeldenianReady) && (m_bIsHeldenianMode)) {
			HeldenianWarEnder();
		}
		if ((!m_bHeldenianRunning) && (m_bIsHeldenianMode)) {
			AutomaticHeldenianEnd();
		}
		if ((m_bHeldenianInitiated) && (m_bIsHeldenianReady)) {
			AutomatedHeldenianTimer();
		}
	}
	if ((dwTime - m_dwWhetherTime) > 1000 * 20) {
		WhetherProcessor();
		m_dwWhetherTime = dwTime;
	}
	if ((dwTime - m_dwCanFightzoneReserveTime) > 7200000) {
		FightzoneReserveProcessor();
		m_dwCanFightzoneReserveTime = dwTime;
	}
	if ((!m_bIsServerShutdowned) && (m_bOnExitProcess) && ((dwTime - m_dwExitProcessTime) > 1000 * 2)) {
		if (_iForcePlayerDisconect(15) == 0) {
			PutLogList("(!) GAME SERVER SHUTDOWN PROCESS COMPLETED! All players are disconnected.");
			m_bIsServerShutdowned = true;
			if ((m_cShutDownCode == 3) || (m_cShutDownCode == 4)) {
				PutLogFileList("(!!!) AUTO-SERVER-REBOOTING!");
				bInit();
				m_iAutoRebootingCount++;
			} else {
				if (m_iFinalShutdownCount == 0) m_iFinalShutdownCount = 20;
			}
		}
		m_dwExitProcessTime = dwTime;
	}
	if ((dwTime - m_dwMapSectorInfoTime) > 1000 * 10) {
		m_dwMapSectorInfoTime = dwTime;
		UpdateMapSectorInfo();
		MineralGenerator();
		m_iMapSectorInfoUpdateCount++;
		if (m_iMapSectorInfoUpdateCount >= 5) {
			AgingMapSectorInfo();
			m_iMapSectorInfoUpdateCount = 0;
		}
	}
}

bool CGame::bReadScheduleConfigFile(const char *pFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadModeA, cReadModeB;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	int iIndex;
	cReadModeA = 0;
	cReadModeB = 0;
	iIndex = 0;
	hFile = CreateFile(pFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(pFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open Schedule file.");
		return false;
	} else {
		PutLogList("(!) Reading Schedule file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadModeA != 0) {
				switch (cReadModeA) {
					case 1:
						if (strcmp(m_cServerName, token) == 0) {
							PutLogList("(!) Success: This server will schedule crusade total war.");
							m_bIsCrusadeWarStarter = true;
						}
						cReadModeA = 0;
						break;
					case 2:
						switch (cReadModeB) {
							case 1:
								if (iIndex >= DEF_MAXSCHEDULE) {
									PutLogList("(!) WARNING! Too many crusade war schedule!");
									return true;
								}
								m_stCrusadeWarSchedule[iIndex].iDay = atoi(token);
								cReadModeB = 2;
								break;
							case 2:
								m_stCrusadeWarSchedule[iIndex].iHour = atoi(token);
								cReadModeB = 3;
								break;
							case 3:
								m_stCrusadeWarSchedule[iIndex].iMinute = atoi(token);
								iIndex++;
								cReadModeA = 0;
								cReadModeB = 0;
								break;
						}
						break;
					case 3:
						switch (cReadModeB) {
							case 1:
								if (iIndex >= DEF_MAXHELDENIAN) {
									PutLogList("(!) WARNING! Too many Heldenians scheduled!");
									return true;
								}
								m_stHeldenianSchedule[iIndex].iDay = atoi(token);
								cReadModeB = 2;
								break;
							case 2:
								m_stHeldenianSchedule[iIndex].StartiHour = atoi(token);
								cReadModeB = 3;
								break;
							case 3:
								m_stHeldenianSchedule[iIndex].StartiMinute = atoi(token);
								cReadModeB = 4;
								break;
							case 4:
								m_stHeldenianSchedule[iIndex].EndiHour = atoi(token);
								cReadModeB = 5;
								break;
							case 5:
								m_stHeldenianSchedule[iIndex].EndiMinute = atoi(token);
								iIndex++;
								cReadModeA = 0;
								cReadModeB = 0;
								break;
						}
						break;
				}
			} else {
				if (memcmp(token, "crusade-server-name", 19) == 0) {
					cReadModeA = 1;
					cReadModeB = 1;
				}
				if (memcmp(token, "crusade-schedule", 16) == 0) {
					cReadModeA = 3;
					cReadModeB = 1;
				}
				if (memcmp(token, "Heldenian-schedule", 18) == 0) {
					cReadModeA = 6;
					cReadModeB = 1;
				}
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

void CGame::GlobalStartCrusadeMode() {
	uint32_t dwCrusadeGUID;
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	// Permite activar mas de una crusada por dia.
	m_iLatestCrusadeDayOfWeek = SysTime.wDayOfWeek;
	/*if (m_iLatestCrusadeDayOfWeek != -1) {
		 if (m_iLatestCrusadeDayOfWeek == SysTime.wDayOfWeek) return;
	}
	else m_iLatestCrusadeDayOfWeek = SysTime.wDayOfWeek;*/
	dwCrusadeGUID = timeGetTime();
	LocalStartCrusadeMode(dwCrusadeGUID);
}

void CGame::OnStartGameSignal() {
	int i;
	for (i = 0; i < DEF_MAXMAPS; i++) {
		if (m_pMapList[i] != nullptr) {
			m_pMapList[i]->_bReadMapInfoFiles();
		}
	}
	bReadCrusadeStructureConfigFile("..\\GameConfigs\\Crusade.cfg");
	_LinkStrikePointMapIndex();
	bReadScheduleConfigFile("..\\GameConfigs\\Schedule.cfg");
	bReadCrusadeGUIDFile("GameData\\CrusadeGUID.txt");
	bReadApocalypseGUIDFile("GameData\\ApocalypseGUID.txt");
	bReadHeldenianGUIDFile("GameData\\HeldenianGUID.txt");
	PutLogList("");
	PutLogList("(!) Game Server Activated.");
}

void CGame::GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char * pMapName) {
	int i;
	int iIndex;
	uint32_t dwTemp;
	uint32_t dwTime;
	wsprintf(G_cTxt, "SetGuildTeleportLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
	PutLogList(G_cTxt);
	dwTime = timeGetTime();
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

int ITEMSPREAD_FIEXD_COORD [25][2] = {
	{ 0, 0}, //1
	{ 1, 0}, //2
	{ 1, 1}, //3
	{ 0, 1}, //4
	{-1, 1}, //5
	{-1, 0}, //6
	{-1, -1}, //7
	{ 0, -1}, //8
	{ 1, -1}, //9
	{ 2, -1}, //10
	{ 2, 0}, //11
	{ 2, 1}, //12
	{ 2, 2}, //13
	{ 1, 2}, //14
	{ 0, 2}, //15
	{-1, 2}, //16
	{-2, 2}, //17
	{-2, 1}, //18
	{-2, 0}, //19
	{-2, -1}, //20
	{-2, -2}, //21
	{-1, -2}, //22
	{ 0, -2}, //23
	{ 1, -2}, //24
	{ 2, -2}, //25
};

bool CGame::bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY, int iItemSpreadType, int iSpreadRange, int *iItemIDs, POINT *BasePos, int *iNumItem) {
	int iProb = 100;
	float fProb;
	float fProbA;
	float fProbB;
	float fProbC;
	int iItemID;
	int iNum = 0;
	for (int i = 0; i < iMax; i++) {
		if (i > iMin) iProb = iProbability;
		fProb = (float) (100 - iProb) / 10.0;
		if (fProb < 1.0) fProb = 1.0;
		fProbA = fProbB = fProbC = fProb;
		fProbA = fProbA * 8.0f;
		fProbB = fProbB * 4.0f;
		fProbC = fProbC;
		iItemID = 0;
		switch (sNpcType) {
			case 69: // Wyvern
				switch (iDice(1, 4)) {
					case 1: if (iDice(1, 8000) == 1) iItemID = 849;
						break; // Kloness Blade
					case 2: if (iDice(1, 2) == 2) iItemID = 20;
						break; // Excaliber
					case 3: if (iDice(1, 500) == 2) iItemID = 610;
						break; // Xelima's Blade
					case 4: if (iDice(1, 1500) == 2) iItemID = 636;
						break; // Ring of Grand Mage
					case 5: if (iDice(1, 250) == 2) iItemID = 633;
						break; // Ring of Demon Power
				}
				if (iItemID == 0) {
					switch (iDice(1, 5)) {
						case 1: if (iDice(1, 500) == 2) iItemID = 642;
							break; // KnecklaceOfIcePro
						case 2: if (iDice(1, 500) == 3) iItemID = 636;
							break; // RingofGrandMage
						case 3: if (iDice(1, 1500) == 3) iItemID = 734;
							break; // RingOfArcmage
						case 4: if (iDice(1, 250) == 3) iItemID = 634;
							break; // RingofWizard
						case 5: if (iDice(1, 1500) == 2) iItemID = 643;
							break; // KnecklaceOfIceEle
					}
				}
				break;
				break;
			case 73: // Fire-Wyvern
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, (45 * fProbA)) == 3) iItemID = 847;
						break; // DarkExecutor
					case 2: if (iDice(1, (35 * fProbA)) == 3) iItemID = 630;
						break; // RingoftheXelima
					case 3: if (iDice(1, (40 * fProbA)) == 3) iItemID = 860;
						break; // NecklaceOfXelima
					case 4: if (iDice(1, (1 * fProbA)) == 1) iItemID = 735;
						break; // RingOfDragonpower
					case 5: if (iDice(1, (2 * fProbA)) == 1) iItemID = 851;
						break; // Kloness Esterk
				}
				if (iItemID == 0) {
					switch (iDice(1, 9)) {
						case 1: if (iDice(1, (700 * fProbB)) == 2) iItemID = 645;
							break; // KnecklaceOfEfreet
						case 2: if (iDice(1, (500 * fProbB)) == 2) iItemID = 638;
							break; // KnecklaceOfFirePro
						case 3: if (iDice(1, (700 * fProbB)) == 3) iItemID = 636;
							break; // RingofGrandMage
						case 4: if (iDice(1, (800 * fProbB)) == 3) iItemID = 734;
							break; // RingOfArcmage
						case 5: if (iDice(1, (600 * fProbB)) == 3) iItemID = 634;
							break; // RingofWizard
						case 6: if (iDice(1, (300 * fProbB)) == 2) iItemID = 290;
							break; // Flameberge+3(LLF)
						case 7: if (iDice(1, (300 * fProbB)) == 3) iItemID = 490;
							break; // BloodSword
						case 8: if (iDice(1, (300 * fProbB)) == 3) iItemID = 491;
							break; // BloodAxe
						case 9: if (iDice(1, (300 * fProbB)) == 3) iItemID = 492;
							break; // BloodRapier
					}
				}
				break;
			case 81: // Abaddon
				switch (iDice(1, 7)) {
					case 1: if (iDice(1, (4 * fProbA)) == 1) iItemID = 846;
						break; // The_Devastator
					case 2: if (iDice(1, (2 * fProbA)) == 1) iItemID = 850;
						break; // Kloness Axe
					case 3: if (iDice(1, (1 * fProbA)) == 1) iItemID = 860;
						break; // NecklaceOfXelima
					case 4: if (iDice(1, (2 * fProbA)) == 1) iItemID = 982;
						break; // ResurWand(MS.30)
					case 5: if (iDice(1, (1 * fProbA)) == 1) iItemID = 631;
						break; // RingoftheAbaddon
					case 6: if (iDice(1, (2 * fProbA)) == 1) iItemID = 980;
						break; // BerserkWand(MS.30)
					case 7: if (iDice(1, (3 * fProbA)) == 1) iItemID = 983;
						break; // AbaddonWand(MS.40)
				}
				if (iItemID == 0) {
					switch (iDice(1, 10)) {
						case 1: if (iDice(1, (4 * fProbB)) == 3) iItemID = 762;
							break; // GBattleHammer
						case 2: if (iDice(1, (4 * fProbB)) == 3) iItemID = 491;
							break; // BloodAxe
						case 3: if (iDice(1, (4 * fProbB)) == 3) iItemID = 492;
							break; // BloodRapier
						case 4: if (iDice(1, (4 * fProbB)) == 3) iItemID = 611;
							break; // XelimaAxe
						case 5: if (iDice(1, (4 * fProbB)) == 3) iItemID = 610;
							break; // XelimaBlade
						case 6: if (iDice(1, (4 * fProbB)) == 3) iItemID = 613;
							break; // Medusa Sword
						case 7: if (iDice(1, (4 * fProbB)) == 3) iItemID = 645;
							break; // KnecklaceOfEfreet
						case 8: if (iDice(1, (4 * fProbB)) == 3) iItemID = 638;
							break; // KnecklaceOfFirePro
						case 9: if (iDice(1, (4 * fProbB)) == 3) iItemID = 3043;
							break; // Strike of the Ghosts Manual
						case 10: if (iDice(1, (4 * fProbB)) == 3) iItemID = 259;
							break; // MagicWand(M.Shield)
					}
				}
				break;
		} // switch
		if (iItemID == 0) {
			switch (iDice(1, 19)) {
				case 1: if (iDice(1, (2 * fProbC)) == 2) iItemID = 871;
					break; // AncientTablets
				case 2: if (iDice(1, (2 * fProbC)) == 2) iItemID = 870;
					break; // AncientTablets
				case 3: if (iDice(1, (2 * fProbC)) == 2) iItemID = 869;
					break; // AncientTablet
				case 4: if (iDice(1, (2 * fProbC)) == 2) iItemID = 868;
					break; // AcientTablet(LU)
				case 5: if (iDice(1, (2 * fProbC)) == 2) iItemID = 650;
					break; // ZemstoneOfSacrifice
				case 6: if (iDice(1, (2 * fProbC)) == 2) iItemID = 656;
					break; // StoneOfXelima
				case 7: if (iDice(1, (2 * fProbC)) == 2) iItemID = 657;
					break; // StoneOfMerien
				case 8: if (iDice(1, (2 * fProbC)) == 2) iItemID = 333;
					break; // PlatinumRing
				case 9: if (iDice(1, (2 * fProbC)) == 2) iItemID = 334;
					break; // LuckyGoldRing
				case 10: if (iDice(1, (2 * fProbC)) == 2) iItemID = 335;
					break; // EmeraldRing
				case 11: if (iDice(1, (2 * fProbC)) == 2) iItemID = 336;
					break; // SapphireRing
				case 12: if (iDice(1, (2 * fProbC)) == 2) iItemID = 653;
					break; // Yellow Ball
				case 13: if (iDice(1, (2 * fProbC)) == 2) iItemID = 290;
					break; // Flameberge+3(LLF)
				case 14: if (iDice(1, (2 * fProbC)) == 2) iItemID = 292;
					break; // GoldenAxe(LLF)
				case 15: if (iDice(1, (2 * fProbC)) == 2) iItemID = 300;
					break; // MagicNecklace(RM10)
				case 16: if (iDice(1, (2 * fProbC)) == 2) iItemID = 311;
					break; // MagicNecklace(DF+10)
				case 17: if (iDice(1, (2 * fProbC)) == 2) iItemID = 305;
					break; // MagicNecklace(DM+1)
				case 18: if (iDice(1, (2 * fProbC)) == 2) iItemID = 308;
					break; // MagicNecklace(MS10)
				case 19: if (iDice(1, (3 * fProbC)) == 2) iItemID = 654;
					break; // Blue Ball
			}
		}
		if (iItemID == 0 && iProb == 100) iItemID = 90; // Gold
		if (iItemID != 0) {
			// item id
			iItemIDs[iNum] = iItemID;
			// item position
			switch (iItemSpreadType) {
				case DEF_ITEMSPREAD_RANDOM:
					BasePos[iNum].x = sBaseX + iSpreadRange - iDice(1, iSpreadRange * 2);
					BasePos[iNum].y = sBaseY + iSpreadRange - iDice(1, iSpreadRange * 2);
					break;
				case DEF_ITEMSPREAD_FIXED:
					BasePos[iNum].x = sBaseX + ITEMSPREAD_FIEXD_COORD[iNum][0];
					BasePos[iNum].y = sBaseY + ITEMSPREAD_FIEXD_COORD[iNum][1];
					break;
			}
			iNum++;
		}
	} // for
	*iNumItem = iNum;
	return true;
}

bool CGame::bReadAdminSetConfigFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	cReadMode = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		return false;
	} else {
		PutLogList("(!) Reading settings file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		//token = strtok( cp, seps );
		while (token != nullptr) {
			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelGMKill = atoi(token);
						} else {
							m_iAdminLevelGMKill = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 2:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelGMRevive = atoi(token);
						} else {
							m_iAdminLevelGMRevive = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 3:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelGMCloseconn = atoi(token);
						} else {
							m_iAdminLevelGMCloseconn = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 4:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelGMCheckRep = atoi(token);
						} else {
							m_iAdminLevelGMCheckRep = 1;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 5:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelWho = atoi(token);
						} else {
							m_iAdminLevelWho = 1;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 6:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelEnergySphere = atoi(token);
						} else {
							m_iAdminLevelEnergySphere = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 7:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelShutdown = atoi(token);
						} else {
							m_iAdminLevelShutdown = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 8:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelObserver = atoi(token);
						} else {
							m_iAdminLevelObserver = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 9:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelShutup = atoi(token);
						} else {
							m_iAdminLevelShutup = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 10:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelCallGaurd = atoi(token);
						} else {
							m_iAdminLevelCallGaurd = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 11:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSummonDemon = atoi(token);
						} else {
							m_iAdminLevelSummonDemon = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 12:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSummonDeath = atoi(token);
						} else {
							m_iAdminLevelSummonDeath = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 13:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelReserveFightzone = atoi(token);
						} else {
							m_iAdminLevelReserveFightzone = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 14:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelCreateFish = atoi(token);
						} else {
							m_iAdminLevelCreateFish = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 15:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelTeleport = atoi(token);
						} else {
							m_iAdminLevelTeleport = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 16:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelCheckIP = atoi(token);
						} else {
							m_iAdminLevelCheckIP = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 17:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelPolymorph = atoi(token);
						} else {
							m_iAdminLevelPolymorph = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 18:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSetInvis = atoi(token);
						} else {
							m_iAdminLevelSetInvis = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 19:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSetZerk = atoi(token);
						} else {
							m_iAdminLevelSetZerk = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 20:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSetIce = atoi(token);
						} else {
							m_iAdminLevelSetIce = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 21:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelGetNpcStatus = atoi(token);
						} else {
							m_iAdminLevelGetNpcStatus = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 22:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSetAttackMode = atoi(token);
						} else {
							m_iAdminLevelSetAttackMode = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 23:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelUnsummonAll = atoi(token);
						} else {
							m_iAdminLevelUnsummonAll = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 24:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelUnsummonDemon = atoi(token);
						} else {
							m_iAdminLevelUnsummonDemon = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 25:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSummon = atoi(token);
						} else {
							m_iAdminLevelSummon = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 26:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSummonAll = atoi(token);
						} else {
							m_iAdminLevelSummonAll = 4;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 27:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSummonPlayer = atoi(token);
						} else {
							m_iAdminLevelSummonPlayer = 1;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 28:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelDisconnectAll = atoi(token);
						} else {
							m_iAdminLevelDisconnectAll = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 29:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelEnableCreateItem = atoi(token);
						} else {
							m_iAdminLevelEnableCreateItem = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 30:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelCreateItem = atoi(token);
						} else {
							m_iAdminLevelCreateItem = 4;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 31:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelStorm = atoi(token);
						} else {
							m_iAdminLevelStorm = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 32:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelWeather = atoi(token);
						} else {
							m_iAdminLevelWeather = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 33:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSetStatus = atoi(token);
						} else {
							m_iAdminLevelSetStatus = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 34:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelGoto = atoi(token);
						} else {
							m_iAdminLevelGoto = 1;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 35:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelMonsterCount = atoi(token);
						} else {
							m_iAdminLevelMonsterCount = 1;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 36:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSetRecallTime = atoi(token);
						} else {
							m_iAdminLevelSetRecallTime = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 37:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelUnsummonBoss = atoi(token);
						} else {
							m_iAdminLevelUnsummonBoss = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 38:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelClearNpc = atoi(token);
						} else {
							m_iAdminLevelClearNpc = 3;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 39:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelTime = atoi(token);
						} else {
							m_iAdminLevelTime = 2;
						}
						cReadMode = 0;
						break;
						//----------------------------------------------------------------
					case 40:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelPushPlayer = atoi(token);
						} else {
							m_iAdminLevelPushPlayer = 2;
						}
						cReadMode = 0;
						break;
						//-----------------------------------------------------------------
					case 41:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelSummonGuild = atoi(token);
						} else {
							m_iAdminLevelSummonGuild = 3;
						}
						cReadMode = 0;
						break;
						//-----------------------------------------------------------------
					case 42:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelCheckStatus = atoi(token);
						} else {
							m_iAdminLevelCheckStatus = 1;
						}
						cReadMode = 0;
						break;
						//-----------------------------------------------------------------
					case 43:
						if ((strlen(token) > 0) && (strlen(token) < 9)) {
							m_iAdminLevelCleanMap = atoi(token);
						} else {
							m_iAdminLevelCleanMap = 1;
						}
						cReadMode = 0;
						break;
						//-----------------------------------------------------------------
				}
			} else {
				if (memcmp(token, "Admin-Level-/kill", 17) == 0) cReadMode = 1;
				if (memcmp(token, "Admin-Level-/revive", 19) == 0) cReadMode = 2;
				if (memcmp(token, "Admin-Level-/closecon", 21) == 0) cReadMode = 3;
				if (memcmp(token, "Admin-Level-/checkrep", 21) == 0) cReadMode = 4;
				if (memcmp(token, "Admin-Level-/who", 16) == 0) cReadMode = 5;
				if (memcmp(token, "Admin-Level-/energysphere", 25) == 0) cReadMode = 6;
				if (memcmp(token, "Admin-Level-/shutdownthisserverrightnow", 39) == 0) cReadMode = 7;
				if (memcmp(token, "Admin-Level-/setobservermode", 28) == 0) cReadMode = 8;
				if (memcmp(token, "Admin-Level-/shutup", 19) == 0) cReadMode = 9;
				if (memcmp(token, "Admin-Level-/attack", 19) == 0) cReadMode = 10;
				if (memcmp(token, "Admin-Level-/summondemon", 24) == 0) cReadMode = 11;
				if (memcmp(token, "Admin-Level-/summondeath", 24) == 0) cReadMode = 12;
				if (memcmp(token, "Admin-Level-/reservefightzone", 28) == 0) cReadMode = 13;
				if (memcmp(token, "Admin-Level-/createfish", 23) == 0) cReadMode = 14;
				if (memcmp(token, "Admin-Level-/teleport", 21) == 0) cReadMode = 15;
				if (memcmp(token, "Admin-Level-/checkip", 20) == 0) cReadMode = 16;
				if (memcmp(token, "Admin-Level-/polymorph", 22) == 0) cReadMode = 17;
				if (memcmp(token, "Admin-Level-/setinvi", 20) == 0) cReadMode = 18;
				if (memcmp(token, "Admin-Level-/setzerk", 20) == 0) cReadMode = 19;
				if (memcmp(token, "Admin-Level-/setfreeze", 22) == 0) cReadMode = 20;
				if (memcmp(token, "Admin-Level-/gns", 16) == 0) cReadMode = 21;
				if (memcmp(token, "Admin-Level-/setattackmode", 26) == 0) cReadMode = 22;
				if (memcmp(token, "Admin-Level-/unsummonall", 24) == 0) cReadMode = 23;
				if (memcmp(token, "Admin-Level-/unsummondemon", 26) == 0) cReadMode = 24;
				if (memcmp(token, "Admin-Level-/summonnpc", 22) == 0) cReadMode = 25;
				if (memcmp(token, "Admin-Level-/summonall", 22) == 0) cReadMode = 26;
				if (memcmp(token, "Admin-Level-/summonplayer", 25) == 0) cReadMode = 27;
				if (memcmp(token, "Admin-Level-/disconnectall", 26) == 0) cReadMode = 28;
				if (memcmp(token, "Admin-Level-/enableadmincreateitem", 34) == 0) cReadMode = 29;
				if (memcmp(token, "Admin-Level-/createitem", 23) == 0) cReadMode = 30;
				if (memcmp(token, "Admin-Level-/storm", 18) == 0) cReadMode = 31;
				if (memcmp(token, "Admin-Level-/weather", 20) == 0) cReadMode = 32;
				if (memcmp(token, "Admin-Level-/setstatus", 22) == 0) cReadMode = 33;
				if (memcmp(token, "Admin-Level-/goto", 17) == 0) cReadMode = 34;
				if (memcmp(token, "Admin-Level-/monstercount", 17) == 0) cReadMode = 35;
				if (memcmp(token, "Admin-Level-/setforcerecalltime", 23) == 0) cReadMode = 36;
				if (memcmp(token, "Admin-Level-/unsummonboss", 25) == 0) cReadMode = 37;
				if (memcmp(token, "Admin-Level-/clearnpc", 21) == 0) cReadMode = 38;
				if (memcmp(token, "Admin-Level-/time", 17) == 0) cReadMode = 39;
				if (memcmp(token, "Admin-Level-/send", 17) == 0) cReadMode = 40;
				if (memcmp(token, "Admin-Level-/summonguild", 24) == 0) cReadMode = 41;
				if (memcmp(token, "Admin-Level-/checkstatus", 24) == 0) cReadMode = 42;
				if (memcmp(token, "Admin-Level-/clearmap", 21) == 0) cReadMode = 43;
			}
			token = pStrTok->pGet();
			//token = strtok( nullptr, seps );
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

/*void CGame::AdminOrder_SummonGuild(CClient &client, char *pData, uint32_t dwMsgSize)
{
 char   seps[] = "= \t\n";
 char   * token, * cp, cBuff[256], cMapName[11], char cGuildName[20];
 uint16_t   *wp;
 int pX;
 int pY;
 int i;
 class  CStrTok * pStrTok;
 uint32_t  dwGoldCount;
	if (client.markedForDeletion_) return;
	if ((dwMsgSize) <= 0) return;
	if (client.m_iAdminUserLevel < m_iAdminLevelSummonGuild) {

		client.SendNotifyMsg(0,DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	if (client.m_iAdminUserLevel == 0) {
		// dwGoldCount = player gold
		dwGoldCount = dwGetItemCount(iClientH, "Gold");
		// if summonguildcost is greater than player gold return
		if (m_iSummonGuildCost > dwGoldCount) {
			return;
		}
		// if summonguildcost is less than player gold
		else {
			// reduce gold by summonguildcost
			SetItemCount(iClientH, "Gold", dwGoldCount - m_iSummonGuildCost);
		}
	}
	std::memset(cBuff, 0, sizeof(cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	pX = client.m_sX;
	pY = client.m_sY;
	//memcpy(cGuildName, client.m_cGuildName, 20);
	memcpy(cMapName, client.map_->m_cName, 11);
	if (strlen(token) > 20)
			memcpy(cGuildName, token, 20);
		else memcpy(cGuildName, token, strlen(token));
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cGuildName, token) == 0)) {
			m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, pX, pY);
	}//m_pClientList[i]->m_cCharName
	wsprintf(G_cTxt,"GM Order(%s): PC(%s) Summoned to (%s)", client.m_cGuildName, cGuildName, cMapName);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	delete pStrTok;
}*/
void CGame::SendCollectedMana() {
	if ((m_iCollectedMana[1] == 0) && (m_iCollectedMana[2] == 0)) return;
	wsprintf(G_cTxt, "Sending Collected Mana: %d %d", m_iCollectedMana[1], m_iCollectedMana[2]);
	PutLogList(G_cTxt);
	CollectedManaHandler(m_iCollectedMana[1], m_iCollectedMana[2]);
	m_iCollectedMana[0] = 0;
	m_iCollectedMana[1] = 0;
	m_iCollectedMana[2] = 0;
}

void CGame::CollectedManaHandler(uint16_t wAresdenMana, uint16_t wElvineMana) {
	if (m_iAresdenMapIndex != -1) {
		m_iAresdenMana += wAresdenMana;
		if (wAresdenMana > 0) {
			wsprintf(G_cTxt, "Aresden Mana: %d Total:%d", wAresdenMana, m_iAresdenMana);
			PutLogList(G_cTxt);
		}
	}
	if (m_iElvineMapIndex != -1) {
		m_iElvineMana += wElvineMana;
		if (wElvineMana > 0) {
			wsprintf(G_cTxt, "Elvine Mana: %d Total:%d", wElvineMana, m_iElvineMana);
			PutLogList(G_cTxt);
		}
	}
}

void CGame::_LinkStrikePointMapIndex() {
	int i;
	int z;
	int x;
	for (i = 0; i < DEF_MAXMAPS; i++)
		if ((m_pMapList[i] != nullptr) && (m_pMapList[i]->m_iTotalStrikePoints != 0)) {
			for (z = 0; z < DEF_MAXSTRIKEPOINTS; z++)
				if (strlen(m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName) != 0) {
					for (x = 0; x < DEF_MAXMAPS; x++)
						if ((m_pMapList[x] != nullptr) && (strcmp(m_pMapList[x]->m_cName, m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName) == 0)) {
							m_pMapList[i]->m_stStrikePoint[z].iMapIndex = x;
							wsprintf(G_cTxt, "(!) Map(%s) Strike Point(%d) Related Map(%s) Index(%d)", m_pMapList[i]->m_cName, z, m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName, x);
							PutLogList(G_cTxt);
							goto LSPMI_LOOPBREAK;
						}
LSPMI_LOOPBREAK:
					;
				}
		}
}

bool CGame::_bRegisterMap(char * pName) {
	int i;
	char cTmpName[11], cTxt[120];
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, pName);
	for (i = 0; i < DEF_MAXMAPS; i++) {
		if ((m_pMapList[i] != nullptr) && (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)) {
			wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map (%s) is already installed! cannot add.", cTmpName);
			PutLogList(cTxt);
			return false;
		}
	}
	for (i = 0; i < DEF_MAXMAPS; i++) {
		if (m_pMapList[i] == nullptr) {
			m_pMapList[i] = std::make_shared<CMap>(i, *this);
			wsprintf(cTxt, "(*) Add map (%s)   - Loading map info files...", pName);
			PutLogList(cTxt);
			if (m_pMapList[i]->bInit(pName) == false) {
				wsprintf(cTxt, "(!!!) Data file loading fail!", pName);
				PutLogList(cTxt);
				return false;
			};
			PutLogList("(*) Data file loading success.");
			if (!middlelandMap_.lock() && (strcmp("middleland", pName) == 0)) middlelandMap_ = m_pMapList[i];
			if ((m_iAresdenMapIndex == -1) && (strcmp("aresden", pName) == 0)) m_iAresdenMapIndex = i;
			if ((m_iElvineMapIndex == -1) && (strcmp("elvine", pName) == 0)) m_iElvineMapIndex = i;
			if ((m_iBTFieldMapIndex == -1) && (strcmp("BtField", pName) == 0)) m_iBTFieldMapIndex = i; // new
			if ((m_iGodHMapIndex == -1) && (strcmp("GodH", pName) == 0)) m_iGodHMapIndex = i; // new
			m_iTotalMaps++;
			return true;
		}
	}
	wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map (%s) canot be added - no more map space.", pName);
	PutLogList(cTxt);
	return false;
}

void CGame::GrandMagicResultHandler(char *cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char * cData) {
	int i;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_GRANDMAGICRESULT, iCrashedStructureNum, iStructureDamageAmount, iCasualities, cMapName, iActiveStructure, 0, 0, 0, 0, iTotalStrikePoints, cData);
		}
}

bool CGame::bReadAdminListConfigFile(const char *pFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadModeA;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	int iIndex;
	char len;
	cReadModeA = 0;
	iIndex = 0;
	hFile = CreateFile(pFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(pFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open AdminList.cfg file.");
		return false;
	} else {
		PutLogList("(!) Reading AdminList.cfg...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadModeA != 0) {
				switch (cReadModeA) {
					case 1:
						if (iIndex >= DEF_MAXADMINS) {
							PutLogList("(!) WARNING! Too many GMs on the AdminList.cfg!");
							return true;
						}
						len = strlen(token);
						if (len > 10) len = 10;
						std::memset(m_stAdminList[iIndex].m_cGMName, 0, sizeof (m_stAdminList[iIndex].m_cGMName));
						memcpy(m_stAdminList[iIndex].m_cGMName, token, strlen(token));
						iIndex++;
						cReadModeA = 0;
						break;
				}
			} else {
				if (memcmp(token, "verified-admin", 14) == 0) {
					cReadModeA = 1;
				}
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

bool CGame::bReadBannedListConfigFile(const char *pFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadModeA;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	int iIndex;
	char len;
	cReadModeA = 0;
	iIndex = 0;
	hFile = CreateFile(pFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(pFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open BannedList.cfg file.");
		return false;
	} else {
		PutLogList("(!) Reading BannedList.cfg...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadModeA != 0) {
				switch (cReadModeA) {
					case 1:
						if (iIndex >= DEF_MAXBANNED) {
							PutLogList("(!) WARNING! Too many banned on the BannedList.cfg!");
							return true;
						}
						len = strlen(token);
						if (len > 20) len = 20;
						std::memset(m_stBannedList[iIndex].m_cBannedIPaddress, 0, sizeof (m_stBannedList[iIndex].m_cBannedIPaddress));
						memcpy(m_stBannedList[iIndex].m_cBannedIPaddress, token, strlen(token));
						iIndex++;
						cReadModeA = 0;
						break;
				}
			} else {
				if (memcmp(token, "banned-ip", 9) == 0) {
					cReadModeA = 1;
				}
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

void CGame::GlobalEndApocalypseMode() {
	LocalEndApocalypseMode();
}

void CGame::GlobalUpdateConfigs(char cConfigType) {
	LocalUpdateConfigs(cConfigType);
}

void CGame::LocalUpdateConfigs(char cConfigType) {
	if (cConfigType == 1) {
		bReadSettingsConfigFile("..\\GameConfigs\\Settings.cfg");
		PutLogList("(!!!) Settings.cfg updated successfully!");
	}
	if (cConfigType == 2) {
		bReadAdminListConfigFile("..\\GameConfigs\\AdminList.cfg");
		PutLogList("(!!!) AdminList.cfg updated successfully!");
	}
	if (cConfigType == 3) {
		bReadBannedListConfigFile("..\\GameConfigs\\BannedList.cfg");
		PutLogList("(!!!) BannedList.cfg updated successfully!");
	}
	if (cConfigType == 4) {
		bReadAdminSetConfigFile("..\\GameConfigs\\AdminSettings.cfg");
		PutLogList("(!!!) AdminSettings.cfg updated successfully!");
	}
}

void CGame::LocalStartApocalypseMode(uint32_t dwApocalypseGUID) {
	int i;
	if (m_bIsApocalypseMode) return;
	m_bIsApocalypseMode = true;
	if (dwApocalypseGUID != 0) {
		_CreateApocalypseGUID(dwApocalypseGUID);
		m_dwApocalypseGUID = dwApocalypseGUID;
	}
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, nullptr);
		}
	}
	PutLogList("(!) Apocalypse Mode ON.");
}

void CGame::LocalEndApocalypseMode() {
	int i;
	if (!m_bIsApocalypseMode) return;
	m_bIsApocalypseMode = false;
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_APOCGATEENDMSG, 0, 0, 0, nullptr);
		}
	}
	PutLogList("(!) Apocalypse Mode OFF.");
	_CreateApocalypseGUID(m_dwApocalypseGUID);
}

bool CGame::bReadApocalypseGUIDFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	cReadMode = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open ApocalypseGUID file.");
		return false;
	} else {
		PutLogList("(!) Reading ApocalypseGUID file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						m_dwApocalypseGUID = atoi(token);
						wsprintf(G_cTxt, "ApocalypseGUID = %d", m_dwApocalypseGUID);
						PutLogList(G_cTxt);
						cReadMode = 0;
						break;
				}
			} else {
				if (memcmp(token, "ApocalypseGUID", 11) == 0) cReadMode = 1;
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

bool CGame::bReadHeldenianGUIDFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	cReadMode = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open HeldenianGUID file.");
		return false;
	} else {
		PutLogList("(!) Reading HeldenianGUID file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						m_dwHeldenianGUID = atoi(token);
						wsprintf(G_cTxt, "HeldenianGUID = %d", m_dwHeldenianGUID);
						PutLogList(G_cTxt);
						cReadMode = 0;
						break;
					case 2:
						m_sLastHeldenianWinner = atoi(token);
						wsprintf(G_cTxt, "HeldenianWinnerSide = %d", m_sLastHeldenianWinner);
						PutLogList(G_cTxt);
						cReadMode = 0;
						break;
				}
			} else {
				if (memcmp(token, "HeldenianGUID", 13) == 0) cReadMode = 1;
				if (memcmp(token, "winner-side", 11) == 0) cReadMode = 2;
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

void CGame::_CreateApocalypseGUID(uint32_t dwApocalypseGUID) {
	char *cp, cTxt[256], cFn[256], cTemp[1024];
	FILE *pFile;
	_mkdir("GameData");
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, "GameData");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, "ApocalypseGUID.txt");
	pFile = fopen(cFn, "wt");
	if (pFile == nullptr) {
		wsprintf(cTxt, "(!) Cannot create ApocalypseGUID(%d) file", dwApocalypseGUID);
		PutLogList(cTxt);
	} else {
		std::memset(cTemp, 0, sizeof (cTemp));
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "ApocalypseGUID = %d\n", dwApocalypseGUID);
		strcat(cTemp, cTxt);
		cp = (char *) cTemp;
		fwrite(cp, strlen(cp), 1, pFile);
		wsprintf(cTxt, "(O) ApocalypseGUID(%d) file created", dwApocalypseGUID);
		PutLogList(cTxt);
	}
	if (pFile != nullptr) fclose(pFile);
}
// New 06/05/2004
// Party Code

void CGame::PartyOperationResultHandler(char *pData) {
	char * cp, cResult, cName[12];
	uint16_t * wp;
	int i;
	int iClientH;
	int iPartyID;
	int iTotal;
	cp = (char *) (pData + 4);
	wp = (uint16_t *) cp;
	cp += 2;
	switch (*wp) {
		case 1:
			cResult = *cp;
			cp++;
			wp = (uint16_t *) cp;
			iClientH = (int) *wp;
			cp += 2;
			std::memset(cName, 0, sizeof (cName));
			memcpy(cName, cp, 10);
			cp += 10;
			wp = (uint16_t *) cp;
			iPartyID = (int) *wp;
			cp += 2;
			m_pClientList[iClientH]->PartyOperationResult_Create(cName, cResult, iPartyID);
			break;
		case 2:
			wp = (uint16_t *) cp;
			iPartyID = *wp;
			cp += 2;
			PartyOperationResult_Delete(iPartyID);
			break;
		case 3:
			wp = (uint16_t *) cp;
			iClientH = *wp;
			cp += 2;
			std::memset(cName, 0, sizeof (cName));
			memcpy(cName, cp, 10);
			cp += 10;
			if ((iClientH < 0) && (iClientH > DEF_MAXCLIENTS)) return;
			if (m_pClientList[iClientH]->markedForDeletion_) return;
			if (strcmp(m_pClientList[iClientH]->m_cCharName, cName) != 0) return;
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH) {
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = 0;
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers--;
					goto PORH_LOOPBREAK1;
				}
PORH_LOOPBREAK1:
			;
			for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
				if ((m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] != 0)) {
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1];
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] = 0;
				}
			m_pClientList[iClientH]->m_iPartyID = 0;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 8, 0, 0, nullptr);
			break;
		case 4:
			cResult = *cp;
			cp++;
			wp = (uint16_t *) cp;
			iClientH = (int) *wp;
			cp += 2;
			std::memset(cName, 0, sizeof (cName));
			memcpy(cName, cp, 10);
			cp += 10;
			wp = (uint16_t *) cp;
			iPartyID = (int) *wp;
			cp += 2;
			m_pClientList[iClientH]->PartyOperationResult_Join(cName, cResult, iPartyID);
			break;
		case 5:
			wp = (uint16_t *) cp;
			iClientH = (int) *wp;
			cp += 2;
			std::memset(cName, 0, sizeof (cName));
			memcpy(cName, cp, 10);
			cp += 10;
			wp = (uint16_t *) cp;
			iTotal = (int) *wp;
			cp += 2;
			m_pClientList[iClientH]->PartyOperationResult_Info(cName, iTotal, cp);
			break;
		case 6:
			cResult = *cp;
			cp++;
			wp = (uint16_t *) cp;
			iClientH = (int) *wp;
			cp += 2;
			std::memset(cName, 0, sizeof (cName));
			memcpy(cName, cp, 10);
			cp += 10;
			wp = (uint16_t *) cp;
			iPartyID = (int) *wp;
			cp += 2;
			m_pClientList[iClientH]->PartyOperationResult_Dismiss(cName, cResult, iPartyID);
			break;
	}
}

void CGame::PartyOperationResult_Delete(int iPartyID) {
	int i;
	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyInfo[iPartyID].iIndex[i] = 0;
		m_stPartyInfo[iPartyID].iTotalMembers = 0;
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 2, 0, 0, nullptr);
			m_pClientList[i]->m_iPartyID = 0;
			m_pClientList[i]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[i]->m_iReqJoinPartyClientH = 0;
			//testcode
			wsprintf(G_cTxt, "Notify delete party: %d", i);
			PutLogList(G_cTxt);
		}
}

int CGame::HeroItemChecker(int iItemID, short sEnemyKill, char cContribution, char cSide) {
	switch (iItemID) {
		case 400: // AresdenHeroCape
			if (sEnemyKill != 0) sEnemyKill = 300;
			if (cContribution != 0) cContribution = 0;
			if (cSide != 0) cSide = 1;
			break;
		case 401: // ElvineHeroCape
			if (sEnemyKill != 0) sEnemyKill = 300;
			if (cContribution != 0) cContribution = 0;
			if (cSide != 0) cSide = 2;
			break;
			// eHeroHelm
		case 403: // aHeroHelm(M)
		case 404: // aHeroHelm(W)
			if (sEnemyKill != 0) sEnemyKill = 150;
			if (cContribution != 0) cContribution = 0;
			if (cSide != 0) cSide = 1;
			break;
		case 405: // aHeroHelm(M)
		case 406: // aHeroHelm(W)
			if (sEnemyKill != 0) sEnemyKill = 150;
			if (cContribution != 0) cContribution = 20;
			if (cSide != 0) cSide = 2;
			break;
			// eHeroCap
		case 407: // aHeroCap(M)
		case 408: // aHeroCap(W)
			if (sEnemyKill != 0) sEnemyKill = 100;
			if (cContribution != 0) cContribution = 20;
			if (cSide != 0) cSide = 1;
			break;
		case 409: // aHeroCap(M)
		case 410: // aHeroCap(W)
			if (sEnemyKill != 0) sEnemyKill = 100;
			if (cContribution != 0) cContribution = 20;
			if (cSide != 0) cSide = 2;
			break;
			// eHeroArmor
		case 411: // eHeroArmor(M)
		case 412: // eHeroArmor(W)
			if (sEnemyKill != 0) sEnemyKill = 300;
			if (cContribution != 0) cContribution = 30;
			if (cSide != 0) cSide = 1;
			break;
		case 413: // eHeroArmor(M)
		case 414: // eHeroArmor(W)
			if (sEnemyKill != 0) sEnemyKill = 300;
			if (cContribution != 0) cContribution = 30;
			if (cSide != 0) cSide = 2;
			break;
			// aHeroRobe
		case 415: // aHeroRobe(M)
		case 416: // aHeroRobe(W)
			if (sEnemyKill != 0) sEnemyKill = 200;
			if (cContribution != 0) cContribution = 20;
			if (cSide != 0) cSide = 1;
			break;
		case 417: // aHeroRobe(M)
		case 418: // aHeroRobe(W)
			if (sEnemyKill != 0) sEnemyKill = 200;
			if (cContribution != 0) cContribution = 20;
			if (cSide != 0) cSide = 2;
			break;
			// aHeroHauberk
		case 419: // aHeroHauberk(M)
		case 420: // aHeroHauberk(W)
			if (sEnemyKill != 0) sEnemyKill = 100;
			if (cContribution != 0) cContribution = 10;
			if (cSide != 0) cSide = 1;
			break;
		case 421: // eHeroHauberk(M)
		case 422: // eHeroHauberk(W)
			if (sEnemyKill != 0) sEnemyKill = 100;
			if (cContribution != 0) cContribution = 10;
			if (cSide != 0) cSide = 2;
			break;
			// aHeroLeggings
		case 423: // aHeroLeggings(M)
		case 424: // aHeroLeggings(W)
			if (sEnemyKill != 0) sEnemyKill = 150;
			if (cContribution != 0) cContribution = 15;
			if (cSide != 0) cSide = 1;
			break;
		case 425: // eHeroLeggings(M)
		case 426: // eHeroLeggings(W)
			if (sEnemyKill != 0) sEnemyKill = 150;
			if (cContribution != 0) cContribution = 15;
			if (cSide != 0) cSide = 2;
			break;
		case 427: // AresdenHeroCape+1
			if (cSide != 0) cSide = 1;
			break;
		case 428: // ElvineHeroCape+1
			if (cSide != 0) cSide = 2;
			break;
		default:
			return 0;
	}
	return iItemID;
}

/*void CGame::Command_GreenBall(CClient &client, char * pData, uint32_t dwMsgSize)
{
char   seps[] = "= \t\n";
char   * token, cBuff[256];
class  CStrTok * pStrTok;
char cPrevMode;
int i, j, sType, sTime;
uint32_t dwTime;
short itemeq;
	 iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
	if (client.m_pItemList[i] != nullptr) {
		switch (client.m_pItemList[i]->m_sIDnum) {
		case 652: iSoX++; iSoxH = i; break;
		}
	}
  dwTime = timeGetTime();
  if (client.markedForDeletion_) return;
  if ((dwMsgSize)   <= 0) return;
  std::memset(cBuff, 0, sizeof(cBuff));
  memcpy(cBuff, pData, dwMsgSize);
  pStrTok = new class CStrTok(cBuff, seps);
  token = pStrTok->pGet();
  token = pStrTok->pGet();
  if (token == nullptr) {
	  delete pStrTok;
	  return;
  }
  sTime = 600;
  for (i = 0; i < DEF_MAXMAPS; i++) {
	  if ((m_pMapList[i] != nullptr) && (m_pMapList[i]->m_bIsFixedDayMode == false)) {
		  cPrevMode = m_pMapList[i]->m_cWhetherStatus;
		  if (m_pMapList[i]->m_cWhetherStatus != nullptr) {
			  if ((dwTime - m_pMapList[i]->m_dwWhetherStartTime) > m_pMapList[i]->m_dwWhetherLastTime)
				  m_pMapList[i]->m_cWhetherStatus = nullptr;
		  }
		  else {
			  sTime *= 1000;
			  m_pMapList[i]->m_cWhetherStatus = sType;
			  m_pMapList[i]->m_dwWhetherStartTime = dwTime;
			  m_pMapList[i]->m_dwWhetherLastTime  = sTime;
		  }
		  if (cPrevMode != m_pMapList[i]->m_cWhetherStatus) {
			  for (j = 1; j < DEF_MAXCLIENTS; j++)
			  if ((m_pClientList[j] != nullptr) && (m_pClientList[j]->m_bIsInitComplete == true) && (m_pClientList[j]->m_cMapIndex == i))
				  m_pClientList[j]->SendNotifyMsg(0,DEF_NOTIFY_WHETHERCHANGE, m_pMapList[i]->m_cWhetherStatus, 0, 0, nullptr);
		  }
	  }
  }
	client.ItemDepleteHandler(itemeq, false, false);
}*/

/*bool CGame::bCheckFarmingSkill(char m_cFarmingSkill, char m_cCropSkill, char m_cHarvestCount)
{
 int iRoll;
	m_cFarmingSkill = ((m_cFarmingSkill - 20)/10)
	m_cCropSkill = ((m_cCropSkill - 20)/10)
	if ((m_cCropSkill < 0) && (m_cCropSkill > 8)) return false;
	if ((m_cFarmingSkill < 0) && (m_cFarmingSkill > 8)) return false;
	iRoll = iDice(1,100)
		if (m_cHarvestCount == 1) {
			if (iRoll >= (m_cCropSkill*m_cFarmingSkill)) {
				2Ah 42
			}
		}
		else if (m_cHarvestCount == 2) {
				28h 40
		}
}*/
void CGame::CalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, char cAttackerType, int iArmorType) {
	short sItemIndex;
	int iDownValue = 1, iHammerChance = 100;
	if (m_pClientList[sTargetH] == nullptr) return;
	if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (cAttackerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sAttackerH] != nullptr)) {
		if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_cSide != m_pClientList[sAttackerH]->m_cSide)) {
			switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) {
				case 14:
					if ((31 == ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4)) || (32 == ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4))) {
						sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND];
						if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
							if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 761) { // BattleHammer
								iDownValue = 10;
								break;
							}
							if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 762) { // GiantBattleHammer
								iDownValue = 10;
								break;
							}
							if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 843) { // BarbarianHammer
								iDownValue = 10;
								break;
							}
						}
					} else {
						iDownValue = 20;
						break;
					}
				case 10: iDownValue = 3;
					break;
				default: iDownValue = 1;
					break;
			}
			if (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true) {
				switch (m_pClientList[sTargetH]->m_iSpecialAbilityType)
					case 52: iDownValue = 0;
				iHammerChance = 0;
			}
		}
	}
	if ((m_pClientList[sTargetH]->m_cSide != 0) && (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > 0)) {
		m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan -= iDownValue;
	}
	if ((m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan <= 0) || (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > 64000)) {
		m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan = 0;
		m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, 0, nullptr);
		m_pClientList[sTargetH]->ReleaseItemHandler(iArmorType, true);
		return;
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sAttackerH] != nullptr) {
		if (cTargetType == DEF_OWNERTYPE_PLAYER) {
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND];
			if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 617) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 618) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 619) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 873) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 874) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 75) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 76)) {
					m_pClientList[sAttackerH]->m_sUsingWeaponSkill = 6;
					return;
				}
			}
		}
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sAttackerH] != nullptr) {
		if (cTargetType == DEF_OWNERTYPE_PLAYER) {
			if ((m_pClientList[sAttackerH]->m_sUsingWeaponSkill == 14) && (iHammerChance == 100)) {
				if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan < 2000) {
					iHammerChance = iDice(6, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
				} else {
					iHammerChance = iDice(4, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
				}
				if ((31 == ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4)) || (32 == ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4))) {
					sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND];
					if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
						if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 761) { // BattleHammer
							iHammerChance = ((iHammerChance * 10) / 5);
						}
						if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 762) { // GiantBattleHammer
							iHammerChance = ((iHammerChance * 10) / 4);
						}
						if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 843) { // GiantBattleHammer
							iHammerChance = ((iHammerChance * 10) / 5);
						}
					}
				}
				if ((m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_sIDnum == 622) || (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_sIDnum == 621)) {
					iHammerChance = 0;
				}
				if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan < iHammerChance) {
					wsprintf(G_cTxt, "(iHammerChance (%d), target armor endurance (%d)!", iHammerChance, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan);
					PutLogList(G_cTxt);
					m_pClientList[sTargetH]->ReleaseItemHandler(iArmorType, true);
					m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, 0, nullptr);
					return;
				}
			}
		}
	}
}

int CGame::iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, bool bNearAttack, bool bIsDash, bool bArrowUse) {
	int iAP_SM;
	int iAP_L;
	int iAttackerHitRatio;
	int iTargetDefenseRatio;
	int iDestHitRatio;
	int iResult;
	int iAP_Abs_Armor;
	int iAP_Abs_Shield;
	char cAttackerName[21], cAttackerDir, cAttackerSide, cTargetDir, cProtect, cCropSkill, cFarmingSkill;
	short sWeaponIndex;
	short sAttackerWeapon;
	short dX;
	short dY;
	short sX;
	short sY;
	short sAtkX;
	short sAtkY;
	short sTgtX;
	short sTgtY;
	uint32_t dwTime = timeGetTime();
	char cDamageMod[256];
	uint16_t wWeaponType = 0;
	double dTmp1;
	double dTmp2;
	double dTmp3;
	bool bKilled = false;
	bool bIsAttackerBerserk;
	int iKilledDice;
	int iDamage;
	int iExp = 0;
	int iWepLifeOff;
	int iSideCondition;
	int iMaxSuperAttack;
	int iWeaponSkill;
	int iComboBonus;
	int iTemp;
	int iMoveDamage;
	int iRepDamage;
	SpecialAbility cAttackerSA = SpecialAbility::NONE;
	int iAttackerSAvalue = 0;
	int iHitPoint;
	char cDamageMoveDir;
	int iPartyID = 0;
	int iWarContribution;
	int tX;
	int tY;
	int iDst1;
	int iDst2;
	short sItemIndex;
	short sSkillUsed;
	std::memset(cAttackerName, 0, sizeof (cAttackerName));
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sAttackerH] == nullptr) return 0;
			if ((m_bAdminSecurity) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return 0;
			if ((m_pClientList[sAttackerH]->map_->m_bIsAttackEnabled == false) && (m_pClientList[sAttackerH]->m_iAdminUserLevel == 0)) return 0;
			if ((m_pClientList[sAttackerH]->map_ == nullptr) && (m_pClientList[sAttackerH]->map_->m_bIsHeldenianMap == true) && (m_bIsHeldenianMode)) return 0;
			if ((!m_bIsCrusadeMode) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true) && (cTargetType == DEF_OWNERTYPE_PLAYER)) return 0;
			if ((m_pClientList[sAttackerH]->m_iStatus & 0x10) != 0) {
				SetInvisibilityFlag(sAttackerH, DEF_OWNERTYPE_PLAYER, false);
				delayEvents_.remove(sAttackerH, DEF_OWNERTYPE_PLAYER, MagicType::INVISIBILITY);
				m_pClientList[sAttackerH]->m_cMagicEffectStatus[MagicType::INVISIBILITY] = 0;
			}
			if ((m_pClientList[sAttackerH]->m_sAppr2 & 0xF000) == 0) return 0;
			iAP_SM = 0;
			iAP_L = 0;
			wWeaponType = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
			sSkillUsed = m_pClientList[sAttackerH]->m_sUsingWeaponSkill;
			if ((bIsDash) && (m_pClientList[sAttackerH]->m_cSkillMastery[sSkillUsed] != 100) && (wWeaponType != 25) && (wWeaponType != 27)) {
				wsprintf(G_cTxt, "TSearch Fullswing Hack: (%s) Player: (%s) - dashing with only (%d) weapon skill.", m_pClientList[sAttackerH]->m_cIPaddress, m_pClientList[sAttackerH]->m_cCharName, m_pClientList[sAttackerH]->m_cSkillMastery[sSkillUsed]);
				PutHackLogFileList(G_cTxt);
				m_pClientList[sAttackerH]->DeleteClient(true, true);
				return 0;
			}
			cAttackerSide = m_pClientList[sAttackerH]->m_cSide;
			if (wWeaponType == 0) {
				iAP_SM = iAP_L = iDice(1, ((m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iStr) / 12));
				if (iAP_SM <= 0) iAP_SM = 1;
				if (iAP_L <= 0) iAP_L = 1;
				iAttackerHitRatio = m_pClientList[sAttackerH]->m_iHitRatio + m_pClientList[sAttackerH]->m_cSkillMastery[5];
				m_pClientList[sAttackerH]->m_sUsingWeaponSkill = 5;
			} else if ((wWeaponType >= 1) && (wWeaponType < 40)) {
				iAP_SM = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_SM, m_pClientList[sAttackerH]->m_cAttackDiceRange_SM);
				iAP_L = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_L, m_pClientList[sAttackerH]->m_cAttackDiceRange_L);
				iAP_SM += m_pClientList[sAttackerH]->m_cAttackBonus_SM;
				iAP_L += m_pClientList[sAttackerH]->m_cAttackBonus_L;
				iAttackerHitRatio = m_pClientList[sAttackerH]->m_iHitRatio;
				dTmp1 = (double) iAP_SM;
				if ((m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iAngelicStr) <= 0)
					dTmp2 = 1.0f;
				else dTmp2 = (double) (m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iAngelicStr);
				dTmp2 = dTmp2 / 5.0f;
				dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
				iAP_SM = (int) (dTmp3 + 0.5f);
				dTmp1 = (double) iAP_L;
				if ((m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iAngelicStr) <= 0)
					dTmp2 = 1.0f;
				else dTmp2 = (double) (m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iAngelicStr);
				dTmp2 = dTmp2 / 5.0f;
				dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
				iAP_L = (int) (dTmp3 + 0.5f);
			} else if (wWeaponType >= 40) {
				iAP_SM = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_SM, m_pClientList[sAttackerH]->m_cAttackDiceRange_SM);
				iAP_L = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_L, m_pClientList[sAttackerH]->m_cAttackDiceRange_L);
				iAP_SM += m_pClientList[sAttackerH]->m_cAttackBonus_SM;
				iAP_L += m_pClientList[sAttackerH]->m_cAttackBonus_L;
				iAttackerHitRatio = m_pClientList[sAttackerH]->m_iHitRatio;
				iAP_SM += iDice(1, ((m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iAngelicStr) / 20));
				iAP_L += iDice(1, ((m_pClientList[sAttackerH]->m_iStr + m_pClientList[sAttackerH]->m_iAngelicStr) / 20));
			}
			iAttackerHitRatio += 50;
			if (iAP_SM <= 0) iAP_SM = 1;
			if (iAP_L <= 0) iAP_L = 1;
			if (m_pClientList[sAttackerH]->m_iCustomItemValue_Attack != 0) {
				if ((m_pClientList[sAttackerH]->m_iMinAP_SM != 0) && (iAP_SM < m_pClientList[sAttackerH]->m_iMinAP_SM)) {
					iAP_SM = m_pClientList[sAttackerH]->m_iMinAP_SM;
				}
				if ((m_pClientList[sAttackerH]->m_iMinAP_L != 0) && (iAP_L < m_pClientList[sAttackerH]->m_iMinAP_L)) {
					iAP_L = m_pClientList[sAttackerH]->m_iMinAP_L;
				}
				if ((m_pClientList[sAttackerH]->m_iMaxAP_SM != 0) && (iAP_SM > m_pClientList[sAttackerH]->m_iMaxAP_SM)) {
					iAP_SM = m_pClientList[sAttackerH]->m_iMaxAP_SM;
				}
				if ((m_pClientList[sAttackerH]->m_iMaxAP_L != 0) && (iAP_L > m_pClientList[sAttackerH]->m_iMaxAP_L)) {
					iAP_L = m_pClientList[sAttackerH]->m_iMaxAP_L;
				}
			}
			if (m_pClientList[sAttackerH]->m_cHeroArmourBonus == 1) {
				iAttackerHitRatio += 100;
				iAP_SM += 5;
				iAP_L += 5;
			}
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::RHAND];
			if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 851) || // KlonessEsterk
						  (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 863) || // KlonessWand(MS.20)
						  (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 864)) { // KlonessWand(MS.10)
					if (m_pClientList[sAttackerH]->m_iRating > 0) {
						iRepDamage = m_pClientList[sAttackerH]->m_iRating / 100;
						if (iRepDamage < 5) iRepDamage = 5;
						iAP_SM += iRepDamage;
						iAP_L += iRepDamage;
					}
					if (cTargetType == DEF_OWNERTYPE_PLAYER) {
						if (m_pClientList[sTargetH] == nullptr) return 0;
						if (m_pClientList[sTargetH]->m_iRating < 0) {
							iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 10);
							if (iRepDamage > 10) iRepDamage = 10;
							iAP_SM += iRepDamage;
							iAP_L += iRepDamage;
						}
					}
				}
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 861) || // BerserkWand(MS.20)
						  (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 862)) { // BerserkWand(MS.10)
					iAP_SM += 1;
					iAP_L += 1;
				}
			}
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND];
			if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 847) &&
						  (m_cDayOrNight == 2)) {
					iAP_SM += 4;
					iAP_L += 4;
				}
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 848) &&
						  (m_cDayOrNight == 1)) {
					iAP_SM += 4;
					iAP_L += 4;
				}
				// +++ Damage a demons con demon slayer...
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 616) && (m_pNpcList[sTargetH]->m_sType == 31)) {
					iAP_L += 5;
				}
				if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 849) || // KlonessBlade
						  (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 850)) { // KlonessAxe
					if (m_pClientList[sAttackerH]->m_iRating > 0) {
						iRepDamage = m_pClientList[sAttackerH]->m_iRating / 100;
						if (iRepDamage < 5) iRepDamage = 5;
						iAP_SM += iRepDamage;
						iAP_L += iRepDamage;
					}
					if (cTargetType == DEF_OWNERTYPE_PLAYER) {
						if (m_pClientList[sTargetH] == nullptr) return 0;
						if (m_pClientList[sTargetH]->m_iRating < 0) {
							iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 10);
							if (iRepDamage > 10) iRepDamage = 10;
							iAP_SM += iRepDamage;
							iAP_L += iRepDamage;
						}
					}
				}
			}
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::NECK];
			if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != nullptr)) {
				if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 859) { // NecklaceOfKloness
					if (cTargetType == DEF_OWNERTYPE_PLAYER) {
						if (m_pClientList[sTargetH] == nullptr) return 0;
						iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 20);
						if (iRepDamage > 5) iRepDamage = 5;
						iAP_SM += iRepDamage;
						iAP_L += iRepDamage;
					}
				}
			}
			cAttackerDir = m_pClientList[sAttackerH]->m_cDir;
			strcpy(cAttackerName, m_pClientList[sAttackerH]->m_cCharName);
			if (m_pClientList[sAttackerH]->m_cMagicEffectStatus[MagicType::BERSERK] != 0)
				bIsAttackerBerserk = true;
			else bIsAttackerBerserk = false;
			if ((!bArrowUse) && (m_pClientList[sAttackerH]->m_iSuperAttackLeft > 0) && (iAttackMode >= 20)) {
				dTmp1 = (double) iAP_SM;
				dTmp2 = (double) m_pClientList[sAttackerH]->m_iLevel;
				dTmp3 = dTmp2 / 100.0f;
				dTmp2 = dTmp1 * dTmp3;
				iTemp = (int) (dTmp2 + 0.5f);
				iAP_SM += iTemp;
				dTmp1 = (double) iAP_L;
				dTmp2 = (double) m_pClientList[sAttackerH]->m_iLevel;
				dTmp3 = dTmp2 / 100.0f;
				dTmp2 = dTmp1 * dTmp3;
				iTemp = (int) (dTmp2 + 0.5f);
				iAP_L += iTemp;
				switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) {
					case 6: iAP_SM += (iAP_SM / 10);
						iAP_L += (iAP_L / 10);
						iAttackerHitRatio += 30;
						break;
					case 7: iAP_SM *= 1;
						iAP_L *= 1;
						break;
					case 8: iAP_SM += (iAP_SM / 10);
						iAP_L += (iAP_L / 10);
						iAttackerHitRatio += 30;
						break;
					case 10: iAP_SM += (iAP_SM / 5);
						iAP_L += (iAP_L / 5);
						break;
					case 14: iAP_SM += (iAP_SM / 5);
						iAP_L += (iAP_L / 5);
						iAttackerHitRatio += 20;
						break;
					case 21: iAP_SM += (iAP_SM / 5);
						iAP_L += (iAP_L / 5);
						iAttackerHitRatio += 50;
						break;
					default: break;
				}
				iAttackerHitRatio += 100;
				iAttackerHitRatio += m_pClientList[sAttackerH]->m_iCustomItemValue_Attack;
			}
			if (bIsDash) {
				iAttackerHitRatio += 20;
				switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) {
					case 8: iAP_SM += (iAP_SM / 10);
						iAP_L += (iAP_L / 10);
						break;
					case 10: iAP_SM += (iAP_SM / 5);
						iAP_L += (iAP_L / 5);
						break;
					case 14: iAP_SM += (iAP_SM / 5);
						iAP_L += (iAP_L / 5);
						break;
					default: break;
				}
			}
			iAttackerHitRatio += m_pClientList[sAttackerH]->m_iAddAR;
			sAtkX = m_pClientList[sAttackerH]->m_sX;
			sAtkY = m_pClientList[sAttackerH]->m_sY;
			iPartyID = m_pClientList[sAttackerH]->m_iPartyID;
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sAttackerH] == nullptr) return 0;
			if (m_pNpcList[sAttackerH]->map_->m_bIsAttackEnabled == false) return 0;
			if ((m_pNpcList[sAttackerH]->m_iStatus & 0x10) != 0) {
				SetInvisibilityFlag(sAttackerH, DEF_OWNERTYPE_NPC, false);
				delayEvents_.remove(sAttackerH, DEF_OWNERTYPE_NPC, MagicType::INVISIBILITY);
				m_pNpcList[sAttackerH]->m_cMagicEffectStatus[MagicType::INVISIBILITY] = 0;
			}
			cAttackerSide = m_pNpcList[sAttackerH]->m_cSide;
			iAP_SM = 0;
			iAP_L = 0;
			if (m_pNpcList[sAttackerH]->m_cAttackDiceThrow != 0)
				iAP_L = iAP_SM = iDice(m_pNpcList[sAttackerH]->m_cAttackDiceThrow, m_pNpcList[sAttackerH]->m_cAttackDiceRange);
			iAttackerHitRatio = m_pNpcList[sAttackerH]->m_iHitRatio;
			cAttackerDir = m_pNpcList[sAttackerH]->m_cDir;
			memcpy(cAttackerName, m_pNpcList[sAttackerH]->m_cNpcName, 20);
			if (m_pNpcList[sAttackerH]->m_cMagicEffectStatus[MagicType::BERSERK] != 0)
				bIsAttackerBerserk = true;
			else bIsAttackerBerserk = false;
			cAttackerSA = m_pNpcList[sAttackerH]->m_cSpecialAbility;
			sAtkX = m_pNpcList[sAttackerH]->m_sX;
			sAtkY = m_pNpcList[sAttackerH]->m_sY;
			break;
	}
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return 0;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return 0;
			if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return 0;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (!m_bIsCrusadeMode) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return 0;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsOwnLocation == true)) return 0;
			if ((m_pClientList[sTargetH]->m_sX != tdX) || (m_pClientList[sTargetH]->m_sY != tdY)) return 0;
			if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return 0;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsNeutral == true)
					  && (m_pClientList[sTargetH]->m_iPKCount == 0)) return 0;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return 0;
			cTargetDir = m_pClientList[sTargetH]->m_cDir;
			iTargetDefenseRatio = m_pClientList[sTargetH]->m_iDefenseRatio;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true)) {
				iSideCondition = m_pClientList[sAttackerH]->iGetPlayerRelationship(sTargetH);
				if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {
					iAP_SM = iAP_SM / 2;
					iAP_L = iAP_L / 2;
				} else {
					if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true) {
						if (m_pClientList[sAttackerH]->m_iGuildGUID == m_pClientList[sTargetH]->m_iGuildGUID) return 0;
						else {
							iAP_SM = iAP_SM / 2;
							iAP_L = iAP_L / 2;
						}
					} else return 0;
				}
			}
			iTargetDefenseRatio += m_pClientList[sTargetH]->m_iAddDR;
			sTgtX = m_pClientList[sTargetH]->m_sX;
			sTgtY = m_pClientList[sTargetH]->m_sY;
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return 0;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return 0;
			if ((m_pNpcList[sTargetH]->m_sX != tdX) || (m_pNpcList[sTargetH]->m_sY != tdY)) return 0;
			cTargetDir = m_pNpcList[sTargetH]->m_cDir;
			iTargetDefenseRatio = m_pNpcList[sTargetH]->m_iDefenseRatio;
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				switch (m_pNpcList[sTargetH]->m_sType) {
					case 40:
					case 41:
						if ((m_pClientList[sAttackerH]->m_cSide == 0) || (m_pNpcList[sTargetH]->m_cSide == m_pClientList[sAttackerH]->m_cSide)) return 0;
						break;
				}
				if ((wWeaponType == 25) && (m_pNpcList[sTargetH]->m_cActionLimit == 5) && (m_pNpcList[sTargetH]->m_iBuildCount > 0)) {
					if ((m_pClientList[sAttackerH]->m_iCrusadeDuty != 2) && (m_pClientList[sAttackerH]->m_iAdminUserLevel == 0)) break;
					switch (m_pNpcList[sTargetH]->m_sType) {
						case 36:
						case 37:
						case 38:
						case 39:
							// administrators instantly build crusade structures
							if (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0) {
								m_pNpcList[sTargetH]->m_sAppr2 = 0;
								m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							}
							switch (m_pNpcList[sTargetH]->m_iBuildCount) {
								case 1:
									m_pNpcList[sTargetH]->m_sAppr2 = 0;
									m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									switch (m_pNpcList[sTargetH]->m_sType) {
										case 36:
											iWarContribution = 700;
											break;
										case 37:
											iWarContribution = 700;
											break;
										case 38:
											iWarContribution = 500;
											break;
										case 39:
											iWarContribution = 500;
											break;
									}
									m_pClientList[sAttackerH]->m_iWarContribution += iWarContribution;
									if (m_pClientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
										m_pClientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
									wsprintf(G_cTxt, "Construction Complete! WarContribution: +%d", iWarContribution);
									PutLogList(G_cTxt);
									m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 0, nullptr);
									break;
								case 5:
									m_pNpcList[sTargetH]->m_sAppr2 = 1;
									m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									break;
								case 10:
									m_pNpcList[sTargetH]->m_sAppr2 = 2;
									m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									break;
							}
							break;
					}
					m_pNpcList[sTargetH]->m_iBuildCount--;
					if (m_pNpcList[sTargetH]->m_iBuildCount <= 0) {
						m_pNpcList[sTargetH]->m_iBuildCount = 0;
					}
					return 0;
				}
				if ((wWeaponType == 27) && (m_pNpcList[sTargetH]->m_cCropType != 0) && (m_pNpcList[sTargetH]->m_cActionLimit == 5) && (m_pNpcList[sTargetH]->m_iBuildCount > 0)) {
					cFarmingSkill = m_pClientList[sAttackerH]->m_cSkillMastery[2];
					cCropSkill = m_pNpcList[sTargetH]->m_cCropSkill;
					if (cFarmingSkill < 20) return 0;
					if (m_pClientList[sAttackerH]->m_iLevel < 20) return 0;
					switch (m_pNpcList[sTargetH]->m_sType) {
						case 64:
							switch (m_pNpcList[sTargetH]->m_iBuildCount) {
								case 1:
									m_pNpcList[sTargetH]->m_sAppr2 = 3;
									m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									//sub_4B67E0
									m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(2, cFarmingSkill <= cCropSkill + 10);
									m_pClientList[sAttackerH]->_CheckFarmingAction(*m_pNpcList[sTargetH], 1);
									m_pNpcList[sTargetH]->DeleteNpc();
									return 0;
								case 8:
									m_pNpcList[sTargetH]->m_sAppr2 = 3;
									m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(2, cFarmingSkill <= cCropSkill + 10);
									m_pClientList[sAttackerH]->_CheckFarmingAction(*m_pNpcList[sTargetH], 0);
									break;
								case 18:
									m_pNpcList[sTargetH]->m_sAppr2 = 2;
									m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(2, cFarmingSkill <= cCropSkill + 10);
									m_pClientList[sAttackerH]->_CheckFarmingAction(*m_pNpcList[sTargetH], 0);
									break;
							}
							break;
					}
					m_pNpcList[sTargetH]->m_iBuildCount--;
					if (m_pNpcList[sTargetH]->m_iBuildCount <= 0) {
						m_pNpcList[sTargetH]->m_iBuildCount = 0;
					}
					return 0;
				}
			}
			sTgtX = m_pNpcList[sTargetH]->m_sX;
			sTgtY = m_pNpcList[sTargetH]->m_sY;
			break;
	}
	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (cTargetType == DEF_OWNERTYPE_PLAYER)) {
		sX = m_pClientList[sAttackerH]->m_sX;
		sY = m_pClientList[sAttackerH]->m_sY;
		dX = m_pClientList[sTargetH]->m_sX;
		dY = m_pClientList[sTargetH]->m_sY;
		if (m_pClientList[sTargetH]->map_->iGetAttribute(sX, sY, 0x00000006) != 0) return 0;
		if (m_pClientList[sTargetH]->map_->iGetAttribute(dX, dY, 0x00000006) != 0) return 0;
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		if ((m_pClientList[sAttackerH]->m_iDex + m_pClientList[sAttackerH]->m_iAngelicDex) > 50) {
			iAttackerHitRatio += ((m_pClientList[sAttackerH]->m_iDex + m_pClientList[sAttackerH]->m_iAngelicDex) - 50);
		}
	}
	if (wWeaponType >= 40) {
		switch (m_pClientList[sAttackerH]->map_->m_cWhetherStatus) {
			case 0: break;
			case 1: iAttackerHitRatio -= (iAttackerHitRatio / 20);
				break;
			case 2: iAttackerHitRatio -= (iAttackerHitRatio / 10);
				break;
			case 3: iAttackerHitRatio -= (iAttackerHitRatio / 4);
				break;
		}
	}
	if (iAttackerHitRatio < 0) iAttackerHitRatio = 0;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT];
			break;
		case DEF_OWNERTYPE_NPC:
			cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT];
			break;
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND] != -1) {
			if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND]] == nullptr) {
				m_pClientList[sAttackerH]->m_bIsItemEquipped[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND]] = false;
				m_pClientList[sAttackerH]->DeleteClient(true, true);
				return 0;
			}
			if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND]]->m_sItemEffectType == ItemEffectType::ATTACK_ARROW) {
				if (m_pClientList[sAttackerH]->m_cArrowIndex == -1) {
					return 0;
				} else {
					if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex] == nullptr)
						return 0;
					if (!bArrowUse)
						m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount--;
					if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount <= 0) {
						m_pClientList[sAttackerH]->ItemDepleteHandler(m_pClientList[sAttackerH]->m_cArrowIndex, false, true);
						m_pClientList[sAttackerH]->m_cArrowIndex = m_pClientList[sAttackerH]->_iGetArrowItemIndex();
					} else {
						m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_SETITEMCOUNT, m_pClientList[sAttackerH]->m_cArrowIndex, m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount, false, nullptr);
						m_pClientList[sAttackerH]->iCalcTotalWeight();
					}
				}
				if (cProtect == 1) return 0;
			} else {
				switch (cProtect) {
					case 3: iTargetDefenseRatio += 40;
						break;
					case 4: iTargetDefenseRatio += 100;
						break;
				}
				if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
			}
		}
	} else {
		switch (cProtect) {
			case 1:
				switch (m_pNpcList[sAttackerH]->m_sType) {
					case 54:
						if ((abs(sTgtX - m_pNpcList[sAttackerH]->m_sX) >= 1) || (abs(sTgtY - m_pNpcList[sAttackerH]->m_sY) >= 1)) return 0;
				}
				break;
			case 3: iTargetDefenseRatio += 40;
				break;
			case 4: iTargetDefenseRatio += 100;
				break;
		}
		if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
	}
	if (cAttackerDir == cTargetDir) iTargetDefenseRatio = iTargetDefenseRatio / 2;
	if (iTargetDefenseRatio < 1) iTargetDefenseRatio = 1;
	dTmp1 = (double) (iAttackerHitRatio);
	dTmp2 = (double) (iTargetDefenseRatio);
	dTmp3 = (dTmp1 / dTmp2) * 50.0f;
	iDestHitRatio = (int) (dTmp3);
	if (iDestHitRatio < DEF_MINIMUMHITRATIO) iDestHitRatio = DEF_MINIMUMHITRATIO;
	if (iDestHitRatio > DEF_MAXIMUMHITRATIO) iDestHitRatio = DEF_MAXIMUMHITRATIO;
	if ((bIsAttackerBerserk) && (iAttackMode < 20)) {
		iAP_SM = iAP_SM * 2;
		iAP_L = iAP_L * 2;
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		iAP_SM += m_pClientList[sAttackerH]->m_iAddPhysicalDamage;
		iAP_L += m_pClientList[sAttackerH]->m_iAddPhysicalDamage;
	}
	if (bNearAttack) {
		iAP_SM = iAP_SM / 2;
		iAP_L = iAP_L / 2;
	}
	if (cTargetType == DEF_OWNERTYPE_PLAYER) {
		iAP_SM -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
		iAP_L -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		if (iAP_SM <= 1) iAP_SM = 1;
		if (iAP_L <= 1) iAP_L = 1;
	} else {
		if (iAP_SM <= 0) iAP_SM = 0;
		if (iAP_L <= 0) iAP_L = 0;
	}
	iResult = iDice(1, 100);
	int unknownVar = 0;
	if (iResult <= iDestHitRatio) {
		if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
			if (((m_pClientList[sAttackerH]->m_iHungerStatus <= 10) || (m_pClientList[sAttackerH]->m_iSP <= 0)) && (iDice(1, 10) == 5)) return false;
			m_pClientList[sAttackerH]->m_iComboAttackCount++;
			if (m_pClientList[sAttackerH]->m_iComboAttackCount < 0) m_pClientList[sAttackerH]->m_iComboAttackCount = 0;
			if (m_pClientList[sAttackerH]->m_iComboAttackCount > 4) m_pClientList[sAttackerH]->m_iComboAttackCount = 1;
			iWeaponSkill = m_pClientList[sAttackerH]->_iGetWeaponSkillType();
			iComboBonus = iGetComboAttackBonus(iWeaponSkill, m_pClientList[sAttackerH]->m_iComboAttackCount);
			if ((m_pClientList[sAttackerH]->m_iComboAttackCount > 1) && (m_pClientList[sAttackerH]->m_iAddCD != 0))
				iComboBonus += m_pClientList[sAttackerH]->m_iAddCD;
			iAP_SM += iComboBonus;
			iAP_L += iComboBonus;
			switch (m_pClientList[sAttackerH]->m_iSpecialWeaponEffectType) {
				case 0: break;
				case 1:
					if ((m_pClientList[sAttackerH]->m_iSuperAttackLeft > 0) && (iAttackMode >= 20)) {
						iAP_SM += m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue;
						iAP_L += m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue;
					}
					break;
				case 2:
					unknownVar = 61;
					iAttackerSAvalue = m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue * 5;
					break;
				case 3:
					unknownVar = 62;
					break;
			}
			if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true) {
				iAP_SM += iAP_SM / 3;
				iAP_L += iAP_L / 3;
			}
			if (m_pClientList[sAttackerH]->bCheckHeldenianMap() == 1) {
				iAP_SM += iAP_SM / 3;
				iAP_L += iAP_L / 3;
			}
			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
				if (m_pClientList[sAttackerH]->m_iLevel <= 80) {
					iAP_SM += iAP_SM;
					iAP_L += iAP_L;
				} else if (m_pClientList[sAttackerH]->m_iLevel <= 100) {
					iAP_SM += (iAP_SM * 7) / 10;
					iAP_L += (iAP_L * 7) / 10;
				} else {
					iAP_SM += iAP_SM / 3;
					iAP_L += iAP_L / 3;
				}
			}
		}
		switch (cTargetType) {
			case DEF_OWNERTYPE_PLAYER:
				m_pClientList[sTargetH]->ClearSkillUsingStatus();
				if ((dwTime - m_pClientList[sTargetH]->m_dwTime) > DEF_RAGPROTECTIONTIME) {
					return 0;
				} else {
					switch (unknownVar) {
						case 62:
							if (m_pClientList[sTargetH]->m_iRating < 0) {
								iTemp = abs(m_pClientList[sTargetH]->m_iRating) / 10;
								if (iTemp > 10) iTemp = 10;
								iAP_SM += iTemp;
							}
							break;
					}
					iAP_Abs_Armor = 0;
					iAP_Abs_Shield = 0;
					iTemp = iDice(1, 10000);
					if ((iTemp >= 1) && (iTemp < 5000)) iHitPoint = 1;
					else if ((iTemp >= 5000) && (iTemp < 7500)) iHitPoint = 2;
					else if ((iTemp >= 7500) && (iTemp < 9000)) iHitPoint = 3;
					else if ((iTemp >= 9000) && (iTemp <= 10000)) iHitPoint = 4;
					switch (iHitPoint) {
						case 1:
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::BODY] > 0) {
								if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::BODY] >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::BODY];
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
						case 2:
							if ((m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::PANTS] +
									  m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::LEGGINGS]) > 0) {
								if ((m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::PANTS] +
										  m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::LEGGINGS]) >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::PANTS] + m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::LEGGINGS]);
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
						case 3:
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::ARMS] > 0) {
								if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::ARMS] >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::ARMS];
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
						case 4:
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::HEAD] > 0) {
								if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::HEAD] >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[EquipPos::HEAD];
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
					}
					// Physical absortion by KahBur.
					if (m_pClientList[sTargetH]->m_iAddAbsPD != 0) {
						dTmp1 = (double) iAP_SM;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsPD;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iAP_SM = iAP_SM - (int) dTmp3;
						dTmp1 = (double) iAP_L;
						dTmp2 = (double) m_pClientList[sTargetH]->m_iAddAbsPD;
						dTmp3 = (dTmp2 / 100.0f) * dTmp1;
						iAP_L = iAP_L - (int) dTmp3;
					}
					if (m_pClientList[sTargetH]->m_iDamageAbsorption_Shield > 0) {
						if (iDice(1, 100) <= (m_pClientList[sTargetH]->m_cSkillMastery[11])) {
							m_pClientList[sTargetH]->CalculateSSN_SkillIndex(11, 1);
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Shield >= 80)
								dTmp1 = 80.0f;
							else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Shield;
							dTmp2 = (double) iAP_SM;
							dTmp3 = (dTmp1 / 100.0f) * dTmp2;
							iAP_Abs_Shield = (int) dTmp3;
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[EquipPos::LHAND];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								if ((m_pClientList[sTargetH]->m_cSide != 0) && (m_pClientList[sTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
									m_pClientList[sTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan--;
								if (m_pClientList[sTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan == 0) {
									m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, nullptr);
									m_pClientList[sTargetH]->ReleaseItemHandler(iTemp, true);
								}
							}
						}
					}
					iAP_SM = iAP_SM - (iAP_Abs_Armor + iAP_Abs_Shield);
					if (iAP_SM <= 0) iAP_SM = 1;
					if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr) && (m_pClientList[sAttackerH]->m_bIsSpecialAbilityEnabled == true)) {
						switch (m_pClientList[sAttackerH]->m_iSpecialAbilityType) {
							case 0: break;
							case 1:
								iTemp = (m_pClientList[sTargetH]->m_iHP / 2);
								if (iTemp > iAP_SM) iAP_SM = iTemp;
								if (iAP_SM <= 0) iAP_SM = 1;
								break;
							case 2:
								if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::ICE] == 0) {
									m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::ICE] = 1;
									SetIceFlag(sTargetH, DEF_OWNERTYPE_PLAYER, true);
									delayEvents_.add(DelayEventType::MAGICRELEASE, MagicType::ICE, dwTime + 30000,
											  sTargetH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
									m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, MagicType::ICE, 1, 0, nullptr);
								}
								break;
							case 3:
								if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] == 0) {
									m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 2;
									delayEvents_.add(DelayEventType::MAGICRELEASE, MagicType::HOLDOBJECT, dwTime + 10000,
											  sTargetH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 10, 0, 0);
									m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, MagicType::HOLDOBJECT, 10, 0, nullptr);
								}
								break;
							case 4:
								iAP_SM = (m_pClientList[sTargetH]->m_iHP);
								break;
							case 5:
								m_pClientList[sAttackerH]->m_iHP += iAP_SM;
								if (m_pClientList[sAttackerH]->iGetMaxHP() < m_pClientList[sAttackerH]->m_iHP) m_pClientList[sAttackerH]->m_iHP = m_pClientList[sAttackerH]->iGetMaxHP();
								m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
								break;
						}
					}
					if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr) && (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true)) {
						switch (m_pClientList[sTargetH]->m_iSpecialAbilityType) {
							case 50:
								if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND] != -1)
									sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND];
								else sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::RHAND];
								if (sWeaponIndex != -1) m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
								break;
							case 51:
								if (iHitPoint == m_pClientList[sTargetH]->m_iSpecialAbilityEquipPos)
									iAP_SM = 0;
								break;
							case 52:
								iAP_SM = 0;
								break;
						}
					}
					if ((m_pClientList[sTargetH]->m_bIsLuckyEffect == true) &&
							  (iDice(1, 10) == 5) && (m_pClientList[sTargetH]->m_iHP <= iAP_SM)) {
						iAP_SM = m_pClientList[sTargetH]->m_iHP - 1;
					}
					switch (iHitPoint) {
						case 1:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[EquipPos::BODY];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							}
							break;
						case 2:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[EquipPos::PANTS];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							} else {
								iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[EquipPos::LEGGINGS];
								if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
									CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
								}
							}
							break;
						case 3:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[EquipPos::ARMS];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							}
							break;
						case 4:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[EquipPos::HEAD];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							}
							break;
					}
					if ((cAttackerSA == SpecialAbility::ANTI_MAGIC_PROT) && (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] != 0)) {
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::PROTECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT], 0, nullptr);
						switch (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT]) {
							case 1:
								SetProtectionFromArrowFlag(sTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
							case 2:
							case 5:
								SetMagicProtectionFlag(sTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
							case 3:
							case 4:
								SetDefenseShieldFlag(sTargetH, DEF_OWNERTYPE_PLAYER, false);
								break;
						}
						m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, MagicType::PROTECT);
					}
					if ((m_pClientList[sTargetH]->m_bIsPoisoned == false) &&
							  ((cAttackerSA == SpecialAbility::POISONOUS) || (cAttackerSA == SpecialAbility::EXTREMELY_POISONOUS) || (unknownVar == 61))) {
						if (bCheckResistingPoisonSuccess(sTargetH, DEF_OWNERTYPE_PLAYER) == false) {
							m_pClientList[sTargetH]->m_bIsPoisoned = true;
							if (cAttackerSA == SpecialAbility::POISONOUS) m_pClientList[sTargetH]->m_iPoisonLevel = 15;
							else if (cAttackerSA == SpecialAbility::EXTREMELY_POISONOUS) m_pClientList[sTargetH]->m_iPoisonLevel = 40;
							else if (unknownVar == 61) m_pClientList[sTargetH]->m_iPoisonLevel = iAttackerSAvalue;
							m_pClientList[sTargetH]->m_dwPoisonTime = dwTime;
							m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, MagicType::POISON, m_pClientList[sTargetH]->m_iPoisonLevel, 0, nullptr);
							SetPoisonFlag(sTargetH, DEF_OWNERTYPE_PLAYER, true);
						}
					}
					m_pClientList[sTargetH]->m_iHP -= iAP_SM;
					if (m_pClientList[sAttackerH] != nullptr) {
						//acidx pwns
						if (m_pClientList[sAttackerH]->iDmgShowon == 1) {
							std::memset(cDamageMod, 0, sizeof (cDamageMod));
							wsprintf(cDamageMod, "You did [%d] Damage to Character [%s] Remaining Life [%d]", iAP_SM, m_pClientList[sTargetH]->m_cCharName, m_pClientList[sTargetH]->m_iHP);
							m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
						}
					}
					if (m_pClientList[sTargetH]->m_iHP <= 0) {
						if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
							//		bAnalyzeCriminalAction(sAttackerH, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						}
						m_pClientList[sTargetH]->ClientKilledHandler(sAttackerH, cAttackerType, iAP_SM);
						bKilled = true;
						iKilledDice = m_pClientList[sTargetH]->m_iLevel;
					} else {
						if (iAP_SM > 0) {
							if (m_pClientList[sTargetH]->m_iAddTransMana > 0) {
								dTmp1 = (double) m_pClientList[sTargetH]->m_iAddTransMana;
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iTemp = (2 * (m_pClientList[sTargetH]->m_iMag + m_pClientList[sTargetH]->m_iAngelicMag)) + (2 * m_pClientList[sTargetH]->m_iLevel) + ((m_pClientList[sTargetH]->m_iInt + m_pClientList[sTargetH]->m_iAngelicInt) / 2);
								m_pClientList[sTargetH]->m_iMP += (int) dTmp3;
								if (m_pClientList[sTargetH]->m_iMP > iTemp) m_pClientList[sTargetH]->m_iMP = iTemp;
							}
							if (m_pClientList[sTargetH]->m_iAddChargeCritical > 0) {
								if (iDice(1, 100) < (m_pClientList[sTargetH]->m_iAddChargeCritical)) {
									iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
									if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
									m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
								}
							}
							m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
							if (cAttackerType == DEF_OWNERTYPE_PLAYER)
								sAttackerWeapon = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
							else sAttackerWeapon = 1;
							if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true))
								iMoveDamage = 60;
							else iMoveDamage = 40;
							if (iAP_SM >= iMoveDamage) {
								if (sTgtX == sAtkX) {
									if (sTgtY == sAtkY) goto CAE_SKIPDAMAGEMOVE;
									else if (sTgtY > sAtkY) cDamageMoveDir = 5;
									else if (sTgtY < sAtkY) cDamageMoveDir = 1;
								} else if (sTgtX > sAtkX) {
									if (sTgtY == sAtkY) cDamageMoveDir = 3;
									else if (sTgtY > sAtkY) cDamageMoveDir = 4;
									else if (sTgtY < sAtkY) cDamageMoveDir = 2;
								} else if (sTgtX < sAtkX) {
									if (sTgtY == sAtkY) cDamageMoveDir = 7;
									else if (sTgtY > sAtkY) cDamageMoveDir = 6;
									else if (sTgtY < sAtkY) cDamageMoveDir = 8;
								}
								m_pClientList[sTargetH]->m_iLastDamage = iAP_SM;
								m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_DAMAGEMOVE, cDamageMoveDir, iAP_SM, sAttackerWeapon, nullptr);
							} else {
CAE_SKIPDAMAGEMOVE:
								;
								int iProb;
								if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
									switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) {
										case 6: iProb = 3500;
											break;
										case 8: iProb = 1000;
											break;
										case 9: iProb = 2900;
											break;
										case 10: iProb = 2500;
											break;
										case 14: iProb = 2000;
											break;
										case 21: iProb = 2000;
											break;
										default: iProb = 1;
											break;
									}
								} else iProb = 1;
								if (iDice(1, 10000) >= iProb)
									m_pClientList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iAP_SM, sAttackerWeapon, 0);
							}
							if (m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] == 1) {
								m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT], 0, nullptr);
								m_pClientList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
								delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, MagicType::HOLDOBJECT);
							}
							m_pClientList[sTargetH]->m_iSuperAttackCount++;
							if (m_pClientList[sTargetH]->m_iSuperAttackCount > 14) {
								m_pClientList[sTargetH]->m_iSuperAttackCount = 0;
								iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
								if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
								m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, nullptr);
							}
						}
					}
				}
				break;
			case DEF_OWNERTYPE_NPC:
				if (m_pNpcList[sTargetH]->m_cBehavior == DEF_BEHAVIOR_DEAD) return 0;
				if (m_pNpcList[sTargetH]->m_bIsKilled == true) return 0;
				if (m_bIsCrusadeMode) {
					if (cAttackerSide == m_pNpcList[sTargetH]->m_cSide) {
						switch (m_pNpcList[sTargetH]->m_sType) {
							case 40:
							case 41:
							case 43:
							case 44:
							case 45:
							case 46:
							case 47:
							case 51:
								return 0;
							default: break;
						}
					} else {
						switch (m_pNpcList[sTargetH]->m_sType) {
							case 41:
								if (cAttackerSide != 0) {
									m_pNpcList[sTargetH]->m_iV1 += iAP_L;
									if (m_pNpcList[sTargetH]->m_iV1 > 500) {
										m_pNpcList[sTargetH]->m_iV1 = 0;
										m_pNpcList[sTargetH]->m_iManaStock--;
										if (m_pNpcList[sTargetH]->m_iManaStock <= 0) m_pNpcList[sTargetH]->m_iManaStock = 0;
										wsprintf(G_cTxt, "ManaStock down: %d", m_pNpcList[sTargetH]->m_iManaStock);
										PutLogList(G_cTxt);
									}
								}
								break;
						}
					}
				}
				switch (m_pNpcList[sTargetH]->m_cActionLimit) {
					case 1:
					case 2:
						return 0;
				}
				if (m_pNpcList[sTargetH]->m_cSize == 0)
					iDamage = iAP_SM;
				else iDamage = iAP_L;
				if (m_pNpcList[sTargetH]->m_iAbsDamage < 0) {
					dTmp1 = (double) iDamage;
					dTmp2 = (double) (abs(m_pNpcList[sTargetH]->m_iAbsDamage)) / 100.0f;
					dTmp3 = dTmp1 * dTmp2;
					dTmp2 = dTmp1 - dTmp3;
					iDamage = (int) dTmp2;
					if (iDamage < 0) iDamage = 1;
					else if ((m_pNpcList[sTargetH]->m_sType == 31) && (cAttackerType == 1) && (m_pClientList[sAttackerH] != nullptr) && (m_pClientList[sAttackerH]->m_iSpecialAbilityType == 7))
						iDamage += iDice(3, 2);
				}
				if ((cAttackerSA == SpecialAbility::ANTI_MAGIC_PROT) && (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] != 0)) {
					switch (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT]) {
						case 1:
							SetProtectionFromArrowFlag(sTargetH, DEF_OWNERTYPE_NPC, false);
							break;
						case 2:
						case 5:
							SetMagicProtectionFlag(sTargetH, DEF_OWNERTYPE_NPC, false);
							break;
						case 3:
						case 4:
							SetDefenseShieldFlag(sTargetH, DEF_OWNERTYPE_NPC, false);
							break;
					}
					m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::PROTECT] = 0;
					delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, MagicType::PROTECT);
				}
				switch (m_pNpcList[sTargetH]->m_cActionLimit) {
					case 0:
					case 3:
					case 5:
						m_pNpcList[sTargetH]->m_iHP -= iDamage;
						if (m_pClientList[sAttackerH] != nullptr) {
							if (m_pClientList[sAttackerH]->iDmgShowon == 1) {
								std::memset(cDamageMod, 0, sizeof (cDamageMod));
								wsprintf(cDamageMod, "You did [%d] Damage to Npc [%s] Remaining Life [%d]", iDamage, m_pNpcList[sTargetH]->m_cNpcName, m_pNpcList[sTargetH]->m_iHP);
								m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
							}
						}
						break;
				}
				if (m_pNpcList[sTargetH]->m_iHP <= 0) {
					m_pNpcList[sTargetH]->NpcKilledHandler(sAttackerH, cAttackerType, iDamage);
					bKilled = true;
					iKilledDice = m_pNpcList[sTargetH]->m_iHitDice;
				} else {
					if ((m_pNpcList[sTargetH]->m_sType != 21) && (m_pNpcList[sTargetH]->m_sType != 55) && (m_pNpcList[sTargetH]->m_sType != 56)
							  && (m_pNpcList[sTargetH]->m_cSide == cAttackerSide)) goto CAE_SKIPCOUNTERATTACK;
					if (m_pNpcList[sTargetH]->m_cActionLimit != 0) goto CAE_SKIPCOUNTERATTACK;
					if (m_pNpcList[sTargetH]->m_bIsPermAttackMode == true) goto CAE_SKIPCOUNTERATTACK;
					if ((m_pNpcList[sTargetH]->m_bIsSummoned == true) && (m_pNpcList[sTargetH]->m_iSummonControlMode == 1)) goto CAE_SKIPCOUNTERATTACK;
					if (m_pNpcList[sTargetH]->m_sType == 51) goto CAE_SKIPCOUNTERATTACK;
					if (iDice(1, 3) == 2) {
						if (m_pNpcList[sTargetH]->m_cBehavior == DEF_BEHAVIOR_ATTACK) {
							tX = tY = 0;
							switch (m_pNpcList[sTargetH]->m_cTargetType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[m_pNpcList[sTargetH]->m_iTargetIndex] != nullptr) {
										tX = m_pClientList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sX;
										tY = m_pClientList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sY;
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[m_pNpcList[sTargetH]->m_iTargetIndex] != nullptr) {
										tX = m_pNpcList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sX;
										tY = m_pNpcList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sY;
									}
									break;
							}
							iDst1 = (m_pNpcList[sTargetH]->m_sX - tX)*(m_pNpcList[sTargetH]->m_sX - tX) + (m_pNpcList[sTargetH]->m_sY - tY)*(m_pNpcList[sTargetH]->m_sY - tY);
							tX = tY = 0;
							switch (cAttackerType) {
								case DEF_OWNERTYPE_PLAYER:
									if (m_pClientList[sAttackerH] != nullptr) {
										tX = m_pClientList[sAttackerH]->m_sX;
										tY = m_pClientList[sAttackerH]->m_sY;
									}
									break;
								case DEF_OWNERTYPE_NPC:
									if (m_pNpcList[sAttackerH] != nullptr) {
										tX = m_pNpcList[sAttackerH]->m_sX;
										tY = m_pNpcList[sAttackerH]->m_sY;
									}
									break;
							}
							iDst2 = (m_pNpcList[sTargetH]->m_sX - tX)*(m_pNpcList[sTargetH]->m_sX - tX) + (m_pNpcList[sTargetH]->m_sY - tY)*(m_pNpcList[sTargetH]->m_sY - tY);
							if (iDst2 <= iDst1) {
								m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
								m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
								m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
								m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
							}
						} else {
							m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
							m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
							m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
							m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
						}
					}
CAE_SKIPCOUNTERATTACK:
					;
					if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0))
						m_pNpcList[sTargetH]->m_dwTime = dwTime;
					if (cAttackerType == DEF_OWNERTYPE_PLAYER)
						sAttackerWeapon = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
					else sAttackerWeapon = 1;
					if ((wWeaponType < 40) && (m_pNpcList[sTargetH]->m_cActionLimit == 4)) {
						if (sTgtX == sAtkX) {
							if (sTgtY == sAtkY) goto CAE_SKIPDAMAGEMOVE2;
							else if (sTgtY > sAtkY) cDamageMoveDir = 5;
							else if (sTgtY < sAtkY) cDamageMoveDir = 1;
						} else if (sTgtX > sAtkX) {
							if (sTgtY == sAtkY) cDamageMoveDir = 3;
							else if (sTgtY > sAtkY) cDamageMoveDir = 4;
							else if (sTgtY < sAtkY) cDamageMoveDir = 2;
						} else if (sTgtX < sAtkX) {
							if (sTgtY == sAtkY) cDamageMoveDir = 7;
							else if (sTgtY > sAtkY) cDamageMoveDir = 6;
							else if (sTgtY < sAtkY) cDamageMoveDir = 8;
						}
						dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
						if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) {
							cDamageMoveDir = iDice(1, 8);
							dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
							dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
							if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) goto CAE_SKIPDAMAGEMOVE2;
						}
						m_pNpcList[sTargetH]->map_->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
						m_pNpcList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
						m_pNpcList[sTargetH]->m_sX = dX;
						m_pNpcList[sTargetH]->m_sY = dY;
						m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;
						m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
						dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
						if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) {
							cDamageMoveDir = iDice(1, 8);
							dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
							dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
							if (m_pNpcList[sTargetH]->map_->bGetMoveable(dX, dY, nullptr) == false) goto CAE_SKIPDAMAGEMOVE2;
						}
						m_pNpcList[sTargetH]->map_->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
						m_pNpcList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
						m_pNpcList[sTargetH]->m_sX = dX;
						m_pNpcList[sTargetH]->m_sY = dY;
						m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;
						m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
						if (m_pNpcList[sTargetH]->bCheckEnergySphereDestination(sAttackerH, cAttackerType) == true) {
							if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
								iExp = (m_pNpcList[sTargetH]->m_iExp / 3);
								if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0)
									iExp += m_pNpcList[sTargetH]->m_iNoDieRemainExp;
								if (m_pClientList[sAttackerH]->m_iAddExp != 0) {
									dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
									dTmp2 = (double) iExp;
									dTmp3 = (dTmp1 / 100.0f) * dTmp2;
									iExp += (int) dTmp3;
								}
								if ((m_bIsCrusadeMode) && (iExp > 10)) iExp = 10;
								m_pClientList[sAttackerH]->GetExp(iExp);
								m_pNpcList[sTargetH]->DeleteNpc();
								return false;
							}
						}
CAE_SKIPDAMAGEMOVE2:
						;
					} else {
						m_pNpcList[sTargetH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, sAttackerWeapon, 0);
					}
					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] == 1) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, MagicType::HOLDOBJECT);
					} else if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] == 2) {
						if ((m_pNpcList[sTargetH]->m_iHitDice > 50) && (iDice(1, 10) == 5)) {
							m_pNpcList[sTargetH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
							delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, MagicType::HOLDOBJECT);
						}
					}
					if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) &&
							  (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
						if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
							iExp = iDamage;
							m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
						} else {
							iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
						if (m_pClientList[sAttackerH]->m_iAddExp != 0) {
							dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddExp;
							dTmp2 = (double) iExp;
							dTmp3 = (dTmp1 / 100.0f) * dTmp2;
							iExp += (int) dTmp3;
						}
						if (m_bIsCrusadeMode) iExp = iExp / 3;
						if (m_pClientList[sAttackerH]->m_iLevel > 100) {
							switch (m_pNpcList[sTargetH]->m_sType) {
								case 55:
								case 56:
									iExp = 0;
									break;
								default: break;
							}
						}
					}
				}
				break;
		}
		if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
			if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND] != -1)
				sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::TWOHAND];
			else sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[EquipPos::RHAND];
			if ((sWeaponIndex != -1) && (!bArrowUse)) {
				if ((m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex] != nullptr) &&
						  (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_sIDnum != 231)) {
					if (!bKilled)
						m_pClientList[sAttackerH]->CalculateSSN_ItemIndex(sWeaponIndex, 1);
					else {
						if (m_pClientList[sAttackerH]->m_iHP <= 3)
							m_pClientList[sAttackerH]->CalculateSSN_ItemIndex(sWeaponIndex, iDice(1, iKilledDice)*2);
						else m_pClientList[sAttackerH]->CalculateSSN_ItemIndex(sWeaponIndex, iDice(1, iKilledDice));
					}
				}
				if ((m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex] != nullptr) &&
						  (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wMaxLifeSpan != 0)) {
					iWepLifeOff = 1;
					if ((wWeaponType >= 1) && (wWeaponType < 40)) {
						switch (m_pClientList[sAttackerH]->map_->m_cWhetherStatus) {
							case 0: break;
							case 1: if (iDice(1, 3) == 1) iWepLifeOff++;
								break;
							case 2: if (iDice(1, 2) == 1) iWepLifeOff += iDice(1, 2);
								break;
							case 3: if (iDice(1, 2) == 1) iWepLifeOff += iDice(1, 3);
								break;
						}
					}
					if (m_pClientList[sAttackerH]->m_cSide != 0) {
						if (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan < iWepLifeOff)
							m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
						else m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan -= iWepLifeOff;
					}
					if (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan == 0) {
						m_pClientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_cEquipPos, sWeaponIndex, 0, nullptr);
						m_pClientList[sAttackerH]->ReleaseItemHandler(sWeaponIndex, true);
					}
				}
			} else {
				if (wWeaponType == 0) {
					m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(5, 1);
				}
			}
		}
	} else {
		if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
			m_pClientList[sAttackerH]->m_iComboAttackCount = 0;
		}
	}
	return iExp;
}

void CGame::MeteorStrikeMsgHandler(char cAttackerSide) {
	int i;
	uint32_t dwTime = timeGetTime();
	switch (cAttackerSide) {
		case 1:
			if (m_iElvineMapIndex != -1) {
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						if (strcmp(m_pClientList[i]->map_->m_cLocationName, "elvine") == 0) {
							m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKECOMING, 1, 0, 0, nullptr);
						} else {
							m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKECOMING, 2, 0, 0, nullptr);
						}
					}
				delayEvents_.add(DelayEventType::METEORSTRIKE, 0, dwTime + 5000, 0, 0, m_pMapList[m_iElvineMapIndex], 0, 0, 0, 0, 0);
			} else {
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if (m_pClientList[i] != nullptr) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKECOMING, 2, 0, 0, nullptr);
					}
			}
			break;
		case 2:
			if (m_iAresdenMapIndex != -1) {
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						if (strcmp(m_pClientList[i]->map_->m_cLocationName, "aresden") == 0) {
							m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKECOMING, 3, 0, 0, nullptr);
						} else {
							m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKECOMING, 4, 0, 0, nullptr);
						}
					}
				delayEvents_.add(DelayEventType::METEORSTRIKE, 0, dwTime + 1000 * 5, 0, 0, m_pMapList[m_iAresdenMapIndex], 0, 0, 0, 0, 0);
			} else {
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if (m_pClientList[i] != nullptr) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKECOMING, 4, 0, 0, nullptr);
					}
			}
			break;
	}
}


/*bool CGame::_bDecodeNpcItemConfigFileContents(char * pData, uint32_t dwMsgSize)
{
 char * pContents, * token, cTxt[120];
 char seps[] = "= \t\n";
 char cReadModeA = 0;
 char cReadModeB = 0;
 int  iNpcConfigListIndex = 0, k = 0;
 class CStrTok * pStrTok;
 class CNpcItem * pTempNpcItem = nullptr ;
	pContents = new char[dwMsgSize+1];
	ZeroMemory(pContents, dwMsgSize+1);
	memcpy(pContents, pData, dwMsgSize);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while( token != nullptr ) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					if (strlen(token) > 20)	{
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - Too long Npc name.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					for (iNpcConfigListIndex = 0; iNpcConfigListIndex < DEF_MAXNPCTYPES; iNpcConfigListIndex++)
						if (m_pNpcConfigList[iNpcConfigListIndex] != nullptr) {
							if(strcmp(m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName,token) == 0) {
								PutLogList(token);
								break;
							}
						}
					if (iNpcConfigListIndex == DEF_MAXNPCTYPES ) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - No exist Npc Name");
						delete pContents;
						delete pStrTok;
						return false;
					}
					cReadModeB = 2;
					break;
				case 2:
					if( strlen(token) > 2 ) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - Type Error.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					m_pNpcConfigList[iNpcConfigListIndex]->m_iNpcItemType = atoi(token);
					cReadModeB = 3;
					break;
				case 3:
					if (strlen(token) > 20)	{
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - Too long Item name.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					if (pTempNpcItem == nullptr)
						pTempNpcItem = new class CNpcItem();
					if( memcmp(token,"[ENDITEM]",9) == 0 ){
						cReadModeA = 0 ;
						cReadModeB = 0 ;
						if ( pTempNpcItem  != nullptr) {
							delete pTempNpcItem ;
							pTempNpcItem = nullptr ;
						}
						break;
					}
					strcpy(pTempNpcItem->m_cName, token);
					if(!m_bReceivedItemList)
					{
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration error - Before Item List receiving.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					for(k=0; k < DEF_MAXITEMTYPES; k++)
					{
						if ( m_pItemConfigList[k] == nullptr )
							continue;
						if (strcmp(token, m_pItemConfigList[k]->m_cName) == 0 ) {
							pTempNpcItem->m_sItemID = m_pItemConfigList[k]->m_sIDnum;
							break;
						}
					}
					if ( k == DEF_MAXITEMTYPES ) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration error - Do Not exist in ITEM LIST");
						delete pContents;
						delete pStrTok;
						return false;
					}
					cReadModeB = 4;
					break;
				case 4:

					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					pTempNpcItem->m_sFirstProbability = atoi(token);
					if (pTempNpcItem->m_sFirstProbability <= 0 ) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - First probability have wrong value");
						delete pContents;
						delete pStrTok;
						return false ;
					}

					if ( pTempNpcItem->m_sFirstProbability > 13)
						pTempNpcItem->m_cFirstTargetValue = 13;
					else if ( pTempNpcItem->m_sFirstProbability > 3)
						pTempNpcItem->m_cFirstTargetValue = 3;
					else
						pTempNpcItem->m_cFirstTargetValue = 1;
					cReadModeB = 5;
					break;
				case 5:

					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					pTempNpcItem->m_sSecondProbability = atoi(token);
					if (pTempNpcItem->m_sSecondProbability <= 0 ) {
						PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file error - Second probability have wrong value");
						delete pContents;
						delete pStrTok;
						return false ;
					}

					if ( pTempNpcItem->m_sSecondProbability > 13)
						pTempNpcItem->m_cSecondTargetValue = 13 ;
					else if ( pTempNpcItem->m_sSecondProbability > 3)
						pTempNpcItem->m_cSecondTargetValue = 3 ;
					else
						pTempNpcItem->m_cSecondTargetValue = 1 ;
					cReadModeB = 3;

					m_pNpcConfigList[iNpcConfigListIndex]->m_vNpcItem.push_back(*pTempNpcItem);
					if( m_pNpcConfigList[iNpcConfigListIndex]->m_iNpcItemMax < pTempNpcItem->m_sSecondProbability )
						m_pNpcConfigList[iNpcConfigListIndex]->m_iNpcItemMax = pTempNpcItem->m_sSecondProbability;
					break;
				} // switch #2
			default:
				break;
			} // switch #1
		} // if
		else {
			if (memcmp(token, "NpcItem", 7) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! NPCITEM configuration file contents error!");
		return false;
	}
	return true;
} // _bDecodeNpcItemConfigFileContents()*/
bool CGame::bGetItemNameWhenDeleteNpc(int & iItemID, short sNpcType) {
	int iResult;
	// NPCë¥¼ ì‚­ì œí• ë•Œ íŠ¹ìˆ˜ ì•„ì�´í…œì�„ ë°œìƒ�ì‹œí‚¬ ê²ƒì�¸ì§€ì�˜ ì—¬ë¶€ë¥¼ ê³„ì‚°í•œë‹¤.
	switch (sNpcType) {
		case 49: // Hellclaw
			iResult = iDice(1, 20000);
			if ((iResult >= 1) && (iResult <= 4999)) {
				if (iDice(1, 2) == 1)
					iItemID = 259; // Green Ball 651
				else iItemID = 20; // Rep Potion 965
			} else if ((iResult > 5000) && (iResult <= 10000)) iItemID = 651; // MagicWand(M.Shield)
			else if ((iResult > 10000) && (iResult <= 13000)) iItemID = 965; // Excaliber
			else if ((iResult > 13000) && (iResult <= 15000)) iItemID = 335; // EmeraldRing
			else if ((iResult > 15000) && (iResult <= 17500)) iItemID = 300; // MagicNecklace(RM10)
			else if ((iResult > 17500) && (iResult <= 18750)) iItemID = 311; // MagicNecklace(DF+10)
			else if ((iResult > 18750) && (iResult <= 19000)) iItemID = 305; // MagicNecklace(DM+1)
			else if ((iResult > 19000) && (iResult <= 19700)) iItemID = 634; // RingofWizard
			else if ((iResult > 19700) && (iResult <= 19844)) iItemID = 635; // RingofMage
			else if ((iResult > 19844) && (iResult <= 19922)) iItemID = 643; // KnecklaceOfIceEle
			else if ((iResult > 19922) && (iResult <= 19961)) iItemID = 866; // ResurWandMS10
			else if ((iResult > 19961) && (iResult <= 19981)) iItemID = 858; // NecklaceOfMerien
			else if ((iResult > 19981) && (iResult <= 19991)) iItemID = 620; // MerienShield
			else if ((iResult > 19991) && (iResult <= 19996)) iItemID = 644; // KnecklaceOfAirEle
			else if ((iResult > 19996) && (iResult <= 19999)) iItemID = 614; // SwordofIceElemental
			else if ((iResult > 19999) && (iResult <= 20000)) iItemID = 636; // RingofGrandMage
			return true;
		case 50: // Tigerworm
			iResult = iDice(1, 10000);
			if ((iResult >= 1) && (iResult <= 4999)) {
				if (iDice(1, 2) == 1)
					iItemID = 614; // Red Ball 652
				else iItemID = 985; // Rep Potion 965
			} else if ((iResult > 5000) && (iResult <= 7499)) iItemID = 965; // IceElementalRapier
			else if ((iResult > 7500) && (iResult <= 8749)) iItemID = 652; // SwordofIceElemental
			else if ((iResult > 8750) && (iResult <= 9374)) iItemID = 633; // RingofDemonpower
			else if ((iResult > 9375) && (iResult <= 9687)) iItemID = 492; // BloodRapier
			else if ((iResult > 9688) && (iResult <= 9843)) iItemID = 490; // BloodSword
			else if ((iResult > 9844) && (iResult <= 9921)) iItemID = 491; // BloodAxe
			else if ((iResult > 9922) && (iResult <= 9960)) iItemID = 291; // MagicWand(MS30-LLF)
			else if ((iResult > 9961) && (iResult <= 9980)) iItemID = 630; // RingoftheXelima
			else if ((iResult > 9981) && (iResult <= 9990)) iItemID = 612; // XelimaRapier
			else if ((iResult > 9991) && (iResult <= 9996)) iItemID = 610; // XelimaBlade
			else if ((iResult > 9996) && (iResult <= 9998)) iItemID = 611; // XelimaAxe
			else if ((iResult > 9999) && (iResult <= 10000)) iItemID = 631; // RingoftheAbaddon
			else if ((iResult > 9999) && (iResult <= 10000)) iItemID = 849; // "KlonessBlade"		2%
			else if ((iResult > 9999) && (iResult <= 10000)) iItemID = 864; // "KlonessWandMS20"		2%
			return true;
		default:
			break;
	}
	if (iDice(1, 45) == 13) {
		switch (sNpcType) {
			case 11: if (iDice(1, 550) != 11) return false;
				break; // Skeleton   2 * 100
			case 12: if (iDice(1, 400) != 11) return false;
				break; // Stone-Golem 2 * 100
			case 13: if (iDice(1, 100) != 11) return false;
				break; // Cyclops  6 * 100
			case 14: if (iDice(1, 700) != 11) return false;
				break; // Orc 4 * 100
			case 17: if (iDice(1, 600) != 11) return false;
				break; // Scorpoin 5 * 100
			case 18: if (iDice(1, 850) != 11) return false;
				break; // Zombie 1 * 100
			case 22: if (iDice(1, 600) != 11) return false;
				break; // Amphis 5 * 100
			case 23: if (iDice(1, 400) != 11) return false;
				break; // Clay-Golem 2 * 100
			case 27: if (iDice(1, 100) != 11) return false;
				break; // Hellhound 7 * 100
			case 28: if (iDice(1, 100) != 11) return false;
				break; // Troll 5 * 100
			case 29: if (iDice(1, 150) != 11) return false;
				break; // Orge  7 * 100
			case 30: if (iDice(1, 120) != 11) return false;
				break; // Liche 1 * 100
			case 31: break; // Demon 5 * 100
			case 32: if (iDice(1, 200) != 11) return false;
				break; // Unicorn 5 * 100
			case 33: if (iDice(1, 300) != 11) return false;
				break; // WereWolf 7 * 100
			case 48: if (iDice(1, 100) != 11) return false;
				break; // Stalker
			case 52: if (iDice(1, 300) != 11) return false;
				break; // Gagoyle
			case 53: if (iDice(1, 500) != 11) return false;
				break; // Beholder
			case 54: if (iDice(1, 200) != 11) return false;
				break; // Dark-Elf
			case 57: if (iDice(1, 400) != 11) return false;
				break; // Giant-Frog
			case 63: if (iDice(1, 300) != 11) return false;
				break; // Frost
			case 79: if (iDice(1, 200) != 11) return false;
				break; // Nizie
			case 70: if (iDice(1, 200) != 11) return false;
				break; // Barlog
			case 71: if (iDice(1, 200) != 11) return false;
				break; // Centaurus
			default: return false;
		}
	} else return false;
	//http://www.helbreath.com/down/d_patch_v2.htm
	switch (sNpcType) {
		case 11: // Skeleton
		case 17: // Scorpoin
		case 14: // Orc
		case 28: // Troll
		case 57: // Giant-Frog
			switch (iDice(1, 7)) {
				case 1: iItemID = 334;
					break; // LuckyGoldRing
				case 2: iItemID = 336;
					break; // SapphireRing
				case 3: if (iDice(1, 15) == 3) iItemID = 335;
					break; // EmeraldRing
				case 4: iItemID = 337;
					break; // RubyRing
				case 5: iItemID = 333;
					break; // PlatinumRing
				case 6: if (iDice(1, 15) == 3) iItemID = 634;
					break; // RingofWizard
				case 7: if (iDice(1, 25) == 3) iItemID = 635;
					break; // RingofMage
			}
			break;
		case 13: // Cyclops
		case 27: // Hellhound
		case 29: // Orge
			switch (iDice(1, 7)) {
				case 1: iItemID = 311;
					break; // MagicNecklace(DF+10)
				case 2: if (iDice(1, 20) == 13) iItemID = 308;
					break; // MagicNecklace(MS10)
				case 3: if (iDice(1, 10) == 13) iItemID = 305;
					break; // MagicNecklace(DM+1)
				case 4: iItemID = 300;
					break; // MagicNecklace(RM10)
				case 5: if (iDice(1, 30) == 13) iItemID = 632;
					break; // RingofOgrepower
				case 6: if (iDice(1, 30) == 13) iItemID = 637;
					break; // KnecklaceOfLightPro
				case 7: if (iDice(1, 30) == 13) iItemID = 638;
					break; // KnecklaceOfFirePro
			}
			break;
		case 18: // Zombie
		case 22: // Amphis
			switch (iDice(1, 4)) {
				case 1: if (iDice(1, 75) == 13) iItemID = 613;
					break; // SwordofMedusa
				case 2: if (iDice(1, 20) == 13) iItemID = 639;
					break; // KnecklaceOfPoisonPro
				case 3: if (iDice(1, 40) == 13) iItemID = 641;
					break; // KnecklaceOfMedusa
				case 4: if (iDice(1, 30) == 13) iItemID = 640;
					break; // KnecklaceOfSufferent
			}
			break;
		case 12: // Stone-Golem
			switch (iDice(1, 5)) {
				case 1: if (iDice(1, 40) == 13) iItemID = 620;
					break; // MerienShield
				case 2: if (iDice(1, 40) == 13) iItemID = 621;
					break; // MerienPlateMail(M)
				case 3: if (iDice(1, 40) == 13) iItemID = 622;
					break; // MerienPlateMail(W)
				case 4: if (iDice(1, 20) == 11) iItemID = 644;
					break; // KnecklaceOfAirEle
				case 5: if (iDice(1, 20) == 11) iItemID = 647;
					break; // KnecklaceOfStoneGolem
			}
			break;
		case 23: // Clay-Golem
			switch (iDice(1, 4)) {
				case 1: if (iDice(1, 40) == 13) iItemID = 620;
					break; // MerienShield
				case 2: if (iDice(1, 40) == 13) iItemID = 621;
					break; // MerienPlateMail(M)
				case 3: if (iDice(1, 40) == 13) iItemID = 622;
					break; // MerienPlateMail(W)
				case 4: if (iDice(1, 20) == 11) iItemID = 644;
					break; // KnecklaceOfAirEle
			}
			break;
		case 32: // Unicorn
			switch (iDice(1, 4)) {
				case 1: if (iDice(1, 40) == 13) iItemID = 620;
					break; // MerienShield
				case 2: if (iDice(1, 40) == 13) iItemID = 621;
					break; // MerienPlateMail(M)
				case 3: if (iDice(1, 40) == 13) iItemID = 622;
					break; // MerienPlateMail(W)
				case 4: if (iDice(1, 20) == 11) iItemID = 644;
					break; // KnecklaceOfAirEle
				case 5: if (iDice(1, 20) == 11) iItemID = 848;
					break; // Lighting Blade
			}
			break;
		case 33: // WereWolf
		case 48: // Stalker
			switch (iDice(1, 2)) {
				case 1: if (iDice(1, 30) == 3) iItemID = 290;
					break; // Flameberge+3(LLF)
				case 2: iItemID = 292;
					break; // GoldenAxe(LLF)
			}
			break;
		case 30: // Liche
			switch (iDice(1, 8)) {
				case 1: if (iDice(1, 10) == 3) iItemID = 380;
					break; // IceStormManual
				case 2: if (iDice(1, 10) == 3) iItemID = 259;
					break; // MagicWand(M.Shield)
				case 3: if (iDice(1, 30) == 3) iItemID = 291;
					break; // MagicWand(MS30-LLF)
				case 4: if (iDice(1, 10) == 3) iItemID = 614;
					break; // SwordofIceElemental
				case 5: if (iDice(1, 10) == 3) iItemID = 642;
					break; // KnecklaceOfIcePro
				case 6: if (iDice(1, 15) == 3) iItemID = 643;
					break; // KnecklaceOfIceEle
				case 7: if (iDice(1, 30) == 3) iItemID = 636;
					break; // RingofGrandMage
				case 8: if (iDice(1, 30) == 3) iItemID = 734;
					break; // RingOfArcmage
			}
			break;
		case 31: // Demon
			switch (iDice(1, 8)) {
				case 1: if (iDice(1, 30) == 3) iItemID = 382;
					break; // BloodyShockW.Manual
				case 2: if (iDice(1, 10) == 3) iItemID = 491;
					break; // BloodAxe
				case 3: if (iDice(1, 10) == 3) iItemID = 490;
					break; // BloodSword
				case 4: if (iDice(1, 10) == 3) iItemID = 492;
					break; // BloodRapier
				case 5: if (iDice(1, 10) == 3) iItemID = 381;
					break; // MassFireStrikeManual
				case 6: if (iDice(1, 30) == 3) iItemID = 633;
					break; // RingofDemonpower
				case 7: if (iDice(1, 10) == 3) iItemID = 645;
					break; // KnecklaceOfEfreet
				case 8: if (iDice(1, 20) == 3) iItemID = 616;
					break; // DemonSlayer
			}
			break;
		case 52: // Gagoyle
			switch (iDice(1, 11)) {
				case 1: if (iDice(1, 30) == 3) iItemID = 382;
					break; // BloodyShockW.Manual
				case 2: if (iDice(1, 20) == 3) iItemID = 610;
					break; // XelimaBlade
				case 3: if (iDice(1, 20) == 3) iItemID = 611;
					break; // XelimaAxe
				case 4: if (iDice(1, 20) == 3) iItemID = 612;
					break; // XelimaRapier
				case 5: if (iDice(1, 10) == 3) iItemID = 381;
					break; // MassFireStrikeManual
				case 6: if (iDice(1, 30) == 3) iItemID = 633;
					break; // RingofDemonpower
				case 7: if (iDice(1, 10) == 3) iItemID = 645;
					break; // KnecklaceOfEfreet
				case 8: if (iDice(1, 40) == 3) iItemID = 630;
					break; // RingoftheXelima
				case 9: if (iDice(1, 50) == 3) iItemID = 631;
					break; // RingoftheAbaddon
				case 10: if (iDice(1, 20) == 3) iItemID = 735;
					break; // RingOfDragonpower
				case 11: if (iDice(1, 40) == 3) iItemID = 20;
					break; // Excaliber
				case 12: if (iDice(1, 60) == 3) iItemID = 846;
					break; // The_Devastator
			}
			break;
		case 53: // Beholder
			if (iDice(1, 20) == 11) iItemID = 646;
			break; // KnecklaceOfBeholder
			break;
		case 54: // Dark-Elf
			if (iDice(1, 20) == 11) iItemID = 618;
			break; // DarkElfBow
			break;
		case 63: // Frost
			if (iDice(1, 40) == 11) iItemID = 845;
			break; // StormBringer
			break;
		case 79: // Nizie
			if (iDice(1, 20) == 11) iItemID = 845;
			break; // StormBringer
			break;
		case 70: // Barlog
			if (iDice(1, 40) == 11) iItemID = 846;
			break; // The_Devastator
			break;
		case 71: // Centaurus
			if (iDice(1, 20) == 11) iItemID = 848;
			break; // Lighting Blade
			break;
	}
	if (iItemID == 0)
		return false;
	else return true;
}

/*bool CGame::bGetItemNameWhenDeleteNpc(int &iItemID, short sNpcType)
{
 class	CNpcItem * CTempNpcItem;
 int	iResult;
 int	iNpcIndex;
 int	iNumNpcitem;
 int	iIndex;
 int	iDiceValue;
 bool	bFirstDice = false, bSecondDice = false;
	for (iNpcIndex = 0; iNpcIndex < DEF_MAXNPCTYPES; iNpcIndex++) {
		if (m_pNpcConfigList[iNpcIndex] != nullptr) {
			if (m_pNpcConfigList[iNpcIndex]->m_sType == sNpcType) break;
		}
	}
	if (iNpcIndex == DEF_MAXNPCTYPES) return false;
	if (m_pNpcConfigList[iNpcIndex]->m_vNpcItem.size() <=  0)  return false; // sub_4BAFD0
	switch (m_pNpcConfigList[iNpcIndex]->m_iNpcItemType) {
	case 1:
		iResult = iDice(1, m_pNpcConfigList[iNpcIndex]->m_vNpcItem.size()) - 1;
		CTempNpcItem = m_pNpcConfigList[iNpcIndex]->m_vNpcItem.at(iResult);
		if (iDice(1,CTempNpcItem.m_sFirstProbability) == CTempNpcItem.m_cFirstTargetValue) bFirstDice  = true;
		if (iDice(1,CTempNpcItem.m_sSecondProbability) == CTempNpcItem.m_cSecondTargetValue) bSecondDice = true;
		if ((bFirstDice) && (bSecondDice)) {
			iItemID = CTempNpcItem.m_sItemID;
			wsprintf(G_cTxt,"NpcType 1 (%d) size(%d) %s(%d) (%d)", sNpcType, m_pNpcConfigList[iNpcIndex]->m_vNpcItem.size(), CTempNpcItem.m_cName, CTempNpcItem.m_sItemID);
			PutLogList(G_cTxt);
		}
		break;
	case 2:
		iNumNpcitem = m_pNpcConfigList[iNpcIndex]->m_vNpcItem.size(); // sub_4BAFD0
		iDiceValue  = iDice(1, m_pNpcConfigList[iNpcIndex]->m_iNpcItemMax);
		for (iIndex = 0; iIndex < iNumNpcitem; iIndex++) {
			CTempNpcItem = m_pNpcConfigList[iNpcIndex]->m_vNpcItem.at(iIndex); // sub_4BB010
			if(CTempNpcItem.m_sFirstProbability <= iDiceValue && iDiceValue < CTempNpcItem.m_sSecondProbability) {
				iItemID = CTempNpcItem.m_sItemID;
				wsprintf(G_cTxt,"NpcType 2 (%d) size(%d) %s(%d) (%d)", sNpcType, m_pNpcConfigList[iNpcIndex]->m_vNpcItem.size(), CTempNpcItem.m_cName, CTempNpcItem.m_sItemID);
				break;
			}
		}
		break;
	} // switch
	if (iItemID <= 0)
		return false;
	return true;
}*/

void CGame::GlobalEndHeldenianMode() {
	if (!m_bIsHeldenianMode) return;
	LocalEndHeldenianMode();
}

void CGame::LocalEndHeldenianMode() {
	register int i, x;
	if (!m_bIsHeldenianMode) return;
	m_bIsHeldenianMode = false;
	m_bHeldenianInitiated = true;
	m_dwHeldenianFinishTime = time(nullptr);
	if (m_bIsHeldenianReady == 1) {
		if (m_cHeldenianModeType == 1) {
			if (m_iHeldenianAresdenLeftTower > m_iHeldenianElvineLeftTower) {
				m_cHeldenianVictoryType = 1;
			} else if (m_iHeldenianAresdenLeftTower < m_iHeldenianElvineLeftTower) {
				m_cHeldenianVictoryType = 2;
			} else if (m_iHeldenianAresdenDead < m_iHeldenianElvineDead) {
				m_cHeldenianVictoryType = 1;
			} else if (m_iHeldenianAresdenDead > m_iHeldenianElvineDead) {
				m_cHeldenianVictoryType = 2;
			} else {
				m_sLastHeldenianWinner = m_cHeldenianVictoryType;
			}
		} else if (m_cHeldenianModeType == 2) {
			m_sLastHeldenianWinner = m_cHeldenianVictoryType;
		}
		m_sLastHeldenianWinner = m_cHeldenianVictoryType;
		if (bNotifyHeldenianWinner() == false) {
			wsprintf(G_cTxt, "(!) HELDENIAN End. Result Report Failed");
			PutLogList(G_cTxt);
		} else {
			wsprintf(G_cTxt, "(!) HELDENIAN End. %d", m_sLastHeldenianWinner);
			PutLogList(G_cTxt);
		}
		for (i = 0; i < DEF_MAXMAPS; i++) {
			if ((i < 0) || (i > 100) || (m_pMapList[i] != nullptr)) {
				for (x = 0; x < DEF_MAXCLIENTS; x++) {
					if ((m_pClientList[x] != nullptr) && (m_pClientList[x]->m_bIsInitComplete == true)) {
						m_pClientList[x]->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANEND, 0, 0, 0, nullptr);
					}
				}
				if (m_pMapList[i]->m_bIsHeldenianMap == true) {
					for (x = 0; x < DEF_MAXNPCS; x++) {
						if (m_pNpcList[x] != nullptr) {
							if (m_pNpcList[x]->map_ != nullptr) {
								m_pNpcList[x]->m_bIsSummoned = true;
								m_pNpcList[x]->RemoveEventNpc();
							}
						}
					}
					m_pMapList[i]->RemoveOccupyFlags();
				}
			}
		}
	}
	_CreateHeldenianGUID(m_dwHeldenianGUID, m_cHeldenianVictoryType);
}

bool CGame::bNotifyHeldenianWinner() {
	if (m_bIsHeldenianReady == 0) {
		return false;
	} else {
		HeldenianWinner();
		return true;
	}
}

void CGame::_CreateHeldenianGUID(uint32_t dwHeldenianGUID, int iWinnerSide) {
	char * cp, cTxt[256], cFn[256], cTemp[1024];
	FILE * pFile;
	_mkdir("GameData");
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, "GameData");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, "HeldenianGUID.Txt");
	pFile = fopen(cFn, "wt");
	if (pFile == nullptr) {
		wsprintf(cTxt, "(!) Cannot create HeldenianGUID(%d) file", dwHeldenianGUID);
		PutLogList(cTxt);
	} else {
		std::memset(cTemp, 0, sizeof (cTemp));
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "HeldenianGUID = %d", dwHeldenianGUID);
		strcat(cTemp, cTxt);
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
		strcat(cTemp, cTxt);
		cp = (char *) cTemp;
		fwrite(cp, strlen(cp), 1, pFile);
		wsprintf(cTxt, "(O) HeldenianGUID(%d) file created", dwHeldenianGUID);
		PutLogList(cTxt);
	}
	if (pFile != nullptr) fclose(pFile);
}

void CGame::HeldenianWarStarter() {
	SYSTEMTIME SysTime;
	int i;
	GetLocalTime(&SysTime);
	for (i = 0; i < DEF_MAXSCHEDULE; i++)
		if ((!m_bIsHeldenianScheduleLoaded) &&
				  (!m_bIsHeldenianSchedule) &&
				  (m_stHeldenianSchedule[i].iDay == SysTime.wDayOfWeek) &&
				  (m_stHeldenianSchedule[i].EndiHour == SysTime.wHour) &&
				  (m_stHeldenianSchedule[i].EndiMinute == SysTime.wMinute) &&
				  (!m_bIsApocalypseMode) &&
				  (!m_bIsCrusadeMode)) {
			m_bIsHeldenianSchedule = true;
			wsprintf(G_cTxt, "Heldenian Start : time(%d %d:%d), index(%d)", m_stHeldenianSchedule[i].iDay, m_stHeldenianSchedule[i].EndiHour, m_stHeldenianSchedule[i].EndiMinute, i);
			PutLogFileList(G_cTxt);
			GlobalStartHeldenianMode();
		}
}

void CGame::HeldenianWarEnder() {
	SYSTEMTIME SysTime;
	int i;
	GetLocalTime(&SysTime);
	for (i = 0; i < DEF_MAXSCHEDULE; i++)
		if ((m_bIsHeldenianScheduleLoaded) &&
				  (m_bIsHeldenianSchedule) &&
				  (m_stHeldenianSchedule[i].iDay != SysTime.wDayOfWeek) &&
				  (m_stHeldenianSchedule[i].EndiHour != SysTime.wHour) &&
				  (m_stHeldenianSchedule[i].EndiMinute != SysTime.wMinute)) {
			m_bIsHeldenianSchedule = false;
			wsprintf(G_cTxt, "Heldenian End : time(%d %d:%d), index(%d)", SysTime.wDayOfWeek, SysTime.wHour, SysTime.wMinute, i);
			PutLogFileList(G_cTxt);
			if (m_cHeldenianModeType == 2) {
				m_cHeldenianVictoryType = m_sLastHeldenianWinner;
			}
			GlobalEndHeldenianMode();
		}
}

void CGame::GlobalStartHeldenianMode() {
	uint32_t dwTime = timeGetTime();
	LocalStartHeldenianMode(m_cHeldenianModeType, m_sLastHeldenianWinner, dwTime);
}

/*********************************************************************************************************************
 **  void CGame::LocalStartHeldenianMode(short sV1, short sV2, uint32_t dwHeldenianGUID)								**
 **  description			:: localy performs all required tasks upon recieving heldenian initiate message				**
 **  last updated		:: December 10, 2004; 12:02 PM; Hypnotoad													**
 **	return value		:: void																						**
 **  commentary			::	-	translated from scratch using official 3.51 chinese hgserver						**
 *********************************************************************************************************************/
void CGame::LocalStartHeldenianMode(short sV1, short sV2, uint32_t dwHeldenianGUID) {
	int i;
	int x;
	int z;
	int iNamingValue;
	char cName[11], cTmp[21], cNpcWaypointIndex[10], cSide, cOwnerType;
	short sOwnerH;
	bool bRet;
	int dX;
	int dY;
	if (m_bIsHeldenianMode) return;
	if ((m_cHeldenianModeType == -1) || (m_cHeldenianModeType != sV1)) m_cHeldenianModeType = sV1;
	if ((m_sLastHeldenianWinner != -1) && (m_sLastHeldenianWinner == sV2)) {
		wsprintf(G_cTxt, "Heldenian Mode : %d , Heldenian Last Winner : %d", m_cHeldenianModeType, m_sLastHeldenianWinner);
		PutLogFileList(G_cTxt);
	}
	if (dwHeldenianGUID != 0) {
		_CreateHeldenianGUID(dwHeldenianGUID, 0);
		m_dwHeldenianGUID = dwHeldenianGUID;
	}
	m_iHeldenianAresdenLeftTower = 0;
	m_iHeldenianElvineLeftTower = 0;
	m_iHeldenianAresdenDead = 0;
	m_iHeldenianElvineDead = 0;
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {
			if (m_pClientList[i]->m_bIsInitComplete != true) break;
			m_pClientList[i]->m_cWarType = 2;
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, nullptr);
			m_pClientList[i]->m_iWarContribution = 0;
			m_pClientList[i]->m_iConstructionPoint = (m_pClientList[i]->m_iCharisma * 300);
			if (m_pClientList[i]->m_iConstructionPoint > 12000) m_pClientList[i]->m_iConstructionPoint = 12000;
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 1, nullptr);
		}
	}
	for (x = 0; x < DEF_MAXMAPS; x++) {
		if (m_pMapList[x] != nullptr) {
			if (m_pMapList[x]->m_bIsHeldenianMap == true) {
				for (i = 0; i < DEF_MAXCLIENTS; i++) {
					if ((m_pClientList[i] != nullptr) &&
							  (m_pClientList[i]->m_bIsInitComplete == true) &&
							  (m_pClientList[i]->m_iAdminUserLevel < 1) &&
							  (m_pMapList[x] == m_pClientList[i]->map_)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_0BE8, 0, 0, 0, nullptr);
						m_pClientList[i]->RequestTeleportHandler("1   ", nullptr, -1, -1);
					}
				}
				for (i = 0; i < DEF_MAXNPCS; i++) {
					if ((m_pNpcList[i] != nullptr) &&
							  (m_pNpcList[i]->m_bIsKilled == false) &&
							  (m_pMapList[x] == m_pNpcList[i]->map_)) {
						m_pNpcList[i]->m_bIsSummoned = true;
						m_pNpcList[i]->RemoveEventNpc();
					}
				}
				if (m_cHeldenianModeType == 1) {
					if (strcmp(m_pMapList[x]->m_cName, "BtField") == 0) {
						for (i = 0; i < MAX_HELDENIANTOWER; i++) {
							iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID < 1) || (m_pMapList[x]->m_stHeldenianTower[i].sTypeID > DEF_MAXNPCTYPES)) break;
							if (iNamingValue != -1) {
								dX = m_pMapList[x]->m_stHeldenianTower[i].dX;
								dY = m_pMapList[x]->m_stHeldenianTower[i].dY;
								cSide = m_pMapList[x]->m_stHeldenianTower[i].cSide;
								for (z = 0; z < DEF_MAXNPCTYPES; z++) {
									if (m_pNpcArchetypes[z] == nullptr) break;
									if (m_pNpcArchetypes[z]->m_sType == m_pMapList[x]->m_stHeldenianTower[i].sTypeID) {
										std::memset(cTmp, 0, sizeof (cTmp));
										strcpy(cTmp, m_pNpcArchetypes[z]->m_cNpcName);
									}
								}
								std::memset(cName, 0, sizeof (cName));
								wsprintf(cName, "XX%d", iNamingValue);
								cName[0] = 95;
								cName[1] = i + 65;
								bRet = m_pMapList[x]->bCreateNewNpc(cTmp, cName, (rand() % 3), SpecialAbility::NONE, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, nullptr, 0, cSide, false, false, false, true, false);
								if (!bRet) {
									m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
								} else {
									m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
									if ((m_pNpcList[sOwnerH] != nullptr) && (sOwnerH > 0) && (sOwnerH < DEF_MAXNPCS)) {
										m_pNpcList[sOwnerH]->m_iBuildCount = 0;
									}
									if (cSide == 1) m_iHeldenianAresdenLeftTower += 1;
									if (cSide == 2) m_iHeldenianElvineLeftTower += 1;
								}
							}
						}
						wsprintf(G_cTxt, "HeldenianAresdenLeftTower : %d , HeldenianElvineLeftTower : %d", m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower);
						PutLogFileList(G_cTxt);
						UpdateHeldenianStatus();
					}
				} else if (m_cHeldenianModeType == 2) {
					if (strcmp(m_pMapList[x]->m_cName, "HRampart") == 0) {
						for (i = 0; i < DEF_MAXHELDENIANDOOR; i++) {
							iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
							if (iNamingValue != -1) {
								dX = m_pMapList[x]->m_stHeldenianGateDoor[i].dX;
								dY = m_pMapList[x]->m_stHeldenianGateDoor[i].dY;
								cSide = m_sLastHeldenianWinner;
								for (z = 0; z < DEF_MAXNPCTYPES; z++) {
									if (m_pNpcArchetypes[z] == nullptr) break;
									if (m_pNpcArchetypes[z]->m_sType == 91) {
										std::memset(cTmp, 0, sizeof (cTmp));
										strcpy(cTmp, m_pNpcArchetypes[z]->m_cNpcName);
									}
								}
								std::memset(cName, 0, sizeof (cName));
								wsprintf(cName, "XX%d", iNamingValue);
								cName[0] = 95;
								cName[1] = i + 65;
								bRet = m_pMapList[x]->bCreateNewNpc(cTmp, cName, (rand() % 3), SpecialAbility::NONE, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, nullptr, 0, cSide, false, false, false, true, false);
								if (!bRet) {
									m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
								} else {
									//m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
									if ((m_pNpcList[bRet] != nullptr) && (bRet > 0) && (bRet < DEF_MAXNPCS)) {
										m_pNpcList[bRet]->m_iBuildCount = 0;
										m_pNpcList[bRet]->m_cDir = m_pMapList[x]->m_stHeldenianGateDoor[i].cDir;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	m_bHeldenianInitiated = true;
	m_bIsHeldenianMode = true;
	wsprintf(G_cTxt, "(!) HELDENIAN Start.");
	PutLogFileList(G_cTxt);
	m_dwHeldenianStartTime = time(nullptr);
}

void CGame::GlobalStartApocalypseMode() {
	uint32_t dwApocalypseGUID = timeGetTime();
	LocalStartApocalypseMode(dwApocalypseGUID);
}

void CGame::AutomatedHeldenianTimer() {
	uint32_t dwTime;
	int x;
	int i;
	dwTime = time(nullptr);
	if ((dwTime == m_dwHeldenianStartTime - 300) && (!m_bHeldenianInitiated) && (!m_bIsHeldenianMode)) {
		wsprintf(G_cTxt, "Heldenian Fight Start");
		PutLogFileList(G_cTxt);
		m_bIsHeldenianMode = false;
		for (x = 0; x < DEF_MAXMAPS; x++) {
			if (m_pMapList[x] == nullptr) return;
			if (m_pMapList[x]->m_bIsHeldenianMap == true) return;
			for (i = 0; i < DEF_MAXCLIENTS; i++) {
				if (m_pClientList[i] != nullptr) {
					if (m_pClientList[i]->m_bIsInitComplete != true) return;
					if (m_pClientList[i]->map_ == m_pMapList[x]) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
					}
				}
			}
		}
	}
	if ((dwTime > (m_dwHeldenianFinishTime - 180)) && (m_bHeldenianInitiated) && (m_bIsHeldenianMode)) {
		wsprintf(G_cTxt, "Heldenian Fight End");
		PutLogFileList(G_cTxt);
		m_bHeldenianInitiated = false;
		for (x = 0; x < DEF_MAXMAPS; x++) {
			if (m_pMapList[x] == nullptr) return;
			if (m_pMapList[x]->m_bIsHeldenianMap == true) return;
			for (i = 0; i < DEF_MAXCLIENTS; i++) {
				if (m_pClientList[i] != nullptr) {
					if (m_pClientList[i]->m_bIsInitComplete != true) return;
					if (m_pClientList[i]->map_ == m_pMapList[x]) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_0BE8, 0, 0, 0, nullptr);
						m_pClientList[i]->RequestTeleportHandler("1   ", nullptr, -1, -1);
					}
				}
			}
		}
	}
}

void CGame::NotifyStartHeldenianFightMode() {
	register int i, x;
	if ((!m_bIsHeldenianMode) && (!m_bHeldenianInitiated) && (!m_bIsHeldenianReady)) return;
	PutLogList("Heldenian Fight Start");
	m_bHeldenianInitiated = false;
	for (i = 0; i < DEF_MAXMAPS; i++) {
		if (m_pMapList[i] != nullptr) {
			if (m_pMapList[i]->m_bIsHeldenianMap == true) {
				for (x = 0; x < DEF_MAXCLIENTS; x++) {
					if (m_pClientList[x] != nullptr) {
						if (m_pClientList[x]->m_bIsInitComplete == true) {
							if (m_pClientList[x]->map_ == m_pMapList[i]) {
								m_pClientList[x]->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, nullptr);
							}
						}
					}
				}
			}
		}
	}
}

bool CGame::UpdateHeldenianStatus() {
	if (!m_bIsHeldenianMode) return false;
	for (int i = 0; i < DEF_MAXMAPS; i++) {
		if (m_pMapList[i] != nullptr) {
			if (m_pMapList[i]->m_bIsHeldenianMap == true) {
				for(auto &iterClient: m_pClientList) {
					if ((iterClient.m_bIsInitComplete) && (strcmp(iterClient.map_->m_cName, "BtField") == 0)) {
						iterClient.SendNotifyMsg(0, DEF_NOTIFY_HELDENIANCOUNT, m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower, m_iHeldenianAresdenDead, nullptr, m_iHeldenianElvineDead, 0);
					}
				}
			}
		}
	}
	return true;
}

void CGame::AutomaticHeldenianEnd() {
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	if ((m_dwHeldenianStartHour == SysTime.wHour) &&
			  (m_dwHeldenianStartMinute == SysTime.wMinute)) {
		GlobalEndHeldenianMode();
		m_bHeldenianRunning = false;
	}
}

void CGame::HeldenianWinner() {
	int x;
	int i;
	for (x = 0; x < DEF_MAXMAPS; x++) {
		if (m_pMapList[x] != nullptr) {
			for (i = 0; i < DEF_MAXCLIENTS; i++) {
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == 1) && (m_pClientList[i]->map_ = m_pMapList[x])) {
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_0BEB, m_sLastHeldenianWinner, 0, 0, nullptr);
				}
			}
		}
	}
}

void CGame::SendThunder(int iClient, short sX, short sY, short sV3, short sV4) {
	char * cp, cData[100];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	std::memset(cData, 0, sizeof (cData));
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_EVENT_COMMON;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_COMMONTYPE_MAGIC;
	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	*sp = sX;
	cp += 2;
	sp = (short *) cp;
	*sp = sY;
	cp += 2;
	sp = (short *) cp;
	*sp = sX;
	cp += 2;
	sp = (short *) cp;
	*sp = sY;
	cp += 2;
	sp = (short *) cp;
	*sp = sV3;
	cp += 2;
	sp = (short *) cp;
	*sp = sV4;
	cp += 2;
	m_pClientList[iClient]->m_pXSock->iSendMsg(cData, 18);
}

/*********************************************************************************************************************
 **  bool CGame::SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, iTemp)		Snoopy			**
 ** description	  :: Sets the staus to send or not Angels to every client							**
 *********************************************************************************************************************/
void CGame::SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp) {
	if (cOwnerType != DEF_OWNERTYPE_PLAYER) return;
	if (m_pClientList[sOwnerH] == nullptr) return;
	switch (iStatus) {
		case 1: // STR Angel
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00001000;
			break;
		case 2: // DEX Angel
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00002000;
			break;
		case 3: // INT Angel
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00004000;
			break;
		case 4: // MAG Angel
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00008000;
			break;
		default:
		case 0: // Remove all Angels
			m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFF00FF;
			break;
	}
	if (iTemp > 4) {
		int iAngelicStars = (iTemp / 3)*(iTemp / 5);
		m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | (iAngelicStars << 8);
	}
	m_pClientList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

/*********************************************************************************************************************
 **  bool CGame::RequestHeldenianScroll(CClient &client, char * pData, uint32_t dwMsgSize)									**
 ** description	  :: Reversed and coded by Snoopy																	**
 *********************************************************************************************************************/
// Majestic Code by Diuuude

void CGame::ReadMajesticSettings() {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadMode;
	char cFn[120];
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	cReadMode = 0;
	strcpy(cFn, "..\\GameConfigs\\MajesticSettings.cfg");
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		return;
	} else {
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		//token = strtok( cp, seps );
		while (token != nullptr) {
			if (cReadMode != 0) {
				switch (cReadMode) {
					case 1:
						if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1)) {
							m_iMajesticPointsPerLevel = atoi(token);
						}
						cReadMode = 0;
						break;
					case 2:
						if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1)) {
							m_iMajesticMinLevel = atoi(token);
						}
						cReadMode = 0;
						break;
					case 3:
						if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1)) {
							m_iMajesticPointsMax = atoi(token);
						}
						cReadMode = 0;
						break;
				}
			} else {
				if (memcmp(token, "majestic-points-per-level", 25) == 0) cReadMode = 1;
				if (memcmp(token, "majestic-min-level", 18) == 0) cReadMode = 2;
				if (memcmp(token, "majestic-points-max", 19) == 0) cReadMode = 3;
			}
			token = pStrTok->pGet();
			//token = strtok( nullptr, seps );
		}
		delete pStrTok;
		delete cp;
	}
	if (pFile != nullptr) fclose(pFile);
	PutLogList("(*) Loaded Majestic Config");
	return;
}
//dkset by diuude and update by drawjer

void CGame::AFKChecker() {
	register int i;
	int sX;
	int sY;
	uint32_t dwTimeNow = timeGetTime();
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {
			if ((m_pClientList[i]->m_bIsInitComplete == true)
					  && ((dwTimeNow - m_pClientList[i]->m_dwAFKCheckTime) > DEF_AFKTIME)
					  && (m_pClientList[i]->m_iHP > 0)
					  && ((m_pClientList[i]->m_iStatus & 0x00000010) == 0)
					  && (iDice(1, 5) == 3)) {
				sX = m_pClientList[i]->m_sX;
				if (m_pClientList[i]->m_bSkillUsingStatus[19] == true)
					sY = m_pClientList[i]->m_sY + 1;
				else sY = m_pClientList[i]->m_sY;
				m_pClientList[i]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
						  sX, sY, sX, sY, 247, m_pClientList[i]->m_sType);
			}
		}
	}
}

//Fin Criticals By MIHD
// MORLA2.2 - Para determinar el Rango devuelve los EKs y PKs del user

bool CGame::bReadBallSystemConfigFile(const char * cFn) {
	FILE * pFile;
	HANDLE hFile;
	uint32_t dwFileSize;
	char * cp, * token, cReadModeA, cReadModeB;
	char seps[] = "= \t\n";
	int iIndex;
	class CStrTok * pStrTok;
	char cTxt[41];
	cReadModeA = 0;
	cReadModeB = 0;
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	dwFileSize = GetFileSize(hFile, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot open Ball System configuration file.");
		return false;
	} else {
		PutLogList("(!) Reading Ball System configuration file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		iIndex = 0;
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != nullptr) {
			if (cReadModeA != 0) {
				switch (cReadModeA) {
					case 1:
						switch (cReadModeB) {
							case 1:
								if (_bGetIsStringIsNumber(token) == false) {
									PutLogList("(!!!) CRITICAL ERROR! Ball System configuration file error - Wrong Data format(1).");
									delete cp;
									delete pStrTok;
									return false;
								}
								iIndex = atoi(token);
								if (m_pBallItemConfigList[iIndex] != nullptr) {
									wsprintf(cTxt, "(!!!) CRITICAL ERROR! Duplicate Ball Item ID (%d)", iIndex);
									PutLogList(cTxt);
									delete pStrTok;
									return false;
								}
								m_pBallItemConfigList[iIndex] = new class CBallSystem;
								m_pBallItemConfigList[iIndex]->m_sBS_ID = iIndex;
								cReadModeB = 2;
								break;
							case 2:
								std::memset(m_pBallItemConfigList[iIndex]->m_cName, 0, sizeof (m_pBallItemConfigList[iIndex]->m_cName));
								memcpy(m_pBallItemConfigList[iIndex]->m_cName, token, strlen(token));
								cReadModeB = 3;
								break;
							case 3:
								m_pBallItemConfigList[iIndex]->m_iReqPoints = atoi(token);
								cReadModeA = 0;
								cReadModeB = 0;
						}
						break;
					default:
						break;
				}
			} else {
				if (memcmp(token, "Ball-Item", 9) == 0) {
					cReadModeA = 1;
					cReadModeB = 1;
				}
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete cp;
		if ((cReadModeA != 0) || (cReadModeB != 0)) {
			PutLogList("(!!!) CRITICAL ERROR! Ball System configuration file contents error!");
			return false;
		}
	}
	if (pFile != nullptr) fclose(pFile);
	return true;
}

void CGame::PURITYLIFE(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3) {
	int iHP;
	int iMaxHP;
	int iSP;
	int iMaxSP;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER)
		if (m_pClientList[sAttackerH] == nullptr) return;
	iHP = iDice(sV1 * 2, sV2 * 2) * sV3;
	iSP = iDice(sV1 * 2, sV2 * 2) * sV3;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER://Sube vida y Stamina del pj target
			if (m_pClientList[sTargetH] == nullptr) return;
			if (m_pClientList[sTargetH]->m_bIsKilled == true) return;
			iMaxSP = m_pClientList[sTargetH]->iGetMaxSP();
			iMaxHP = m_pClientList[sTargetH]->iGetMaxHP();
			if (m_pClientList[sTargetH]->m_iSP > iMaxSP)
				m_pClientList[sTargetH]->m_iSP = iMaxSP;
			else if (m_pClientList[sTargetH]->m_iSP < iMaxSP)
				m_pClientList[sTargetH]->m_iSP += iSP;
			if (m_pClientList[sTargetH]->m_iSP <= 0)
				m_pClientList[sTargetH]->m_iSP = 1;
			if (m_pClientList[sTargetH]->m_iHP > iMaxHP)
				m_pClientList[sTargetH]->m_iHP = iMaxHP;
			else if (m_pClientList[sTargetH]->m_iHP < iMaxHP)
				m_pClientList[sTargetH]->m_iHP += iHP;
			if (m_pClientList[sTargetH]->m_iHP <= 0)
				m_pClientList[sTargetH]->m_iHP = 1;
			m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
			m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_SP, 0, 0, 0, nullptr);
			break;
		case DEF_OWNERTYPE_NPC://sube vida del npc target
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if (m_pNpcList[sTargetH]->m_bIsKilled == true) return;
			iMaxHP = m_pNpcList[sTargetH]->m_iHitDice * 4;
			if (m_pNpcList[sTargetH]->m_iHP < iMaxHP)
				m_pNpcList[sTargetH]->m_iHP += iHP;
			if (m_pNpcList[sTargetH]->m_iHP > iMaxHP) m_pNpcList[sTargetH]->m_iHP = iMaxHP;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) m_pNpcList[sTargetH]->m_iHP = 1;
			break;
	}
}

void CGame::HappyHuor() {
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	uint32_t dwTime = timeGetTime();
	switch (SysTime.wDayOfWeek) {
		case 1:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			break;
		case 2:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			break;
		case 3:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			break;
		case 4:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			// Happy Hour by MrKrhiz para MundoHelbreath & Helbreath&Mas
			break;
		case 5:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			break;
		case 6:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			break;
		case 0:
			dwHappyHourTime = dwTime;
			m_bHappyHour = true;
			break;
	}
	StarHappyHour();
}

void CGame::StarHappyHour() {
	int i;
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HAPPYHOURSTAR, 0, 0, 0, nullptr);
		}
	}
}

void CGame::EndHappyHour() {
	int i;
	for (i = 0; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HAPPYHOUREND, 0, 0, 0, nullptr);
		}
	}
	m_bHappyHour = false;
}
// MORLA 2.3 - Inicio Manual Happy Hour

void CGame::StartCaptureTheFlag() {
	if (m_bIsCTFMode) return;
	m_sFlagCountWin = 5;
	wsprintf(G_cTxt, "Capture The Flag: Win Flag Count(%d)", m_sFlagCountWin);
	PutLogList(G_cTxt);
	m_bIsCTFMode = true;
	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != nullptr) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CAPTURETHEFLAGSTART, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		}
	}
	PutLogList("(*) Capture The Flag Started!");
}

void CGame::EndCaptureTheFlag(int iOption) {
	if (!m_bIsCTFMode) return;
	int i;
	switch (iOption) {
		case 0://Finished by GM
			for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_TIECTF, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
			_CreateCTFGUID(0);
			break;
		case 1://Aresden Win
			for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ARESDENWINCTF, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
			_CreateCTFGUID(1);
			break;
		case 2://Elvine Win
			for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ELVINEWINCTF, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
			_CreateCTFGUID(2);
			break;
	}
}

void CGame::_CreateCTFGUID(int iWinnerSide) {
	char * cp, cTxt[256], cFn[256], cTemp[1024];
	FILE * pFile;
	_mkdir("GameData");
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, "GameData");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, "CTFGUID.Txt");
	pFile = fopen(cFn, "wt");
	if (pFile == nullptr) {
		PutLogList("(!) Cannot create CTFGUID(%d) file");
	} else {
		std::memset(cTemp, 0, sizeof (cTemp));
		std::memset(cTxt, 0, sizeof (cTxt));
		wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
		strcat(cTemp, cTxt);
		cp = (char *) cTemp;
		fwrite(cp, strlen(cp), 1, pFile);
		PutLogList("(O) CTFGUID(%d) file created");
	}
	if (pFile != nullptr) fclose(pFile);
}
