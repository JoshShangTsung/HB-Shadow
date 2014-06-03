#include "Game.h"
#include <cstring>
extern char G_cTxt[512];
extern HWND G_hWnd;

void CGame::Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr) {
	int iPartyID, iDamage, iSideCondition, iIndex, iRemainLife, iTemp, iExp, iMaxSuperAttack, iRepDamage;
	char cAttackerSide, cDamageMoveDir, cDamageMinimum;
	uint32_t dwTime;
	register double dTmp1, dTmp2, dTmp3;
	short sTgtX, sTgtY, sItemIndex;
	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] == nullptr)) return;
	if ((cAttackerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sAttackerH] == nullptr)) return;
	if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->map_ != 0) &&
			  (m_pClientList[sAttackerH]->map_->m_bIsHeldenianMap == 1) && (m_bHeldenianInitiated == true)) return;
	dwTime = timeGetTime();
	sTgtX = 0;
	sTgtY = 0;
	iDamage = iDice(sV1, sV2) + sV3;
	if (iDamage <= 0) iDamage = 0;
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER:
			if ((m_bAdminSecurity == true) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return;
			if (m_pClientList[sAttackerH]->m_cHeroArmourBonus == 2) iDamage += 4;
			if ((m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] == -1) || (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] == -1)) {
				sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
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
				sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
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
			if ((m_bIsCrusadeMode == false) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true) && (cTargetType == DEF_OWNERTYPE_PLAYER)) return;
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
			if (bCheckHeldenianMap(sAttackerH, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1) {
				iDamage += iDamage / 3;
			}
			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == true) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
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
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == false) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) &&
					  (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsOwnLocation == true)) return;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				if (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) {
					iSideCondition = iGetPlayerRelationship(sAttackerH, sTargetH);
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
			ClearSkillUsingStatus(sTargetH);
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
			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
				iDamage = iDamage / 2;
			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) break;
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
					SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
					if (m_pClientList[sTargetH]->m_bSkillUsingStatus[19] != true) {
						m_pClientList[sTargetH]->map_->ClearOwner(0, sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						m_pClientList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
					}
					if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, nullptr);
						m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
					}
				}
			}
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if ((m_bIsCrusadeMode == true) && (cAttackerSide == m_pNpcList[sTargetH]->m_cSide)) return;
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
			if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
				iDamage = iDamage / 2;
			m_pNpcList[sTargetH]->m_iHP -= iDamage;
			if (m_pNpcList[sTargetH]->m_iHP < 0) {
				NpcKilledHandler(sAttackerH, cAttackerType, sTargetH, iDamage);
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
				SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
				if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0)) {
					if ((cAttackerType == DEF_OWNERTYPE_NPC) &&
							  (m_pNpcList[sAttackerH]->m_sType == m_pNpcList[sTargetH]->m_sType) &&
							  (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide)) return;
					m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
					m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
					m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
					m_pNpcList[sTargetH]->m_dwTime = dwTime;
					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
					}
					if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
						if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
							iExp = iDamage;
							if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;
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
							if (bExp == true)
								GetExp(sAttackerH, iExp, true);
							else GetExp(sAttackerH, (iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
						} else {
							iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;
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
							if (bExp == true)
								GetExp(sAttackerH, iExp, true);
							else GetExp(sAttackerH, (iExp / 2), true);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
					}
				}
			}
			break;
	}
}

void CGame::Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, bool bExp, int iAttr) {
	int iDamage, iSideCondition, iIndex, iRemainLife, iTemp, iMaxSuperAttack;
	uint32_t dwTime, wWeaponType;
	char cDamageMod[256];
	char cAttackerSide, cDamageMoveDir;
	register double dTmp1, dTmp2, dTmp3;
	int iPartyID, iMoveDamage;
	short sTgtX, sTgtY;
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
			if ((m_bAdminSecurity == true) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return;
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
			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == true) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
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
			if (bCheckHeldenianMap(sAttackerH, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1) {
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
			if ((m_bIsCrusadeMode == false) && (m_pClientList[sTargetH]->m_iPKCount == 0) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;
			if ((m_bIsCrusadeMode == false) && (m_pClientList[sTargetH]->m_iPKCount == 0) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true)) return;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) && (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return;
			if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return;
			m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
			if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
				if (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) {
					iSideCondition = iGetPlayerRelationship(sAttackerH, sTargetH);
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
			ClearSkillUsingStatus(sTargetH);
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
			if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
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
						SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
					}
					if (m_pClientList[sTargetH]->m_bSkillUsingStatus[19] != true) {
						m_pClientList[sTargetH]->map_->ClearOwner(0, sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
						m_pClientList[sTargetH]->map_->SetOwner(sTargetH, DEF_OWNERTYPE_PLAYER, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
					}
					if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
						// 1: Hold-Person
						// 2: Paralize
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, nullptr);
						m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
					}
				}
			}
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return;
			if (m_pNpcList[sTargetH]->m_iHP <= 0) return;
			if ((m_bIsCrusadeMode == true) && (cAttackerSide == m_pNpcList[sTargetH]->m_cSide)) return;
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
			if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
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
				NpcKilledHandler(sAttackerH, cAttackerType, sTargetH, iDamage);
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
				SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
				if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0)) {
					if ((cAttackerType == DEF_OWNERTYPE_NPC) &&
							  (m_pNpcList[sAttackerH]->m_sType == m_pNpcList[sTargetH]->m_sType) &&
							  (m_pNpcList[sAttackerH]->m_cSide == m_pNpcList[sTargetH]->m_cSide)) return;
					m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
					m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
					m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
					m_pNpcList[sTargetH]->m_dwTime = dwTime;
					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
					}
					//Crusade
					int iExp;
					if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) &&
							  (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
						if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
							// Crusade
							iExp = iDamage;
							if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;
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
							if (bExp == true)
								GetExp(sAttackerH, iExp); //m_pClientList[sAttackerH]->m_iExpStock += iExp;     //iDamage;
							else GetExp(sAttackerH, (iExp / 2)); //m_pClientList[sAttackerH]->m_iExpStock += (iExp/2); //(iDamage/2);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
						} else {
							// Crusade
							iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;
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
							if (bExp == true)
								GetExp(sAttackerH, iExp); //m_pClientList[sAttackerH]->m_iExpStock += iExp;     //m_pNpcList[sTargetH]->m_iNoDieRemainExp;
							else GetExp(sAttackerH, (iExp / 2)); //m_pClientList[sAttackerH]->m_iExpStock += (iExp/2); //(m_pNpcList[sTargetH]->m_iNoDieRemainExp/2);
							m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
						}
					}
				}
			}
			break;
	}
}

void CGame::Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3) {
	int iHP, iMaxHP;
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
	int iSP, iMaxSP;
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
	double dTmp1, dTmp2, dTmp3;
	int iTargetMagicResistRatio, iDestHitRatio, iResult;
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
			cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return false;
			iTargetMagicResistRatio = m_pNpcList[sTargetH]->m_cResistMagic;
			cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
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

bool CGame::bCheckResistingIceSuccess(char /*cAttackerDir*/, short sTargetH, char cTargetType, int /*iHitRatio*/) {
	int iTargetIceResistRatio, iResult;
	switch (cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sTargetH] == nullptr) return false;
			if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
			iTargetIceResistRatio = m_pClientList[sTargetH]->m_iAddAbsWater * 2;
			if (m_pClientList[sTargetH]->m_dwWarmEffectTime == 0) {
			} else if ((timeGetTime() - m_pClientList[sTargetH]->m_dwWarmEffectTime) < 1000 * 30) return true;
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sTargetH] == nullptr) return false;
			iTargetIceResistRatio = (m_pNpcList[sTargetH]->m_cResistMagic) - (m_pNpcList[sTargetH]->m_cResistMagic / 3);
			break;
	}
	if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;
	iResult = iDice(1, 100);
	if (iResult <= iTargetIceResistRatio) return true;
	return false;
}

bool CGame::bCheckTotalSkillMasteryPoints(int iClientH, int iSkill) {
	register int i;
	int iRemainPoint, iTotalPoints, iWeaponIndex, iDownSkillSSN, iDownPoint;
	short sDownSkillIndex;
	if (m_pClientList[iClientH] == nullptr) return false;
	iTotalPoints = 0;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		iTotalPoints += m_pClientList[iClientH]->m_cSkillMastery[i];
	iRemainPoint = iTotalPoints - DEF_MAXSKILLPOINTS;
	if (iRemainPoint > 0) {
		while (iRemainPoint > 0) {
			sDownSkillIndex = -1; // v1.4
			if (m_pClientList[iClientH]->m_iDownSkillIndex != -1) {
				switch (m_pClientList[iClientH]->m_iDownSkillIndex) {
					case 3:
						/* ÀÌ ½ºÅ³µµ 0À¸·Î µÈ´Ù.
						case 4:
						case 5:
						case 7:
							
							if (m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_iDownSkillIndex] > 20) {
								sDownSkillIndex = m_pClientList[iClientH]->m_iDownSkillIndex;
							}
							else {
								
								iDownSkillSSN = 99999999;
								for (i = 0; i < DEF_MAXSKILLTYPE; i++)
								if ((m_pClientList[iClientH]->m_cSkillMastery[i] >= 21) && (i != iSkill) &&
									(m_pClientList[iClientH]->m_iSkillSSN[i] <= iDownSkillSSN)) {
									
									iDownSkillSSN = m_pClientList[iClientH]->m_iSkillSSN[i];
									sDownSkillIndex = i;
								}
							}
							break;
						 */
					default:
						if (m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_iDownSkillIndex] > 0) {
							sDownSkillIndex = m_pClientList[iClientH]->m_iDownSkillIndex;
						} else {
							iDownSkillSSN = 99999999;
							for (i = 0; i < DEF_MAXSKILLTYPE; i++)
								if ((m_pClientList[iClientH]->m_cSkillMastery[i] >= 21) && (i != iSkill) &&
										  (m_pClientList[iClientH]->m_iSkillSSN[i] <= iDownSkillSSN)) {
									iDownSkillSSN = m_pClientList[iClientH]->m_iSkillSSN[i];
									sDownSkillIndex = i;
								}
						}
						break;
				}
			}
			if (sDownSkillIndex != -1) {
				if (m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] <= 20) // v1.4
					iDownPoint = m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex];
				else iDownPoint = 1;
				m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] -= iDownPoint; // v1.4
				m_pClientList[iClientH]->m_iSkillSSN[sDownSkillIndex] = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] + 1] - 1;
				iRemainPoint -= iDownPoint; // v1.4
				if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1) {
					iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
					if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex) {
						m_pClientList[iClientH]->m_iHitRatio -= iDownPoint; // v1.4
						if (m_pClientList[iClientH]->m_iHitRatio < 0) m_pClientList[iClientH]->m_iHitRatio = 0;
					}
				}
				if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
					iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
					if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex) {
						m_pClientList[iClientH]->m_iHitRatio -= iDownPoint; // v1.4
						if (m_pClientList[iClientH]->m_iHitRatio < 0) m_pClientList[iClientH]->m_iHitRatio = 0;
					}
				}
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILL, sDownSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex], 0, nullptr);
			} else {
				return false;
			}
		}
		return true;
	}
	return false;
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
			
			if (m_bOnExitProcess == false) {
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
			if (m_bF1pressed == true) {
				PutLogList("(!) Send server shutdown announcement1...");
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_SERVERSHUTDOWN, 1, 0, 0, nullptr);
					}
			}
			break;
		case VK_F7:
			if (m_bF1pressed == true) {
				PutLogList("(!) Send server shutdown announcement2...");
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_SERVERSHUTDOWN, 2, 0, 0, nullptr);
					}
			}
			break;
		case VK_F9:
			if ((m_bF1pressed == true)) {
				PutLogList("(!!!) Resume Crusade Mode...");
				LocalStartCrusadeMode(0);
			}
			break;
		case VK_F11:
			if ((m_bF1pressed == true)) {
				PutLogList("(!!!) ManualEndCrusadeMode: side 0");
				ManualEndCrusadeMode(0);
			}
			break;
#define VK_1 0x31
		case VK_1:
			if ((m_bF1pressed == true)) {
				GlobalUpdateConfigs(1);
			}
			break;
#define VK_2 0x32
		case VK_2:
			if ((m_bF1pressed == true)) {
				GlobalUpdateConfigs(2);
			}
			break;
#define VK_3 0x33
		case VK_3:
			if ((m_bF1pressed == true)) {
				GlobalUpdateConfigs(3);
			}
			break;
#define VK_4 0x34
		case VK_4:
			if ((m_bF1pressed == true)) {
				GlobalUpdateConfigs(1);
			}
			break;
			/*#define VK_H 0x49 // H key
			case VK_H:
				if ((m_bF1pressed == true)) {
					GlobalStartHeldenianMode();
				}
				break;*/
			//Crusade Testcode
		case VK_HOME:
			if ((m_bF1pressed == true)) {
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
	for (i = 1; i < DEF_MAXNPCS; i++)
		if ((m_pNpcList[i] != nullptr) && (m_pNpcList[i]->m_cMoveType == DEF_MOVETYPE_FOLLOW)) {
			if ((m_pNpcList[i]->m_iFollowOwnerIndex == sOwnerH) && (m_pNpcList[i]->m_cFollowOwnerType == cOwnerType))
				iTotal++;
		}
	return iTotal;
}

void CGame::SendObjectMotionRejectMsg(int iClientH) {
	char * cp, cData[30];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	int iRet;
	m_pClientList[iClientH]->m_bIsMoveBlocked = true; // v2.171
	dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_REJECT;
	cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sX;
	cp += 2;
	sp = (short *) cp;
	*sp = m_pClientList[iClientH]->m_sY;
	cp += 2;
	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 10);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[iClientH]->DeleteClient(true, true);
			return;
	}
	return;
}

int CGame::_iGetTotalClients() {
	register int i, iTotal;
	iTotal = 0;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != nullptr) iTotal++;
	return iTotal;
}
char _tmp_cCorpseX[] = {0, 1, 1, 1, 0, -1, -1, -1, 0, 0, 0, 0};
char _tmp_cCorpseY[] = {-1, -1, 0, 1, 1, 1, 0, -1, 0, 0, 0};

void CGame::ClearSkillUsingStatus(int iClientH) {
	register int i;
	short tX, fX, tY, fY;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) {
		tX = m_pClientList[iClientH]->m_sX;
		tY = m_pClientList[iClientH]->m_sY;
		if (m_pClientList[iClientH]->map_->bGetMoveable(tX, tY, nullptr) == false) {
			fX = m_pClientList[iClientH]->m_sX + _tmp_cCorpseX[m_pClientList[iClientH]->m_cDir];
			fY = m_pClientList[iClientH]->m_sY + _tmp_cCorpseY[m_pClientList[iClientH]->m_cDir];
			if (m_pClientList[iClientH]->map_->bGetMoveable(fX, fY, nullptr) == false) {
				m_pClientList[iClientH]->m_cDir = iDice(1, 8);
				fX = m_pClientList[iClientH]->m_sX + _tmp_cCorpseX[m_pClientList[iClientH]->m_cDir];
				fY = m_pClientList[iClientH]->m_sY + _tmp_cCorpseY[m_pClientList[iClientH]->m_cDir];
				if (m_pClientList[iClientH]->map_->bGetMoveable(fX, fY, nullptr) == false) {
					return;
				}
			}
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_DAMAGEMOVE, m_pClientList[iClientH]->m_cDir, 0, 0, nullptr);
		}
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_pClientList[iClientH]->m_bSkillUsingStatus[i] = false;
		m_pClientList[iClientH]->m_iSkillUsingTimeID[i] = 0;
	}
	if (m_pClientList[iClientH]->m_iAllocatedFish != 0) {
		if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] != nullptr)
			m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;
		m_pClientList[iClientH]->m_iAllocatedFish = 0;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FISHCANCELED, 0, 0, 0, nullptr);
	}
}

int CGame::iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, MapPtr map, int dX, int dY) {
	class CItem * pItem;
	char cItemName[21];
	short lX, lY;
	int iResult, iFish;
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
			_TamingHandler(iOwnerH, iSkillNum, map, dX, dY);
			break;
		case DEF_SKILLEFFECTTYPE_GET:
			std::memset(cItemName, 0, sizeof (cItemName));
			switch (m_pSkillConfigList[iSkillNum]->m_sValue1) {
				case 1:
					// ±¤¹°
					wsprintf(cItemName, "Meat");
					break;
				case 2:
					//if (map->bGetIsWater(dX, dY) == false) return 0;
					if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
						iFish = iCheckFish(iOwnerH, map, dX, dY);
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

void CGame::UseSkillHandler(int iClientH, int iV1, int /*iV2*/, int /*iV3*/) {
	char cOwnerType;
	short sAttackerWeapon, sOwnerH;
	int iResult, iPlayerSkillLevel;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((iV1 < 0) || (iV1 >= DEF_MAXSKILLTYPE)) return;
	if (m_pSkillConfigList[iV1] == nullptr) return;
	if (m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] == true) return;
	/*
	if (iV1 != 19) {
		m_pClientList[iClientH]->m_iAbuseCount++;
		if ((m_pClientList[iClientH]->m_iAbuseCount % 30) == 0) {
			wsprintf(G_cTxt, "(!) ÇØÅ· ¿ëÀÇÀÚ(%s) Skill(%d) Tries(%d)",m_pClientList[iClientH]->m_cCharName,
																						 iV1, m_pClientList[iClientH]->m_iAbuseCount);
			PutLogFileList(G_cTxt);
		}
	}
	 */
	iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[iV1];
	iResult = iDice(1, 100);
	if (iResult > iPlayerSkillLevel) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
		return;
	}
	switch (m_pSkillConfigList[iV1]->m_sType) {
		case DEF_SKILLEFFECTTYPE_PRETEND:
			switch (m_pSkillConfigList[iV1]->m_sValue1) {
				case 1:
					if (m_pClientList[iClientH]->map_->m_bIsFightZone == true) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
						return;
					}
					m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
					if (sOwnerH != 0) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
						return;
					}
					iResult = 0;
					if (m_pClientList[iClientH]->m_iAdminUserLevel <= 0) {
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY - 1);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY + 1);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY - 1);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY - 1);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY + 1);
						iResult += sOwnerH;
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY + 1);
						iResult += sOwnerH;
					}
					if (iResult != 0) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, nullptr);
						return;
					}
					m_pClientList[iClientH]->CalculateSSN_SkillIndex(iV1, 1);
					sAttackerWeapon = 1;
					SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, sAttackerWeapon, 0);
					m_pClientList[iClientH]->map_->ClearOwner(14, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
					m_pClientList[iClientH]->map_->SetDeadOwner(iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
					break;
			}
			break;
	}
	m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] = true;
}

void CGame::ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char * /*pItemName*/) {
	char cItemCategory, cItemName[21];
	short sRemainLife;
	int iPrice;
	double d1, d2, d3;
	bool bNeutral;
	uint32_t dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2, dwMul1, dwMul2;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == nullptr) return;
	if (iNum <= 0) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount < iNum) return;
	m_pClientList[iClientH]->iCalcTotalWeight();
	CItem gold;
	std::memset(cItemName, 0, sizeof (cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(gold, cItemName);
	// v1.42
	bNeutral = false;
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) bNeutral = true;
	switch (cSellToWhom) {
		case 15:
		case 24:
			cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;
			if ((cItemCategory >= 11) && (cItemCategory <= 50)) {
				iPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) * iNum;
				sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
				if (bNeutral == true) iPrice = iPrice / 2;
				if (iPrice <= 0) iPrice = 1;
				if (iPrice > 1000000) iPrice = 1000000;
				if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(gold, iPrice) > m_pClientList[iClientH]->_iCalcMaxLoad()) {
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
				} else m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName, iNum);
			} else if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
				sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
				if (sRemainLife == 0) {
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 2, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
				} else {
					d1 = (double) sRemainLife;
					if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
						d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
					else d2 = 1.0f;
					d3 = (d1 / d2) * 0.5f;
					d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice;
					d3 = d3 * d2;
					iPrice = (int) d3;
					iPrice = iPrice*iNum;
					dwAddPrice1 = 0;
					dwAddPrice2 = 0;
					if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != 0) {
						dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
						dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
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
					if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != 0) {
						dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
						dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;
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
					if (bNeutral == true) iPrice = iPrice / 2;
					if (iPrice <= 0) iPrice = 1;
					if (iPrice > 1000000) iPrice = 1000000;
					if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(gold, iPrice) > m_pClientList[iClientH]->_iCalcMaxLoad()) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
					} else m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName, iNum);
				}
			} else m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 1, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			break;
		default:
			break;
	}
}

void CGame::ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char * /*pString*/) {
	short sRemainLife;
	int iPrice;
	double d1, d2, d3;
	char * cp, cItemName[21], cData[120], cItemCategory;
	uint32_t * dwp, dwMul1, dwMul2, dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2;
	uint16_t * wp;
	int iEraseReq, iRet;
	short * sp;
	bool bNeutral;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == nullptr) return;
	if (iNum <= 0) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount < iNum) return;
	// New 18/05/2004
	if (m_pClientList[iClientH]->m_pIsProcessingAllowed == false) return;
	m_pClientList[iClientH]->iCalcTotalWeight();
	cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;
	// v1.42
	bNeutral = false;
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) bNeutral = true;
	iPrice = 0;
	if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife <= 0) {
			return;
		} else {
			d1 = (double) sRemainLife;
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			iPrice = (short) d3;
			iPrice = iPrice*iNum;
			dwAddPrice1 = 0;
			dwAddPrice2 = 0;
			if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != 0) {
				dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
				dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
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
			if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != 0) {
				dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
				dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;
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
			if (bNeutral == true) iPrice = iPrice / 2;
			if (iPrice <= 0) iPrice = 1;
			if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMSOLD, cItemID, 0, 0, nullptr);
			_bItemLog(DEF_ITEMLOG_SELL, iClientH, (int) - 1, &*m_pClientList[iClientH]->m_pItemList[cItemID]);
			if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
					  (m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
				// v1.41 !!!
				SetItemCount(iClientH, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount - iNum);
			} else m_pClientList[iClientH]->ItemDepleteHandler(cItemID, false, false);
		}
	} else
		if ((cItemCategory >= 11) && (cItemCategory <= 50)) {
		iPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		iPrice = iPrice*iNum;
		if (bNeutral == true) iPrice = iPrice / 2;
		if (iPrice <= 0) iPrice = 1;
		if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMSOLD, cItemID, 0, 0, nullptr);
		_bItemLog(DEF_ITEMLOG_SELL, iClientH, (int) - 1, &*m_pClientList[iClientH]->m_pItemList[cItemID]);
		if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
				  (m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
			// v1.41 !!!
			SetItemCount(iClientH, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount - iNum);
		} else m_pClientList[iClientH]->ItemDepleteHandler(cItemID, false, true);
	}
	if (iPrice <= 0) return;
	CItem *gold = new CItem();
	std::memset(cItemName, 0, sizeof (cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(*gold, cItemName);
	gold->m_dwCount = iPrice;
	if (_bAddClientItemList(iClientH, gold, &iEraseReq) == true) {
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
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
		m_pClientList[iClientH]->iCalcTotalWeight();
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				m_pClientList[iClientH]->DeleteClient(true, true);
				break;
		}
	} else {
		m_pClientList[iClientH]->map_->bSetItem(m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY, gold);
		m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				  gold->m_sSprite, gold->m_sSpriteFrame, gold->m_cItemColor); // v1.4 color
		m_pClientList[iClientH]->iCalcTotalWeight();
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				m_pClientList[iClientH]->DeleteClient(true, true);
				return;
		}
	}
}

void CGame::ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char * /*pString*/) {
	char cItemCategory;
	short sRemainLife, sPrice;
	double d1, d2, d3;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == nullptr) return;
	cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;
	if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
		if (cRepairWhom != 24) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			return;
		}
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		} else {
			d1 = (double) sRemainLife;
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short) d3;
		}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
	} else if (((cItemCategory >= 43) && (cItemCategory <= 50)) || ((cItemCategory >= 11) && (cItemCategory <= 12))) {
		if (cRepairWhom != 15) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			return;
		}
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		} else {
			d1 = (double) sRemainLife;
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short) d3;
		}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
	} else {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 1, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
	}
}

void CGame::ReqRepairItemCofirmHandler(int iClientH, char cItemID, char * /*pString*/) {
	short sRemainLife, sPrice;
	char * cp, cItemCategory, cData[120];
	double d1, d2, d3;
	uint32_t * dwp, dwGoldCount;
	uint16_t * wp;
	int iRet;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == nullptr) return;
	// New 18/05/2004
	if (m_pClientList[iClientH]->m_pIsProcessingAllowed == false) return;
	//testcode
	//PutLogList("Repair!");
	cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;
	if (((cItemCategory >= 1) && (cItemCategory <= 10)) || ((cItemCategory >= 43) && (cItemCategory <= 50)) ||
			  ((cItemCategory >= 11) && (cItemCategory <= 12))) {
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		} else {
			d1 = (double) abs(sRemainLife);
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double) abs(m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan);
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double) m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice;
			d3 = d3 * d2;
			sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short) d3;
		}
		dwGoldCount = dwGetItemCount(iClientH, "Gold");
		if (dwGoldCount < (uint32_t) sPrice) {
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_NOTENOUGHGOLD;
			cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = cItemID;
			cp++;
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 7);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					m_pClientList[iClientH]->DeleteClient(true, true);
					return;
			}
			return;
		} else {
			m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMREPAIRED, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan, 0, nullptr);
			m_pClientList[iClientH]->iCalcTotalWeight();
			m_stCityStatus[m_pClientList[iClientH]->m_cSide].iFunds += sPrice;
		}
	} else {
	}
}

void CGame::CheckAndNotifyPlayerConnection(int iClientH, char * pMsg, uint32_t dwSize) {
	char seps[] = "= \t\n";
	char * token, * cp, cName[11], cBuff[256], cPlayerLocation[120];
	class CStrTok * pStrTok;
	register int i;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
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
			if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) { // GM's get more info
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
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERONGAME, 0, 0, 0, m_pClientList[i]->m_cCharName, 0, 0, 0, 0, 0, 0, cPlayerLocation);
			delete pStrTok;
			return;
		}
	delete pStrTok;
}

void CGame::ToggleWhisperPlayer(int iClientH, char * pMsg, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256];
	if (m_pClientList[iClientH] == nullptr) return;
	if (dwMsgSize <= 0) return;
	std::memset(cName, 0, sizeof (cName));
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pMsg, dwMsgSize);
	CStrTok pStrTok(cBuff, seps);
	token = pStrTok.pGet();
	token = pStrTok.pGet();
	if (token == nullptr) {
		m_pClientList[iClientH]->whisperedPlayer_.reset();
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, cName);
	} else {
		if (strlen(token) > 10)
			memcpy(cName, token, 10);
		else memcpy(cName, token, strlen(token));
		m_pClientList[iClientH]->whisperedPlayer_.reset();
		for (auto &iterClient : m_pClientList) {
			if (memcmp(iterClient.m_cCharName, cName, 10) == 0) {
				if (iterClient.id_ == iClientH) {
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You shouldnt talk to yourself!");
					return;
				}
				m_pClientList[iClientH]->whisperedPlayer_ = iterClient.shared_from_this();
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_WHISPERMODEON, 0, 0, 0, iterClient.m_cCharName);
				break;
			}
		}
		if (!m_pClientList[iClientH]->whisperedPlayer_.lock()) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "This player is not online!");
		}
	}
}

void CGame::SetPlayerProfile(int iClientH, char * pMsg, uint32_t dwMsgSize) {
	char cTemp[256];
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize - 7) <= 0) return;
	std::memset(cTemp, 0, sizeof (cTemp));
	memcpy(cTemp, (pMsg + 7), dwMsgSize - 7);
	for (i = 0; i < 256; i++)
		if (cTemp[i] == ' ') cTemp[i] = '_';
	cTemp[255] = 0;
	std::memset(m_pClientList[iClientH]->m_cProfile, 0, sizeof (m_pClientList[iClientH]->m_cProfile));
	strcpy(m_pClientList[iClientH]->m_cProfile, cTemp);
}

void CGame::GetPlayerProfile(int iClientH, char * pMsg, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256], cBuff2[500];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
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
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERPROFILE, 0, 0, 0, cBuff2);
				delete pStrTok;
				return;
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
	}
	delete pStrTok;
	return;
}

void CGame::___RestorePlayerCharacteristics(int iClientH) {
	int iStr, iDex, iInt, iVit, iMag, iCharisma;
	int iOriginalPoint, iCurPoint, iVerifyPoint, iToBeRestoredPoint;
	int iMax, iA, iB;
	bool bFlag;
	char cTxt[120];
	return;
	if (m_pClientList[iClientH] == nullptr) return;
	iStr = m_pClientList[iClientH]->m_iStr;
	iDex = m_pClientList[iClientH]->m_iDex;
	iInt = m_pClientList[iClientH]->m_iInt;
	iVit = m_pClientList[iClientH]->m_iVit;
	iMag = m_pClientList[iClientH]->m_iMag;
	iCharisma = m_pClientList[iClientH]->m_iCharisma;
	iCurPoint = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iInt +
			  m_pClientList[iClientH]->m_iVit + m_pClientList[iClientH]->m_iDex +
			  m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;
	iOriginalPoint = (m_pClientList[iClientH]->m_iLevel - 1)*3 + 70;
	iToBeRestoredPoint = iOriginalPoint - iCurPoint;
	if (iToBeRestoredPoint == 0) return;
	if (iToBeRestoredPoint > 0) {
		while (1) {
			bFlag = false;
			if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iStr < 10)) {
				m_pClientList[iClientH]->m_iStr++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iMag < 10)) {
				m_pClientList[iClientH]->m_iMag++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iInt < 10)) {
				m_pClientList[iClientH]->m_iInt++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iDex < 10)) {
				m_pClientList[iClientH]->m_iDex++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iVit < 10)) {
				m_pClientList[iClientH]->m_iVit++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iCharisma < 10)) {
				m_pClientList[iClientH]->m_iCharisma++;
				iToBeRestoredPoint--;
				bFlag = true;
			}
			if (bFlag == false) break;
			if (iToBeRestoredPoint <= 0) break;
		}
		iMax = m_pClientList[iClientH]->m_cSkillMastery[5];
		if (m_pClientList[iClientH]->m_iStr < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iStr < (iMax / 2))) {
					m_pClientList[iClientH]->m_iStr++;
					iToBeRestoredPoint--;
				}
				if (m_pClientList[iClientH]->m_iStr == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		iA = m_pClientList[iClientH]->m_cSkillMastery[7];
		iB = m_pClientList[iClientH]->m_cSkillMastery[8];
		if (iA > iB)
			iMax = iA;
		else iMax = iB;
		iA = m_pClientList[iClientH]->m_cSkillMastery[9];
		if (iA > iMax) iMax = iA;
		iA = m_pClientList[iClientH]->m_cSkillMastery[6];
		if (iA > iMax) iMax = iA;
		if (m_pClientList[iClientH]->m_iDex < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iDex < (iMax / 2))) {
					m_pClientList[iClientH]->m_iDex++;
					iToBeRestoredPoint--;
				}
				if (m_pClientList[iClientH]->m_iDex == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		iMax = m_pClientList[iClientH]->m_cSkillMastery[19];
		if (m_pClientList[iClientH]->m_iInt < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iInt < (iMax / 2))) {
					m_pClientList[iClientH]->m_iInt++;
					iToBeRestoredPoint--;
				}
				if (m_pClientList[iClientH]->m_iInt == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		iA = m_pClientList[iClientH]->m_cSkillMastery[3];
		iB = m_pClientList[iClientH]->m_cSkillMastery[4];
		if (iA > iB)
			iMax = iA;
		else iMax = iB;
		if (m_pClientList[iClientH]->m_iMag < (iMax / 2)) {
			while (1) {
				if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iMag < (iMax / 2))) {
					m_pClientList[iClientH]->m_iMag++;
					iToBeRestoredPoint--;
				}
				if (m_pClientList[iClientH]->m_iMag == (iMax / 2)) break;
				if (iToBeRestoredPoint <= 0) break;
			}
		}
		while (iToBeRestoredPoint != 0) {
			switch (iDice(1, 6)) {
				case 1:
					if (m_pClientList[iClientH]->m_iStr < DEF_CHARPOINTLIMIT) {
						m_pClientList[iClientH]->m_iStr++;
						iToBeRestoredPoint--;
					}
					break;
				case 2:
					if (m_pClientList[iClientH]->m_iVit < DEF_CHARPOINTLIMIT) {
						m_pClientList[iClientH]->m_iVit++;
						iToBeRestoredPoint--;
					}
					break;
				case 3:
					if (m_pClientList[iClientH]->m_iDex < DEF_CHARPOINTLIMIT) {
						m_pClientList[iClientH]->m_iDex++;
						iToBeRestoredPoint--;
					}
					break;
				case 4:
					if (m_pClientList[iClientH]->m_iMag < DEF_CHARPOINTLIMIT) {
						m_pClientList[iClientH]->m_iMag++;
						iToBeRestoredPoint--;
					}
					break;
				case 5:
					if (m_pClientList[iClientH]->m_iInt < DEF_CHARPOINTLIMIT) {
						m_pClientList[iClientH]->m_iInt++;
						iToBeRestoredPoint--;
					}
					break;
				case 6:
					if (m_pClientList[iClientH]->m_iCharisma < DEF_CHARPOINTLIMIT) {
						m_pClientList[iClientH]->m_iCharisma++;
						iToBeRestoredPoint--;
					}
					break;
			}
		}
		iVerifyPoint = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iInt +
				  m_pClientList[iClientH]->m_iVit + m_pClientList[iClientH]->m_iDex +
				  m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;
		if (iVerifyPoint != iOriginalPoint) {
			wsprintf(cTxt, "(T_T) RestorePlayerCharacteristics(Minor) FAIL! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
			m_pClientList[iClientH]->m_iStr = iStr;
			m_pClientList[iClientH]->m_iDex = iDex;
			m_pClientList[iClientH]->m_iInt = iInt;
			m_pClientList[iClientH]->m_iVit = iVit;
			m_pClientList[iClientH]->m_iMag = iMag;
			m_pClientList[iClientH]->m_iCharisma = iCharisma;
		} else {
			wsprintf(cTxt, "(^o^) RestorePlayerCharacteristics(Minor) SUCCESS! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
		}
	} else {
		while (1) {
			bFlag = false;
			if (m_pClientList[iClientH]->m_iStr > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				m_pClientList[iClientH]->m_iStr--;
				iToBeRestoredPoint++;
			}
			if (m_pClientList[iClientH]->m_iDex > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				m_pClientList[iClientH]->m_iDex--;
				iToBeRestoredPoint++;
			}
			if (m_pClientList[iClientH]->m_iVit > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				m_pClientList[iClientH]->m_iVit--;
				iToBeRestoredPoint++;
			}
			if (m_pClientList[iClientH]->m_iInt > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				m_pClientList[iClientH]->m_iInt--;
				iToBeRestoredPoint++;
			}
			if (m_pClientList[iClientH]->m_iMag > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				m_pClientList[iClientH]->m_iMag--;
				iToBeRestoredPoint++;
			}
			if (m_pClientList[iClientH]->m_iCharisma > DEF_CHARPOINTLIMIT) {
				bFlag = true;
				m_pClientList[iClientH]->m_iCharisma--;
				iToBeRestoredPoint++;
			}
			if (bFlag == false) break;
			if (iToBeRestoredPoint >= 0) break;
		}
		if (iToBeRestoredPoint < 0) {
			while (iToBeRestoredPoint != 0) {
				switch (iDice(1, 6)) {
					case 1:
						if (m_pClientList[iClientH]->m_iStr > 10) {
							m_pClientList[iClientH]->m_iStr--;
							iToBeRestoredPoint++;
						}
						break;
					case 2:
						if (m_pClientList[iClientH]->m_iVit > 10) {
							m_pClientList[iClientH]->m_iVit--;
							iToBeRestoredPoint++;
						}
						break;
					case 3:
						if (m_pClientList[iClientH]->m_iDex > 10) {
							m_pClientList[iClientH]->m_iDex--;
							iToBeRestoredPoint++;
						}
						break;
					case 4:
						if (m_pClientList[iClientH]->m_iMag > 10) {
							m_pClientList[iClientH]->m_iMag--;
							iToBeRestoredPoint++;
						}
						break;
					case 5:
						if (m_pClientList[iClientH]->m_iInt > 10) {
							m_pClientList[iClientH]->m_iInt--;
							iToBeRestoredPoint++;
						}
						break;
					case 6:
						if (m_pClientList[iClientH]->m_iCharisma > 10) {
							m_pClientList[iClientH]->m_iCharisma--;
							iToBeRestoredPoint++;
						}
						break;
				}
			}
		} else {
			while (iToBeRestoredPoint != 0) {
				switch (iDice(1, 6)) {
					case 1:
						if (m_pClientList[iClientH]->m_iStr < DEF_CHARPOINTLIMIT) {
							m_pClientList[iClientH]->m_iStr++;
							iToBeRestoredPoint--;
						}
						break;
					case 2:
						if (m_pClientList[iClientH]->m_iVit < DEF_CHARPOINTLIMIT) {
							m_pClientList[iClientH]->m_iVit++;
							iToBeRestoredPoint--;
						}
						break;
					case 3:
						if (m_pClientList[iClientH]->m_iDex < DEF_CHARPOINTLIMIT) {
							m_pClientList[iClientH]->m_iDex++;
							iToBeRestoredPoint--;
						}
						break;
					case 4:
						if (m_pClientList[iClientH]->m_iMag < DEF_CHARPOINTLIMIT) {
							m_pClientList[iClientH]->m_iMag++;
							iToBeRestoredPoint--;
						}
						break;
					case 5:
						if (m_pClientList[iClientH]->m_iInt < DEF_CHARPOINTLIMIT) {
							m_pClientList[iClientH]->m_iInt++;
							iToBeRestoredPoint--;
						}
						break;
					case 6:
						if (m_pClientList[iClientH]->m_iCharisma < DEF_CHARPOINTLIMIT) {
							m_pClientList[iClientH]->m_iCharisma++;
							iToBeRestoredPoint--;
						}
						break;
				}
			}
		}
		iVerifyPoint = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iInt +
				  m_pClientList[iClientH]->m_iVit + m_pClientList[iClientH]->m_iDex +
				  m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;
		if (iVerifyPoint != iOriginalPoint) {
			wsprintf(cTxt, "(T_T) RestorePlayerCharacteristics(Over) FAIL! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
			/*
			m_pClientList[iClientH]->m_iStr = iStr;
			m_pClientList[iClientH]->m_iDex = iDex;
			m_pClientList[iClientH]->m_iInt = iInt;
			m_pClientList[iClientH]->m_iVit = iVit;
			m_pClientList[iClientH]->m_iMag = iMag;
			m_pClientList[iClientH]->m_iCharisma = iCharisma;
			 */
		} else {
			wsprintf(cTxt, "(^o^) RestorePlayerCharacteristics(Over) SUCCESS! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
			PutLogList(cTxt);
		}
	}
}

bool CGame::bAnalyzeCriminalAction(int iClientH, short dX, short dY, bool bIsCheck) {
	int iNamingValue, tX, tY;
	short sOwnerH;
	char cOwnerType, cName[11], cNpcName[21];
	char cNpcWaypoint[11];
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return false;
	if (m_bIsCrusadeMode == true) return false;
	m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr)) {
		if (_bGetIsPlayerHostile(iClientH, sOwnerH) != true) {
			if (bIsCheck == true) return true;
			std::memset(cNpcName, 0, sizeof (cNpcName));
			if (strcmp(m_pClientList[iClientH]->map_->m_cName, "aresden") == 0)
				strcpy(cNpcName, "Guard-Aresden");
			else if (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvine") == 0)
				strcpy(cNpcName, "Guard-Elvine");
			else if (strcmp(m_pClientList[iClientH]->map_->m_cName, "default") == 0)
				strcpy(cNpcName, "Guard-Neutral");
			iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
			if (iNamingValue == -1) {
			} else {
				std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
				std::memset(cName, 0, sizeof (cName));
				wsprintf(cName, "XX%d", iNamingValue);
				cName[0] = '_';
				cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
				tX = (int) m_pClientList[iClientH]->m_sX;
				tY = (int) m_pClientList[iClientH]->m_sY;
				if (bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, 0, DEF_MOVETYPE_RANDOM,
						  &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, true) == false) {
					m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
				} else {
					bSetNpcAttackMode(cName, iClientH, DEF_OWNERTYPE_PLAYER, true);
				}
			}
		}
	}
	return false;
}

bool CGame::_bGetIsPlayerHostile(int iClientH, int sOwnerH) {
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[sOwnerH] == nullptr) return false;
	if (iClientH == sOwnerH) return true;
	if (m_pClientList[iClientH]->m_cSide == 0) {
		if (m_pClientList[sOwnerH]->m_iPKCount != 0)
			return true;
		else return false;
	} else {
		if (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide) {
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

void CGame::PoisonEffect(int iClientH, int /*iV1*/) {
	int iPoisonLevel, iDamage, iPrevHP, iProb;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	iPoisonLevel = m_pClientList[iClientH]->m_iPoisonLevel;
	iDamage = iDice(1, iPoisonLevel);
	iPrevHP = m_pClientList[iClientH]->m_iHP;
	m_pClientList[iClientH]->m_iHP -= iDamage;
	if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 1;
	if (iPrevHP != m_pClientList[iClientH]->m_iHP)
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
	iProb = m_pClientList[iClientH]->m_cSkillMastery[23] - 10 + m_pClientList[iClientH]->m_iAddPR;
	if (iProb <= 10) iProb = 10;
	if (iDice(1, 100) <= iProb) {
		m_pClientList[iClientH]->m_bIsPoisoned = false;
		SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, false); // remove poison aura after effect complete
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, nullptr);
	}
}

bool CGame::bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType) {
	int iResist, iResult;
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
	if (m_bManualTime == true) return;
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

void CGame::ShutUpPlayer(int iClientH, char * pMsg, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i, iTime;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelShutup) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERSHUTUP, iTime, 0, 0, cName);
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERSHUTUP, iTime, 0, 0, cName);
				// Admin Log
				wsprintf(G_cTxt, "GM Order(%s): Shutup PC(%s) (%d)Min", m_pClientList[iClientH]->m_cCharName,
						  m_pClientList[i]->m_cCharName, iTime);
				PutAdminLogFileList(G_cTxt);
				delete pStrTok;
				return;
			}
	}
	delete pStrTok;
	return;
}

void CGame::SetPlayerReputation(int iClientH, char * pMsg, char cValue, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iLevel < 40) return;
	if ((m_pClientList[iClientH]->m_iTimeLeft_Rating != 0) || (m_pClientList[iClientH]->m_iPKCount != 0)) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTRATING, m_pClientList[iClientH]->m_iTimeLeft_Rating, 0, 0, nullptr);
		return;
	} else if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTRATING, 0, 0, 0, nullptr);
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
					m_pClientList[iClientH]->m_iTimeLeft_Rating = 20 * 60;
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_RATINGPLAYER, cValue, 0, 0, cName);
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_RATINGPLAYER, cValue, 0, 0, cName);
					delete pStrTok;
					return;
				}
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
	}
	delete pStrTok;
	return;
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
	char cTemp, cBuffer[1000], cKey;
	uint32_t dwSize, dwTime = timeGetTime();
	register int i;
	int iMsgIndex, iTemp;
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

void CGame::CalcExpStock(int iClientH) {
	bool bIsLevelUp;
	class CItem * pItem;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iExpStock <= 0) return;
	// !!!!
	//if ((m_pClientList[iClientH]->m_iLevel >= m_iPlayerMaxLevel) && (m_pClientList[iClientH]->m_iExp >= m_iLevelExpTable[m_iPlayerMaxLevel])) return;
	if (m_pClientList[iClientH]->map_->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) {
		m_pClientList[iClientH]->m_iExpStock = 0;
		return;
	}
	m_pClientList[iClientH]->m_iExp += m_pClientList[iClientH]->m_iExpStock;
	m_pClientList[iClientH]->m_iAutoExpAmount += m_pClientList[iClientH]->m_iExpStock;
	m_pClientList[iClientH]->m_iExpStock = 0;
	if (bCheckLimitedUser(iClientH) == false) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_EXP, 0, 0, 0, nullptr);
	}
	bIsLevelUp = bCheckLevelUp(iClientH);
	if ((bIsLevelUp == true) && (m_pClientList[iClientH]->m_iLevel <= 5)) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, "Gold") == false) {
			delete pItem;
			return;
		} else pItem->m_dwCount = (uint32_t) 100;
		bAddItem(iClientH, pItem, 0);
	}
	if ((bIsLevelUp == true) && (m_pClientList[iClientH]->m_iLevel > 5) && (m_pClientList[iClientH]->m_iLevel <= 20)) {
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, "Gold") == false) {
			delete pItem;
			return;
		} else pItem->m_dwCount = (uint32_t) 300;
		bAddItem(iClientH, pItem, 0);
	}
}

void CGame::___RestorePlayerRating(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iRating < -10000) m_pClientList[iClientH]->m_iRating = 0;
	if (m_pClientList[iClientH]->m_iRating > 10000) m_pClientList[iClientH]->m_iRating = 0;
}

int CGame::iGetExpLevel(int iExp) {
	register int i;
	for (i = 1; i < 1001; i++)
		if ((m_iLevelExpTable[i] <= iExp) && (m_iLevelExpTable[i + 1] > iExp)) return i;
	return 0;
}

void CGame::AdminOrder_CallGuard(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	int iNamingValue, tX, tY;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelCallGaurd) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
					if (bCreateNewNpc(cNpcName, cName, m_pClientList[i]->map_->m_cName, 0, 0, DEF_MOVETYPE_RANDOM,
							  &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, true) == false) {
						m_pClientList[i]->map_->SetNamingValueEmpty(iNamingValue);
					} else {
						bSetNpcAttackMode(cName, i, DEF_OWNERTYPE_PLAYER, true);
					}
				}
				delete pStrTok;
				return;
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CGame::AdminOrder_Kill(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	const char * token;
	char cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	int sAttackerWeapon, sDamage;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelGMKill) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_KILLED, 0, 0, 0, m_pClientList[iClientH]->m_cCharName);
				sAttackerWeapon = 1;
				SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
				m_pClientList[i]->map_->ClearOwner(12, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				m_pClientList[i]->map_->SetDeadOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				delete pStrTok;
				return;
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CGame::AdminOrder_Revive(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	int sAttackerWeapon, sDamage, sHP;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelGMRevive) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				//			SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGEMOVE, m_pClientList[i]->m_iLastDamage, 0, 0);
				m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
				sAttackerWeapon = 1;
				SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, sDamage, sAttackerWeapon, 0);
				m_pClientList[i]->map_->ClearOwner(14, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				m_pClientList[i]->map_->SetDeadOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				delete pStrTok;
				return;
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CGame::AdminOrder_SummonDemon(int iClientH) {
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	register int iNamingValue;
	int tX, tY;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSummonDemon) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		std::memset(cNpcName, 0, sizeof (cNpcName));
		strcpy(cNpcName, "Demon");
		std::memset(cName, 0, sizeof (cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
		std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
		tX = (int) m_pClientList[iClientH]->m_sX;
		tY = (int) m_pClientList[iClientH]->m_sY;
		if (bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, (rand() % 9),
				  DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
			m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
		} else {
			// Admin Log
			wsprintf(G_cTxt, "Admin Order(%s): Summon Demon", m_pClientList[iClientH]->m_cCharName);
			PutAdminLogFileList(G_cTxt);
		}
	}
}

void CGame::AdminOrder_SummonDeath(int iClientH) {
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	register int iNamingValue;
	int tX, tY;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSummonDeath) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		std::memset(cNpcName, 0, sizeof (cNpcName));
		strcpy(cNpcName, "Wyvern");
		std::memset(cName, 0, sizeof (cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
		std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
		tX = (int) m_pClientList[iClientH]->m_sX;
		tY = (int) m_pClientList[iClientH]->m_sY;
		if (bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, (rand() % 9),
				  DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
			m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
		} else {
			// Admin Log
			wsprintf(G_cTxt, "Admin Order(%s): Summon Death", m_pClientList[iClientH]->m_cCharName);
			PutAdminLogFileList(G_cTxt);
		}
	}
}

void CGame::AdminOrder_ReserveFightzone(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	int iNum;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelReserveFightzone) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (m_iFightzoneNoForceRecall == 0) {
		m_iFightzoneNoForceRecall = 1;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -5, 0, 0, nullptr);
	} else {
		m_iFightzoneNoForceRecall = 0;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -4, 0, 0, nullptr);
	}
	if (token == nullptr) {
		delete pStrTok;
		return;
	}
	iNum = atoi(token) - 1;
	if (m_iFightZoneReserve[iNum] != -1) {
		// Admin Log
		wsprintf(G_cTxt, "Admin Order(%s):  %d FightzoneReserved", m_pClientList[iClientH]->m_cCharName, iNum);
		PutAdminLogFileList(G_cTxt);
		m_iFightZoneReserve[iNum] = -1;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, iNum + 1, 0, 0, nullptr);
	} else {
		wsprintf(G_cTxt, "Admin Order(%s):  %d Cancel FightzoneReserved", m_pClientList[iClientH]->m_cCharName, iNum);
		PutAdminLogFileList(G_cTxt);
		m_iFightZoneReserve[iNum] = 0;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -3, 0, 0, nullptr);
	}
	return;
}

void CGame::AdminOrder_CloseConn(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cTargetName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	/*
	
	if ((memcmp(m_pClientList[iClientH]->map_->m_cName, "fightzone", 9) == 0) && (m_pClientList[iClientH]->m_iSkillSSN[59] == 1)) {
		goto AOCC_PASS;
	}
	 */
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelGMCloseconn) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				wsprintf(G_cTxt, "Admin Order(%s): Close Conn", m_pClientList[iClientH]->m_cCharName);
				PutAdminLogFileList(G_cTxt);
				delete pStrTok;
				return;
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CGame::UserCommand_BanGuildsman(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cTargetName[11], cBuff[256];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iGuildRank != 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, nullptr);
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
				if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0) {
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTBANGUILDMAN, 0, 0, 0, nullptr);
					delete pStrTok;
					return;
				}
				bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);
				SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, nullptr);
				std::memset(m_pClientList[i]->m_cGuildName, 0, sizeof (m_pClientList[i]->m_cGuildName));
				strcpy(m_pClientList[i]->m_cGuildName, "NONE");
				m_pClientList[i]->m_iGuildRank = -1;
				m_pClientList[i]->m_iGuildGUID = -1;
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SUCCESSBANGUILDMAN, 0, 0, 0, nullptr);
				m_pClientList[i]->SendNotifyMsg(iClientH, DEF_COMMONTYPE_BANGUILD, 0, 0, 0, nullptr);
				SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
				delete pStrTok;
				return;
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	}
	delete pStrTok;
	return;
}

void CGame::UserCommand_DissmissGuild(int iClientH, char * pData, uint32_t dwMsgSize) {
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
					iDynamicHandle = dynamicObjects_.iAddDynamicObjectList(i, 0, DEF_DYNAMICOBJECT_FISH, map, sX, sY, dwLastTime);
					break;
				default:
					iDynamicHandle = dynamicObjects_.iAddDynamicObjectList(i, 0, DEF_DYNAMICOBJECT_FISHOBJECT, map, sX, sY, dwLastTime);
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
		dynObj.map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, dynObj.m_sX, dynObj.m_sY, dynObj.m_sType, iH, 0);
		dynObj.map_->SetDynamicObject(0, 0, dynObj.m_sX, dynObj.m_sY, dwTime);
		dynObj.map_->m_iCurFish--;
		dynObjPtr.reset();
	}
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) &&
				  (m_pClientList[i]->m_iAllocatedFish == iHandle)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_FISHCANCELED, iDelMode, 0, 0, nullptr);
			ClearSkillUsingStatus(i);
		}
	}
	delete m_pFish[iHandle];
	m_pFish[iHandle] = nullptr;
	return true;
}

int CGame::iCheckFish(int iClientH, MapPtr map, short dX, short dY) {
	register int i;
	short sDistX, sDistY;
	if (m_pClientList[iClientH] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;
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
				if (m_pClientList[iClientH]->m_iAllocatedFish != 0) return 0;
				if (m_pClientList[iClientH]->map_ != map) return 0;
				m_pClientList[iClientH]->m_iAllocatedFish = dynObj.m_sOwner;
				m_pClientList[iClientH]->m_iFishChance = 1;
				m_pClientList[iClientH]->m_bSkillUsingStatus[1] = true;
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_EVENTFISHMODE, (m_pFish[dynObj.m_sOwner]->m_pItem->m_wPrice / 2), m_pFish[dynObj.m_sOwner]->m_pItem->m_sSprite,
						  m_pFish[dynObj.m_sOwner]->m_pItem->m_sSpriteFrame, m_pFish[dynObj.m_sOwner]->m_pItem->m_cName);
				m_pFish[dynObj.m_sOwner]->m_sEngagingCount++;
				return i;
			}
		}
	}
	return 0;
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

void CGame::AdminOrder_CreateFish(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cItemName[21];
	class CStrTok * pStrTok;
	class CItem * pItem;
	register int tX, tY, iType;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelCreateFish) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			iCreateFish(m_pClientList[iClientH]->map_, tX, tY, iType, pItem, 1, 60000 * 20);
		} else delete pItem;
	}
	delete pStrTok;
	return;
}

void CGame::AdminOrder_Teleport(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cMapName[256];
	class CStrTok * pStrTok;
	register int dX, dY;
	bool bFlag;
	dX = dY = -1;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelTeleport) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
	if (bFlag == true)
		m_pClientList[iClientH]->RequestTeleportHandler("2   ", cMapName, dX, dY);
	delete pStrTok;
	return;
}

void CGame::ReqGetFishThisTimeHandler(int iClientH) {
	register int iResult, iFishH;
	class CItem * pItem;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iAllocatedFish == 0) return;
	if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] == nullptr) return;
	m_pClientList[iClientH]->m_bSkillUsingStatus[1] = false;
	iResult = iDice(1, 100);
	if (m_pClientList[iClientH]->m_iFishChance >= iResult) {
		GetExp(iClientH, iDice(m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty, 5)); //m_pClientList[iClientH]->m_iExpStock += iDice(m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty, 5);
		m_pClientList[iClientH]->CalculateSSN_SkillIndex(1, m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty);
		pItem = m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem;
		m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem = nullptr;
		m_pClientList[iClientH]->map_->bSetItem(m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY,
				  pItem);
		m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FISHSUCCESS, 0, 0, 0, nullptr);
		iFishH = m_pClientList[iClientH]->m_iAllocatedFish;
		m_pClientList[iClientH]->m_iAllocatedFish = 0;
		bDeleteFish(iFishH, 1);
		return;
	}
	m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FISHFAIL, 0, 0, 0, nullptr);
	m_pClientList[iClientH]->m_iAllocatedFish = 0;
}

void CGame::FishGenerator() {
	register int i;
	int iP, tX, tY;
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
	int i, j;
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

int CGame::iGetPlayerRelationship(int iClientH, int iOpponentH) {
	int iRet;
	if (m_pClientList[iClientH] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;
	if (m_pClientList[iOpponentH] == nullptr) return 0;
	if (m_pClientList[iOpponentH]->m_bIsInitComplete == false) return 0;
	iRet = 0;
	if (m_pClientList[iClientH]->m_iPKCount != 0) {
		if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
				  (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) && (memcmp(m_pClientList[iOpponentH]->m_cLocation, "NONE", 4) != 0)) {
			iRet = 7;
		} else iRet = 2;
	} else if (m_pClientList[iOpponentH]->m_iPKCount != 0) {
		if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
				  (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0))
			iRet = 6;
		else iRet = 2;
	} else {
		if (m_pClientList[iClientH]->m_cSide != m_pClientList[iOpponentH]->m_cSide) {
			if ((m_pClientList[iClientH]->m_cSide != 0) && (m_pClientList[iOpponentH]->m_cSide != 0)) {
				iRet = 2;
			} else {
				iRet = 0;
			}
		} else {
			if ((memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) == 0) &&
					  (memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0)) {
				if (m_pClientList[iOpponentH]->m_iGuildRank == 0)
					iRet = 5;
				else iRet = 3;
			} else
				if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
					  (memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0) &&
					  (memcmp(m_pClientList[iOpponentH]->m_cGuildName, "NONE", 4) != 0) &&
					  (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) != 0)) {
				iRet = 4;
			} else iRet = 1;
		}
	}
	return iRet;
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

/*int CGame::iGetPlayerRelationship_SendEvent(int iClientH, int iOpponentH)
{
 int iRet;
	
	 
	if (m_pClientList[iClientH] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;
	if (m_pClientList[iOpponentH] == nullptr) return 0;
	if (m_pClientList[iOpponentH]->m_bIsInitComplete == false) return 0;
	iRet = 0;
	if (m_pClientList[iClientH]->m_iPKCount != 0) {
		
		if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) != 0) &&
			(memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) && (memcmp(m_pClientList[iOpponentH]->m_cLocation, "NONE", 4) != 0))
			 iRet = 2; 
		else iRet = 6; // PK
	}
	else if (m_pClientList[iOpponentH]->m_iPKCount != 0) {
		
		if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) != 0) &&
			(memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) && (memcmp(m_pClientList[iOpponentH]->m_cLocation, "NONE", 4) != 0))
			 iRet = 2; 
		else {
			iRet = 7; 
		}
	}
	else {
		
		if (m_pClientList[iClientH]->m_cSide != m_pClientList[iOpponentH]->m_cSide) {
			
			if ((m_pClientList[iClientH]->m_cSide != 0) && (m_pClientList[iOpponentH]->m_cSide != 0)) {
				
				iRet = 2;
			}
			else {
				
				iRet = 0;
			}
		}
		else {
			
			if ((m_pClientList[iClientH]->m_cSide == m_pClientList[iOpponentH]->m_cSide) &&
				(m_pClientList[iClientH]->m_cSide == 0)) {
				iRet = 0; 
			}
			else
			if ((memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) == 0) &&
				(memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0) ) {
				
				if (m_pClientList[iClientH]->m_iGuildRank == 0)
					 iRet = 5;	
				else iRet = 3;	
			}
			else
			if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
				(memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0) &&
				(memcmp(m_pClientList[iOpponentH]->m_cGuildName, "NONE", 4) != 0) &&
				(memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) != 0)) {
				
				iRet = 4; 
			}
			else {
				iRet = 1; 
			}
		}
	}
	return iRet;
}*/
int CGame::iGetNpcRelationship_SendEvent(int iNpcH, int iOpponentH) {
	int iRet;
	if (m_pClientList[iOpponentH] == nullptr) return 0;
	if (m_pClientList[iOpponentH]->m_bIsInitComplete == false) return 0;
	if (m_pNpcList[iNpcH] == nullptr) return 0;
	iRet = 0;
	if (m_pClientList[iOpponentH]->m_iPKCount != 0) {
		if (m_pNpcList[iNpcH]->m_cSide == m_pClientList[iOpponentH]->m_cSide)
			iRet = 7;
		else iRet = 2;
	} else {
		if (m_pNpcList[iNpcH]->m_cSide != m_pClientList[iOpponentH]->m_cSide) {
			if (m_pNpcList[iNpcH]->m_cSide == 10)
				iRet = 2;
			else
				if (m_pNpcList[iNpcH]->m_cSide == 0) iRet = 0;
			else
				if (m_pClientList[iOpponentH]->m_cSide == 0)
				iRet = 0;
			else iRet = 2;
		} else iRet = 1;
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

void CGame::AdminOrder_CheckIP(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cIP[21], cInfoString[500];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelCheckIP) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
			}
	}
	delete pStrTok;
}

void CGame::ToggleSafeAttackModeHandler(int iClientH) //v1.1
{
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	if (m_pClientList[iClientH]->m_bIsSafeAttackMode == true)
		m_pClientList[iClientH]->m_bIsSafeAttackMode = false;
	else m_pClientList[iClientH]->m_bIsSafeAttackMode = true;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, nullptr);
}

void CGame::NpcRequestAssistance(int iNpcH) {
	register int ix, iy, sX, sY;
	short sOwnerH;
	char cOwnerType;
	if (m_pNpcList[iNpcH] == nullptr) return;
	sX = m_pNpcList[iNpcH]->m_sX;
	sY = m_pNpcList[iNpcH]->m_sY;
	for (ix = sX - 8; ix <= sX + 8; ix++)
		for (iy = sY - 8; iy <= sY + 8; iy++) {
			m_pNpcList[iNpcH]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
			if ((sOwnerH != 0) && (m_pNpcList[sOwnerH] != nullptr) && (cOwnerType == DEF_OWNERTYPE_NPC) &&
					  (iNpcH != sOwnerH) && (m_pNpcList[sOwnerH]->m_cSide == m_pNpcList[iNpcH]->m_cSide) &&
					  (m_pNpcList[sOwnerH]->m_bIsPermAttackMode == false) && (m_pNpcList[sOwnerH]->m_cBehavior == DEF_BEHAVIOR_MOVE)) {
				m_pNpcList[sOwnerH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				m_pNpcList[sOwnerH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[sOwnerH]->m_iTargetIndex = m_pNpcList[iNpcH]->m_iTargetIndex;
				m_pNpcList[sOwnerH]->m_cTargetType = m_pNpcList[iNpcH]->m_cTargetType;
				return;
			}
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

void CGame::AdminOrder_Polymorph(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelPolymorph) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (memcmp(token, "off", 3) == 0)
			m_pClientList[iClientH]->m_sType = m_pClientList[iClientH]->m_sOriginalType;
		else if (memcmp(token, "Slime", 5) == 0)
			m_pClientList[iClientH]->m_sType = 10;
		else if (memcmp(token, "Skeleton", 8) == 0)
			m_pClientList[iClientH]->m_sType = 11;
		else if (memcmp(token, "Stone-Golem", 11) == 0)
			m_pClientList[iClientH]->m_sType = 12;
		else if (memcmp(token, "Cyclops", 7) == 0)
			m_pClientList[iClientH]->m_sType = 13;
		else if (memcmp(token, "Orc", 3) == 0)
			m_pClientList[iClientH]->m_sType = 14;
		else if (memcmp(token, "ShopKeeper", 10) == 0)
			m_pClientList[iClientH]->m_sType = 15;
		else if (memcmp(token, "Giant-Ant", 9) == 0)
			m_pClientList[iClientH]->m_sType = 16;
		else if (memcmp(token, "Scorpion", 8) == 0)
			m_pClientList[iClientH]->m_sType = 17;
		else if (memcmp(token, "Zombie", 6) == 0)
			m_pClientList[iClientH]->m_sType = 18;
		else if (memcmp(token, "Gandlf", 6) == 0)
			m_pClientList[iClientH]->m_sType = 19;
		else if (memcmp(token, "Howard", 6) == 0)
			m_pClientList[iClientH]->m_sType = 20;
		else if (memcmp(token, "Gaurd", 5) == 0)
			m_pClientList[iClientH]->m_sType = 21;
		else if (memcmp(token, "Amphis", 6) == 0)
			m_pClientList[iClientH]->m_sType = 22;
		else if (memcmp(token, "Clay-Golem", 10) == 0)
			m_pClientList[iClientH]->m_sType = 23;
		else if (memcmp(token, "Tom", 3) == 0)
			m_pClientList[iClientH]->m_sType = 24;
		else if (memcmp(token, "William", 7) == 0)
			m_pClientList[iClientH]->m_sType = 25;
		else if (memcmp(token, "Kennedy", 7) == 0)
			m_pClientList[iClientH]->m_sType = 26;
		else if (memcmp(token, "Hellbound", 9) == 0)
			m_pClientList[iClientH]->m_sType = 27;
		else if (memcmp(token, "Troll", 5) == 0)
			m_pClientList[iClientH]->m_sType = 28;
		else if (memcmp(token, "Orge", 4) == 0)
			m_pClientList[iClientH]->m_sType = 29;
		else if (memcmp(token, "Liche", 5) == 0)
			m_pClientList[iClientH]->m_sType = 30;
		else if (memcmp(token, "Demon", 5) == 0)
			m_pClientList[iClientH]->m_sType = 31;
		else if (memcmp(token, "Unicorn", 7) == 0)
			m_pClientList[iClientH]->m_sType = 32;
		else if (memcmp(token, "WereWolf", 8) == 0)
			m_pClientList[iClientH]->m_sType = 33;
		else if (memcmp(token, "Dummy", 5) == 0)
			m_pClientList[iClientH]->m_sType = 34;
		else if (memcmp(token, "Energy-Sphere", 13) == 0)
			m_pClientList[iClientH]->m_sType = 35;
		else if (memcmp(token, "AGT", 3) == 0)
			m_pClientList[iClientH]->m_sType = 36;
		else if (memcmp(token, "CGT", 3) == 0)
			m_pClientList[iClientH]->m_sType = 37;
		else if (memcmp(token, "MS", 2) == 0)
			m_pClientList[iClientH]->m_sType = 38;
		else if (memcmp(token, "DT", 2) == 0)
			m_pClientList[iClientH]->m_sType = 39;
		else if (memcmp(token, "ESG", 3) == 0)
			m_pClientList[iClientH]->m_sType = 40;
		else if (memcmp(token, "GMG", 3) == 0)
			m_pClientList[iClientH]->m_sType = 41;
		else if (memcmp(token, "ManaStone", 9) == 0)
			m_pClientList[iClientH]->m_sType = 42;
		else if (memcmp(token, "LWB", 3) == 0)
			m_pClientList[iClientH]->m_sType = 43;
		else if (memcmp(token, "GHK", 3) == 0)
			m_pClientList[iClientH]->m_sType = 44;
		else if (memcmp(token, "GHC", 6) == 0)
			m_pClientList[iClientH]->m_sType = 45;
		else if (memcmp(token, "TK", 2) == 0)
			m_pClientList[iClientH]->m_sType = 46;
		else if (memcmp(token, "BG", 2) == 0)
			m_pClientList[iClientH]->m_sType = 47;
		else if (memcmp(token, "Stalker", 7) == 0)
			m_pClientList[iClientH]->m_sType = 48;
		else if (memcmp(token, "Hellclaw", 8) == 0)
			m_pClientList[iClientH]->m_sType = 49;
		else if (memcmp(token, "Tigerworm", 8) == 0)
			m_pClientList[iClientH]->m_sType = 50;
		else if (memcmp(token, "CP", 2) == 0)
			m_pClientList[iClientH]->m_sType = 51;
		else if (memcmp(token, "Gagoyle", 7) == 0)
			m_pClientList[iClientH]->m_sType = 52;
		else if (memcmp(token, "Beholder", 8) == 0)
			m_pClientList[iClientH]->m_sType = 53;
		else if (memcmp(token, "Dark-Elf", 8) == 0)
			m_pClientList[iClientH]->m_sType = 54;
		else if (memcmp(token, "Rabbit", 6) == 0)
			m_pClientList[iClientH]->m_sType = 55;
		else if (memcmp(token, "Cat", 3) == 0)
			m_pClientList[iClientH]->m_sType = 56;
		else if (memcmp(token, "Giant-Frog", 10) == 0)
			m_pClientList[iClientH]->m_sType = 57;
		else if (memcmp(token, "Mountain-Giant", 14) == 0)
			m_pClientList[iClientH]->m_sType = 58;
		else if (memcmp(token, "Ettin", 5) == 0)
			m_pClientList[iClientH]->m_sType = 59;
		else if (memcmp(token, "Cannibal-Plant", 13) == 0)
			m_pClientList[iClientH]->m_sType = 60;
		else if (memcmp(token, "Rudolph", 7) == 0)
			m_pClientList[iClientH]->m_sType = 61;
		else if (memcmp(token, "DireBoar", 8) == 0)
			m_pClientList[iClientH]->m_sType = 62;
		else if (memcmp(token, "Frost", 5) == 0)
			m_pClientList[iClientH]->m_sType = 63;
		else if (memcmp(token, "Crops", 5) == 0)
			m_pClientList[iClientH]->m_sType = 64;
		else if (memcmp(token, "Ice-Golem", 9) == 0)
			m_pClientList[iClientH]->m_sType = 65;
			//else if (memcmp(token, "Wyvern", 6) == 0)
			//	m_pClientList[iClientH]->m_sType = 66;
		else if (memcmp(token, "McGaffin", 8) == 0)
			m_pClientList[iClientH]->m_sType = 67;
		else if (memcmp(token, "Perry", 5) == 0)
			m_pClientList[iClientH]->m_sType = 68;
		else if (memcmp(token, "Devlin", 6) == 0)
			m_pClientList[iClientH]->m_sType = 69;
		else if (memcmp(token, "Barlog", 6) == 0)
			m_pClientList[iClientH]->m_sType = 70;
		else if (memcmp(token, "Centaurus", 9) == 0)
			m_pClientList[iClientH]->m_sType = 71;
		else if (memcmp(token, "Claw-Turtle", 11) == 0)
			m_pClientList[iClientH]->m_sType = 72;
			//else if (memcmp(token, "Fire-Wyvern", 11) == 0)
			//	m_pClientList[iClientH]->m_sType = 73;
		else if (memcmp(token, "Giant-Crayfish", 14) == 0)
			m_pClientList[iClientH]->m_sType = 74;
		else if (memcmp(token, "Giant-Lizard", 12) == 0)
			m_pClientList[iClientH]->m_sType = 75;
		else if (memcmp(token, "Giant-Plant", 11) == 0)
			m_pClientList[iClientH]->m_sType = 76;
		else if (memcmp(token, "MasterMage-Orc", 14) == 0)
			m_pClientList[iClientH]->m_sType = 77;
		else if (memcmp(token, "Minotaurs", 9) == 0)
			m_pClientList[iClientH]->m_sType = 78;
		else if (memcmp(token, "Nizie", 5) == 0)
			m_pClientList[iClientH]->m_sType = 79;
		else if (memcmp(token, "Tentocle", 8) == 0)
			m_pClientList[iClientH]->m_sType = 80;
			//else if (memcmp(token, "Abaddon", 7) == 0)
			//	m_pClientList[iClientH]->m_sType = 81;
		else if (memcmp(token, "Sor", 3) == 0)
			m_pClientList[iClientH]->m_sType = 82;
		else if (memcmp(token, "ATK", 3) == 0)
			m_pClientList[iClientH]->m_sType = 83;
		else if (memcmp(token, "Elf", 3) == 0)
			m_pClientList[iClientH]->m_sType = 84;
		else if (memcmp(token, "DSK", 3) == 0)
			m_pClientList[iClientH]->m_sType = 85;
		else if (memcmp(token, "HBT", 3) == 0)
			m_pClientList[iClientH]->m_sType = 86;
		else if (memcmp(token, "CT", 2) == 0)
			m_pClientList[iClientH]->m_sType = 87;
		else if (memcmp(token, "Bar", 3) == 0)
			m_pClientList[iClientH]->m_sType = 88;
		else if (memcmp(token, "AGC", 3) == 0)
			m_pClientList[iClientH]->m_sType = 89;
		else if (memcmp(token, "Gail", 4) == 0)
			m_pClientList[iClientH]->m_sType = 90;
		else if (memcmp(token, "gate", 4) == 0)
			m_pClientList[iClientH]->m_sType = 91;
	}
	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	delete pStrTok;
}

void CGame::AdminOrder_SetInvi(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSetInvis) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '1') SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		else
			if (token[0] == '0') SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
	}
	delete pStrTok;
}

bool CGame::bOnClose() {
	if (m_bIsServerShutdowned == false)
		if (MessageBox(nullptr, "Player data not saved! Shutdown server now?", m_cServerName, MB_ICONEXCLAMATION | MB_YESNO) == IDYES) return true;
		else return false;
	else return true;
	return false;
}

void CGame::AdminOrder_SetZerk(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSetZerk) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '1') {
			SetBerserkFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
			m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = (char) m_pMagicConfigList[32]->m_sValue4;
		} else
			if (token[0] == '0') SetBerserkFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
	}
	delete pStrTok;
}

void CGame::AdminOrder_SetFreeze(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSetIce) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '1') SetIceFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		else
			if (token[0] == '0') SetIceFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
	}
	delete pStrTok;
}
// 05/24/2004 - Hypnotoad - Hammer and Wand train to 100% fixed

void CGame::_CheckAttackType(int iClientH, short *spType) {
	uint16_t wType;
	if (m_pClientList[iClientH] == nullptr) return;
	wType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
	switch (*spType) {
		case 2:
			if (m_pClientList[iClientH]->m_cArrowIndex == -1) *spType = 0;
			if (wType < 40) *spType = 1;
			break;
		case 20:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[5] < 100) *spType = 1;
			break;
		case 21:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[7] < 100) *spType = 1;
			break;
		case 22:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[9] < 100) *spType = 1;
			break;
		case 23:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[8] < 100) *spType = 1;
			break;
		case 24:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[10] < 100) *spType = 1;
			break;
		case 25:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 2;
			if (m_pClientList[iClientH]->m_cSkillMastery[6] < 100) *spType = 2;
			if (m_pClientList[iClientH]->m_cArrowIndex == -1) *spType = 0;
			if (wType < 40) *spType = 1;
			break;
		case 26:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[14] < 100) *spType = 1;
			break;
		case 27:
			if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0) *spType = 1;
			if (m_pClientList[iClientH]->m_cSkillMastery[21] < 100) *spType = 1;
			break;
	}
}

void CGame::ReqCreatePortionHandler(int iClientH, char *pData) {
	uint32_t * dwp;
	uint16_t * wp;
	char * cp, cI[6], cPortionName[21], cData[120];
	int iRet, i, j, iEraseReq, iSkillLimit, iSkillLevel, iResult, iDifficulty;
	short * sp, sItemIndex[6], sTemp;
	short sItemNumber[6], sItemArray[12];
	bool bDup, bFlag;
	class CItem * pItem;
	if (m_pClientList[iClientH] == nullptr) return;
	m_pClientList[iClientH]->m_iSkillMsgRecvCount++;
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
		if ((cI[i] >= 0) && (m_pClientList[iClientH]->m_pItemList[cI[i]] == nullptr)) return;
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
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]] == nullptr) return;
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount < sItemNumber[i]) return;
		}
	bFlag = true;
	while (bFlag == true) {
		bFlag = false;
		for (i = 0; i < 5; i++)
			if ((sItemIndex[i] != -1) && (sItemIndex[i + 1] != -1)) {
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum) <
						  (m_pClientList[iClientH]->m_pItemList[sItemIndex[i + 1]]->m_sIDnum)) {
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
			 wsprintf(cTemp, "(%d) ", m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum);
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
			sItemArray[j] = m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum;
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
			if (bFlag == false) {
				std::memset(cPortionName, 0, sizeof (cPortionName));
				memcpy(cPortionName, m_pPortionConfigList[i]->m_cName, 20);
				iSkillLimit = m_pPortionConfigList[i]->m_iSkillLimit;
				iDifficulty = m_pPortionConfigList[i]->m_iDifficulty;
			}
		}
	if (strlen(cPortionName) == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOMATCHINGPORTION, 0, 0, 0, nullptr);
		return;
	}
	iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[12];
	if (iSkillLimit > iSkillLevel) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_LOWPORTIONSKILL, 0, 0, 0, cPortionName);
		return;
	}
	iSkillLevel -= iDifficulty;
	if (iSkillLevel <= 0) iSkillLevel = 1;
	iResult = iDice(1, 100);
	if (iResult > iSkillLevel) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PORTIONFAIL, 0, 0, 0, cPortionName);
		return;
	}
	m_pClientList[iClientH]->CalculateSSN_SkillIndex(12, 1);
	if (strlen(cPortionName) != 0) {
		pItem = nullptr;
		pItem = new class CItem;
		if (pItem == nullptr) return;
		for (i = 0; i < 6; i++)
			if (sItemIndex[i] != -1) {
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME)
					// v1.41 !!!
					SetItemCount(iClientH, sItemIndex[i], //     m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cName,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount - sItemNumber[i]);
				else m_pClientList[iClientH]->ItemDepleteHandler(sItemIndex[i], false, true);
			}
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PORTIONSUCCESS, 0, 0, 0, cPortionName);
		m_pClientList[iClientH]->m_iExpStock += iDice(1, (iDifficulty / 3));
		if ((_bInitItemAttr(*pItem, cPortionName) == true)) {
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
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
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						break;
				}
				//if ((pItem->m_wPrice * pItem->m_dwCount) > 1000)
				//	SendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, iClientH);
			} else {
				m_pClientList[iClientH]->map_->bSetItem(m_pClientList[iClientH]->m_sX,
						  m_pClientList[iClientH]->m_sY, pItem);
				m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
						  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
						  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						break;
				}
			}
		} else {
			delete pItem;
			pItem = nullptr;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
///		Snoopy: Added Crafting to the same file than potions
//////////////////////////////////////////////////////////////////////////////////////////

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

void CGame::LocalSavePlayerData(int iClientH) {
	char * pData, * cp, cFn[256], cDir[256], cTxt[256], cCharDir[256];
	int iSize;
	FILE * pFile;
	SYSTEMTIME SysTime;
	if (m_pClientList[iClientH] == nullptr) return;
	pData = new char[30000];
	if (pData == nullptr) return;
	ZeroMemory(pData, 30000);
	cp = (char *) (pData);
	iSize = _iComposePlayerDataFileContents(iClientH, cp);
	GetLocalTime(&SysTime);
	ZeroMemory(cCharDir, sizeof (cDir));
	wsprintf(cCharDir, "Character_%d_%d_%d_%d", SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	std::memset(cDir, 0, sizeof (cDir));
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, cCharDir);
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	wsprintf(cTxt, "AscII%d", (unsigned char) m_pClientList[iClientH]->m_cCharName[0]);
	strcat(cFn, cTxt);
	strcpy(cDir, cFn);
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, m_pClientList[iClientH]->m_cCharName);
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

void CGame::_CheckMiningAction(int iClientH, int dX, int dY) {
	short sType;
	uint32_t dwRegisterTime;
	int iDynamicIndex, iSkillLevel, iResult, iItemID;
	class CItem * pItem;
	uint16_t wWeaponType;
	if (m_pClientList[iClientH] == nullptr) return;
	m_pClientList[iClientH]->map_->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);
	switch (sType) {
		case DEF_DYNAMICOBJECT_MINERAL1:
		case DEF_DYNAMICOBJECT_MINERAL2:
			wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
			if (wWeaponType == 25) {
			} else return;
			if ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) == 0) return;
			iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[0];
			if (iSkillLevel == 0) break;
			if (dynamicObjects_[iDynamicIndex] == nullptr) break;
			iSkillLevel -= m_pMineral[dynamicObjects_[iDynamicIndex]->m_iV1]->m_iDifficulty;
			if (iSkillLevel <= 0) iSkillLevel = 1;
			iResult = iDice(1, 100);
			if (iResult <= iSkillLevel) {
				m_pClientList[iClientH]->CalculateSSN_SkillIndex(0, 1);
				switch (m_pMineral[dynamicObjects_[iDynamicIndex]->m_iV1]->m_cType) {
					case 1:
						switch (iDice(1, 5)) {
							case 1:
							case 2:
							case 3:
								iItemID = 355; // Coal
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 4:
								iItemID = 357; // IronOre
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 5:
								iItemID = 507; // BlondeStone
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
						}
						break;
					case 2:
						switch (iDice(1, 5)) {
							case 1:
							case 2:
								iItemID = 355; // Coal
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 3:
							case 4:
								iItemID = 357; // IronOre
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 5:
								if (iDice(1, 3) == 2) {
									iItemID = 356; // SilverNugget
									m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
								} else {
									iItemID = 507; // BlondeStone
									m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								}
								break;
						}
						break;
					case 3:
						switch (iDice(1, 6)) {
							case 1:
								iItemID = 355; // Coal
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 2:
							case 3:
							case 4:
							case 5:
								iItemID = 357; // IronOre
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 6:
								if (iDice(1, 8) == 3) {
									if (iDice(1, 2) == 1) {
										iItemID = 356; // SilverNugget
										m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
									} else {
										iItemID = 357; // IronOre
										m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
									}
									break;
								} else {
									iItemID = 357; // IronOre
									m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								}
								break;
						}
						break;
					case 4:
						switch (iDice(1, 6)) {
							case 1:
								iItemID = 355; // Coal
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 2:
								if (iDice(1, 3) == 2) {
									iItemID = 356; // SilverNugget
									m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
								}
								break;
							case 3:
							case 4:
							case 5:
								iItemID = 357; // IronOre
								m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
								break;
							case 6:
								if (iDice(1, 8) == 3) {
									if (iDice(1, 4) == 3) {
										if (iDice(1, 4) < 3) {
											iItemID = 508; // Mithral
											m_pClientList[iClientH]->m_iExpStock += iDice(1, 6);
										} else {
											iItemID = 354; // GoldNugget
											m_pClientList[iClientH]->m_iExpStock += iDice(1, 5);
										}
										break;
									} else {
										iItemID = 356; // SilverNugget
										m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
									}
									break;
								} else {
									if (iDice(1, 2) == 1) {
										iItemID = 354; // GoldNugget
										m_pClientList[iClientH]->m_iExpStock += iDice(1, 5);
									} else {
										iItemID = 357; // IronOre
										m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
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
								m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								break;
							default:
								iItemID = 358; // Crystal
								m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								break;
						}
						break;
					case 6:
						switch (iDice(1, 5)) {
							case 1:
								if (iDice(1, 6) == 3) {
									iItemID = 353; // Emerald
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 4);
								} else {
									iItemID = 358; // Crystal
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								}
								break;
							case 2:
								if (iDice(1, 6) == 3) {
									iItemID = 352; // Saphire
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 4);
								} else {
									iItemID = 358; // Crystal
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								}
								break;
							case 3:
								if (iDice(1, 6) == 3) {
									iItemID = 351; // Ruby
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 4);
								} else {
									iItemID = 358; // Crystal
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								}
								break;
							case 4:
								iItemID = 358; // Crystal
								m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								break;
							case 5:
								if (iDice(1, 12) == 3) {
									iItemID = 350; // Diamond
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 5);
								} else {
									iItemID = 358; // Crystal
									m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
								}
								break;
						}
						break;
				}
				pItem = new class CItem;
				if (_bInitItemAttr(*pItem, iItemID) == false) {
					delete pItem;
				} else {
					m_pClientList[iClientH]->map_->bSetItem(m_pClientList[iClientH]->m_sX,
							  m_pClientList[iClientH]->m_sY, pItem);
					m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
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

bool CGame::bDeleteMineral(int iIndex) {
	int iDynamicIndex;
	uint32_t dwTime;
	dwTime = timeGetTime();
	if (m_pMineral[iIndex] == nullptr) return false;
	iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
	if (dynamicObjects_[iDynamicIndex] == nullptr) return false;
	dynamicObjects_[iDynamicIndex]->map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT,
			  dynamicObjects_[iDynamicIndex]->m_sX, dynamicObjects_[iDynamicIndex]->m_sY,
			  dynamicObjects_[iDynamicIndex]->m_sType, iDynamicIndex, 0);
	dynamicObjects_[iDynamicIndex]->map_->SetDynamicObject(0, 0, dynamicObjects_[iDynamicIndex]->m_sX, dynamicObjects_[iDynamicIndex]->m_sY, dwTime);
	m_pMineral[iIndex]->map_->SetTempMoveAllowedFlag(dynamicObjects_[iDynamicIndex]->m_sX, dynamicObjects_[iDynamicIndex]->m_sY, true);
	m_pMineral[iIndex]->map_->m_iCurMineral--;
	delete m_pMineral[iIndex];
	m_pMineral[iIndex] = nullptr;
	return true;
}

void CGame::NpcTalkHandler(int iClientH, int iWho) {
	char cRewardName[21], cTargetName[21];
	int iResMode, iQuestNum, iQuestType, iRewardType, iRewardAmount, iContribution, iX, iY, iRange, iTargetType, iTargetCount;
	iQuestNum = 0;
	std::memset(cTargetName, 0, sizeof (cTargetName));
	if (m_pClientList[iClientH] == nullptr) return;
	switch (iWho) {
		case 1: break;
		case 2: break;
		case 3: break;
		case 4:
			iQuestNum = _iTalkToNpcResult_Cityhall(iClientH, &iQuestType, &iResMode, &iRewardType, &iRewardAmount, &iContribution, cTargetName, &iTargetType, &iTargetCount, &iX, &iY, &iRange);
			break;
		case 5: break;
		case 6: break;
		case 32: break;
		case 21:
			iQuestNum = _iTalkToNpcResult_Guard(iClientH, &iQuestType, &iResMode, &iRewardType, &iRewardAmount, &iContribution, cTargetName, &iTargetType, &iTargetCount, &iX, &iY, &iRange);
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
		m_pClientList[iClientH]->m_iAskedQuest = iQuestNum;
		m_pClientList[iClientH]->m_iQuestRewardType = iRewardType;
		m_pClientList[iClientH]->m_iQuestRewardAmount = iRewardAmount;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, iQuestType, iResMode, iRewardAmount, cRewardName, iContribution,
				  iTargetType, iTargetCount, iX, iY, iRange, cTargetName);
	} else {
		switch (iQuestNum) {
			case 0: m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (iWho + 130), 0, 0, nullptr, 0);
				break;
			case -1:
			case -2:
			case -3:
			case -4: m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, abs(iQuestNum) + 100, 0, 0, nullptr, 0);
				break;
			case -5: break;
		}
	}
}

void CGame::AdminOrder_GetNpcStatus(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cNpcName[256];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelGetNpcStatus) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINIFO, 1, i, 0, nullptr);
			}
		}
	delete pStrTok;
	return;
}

int CGame::_iGetWeaponSkillType(int iClientH) {
	uint16_t wWeaponType;
	if (m_pClientList[iClientH] == nullptr) return 0;
	wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
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

void CGame::SetDownSkillIndexHandler(int iClientH, int iSkillIndex) {
	if (m_pClientList[iClientH] == nullptr) return;
	if ((iSkillIndex < 0) || (iSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (m_pClientList[iClientH]->m_cSkillMastery[iSkillIndex] > 0)
		m_pClientList[iClientH]->m_iDownSkillIndex = iSkillIndex;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, 0, 0, nullptr);
}

bool CGame::_bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID) {
	bool bRet;
	if (m_pClientList[iClientH] == nullptr) return false;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return false;
	switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
		case DEF_ITEMEFFECTTYPE_OCCUPYFLAG:
			bRet = m_pClientList[iClientH]->map_->__bSetOccupyFlag(dX, dY,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,
					  0, iClientH, 0);
			if (bRet == true) {
				GetExp(iClientH, (iDice(m_pClientList[iClientH]->m_iLevel, 10)));
			} else {
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOTFLAGSPOT, 0, 0, 0, nullptr);
			}
			return bRet;
			// crusade
		case DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT:
			bRet = __bSetConstructionKit(dX, dY, //
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1, //
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2, //
					  iClientH);
			if (bRet == true) {
			} else {
			}
			return bRet;
		case DEF_ITEMEFFECTTYPE_DYE:
			if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
				if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != nullptr) {
					if ((m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 11) ||
							  (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 12)) {
						m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, 0, nullptr);
						return true;
					} else {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, nullptr);
						return false;
					}
				}
			}
			break;
		case DEF_ITEMEFFECTTYPE_ARMORDYE:
			if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
				if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != nullptr) {
					if (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 6) {
						m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, 0, nullptr);
						return true;
					} else {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, nullptr);
						return false;
					}
				}
			}
			break;
		case DEF_ITEMEFFECTTYPE_FARMING:
			bRet = bPlantSeedBag(dX, dY,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,
					  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2,
					  iClientH);
			return bRet;
		default:
			break;
	}
	return true;
}

void CGame::SetSummonMobAction(int iClientH, int iMode, uint32_t dwMsgSize, char *pData) {
	register int i, iTargetIndex;
	char seps[] = "= \t\n";
	char * token, cTargetName[11], cBuff[256];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_cSide == 0) return;
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
							  (strcmp(m_pClientList[iClientH]->map_->m_cName, m_pClientList[i]->map_->m_cName) == 0)) {
						iTargetIndex = i;
						goto SSMA_SKIPSEARCH;
					}
				}
			}
SSMA_SKIPSEARCH:
			;
			if ((iTargetIndex != 0) && (m_pClientList[iTargetIndex]->m_cSide != 0) &&
					  (m_pClientList[iTargetIndex]->m_cSide != m_pClientList[iClientH]->m_cSide)) {
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

void CGame::GetOccupyFlagHandler(int iClientH) {
	int i, iNum, iRet, iEraseReq, iEKNum;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iEnemyKillCount < 3) return;
	if (m_pClientList[iClientH]->m_cSide == 0) return;
	std::memset(cItemName, 0, sizeof (cItemName));
	switch (m_pClientList[iClientH]->m_cSide) {
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
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;
				if (m_pClientList[iClientH]->m_iEnemyKillCount >= 10) {
					iEKNum = 10;
					m_pClientList[iClientH]->m_iEnemyKillCount -= 10;
				} else {
					iEKNum = m_pClientList[iClientH]->m_iEnemyKillCount;
					m_pClientList[iClientH]->m_iEnemyKillCount = iEKNum;
				}
				pItem->m_sItemSpecEffectValue1 = iEKNum;
				wsprintf(G_cTxt, "PC(%s) obtained a Flag (%d).   %s(%d %d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cSide, m_pClientList[iClientH]->map_->m_cName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
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
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
				m_pClientList[iClientH]->iCalcTotalWeight();
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						return;
				}
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, 0, 0, nullptr);
			} else {
				delete pItem;
				m_pClientList[iClientH]->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CGame::GetFightzoneTicketHandler(int iClientH) {
	int iRet, iEraseReq, iMonth, iDay, iHour;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iFightZoneTicketNumber <= 0) {
		m_pClientList[iClientH]->m_iFightzoneNumber *= -1;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_FIGHTZONERESERVE, -1, 0, 0, nullptr);
		return;
	}
	std::memset(cItemName, 0, sizeof (cItemName));
	if (m_pClientList[iClientH]->m_iFightzoneNumber == 1)
		strcpy(cItemName, "ArenaTicket");
	else wsprintf(cItemName, "ArenaTicket(%d)", m_pClientList[iClientH]->m_iFightzoneNumber);
	pItem = new class CItem;
	if (_bInitItemAttr(*pItem, cItemName) == false) {
		delete pItem;
		return;
	}
	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
		if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;
		m_pClientList[iClientH]->m_iFightZoneTicketNumber = m_pClientList[iClientH]->m_iFightZoneTicketNumber - 1;
		pItem->m_sTouchEffectType = DEF_ITET_DATE;
		iMonth = m_pClientList[iClientH]->m_iReserveTime / 10000;
		iDay = (m_pClientList[iClientH]->m_iReserveTime - iMonth * 10000) / 100;
		iHour = m_pClientList[iClientH]->m_iReserveTime - iMonth * 10000 - iDay * 100;
		pItem->m_sTouchEffectValue1 = iMonth;
		pItem->m_sTouchEffectValue2 = iDay;
		pItem->m_sTouchEffectValue3 = iHour;
		wsprintf(G_cTxt, "(*) Get FIGHTZONETICKET : Char(%s) TICKENUMBER (%d)(%d)(%d)", m_pClientList[iClientH]->m_cCharName, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
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
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
		m_pClientList[iClientH]->iCalcTotalWeight();
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				m_pClientList[iClientH]->DeleteClient(true, true);
				return;
		}
	} else {
		delete pItem;
		m_pClientList[iClientH]->iCalcTotalWeight();
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
		switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				m_pClientList[iClientH]->DeleteClient(true, true);
				return;
		}
	}
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
	int dX, dY, iEKNum, iTotalFlags;
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

void CGame::GetHeroMantleHandler(int iClientH, int iItemID, char */*pString*/) {
	int i, iNum, iRet, iEraseReq;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
	if (m_pClientList[iClientH]->m_cSide == 0) return;
	if (_iGetItemSpaceLeft(iClientH) == 0) {
		SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	//Prevents a crash if item dosent exist
	if (m_pItemConfigList[iItemID] == nullptr) return;
	switch (iItemID) {
			// Hero Cape
		case 400: //Aresden HeroCape
		case 401: //Elvine HeroCape
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 300) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
			break;
			// Hero Helm
		case 403: //Aresden HeroHelm(M)
		case 404: //Aresden HeroHelm(W)
		case 405: //Elvine HeroHelm(M)
		case 406: //Elvine HeroHelm(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 150) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
			if (m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
			// Hero Cap
		case 407: //Aresden HeroCap(M)
		case 408: //Aresden HeroCap(W)
		case 409: //Elvine HeroHelm(M)
		case 410: //Elvine HeroHelm(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
			if (m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
			// Hero Armour
		case 411: //Aresden HeroArmour(M)
		case 412: //Aresden HeroArmour(W)
		case 413: //Elvine HeroArmour(M)
		case 414: //Elvine HeroArmour(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 300) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
			if (m_pClientList[iClientH]->m_iContribution < 30) return;
			m_pClientList[iClientH]->m_iContribution -= 30;
			break;
			// Hero Robe
		case 415: //Aresden HeroRobe(M)
		case 416: //Aresden HeroRobe(W)
		case 417: //Elvine HeroRobe(M)
		case 418: //Elvine HeroRobe(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 200) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 200;
			if (m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
			// Hero Hauberk
		case 419: //Aresden HeroHauberk(M)
		case 420: //Aresden HeroHauberk(W)
		case 421: //Elvine HeroHauberk(M)
		case 422: //Elvine HeroHauberk(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
			if (m_pClientList[iClientH]->m_iContribution < 10) return;
			m_pClientList[iClientH]->m_iContribution -= 10;
			break;
			// Hero Leggings
		case 423: //Aresden HeroLeggings(M)
		case 424: //Aresden HeroLeggings(W)
		case 425: //Elvine HeroLeggings(M)
		case 426: //Elvine HeroLeggings(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 150) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
			if (m_pClientList[iClientH]->m_iContribution < 15) return;
			m_pClientList[iClientH]->m_iContribution -= 15;
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
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;
				wsprintf(G_cTxt, "(*) Get HeroItem : Char(%s) Player-EK(%d) Player-Contr(%d) Hero Obtained(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iEnemyKillCount, m_pClientList[iClientH]->m_iContribution, cItemName);
				PutLogFileList(G_cTxt);
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
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
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
				m_pClientList[iClientH]->iCalcTotalWeight();
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						return;
				}
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, 0, 0, nullptr);
			} else {
				delete pItem;
				m_pClientList[iClientH]->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CGame::_SetItemPos(CClient &client, char *pData) {
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
	if (client.m_pItemList[cItemIndex] != nullptr) {
		client.m_ItemPosList[cItemIndex].x = sX;
		client.m_ItemPosList[cItemIndex].y = sY;
	}
}

void CGame::CheckUniqueItemEquipment(int iClientH) {
	register int i, iDamage;
	if (m_pClientList[iClientH] == nullptr) return;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {
			if ((m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER) &&
					  (m_pClientList[iClientH]->m_bIsItemEquipped[i] == true)) {
				if ((m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue1 == m_pClientList[iClientH]->m_sCharIDnum1) &&
						  (m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue2 == m_pClientList[iClientH]->m_sCharIDnum2) &&
						  (m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue3 == m_pClientList[iClientH]->m_sCharIDnum3)) {
				} else {
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[i]->m_cEquipPos, i, 0, nullptr);
					m_pClientList[iClientH]->ReleaseItemHandler(i, true);
					iDamage = iDice(10, 10);
					m_pClientList[iClientH]->m_iHP -= iDamage;
					if (m_pClientList[iClientH]->m_iHP <= 0) {
						m_pClientList[iClientH]->ClientKilledHandler(0, 0, iDamage);
					}
				}
			}
		}
}

void CGame::_BWM_Init(CClient &client, char */*pData*/) {
	client.m_bIsBWMonitor = true;
	wsprintf(G_cTxt, "(*) BWMonitor(%d) registered.", client.id_);
	PutLogList(G_cTxt);
}

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

void CGame::ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char */*pItemName*/) {
	short sOwnerH;
	char cOwnerType;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == nullptr) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount < iAmount) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
	if (m_pClientList[iClientH]->m_bIsExchangeMode == true) return;
	if (wObjectID >= DEF_MAXCLIENTS) return;
	m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
	if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
		if ((m_bAdminSecurity == true) && (m_pClientList[sOwnerH]->m_iAdminUserLevel > 0)) {
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
			_ClearExchangeStatus(iClientH);
		} else {
			if ((m_pClientList[sOwnerH]->m_bIsExchangeMode == true) || (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) ||
					  (m_pClientList[sOwnerH]->map_->m_bIsFightZone == true)) {
				_ClearExchangeStatus(iClientH);
			} else {
				m_pClientList[iClientH]->m_bIsExchangeMode = true;
				m_pClientList[iClientH]->exchangingWith_ = m_pClientList[sOwnerH]->shared_from_this();
				//Clear items in the list
				m_pClientList[iClientH]->iExchangeCount = 0;
				m_pClientList[sOwnerH]->iExchangeCount = 0;
				for (int i = 0; i < 4; i++) {
					//Clear the trader
					std::memset(m_pClientList[iClientH]->m_cExchangeItemName[i], 0, sizeof (m_pClientList[iClientH]->m_cExchangeItemName[i]));
					m_pClientList[iClientH]->m_cExchangeItemIndex[i] = -1;
					m_pClientList[iClientH]->m_iExchangeItemAmount[i] = 0;
					//Clear the guy we're trading with
					std::memset(m_pClientList[sOwnerH]->m_cExchangeItemName[i], 0, sizeof (m_pClientList[sOwnerH]->m_cExchangeItemName[i]));
					m_pClientList[sOwnerH]->m_cExchangeItemIndex[i] = -1;
					m_pClientList[sOwnerH]->m_iExchangeItemAmount[i] = 0;
				}
				m_pClientList[iClientH]->m_cExchangeItemIndex[m_pClientList[iClientH]->iExchangeCount] = (char) sItemIndex;
				m_pClientList[iClientH]->m_iExchangeItemAmount[m_pClientList[iClientH]->iExchangeCount] = iAmount;
				//std::memset(m_pClientList[iClientH]->m_cExchangeItemName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName));
				memcpy(m_pClientList[iClientH]->m_cExchangeItemName[m_pClientList[iClientH]->iExchangeCount], m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, 20);
				m_pClientList[sOwnerH]->m_bIsExchangeMode = true;
				m_pClientList[sOwnerH]->exchangingWith_ = m_pClientList[iClientH]->shared_from_this();
				m_pClientList[iClientH]->iExchangeCount++;
				m_pClientList[iClientH]->SendNotifyMsg(iClientH, DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex + 1000, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute);
				m_pClientList[sOwnerH]->SendNotifyMsg(iClientH, DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
						  m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute);
			}
		}
	} else {
		_ClearExchangeStatus(iClientH);
	}
}

void CGame::SetExchangeItem(int iClientH, int iItemIndex, int iAmount) {
	int iExH;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
	if (m_pClientList[iClientH]->iExchangeCount > 4) return; //only 4 items trade
	//no admin trade
	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) {
		auto with = m_pClientList[iClientH]->exchangingWith_.lock();
		if (with) {
			_ClearExchangeStatus(with->id_);
		}
		_ClearExchangeStatus(iClientH);
	}
	if ((m_pClientList[iClientH]->m_bIsExchangeMode == true)) {
		auto with = m_pClientList[iClientH]->exchangingWith_.lock();
		if (with) {
			if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == nullptr) return;
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount < iAmount) return;
			//No Duplicate items
			for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++) {
				if (m_pClientList[iClientH]->m_cExchangeItemIndex[i] == (char) iItemIndex) {
					_ClearExchangeStatus(with->id_);
					_ClearExchangeStatus(iClientH);
					return;
				}
			}
			m_pClientList[iClientH]->m_cExchangeItemIndex[m_pClientList[iClientH]->iExchangeCount] = (char) iItemIndex;
			m_pClientList[iClientH]->m_iExchangeItemAmount[m_pClientList[iClientH]->iExchangeCount] = iAmount;
			//std::memset(m_pClientList[iClientH]->m_cExchangeItemName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName));
			memcpy(m_pClientList[iClientH]->m_cExchangeItemName[m_pClientList[iClientH]->iExchangeCount], m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, 20);
			//m_pClientList[iClientH]->m_cExchangeItemIndex  = iItemIndex;
			//m_pClientList[iClientH]->m_iExchangeItemAmount = iAmount;
			//std::memset(m_pClientList[iClientH]->m_cExchangeItemName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName));
			//memcpy(m_pClientList[iClientH]->m_cExchangeItemName, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, 20);
			m_pClientList[iClientH]->iExchangeCount++;
			m_pClientList[iClientH]->SendNotifyMsg(iClientH, DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex + 1000, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
			with->SendNotifyMsg(iClientH, DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
					  m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
		}
	}
}

void CGame::ConfirmExchangeItem(int iClientH) {
	int iItemWeightA, iItemWeightB, iWeightLeftA, iWeightLeftB, iAmountLeft;
	class CItem * pItemA[4], * pItemB[4], * pItemAcopy[4], * pItemBcopy[4];
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if ((m_pClientList[iClientH]->m_bIsExchangeMode == true)) {
		auto with = m_pClientList[iClientH]->exchangingWith_.lock();
		if (with) {
			if (iClientH == with->id_) return;
			if ((with->m_bIsExchangeMode != true) || !with->exchangingWith_.lock() || with->exchangingWith_.lock()->id_ != iClientH) {
				_ClearExchangeStatus(iClientH);
				_ClearExchangeStatus(with->id_);
				return;
			} else {
				m_pClientList[iClientH]->m_bIsExchangeConfirm = true;
				if (with->m_bIsExchangeConfirm == true) {
					//Check all items
					for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++) {
						if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]] == nullptr) ||
								  (memcmp(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName, m_pClientList[iClientH]->m_cExchangeItemName[i], 20) != 0)) {
							_ClearExchangeStatus(iClientH);
							_ClearExchangeStatus(with->id_);
							return;
						}
					}
					for (int i = 0; i < with->iExchangeCount; i++) {
						if ((with->m_pItemList[with->m_cExchangeItemIndex[i]] == nullptr) ||
								  (memcmp(with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cName, with->m_cExchangeItemName[i], 20) != 0)) {
							_ClearExchangeStatus(iClientH);
							_ClearExchangeStatus(with->id_);
							return;
						}
					}
					iWeightLeftA = m_pClientList[iClientH]->_iCalcMaxLoad() - m_pClientList[iClientH]->iCalcTotalWeight();
					iWeightLeftB = with->_iCalcMaxLoad() - with->iCalcTotalWeight();
					//Calculate weight for items
					iItemWeightA = 0;
					for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++) {
						iItemWeightA = iGetItemWeight(*m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]],
								  m_pClientList[iClientH]->m_iExchangeItemAmount[i]);
					}
					iItemWeightB = 0;
					for (int i = 0; i < with->iExchangeCount; i++) {
						iItemWeightB = iGetItemWeight(*with->m_pItemList[with->m_cExchangeItemIndex[i]],
								  with->m_iExchangeItemAmount[i]);
					}
					//See if the other person can take the item weightload
					if ((iWeightLeftA < iItemWeightB) || (iWeightLeftB < iItemWeightA)) {
						_ClearExchangeStatus(iClientH);
						_ClearExchangeStatus(with->id_);
						return;
					}
					for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++) {
						if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							if (m_pClientList[iClientH]->m_iExchangeItemAmount[i] > m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_dwCount) {
								_ClearExchangeStatus(iClientH);
								_ClearExchangeStatus(with->id_);
								return;
							}
							pItemA[i] = new class CItem;
							_bInitItemAttr(*pItemA[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
							pItemA[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
							pItemAcopy[i] = new class CItem;
							_bInitItemAttr(*pItemAcopy[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemAcopy[i], pItemA[i]);
							pItemAcopy[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
						} else {
							pItemA[i] = (class CItem *)&*m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]];
							pItemA[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
							pItemAcopy[i] = new class CItem;
							_bInitItemAttr(*pItemAcopy[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemAcopy[i], pItemA[i]);
							pItemAcopy[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
						}
					}
					for (int i = 0; i < with->iExchangeCount; i++) {
						if ((with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							if (with->m_iExchangeItemAmount[i] > with->m_pItemList[with->m_cExchangeItemIndex[i]]->m_dwCount) {
								_ClearExchangeStatus(iClientH);
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
						bAddItem(iClientH, pItemB[i], 0);
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
							with->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, with->m_cExchangeItemIndex[i], with->m_iExchangeItemAmount[i], 0, m_pClientList[iClientH]->m_cCharName);
							with->m_pItemList[with->m_cExchangeItemIndex[i]] = nullptr;
						}
					}
					for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++) {
						bAddItem(with->id_, pItemA[i], 0);
						_bItemLog(DEF_ITEMLOG_EXCHANGE, iClientH, with->id_, pItemAcopy[i]);
						delete pItemAcopy[i];
						pItemAcopy[i] = nullptr;
						if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
								  (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							//
							iAmountLeft = (int) m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_dwCount - m_pClientList[iClientH]->m_iExchangeItemAmount[i];
							if (iAmountLeft < 0) iAmountLeft = 0;
							// v1.41 !!!
							SetItemCount(iClientH, m_pClientList[iClientH]->m_cExchangeItemIndex[i], iAmountLeft);
							// m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex]->m_cName, iAmountLeft);
							//
						} else {
							m_pClientList[iClientH]->ReleaseItemHandler(m_pClientList[iClientH]->m_cExchangeItemIndex[i], true);
							m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, m_pClientList[iClientH]->m_cExchangeItemIndex[i], m_pClientList[iClientH]->m_iExchangeItemAmount[i], 0, with->m_cCharName);
							m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]] = nullptr;
						}
					}
					m_pClientList[iClientH]->m_bIsExchangeMode = false;
					m_pClientList[iClientH]->m_bIsExchangeConfirm = false;
					m_pClientList[iClientH]->exchangingWith_.reset();
					m_pClientList[iClientH]->iExchangeCount = 0;
					with->m_bIsExchangeMode = false;
					with->m_bIsExchangeConfirm = false;
					with->exchangingWith_.reset();
					with->iExchangeCount = 0;
					for (int i = 0; i < 4; i++) {
						m_pClientList[iClientH]->m_cExchangeItemIndex[i] = -1;
						with->m_cExchangeItemIndex[i] = -1;
					}
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, 0, 0, 0, nullptr);
					with->SendNotifyMsg(0, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, 0, 0, 0, nullptr);
					m_pClientList[iClientH]->iCalcTotalWeight();
					with->iCalcTotalWeight();
					return;
				}
			}
		}
	}
}

int CGame::_iGetItemSpaceLeft(int iClientH) {
	int i, iTotalItem;
	iTotalItem = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) iTotalItem++;
	return (DEF_MAXITEMS - iTotalItem);
}

bool CGame::bAddItem(int iClientH, CItem * pItem, char /*cMode*/) {
	char * cp, cData[256];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	int iEraseReq;
	std::memset(cData, 0, sizeof (cData));
	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
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
		m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
		return true;
	} else {
		m_pClientList[iClientH]->map_->bSetItem(m_pClientList[iClientH]->m_sX,
				  m_pClientList[iClientH]->m_sY,
				  pItem);
		m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
		m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
		return true;
	}
	return false;
}

int CGame::_iTalkToNpcResult_Cityhall(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange) {
	int iQuest, iEraseReq, iExp;
	class CItem * pItem;
	// Return Code
	if (m_pClientList[iClientH] == nullptr) return 0;
	/*if (((memcmp(m_pClientList[iClientH]->map_->m_cName, "cityhall_1", 10) != 1) ||
		(memcmp(m_pClientList[iClientH]->map_->m_cName, "cityhall_2", 10) != 1)) &&
		(memcmp(m_pClientList[i]->m_cLocation, "NONE",4) != 0)) {
			wsprintf(G_cTxt, "Traveller hack: (%s) Player: (%s) - is talking to William from a remote map.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			PutHackLogFileList(G_cTxt);
			m_pClientList[iClientH]->DeleteClient(true, true);
			return 0;
	}*/
	if (m_pClientList[iClientH]->m_iQuest != 0) {
		if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest] == nullptr) return -4;
		else if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iFrom == 4) {
			if (m_pClientList[iClientH]->m_bIsQuestCompleted == true) {
				if ((m_pClientList[iClientH]->m_iQuestRewardType > 0) &&
						  (m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType] != nullptr)) {
					pItem = new class CItem;
					_bInitItemAttr(*pItem, m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType]->m_cName);
					pItem->m_dwCount = m_pClientList[iClientH]->m_iQuestRewardAmount;
					if (_bCheckItemReceiveCondition(iClientH, pItem) == true) {
						_bAddClientItemList(iClientH, pItem, &iEraseReq);
						SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, 0);
						if (iEraseReq == 1) delete pItem;
						m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount,
								  m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType]->m_cName, m_pClientList[iClientH]->m_iContribution);
						m_pClientList[iClientH]->_ClearQuestStatus();
						return -5;
					} else {
						delete pItem;
						SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 0, m_pClientList[iClientH]->m_iQuestRewardAmount,
								  m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType]->m_cName, m_pClientList[iClientH]->m_iContribution);
						return -5;
					}
				} else if (m_pClientList[iClientH]->m_iQuestRewardType == -1) {
					m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iQuestRewardAmount;
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount,
							  "°æÇèÄ¡              ", m_pClientList[iClientH]->m_iContribution);
					m_pClientList[iClientH]->_ClearQuestStatus();
					return -5;
				} else if (m_pClientList[iClientH]->m_iQuestRewardType == -2) {
					//***
					iExp = iDice(1, (10 * m_pClientList[iClientH]->m_iLevel));
					//***
					iExp = iExp * m_pClientList[iClientH]->m_iQuestRewardAmount;
					m_pClientList[iClientH]->m_iExpStock += iExp;
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, iExp,
							  "°æÇèÄ¡              ", m_pClientList[iClientH]->m_iContribution);
					m_pClientList[iClientH]->_ClearQuestStatus();
					return -5;
				} else {
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTREWARD, 4, 1, 0,
							  "                     ", m_pClientList[iClientH]->m_iContribution);
					m_pClientList[iClientH]->_ClearQuestStatus();
					return -5;
				}
			} else return -1;
		}
		return -4;
	}
	if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->map_->m_cLocationName, 10) == 0) {
		if (m_pClientList[iClientH]->m_iPKCount > 0) return -3;
		iQuest = __iSearchForQuest(iClientH, 4, pQuestType, pMode, pRewardType, pRewardAmount, pContribution, pTargetName, pTargetType, pTargetCount, pX, pY, pRange);
		if (iQuest <= 0) return -4;
		return iQuest;
	} else return -2;
	return -4;
}

int CGame::_iTalkToNpcResult_Guard(int iClientH, int * /*pQuestType*/, int * /*pMode*/, int * /*pRewardType*/, int * /*pRewardAmount*/, int * /*pContribution*/, char * /*pTargetName*/, int * /*pTargetType*/, int * /*pTargetCount*/, int * /*pX*/, int * /*pY*/, int * /*pRange*/) {
	if (m_pClientList[iClientH] == nullptr) return 0;
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) {
		if (memcmp(m_pClientList[iClientH]->map_->m_cName, "aresden", 7) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (200), 0, 0, nullptr, 0);
			return 1000;
		} else
			if (memcmp(m_pClientList[iClientH]->map_->m_cName, "elv", 3) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (201), 0, 0, nullptr, 0);
			return 1001;
		}
	} else
		if (memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0) {
		if (memcmp(m_pClientList[iClientH]->map_->m_cName, "aresden", 7) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (202), 0, 0, nullptr, 0);
			return 1002;
		} else
			if (memcmp(m_pClientList[iClientH]->map_->m_cName, "elv", 3) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (203), 0, 0, nullptr, 0);
			return 1003;
		}
	} else
		if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) {
		if (memcmp(m_pClientList[iClientH]->map_->m_cName, "aresden", 7) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (204), 0, 0, nullptr, 0);
			return 1004;
		} else
			if (memcmp(m_pClientList[iClientH]->map_->m_cName, "elvine", 6) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (205), 0, 0, nullptr, 0);
			return 1005;
		} else
			if (memcmp(m_pClientList[iClientH]->map_->m_cName, "default", 7) == 0) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NPCTALK, (206), 0, 0, nullptr, 0);
			return 1006;
		}
	}
	return 0;
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

int CGame::__iSearchForQuest(int iClientH, int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange) {
	int i, iQuestList[DEF_MAXQUESTTYPE], iIndex, iQuest, iReward, iQuestIndex;
	if (m_pClientList[iClientH] == nullptr) return -1;
	iIndex = 0;
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		iQuestList[i] = -1;
	for (i = 1; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != nullptr) {
			if (m_pQuestConfigList[i]->m_iFrom != iWho) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_cSide != m_pClientList[iClientH]->m_cSide) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMinLevel > m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMaxLevel < m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iReqContribution > m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iRequiredSkillNum != -1) {
				if (m_pClientList[iClientH]->m_cSkillMastery[m_pQuestConfigList[i]->m_iRequiredSkillNum] <
						  m_pQuestConfigList[i]->m_iRequiredSkillLevel) goto SFQ_SKIP;
			}
			if ((m_bIsCrusadeMode == true) && (m_pQuestConfigList[i]->m_iAssignType != 1)) goto SFQ_SKIP;
			if ((m_bIsCrusadeMode == false) && (m_pQuestConfigList[i]->m_iAssignType == 1)) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iContributionLimit < m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;
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
// New 14/05/2004

void CGame::QuestAcceptedHandler(int iClientH) {
	int iIndex;
	if (m_pClientList[iClientH] == nullptr) return;
	// Does the quest exist ??
	if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest] == nullptr) return;
	if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iAssignType == 1) {
		switch (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iType) {
			case 10:
				m_pClientList[iClientH]->_ClearQuestStatus();
				m_pClientList[iClientH]->RequestTeleportHandler("2   ", m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_cTargetName,
						  m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sX, m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sY);
				return;
		}
	}
	m_pClientList[iClientH]->m_iQuest = m_pClientList[iClientH]->m_iAskedQuest;
	iIndex = m_pClientList[iClientH]->m_iQuest;
	m_pClientList[iClientH]->m_iQuestID = m_pQuestConfigList[iIndex]->m_iQuestID;
	m_pClientList[iClientH]->m_iCurQuestCount = 0;
	m_pClientList[iClientH]->m_bIsQuestCompleted = false;
	_CheckQuestEnvironment(iClientH);
	_SendQuestContents(iClientH);
}

void CGame::_SendQuestContents(int iClientH) {
	int iWho, iIndex, iQuestType, iContribution, iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted;
	char cTargetName[21];
	if (m_pClientList[iClientH] == nullptr) return;
	iIndex = m_pClientList[iClientH]->m_iQuest;
	if (iIndex == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTCONTENTS, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
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
		iQuestCompleted = (int) m_pClientList[iClientH]->m_bIsQuestCompleted;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTCONTENTS, iWho, iQuestType, iContribution, nullptr,
				  iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted, cTargetName);
	}
}

void CGame::_CheckQuestEnvironment(int iClientH) {
	int iIndex;
	char cTargetName[21];
	if (m_pClientList[iClientH] == nullptr) return;
	iIndex = m_pClientList[iClientH]->m_iQuest;
	if (iIndex == 0) return;
	if (m_pQuestConfigList[iIndex] == nullptr) return;
	if (iIndex >= 35 && iIndex <= 40) {
		m_pClientList[iClientH]->m_iQuest = 0;
		m_pClientList[iClientH]->m_iQuestID = 0;
		m_pClientList[iClientH]->m_iQuestRewardAmount = 0;
		m_pClientList[iClientH]->m_iQuestRewardType = 0;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, nullptr);
		return;
	}
	if (m_pQuestConfigList[iIndex]->m_iQuestID != m_pClientList[iClientH]->m_iQuestID) {
		m_pClientList[iClientH]->m_iQuest = 0;
		m_pClientList[iClientH]->m_iQuestID = 0;
		m_pClientList[iClientH]->m_iQuestRewardAmount = 0;
		m_pClientList[iClientH]->m_iQuestRewardType = 0;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, nullptr);
		return;
	}
	switch (m_pQuestConfigList[iIndex]->m_iType) {
		case DEF_QUESTTYPE_MONSTERHUNT:
		case DEF_QUESTTYPE_GOPLACE:
			std::memset(cTargetName, 0, sizeof (cTargetName));
			memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
			if (memcmp(m_pClientList[iClientH]->map_->m_cName, cTargetName, 10) == 0)
				m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = true;
			else m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = false;
			break;
	}
}

bool CGame::_bCheckIsQuestCompleted(int iClientH) {
	int iQuestIndex;
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_bIsQuestCompleted == true) return false;
	iQuestIndex = m_pClientList[iClientH]->m_iQuest;
	if (iQuestIndex == 0) return false;
	if (m_pQuestConfigList[iQuestIndex] != nullptr) {
		switch (m_pQuestConfigList[iQuestIndex]->m_iType) {
			case DEF_QUESTTYPE_MONSTERHUNT:
				if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == true) &&
						  (m_pClientList[iClientH]->m_iCurQuestCount >= m_pQuestConfigList[iQuestIndex]->m_iMaxCount)) {
					m_pClientList[iClientH]->m_bIsQuestCompleted = true;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOMPLETED, 0, 0, 0, nullptr);
					return true;
				}
				break;
			case DEF_QUESTTYPE_GOPLACE:
				if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == true) &&
						  (m_pClientList[iClientH]->m_sX >= m_pQuestConfigList[iQuestIndex]->m_sX - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						  (m_pClientList[iClientH]->m_sX <= m_pQuestConfigList[iQuestIndex]->m_sX + m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						  (m_pClientList[iClientH]->m_sY >= m_pQuestConfigList[iQuestIndex]->m_sY - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
						  (m_pClientList[iClientH]->m_sY <= m_pQuestConfigList[iQuestIndex]->m_sY + m_pQuestConfigList[iQuestIndex]->m_iRange)) {
					m_pClientList[iClientH]->m_bIsQuestCompleted = true;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOMPLETED, 0, 0, 0, nullptr);
					return true;
				}
				break;
		}
	}
	return false;
}

void CGame::SendItemNotifyMsg(int iClientH, uint16_t wMsgType, CItem *pItem, int iV1) {
	char * cp, cData[512];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp;
	if (m_pClientList[iClientH] == nullptr) return;
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
			m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
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
			m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 48);
			break;
		case DEF_NOTIFY_CANNOTCARRYMOREITEM:
			m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
			break;
	}
}

bool CGame::_bCheckItemReceiveCondition(int iClientH, CItem *pItem) {
	int i;
	if (m_pClientList[iClientH] == nullptr) return false;
	/*
	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {
		
		if ((m_pClientList[iClientH]->m_iCurWeightLoad + (pItem->m_wWeight * pItem->m_dwCount)) > (uint32_t)m_pClientList[iClientH]->_iCalcMaxLoad())
			return false;
	}
	else {
		
		if ((m_pClientList[iClientH]->m_iCurWeightLoad + pItem->m_wWeight) > m_pClientList[iClientH]->_iCalcMaxLoad())
			return false;
	}
	 */
	if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(*pItem, pItem->m_dwCount) > (uint32_t) m_pClientList[iClientH]->_iCalcMaxLoad())
		return false;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] == nullptr) return true;
	return false;
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
	while (bLoopFlag == false) {
		if ((m_pSubLogSock[m_iCurSubLogSockIndex] != nullptr) && (m_bIsSubLogSockAvailable[m_iCurSubLogSockIndex] == true))
			bLoopFlag = true;
		else m_iCurSubLogSockIndex++;
		iCnt++;
		if (iCnt >= DEF_MAXSUBLOGSOCK) {
			if (m_bOnExitProcess == false) {
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
	int i, iIndex = 0;
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

void CGame::BuildItemHandler(int iClientH, char *pData) {
	char * cp, cName[21], cElementItemID[6], cColor;
	int i, x, z, iMatch, iCount, iPlayerSkillLevel, iResult, iTotalValue, iResultValue, iTemp, iItemCount[DEF_MAXITEMS];
	class CItem * pItem;
	bool bFlag, bItemFlag[6];
	double dV1, dV2, dV3;
	uint32_t dwTemp, dwTemp2, dwType, dwValue;
	uint16_t wTemp;
	if (m_pClientList[iClientH] == nullptr) return;
	m_pClientList[iClientH]->m_iSkillMsgRecvCount++;
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
	while (bFlag == true) {
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
	iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[13];
	iResult = iDice(1, 100);
	if (iResult > iPlayerSkillLevel) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, nullptr);
		return;
	}
	for (i = 0; i < 6; i++)
		if (cElementItemID[i] != -1) {
			if ((cElementItemID[i] < 0) || (cElementItemID[i] > DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[cElementItemID[i]] == nullptr) return;
		}
	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		if (m_pBuildItemList[i] != nullptr) {
			if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0) {
				if (m_pBuildItemList[i]->m_iSkillLimit > m_pClientList[iClientH]->m_cSkillMastery[13]) return;
				for (x = 0; x < DEF_MAXITEMS; x++)
					if (m_pClientList[iClientH]->m_pItemList[x] != nullptr)
						iItemCount[x] = m_pClientList[iClientH]->m_pItemList[x]->m_dwCount;
					else iItemCount[x] = 0;
				iMatch = 0;
				iTotalValue = 0;
				for (x = 0; x < 6; x++) {
					if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0) {
						iMatch++;
					} else {
						for (z = 0; z < 6; z++)
							if ((cElementItemID[z] != -1) && (bItemFlag[z] == false)) {
								if ((m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sIDnum == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
										  (m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
										  (iItemCount[cElementItemID[z]] > 0)) {
									iTemp = m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sItemSpecEffectValue2;
									if (iTemp > m_pClientList[iClientH]->m_cSkillMastery[13]) {
										iTemp = iTemp - (iTemp - m_pClientList[iClientH]->m_cSkillMastery[13]) / 2;
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
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, nullptr);
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
				bAddItem(iClientH, pItem, 0);
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_BUILDITEMSUCCESS, pItem->m_sItemSpecEffectValue2, pItem->m_cItemType, 0, nullptr);
#ifdef DEF_TAIWANLOG
				_bItemLog(DEF_ITEMLOG_MAKE, iClientH, (int) - 1, pItem);
#endif
				for (x = 0; x < 6; x++)
					if (cElementItemID[x] != -1) {
						if (m_pClientList[iClientH]->m_pItemList[cElementItemID[x]] == nullptr) {
							// ### BUG POINT!!!
							wsprintf(G_cTxt, "(?) Char(%s) ElementItemID(%d)", m_pClientList[iClientH]->m_cCharName, cElementItemID[x]);
							PutLogFileList(G_cTxt);
						} else {
							iCount = m_pClientList[iClientH]->m_pItemList[cElementItemID[x]]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
							if (iCount < 0) iCount = 0;
							SetItemCount(iClientH, cElementItemID[x], iCount);
						}
					}
				if (m_pBuildItemList[i]->m_iMaxSkill > m_pClientList[iClientH]->m_cSkillMastery[13])
					m_pClientList[iClientH]->CalculateSSN_SkillIndex(13, 1);
				GetExp(iClientH, iDice(1, (m_pBuildItemList[i]->m_iSkillLimit / 4))); //m_pClientList[iClientH]->m_iExpStock += iDice(1, (m_pBuildItemList[i]->m_iSkillLimit/4));
				return;
			}
		}
	/*
	
	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
	if (m_pBuildItemList[i] != nullptr) {
		if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0) {
			if (m_pBuildItemList[i]->m_iSkillLimit > m_pClientList[iClientH]->m_cSkillMastery[13]) return;
            
			iMatch = 0;
			for (x = 0; x < 6; x++) m_pBuildItemList[i]->m_iIndex[x] = -1;
			for (x = 0; x < DEF_MAXITEMS; x++) bItemFlag[x] = false;
			iTotalValue = 0;
			for (x = 0; x < 6; x++)
			if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0) iMatch++;
			else {
				bFlag = false;
				for (z = 0; z < DEF_MAXITEMS; z++)
				if ((m_pClientList[iClientH]->m_pItemList[z] != nullptr)) {
					if ((m_pClientList[iClientH]->m_pItemList[z]->m_sIDnum  == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
						(m_pClientList[iClientH]->m_pItemList[z]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
						(bItemFlag[z] == false)) {
                  
						m_pBuildItemList[i]->m_iIndex[x] = z;
						bItemFlag[z] = true;
						iTotalValue += (m_pClientList[iClientH]->m_pItemList[z]->m_sItemSpecEffectValue1 * m_pBuildItemList[i]->m_iMaterialItemValue[x]);
                  
						bFlag = true;
						goto BIH_LOOPBREAK;
					}
				}
            
BIH_LOOPBREAK:;
				if (bFlag == true) iMatch++;
			}
         
			if (iMatch != 6) {
				m_pClientList[iClientH]->SendNotifyMsg(0,DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, nullptr);
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
				iCount = m_pClientList[iClientH]->m_pItemList[iIndex]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
				if (iCount < 0) iCount = 0;
				SetItemCount(iClientH, iIndex, iCount);
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
			bAddItem(iClientH, pItem, nullptr);
			m_pClientList[iClientH]->SendNotifyMsg(0,DEF_NOTIFY_BUILDITEMSUCCESS, 0, 0, 0, nullptr);
		}
	}
	 */
}

void CGame::AdminOrder_SetAttackMode(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	/*
	
	if ((memcmp(m_pClientList[iClientH]->map_->m_cName, "fightzone", 9) == 0) && (m_pClientList[iClientH]->m_iSkillSSN[59] == 1)) {
		goto AOCC_SAM;
	}
	 */
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSetAttackMode) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			m_pClientList[iClientH]->map_->m_bIsAttackEnabled = true;
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) && (m_pClientList[i]->map_ == m_pClientList[iClientH]->map_))
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_GLOBALATTACKMODE, (int) m_pClientList[iClientH]->map_->m_bIsAttackEnabled, 0, 0, nullptr);
		} else if (token[0] == '0') {
			m_pClientList[iClientH]->map_->m_bIsAttackEnabled = false;
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true) && (m_pClientList[i]->map_ == m_pClientList[iClientH]->map_))
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_GLOBALATTACKMODE, (int) m_pClientList[iClientH]->map_->m_bIsAttackEnabled, 0, 0, nullptr);
		}
	}
	delete pStrTok;
}

void CGame::AdminOrder_UnsummonAll(int iClientH) {
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelUnsummonAll) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	for (i = 1; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {
			if ((m_pNpcList[i]->m_bIsSummoned == true) && (m_pNpcList[i]->m_bIsKilled == false))
				NpcKilledHandler(iClientH, DEF_OWNERTYPE_PLAYER, i, 0);
		}
}

void CGame::AdminOrder_UnsummonDemon(int iClientH) {
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelUnsummonDemon) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	for (i = 1; i < DEF_MAXNPCS; i++)
		if (m_pNpcList[i] != nullptr) {
			if ((m_pNpcList[i]->m_sType == 31) && (m_pNpcList[i]->m_bIsKilled == false))
				NpcKilledHandler(iClientH, DEF_OWNERTYPE_PLAYER, i, 0);
		}
}
// 3.51 - 05/17/2004 - Hypnotoad/[KLKS] - Monster Special Abilities

char CGame::_cGetSpecialAbility(int iKindSA) {
	char cSA;
	switch (iKindSA) {
		case 1:
			// Slime, Orc, Orge, WereWolf, YB-, Rabbit, Mountain-Giant, Stalker, Hellclaw,
			// Wyvern, Fire-Wyvern, Barlog, Tentocle, Centaurus, Giant-Lizard, Minotaurus,
			// Abaddon, Claw-Turtle, Giant-Cray-Fish, Giant-Plant, MasterMage-Orc, Nizie,
			// Tigerworm
			switch (iDice(1, 2)) {
				case 1: cSA = 3;
					break; // Anti-Physical Damage
				case 2: cSA = 4;
					break; // Anti-Magic Damage
			}
			break;
		case 2:
			// Giant-Ant, Cat, Giant-Frog,
			switch (iDice(1, 3)) {
				case 1: cSA = 3;
					break; // Anti-Physical Damage
				case 2: cSA = 4;
					break; // Anti-Magic Damage
				case 3: cSA = 5;
					break; // Poisonous
			}
			break;
		case 3:
			// Zombie, Scorpion, Amphis, Troll, Dark-Elf
			switch (iDice(1, 4)) {
				case 1: cSA = 3;
					break; // Anti-Physical Damage
				case 2: cSA = 4;
					break; // Anti-Magic Damage
				case 3: cSA = 5;
					break; // Poisonous
				case 4: cSA = 6;
					break; // Critical Poisonous
			}
			break;
		case 4:
			// no linked Npc
			switch (iDice(1, 3)) {
				case 1: cSA = 3;
					break; // Anti-Physical Damage
				case 2: cSA = 4;
					break; // Anti-Magic Damage
				case 3: cSA = 7;
					break; // Explosive
			}
			break;
		case 5:
			// Stone-Golem, Clay-Golem, Beholder, Cannibal-Plant, Rudolph, DireBoar
			switch (iDice(1, 4)) {
				case 1: cSA = 3;
					break; // Anti-Physical Damage
				case 2: cSA = 4;
					break; // Anti-Magic Damage
				case 3: cSA = 7;
					break; // Explosive
				case 4: cSA = 8;
					break; // Critical-Explosive
			}
			break;
		case 6:
			// no linked Npc
			switch (iDice(1, 3)) {
				case 1: cSA = 3;
					break; // Anti-Physical Damage
				case 2: cSA = 4;
					break; // Anti-Magic Damage
				case 3: cSA = 5;
					break; // Poisonous
			}
			break;
		case 7:
			// Orc-Mage, Unicorn
			switch (iDice(1, 3)) {
				case 1: cSA = 1;
					break; // Clairvoyant
				case 2: cSA = 2;
					break; // Distruction of Magic Protection
				case 3: cSA = 4;
					break; // Anti-Magic Damage
			}
			break;
		case 8:
			// Frost, Ice-Golem, Ettin, Gagoyle, Demon, Liche, Hellbound, Cyclops,
			// Skeleton
			switch (iDice(1, 5)) {
				case 1: cSA = 1;
					break; // Clairvoyant
				case 2: cSA = 2;
					break; // Distruction of Magic Protection
				case 3: cSA = 4;
					break; // Anti-Magic Damage
				case 4: cSA = 3;
					break; // Anti-Physical Damage
				case 5: cSA = 8;
					break; // Critical-Explosive
			}
			break;
		case 9:
			// no linked Npc
			cSA = iDice(1, 8); // All abilities available
			break;
	}
	return cSA;
}

void CGame::AdminOrder_Summon(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cName_Master[10], cName_Slave[10], cNpcName[256], cWaypoint[11], cSA;
	int pX, pY, j, iNum, iNamingValue;
	class CStrTok * pStrTok;
	bool bMaster;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSummon) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
	pX = m_pClientList[iClientH]->m_sX;
	pY = m_pClientList[iClientH]->m_sY;
	wsprintf(G_cTxt, "(!) Admin Order: Summon(%s)-(%d)", cNpcName, iNum);
	PutLogList(G_cTxt);
	iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
	if (iNamingValue != -1) {
		std::memset(cName_Master, 0, sizeof (cName_Master));
		wsprintf(cName_Master, "XX%d", iNamingValue);
		cName_Master[0] = '_';
		cName_Master[1] = m_pClientList[iClientH]->map_->id_ + 65;
		if ((bMaster = bCreateNewNpc(cNpcName, cName_Master, m_pClientList[iClientH]->map_->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false, true)) == false) {
			m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
		}
	}
	for (j = 0; j < (iNum - 1); j++) {
		iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
		if (iNamingValue != -1) {
			std::memset(cName_Slave, 0, sizeof (cName_Slave));
			wsprintf(cName_Slave, "XX%d", iNamingValue);
			cName_Slave[0] = '_';
			cName_Slave[1] = m_pClientList[iClientH]->map_->id_ + 65;
			if (bCreateNewNpc(cNpcName, cName_Slave, m_pClientList[iClientH]->map_->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false) == false) {
				m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				bSetNpcFollowMode(cName_Slave, cName_Master, DEF_OWNERTYPE_NPC);
			}
		}
	}
	delete pStrTok;
}

void CGame::AdminOrder_SummonAll(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cLocation[11], cMapName[11];
	int pX, pY, i;
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSummonAll) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
	pX = m_pClientList[iClientH]->m_sX;
	pY = m_pClientList[iClientH]->m_sY;
	memcpy(cMapName, m_pClientList[iClientH]->map_->m_cName, 11);
	if (strlen(token) > 10)
		memcpy(cLocation, token, 10);
	else memcpy(cLocation, token, strlen(token));
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cLocation, token) == 0)) {
			m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, pX, pY);
		}
	wsprintf(G_cTxt, "GM Order(%s): PC(%s) Summoned to (%s)", m_pClientList[iClientH]->m_cLocation,
			  cLocation, cMapName);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	delete pStrTok;
}

void CGame::AdminOrder_SummonPlayer(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cMapName[11], cName[11];
	int pX, pY, i;
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSummonPlayer) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
	pX = m_pClientList[iClientH]->m_sX;
	pY = m_pClientList[iClientH]->m_sY;
	memcpy(cMapName, m_pClientList[iClientH]->map_->m_cName, 11);
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
	wsprintf(G_cTxt, "GM Order(%s): PC(%s) Summoned to (%s)", m_pClientList[iClientH]->m_cCharName,
			  cName, cMapName);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	delete pStrTok;
}

void CGame::CheckSpecialEvent(int iClientH) {
	class CItem * pItem;
	char cItemName[21];
	int iEraseReq;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iSpecialEventID == 200081) {
		if (m_pClientList[iClientH]->m_iLevel < 11) {
			m_pClientList[iClientH]->m_iSpecialEventID = 0;
			return;
		}
		std::memset(cItemName, 0, sizeof (cItemName));
		strcpy(cItemName, "MemorialRing");
		pItem = new class CItem;
		if (_bInitItemAttr(*pItem, cItemName) == false) {
			delete pItem;
		} else {
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;
				wsprintf(G_cTxt, "(*) Get MemorialRing  : Char(%s)", m_pClientList[iClientH]->m_cCharName);
				PutLogFileList(G_cTxt);
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				pItem->m_cItemColor = 9;
				m_pClientList[iClientH]->m_iSpecialEventID = 0;
			}
		}
	}
}

void CGame::AdminOrder_DisconnectAll(int iClientH, char */*pData*/, uint32_t dwMsgSize) {
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelDisconnectAll) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	_iForcePlayerDisconect(DEF_MAXCLIENTS);
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
							m_pDupItemIDList[iIndex]->m_sTouchEffectType = atoi(token);
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
	uint32_t dwSWEType, dwSWEValue;
	double dV1, dV2, dV3;
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

void CGame::RequestNoticementHandler(CClient &client, char * pData) {
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
		client.m_pXSock->iSendMsg(cp, m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2);
		delete cp;
	} else {
		char cData[120];
		std::memset(cData, 0, sizeof (cData));
		uint32_t *dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_NOTICEMENT;
		uint16_t *wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_MSGTYPE_CONFIRM;
		client.m_pXSock->iSendMsg(cData, 6);
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

void CGame::_TamingHandler(int iClientH, int iSkillNum, MapPtr map, int dX, int dY) {
	int iSkillLevel, iRange, iTamingLevel, iResult, iX, iY;
	short sOwnerH;
	char cOwnerType;
	if (m_pClientList[iClientH] == nullptr) return;
	if (!map) return;
	iSkillLevel = (int) m_pClientList[iClientH]->m_cSkillMastery[iSkillNum];
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

void CGame::GetMagicAbilityHandler(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_cSkillMastery[4] != 0) return;
	m_pClientList[iClientH]->m_cSkillMastery[4] = 20;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILL, 4, m_pClientList[iClientH]->m_cSkillMastery[4], 0, nullptr);
	bCheckTotalSkillMasteryPoints(iClientH, 4);
}

int CGame::iRequestPanningMapDataRequest(CClient &client, char * pData) {
	char * cp, cDir, cData[3000];
	uint32_t * dwp;
	uint16_t * wp;
	short * sp, dX, dY;
	int iRet, iSize;
	if (client.m_bIsObserverMode == false) return 0;
	if (client.m_bIsKilled == true) return 0;
	if (client.m_bIsInitComplete == false) return 0;
	dX = client.m_sX;
	dY = client.m_sY;
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
	client.m_sX = dX;
	client.m_sY = dY;
	client.m_cDir = cDir;
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
	iSize = iComposeMoveMapData((short) (dX - 10), (short) (dY - 7), client.id_, cDir, cp);
	iRet = client.m_pXSock->iSendMsg(cData, iSize + 12 + 1 + 4);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[client.id_]->DeleteClient(true, true);
			return 0;
	}
	return 1;
}

void CGame::AdminOrder_SetObserverMode(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsObserverMode == true) {
		m_pClientList[iClientH]->map_->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_OBSERVERMODE, 0, 0, 0, nullptr);
		m_pClientList[iClientH]->m_bIsObserverMode = false;
	} else {
		m_pClientList[iClientH]->map_->ClearOwner(1, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
		//iRequestPanningMapDataRequest(iClientH, pData)
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_OBSERVERMODE, 1, 0, 0, nullptr);
		m_pClientList[iClientH]->m_bIsObserverMode = true;
	}
}

void CGame::AdminOrder_EnableAdminCommand(int iClientH, char *pData, uint32_t dwMsgSize) {
	char * token, cBuff[256], len;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
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
			m_pClientList[iClientH]->m_bIsAdminCommandEnabled = true;
		} else {
			wsprintf(G_cTxt, "(%s) Player(%s) attempts to access /enableadmincommand with %s", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, token);
			PutHackLogFileList(G_cTxt);
			return;
		}
	}
}

void CGame::AdminOrder_CreateItem(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * cp, * token, cBuff[256], cItemName[256], cData[256], cTemp[256], cAttribute[256], cValue[256];
	SYSTEMTIME SysTime;
	class CStrTok * pStrTok;
	class CItem * pItem;
	short * sp;
	int iTemp, iEraseReq, iValue;
	uint32_t * dwp;
	uint16_t * wp, wTemp;
	double dV1, dV2, dV3;
	// if the client doesnt exist than no effect.
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	// if the command is just "/createitem " than no effect.
	if ((dwMsgSize) <= 0) return;
	// if client's admin level is less than 4 no effect.
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelCreateItem) {
		// if the admin user level is not 0 and less than 4 send message
		if (m_pClientList[iClientH]->m_iAdminUserLevel != 0)
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	} // close if (m_pClientList[iClientH]->m_iAdminUserLevel < 4) {
	// IP address not able to create item
	// DISABLED: if (memcmp(m_pClientList[iClientH]->m_cIPaddress, "192.168.1.", 12) != 0) return;
	// check if the client has typed "/enableadmincreateitem"
	//if (m_pClientList[iClientH]->m_bIsAdminCreateItemEnabled == false) return;
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
	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
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
		m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
		// v2.14 Admin Log
		wsprintf(G_cTxt, "(%s) GM Order(%s): Create ItemName(%s)", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, cItemName);
		bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, 0, G_cTxt);
		return;
	} else {
		delete pItem;
		return;
	}
}

void CGame::RequestSellItemListHandler(CClient &client, char * pData) {
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
	int id = client.id_;
	for (i = 0; i < 12; i++) {
		cIndex = stTemp[i].cIndex;
		iAmount = stTemp[i].iAmount;
		if ((cIndex == -1) || (cIndex < 0) || (cIndex >= DEF_MAXITEMS)) return;
		if (client.m_pItemList[cIndex] == nullptr) return;
		ReqSellItemConfirmHandler(id, cIndex, iAmount, nullptr);
		if (m_pClientList[id] == nullptr) return;
	}
}

void CGame::CreateNewPartyHandler(int iClientH) {
	bool bFlag;
	if (m_pClientList[iClientH] == nullptr) return;
	bFlag = m_pClientList[iClientH]->bCreateNewParty();
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_RESPONSE_CREATENEWPARTY, (int) bFlag, 0, 0, nullptr);
}

void CGame::JoinPartyHandler(int iClientH, int iV1, char *pMemberName) {
	short sAppr2;
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	switch (iV1) {
		case 0:
			RequestDeletePartyHandler(iClientH);
			break;
		case 1:
			//testcode
			wsprintf(G_cTxt, "Join Party Req: %s(%d) ID(%d) Stat(%d) ReqJoinH(%d) ReqJoinName(%s)", m_pClientList[iClientH]->m_cCharName, iClientH,
					  m_pClientList[iClientH]->m_iPartyID, m_pClientList[iClientH]->m_iPartyStatus, m_pClientList[iClientH]->m_iReqJoinPartyClientH,
					  m_pClientList[iClientH]->m_cReqJoinPartyName);
			PutLogList(G_cTxt);
			if ((m_pClientList[iClientH]->m_iPartyID != 0) || (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL)) {
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
				m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
				std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
				m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				//testcode
				PutLogList("Join Party Reject (1)");
				return;
			}
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cCharName, pMemberName) == 0)) {
					sAppr2 = (short) ((m_pClientList[i]->m_sAppr2 & 0xF000) >> 12);
					if (sAppr2 != 0) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
						//testcode
						PutLogList("Join Party Reject (2)");
					} else if (m_pClientList[i]->m_cSide != m_pClientList[iClientH]->m_cSide) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
						//testcode
						PutLogList("Join Party Reject (3)");
					} else if (m_pClientList[i]->m_iPartyStatus == DEF_PARTYSTATUS_PROCESSING) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
						//testcode
						PutLogList("Join Party Reject (4)");
						//testcode
						wsprintf(G_cTxt, "Party join reject(2) ClientH:%d ID:%d JoinName:%d", i, m_pClientList[i]->m_iPartyID, m_pClientList[i]->m_cReqJoinPartyName);
						PutLogList(G_cTxt);
						m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
						std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
						m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
					} else {
						m_pClientList[i]->m_iReqJoinPartyClientH = iClientH;
						std::memset(m_pClientList[i]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[i]->m_cReqJoinPartyName));
						strcpy(m_pClientList[i]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName);
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_QUERY_JOINPARTY, 0, 0, 0, m_pClientList[iClientH]->m_cCharName);
						m_pClientList[iClientH]->m_iReqJoinPartyClientH = i;
						std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
						strcpy(m_pClientList[iClientH]->m_cReqJoinPartyName, m_pClientList[i]->m_cCharName);
						m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
					}
					return;
				}
			break;
		case 2:
			if (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) {
			}
			break;
	}
}

bool CGame::bCheckEnergySphereDestination(int iNpcH, short sAttackerH, char cAttackerType) {
	int i, sX, sY, dX, dY;
	char cResult;
	if (m_pNpcList[iNpcH] == nullptr) return false;
	if (m_pNpcList[iNpcH]->map_->m_iCurEnergySphereGoalPointIndex == -1) return false;
	if (m_pNpcList[iNpcH]->map_ != middlelandMap_.lock()) {
		MapPtr goalMap = m_pNpcList[iNpcH]->map_;
		sX = m_pNpcList[iNpcH]->m_sX;
		sY = m_pNpcList[iNpcH]->m_sY;
		cResult = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].cResult;
		dX = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].aresdenX;
		dY = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].aresdenY;
		if ((sX >= dX - 2) && (sX <= dX + 2) && (sY >= dY - 2) && (sY <= dY + 2)) {
			goalMap->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
				if (m_pClientList[sAttackerH]->m_cSide == 1) { // Aresden (Side:1)
					m_pClientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Aresden Player (%s)", m_pClientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					m_pClientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 2, m_pClientList[sAttackerH]->m_cCharName);
					}
			}
			return true;
		}
		dX = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].elvineX;
		dY = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].elvineY;
		if ((sX >= dX - 2) && (sX <= dX + 2) && (sY >= dY - 2) && (sY <= dY + 2)) {
			goalMap->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
				if (m_pClientList[sAttackerH]->m_cSide == 2) { // Elvine (Side:2)
					m_pClientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Elvine Player (%s)", m_pClientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					m_pClientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						//
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 1, m_pClientList[sAttackerH]->m_cCharName);
					}
			}
		}
		return false;
	} else {
		sX = m_pNpcList[iNpcH]->m_sX;
		sY = m_pNpcList[iNpcH]->m_sY;
		auto ml = middlelandMap_.lock();
		cResult = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].cResult;
		dX = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].aresdenX;
		dY = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].aresdenY;
		if ((sX >= dX - 4) && (sX <= dX + 4) && (sY >= dY - 4) && (sY <= dY + 4)) {
			ml->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
				if (m_pClientList[sAttackerH]->m_cSide == 1) { // Aresden (Side:1)
					m_pClientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Aresden Player (%s)", m_pClientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					m_pClientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 2, m_pClientList[sAttackerH]->m_cCharName);
					}
			}
			return true;
		}
		dX = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].elvineX;
		dY = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].elvineY;
		if ((sX >= dX - 4) && (sX <= dX + 4) && (sY >= dY - 4) && (sY <= dY + 4)) {
			ml->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != nullptr)) {
				if (m_pClientList[sAttackerH]->m_cSide == 2) { // Elvine (Side:2)
					m_pClientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Aresden Player (%s)", m_pClientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					m_pClientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
						//
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 1, m_pClientList[sAttackerH]->m_cCharName);
					}
			}
			return true;
		}
		return false;
	}
}

void CGame::EnergySphereProcessor(bool bIsAdminCreate, int iClientH) {
	int i, iNamingValue, iCIndex, iTemp, pX, pY;
	char cSA, cName_Internal[31], cWaypoint[31];
	auto ml = middlelandMap_.lock();
	if (bIsAdminCreate != true) {
		if (ml == nullptr) return;
		if (iDice(1, 2000) != 123) return;
		if (m_iTotalClients < 500) return;
		if (ml->m_iCurEnergySphereGoalPointIndex >= 0) return;
		iCIndex = iDice(1, ml->m_iTotalEnergySphereCreationPoint);
		if (ml->m_stEnergySphereCreationList[iCIndex].cType == 0) return;
		cSA = 0;
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
			if ((bCreateNewNpc("Energy-Sphere", cName_Internal, ml->m_cName, (rand() % 5), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false)) == false) {
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
		if (m_pClientList[iClientH]->map_->m_iCurEnergySphereGoalPointIndex >= 0) return;
		iCIndex = iDice(1, m_pClientList[iClientH]->map_->m_iTotalEnergySphereCreationPoint);
		if (m_pClientList[iClientH]->map_->m_stEnergySphereCreationList[iCIndex].cType == 0) return;
		cSA = 0;
		pX = m_pClientList[iClientH]->map_->m_stEnergySphereCreationList[iCIndex].sX;
		pY = m_pClientList[iClientH]->map_->m_stEnergySphereCreationList[iCIndex].sY;
		std::memset(cWaypoint, 0, sizeof (cWaypoint));
		iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
		if (iNamingValue != -1) {
			//
			std::memset(cName_Internal, 0, sizeof (cName_Internal));
			wsprintf(cName_Internal, "XX%d", iNamingValue);
			cName_Internal[0] = '_';
			cName_Internal[1] = m_pClientList[iClientH]->map_->id_ + 65;
			if ((bCreateNewNpc("Energy-Sphere", cName_Internal, m_pClientList[iClientH]->map_->m_cName, (rand() % 5), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false)) == false) {
				m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
				return;
			}
		}
		iTemp = iDice(1, m_pClientList[iClientH]->map_->m_iTotalEnergySphereGoalPoint);
		if (m_pClientList[iClientH]->map_->m_stEnergySphereGoalList[iTemp].cResult == 0) return;
		m_pClientList[iClientH]->map_->m_iCurEnergySphereGoalPointIndex = iTemp;
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

void CGame::ActivateSpecialAbilityHandler(int iClientH) {
	uint32_t dwTime = timeGetTime();
	short sTemp;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iSpecialAbilityTime != 0) return;
	if (m_pClientList[iClientH]->m_iSpecialAbilityType == 0) return;
	if (m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == true) return;
	m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = true;
	m_pClientList[iClientH]->m_dwSpecialAbilityStartTime = dwTime;
	m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
	sTemp = m_pClientList[iClientH]->m_sAppr4;
	sTemp = sTemp & 0xFF0F;
	switch (m_pClientList[iClientH]->m_iSpecialAbilityType) {
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
	m_pClientList[iClientH]->m_sAppr4 = sTemp;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityLastSec, nullptr);
	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CGame::CancelQuestHandler(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	m_pClientList[iClientH]->_ClearQuestStatus();
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, nullptr);
}

int CGame::iGetItemWeight(CItem &item, int iCount) {
	int iWeight = (item.m_wWeight);
	if (iCount < 0) iCount = 1;
	iWeight = iWeight * iCount;
	if (item.m_sIDnum == 90) iWeight = iWeight / 20;
	if (iWeight <= 0) iWeight = 1;
	return iWeight;
}

void CGame::UpdateMapSectorInfo() {
	int i, ix, iy;
	int iMaxNeutralActivity, iMaxAresdenActivity, iMaxElvineActivity, iMaxMonsterActivity, iMaxPlayerActivity;
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
	int i, ix, iy;
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

bool CGame::__bSetConstructionKit(int dX, int dY, int iType, int /*iTimeCost*/, int iClientH) {
	int iNamingValue, ix, iy, tX, tY;
	char cNpcName[21], cName[21], cNpcWaypoint[11], cOwnerType;
	short sOwnerH;
	if ((m_bIsCrusadeMode == false) || (m_pClientList[iClientH]->m_iCrusadeDuty != 2)) return false;
	if (m_pClientList[iClientH]->map_->m_iTotalCrusadeStructures >= DEF_MAXCRUSADESTRUCTURES) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOMORECRUSADESTRUCTURE, 0, 0, 0, nullptr);
		return false;
	}
	iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		for (ix = dX - 3; ix <= dX + 5; ix++)
			for (iy = dY - 3; iy <= dX + 5; iy++) {
				m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
				if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sOwnerH]->m_cActionLimit == 5)) return false;
			}
		std::memset(cNpcName, 0, sizeof (cNpcName));
		if (m_pClientList[iClientH]->m_cSide == 1) {
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
		} else if (m_pClientList[iClientH]->m_cSide == 2) {
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
		cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
		std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
		tX = (int) dX;
		tY = (int) dY;
		if (bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, (rand() % 9), DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
			m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
		} else {
			wsprintf(G_cTxt, "Structure(%s) construction begin(%d,%d)!", cNpcName, tX, tY);
			PutLogList(G_cTxt);
			return true;
		}
	}
	return false;
}

void CGame::LocalStartCrusadeMode(uint32_t dwCrusadeGUID) {
	int i;
	if (m_bIsCrusadeMode == true) return;
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
	_bCrusadeLog(DEF_CRUSADELOG_STARTCRUSADE, 0, 0, nullptr);
}

void CGame::LocalEndCrusadeMode(int iWinnerSide) {
	int i;
	wsprintf(G_cTxt, "LocalEndCrusadeMode(%d)", iWinnerSide);
	PutLogList(G_cTxt);
	if (m_bIsCrusadeMode == false) return;
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
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, false, false, "Elvine Win!");
	} else if (iWinnerSide == 1) {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, false, false, "Aresden Win!");
	} else {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, false, false, "Drawn!");
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
	int i, z, tX, tY, iNamingValue;
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
								strcpy(cNpcName, m_pNpcConfigList[m_stCrusadeStructures[i].cType]->m_cNpcName);
								break;
						}
						tX = (int) m_stCrusadeStructures[i].dX;
						tY = (int) m_stCrusadeStructures[i].dY;
						if (bCreateNewNpc(cNpcName, cName, m_pMapList[z]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWayPoint, nullptr, 0, -1, false) == false) {
							m_pMapList[z]->SetNamingValueEmpty(iNamingValue);
						} else {
							wsprintf(G_cTxt, "(!) Creating Crusade Structure(%s) at %s(%d, %d)", cNpcName, m_stCrusadeStructures[i].cMapName, tX, tY);
							PutLogList(G_cTxt);
						}
					}
				}
		}
}

void CGame::RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char * pMapName) {
	register int i;
	int iIndex;
	uint32_t dwTemp, dwTime;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
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

void CGame::RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode) {
	char cName[6], cNpcName[21], cMapName[11], cNpcWayPoint[11], cOwnerType;
	register int i, x;
	int iNamingValue, tX, tY, ix, iy;
	bool bRet;
	short sOwnerH;
	uint32_t dwTime = timeGetTime();
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	std::memset(cNpcWayPoint, 0, sizeof (cNpcWayPoint));
	std::memset(cNpcName, 0, sizeof (cNpcName));
	std::memset(cMapName, 0, sizeof (cMapName));
	if (cType < 0) return;
	if (cType >= DEF_MAXNPCTYPES) return;
	if (cNum > 10) return;
	if (m_pClientList[iClientH]->m_iConstructionPoint < m_iNpcConstructionPoint[cType]) return;
	if ((m_pClientList[iClientH]->map_ != nullptr) && (m_pClientList[iClientH]->map_->m_bIsFixedDayMode == true)) return;
	cNum = 1;
	for (x = 1; x <= cNum; x++) {
		iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
		if (iNamingValue == -1) {
		} else {
			std::memset(cName, 0, sizeof (cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
			switch (cType) {
				case 43:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "LWB-Aresden");
							break;
						case 2: strcpy(cNpcName, "LWB-Elvine");
							break;
					}
					break;
				case 36:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "AGT-Aresden");
							break;
						case 2: strcpy(cNpcName, "AGT-Elvine");
							break;
					}
					break;
				case 37:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "CGT-Aresden");
							break;
						case 2: strcpy(cNpcName, "CGT-Elvine");
							break;
					}
					break;
				case 38:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "MS-Aresden");
							break;
						case 2: strcpy(cNpcName, "MS-Elvine");
							break;
					}
					break;
				case 39:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "DT-Aresden");
							break;
						case 2: strcpy(cNpcName, "DT-Elvine");
							break;
					}
					break;
				case 51:
					switch (m_pClientList[iClientH]->m_cSide) {
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
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "Sor-Aresden");
							break;
						case 2: strcpy(cNpcName, "Sor-Elvine");
							break;
					}
					break;
				case 83:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "ATK-Aresden");
							break;
						case 2: strcpy(cNpcName, "ATK-Elvine");
							break;
					}
					break;
				case 84:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "Elf-Aresden");
							break;
						case 2: strcpy(cNpcName, "Elf-Elvine");
							break;
					}
					break;
				case 85:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "DSK-Aresden");
							break;
						case 2: strcpy(cNpcName, "DSK-Elvine");
							break;
					}
					break;
				case 86:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "HBT-Aresden");
							break;
						case 2: strcpy(cNpcName, "HBT-Elvine");
							break;
					}
					break;
				case 87:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "CT-Aresden");
							break;
						case 2: strcpy(cNpcName, "CT-Elvine");
							break;
					}
					break;
				case 88:
					switch (m_pClientList[iClientH]->m_cSide) {
						case 1: strcpy(cNpcName, "Bar-Aresden");
							break;
						case 2: strcpy(cNpcName, "Bar-Elvine");
							break;
					}
					break;
				case 89:
					switch (m_pClientList[iClientH]->m_cSide) {
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
					if (strcmp(m_pClientList[iClientH]->m_cConstructMapName, m_pClientList[iClientH]->map_->m_cName) != 0) bRet = true;
					if (abs(m_pClientList[iClientH]->m_sX - m_pClientList[iClientH]->m_iConstructLocX) > 10) bRet = true;
					if (abs(m_pClientList[iClientH]->m_sY - m_pClientList[iClientH]->m_iConstructLocY) > 10) bRet = true;
					if (bRet == true) {
						m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 2, 0, 0, nullptr);
						return;
					}
					for (i = 0; i < DEF_MAXGUILDS; i++)
						if (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID) {
							m_pGuildTeleportLoc[i].m_dwTime = dwTime;
							if (m_pGuildTeleportLoc[i].m_iV2 >= DEF_MAXCONSTRUCTNUM) {
								m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
								m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 3, 0, 0, nullptr);
								return;
							} else {
								m_pGuildTeleportLoc[i].m_iV2++;
								goto RSWU_LOOPBREAK;
							}
						}
					m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 3, 0, 0, nullptr);
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
							m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
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
			if (bRet == true) {
				m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANNOTCONSTRUCT, 1, 0, 0, nullptr);
				return;
			}
			if (cMode == 0) {
				bRet = bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, 0, DEF_MOVETYPE_FOLLOW, &tX, &tY, cNpcWayPoint, nullptr, 0, -1, false, false, false, false, m_pClientList[iClientH]->m_iGuildGUID);
				bSetNpcFollowMode(cName, m_pClientList[iClientH]->m_cCharName, DEF_OWNERTYPE_PLAYER);
			} else bRet = bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, 0, DEF_MOVETYPE_GUARD, &tX, &tY, cNpcWayPoint, nullptr, 0, -1, false, false, false, false, m_pClientList[iClientH]->m_iGuildGUID);
			if (bRet == false) {
				m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				m_pClientList[iClientH]->m_iConstructionPoint -= m_iNpcConstructionPoint[cType];
				if (m_pClientList[iClientH]->m_iConstructionPoint < 0) m_pClientList[iClientH]->m_iConstructionPoint = 0;
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, nullptr);
			}
		}
	}
}

void CGame::CheckConnectionHandler(int iClientH, char *pData) {
	char * cp;
	uint32_t * dwp, dwTimeRcv, dwTime, dwTimeGapClient, dwTimeGapServer;
	if (m_pClientList[iClientH] == nullptr) return;
	//m_pClientList[iClientH]->m_cConnectionCheck = 0;
	dwTime = timeGetTime();
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *) cp;
	dwTimeRcv = *dwp;
	CheckDenialServiceAttack(iClientH, dwTimeRcv);
	if (m_pClientList[iClientH]->m_dwInitCCTimeRcv == 0) {
		m_pClientList[iClientH]->m_dwInitCCTimeRcv = dwTimeRcv;
		m_pClientList[iClientH]->m_dwInitCCTime = dwTime;
	} else {
		dwTimeGapClient = (dwTimeRcv - m_pClientList[iClientH]->m_dwInitCCTimeRcv);
		dwTimeGapServer = (dwTime - m_pClientList[iClientH]->m_dwInitCCTime);
		if (dwTimeGapClient < dwTimeGapServer) return;
		if ((abs(dwTimeGapClient - dwTimeGapServer)) >= (DEF_CLIENTTIMEOUT)) {
			m_pClientList[iClientH]->DeleteClient(true, true);
			return;
		}
	}
}

void CGame::SelectCrusadeDutyHandler(int iClientH, int iDuty) {
	if (m_pClientList[iClientH] == nullptr) return;
	if ((m_pClientList[iClientH]->m_iGuildRank != 0) && (iDuty == 3)) return;
	if (m_iLastCrusadeWinner == m_pClientList[iClientH]->m_cSide &&
			  m_pClientList[iClientH]->m_dwCrusadeGUID == 0 && iDuty == 3) {
		m_pClientList[iClientH]->m_iConstructionPoint = 3000;
	}
	m_pClientList[iClientH]->m_iCrusadeDuty = iDuty;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, 0, nullptr);
	if (iDuty == 1)
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, 0, "Fighter");
	else if (iDuty == 2)
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, 0, "Constructor");
	else
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, 0, "Commander");
}

void CGame::MapStatusHandler(int iClientH, int iMode, char * pMapName) {
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	switch (iMode) {
		case 1:
			if (m_pClientList[iClientH]->m_iCrusadeDuty == 0) return;
			for (i = 0; i < DEF_MAXGUILDS; i++)
				if ((m_pGuildTeleportLoc[i].m_iV1 != 0) && (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID)) {
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_TCLOC, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY, 0, m_pGuildTeleportLoc[i].m_cDestMapName, m_pGuildTeleportLoc[i].m_sDestX2, m_pGuildTeleportLoc[i].m_sDestY2, 0, 0, 0, 0, m_pGuildTeleportLoc[i].m_cDestMapName2);
					std::memset(m_pClientList[iClientH]->m_cConstructMapName, 0, sizeof (m_pClientList[iClientH]->m_cConstructMapName));
					memcpy(m_pClientList[iClientH]->m_cConstructMapName, m_pGuildTeleportLoc[i].m_cDestMapName2, 10);
					m_pClientList[iClientH]->m_iConstructLocX = m_pGuildTeleportLoc[i].m_sDestX2;
					m_pClientList[iClientH]->m_iConstructLocY = m_pGuildTeleportLoc[i].m_sDestY2;
					return;
				}
			break;
		case 3:
			for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
				m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = 0;
				m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = 0;
				m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = 0;
				m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = 0;
			}
			m_pClientList[iClientH]->m_iCSIsendPoint = 0;
			std::memset(m_pClientList[iClientH]->m_cSendingMapName, 0, sizeof (m_pClientList[iClientH]->m_cSendingMapName));
			if (strcmp(pMapName, m_pClientList[iClientH]->map_->m_cName) == 0) {
				for (i = 0; i < m_pClientList[iClientH]->map_->m_iTotalCrusadeStructures; i++) {
					if (m_pClientList[iClientH]->m_iCrusadeDuty == 3) {
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].cType;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].cSide;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].sX;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].sY;
					} else if (m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].cType == 42) {
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].cType;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].cSide;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].sX;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_pClientList[iClientH]->map_->m_stCrusadeStructureInfo[i].sY;
					}
				}
				memcpy(m_pClientList[iClientH]->m_cSendingMapName, m_pClientList[iClientH]->map_->m_cName, 10);
			} else {
				if (strcmp(pMapName, "middleland") == 0) {
					for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++) {
						if (m_pClientList[iClientH]->m_iCrusadeDuty == 3) {
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
						} else if (m_stMiddleCrusadeStructureInfo[i].cType == 42) {
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
							m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
						}
					}
					strcpy(m_pClientList[iClientH]->m_cSendingMapName, "middleland");
				} else {
				}
			}
			_SendMapStatus(iClientH);
			break;
	}
}

void CGame::_SendMapStatus(int iClientH) {
	register int i, iDataSize;
	char *cp, cData[DEF_MAXCRUSADESTRUCTURES * 6];
	short * sp;
	std::memset(cData, 0, sizeof (cData));
	cp = (char *) (cData);
	memcpy(cp, m_pClientList[iClientH]->m_cSendingMapName, 10);
	cp += 10;
	sp = (short *) cp;
	*sp = (short) m_pClientList[iClientH]->m_iCSIsendPoint;
	cp += 2;
	cp++;
	if (m_pClientList[iClientH]->m_iCSIsendPoint == 0)
		m_pClientList[iClientH]->m_bIsSendingMapStatus = true;
	iDataSize = 0;
	for (i = 0; i < 100; i++) {
		if (m_pClientList[iClientH]->m_iCSIsendPoint >= DEF_MAXCRUSADESTRUCTURES) goto SMS_ENDOFDATA;
		if (m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cType == 0) goto SMS_ENDOFDATA;
		*cp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cType;
		cp++;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].sX;
		cp += 2;
		sp = (short *) cp;
		*sp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].sY;
		cp += 2;
		*cp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cSide;
		cp++;
		iDataSize += 6;
		m_pClientList[iClientH]->m_iCSIsendPoint++;
	}
	cp = (char *) (cData + 12);
	*cp = (iDataSize / 6);
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_MAPSTATUSNEXT, iDataSize + 13, 0, 0, cData);
	return;
SMS_ENDOFDATA:
	;
	cp = (char *) (cData + 12);
	*cp = (iDataSize / 6);
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_MAPSTATUSLAST, iDataSize + 13, 0, 0, cData);
	m_pClientList[iClientH]->m_bIsSendingMapStatus = false;
	return;
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
					DeleteNpc(i);
					break;
			}
		}
}

void CGame::RequestHelpHandler(int iClientH) {
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iGuildRank == -1) return;
	if (m_pClientList[iClientH]->m_iCrusadeDuty != 1) return;
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iGuildRank == 0) &&
				  (m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == m_pClientList[iClientH]->m_iGuildGUID)) {
			m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_HELP, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_iHP, m_pClientList[iClientH]->m_cCharName);
			return;
		}
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_HELPFAILED, 0, 0, 0, nullptr);
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
	int i, iIndex;
	uint32_t dwTemp, dwTime;
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

void CGame::CheckCommanderConstructionPoint(int iClientH) {
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_bIsCrusadeMode == false) return;
	if (m_pClientList[iClientH]->m_iConstructionPoint <= 0) return;
	switch (m_pClientList[iClientH]->m_iCrusadeDuty) {
		case 1:
		case 2:
			for (i = 0; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == m_pClientList[iClientH]->m_iGuildGUID)) {
					m_pClientList[i]->m_iConstructionPoint += m_pClientList[iClientH]->m_iConstructionPoint;
					m_pClientList[i]->m_iWarContribution += (m_pClientList[iClientH]->m_iConstructionPoint / 10);
					if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
						m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
					if (m_pClientList[i]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
						m_pClientList[i]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
					m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, nullptr);
					m_pClientList[iClientH]->m_iConstructionPoint = 0;
					return;
				}
			m_pClientList[iClientH]->m_iConstructionPoint = 0;
			break;
		case 3:
			break;
	}
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

bool CGame::bAddClientShortCut(int iClientH) {
	int i;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if (m_iClientShortCut[i] == iClientH) return false;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if (m_iClientShortCut[i] == 0) {
			m_iClientShortCut[i] = iClientH;
			return true;
		}
	return false;
}

void CGame::RemoveClientShortCut(int iClientH) {
	int i;
	for (i = 0; i < DEF_MAXCLIENTS + 1; i++)
		if (m_iClientShortCut[i] == iClientH) {
			m_iClientShortCut[i] = 0;
			goto RCSC_LOOPBREAK;
		}
RCSC_LOOPBREAK:
	;
	//m_iClientShortCut[i] = m_iClientShortCut[m_iTotalClients+1];
	//m_iClientShortCut[m_iTotalClients+1] = 0;
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_iClientShortCut[i] == 0) && (m_iClientShortCut[i + 1] != 0)) {
			m_iClientShortCut[i] = m_iClientShortCut[i + 1];
			m_iClientShortCut[i + 1] = 0;
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
	if (m_bIsCrusadeMode == false) return;
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

void CGame::AdminOrder_SummonStorm(int iClientH, char* pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cOwnerType;
	int i;
	class CStrTok * pStrTok;
	short sOwnerH, sType, sX, sY;
	int dX1, dY1, iWhetherBonus, iResult = 0, tX, tY, iErr, ix, iy;
	int iStormCount, iMagicAttr;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelStorm) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
		iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pClientList[iClientH]->map_->m_cWhetherStatus);
		//Magic Attribute
		iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;
		//Calculate Area Coords
		//Gives Right
		dX1 = m_pClientList[iClientH]->m_sX;
		dX1 -= 10;
		//Gives Top
		dY1 = m_pClientList[iClientH]->m_sY;
		dY1 -= 7;
		while (iStormCount != 0) {
			int randX, randY;
			randX = (rand() % 20) + dX1;
			randY = (rand() % 13) + dY1;
			switch (m_pMagicConfigList[sType]->m_sType) {
				case DEF_MAGICTYPE_DAMAGE_SPOT:
					//Calculate Damage
					m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, randX, randY);
					Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, randX, randY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					}
					//Show effects
					m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, 
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, randX, randY, (sType + 100), m_pClientList[iClientH]->m_sType);
					break;
				case DEF_MAGICTYPE_DAMAGE_LINEAR:
					sX = m_pClientList[iClientH]->m_sX;
					sY = m_pClientList[iClientH]->m_sY;
					for (i = 2; i < 10; i++) {
						iErr = 0;
						m_Misc.GetPoint2(sX, sY, randX, randY, &tX, &tY, &iErr, i);
						// tx, ty
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx-1, ty
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx+1, ty
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx, ty-1
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						// tx, ty+1
						m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
								  (m_pClientList[sOwnerH]->m_iHP > 0)) {
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
						if ((abs(tX - randX) <= 1) && (abs(tY - randY) <= 1)) break;
					}
					for (iy = randY - m_pMagicConfigList[sType]->m_sValue3; iy <= randY + m_pMagicConfigList[sType]->m_sValue3; iy++)
						for (ix = randX - m_pMagicConfigList[sType]->m_sValue2; ix <= randX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
							m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
									Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							}
						}
					// dX, dY
					m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, randX, randY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr); // v1.41 false
					m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, randX, randY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr); // v1.41 false
					}
					//Show effects
					m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, randX, randY, (sType + 100), m_pClientList[iClientH]->m_sType);
					break;
				case DEF_MAGICTYPE_DAMAGE_AREA:
					m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, randX, randY);
					if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, randX, randY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
							  (m_pClientList[sOwnerH]->m_iHP > 0)) {
						if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
					}
					for (iy = randY - m_pMagicConfigList[sType]->m_sValue3; iy <= randY + m_pMagicConfigList[sType]->m_sValue3; iy++)
						for (ix = randX - m_pMagicConfigList[sType]->m_sValue2; ix <= randX + m_pMagicConfigList[sType]->m_sValue2; ix++) {
							m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, randX, randY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							m_pClientList[iClientH]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {
								if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
									Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, randX, randY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
							}
						}
					//Show effects
					m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
							  m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, randX, randY, (sType + 100), m_pClientList[iClientH]->m_sType);
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

void CGame::SendMsg(short sOwnerH, char /*cOwnerType*/, bool /*bStatus*/, long lPass) {
	if (m_pClientList[sOwnerH] == nullptr) return;
	//m_pClientList[sOwnerH]->SendNotifyMsg(0,DEF_NOTIFY_LEVELUP, 0, 0, 0, nullptr);
	m_pClientList[sOwnerH]->SendNotifyMsg(0, lPass, 0, 0, 0, nullptr);
}

void CGame::AdminOrder_Weather(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char cPrevMode;
	int i, j, sType, sTime;
	uint32_t dwTime;
	dwTime = timeGetTime();
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelWeather) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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

void CGame::RequestChangePlayMode(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iPKCount > 0) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (memcmp(m_pClientList[iClientH]->map_->m_cName, "cityhall", 8) != 0) return;
	if (m_pClientList[iClientH]->m_iLevel < 100 ||
			  m_pClientList[iClientH]->m_bIsPlayerCivil == true) {
		if (memcmp(m_pClientList[iClientH]->m_cLocation, "aresden", 7) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "arehunter");
		else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvine", 6) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "elvhunter");
		else if (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "aresden");
		else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "elvine");
		if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
			m_pClientList[iClientH]->m_bIsPlayerCivil = false;
		else m_pClientList[iClientH]->m_bIsPlayerCivil = true;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CHANGEPLAYMODE, 0, 0, 0, m_pClientList[iClientH]->m_cLocation);
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, 100, 0, 0, 0);
	}
}

void CGame::AdminOrder_SetStatus(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSetStatus) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	std::memset(cBuff, 0, sizeof (cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	/*auto dummy = */pStrTok->pGet();
	if (token != nullptr) {
		if (token[0] == '0') {
			SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetIllusionFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetDefenseShieldFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetMagicProtectionFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetProtectionFromArrowFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetIllusionMovementFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetInhibitionCastingFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
			SetHeroFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
		}
		if (token[0] == '1') {
			SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '2') {
			SetIllusionFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '3') {
			SetDefenseShieldFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '4') {
			SetMagicProtectionFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '5') {
			SetProtectionFromArrowFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '6') {
			SetIllusionMovementFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '7') {
			SetInhibitionCastingFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
		if (token[0] == '8') {
			SetHeroFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
		}
	}
	delete pStrTok;
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000010;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000010;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00100000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00100000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000020;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000020;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000040;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000040;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000080;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000080;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x01000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x01000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00020000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00020000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x02000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x02000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x04000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x04000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x08000000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[sOwnerH] == nullptr) return;
			if (bStatus == true)
				m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x08000000;
			else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
			if (bStatus == true)
				m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00200000;
			else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFDFFFFF;
			SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
			break;
	}
}
// New 07/05/2004

void CGame::RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex) {
	if (m_pClientList[iClientH] == nullptr) return;
	if ((iObjectID <= 0) || (iObjectID >= DEF_MAXCLIENTS)) return;
	if (m_pClientList[iObjectID] == nullptr) {
	} else {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_REQGUILDNAMEANSWER, m_pClientList[iObjectID]->m_iGuildRank, iIndex, 0, m_pClientList[iObjectID]->m_cGuildName);
	}
}
// Item Logging

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
		bSendMsgToLS(MSGID_GAMEITEMLOG, iGiveH, 0, G_cTxt);
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
		case DEF_ITEMLOG_DROP:
			wsprintf(cTxt, "(%s) PC(%s)\tDrop\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_GET:
			wsprintf(cTxt, "(%s) PC(%s)\tGet\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_MAKE:
			wsprintf(cTxt, "(%s) PC(%s)\tMake\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_DEPLETE:
			wsprintf(cTxt, "(%s) PC(%s)\tDeplete\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_BUY:
			iItemCount = iRecvH;
			wsprintf(cTxt, "(%s) PC(%s)\tBuy\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, iItemCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_SELL:
			wsprintf(cTxt, "(%s) PC(%s)\tSell\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_RETRIEVE:
			wsprintf(cTxt, "(%s) PC(%s)\tRetrieve\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_DEPOSIT:
			wsprintf(cTxt, "(%s) PC(%s)\tDeposit\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_UPGRADEFAIL:
			wsprintf(cTxt, "(%s) PC(%s)\tUpgrade Fail\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName,
					  pItem->m_dwCount, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		case DEF_ITEMLOG_UPGRADESUCCESS:
			wsprintf(cTxt, "(%s) PC(%s)\tUpgrade Success\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName,
					  pItem->m_dwCount, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
					  m_pClientList[iGiveH]->map_->m_cName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
			break;
		default:
			return false;
	}
	bSendMsgToLS(MSGID_GAMEITEMLOG, iGiveH, 0, cTxt);
	return true;
}

bool CGame::_bItemLog(int iAction, int iClientH, char * cName, class CItem * pItem) {
	if (pItem == nullptr) return false;
	if (_bCheckGoodItem(pItem) == false) return false;
	if (iAction != DEF_ITEMLOG_NEWGENDROP) {
		if (m_pClientList[iClientH] == nullptr) return false;
	}
	char cTxt[200], cTemp1[120];
	std::memset(cTxt, 0, sizeof (cTxt));
	std::memset(cTemp1, 0, sizeof (cTemp1));
	if (m_pClientList[iClientH] != nullptr) m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTemp1);
	switch (iAction) {
		case DEF_ITEMLOG_NEWGENDROP:
			if (pItem == nullptr) return false;
			wsprintf(cTxt, "NPC(%s)\tDrop\t%s(%d %d %d %d)", cName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
			break;
		case DEF_ITEMLOG_SKILLLEARN:
		case DEF_ITEMLOG_MAGICLEARN:
			if (cName == nullptr) return false;
			if (m_pClientList[iClientH] == nullptr) return false;
			wsprintf(cTxt, "PC(%s)\tLearn\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
					  m_pClientList[iClientH]->map_->m_cName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
			break;
		case DEF_ITEMLOG_SUMMONMONSTER:
			if (cName == nullptr) return false;
			if (m_pClientList[iClientH] == nullptr) return false;
			wsprintf(cTxt, "PC(%s)\tSummon\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
					  m_pClientList[iClientH]->map_->m_cName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
			break;
		case DEF_ITEMLOG_POISONED:
			if (m_pClientList[iClientH] == nullptr) return false;
			wsprintf(cTxt, "PC(%s)\tBe Poisoned\t \t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName,
					  m_pClientList[iClientH]->map_->m_cName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
			break;
		case DEF_ITEMLOG_REPAIR:
			if (cName == nullptr) return false;
			if (m_pClientList[iClientH] == nullptr) return false;
			wsprintf(cTxt, "PC(%s)\tRepair\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
					  m_pClientList[iClientH]->map_->m_cName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
			break;
		default:
			return false;
	}
	bSendMsgToLS(MSGID_GAMEITEMLOG, iClientH, 0, cTxt);
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

/*void CGame::GetExp(int iClientH, int iExp, bool bIsAttackerOwn)
{
 double dV1, dV2, dV3;
 int i, iH, iUnitValue;
 uint32_t dwTime = timeGetTime();
	
	if (m_pClientList[iClientH] == nullptr) return;
	if (iExp <= 0) return;
	if (m_pClientList[iClientH]->m_iLevel <= 80) {
		dV1 = (double)(80 - m_pClientList[iClientH]->m_iLevel);
		dV2 = dV1 * 0.025f;
		dV3 = (double)iExp;
		dV1 = (dV2 + 1.025f)*dV3;
		iExp = (int)dV1;
	}
	else { //Lower exp
		if ((m_pClientList[iClientH]->m_iLevel >= 100) &&((strcmp(m_pClientList[iClientH]->map_->m_cName, "arefarm") == 0)
		|| (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvfarm") == 0))) {
			iExp = (iExp/10) ;
	}
	else if ((strcmp(m_pClientList[iClientH]->map_->m_cName, "arefarm") == 0)
		|| (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvfarm") == 0)) {
			iExp = (iExp*1/4) ;
	}
	if ((m_pClientList[iClientH]->m_iPartyID != nullptr) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) &&
		((dwTime - m_pClientList[iClientH]->m_dwLastActionTime) < 1000*60*5)) {
		if ((iExp >= m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers > 0)) {
			dV1 = (double)m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers;
			dV2 = 2.5f * dV1;
			dV3 = (double)iExp;
			dV1 = ((dV2/100.0f) * dV3) +0.5f;
			iExp += (int)dV1;
			dV1 = (double)iExp;
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers <= 0)
				 dV2 = 1.0f;
			else dV2 = (double)m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers;
			dV3 = (dV1 / dV2) + 0.5f;
			iUnitValue = (int)dV3;
			for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++) {
				iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
			if (m_pClientList[iH]->m_iLevel >= 1 && m_pClientList[iH]->m_iLevel < 20)
				iUnitValue *= (int)3.5;
			else if (m_pClientList[iH]->m_iLevel >= 20 && m_pClientList[iH]->m_iLevel < 121)
				iUnitValue *= 3;
			else if (m_pClientList[iH]->m_iLevel >= 120 && m_pClientList[iH]->m_iLevel < 161)
				iUnitValue *= 2;
			else if (m_pClientList[iH]->m_iLevel >= 160 && m_pClientList[iH]->m_iLevel < 180)
				iUnitValue *= (int)1.5;
			if((m_pClientList[iH]->m_iStatus & 0x10000) != 0)
				iUnitValue *= 3;
			if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_iHP > 0)) m_pClientList[iH]->m_iExpStock += iUnitValue;
			}
				if ((bIsAttackerOwn == true) && (iTotalPartyMembers > 1))
					m_pClientList[iClientH]->m_iExpStock += (iUnitValue/10);
		}
	} // if
	if (m_pClientList[iClientH]->m_iLevel >= 1 && m_pClientList[iClientH]->m_iLevel < 20)
		iExp *= (int)3.5;
	else if (m_pClientList[iClientH]->m_iLevel >= 20 && m_pClientList[iClientH]->m_iLevel < 121)
		iExp *= 3;
	else if (m_pClientList[iClientH]->m_iLevel > 120 && m_pClientList[iClientH]->m_iLevel < 161)
		iExp *= 2;
	else if (m_pClientList[iClientH]->m_iLevel > 160 && m_pClientList[iClientH]->m_iLevel < 180)
		iExp *= (int)1.5;
	if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0)
			iExp *= 3;
	else m_pClientList[iClientH]->m_iExpStock += iExp;
	}
}*/
void CGame::GetExp(int iClientH, int iExp, bool /*bIsAttackerOwn*/) {
	double dV1, dV2, dV3;
	int i, iH, iUnitValue;
	int iTotalPartyMembers;
	if (m_pClientList[iClientH] == nullptr) return;
	if (iExp <= 0) return;
	if (m_pClientList[iClientH]->m_iLevel <= 80) {
		dV1 = (double) (80 - m_pClientList[iClientH]->m_iLevel);
		dV2 = dV1 * 0.025f;
		dV3 = (double) iExp;
		dV1 = (dV2 + 1.025f) * dV3;
		iExp = (int) dV1;
	} else { //Lower exp
		if ((m_pClientList[iClientH]->m_iLevel >= 100) &&((strcmp(m_pClientList[iClientH]->map_->m_cName, "aresden") == 0)
				  || (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvine") == 0))) {
			iExp = (iExp / 10);
		} else if ((strcmp(m_pClientList[iClientH]->map_->m_cName, "aresden") == 0)
				  || (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvine") == 0)) {
			iExp = (iExp * 1 / 4);
		}
	}
	//Check for party status, else give exp to player
	//if ((m_pClientList[iClientH]->m_iPartyID != nullptr) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) &&
	//	((dwTime - m_pClientList[iClientH]->m_dwLastActionTime) < 1000*60*5)) {
	if ((m_pClientList[iClientH]->m_iPartyID != 0) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)) {
		//Only divide exp if > 1 person and exp > 10
		if (iExp >= 10 && m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers > 0) {
			//Calc total ppl in party
			iTotalPartyMembers = 0;
			for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++) {
				iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
				if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_iHP > 0)) {
					//Newly added, Only players on same map get exp :}
					//if(m_pClientList[iH]->m_cMapIndex == m_pClientList[iClientH]->m_cMapIndex)
					if(m_pClientList[iH]->map_ == m_pClientList[iClientH]->map_) {
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
				iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
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
			if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0)
				iExp *= 3;
			m_pClientList[iClientH]->m_iExpStock += iExp;
		}
	} else {
		if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0)
			iExp *= 3;
		//Add exp to player
		m_pClientList[iClientH]->m_iExpStock += iExp;
	}
}

/*
void CGame::GetExp(int iClientH, int iExp, bool bIsAttackerOwn)
{
	double dV1, dV2, dV3;
	int i, iH, iUnitValue;
	uint32_t dwTime = timeGetTime();
	
	if (m_pClientList[iClientH] == nullptr) return;
	if (iExp <= 0) return;
	
	if (m_pClientList[iClientH]->m_iLevel <= 80) {
		dV1 = (double)(80 - m_pClientList[iClientH]->m_iLevel);
		dV2 = dV1 * 0.025f;
		dV3 = (double)iExp;
		dV1 = (dV2 + 1.025f)*dV3;
		iExp = (int)dV1;
	}
	else {
		
		if ((m_pClientList[iClientH]->m_iLevel >= 100) &&((strcmp(m_pClientList[iClientH]->map_->m_cName, "arefarm") == 0)
			|| (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvfarm") == 0))) {
				iExp = (iExp/10) ;
			} 
		else if ((strcmp(m_pClientList[iClientH]->map_->m_cName, "arefarm") == 0)
			|| (strcmp(m_pClientList[iClientH]->map_->m_cName, "elvfarm") == 0)) {
				iExp = (iExp*1/4) ;
			}
	}
	if ((m_pClientList[iClientH]->m_iPartyID != nullptr) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) &&
		((dwTime - m_pClientList[iClientH]->m_dwLastActionTime) < 1000*60*5)) {
			
			if ((iExp >= m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers > 0)) {
				
				
				dV1 = (double)m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers;
				dV2 = 2.5f * dV1;
				dV3 = (double)iExp;
				dV1 = ((dV2/100.0f) * dV3) +0.5f;
				iExp += (int)dV1;
				dV1 = (double)iExp;
				if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers <= 0)
					dV2 = 1.0f;
				else dV2 = (double)m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers;
				dV3 = (dV1 / dV2) + 0.5f;
				iUnitValue = (int)dV3;
#if defined(DEF_TESTSERVER)
				iUnitValue = iUnitValue * 10;
#endif
				
				for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++) {
					iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
					if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_iHP > 0)) {
						char buff[1000];
						wsprintf(buff,"%s GetExp %d",m_pClientList[iH]->m_cCharName,iUnitValue);
						PutLogList(buff);
						m_pClientList[iH]->m_iExpStock += iUnitValue;
					}
				}
				
				if (bIsAttackerOwn == true) m_pClientList[iClientH]->m_iExpStock += (iUnitValue/10);
			}
#if defined(DEF_TESTSERVER)
			else m_pClientList[iClientH]->m_iExpStock += (iExp * 10);
#else
			else m_pClientList[iClientH]->m_iExpStock += iExp;
#endif
		} // if
#if defined(DEF_TESTSERVER)
	else m_pClientList[iClientH]->m_iExpStock += (iExp * 10);
#else
	else m_pClientList[iClientH]->m_iExpStock += iExp;
#endif
}*/
void CGame::AdminOrder_GoTo(int iClientH, char* pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cPlayerName[11], cMapName[32];
	class CStrTok * pStrTok;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelGoto) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) {
					if (m_pClientList[i]->m_sX == -1 && m_pClientList[i]->m_sX == -1) {
						wsprintf(cBuff, "GM Order(%s): GoTo MapName(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[i]->map_->m_cName);
					} else {
						wsprintf(cBuff, "GM Order(%s): GoTo MapName(%s)(%d %d)", m_pClientList[iClientH]->m_cCharName,
								  m_pClientList[i]->map_->m_cName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					}
					bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cBuff);
					std::memset(cMapName, 0, sizeof (cMapName));
					strcpy(cMapName, m_pClientList[i]->map_->m_cName);
					m_pClientList[iClientH]->RequestTeleportHandler("2   ", cMapName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
				}
				delete pStrTok;
				return;
			}
		}
	}
	m_pClientList[iClientH]->m_bIsAdminOrderGoto = true;
	delete pStrTok;
}

void CGame::AdminOrder_MonsterCount(int iClientH, char* /*pData*/, uint32_t dwMsgSize) {
	int iMonsterCount;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelMonsterCount) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	iMonsterCount = m_pClientList[iClientH]->map_->m_iTotalAliveObject;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_MONSTERCOUNT, iMonsterCount, 0, 0, nullptr);
}

void CGame::AdminOrder_SetForceRecallTime(int iClientH, char *pData, uint32_t dwMsgSize) {
	class CStrTok * pStrTok;
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	int iTime;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSetRecallTime) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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

void CGame::AdminOrder_UnsummonBoss(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelUnsummonBoss) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
					NpcKilledHandler(iClientH, DEF_OWNERTYPE_PLAYER, i, 0);
					break;
			}
		}
	}
}

void CGame::AdminOrder_ClearNpc(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelClearNpc) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
					NpcKilledHandler(iClientH, DEF_OWNERTYPE_PLAYER, i, 0);
					break;
			}
		}
	}
}
// New 12/05/2004

void CGame::RemoveCrusadeNpcs(void) {
	for (int i = 0; i < DEF_MAXNPCS; i++) {
		if (m_pNpcList[i] != nullptr) {
			if ((m_pNpcList[i]->m_sType >= 43 && m_pNpcList[i]->m_sType <= 47) || m_pNpcList[i]->m_sType == 51) {
				NpcKilledHandler(0, 0, i, 0);
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

bool CGame::_bCrusadeLog(int iAction, int iClientH, int iData, const char * cName) {
	char cTxt[200];
	std::memset(cTxt, 0, sizeof (cTxt));
	switch (iAction) {
		case DEF_CRUSADELOG_ENDCRUSADE:
			if (cName == nullptr) return false;
			wsprintf(cTxt, "\tEnd Crusade\t%s", cName);
			break;
		case DEF_CRUSADELOG_SELECTDUTY:
			if (cName == nullptr) return false;
			if (m_pClientList[iClientH] == nullptr) return false;
			wsprintf(cTxt, "PC(%s)\tSelect Duty\t(%s)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, cName, m_pClientList[iClientH]->m_cGuildName);
			break;
		case DEF_CRUSADELOG_GETEXP:
			if (m_pClientList[iClientH] == nullptr) return false;
			wsprintf(cTxt, "PC(%s)\tGet Exp\t(%d)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, iData, m_pClientList[iClientH]->m_cGuildName);
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
// Testcode, found reference to it in 3.51, InitPlayerData and RequestTeleportHandler

int CGame::iGetPlayerABSStatus(int iClientH) {
	int iRet;
	if (m_pClientList[iClientH] == nullptr) return 0;
	iRet = 0;
	if (m_pClientList[iClientH]->m_iPKCount != 0) {
		iRet = 8;
	}
	if (m_pClientList[iClientH]->m_cSide != 0) {
		iRet = iRet | 4;
	}
	if (m_pClientList[iClientH]->m_cSide == 1) {
		iRet = iRet | 2;
	}
	if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
		iRet = iRet | 1;
	return iRet;
}
//Init item based in its ID

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

void CGame::SetSlateFlag(int iClientH, short sType, bool bFlag) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (sType == DEF_NOTIFY_SLATECLEAR) {
		m_pClientList[iClientH]->m_iStatus &= 0xFFBFFFFF;
		m_pClientList[iClientH]->m_iStatus &= 0xFF7FFFFF;
		m_pClientList[iClientH]->m_iStatus &= 0xFFFEFFFF;
		return;
	}
	if (bFlag == true) {
		if (sType == 1) { // Invincible slate
			m_pClientList[iClientH]->m_iStatus |= 0x400000;
		} else if (sType == 3) { // Mana slate
			m_pClientList[iClientH]->m_iStatus |= 0x800000;
		} else if (sType == 4) { // Exp slate
			m_pClientList[iClientH]->m_iStatus |= 0x10000;
		}
	} else {
		if ((m_pClientList[iClientH]->m_iStatus & 0x400000) != 0) {
			m_pClientList[iClientH]->m_iStatus &= 0xFFBFFFFF;
		} else if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0) {
			m_pClientList[iClientH]->m_iStatus &= 0xFF7FFFFF;
		} else if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0) {
			m_pClientList[iClientH]->m_iStatus &= 0xFFFEFFFF;
		}
	}
	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}
// New 18/05/2004

void CGame::SetPlayingStatus(int iClientH) {
	char cMapName[11], cLocation[11];
	if (m_pClientList[iClientH] == nullptr) return;
	std::memset(cMapName, 0, sizeof (cMapName));
	std::memset(cLocation, 0, sizeof (cLocation));
	strcpy(cLocation, m_pClientList[iClientH]->m_cLocation);
	strcpy(cMapName, m_pClientList[iClientH]->map_->m_cName);
	m_pClientList[iClientH]->m_cSide = 0;
	m_pClientList[iClientH]->m_bIsOwnLocation = false;
	m_pClientList[iClientH]->m_bIsPlayerCivil = false;
	if (memcmp(cLocation, cMapName, 3) == 0) {
		m_pClientList[iClientH]->m_bIsOwnLocation = true;
	}
	if (memcmp(cLocation, "are", 3) == 0)
		m_pClientList[iClientH]->m_cSide = 1;
	else if (memcmp(cLocation, "elv", 3) == 0)
		m_pClientList[iClientH]->m_cSide = 2;
	else {
		if (strcmp(cMapName, "elvine") == 0 || strcmp(cMapName, "aresden") == 0) {
			m_pClientList[iClientH]->m_bIsOwnLocation = true;
		}
		m_pClientList[iClientH]->m_bIsNeutral = true;
	}
	if (memcmp(cLocation, "arehunter", 9) == 0 || memcmp(cLocation, "elvhunter", 9) == 0) {
		m_pClientList[iClientH]->m_bIsPlayerCivil = true;
	}
	if (memcmp(m_pClientList[iClientH]->map_->m_cName, "bisle", 5) == 0) {
		m_pClientList[iClientH]->m_bIsPlayerCivil = false;
	}
	if (memcmp(m_pClientList[iClientH]->map_->m_cName, "bsmith", 6) == 0 ||
			  memcmp(m_pClientList[iClientH]->map_->m_cName, "gldhall", 7) == 0 ||
			  memcmp(m_pClientList[iClientH]->map_->m_cName, "gshop", 5) == 0)
		m_pClientList[iClientH]->m_pIsProcessingAllowed = true;
	else
		m_pClientList[iClientH]->m_pIsProcessingAllowed = false;
}

void CGame::ForceChangePlayMode(int iClientH, bool bNotify) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0)
		strcpy(m_pClientList[iClientH]->m_cLocation, "aresden");
	else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0)
		strcpy(m_pClientList[iClientH]->m_cLocation, "elvine");
	if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
		m_pClientList[iClientH]->m_bIsPlayerCivil = false;
	if (bNotify) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CHANGEPLAYMODE, 0, 0, 0, m_pClientList[iClientH]->m_cLocation);
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
	}
}

void CGame::ShowVersion(int iClientH) {
	char cVerMessage[256];
	std::memset(cVerMessage, 0, sizeof (cVerMessage));
	wsprintf(cVerMessage, "Helbreath Sources %s.%s - www.xtremehb.com", DEF_UPPERVERSION, DEF_LOWERVERSION);
	ShowClientMsg(iClientH, cVerMessage);
}
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

bool CGame::bCheckClientAttackFrequency(int iClientH, uint32_t dwClientTime) {
	uint32_t dwTimeGap;
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return false;
	if (m_pClientList[iClientH]->m_dwAttackFreqTime == 0)
		m_pClientList[iClientH]->m_dwAttackFreqTime = dwClientTime;
	else {
		dwTimeGap = dwClientTime - m_pClientList[iClientH]->m_dwAttackFreqTime;
		m_pClientList[iClientH]->m_dwAttackFreqTime = dwClientTime;
		if (dwTimeGap < 450) {
			wsprintf(G_cTxt, "Swing Hack: (%s) Player: (%s) - attacking with weapon at irregular rates.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			PutHackLogFileList(G_cTxt);
			m_pClientList[iClientH]->DeleteClient(true, true);
			return false;
		}
		//testcode
		//wsprintf(G_cTxt, "Attack: %d", dwTimeGap);
		//PutLogList(G_cTxt);
	}
	return false;
}

bool CGame::bCheckClientMagicFrequency(int iClientH, uint32_t dwClientTime) {
	uint32_t dwTimeGap;
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_dwMagicFreqTime == 0)
		m_pClientList[iClientH]->m_dwMagicFreqTime = dwClientTime;
	else {
		dwTimeGap = dwClientTime - m_pClientList[iClientH]->m_dwMagicFreqTime;
		m_pClientList[iClientH]->m_dwMagicFreqTime = dwClientTime;
		if ((dwTimeGap < 1500) && (m_pClientList[iClientH]->m_bMagicConfirm == true)) {
			wsprintf(G_cTxt, "Speed Cast: (%s) Player: (%s) - casting magic at irregular rates. ", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			PutHackLogFileList(G_cTxt);
			m_pClientList[iClientH]->DeleteClient(true, true);
			return false;
		}
		m_pClientList[iClientH]->m_iSpellCount--;
		m_pClientList[iClientH]->m_bMagicConfirm = false;
		m_pClientList[iClientH]->m_bMagicPauseTime = false;
		//testcode
		//wsprintf(G_cTxt, "Magic: %d", dwTimeGap);
		//PutLogList(G_cTxt);
	}
	return false;
}

bool CGame::bCheckClientMoveFrequency(int iClientH, uint32_t dwClientTime) {
	uint32_t dwTimeGap;
	if (m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return false;
	if (m_pClientList[iClientH]->m_dwMoveFreqTime == 0)
		m_pClientList[iClientH]->m_dwMoveFreqTime = dwClientTime;
	else {
		if (m_pClientList[iClientH]->m_bIsMoveBlocked == true) {
			m_pClientList[iClientH]->m_dwMoveFreqTime = 0;
			m_pClientList[iClientH]->m_bIsMoveBlocked = false;
			return false;
		}
		if (m_pClientList[iClientH]->m_bIsAttackModeChange == true) {
			m_pClientList[iClientH]->m_dwMoveFreqTime = 0;
			m_pClientList[iClientH]->m_bIsAttackModeChange = false;
			return false;
		}
		dwTimeGap = dwClientTime - m_pClientList[iClientH]->m_dwMoveFreqTime;
		m_pClientList[iClientH]->m_dwMoveFreqTime = dwClientTime;
		if ((dwTimeGap < 200) && (dwTimeGap >= 0)) {
			wsprintf(G_cTxt, "Speed Hack: (%s) Player: (%s) - running too fast.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			PutHackLogFileList(G_cTxt);
			m_pClientList[iClientH]->DeleteClient(true, true);
			return false;
		}
		// testcode
		// wsprintf(G_cTxt, "Move: %d", dwTimeGap);
		// PutLogList(G_cTxt);
	}
	return false;
}

/*
bool CGame::bCheckClientInvisibility(short iClientH)
{
	bool bFlag;
	int iShortCutIndex;
	int i;
	short sRange;
	if(m_pClientList[iClientH] == nullptr) return false;
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return false;
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
			if ((bFlag == true) && (m_pClientList[i] != nullptr) && (m_pClientList[i]->m_bIsInitComplete == true)) {
				if ( (m_pClientList[i]->m_cMapIndex == m_pClientList[iClientH]->m_cMapIndex) &&
					  (m_pClientList[i]->m_sX >= m_pClientList[iClientH]->m_sX - 10 - sRange) &&
					 (m_pClientList[i]->m_sX <= m_pClientList[iClientH]->m_sX + 10 + sRange) &&
					 (m_pClientList[i]->m_sY >= m_pClientList[iClientH]->m_sY - 8 - sRange) &&
					 (m_pClientList[i]->m_sY <= m_pClientList[iClientH]->m_sY + 8 + sRange) ) {
			// iClient = center screen character
			// i = visable characters on screen
					if ( (m_pClientList[iClientH]->m_cSide != m_pClientList[i]->m_cSide)
								&& (i != iClientH)
								&& (m_pClientList[iClientH]->m_sAppr4 == m_pClientList[iClientH]->m_sAppr4 & 0x0001)
								) {
						//wsprintf(G_cTxt, "(!) Invisibility Hack Suspect: (%s)", m_pClientList[iClientH]->m_cCharName);
						//PutLogList(G_cTxt);
						//m_pClientList[iClientH]->DeleteClient(true, true);
						}
					}
			}
		}
	return false;
}*/
void CGame::CrusadeWarStarter() {
	SYSTEMTIME SysTime;
	int i;
	if (m_bIsCrusadeMode == true) return;
	if (m_bIsCrusadeWarStarter == false) return;
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
	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] && m_pClientList[i]->m_bMarkedForDeletion) {
			m_pClientList[i].reset();
			RemoveClientShortCut(i);
			--m_iTotalClients;
		}
	}
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
		if ((m_bIsGameStarted == false) && (m_bIsItemAvailable == true) &&
				  (m_bIsNpcAvailable == true) &&
				  (m_bIsLogSockAvailable == true) && (m_bIsMagicAvailable == true) &&
				  (m_bIsSkillAvailable == true) && (m_bIsPortionAvailable == true) &&
				  (m_bIsQuestAvailable == true) && (m_bIsBuildItemAvailable == true) &&
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
	if (m_bHappyHour == true) {
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
		EnergySphereProcessor();
		m_dwGameTime3 = dwTime;
	}
	if ((dwTime - m_dwGameTime7) > 100000) { //Timer
		switch (iDice(1, 19)) { //Pick a random advert
				//Change these to whatever you want to display
			case 1: wsprintf(cAdvert, "Server-Info: Criticals: Ctrl + C (300 Criticals = 5000 Gold).");
				break;
			case 2: wsprintf(cAdvert, "Server-Info: MIDDLELAND NO TIENE PITS (SOLO SE USA EN CRUSADE).");
				break;
			case 3: wsprintf(cAdvert, "Server-Info: Lista de Drops y dem�s guias en la Web!.");
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
			case 10: wsprintf(cAdvert, "SERVER-Info: Pedimos disculpas a todos los afectados por la caida del primer d�a!.");
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
		if ((m_bIsHeldenianReady == true) && (m_bIsHeldenianMode == false)) {
			HeldenianWarStarter();
		}
		if ((m_bIsHeldenianReady == true) && (m_bIsHeldenianMode == true)) {
			HeldenianWarEnder();
		}
		if ((m_bHeldenianRunning == false) && (m_bIsHeldenianMode == true)) {
			AutomaticHeldenianEnd();
		}
		if ((m_bHeldenianInitiated == true) && (m_bIsHeldenianReady == true)) {
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
	if ((m_bIsServerShutdowned == false) && (m_bOnExitProcess == true) && ((dwTime - m_dwExitProcessTime) > 1000 * 2)) {
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

void CGame::RequestGuildTeleportHandler(int iClientH) {
	register int i;
	char cMapName[11];
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iLockedMapTime != 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_LOCKEDMAP, m_pClientList[iClientH]->m_iLockedMapTime, 0, 0, m_pClientList[iClientH]->lockedMap_->m_cName);
		return;
	}
	if (!m_bIsCrusadeMode) {
		wsprintf(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - setting teleport location when crusade is disabled.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutLogList(G_cTxt);
		m_pClientList[iClientH]->DeleteClient(true, true);
		return;
	}
	if (m_pClientList[iClientH]->m_iCrusadeDuty == 0) {
		wsprintf(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - teleporting when not in a guild", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutLogList(G_cTxt);
		m_pClientList[iClientH]->DeleteClient(true, true);
		return;
	}
	if (m_pClientList[iClientH]->map_ == middlelandMap_.lock()) return;
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID) {
			std::memset(cMapName, 0, sizeof (cMapName));
			strcpy(cMapName, m_pGuildTeleportLoc[i].m_cDestMapName);
			wsprintf(G_cTxt, "ReqGuildTeleport: %d %d %d %s", m_pClientList[iClientH]->m_iGuildGUID, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY, cMapName);
			PutLogList(G_cTxt);
			m_pClientList[iClientH]->RequestTeleportHandler("2   ", cMapName, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY);
			return;
		}
	switch (m_pClientList[iClientH]->m_cSide) {
		case 1:
			break;
		case 2:
			break;
	}
}

void CGame::GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char * pMapName) {
	int i, iIndex;
	uint32_t dwTemp, dwTime;
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

void CGame::RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, const char * pMapName) {
	register int i;
	int iIndex;
	uint32_t dwTemp, dwTime;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
	if (!m_bIsCrusadeMode) {
		wsprintf(G_cTxt, "Accessing Crusade Set Teleport: (%s) Player: (%s) - setting point when not a crusade.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutLogList(G_cTxt);
		m_pClientList[iClientH]->DeleteClient(true, true);
		return;
	}
	if (m_pClientList[iClientH]->m_iCrusadeDuty != 3) {
		wsprintf(G_cTxt, "Accessing Crusade Set Teleport: (%s) Player: (%s) - setting point when not a guildmaster.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutLogList(G_cTxt);
		m_pClientList[iClientH]->DeleteClient(true, true);
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

void CGame::CancelExchangeItem(int iClientH) {
	auto with = m_pClientList[iClientH]->exchangingWith_.lock();
	if (with) {
		_ClearExchangeStatus(with->id_);
	}
	_ClearExchangeStatus(iClientH);
}

void CGame::_ClearExchangeStatus(int iClientH) {
	if ((iClientH <= 0) || (iClientH >= DEF_MAXCLIENTS)) return;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->exchangingWith_.lock()) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CANCELEXCHANGEITEM, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
	}
	m_pClientList[iClientH]->m_dwInitCCTime = false;
	m_pClientList[iClientH]->m_iAlterItemDropIndex = 0;
	m_pClientList[iClientH]->exchangingWith_.reset();
	m_pClientList[iClientH]->m_bIsExchangeMode = false;
	m_pClientList[iClientH]->m_bIsExchangeConfirm = false;
}

void CGame::SetForceRecallTime(int iClientH) {
	int iTL_ = 0;
	SYSTEMTIME SysTime;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsWarLocation == false) return;
	//PutLogProcessList("PROCESS #353");
	if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0) {
		if (m_sForceRecallTime > 0) {
			m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sForceRecallTime;
		} else {
			GetLocalTime(&SysTime);
			switch (SysTime.wDayOfWeek) {
				case 1: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeMonday;
					break;
				case 2: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeTuesday;
					break;
				case 3: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeWednesday;
					break;
				case 4: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeThursday;
					break;
				case 5: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeFriday;
					break;
				case 6: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSaturday;
					break;
				case 0: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSunday;
					break;
			}
		}
	} else { // if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0) 
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
			if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > iTL_)
				m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = iTL_;
		}
	}
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
	char cMsg[120];
	std::memset(cMsg, 0, sizeof (cMsg));
	if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall >= 60) {
		int azucar = (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall / 60);
		wsprintf(cMsg, "Time left force recall %d Min", azucar);
	}// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	else {
		wsprintf(cMsg, "Time left force recall %d Seconds", m_pClientList[iClientH]->m_iTimeLeft_ForceRecall);
	}
	ShowClientMsg(iClientH, cMsg);
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -		
}

void CGame::CheckForceRecallTime(int iClientH) {
	SYSTEMTIME SysTime;
	int iTL_;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return;
	//PutLogProcessList("PROCESS #354");
	if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0) {
		// has admin set a recall time ??
		if (m_sForceRecallTime > 0) {
			m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = m_sForceRecallTime * 60;
		}// use standard recall time calculations
		else {
			GetLocalTime(&SysTime);
			switch (SysTime.wDayOfWeek) {
				case 1: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeMonday;
					break;
				case 2: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeTuesday;
					break;
				case 3: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeWednesday;
					break;
				case 4: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeThursday;
					break;
				case 5: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeFriday;
					break;
				case 6: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSaturday;
					break;
				case 0: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * m_sRaidTimeSunday;
					break;
				default: m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 60 * 1;
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
			if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > iTL_)
				m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = iTL_;
		}
	}
	m_pClientList[iClientH]->m_bIsWarLocation = true;
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
	char cMsg[120];
	std::memset(cMsg, 0, sizeof (cMsg));
	if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall >= 60) {
		int azucar = (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall / 60);
		wsprintf(cMsg, "Time left force recall %d Min", azucar);
	}// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	else {
		wsprintf(cMsg, "Time left force recall %d Seconds", m_pClientList[iClientH]->m_iTimeLeft_ForceRecall);
	}
	ShowClientMsg(iClientH, cMsg);
	// T I M E - L E F T - F O R C E - R E C A L L - - - - - - - - - - - - - - - - - - - - - - - - - - - - -		
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

bool CGame::bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
		  int iItemSpreadType, int iSpreadRange,
		  int *iItemIDs, POINT *BasePos, int *iNumItem) {
	int iProb = 100;
	float fProb, fProbA, fProbB, fProbC;
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
// 05/21/2004 - Hypnotoad - Balances to drop status

void CGame::NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType) {
	class CItem * pItem;
	char cColor, cItemName[21];
	int iGenLevel, iResult, iItemID;
	uint32_t dwType, dwValue;
	double dTmp1, dTmp2, dTmp3;
	if (m_pNpcList[iNpcH] == nullptr) return;
	if ((cAttackerType != DEF_OWNERTYPE_PLAYER) || (m_pNpcList[iNpcH]->m_bIsSummoned == true)) return;
	if (m_pNpcList[iNpcH]->m_bIsUnsummoned == true) return;
	std::memset(cItemName, 0, sizeof (cItemName));
	switch (m_pNpcList[iNpcH]->m_sType) {
			// NPC not dropping Gold
		case 21: // Guard
		case 34: // Dummy
		case 64: // Crop
			return;
	}
	// 6500 default; the lower the greater the Weapon/Armor/Wand Drop
	if (iDice(1, 10000) >= m_iPrimaryDropRate) {
		// 35% Drop 60% of that is gold
		// 35% Chance of drop (35/100)
		if (iDice(1, 10000) <= 4000) {
			iItemID = 90; // Gold: (35/100) * (60/100) = 21%
			// If a non-existing itemID is given create no item
			pItem = new class CItem;
			if (_bInitItemAttr(*pItem, iItemID) == false) {
				delete pItem;
				return;
			}
			pItem->m_dwCount = (uint32_t) (iDice(1, (m_pNpcList[iNpcH]->m_iGoldDiceMax - m_pNpcList[iNpcH]->m_iGoldDiceMin)) + m_pNpcList[iNpcH]->m_iGoldDiceMin);
			// v1.42 Gold 
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_iAddGold != 0)) {
				dTmp1 = (double) m_pClientList[sAttackerH]->m_iAddGold;
				dTmp2 = (double) pItem->m_dwCount;
				dTmp3 = (dTmp1 / 100.0f) * dTmp2;
				pItem->m_dwCount += (int) dTmp3;
			}
		} else {
			// 9000 default; the lower the greater the Weapon/Armor/Wand Drop
			// 35% Drop 40% of that is an Item 
			dTmp1 = m_pClientList[sAttackerH]->m_iRating*m_cRepDropModifier;
			if (dTmp1 > 3000) dTmp1 = 3000;
			if (dTmp1 < -3000) dTmp1 = -3000;
			dTmp2 = (m_iSecondaryDropRate - (dTmp1));
			if (iDice(1, 10000) <= dTmp2) {
				// 40% Drop 90% of that is a standard drop
				// Standard Drop Calculation: (35/100) * (40/100) * (90/100) = 12.6%
				iResult = iDice(1, 13000);
				if ((iResult >= 1) && (iResult <= 3000)) dwValue = 1;
				else if ((iResult >= 3001) && (iResult <= 4000)) dwValue = 2;
				else if ((iResult >= 4001) && (iResult <= 5500)) dwValue = 3;
				else if ((iResult >= 5501) && (iResult <= 6700)) dwValue = 4;
				else if ((iResult >= 6701) && (iResult <= 8500)) dwValue = 5;
				else if ((iResult >= 8501) && (iResult <= 8999)) dwValue = 6;
				else if ((iResult >= 9000) && (iResult <= 10500)) dwValue = 7;
				else if ((iResult >= 10501) && (iResult <= 11200)) dwValue = 8;
				else if ((iResult >= 11201) && (iResult <= 13000)) dwValue = 9;
				switch (dwValue) {
					case 1: iItemID = 95;
						break; // Green Potion
					case 2: iItemID = 91;
						break; // Red Potion
					case 3: iItemID = 93;
						break; // Blue Potion
					case 4: iItemID = 96;
						break; // Big Green Potion
					case 5: iItemID = 92;
						break; // Big Red Potion
					case 6: iItemID = 94;
						break; // Big Blue Potion
					case 7: switch (iDice(1, 2)) {
							case 1: iItemID = 390;
								break; // Power Green Potion
							case 2: iItemID = 95;
								break; // Green Potion
						}
						break;
					case 8: switch (iDice(1, 6)) {
							case 1: iItemID = 391;
								break; // Super Power Green Potion
							case 2: iItemID = 650;
								break; // Zemstone of Sacrifice
							case 3: iItemID = 656;
								break; // Xelima Stone
							case 4: iItemID = 657;
								break; // Merien Stone
							case 5: iItemID = 95;
								break; // Green Potion
							case 6: switch (iDice(1, 5)) {
									case 1: iItemID = 651;
										break; // GreenBall
									case 2: iItemID = 652;
										break; // RedBall
									case 3: iItemID = 653;
										break; // YellowBall
									case 4: iItemID = 654;
										break; // BlueBall
									case 5: switch (iDice(1, 11)) {
											case 1: iItemID = 881;
												break; // ArmorDye(Indigo)
											case 2: iItemID = 882;
												break; // ArmorDye(Crimson-Red)
											case 3: iItemID = 883;
												break; // ArmorDye(Gold)
											case 4: iItemID = 884;
												break; // ArmorDye(Aqua)
											case 5: iItemID = 885;
												break; // ArmorDye(Pink)
											case 6: iItemID = 886;
												break; // ArmorDye(Violet)
											case 7: iItemID = 887;
												break; // ArmorDye(Blue)
											case 8: iItemID = 888;
												break; // ArmorDye(Khaki)
											case 9: iItemID = 889;
												break; // ArmorDye(Yellow)
											case 10: iItemID = 890;
												break; // ArmorDye(Red)
											case 11: iItemID = 655;
												break; // PearlBall
										}
										break;
								}
								break;
						}
						break;
					case 9:
						SYSTEMTIME SysTime;
						GetLocalTime(&SysTime);
						if (((short) SysTime.wMonth == 12) && (m_pNpcList[iNpcH]->m_sType == 61 || 55)) {
							switch (iDice(1, 4)) {
								case 1: iItemID = 780;
									break; // Red Candy
								case 2: iItemID = 781;
									break; // Blue Candy
								case 3: iItemID = 782;
									break; // Green Candy
								case 4: iItemID = 91;
									break; // Red Potion
							}
						}
						break;
				}
				// If a non-existing item is created then delete the item
				pItem = new class CItem;
				if (_bInitItemAttr(*pItem, iItemID) == false) {
					delete pItem;
					return;
				}
			} else {
				// Valuable Drop Calculation: (35/100) * (40/100) * (10/100) = 1.4%
				// Define iGenLevel using Npc.cfg#
				switch (m_pNpcList[iNpcH]->m_sType) {
					case 10: // Slime
					case 16: // Giant-Ant
					case 22: // Amphis
					case 55: // Rabbit
					case 56: //	Cat
						iGenLevel = 1;
						break;
					case 11: // Skeleton
					case 14: // Orc, Orc-Mage
					case 17: // Scorpion
					case 18: // Zombie
						iGenLevel = 2;
						break;
					case 12: // Stone-Golem
					case 23: // Clay-Golem
						iGenLevel = 3;
						break;
					case 27: // Hellbound
					case 61: // Rudolph
						iGenLevel = 4;
						break;
					case 72: // Claw-Turtle
					case 76: // Giant-Plant
					case 74: // Giant-Crayfish
					case 13: // Cyclops
					case 28: // Troll
					case 53: // Beholder
					case 60: // Cannibal-Plant
					case 62: // DireBoar
						iGenLevel = 5;
						break;
					case 29: // Orge
					case 33: // WereWolf
					case 48: // Stalker
					case 54: // Dark-Elf
					case 65: // Ice-Golem
					case 78: // Minotaurus
						iGenLevel = 6;
						break;
					case 70: // Balrogs
					case 71: // Centaurus
					case 30: // Liche
					case 63: // Frost
					case 79: // Nizie
					case 59: // Ettin
					case 75: // Lizards
						iGenLevel = 7;
						break;
					case 31: // Demon
					case 32: // Unicorn
					case 49: // Hellclaw
					case 50: // Tigerworm
					case 52: // Gagoyle
						iGenLevel = 8;
						break;
					case 58: // MountainGiant
						iGenLevel = 9;
						break;
					case 77: // MasterMage-Orc
						iGenLevel = 10;
						break;
				}
				if (iGenLevel == 0) return;
				// Weapon Drop:
				// 1.4% chance Valuable Drop 60% that it is a Weapon
				if (iDice(1, 10000) <= 4000) {
					if (iDice(1, 10000) <= 6000) {
						// 70% the Weapon is Melee
						switch (iGenLevel) {
							case 1: // Slime, Giant-Ant, Amphis, Rabbit, Cat
								switch (iDice(1, 3)) {
									case 1: iItemID = 1;
										break; // Dagger
									case 2: iItemID = 8;
										break; // ShortSword
									case 3: iItemID = 59;
										break; // LightAxe
								}
								break;
							case 2: // Skeleton, Orc, Orc-Mage, Scorpion, Zombie
								switch (iDice(1, 6)) {
									case 1: iItemID = 12;
										break; // MainGauche
									case 2: iItemID = 15;
										break; // Gradius
									case 3: iItemID = 65;
										break; // SexonAxe
									case 4: iItemID = 62;
										break; // Tomahoc
									case 5: iItemID = 23;
										break; // Sabre
									case 6: iItemID = 31;
										break; // Esterk
								}
								break;
							case 3: // Stone-Golem, Clay-Golem
								switch (iDice(1, 4)) {
									case 1: iItemID = 17;
										break; // LongSword
									case 2: iItemID = 68;
										break; // DoubleAxe
									case 3: iItemID = 23;
										break; // Sabre
									case 4: iItemID = 31;
										break; // Esterk
								}
								break;
							case 4: // Hellbound, Rudolph
								switch (iDice(1, 2)) {
									case 1: iItemID = 25;
										break; // Scimitar
									case 2: iItemID = 31;
										break; // Esterk
								}
								break;
							case 5: // Cyclops, Troll, Beholder, Cannibal-Plant, DireBoar
								switch (iDice(1, 3)) {
									case 1: iItemID = 31;
										break; // Esterk
									case 2: iItemID = 34;
										break; // Rapier
									case 3: iItemID = 71;
										break; // WarAxe
								}
								break;
							case 6: // Orge, WereWolf, Stalker, Dark-Elf, Ice-Golem
								switch (iDice(1, 3)) {
									case 1: iItemID = 617;
										break; // CompositeBow
									case 2: iItemID = 615;
										break; // Giant Sword
									case 3: iItemID = 560;
										break; // Battle Axe
								}
								break;
							case 7: // Liche, Frost
								switch (iDice(1, 2)) {
									case 1: iItemID = 54;
										break; // Flamebergr
									case 2: iItemID = 761;
										break; // Battle Hammer
								}
								break;
							case 8: // Demon, Unicorn, Hellclaw, Tigerworm, Gagoyle
								switch (iDice(1, 3)) {
									case 1: iItemID = 844;
										break; // Black Shadow Sword
									case 2: iItemID = 843;
										break; // Barbarian Hammer
									case 3: iItemID = 560;
										break; // BattleAxe
								}
								break;
							case 9: // MountainGiant
								switch (iDice(1, 6)) {
									case 1: iItemID = 23;
										break; // Sabre
									case 2: iItemID = 25;
										break; // Scimitar
									case 3: iItemID = 28;
										break; // Falchion
									case 4: iItemID = 31;
										break; // Esterk
									case 5: iItemID = 34;
										break; // Rapier
									case 6: iItemID = 760;
										break; // Hammer
								}
								break;
							case 10: // Ettin
								switch (iDice(1, 4)) {
									case 1: iItemID = 844;
										break; // Black Shadow Sword
									case 2: iItemID = 761;
										break; // GiantHammer
									case 3: iItemID = 843;
										break; // Barbarian Hammer
									case 4: iItemID = 560;
										break; // BattleAxe
								}
								break;
						}
					} else {
						// 30% the weapon is a Wand
						switch (iGenLevel) {
							case 2:
							case 3:
								iItemID = 258;
								break; // MagicWand(MS0)
							case 4:
							case 5:
							case 6:
								iItemID = 257;
								break; // MagicWand(MS10)
							case 7:
							case 8:
								iItemID = 256;
								break; // MagicWand(MS20) //Changed 03-03-12
							case 9:
							case 10:
								break;
						}
					}
				} else {
					// 1.4% chance Valuable Drop 40% that drop is an Armor/Shield
					switch (iGenLevel) {
						case 1: // Slime, Giant-Ant, Amphis, Rabbit, Cat
						case 2: // Skeleton, Orc, Orc-Mage, Scorpion, Zombie
							switch (iDice(1, 2)) {
								case 1: iItemID = 79;
									break; // WoodShield
								case 2: iItemID = 81;
									break; // TargeShield
							}
							break;
						case 3: switch (iDice(1, 2)) {// Stone-Golem, Clay-Golem
								case 1: iItemID = 450;
									break; // Shoes
								case 2: iItemID = 402;
									break; // Cape
							}
							break;
						case 4: // Hellbound, Rudolph
							switch (iDice(1, 3)) {
								case 1: iItemID = 450; // Shoes
								case 2: iItemID = 402; // Cape
								case 3: iItemID = 84;
									break; // IronShield
							}
							break;
						case 5: // Cyclops, Troll, Beholder, Cannibal-Plant, DireBoar
							switch (iDice(1, 12)) {
								case 1: iItemID = 916;
									break; // Dream Plate (M)
								case 2: iItemID = 917;
									break; // Dream Plate (W
								case 3: iItemID = 918;
									break; // Dream Leggings(M)
								case 4: iItemID = 919;
									break; // Dream Leggings(W)
								case 5: iItemID = 920;
									break; // Dream Hauberk(M)
								case 6: iItemID = 921;
									break; // Dream Hauberk(W)
								case 7: iItemID = 922;
									break; // Dream Hauberk(M)
								case 8: iItemID = 923;
									break; // Dream Hauberk(W)
								case 9: iItemID = 924;
									break; // Dream Helm (M)
								case 10: iItemID = 925;
									break; // Dream Helm (W)
								case 11: iItemID = 926;
									break; // Dream Helm (M)
								case 12: iItemID = 927;
									break; // Dream Helm (W)
							}
							break;
						case 6: // Orge, WereWolf, Stalker, Dark-Elf, Ice-Golem
							switch (iDice(1, 12)) {
								case 1: iItemID = 934;
									break; // ExtasisMail(M)
								case 2: iItemID = 935;
									break; // ExtasisMail(W)
								case 3: iItemID = 928;
									break; // ExtasisPlateMail(M)
								case 4: iItemID = 929;
									break; // ExtasisPlateMail(W)
								case 5: iItemID = 938;
									break; // ExtasisWings-Helm(M)
								case 6: iItemID = 939;
									break; // ExtasisWings-Helm(W)
								case 7: iItemID = 936;
									break; // Wizard-Hat(M)
								case 8: iItemID = 937;
									break; // Wizard-Hat(W)
								case 9: iItemID = 932;
									break; // ExtasisHauberk(M)
								case 10: iItemID = 933;
									break; // ExtasisHauberk(W)
								case 11: iItemID = 930;
									break; // ExtasisChainHose(M)
								case 12: iItemID = 931;
									break; // ExtasisChainHose(W)
							}
							break;
						case 7: // Liche, Frost, Ettin & Lizards
							switch (iDice(1, 12)) {
								case 1: iItemID = 944;
									break; // RuneHauberk(M)
								case 2: iItemID = 945;
									break; // RuneHauberk(W)
								case 3: iItemID = 940;
									break; // RunePlateMail(M)
								case 4: iItemID = 941;
									break; // RunePlateMail(W)
								case 5: iItemID = 942;
									break; // RuneLeggings(M)
								case 6: iItemID = 943;
									break; // RuneLeggings(W)
								case 7: iItemID = 948;
									break; // RuneHat(M)
								case 8: iItemID = 949;
									break; // RuneHat(M)
								case 9: iItemID = 950;
									break; // RuneFull-Helm(M)
								case 10: iItemID = 951;
									break; // RuneFull-Helm(M)
								case 11: iItemID = 946;
									break; //Rune Chain Mail(M)
								case 12: iItemID = 947;
									break; // Rune Chain Mail (W)
							}
							break;
						case 8: switch (iDice(1, 2)) {// Demon, Unicorn, Hellclaw, Tigerworm, Gagoyle
								case 1: iItemID = 402; // Cape
								case 2: iItemID = 450; //Shoes.
							}
							break;
						case 9: switch (iDice(1, 12)) { //M-G only.
								case 1: iItemID = 944;
									break; // RuneHauberk(M)
								case 2: iItemID = 945;
									break; // RuneHauberk(W)
								case 3: iItemID = 940;
									break; // RunePlateMail(M)
								case 4: iItemID = 941;
									break; // RunePlateMail(W)
								case 5: iItemID = 942;
									break; // RuneLeggings(M)
								case 6: iItemID = 943;
									break; // RuneLeggings(W)
								case 7: iItemID = 948;
									break; // RuneHat(M)
								case 8: iItemID = 949;
									break; // RuneHat(M)
								case 9: iItemID = 950;
									break; // RuneFull-Helm(M)
								case 10: iItemID = 951;
									break; // RuneFull-Helm(M)
								case 11: iItemID = 946;
									break; //Rune Chain Mail(M)
								case 12: iItemID = 947;
									break; // Rune Chain Mail (W)
							}
							break;
						case 10: switch (iDice(1, 12)) { //Master Mage ORc..
								case 1: iItemID = 944;
									break; // RuneHauberk(M)
								case 2: iItemID = 945;
									break; // RuneHauberk(W)
								case 3: iItemID = 940;
									break; // RunePlateMail(M)
								case 4: iItemID = 941;
									break; // RunePlateMail(W)
								case 5: iItemID = 942;
									break; // RuneLeggings(M)
								case 6: iItemID = 943;
									break; // RuneLeggings(W)
								case 7: iItemID = 948;
									break; // RuneHat(M)
								case 8: iItemID = 949;
									break; // RuneHat(M)
								case 9: iItemID = 950;
									break; // RuneFull-Helm(M)
								case 10: iItemID = 951;
									break; // RuneFull-Helm(M)
								case 11: iItemID = 946;
									break; //Rune Chain Mail(M)
								case 12: iItemID = 947;
									break; // Rune Chain Mail (W)
							}
							break;
					}
				}
				pItem = new class CItem;
				if (_bInitItemAttr(*pItem, iItemID) == false) {
					delete pItem;
					return;
				}
				if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
					iResult = iDice(1, 10000);
					if ((iResult >= 1) && (iResult <= 299)) {
						dwType = 6;
						cColor = 2;
					} else if ((iResult >= 300) && (iResult <= 999)) {
						dwType = 8;
						cColor = 3;
					} else if ((iResult >= 1000) && (iResult <= 2499)) {
						dwType = 1;
						cColor = 5;
					} else if ((iResult >= 2500) && (iResult <= 4499)) {
						dwType = 5;
						cColor = 1;
					} else if ((iResult >= 4500) && (iResult <= 6499)) {
						dwType = 3;
						cColor = 7;
					} else if ((iResult >= 6500) && (iResult <= 8099)) {
						dwType = 2;
						cColor = 4;
					} else if ((iResult >= 8100) && (iResult <= 9699)) {
						dwType = 7;
						cColor = 6;
					} else if ((iResult >= 9700) && (iResult <= 10000)) {
						dwType = 9;
						cColor = 8;
					}
					pItem->m_cItemColor = cColor;
					iResult = iDice(1, 30000);
					if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
					else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
					else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 3; // 4356/29348 = 14.8%
					else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
					else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 5; // 1897/29348 = 6.4%
					else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 6; // 1252/29348 = 4.2%
					else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 7; // 826/29348 = 2.8%
					else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
					else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 9; // 360/29348 = 1.2%
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
					if ((iGenLevel <= 2) && (dwValue > 7)) dwValue = 7;
					pItem->m_dwAttribute = 0;
					dwType = dwType << 20;
					dwValue = dwValue << 16;
					pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					if (iDice(1, 10000) >= 6000) {
						iResult = iDice(1, 10000);
						if ((iResult >= 1) && (iResult <= 4999)) dwType = 2;
						else if ((iResult >= 5000) && (iResult <= 8499)) dwType = 10;
						else if ((iResult >= 8500) && (iResult <= 9499)) dwType = 12;
						else if ((iResult >= 9500) && (iResult <= 10000)) dwType = 11;
						iResult = iDice(1, 30000);
						if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
						else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
						else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 3; // 4356/29348 = 14.8%
						else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
						else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 5; // 1897/29348 = 6.4%
						else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 6; // 1252/29348 = 4.2%
						else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 7; // 826/29348 = 2.8%
						else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
						else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 9; // 360/29348 = 1.2%
						else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 10; // 237/29348 = 0.8%
						else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 11; // 156/29348 = 0.5%
						else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 12; // 103/29348 = 0.3%
						else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 13; // 68/29348 = 0.1%
						else dwValue = 1; // v2.03 906
						switch (dwType) {
							case 2:
								if (dwValue <= 3) dwValue = 3;
								break;
							case 10:
								if (dwValue > 7) dwValue = 7;
								break;
							case 11:
								dwValue = 2;
								break;
							case 12:
								dwValue = 5;
								break;
						}
						if ((iGenLevel <= 2) && (dwValue > 7)) dwValue = 7;
						dwType = dwType << 12;
						dwValue = dwValue << 8;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					}
				} else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE) {
					dwType = 10;
					cColor = 5;
					pItem->m_cItemColor = cColor;
					iResult = iDice(1, 30000);
					if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
					else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
					else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 3; // 4356/29348 = 14.8%
					else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
					else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 5; // 1897/29348 = 6.4%
					else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 6; // 1252/29348 = 4.2%
					else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 7; // 826/29348 = 2.8%
					else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
					else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 9; // 360/29348 = 1.2%
					else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 10; // 237/29348 = 0.8%
					else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 11; // 156/29348 = 0.5%
					else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 12; // 103/29348 = 0.3%
					else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 13; // 68/29348 = 0.1%
					else dwValue = 1; // v2.03 906
					if ((iGenLevel <= 2) && (dwValue > 7)) dwValue = 7;
					pItem->m_dwAttribute = 0;
					dwType = dwType << 20;
					dwValue = dwValue << 16;
					pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					if (iDice(1, 10000) >= 6000) {
						iResult = iDice(1, 10000);
						if ((iResult >= 1) && (iResult <= 4999)) dwType = 2;
						else if ((iResult >= 5000) && (iResult <= 8499)) dwType = 10;
						else if ((iResult >= 8500) && (iResult <= 9499)) dwType = 12;
						else if ((iResult >= 9500) && (iResult <= 10000)) dwType = 11;
						iResult = iDice(1, 30000);
						if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
						else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
						else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 3; // 4356/29348 = 14.8%
						else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
						else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 5; // 1897/29348 = 6.4%
						else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 6; // 1252/29348 = 4.2%
						else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 7; // 826/29348 = 2.8%
						else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
						else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 9; // 360/29348 = 1.2%
						else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 10; // 237/29348 = 0.8%
						else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 11; // 156/29348 = 0.5%
						else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 12; // 103/29348 = 0.3%
						else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 13; // 68/29348 = 0.1%
						else dwValue = 1; // v2.03 906
						if ((iGenLevel <= 2) && (dwValue > 7)) dwValue = 7;
						switch (dwType) {
							case 2:
								if (dwValue <= 3) dwValue = 3;
								break;
							case 10:
								if (dwValue > 7) dwValue = 7;
								break;
							case 11:
								dwValue = 2;
								break;
							case 12:
								dwValue = 5;
								break;
						}
						dwType = dwType << 12;
						dwValue = dwValue << 8;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					}
				} else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE) {
					iResult = iDice(1, 10000);
					if ((iResult >= 1) && (iResult <= 5999)) dwType = 8;
					else if ((iResult >= 6000) && (iResult <= 8999)) dwType = 6;
					else if ((iResult >= 9000) && (iResult <= 9554)) dwType = 11; //dwType = 11;
					else if ((iResult >= 9555) && (iResult <= 10000)) dwType = 12; //dwType = 12;
					iResult = iDice(1, 30000);
					if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
					else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
					else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 3; // 4356/29348 = 14.8%
					else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
					else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 5; // 1897/29348 = 6.4%
					else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 6; // 1252/29348 = 4.2%
					else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 7; // 826/29348 = 2.8%
					else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
					else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 9; // 360/29348 = 1.2%
					else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 10; // 237/29348 = 0.8%
					else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 11; // 156/29348 = 0.5%
					else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 12; // 103/29348 = 0.3%
					else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 13; // 68/29348 = 0.1%
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
							if ((iGenLevel <= 3) && (dwValue > 2)) dwValue = 2;
							break;
					}
					if ((iGenLevel <= 2) && (dwValue > 7)) dwValue = 7;
					pItem->m_dwAttribute = 0;
					dwType = dwType << 20;
					dwValue = dwValue << 16;
					pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					if (iDice(1, 10000) >= 6000) {
						iResult = iDice(1, 10000);
						if ((iResult >= 1) && (iResult <= 999)) dwType = 3;
						else if ((iResult >= 1000) && (iResult <= 3999)) dwType = 1;
						else if ((iResult >= 4000) && (iResult <= 5499)) dwType = 5;
						else if ((iResult >= 5500) && (iResult <= 6499)) dwType = 4;
						else if ((iResult >= 6500) && (iResult <= 7499)) dwType = 6;
						else if ((iResult >= 7500) && (iResult <= 9399)) dwType = 7;
						else if ((iResult >= 9400) && (iResult <= 9799)) dwType = 8;
						else if ((iResult >= 9800) && (iResult <= 10000)) dwType = 9;
						iResult = iDice(1, 30000);
						if ((iResult >= 1) && (iResult < 10000)) dwValue = 1; // 10000/29348 = 34%
						else if ((iResult >= 10000) && (iResult < 17400)) dwValue = 2; // 6600/29348 = 22.4%
						else if ((iResult >= 17400) && (iResult < 22400)) dwValue = 3; // 4356/29348 = 14.8%
						else if ((iResult >= 22400) && (iResult < 25400)) dwValue = 4; // 2874/29348 = 9.7%
						else if ((iResult >= 25400) && (iResult < 27400)) dwValue = 5; // 1897/29348 = 6.4%
						else if ((iResult >= 27400) && (iResult < 28400)) dwValue = 6; // 1252/29348 = 4.2%
						else if ((iResult >= 28400) && (iResult < 28900)) dwValue = 7; // 826/29348 = 2.8%
						else if ((iResult >= 28900) && (iResult < 29300)) dwValue = 8; // 545/29348 = 1.85%
						else if ((iResult >= 29300) && (iResult < 29600)) dwValue = 9; // 360/29348 = 1.2%
						else if ((iResult >= 29600) && (iResult < 29800)) dwValue = 10; // 237/29348 = 0.8%
						else if ((iResult >= 29800) && (iResult < 29900)) dwValue = 11; // 156/29348 = 0.5%
						else if ((iResult >= 29900) && (iResult < 29970)) dwValue = 12; // 103/29348 = 0.3%
						else if ((iResult >= 29970) && (iResult <= 30000)) dwValue = 13; // 68/29348 = 0.1%
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
						if ((iGenLevel <= 2) && (dwValue > 7)) dwValue = 7;
						dwType = dwType << 12;
						dwValue = dwValue << 8;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					}
				}
				_AdjustRareItemValue(*pItem);
			}
		}
		pItem->m_sTouchEffectType = DEF_ITET_ID;
		pItem->m_sTouchEffectValue1 = iDice(1, 100000);
		pItem->m_sTouchEffectValue2 = iDice(1, 100000);
		//pItem->m_sTouchEffectValue3 = timeGetTime();
		SYSTEMTIME SysTime;
		char cTemp[256];
		GetLocalTime(&SysTime);
		std::memset(cTemp, 0, sizeof (cTemp));
		wsprintf(cTemp, "%d%2d", (short) SysTime.wMonth, (short) SysTime.wDay);
		pItem->m_sTouchEffectValue3 = atoi(cTemp);
		m_pNpcList[iNpcH]->map_->bSetItem(m_pNpcList[iNpcH]->m_sX,
				  m_pNpcList[iNpcH]->m_sY,
				  pItem);
		m_pNpcList[iNpcH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, 
				  m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, 0, pItem);
	}
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

void CGame::AdminOrder_Time(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	int i;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelTime) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			wsprintf(G_cTxt, "(%s) Admin(%s) Order: Forces night mode", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
		} else if (token[0] == '1') {
			m_cDayOrNight = 1;
			wsprintf(G_cTxt, "(%s) Admin(%s) Order: Forces day mode", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
			bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
		} else if (token[0] == '0') {
			wsprintf(G_cTxt, "(%s) Admin(%s) Order: Disables force time", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
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

void CGame::AdminOrder_CheckRep(int iClientH, char *pData, uint32_t dwMsgSize) {
	class CStrTok * pStrTok;
	const char * token;
	char cName[11], cTargetName[11], cRepMessage[256], cTemp[256], seps[] = "= \t\n", cBuff[256];
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	std::memset(cTemp, 0, sizeof (cTemp));
	std::memset(cRepMessage, 0, sizeof (cRepMessage));
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 6) {
		wsprintf(cRepMessage, "You have %d reputation points.", m_pClientList[iClientH]->m_iRating);
		//ShowClientMsg(iClientH, cRepMessage);
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cRepMessage);
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
					ShowClientMsg(iClientH, cRepMessage);
				}
		}
	}
}

void CGame::AdminOrder_Pushplayer(int iClientH, char * pData, uint32_t dwMsgSize) {
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
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelPushPlayer) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				if (bFlag == true)
					//Reqeust the Push/Teleport
					m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, dX, dY);
				wsprintf(G_cTxt, "(%s) GM(%s) sends (%s) Player(%s) to [%s](%d,%d)", m_pClientList[iClientH]->m_cIPaddress,
						  m_pClientList[iClientH]->m_cCharName, m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName, cMapName, dX, dY);
				bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
				delete pStrTok;
				return;
			}
	}
}

/*void CGame::AdminOrder_SummonGuild(int iClientH, char *pData, uint32_t dwMsgSize)
{
 char   seps[] = "= \t\n";
 char   * token, * cp, cBuff[256], cMapName[11], char cGuildName[20];
 uint16_t   *wp;
 int    pX, pY, i;
 class  CStrTok * pStrTok;
 uint32_t  dwGoldCount;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelSummonGuild) {
		
		m_pClientList[iClientH]->SendNotifyMsg(0,DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
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
	pX = m_pClientList[iClientH]->m_sX;
	pY = m_pClientList[iClientH]->m_sY;
	//memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);
	memcpy(cMapName, m_pClientList[iClientH]->map_->m_cName, 11);
	if (strlen(token) > 20)
			memcpy(cGuildName, token, 20);
		else memcpy(cGuildName, token, strlen(token));
	for (i = 0; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != nullptr) && (strcmp(m_pClientList[i]->m_cGuildName, token) == 0)) {
			m_pClientList[i]->RequestTeleportHandler("2   ", cMapName, pX, pY);
	}//m_pClientList[i]->m_cCharName
	wsprintf(G_cTxt,"GM Order(%s): PC(%s) Summoned to (%s)", m_pClientList[iClientH]->m_cGuildName, cGuildName, cMapName);
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
	int i, z, x;
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
	for (i = 0; i < DEF_MAXMAPS; i++)
		if ((m_pMapList[i] != nullptr) && (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)) {
			wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map (%s) is already installed! cannot add.", cTmpName);
			PutLogList(cTxt);
			return false;
		}
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] == nullptr) {
			m_pMapList[i] = std::make_shared<CMap>(i, this);
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

void CGame::AdminOrder_CheckStats(int iClientH, char *pData, uint32_t dwMsgSize) {
	char cStatMessage[256];
	char seps[] = "= \t\n";
	char cBuff[256];
	class CStrTok * pStrTok;
	const char * token;
	char cName[11], cTargetName[11];
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 1) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
				ShowClientMsg(iClientH, cStatMessage);
			}
	}
}

bool CGame::bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, bool bBonus) {
	int iValue, iProb, iResult;
	if (m_pClientList[iClientH]->m_pItemList[iSomH] == nullptr) return false;
	iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
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
	if (((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 > 100)) {
		if (iProb > 20)
			iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 10);
		else if (iProb > 7)
			iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 20);
		else
			iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 40);
	}
	if (bBonus == true) iProb *= 2;
	iProb *= 100;
	iResult = iDice(1, 10000);
	if (iProb >= iResult) {
		_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int) - 1, &*m_pClientList[iClientH]->m_pItemList[iItemIndex]);
		return true;
	}
	_bItemLog(DEF_ITEMLOG_UPGRADEFAIL, iClientH, (int) - 1, &*m_pClientList[iClientH]->m_pItemList[iItemIndex]);
	return false;
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

void CGame::AdminOrder_CleanMap(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	class CStrTok * pStrTok;
	char * token, cMapName[11], cBuff[256];
	bool bFlag = false; //Used to check if we are on the map we wanna clear
	int i;
	CItem *pItem;
	short sRemainItemSprite, sRemainItemSpriteFrame, dX, dY;
	char cRemainItemColor, len;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelCleanMap) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
					if (memcmp(m_pClientList[iClientH]->map_->m_cName, cMapName, len) != 0) return;
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

void CGame::ShowClientMsg(int iClientH, const char* pMsg) {
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
	m_pClientList[iClientH]->m_pXSock->iSendMsg(cTemp, dwMsgSize + 22);
}

void CGame::Command_YellowBall(int iClientH, char* pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cPlayerName[11], cMapName[32];
	class CStrTok * pStrTok;
	int iSoxH, iSoX, i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {
			switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
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
					if ((m_pClientList[iClientH]->m_cLocation) != (m_pClientList[i]->m_cLocation)) return;
					std::memset(cMapName, 0, sizeof (cMapName));
					strcpy(cMapName, m_pClientList[i]->map_->m_cName);
					wsprintf(G_cTxt, "(%s) Player: (%s) - YellowBall MapName(%s)(%d %d)", m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName, m_pClientList[i]->map_->m_cName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					PutItemLogFileList(G_cTxt);
					m_pClientList[iClientH]->ItemDepleteHandler(iSoxH, true, true);
					m_pClientList[iClientH]->RequestTeleportHandler("2   ", cMapName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					delete pStrTok;
					return;
				}
			}
		}
		m_pClientList[iClientH]->m_bIsAdminOrderGoto = true;
		delete pStrTok;
	}
}

void CGame::Command_RedBall(int iClientH, char */*pData*/, uint32_t /*dwMsgSize*/) {
	char cName[21], cNpcName[21], cNpcWaypoint[11];
	register int iNamingValue;
	int tX, tY, i, x, iNpcID;
	int iSoxH, iSoX;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone1", 9) != 0) &&
			  (memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone2", 9) != 0) &&
			  (memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone3", 9) != 0) &&
			  (memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone4", 9) != 0)) return;
	iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {
			switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
				case 652: iSoX++;
					iSoxH = i;
					break;
			}
		}
	if (iSoX > 0) {
		iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
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
			cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
			std::memset(cNpcWaypoint, 0, sizeof (cNpcWaypoint));
			tX = (int) m_pClientList[iClientH]->m_sX;
			tY = (int) m_pClientList[iClientH]->m_sY;
			if (bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, (rand() % 9),
					  DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, nullptr, 0, -1, false, false) == false) {
				m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				wsprintf(G_cTxt, "(%s) Player: (%s) - RedBall used %s [%s(%d, %d)] ", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, cNpcName, m_pClientList[iClientH]->map_->m_cName, tX, tY);
				PutItemLogFileList(G_cTxt);
			}
		}
		for (x = 1; x < DEF_MAXCLIENTS; x++)
			if ((m_pClientList[x] != nullptr) && (m_pClientList[x]->m_bIsInitComplete == true)) {
				m_pClientList[x]->SendNotifyMsg(0, DEF_NOTIFY_SPAWNEVENT, tX, tY, iNpcID, nullptr, 0, 0);
			}
		m_pClientList[iClientH]->ItemDepleteHandler(iSoxH, true, true);
	}
}

void CGame::Command_BlueBall(int iClientH, char */*pData*/, uint32_t /*dwMsgSize*/) {
	char cName_Master[10], cName_Slave[10], cNpcName[256], cWaypoint[11], cSA;
	int pX, pY, j, iNum, iNamingValue, iNpcID;
	register int i, x;
	bool bMaster;
	int iSoxH, iSoX;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone1", 9) != 0) &&
			  (memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone2", 9) != 0) &&
			  (memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone3", 9) != 0) &&
			  (memcmp(m_pClientList[iClientH]->map_->m_cName, "huntzone4", 9) != 0)) return;
	iSoX = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {
			switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
				case 654: iSoX++;
					iSoxH = i;
					break;
			}
		}
	if (iSoX > 0) {
		iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
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
			pX = m_pClientList[iClientH]->m_sX;
			pY = m_pClientList[iClientH]->m_sY;
			/*std::memset(cBallMessage, 0, sizeof(cBallMessage));
			wsprintf(cBallMessage, "BlueBall event-%s %s(%d,%d)", cNpcName, m_pClientList[iClientH]->map_->m_cName, pX, pY);
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[iClientH]->m_cLocation) == (m_pClientList[i]->m_cLocation))
					ShowClientMsg(i, cBallMessage);*/
			wsprintf(G_cTxt, "(!) BlueBallEvent: SummonMob (%s)-(%d)", cNpcName, iNum);
			PutLogList(G_cTxt);
			iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
			if (iNamingValue != -1) {
				std::memset(cName_Master, 0, sizeof (cName_Master));
				wsprintf(cName_Master, "XX%d", iNamingValue);
				cName_Master[0] = '_';
				cName_Master[1] = m_pClientList[iClientH]->map_->id_ + 65;
				if ((bMaster = bCreateNewNpc(cNpcName, cName_Master, m_pClientList[iClientH]->map_->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false, true)) == false) {
					m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
				}
			}
			for (j = 0; j < (iNum - 1); j++) {
				iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
				if (iNamingValue != -1) {
					// Slave Mob
					std::memset(cName_Slave, 0, sizeof (cName_Slave));
					wsprintf(cName_Slave, "XX%d", iNamingValue);
					cName_Slave[0] = '_';
					cName_Slave[1] = m_pClientList[iClientH]->map_->id_ + 65;
					if (bCreateNewNpc(cNpcName, cName_Slave, m_pClientList[iClientH]->map_->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, nullptr, 0, -1, false, false, false) == false) {
						m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
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
	m_pClientList[iClientH]->ItemDepleteHandler(iSoxH, true, true);
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
	if (m_bIsApocalypseMode == true) return;
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
	if (m_bIsApocalypseMode == false) return;
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

void CGame::RequestCreatePartyHandler(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0 && m_pClientList[iClientH]->m_iAdminUserLevel < 3) return; // v2.23 22/04/06 15:24 AdminSecurity 
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL) {
		return;
	}
	m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
}

void CGame::PartyOperationResultHandler(char *pData) {
	char * cp, cResult, cName[12];
	uint16_t * wp;
	int i, iClientH, iPartyID, iTotal;
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
			PartyOperationResult_Create(iClientH, cName, cResult, iPartyID);
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
			if (m_pClientList[iClientH] == nullptr) return;
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
			PartyOperationResult_Join(iClientH, cName, cResult, iPartyID);
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
			PartyOperationResult_Info(iClientH, cName, iTotal, cp);
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
			PartyOperationResult_Dismiss(iClientH, cName, cResult, iPartyID);
			break;
	}
}

void CGame::PartyOperationResult_Create(int iClientH, char *pName, int iResult, int iPartyID) {
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
	switch (iResult) {
		case 0:
			if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
			m_pClientList[iClientH]->m_iPartyID = 0;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 1, 0, 0, nullptr);
			break;
		case 1:
			if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
			m_pClientList[iClientH]->m_iPartyID = iPartyID;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 1, 1, 0, nullptr);
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) {
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = iClientH;
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers++;
					goto PORC_LOOPBREAK1;
				}
PORC_LOOPBREAK1:
			;
			if ((m_pClientList[iClientH]->m_iReqJoinPartyClientH != 0) && (strlen(m_pClientList[iClientH]->m_cReqJoinPartyName) != 0)) {
				m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
				std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
			}
			break;
	}
}

void CGame::PartyOperationResult_Join(int iClientH, char *pName, int iResult, int iPartyID) {
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	switch (iResult) {
		case 0:
			if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
			m_pClientList[iClientH]->m_iPartyID = 0;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 4, 0, 0, pName);
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
			std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
			break;
		case 1:
			if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
			if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
			m_pClientList[iClientH]->m_iPartyID = iPartyID;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 4, 1, 0, pName);
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
			std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) {
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = iClientH;
					m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers++;
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

void CGame::PartyOperationResult_Dismiss(int iClientH, char *pName, int iResult, int iPartyID) {
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
				if (m_pClientList[iClientH] != nullptr) {
					m_pClientList[iClientH]->m_iPartyID = 0;
					m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
					m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((m_pClientList[i] != nullptr) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
						m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 6, 1, 0, pName);
					}
				return;
			}
			if ((m_pClientList[iClientH] != nullptr) && (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)) return;
			if ((m_pClientList[iClientH] != nullptr) && (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0)) return;
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
			if (m_pClientList[iClientH] != nullptr) {
				m_pClientList[iClientH]->m_iPartyID = 0;
				m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
			}
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

void CGame::RequestJoinPartyHandler(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256], cName[12];
	class CStrTok * pStrTok;
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel > 0 && m_pClientList[iClientH]->m_iAdminUserLevel < 3) return; // v2.23 22/04/06 15:24 AdminSecurity 
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
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
	delete pStrTok;
}

void CGame::RequestDismissPartyHandler(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;
	m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
}

void CGame::GetPartyInfoHandler(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;
}

void CGame::PartyOperationResult_Info(int iClientH, char * pName, int iTotal, char *pNameList) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 5, 1, iTotal, pNameList);
}

void CGame::RequestDeletePartyHandler(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iPartyID != 0) {
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
	}
}

void CGame::RequestAcceptJoinPartyHandler(int iClientH, int iResult) {
	int iH;
	if (m_pClientList[iClientH] == nullptr) return;
	switch (iResult) {
		case 0:
			iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
			if (m_pClientList[iH] == nullptr) {
				return;
			}
			if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0) {
				return;
			}
			if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
				return;
			}
			if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0)) {
				return;
			}
			m_pClientList[iH]->SendNotifyMsg(0, DEF_NOTIFY_PARTY, 7, 0, 0, nullptr);
			m_pClientList[iH]->m_iPartyID = 0;
			m_pClientList[iH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iH]->m_iReqJoinPartyClientH = 0;
			std::memset(m_pClientList[iH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iH]->m_cReqJoinPartyName));
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
			std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
			break;
		case 1:
			if ((m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) && (m_pClientList[iClientH]->m_iPartyID != 0)) {
				iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
				if (m_pClientList[iH] == nullptr) {
					return;
				}
				if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0) {
					return;
				}
				if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
					return;
				}
				if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0)) {
					return;
				}
			} else {
				iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
				if (m_pClientList[iH] == nullptr) {
					return;
				}
				if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0) {
					return;
				}
				if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
					return;
				}
				if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0)) {
					return;
				}
				if (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_NULL) {
					RequestCreatePartyHandler(iClientH);
				} else {
				}
			}
			break;
		case 2:
			if ((m_pClientList[iClientH]->m_iPartyID != 0) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)) {
				RequestDismissPartyHandler(iClientH);
			} else {
				iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
				if ((m_pClientList[iH] != nullptr) && (m_pClientList[iH]->m_iReqJoinPartyClientH == iClientH) &&
						  (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) == 0)) {
					m_pClientList[iH]->m_iReqJoinPartyClientH = 0;
					std::memset(m_pClientList[iH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iH]->m_cReqJoinPartyName));
				}
				m_pClientList[iClientH]->m_iPartyID = 0;
				m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
				std::memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof (m_pClientList[iClientH]->m_cReqJoinPartyName));
			}
			break;
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

/*void CGame::Command_GreenBall(int iClientH, char * pData, uint32_t dwMsgSize)
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
	if (m_pClientList[iClientH]->m_pItemList[i] != nullptr) {
		switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
		case 652: iSoX++; iSoxH = i; break;
		}
	}
  dwTime = timeGetTime();
  if (m_pClientList[iClientH] == nullptr) return;
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
	m_pClientList[iClientH]->ItemDepleteHandler(itemeq, false, false);
}*/
void CGame::TimeHitPointsUp(int iClientH) {
	register int iMaxHP, iTemp, iTotal;
	double dV1, dV2, dV3;
	/*
	.text:0043A679                 mov     [ebp+m_pClientList], ecx
	.text:0043A67C                 mov     eax, [ebp+iClientH]
	.text:0043A67F                 mov     ecx, [ebp+m_pClientList]
	.text:0043A682                 cmp     dword ptr [ecx+eax*4+16Ch], 0 ; m_pClientList[iClientH] == 0
	.text:0043A68A                 jnz     short loc_43A691;
	.text:0043A68C                 jmp     loc_43A8F4;
	 */
	if (m_pClientList[iClientH] == nullptr) return;
	/*
	.text:0043A691                 mov     edx, [ebp+iClientH]
	.text:0043A694                 mov     eax, [ebp+m_pClientList]
	.text:0043A697                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
	.text:0043A69E                 cmp     dword ptr [ecx+2Ch], 0 ; m_bIsInitComplete == 0
	.text:0043A6A2                 jnz     short loc_43A6A9;
	.text:0043A6A4                 jmp     break loc_43A8F4;
	 */
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	/*
	.text:0043A6A9                 mov     edx, [ebp+iClientH]
	.text:0043A6AC                 mov     eax, [ebp+m_pClientList]
	.text:0043A6AF                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
	.text:0043A6B6                 cmp     dword ptr [ecx+0B6Ch], 0 ; m_iHungerStatus <= 0
	.text:0043A6BD                 jg      short loc_43A6C4
	.text:0043A6BF                 jmp     loc_43A8F4
	 */
	if (m_pClientList[iClientH]->m_iHungerStatus <= 0) return;
	/*
	.text:0043A6C4                 mov     edx, [ebp+iClientH]
	.text:0043A6C7                 mov     eax, [ebp+m_pClientList]
	.text:0043A6CA                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
	.text:0043A6D1                 cmp     dword ptr [ecx+0C8h], 1 ; m_bIsKilled == true
	.text:0043A6D8                 jnz     short loc_43A6DF
	.text:0043A6DA                 jmp     loc_43A8F4
	 */
	if (m_pClientList[iClientH]->m_bIsKilled == true) return;
	/*
	.text:0043A6DF                 mov     edx, [ebp+iClientH]
	.text:0043A6E2                 mov     eax, [ebp+m_pClientList]
	.text:0043A6E5                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
	.text:0043A6EC                 cmp     dword ptr [ecx+864h], 1 ; m_pClientList[i]->m_bSkillUsingStatus[19]
	.text:0043A6F3                 jnz     short loc_43A6FA
	.text:0043A6F5                 jmp     loc_43A8F4
	 */
	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;
	/*
	.text:0043A6FA                 mov     edx, [ebp+iClientH]
	.text:0043A6FD                 push    edx
	.text:0043A6FE                 mov     ecx, [ebp+m_pClientList]
	.text:0043A701                 call    CGame__iGetMaxHP
	.text:0043A706                 mov     [ebp+iMaxHP], eax
	 */
	iMaxHP = m_pClientList[iClientH]->iGetMaxHP();
	/*
	.text:0043A709                 mov     eax, [ebp+iClientH]
	.text:0043A70C                 mov     ecx, [ebp+m_pClientList]
	.text:0043A70F                 mov     edx, [ecx+eax*4+16Ch]
	.text:0043A716                 mov     eax, [edx+0ACh] ; m_iHP < iMaxHP
	.text:0043A71C                 cmp     eax, [ebp+iMaxHP]
	.text:0043A71F                 jge     loc_43A8DD
	 */
	if (m_pClientList[iClientH]->m_iHP < iMaxHP) {
		/*
		.text:0043A725                 mov     ecx, [ebp+iClientH]
		.text:0043A728                 mov     edx, [ebp+m_pClientList]
		.text:0043A72B                 mov     eax, [edx+ecx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A732                 mov     ecx, [eax+120h] ; m_iVit
		.text:0043A738                 push    ecx
		.text:0043A739                 push    1
		.text:0043A73B                 mov     ecx, [ebp+m_pClientList]
		.text:0043A73E                 call    CGame__iDice
		.text:0043A743                 mov     [ebp+iTemp], eax
		 */
		iTemp = iDice(1, (m_pClientList[iClientH]->m_iVit));
		/*
		.text:0043A746                 mov     edx, [ebp+iClientH]
		.text:0043A749                 mov     eax, [ebp+m_pClientList]
		.text:0043A74C                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A753                 mov     eax, [ecx+120h] ; m_iVit
		.text:0043A759                 cdq
		.text:0043A75A                 sub     eax, edx
		.text:0043A75C                 sar     eax, 1
		.text:0043A75E                 cmp     [ebp+iTemp], eax
		.text:0043A761                 jge     short loc_43A77E
		.text:0043A763                 mov     edx, [ebp+iClientH]
		.text:0043A766                 mov     eax, [ebp+m_pClientList]
		.text:0043A769                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A770                 mov     eax, [ecx+120h] ; m_iVit
		.text:0043A776                 cdq
		.text:0043A777                 sub     eax, edx
		.text:0043A779                 sar     eax, 1
		.text:0043A77B                 mov     [ebp+iTemp], eax
		 */
		if (iTemp < (m_pClientList[iClientH]->m_iVit / 2)) iTemp = (m_pClientList[iClientH]->m_iVit / 2);
		/*
		.text:0043A77E                 mov     edx, [ebp+iClientH]
		.text:0043A781                 mov     eax, [ebp+m_pClientList]
		.text:0043A784                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A78B                 cmp     dword ptr [ecx+0C1Ch], 0 ; m_iSideEffect_MaxHPdown != 0
		.text:0043A792                 jz      short loc_43A7B3
		 */
		if (m_pClientList[iClientH]->m_iSideEffect_MaxHPdown != 0)
			/*
			.text:0043A7A5                 idiv    dword ptr [ecx+0C1Ch] ; m_iSideEffect_MaxHPdown
			.text:0043A7AB                 mov     edx, [ebp+iTemp] ; (iTemp / m_iSideEffect_MaxHPdown)
			.text:0043A7AE                 sub     edx, eax
			.text:0043A7B0                 mov     [ebp+iTemp], edx ; iTemp =
			 */
			iTemp -= (iTemp / m_pClientList[iClientH]->m_iSideEffect_MaxHPdown);
		/*
		.text:0043A7B3                 mov     eax, [ebp+iClientH]
		.text:0043A7B6                 mov     ecx, [ebp+m_pClientList]
		.text:0043A7B9                 mov     edx, [ecx+eax*4+16Ch]
		.text:0043A7C0                 mov     eax, [ebp+iTemp]
		.text:0043A7C3                 add     eax, [edx+0B0h]
		.text:0043A7C9                 mov     [ebp+iTotal], eax
		 */
		iTotal = iTemp + m_pClientList[iClientH]->m_iHPstock;
		/*
		.text:0043A7CC                 mov     ecx, [ebp+iClientH]
		.text:0043A7CF                 mov     edx, [ebp+m_pClientList]
		.text:0043A7D2                 mov     eax, [edx+ecx*4+16Ch] ; m_pClientList[iClientH]->
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
		if (m_pClientList[iClientH]->m_iAddHP != 0) {
			dV2 = (double) iTotal;
			dV3 = (double) m_pClientList[iClientH]->m_iAddHP;
			dV1 = (dV3 / 100.0f) * dV2;
			iTotal += (int) dV1;
		}
		/*
		.text:0043A830                 mov     eax, [ebp+iClientH]
		.text:0043A833                 mov     ecx, [ebp+m_pClientList]
		.text:0043A836                 mov     edx, [ecx+eax*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A83D                 mov     eax, [edx+0ACh] ; m_iHP
		.text:0043A843                 add     eax, [ebp+iTotal]
		.text:0043A846                 mov     ecx, [ebp+iClientH]
		.text:0043A849                 mov     edx, [ebp+m_pClientList]
		.text:0043A84C                 mov     ecx, [edx+ecx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A853                 mov     [ecx+0ACh], eax ; m_iHP
		 */
		m_pClientList[iClientH]->m_iHP += iTotal;
		/*
		.text:0043A859                 mov     edx, [ebp+iClientH]
		.text:0043A85C                 mov     eax, [ebp+m_pClientList]
		.text:0043A85F                 mov     ecx, [eax+edx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A866                 mov     edx, [ecx+0ACh] ; m_iHP
		.text:0043A86C                 cmp     edx, [ebp+iMaxHP]
		.text:0043A86F                 jle     short loc_43A887
		.text:0043A871                 mov     eax, [ebp+iClientH]
		.text:0043A874                 mov     ecx, [ebp+m_pClientList]
		.text:0043A877                 mov     edx, [ecx+eax*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A87E                 mov     eax, [ebp+iMaxHP]
		.text:0043A881                 mov     [edx+0ACh], eax ; m_iHP
		 */
		if (m_pClientList[iClientH]->m_iHP > iMaxHP) m_pClientList[iClientH]->m_iHP = iMaxHP;
		/*
		.text:0043A887                 mov     ecx, [ebp+iClientH]
		.text:0043A88A                 mov     edx, [ebp+m_pClientList]
		.text:0043A88D                 mov     eax, [edx+ecx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A894                 cmp     dword ptr [eax+m_iHP], 0
		.text:0043A89B                 jg      short loc_43A8B4
		.text:0043A89D                 mov     ecx, [ebp+iClientH]
		.text:0043A8A0                 mov     edx, [ebp+m_pClientList]
		.text:0043A8A3                 mov     eax, [edx+ecx*4+16Ch] ; m_pClientList[iClientH]->
		.text:0043A8AA                 mov     dword ptr [eax+m_iHP], 0
		 */
		if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 0;
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
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
	}
	/*
	.text:0043A8DD                 mov     edx, [ebp+iClientH]
	.text:0043A8E0                 mov     eax, [ebp+m_pClientList]
	.text:0043A8E3                 mov     ecx, [eax+edx*4+16Ch]
	.text:0043A8EA                 mov     dword ptr [ecx+0B0h], 0 ; m_iHPstock = 0
	 */
	m_pClientList[iClientH]->m_iHPstock = 0;
}

char CGame::_cCheckHeroItemEquipped(int iClientH) {
	short sHeroLeggings, sHeroHauberk, sHeroArmor, sHeroHelm;
	if (m_pClientList[iClientH] == nullptr) return 0;
	sHeroHelm = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
	sHeroArmor = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
	sHeroHauberk = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
	sHeroLeggings = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
	if ((sHeroHelm < 0) || (sHeroLeggings < 0) || (sHeroArmor < 0) || (sHeroHauberk < 0)) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroHelm] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroLeggings] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroArmor] == nullptr) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroHauberk] == nullptr) return 0;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 403) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 411) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 1;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 407) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 415) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 2;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 404) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 412) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 1;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 408) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 416) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 2;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 405) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 413) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 1;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 409) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 417) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 2;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 406) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 414) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 1;
	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 410) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 418) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
			  (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 2;
	return 0;
}

bool CGame::bPlantSeedBag(int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH) {
	int iNamingValue, tX, tY;
	short sOwnerH;
	char cOwnerType, cNpcName[21], cName[21], cNpcWaypointIndex[11];
	bool bRet;
	if (m_pClientList[iClientH]->map_->m_iTotalAgriculture >= 200) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOMOREAGRICULTURE, 0, 0, 0, nullptr);
		return false;
	}
	if (iItemEffectValue2 > m_pClientList[iClientH]->m_cSkillMastery[2]) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_AGRICULTURESKILLLIMIT, 0, 0, 0, nullptr);
		return false;
	}
	iNamingValue = m_pClientList[iClientH]->map_->iGetEmptyNamingValue();
	if (iNamingValue == -1) {
	} else {
		m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
		if (sOwnerH != 0 && sOwnerH == DEF_OWNERTYPE_NPC && m_pNpcList[sOwnerH]->m_cActionLimit == 5) {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_AGRICULTURENOAREA, 0, 0, 0, nullptr);
			return false;
		} else {
			if (m_pClientList[iClientH]->map_->bGetIsFarm(dX, dY) == false) {
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_AGRICULTURENOAREA, 0, 0, 0, nullptr);
				return false;
			}
			std::memset(cNpcName, 0, sizeof (cNpcName));
			strcpy(cNpcName, "Crops");
			std::memset(cName, 0, sizeof (cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = m_pClientList[iClientH]->map_->id_ + 65;
			std::memset(cNpcWaypointIndex, 0, sizeof (cNpcWaypointIndex));
			tX = dX;
			tY = dY;
			bRet = bCreateNewNpc(cNpcName, cName, m_pClientList[iClientH]->map_->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypointIndex, nullptr, 0, 0, false, true);
			if (bRet == false) {
				m_pClientList[iClientH]->map_->SetNamingValueEmpty(iNamingValue);
			} else {
				m_pClientList[iClientH]->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
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
				SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
				wsprintf(G_cTxt, "(skill:%d type:%d)plant(%s) Agriculture begin(%d,%d) sum(%d)!", m_pNpcList[sOwnerH]->m_cCropSkill, m_pNpcList[sOwnerH]->m_cCropType, cNpcName, tX, tY, m_pClientList[iClientH]->map_->m_iTotalAgriculture);
				PutLogList(G_cTxt);
				return true;
			}
		}
	}
	return false;
}

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
void CGame::_CheckFarmingAction(short sAttackerH, short sTargetH, bool bType) {
	char cCropType;
	int iItemID;
	class CItem * pItem;
	iItemID = 0;
	cCropType = 0;
	cCropType = m_pNpcList[sTargetH]->m_cCropType;
	switch (cCropType) {
		case 1: GetExp(sAttackerH, iDice(3, 10));
			iItemID = 820;
			break; // WaterMelon
		case 2: GetExp(sAttackerH, iDice(3, 10));
			iItemID = 821;
			break; // Pumpkin
		case 3: GetExp(sAttackerH, iDice(4, 10));
			iItemID = 822;
			break; // Garlic
		case 4: GetExp(sAttackerH, iDice(4, 10));
			iItemID = 823;
			break; // Barley
		case 5: GetExp(sAttackerH, iDice(5, 10));
			iItemID = 824;
			break; // Carrot
		case 6: GetExp(sAttackerH, iDice(5, 10));
			iItemID = 825;
			break; // Radish
		case 7: GetExp(sAttackerH, iDice(6, 10));
			iItemID = 826;
			break; // Corn
		case 8: GetExp(sAttackerH, iDice(6, 10));
			iItemID = 827;
			break; // ChineseBellflower
		case 9: GetExp(sAttackerH, iDice(7, 10));
			iItemID = 828;
			break; // Melone
		case 10: GetExp(sAttackerH, iDice(7, 10));
			iItemID = 829;
			break; // Tommato
		case 11: GetExp(sAttackerH, iDice(8, 10));
			iItemID = 830;
			break; // Grapes
		case 12: GetExp(sAttackerH, iDice(8, 10));
			iItemID = 831;
			break; // BlueGrapes
		case 13: GetExp(sAttackerH, iDice(9, 10));
			iItemID = 832;
			break; // Mushroom
		default: GetExp(sAttackerH, iDice(10, 10));
			iItemID = 721;
			break; // Ginseng
	}
	pItem = new class CItem;
	if (_bInitItemAttr(*pItem, iItemID) == false) {
		delete pItem;
	}
	if (bType == 0) {
		m_pClientList[sAttackerH]->map_->bSetItem(m_pClientList[sAttackerH]->m_sX, m_pClientList[sAttackerH]->m_sY, pItem);
		m_pClientList[sAttackerH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, 
				  m_pClientList[sAttackerH]->m_sX, m_pClientList[sAttackerH]->m_sY, pItem->m_sSprite,
				  pItem->m_sSpriteFrame, pItem->m_cItemColor, false);
	} else if (bType == 1) {
		m_pNpcList[sTargetH]->map_->bSetItem(m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY, pItem);
		m_pNpcList[sTargetH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, 
				  m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY, pItem->m_sSprite,
				  pItem->m_sSpriteFrame, pItem->m_cItemColor, false);
	}
}

void CGame::CalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, char cAttackerType, int iArmorType) {
	short sItemIndex;
	int iDownValue = 1, iHammerChance = 100;
	if (m_pClientList[sTargetH] == nullptr) return;
	if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (cAttackerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sAttackerH] != nullptr)) {
		if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_cSide != m_pClientList[sAttackerH]->m_cSide)) {
			switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) {
				case 14:
					if ((31 == ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4)) || (32 == ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4))) {
						sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
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
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
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
					sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
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
	int iAP_SM, iAP_L, iAttackerHitRatio, iTargetDefenseRatio, iDestHitRatio, iResult, iAP_Abs_Armor, iAP_Abs_Shield;
	char cAttackerName[21], cAttackerDir, cAttackerSide, cTargetDir, cProtect, cCropSkill, cFarmingSkill;
	short sWeaponIndex, sAttackerWeapon, dX, dY, sX, sY, sAtkX, sAtkY, sTgtX, sTgtY;
	uint32_t dwTime;
	char cDamageMod[256];
	uint16_t wWeaponType;
	double dTmp1, dTmp2, dTmp3;
	bool bKilled;
	bool bIsAttackerBerserk;
	int iKilledDice, iDamage, iExp, iWepLifeOff, iSideCondition, iMaxSuperAttack, iWeaponSkill, iComboBonus, iTemp;
	int iMoveDamage, iRepDamage;
	char cAttackerSA;
	int iAttackerSAvalue, iHitPoint;
	char cDamageMoveDir;
	int iPartyID, iWarContribution, tX, tY, iDst1, iDst2;
	short sItemIndex;
	short sSkillUsed;
	dwTime = timeGetTime();
	bKilled = false;
	iExp = 0;
	iPartyID = 0;
	std::memset(cAttackerName, 0, sizeof (cAttackerName));
	cAttackerSA = 0;
	iAttackerSAvalue = 0;
	wWeaponType = 0;
	switch (cAttackerType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[sAttackerH] == nullptr) return 0;
			if ((m_bAdminSecurity == true) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return 0;
			if ((m_pClientList[sAttackerH]->map_->m_bIsAttackEnabled == false) && (m_pClientList[sAttackerH]->m_iAdminUserLevel == 0)) return 0;
			if ((m_pClientList[sAttackerH]->map_ == nullptr) && (m_pClientList[sAttackerH]->map_->m_bIsHeldenianMap == true) && (m_bIsHeldenianMode == true)) return 0;
			if ((m_bIsCrusadeMode == false) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true) && (cTargetType == DEF_OWNERTYPE_PLAYER)) return 0;
			if ((m_pClientList[sAttackerH]->m_iStatus & 0x10) != 0) {
				SetInvisibilityFlag(sAttackerH, DEF_OWNERTYPE_PLAYER, false);
				delayEvents_.remove(sAttackerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
				m_pClientList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
			}
			if ((m_pClientList[sAttackerH]->m_sAppr2 & 0xF000) == 0) return 0;
			iAP_SM = 0;
			iAP_L = 0;
			wWeaponType = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
			sSkillUsed = m_pClientList[sAttackerH]->m_sUsingWeaponSkill;
			if ((bIsDash == true) && (m_pClientList[sAttackerH]->m_cSkillMastery[sSkillUsed] != 100) && (wWeaponType != 25) && (wWeaponType != 27)) {
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
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
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
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
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
			sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
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
			if (m_pClientList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0)
				bIsAttackerBerserk = true;
			else bIsAttackerBerserk = false;
			if ((bArrowUse != true) && (m_pClientList[sAttackerH]->m_iSuperAttackLeft > 0) && (iAttackMode >= 20)) {
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
			if (bIsDash == true) {
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
				delayEvents_.remove(sAttackerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
				m_pNpcList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
			}
			cAttackerSide = m_pNpcList[sAttackerH]->m_cSide;
			iAP_SM = 0;
			iAP_L = 0;
			if (m_pNpcList[sAttackerH]->m_cAttackDiceThrow != 0)
				iAP_L = iAP_SM = iDice(m_pNpcList[sAttackerH]->m_cAttackDiceThrow, m_pNpcList[sAttackerH]->m_cAttackDiceRange);
			iAttackerHitRatio = m_pNpcList[sAttackerH]->m_iHitRatio;
			cAttackerDir = m_pNpcList[sAttackerH]->m_cDir;
			memcpy(cAttackerName, m_pNpcList[sAttackerH]->m_cNpcName, 20);
			if (m_pNpcList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0)
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
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == false) &&
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
				iSideCondition = iGetPlayerRelationship(sAttackerH, sTargetH);
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
								SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
							}
							switch (m_pNpcList[sTargetH]->m_iBuildCount) {
								case 1:
									m_pNpcList[sTargetH]->m_sAppr2 = 0;
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									break;
								case 10:
									m_pNpcList[sTargetH]->m_sAppr2 = 2;
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
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
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									//sub_4B67E0
									m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(2, cFarmingSkill <= cCropSkill + 10);
									_CheckFarmingAction(sAttackerH, sTargetH, 1);
									DeleteNpc(sTargetH);
									return 0;
								case 8:
									m_pNpcList[sTargetH]->m_sAppr2 = 3;
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(2, cFarmingSkill <= cCropSkill + 10);
									_CheckFarmingAction(sAttackerH, sTargetH, 0);
									break;
								case 18:
									m_pNpcList[sTargetH]->m_sAppr2 = 2;
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
									m_pClientList[sAttackerH]->CalculateSSN_SkillIndex(2, cFarmingSkill <= cCropSkill + 10);
									_CheckFarmingAction(sAttackerH, sTargetH, 0);
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
			cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
			break;
		case DEF_OWNERTYPE_NPC:
			cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
			break;
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1) {
			if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]] == nullptr) {
				m_pClientList[sAttackerH]->m_bIsItemEquipped[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]] = false;
				m_pClientList[sAttackerH]->DeleteClient(true, true);
				return 0;
			}
			if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_ARROW) {
				if (m_pClientList[sAttackerH]->m_cArrowIndex == -1) {
					return 0;
				} else {
					if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex] == nullptr)
						return 0;
					if (bArrowUse != true)
						m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount--;
					if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount <= 0) {
						m_pClientList[sAttackerH]->ItemDepleteHandler(m_pClientList[sAttackerH]->m_cArrowIndex, false, true);
						m_pClientList[sAttackerH]->m_cArrowIndex = _iGetArrowItemIndex(sAttackerH);
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
	if ((bIsAttackerBerserk == true) && (iAttackMode < 20)) {
		iAP_SM = iAP_SM * 2;
		iAP_L = iAP_L * 2;
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		iAP_SM += m_pClientList[sAttackerH]->m_iAddPhysicalDamage;
		iAP_L += m_pClientList[sAttackerH]->m_iAddPhysicalDamage;
	}
	if (bNearAttack == true) {
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
	if (iResult <= iDestHitRatio) {
		if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
			if (((m_pClientList[sAttackerH]->m_iHungerStatus <= 10) || (m_pClientList[sAttackerH]->m_iSP <= 0)) && (iDice(1, 10) == 5)) return false;
			m_pClientList[sAttackerH]->m_iComboAttackCount++;
			if (m_pClientList[sAttackerH]->m_iComboAttackCount < 0) m_pClientList[sAttackerH]->m_iComboAttackCount = 0;
			if (m_pClientList[sAttackerH]->m_iComboAttackCount > 4) m_pClientList[sAttackerH]->m_iComboAttackCount = 1;
			iWeaponSkill = _iGetWeaponSkillType(sAttackerH);
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
					cAttackerSA = 61;
					iAttackerSAvalue = m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue * 5;
					break;
				case 3:
					cAttackerSA = 62;
					break;
			}
			if (m_pClientList[sAttackerH]->map_->m_bIsFightZone == true) {
				iAP_SM += iAP_SM / 3;
				iAP_L += iAP_L / 3;
			}
			if (bCheckHeldenianMap(sAttackerH, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1) {
				iAP_SM += iAP_SM / 3;
				iAP_L += iAP_L / 3;
			}
			if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == true) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1)) {
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
				ClearSkillUsingStatus(sTargetH);
				if ((dwTime - m_pClientList[sTargetH]->m_dwTime) > DEF_RAGPROTECTIONTIME) {
					return 0;
				} else {
					switch (cAttackerSA) {
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
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] > 0) {
								if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY];
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
						case 2:
							if ((m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] +
									  m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS]) > 0) {
								if ((m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] +
										  m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS]) >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] + m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS]);
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
						case 3:
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] > 0) {
								if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS];
								dTmp2 = (double) iAP_SM;
								dTmp3 = (dTmp1 / 100.0f) * dTmp2;
								iAP_Abs_Armor = (int) dTmp3;
							}
							break;
						case 4:
							if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] > 0) {
								if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] >= 80)
									dTmp1 = 80.0f;
								else dTmp1 = (double) m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD];
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
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND];
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
								if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(sTargetH, DEF_OWNERTYPE_PLAYER, true);
									delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + 30000,
											  sTargetH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
									m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
								}
								break;
							case 3:
								if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 0) {
									m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 2;
									delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + 10000,
											  sTargetH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 10, 0, 0);
									m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, 10, 0, nullptr);
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
								if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
									sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
								else sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
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
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							}
							break;
						case 2:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							} else {
								iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
								if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
									CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
								}
							}
							break;
						case 3:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							}
							break;
						case 4:
							iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
							if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != nullptr)) {
								CalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, cAttackerType, iTemp);
							}
							break;
					}
					if ((cAttackerSA == 2) && (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0)) {
						m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_PROTECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT], 0, nullptr);
						switch (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT]) {
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
						m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
					}
					if ((m_pClientList[sTargetH]->m_bIsPoisoned == false) &&
							  ((cAttackerSA == 5) || (cAttackerSA == 6) || (cAttackerSA == 61))) {
						if (bCheckResistingPoisonSuccess(sTargetH, DEF_OWNERTYPE_PLAYER) == false) {
							m_pClientList[sTargetH]->m_bIsPoisoned = true;
							if (cAttackerSA == 5) m_pClientList[sTargetH]->m_iPoisonLevel = 15;
							else if (cAttackerSA == 6) m_pClientList[sTargetH]->m_iPoisonLevel = 40;
							else if (cAttackerSA == 61) m_pClientList[sTargetH]->m_iPoisonLevel = iAttackerSAvalue;
							m_pClientList[sTargetH]->m_dwPoisonTime = dwTime;
							m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pClientList[sTargetH]->m_iPoisonLevel, 0, nullptr);
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
									SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iAP_SM, sAttackerWeapon, 0);
							}
							if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 1) {
								m_pClientList[sTargetH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, nullptr);
								m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
								delayEvents_.remove(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
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
				if (m_bIsCrusadeMode == true) {
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
				if ((cAttackerSA == 2) && (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0)) {
					switch (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT]) {
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
					m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = 0;
					delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_PROTECT);
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
					NpcKilledHandler(sAttackerH, cAttackerType, sTargetH, iDamage);
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
						SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
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
						SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
						if (bCheckEnergySphereDestination(sTargetH, sAttackerH, cAttackerType) == true) {
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
								if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;
								GetExp(sAttackerH, iExp);
								DeleteNpc(sTargetH);
								return false;
							}
						}
CAE_SKIPDAMAGEMOVE2:
						;
					} else {
						SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, sAttackerWeapon, 0);
					}
					if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 1) {
						m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
						delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
					} else if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 2) {
						if ((m_pNpcList[sTargetH]->m_iHitDice > 50) && (iDice(1, 10) == 5)) {
							m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
							delayEvents_.remove(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
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
						if (m_bIsCrusadeMode == true) iExp = iExp / 3;
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
			if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
				sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
			else sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
			if ((sWeaponIndex != -1) && (bArrowUse != true)) {
				if ((m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex] != nullptr) &&
						  (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_sIDnum != 231)) {
					if (bKilled == false)
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

bool CGame::_bNpcBehavior_ManaCollector(int iNpcH) {
	int dX, dY, iMaxMP, iTotal;
	short sOwnerH;
	char cOwnerType;
	double dV1, dV2, dV3;
	bool bRet;
	if (m_pNpcList[iNpcH] == nullptr) return false;
	if (m_pNpcList[iNpcH]->m_sAppr2 != 0) return false;
	bRet = false;
	for (dX = m_pNpcList[iNpcH]->m_sX - 5; dX <= m_pNpcList[iNpcH]->m_sX + 5; dX++)
		for (dY = m_pNpcList[iNpcH]->m_sY - 5; dY <= m_pNpcList[iNpcH]->m_sY + 5; dY++) {
			m_pNpcList[iNpcH]->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (sOwnerH != 0) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pNpcList[iNpcH]->m_cSide == m_pClientList[sOwnerH]->m_cSide) {
							iMaxMP = (2 * m_pClientList[sOwnerH]->m_iMag) + (2 * m_pClientList[sOwnerH]->m_iLevel) + (m_pClientList[sOwnerH]->m_iInt / 2);
							if (m_pClientList[sOwnerH]->m_iMP < iMaxMP) {
								iTotal = iDice(1, (m_pClientList[sOwnerH]->m_iMag));
								if (m_pClientList[sOwnerH]->m_iAddMP != 0) {
									dV2 = (double) iTotal;
									dV3 = (double) m_pClientList[sOwnerH]->m_iAddMP;
									dV1 = (dV3 / 100.0f) * dV2;
									iTotal += (int) dV1;
								}
								m_pClientList[sOwnerH]->m_iMP += iTotal;
								if (m_pClientList[sOwnerH]->m_iMP > iMaxMP)
									m_pClientList[sOwnerH]->m_iMP = iMaxMP;
								m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
							}
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if ((m_pNpcList[sOwnerH]->m_sType == 42) && (m_pNpcList[sOwnerH]->m_iV1 > 0)) {
							if (m_pNpcList[sOwnerH]->m_iV1 >= 3) {
								m_iCollectedMana[m_pNpcList[iNpcH]->m_cSide] += 3;
								m_pNpcList[sOwnerH]->m_iV1 -= 3;
								bRet = true;
							} else {
								m_iCollectedMana[m_pNpcList[iNpcH]->m_cSide] += m_pNpcList[sOwnerH]->m_iV1;
								m_pNpcList[sOwnerH]->m_iV1 = 0;
								bRet = true;
							}
						}
						break;
				}
			}
		}
	return bRet;
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

void CGame::_NpcBehavior_GrandMagicGenerator(int iNpcH) {
	switch (m_pNpcList[iNpcH]->m_cSide) {
		case 1:
			if (m_iAresdenMana > DEF_GMGMANACONSUMEUNIT) {
				m_iAresdenMana = 0;
				m_pNpcList[iNpcH]->m_iManaStock++;
				if (m_pNpcList[iNpcH]->m_iManaStock > m_pNpcList[iNpcH]->m_iMaxMana) {
					MeteorStrikeMsgHandler(1);
					m_pNpcList[iNpcH]->m_iManaStock = 0;
					m_iAresdenMana = 0;
				}
				wsprintf(G_cTxt, "(!) Aresden GMG %d/%d", m_pNpcList[iNpcH]->m_iManaStock, m_pNpcList[iNpcH]->m_iMaxMana);
				PutLogList(G_cTxt);
			}
			break;
		case 2:
			if (m_iElvineMana > DEF_GMGMANACONSUMEUNIT) {
				m_iElvineMana = 0;
				m_pNpcList[iNpcH]->m_iManaStock++;
				if (m_pNpcList[iNpcH]->m_iManaStock > m_pNpcList[iNpcH]->m_iMaxMana) {
					MeteorStrikeMsgHandler(2);
					m_pNpcList[iNpcH]->m_iManaStock = 0;
					m_iElvineMana = 0;
				}
				wsprintf(G_cTxt, "(!) Elvine GMG %d/%d", m_pNpcList[iNpcH]->m_iManaStock, m_pNpcList[iNpcH]->m_iMaxMana);
				PutLogList(G_cTxt);
			}
			break;
	}
}

bool CGame::_bNpcBehavior_Detector(int iNpcH) {
	int dX, dY;
	short sOwnerH;
	char cOwnerType, cSide;
	bool bFlag = false;
	if (m_pNpcList[iNpcH] == nullptr) return false;
	if (m_pNpcList[iNpcH]->m_sAppr2 != 0) return false;
	for (dX = m_pNpcList[iNpcH]->m_sX - 10; dX <= m_pNpcList[iNpcH]->m_sX + 10; dX++)
		for (dY = m_pNpcList[iNpcH]->m_sY - 10; dY <= m_pNpcList[iNpcH]->m_sY + 10; dY++) {
			m_pNpcList[iNpcH]->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			cSide = 0;
			if (sOwnerH != 0) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						cSide = m_pClientList[sOwnerH]->m_cSide;
						break;
					case DEF_OWNERTYPE_NPC:
						cSide = m_pNpcList[sOwnerH]->m_cSide;
						break;
				}
			}
			if ((cSide != 0) && (cSide != m_pNpcList[iNpcH]->m_cSide)) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
							m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
							SetInvisibilityFlag(sOwnerH, cOwnerType, false);
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
							m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
							SetInvisibilityFlag(sOwnerH, cOwnerType, false);
						}
						break;
				}
				bFlag = true;
			}
		}
	return bFlag;
}
// October 19,2004 - 3.51 translated

void CGame::NpcBehavior_Dead(int iNpcH) {
	uint32_t dwTime;
	if (m_pNpcList[iNpcH] == nullptr) return;
	dwTime = timeGetTime();
	m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;
	if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 5) {
		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
	}
	if ((dwTime - m_pNpcList[iNpcH]->m_dwDeadTime) > m_pNpcList[iNpcH]->m_dwRegenTime)
		DeleteNpc(iNpcH);
}
// v2.15 2002-8-7 // 2002-09-06 #1

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
	// NPC를 삭제할때 특수 아이템을 발생시킬 것인지의 여부를 계산한다.
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
		if ((bFirstDice == true) && (bSecondDice == true)) {
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
void CGame::StormBringer(int iClientH, short dX, short dY) {
	char cOwnerType;
	short sOwner, sAppr2, sAttackerWeapon;
	int iDamage, iTemp, iV1, iV2, iV3;
	//Stormbringer
	if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
		m_pClientList[iClientH]->map_->GetOwner(&sOwner, &cOwnerType, dX, dY);
		iTemp = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
		sAppr2 = (short) ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) >> 12);
		if (memcmp(m_pClientList[iClientH]->m_pItemList[iTemp]->m_cName, "StormBringer", 12) == 0) {
			switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwner]->m_iAdminUserLevel < 3) {
						if (sAppr2 != 0) {
							iV1 = m_pClientList[iClientH]->m_cAttackDiceThrow_L;
							iV2 = m_pClientList[iClientH]->m_cAttackDiceRange_L;
							iV3 = m_pClientList[iClientH]->m_cAttackBonus_L;
							if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0) {
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
								SendEventToNearClient_TypeA(sOwner, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, iDamage, sAttackerWeapon, 0);
								m_pClientList[sOwner]->map_->ClearOwner(14, sOwner, DEF_OWNERTYPE_PLAYER, m_pClientList[sOwner]->m_sX, m_pClientList[sOwner]->m_sY);
								m_pClientList[sOwner]->map_->SetDeadOwner(sOwner, DEF_OWNERTYPE_PLAYER, m_pClientList[sOwner]->m_sX, m_pClientList[sOwner]->m_sY);
							} else {
								m_pClientList[sOwner]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
								SendEventToNearClient_TypeA(sOwner, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
							}
						}
					}
					break;
			}
		}
	}
}

bool CGame::_bCheckCharacterData(int iClientH) {
	register int i;
	if ((m_pClientList[iClientH]->m_iStr > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iVit > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iDex > m_sCharStatLimit) ||
			  (m_pClientList[iClientH]->m_iMag > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iInt > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iCharisma > m_sCharStatLimit)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) stat points are greater then server accepts.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((m_pClientList[iClientH]->m_iLevel > m_sMaxPlayerLevel) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) level above max server level.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if (m_pClientList[iClientH]->m_iExp < 0) {
		m_pClientList[iClientH]->m_iExp = DEF_PLAYER_MAX_EXPERIENCE; // juan249 - Exp negativa Fix.
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) experience is below 0 - (Exp:%d). - FIXED", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iExp);
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((m_pClientList[iClientH]->m_iHP > m_pClientList[iClientH]->iGetMaxHP()) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) HP: current/maximum (%d/%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iHP, m_pClientList[iClientH]->iGetMaxHP());
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((m_pClientList[iClientH]->m_iMP > m_pClientList[iClientH]->iGetMaxMP()) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) MP: current/maximum (%d/%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iMP, m_pClientList[iClientH]->iGetMaxMP());
		PutHackLogFileList(G_cTxt);
		return false;
	}
	if ((m_pClientList[iClientH]->m_iSP > m_pClientList[iClientH]->iGetMaxSP()) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) {
		wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) SP: current/maximum (%d/%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iSP, m_pClientList[iClientH]->iGetMaxSP());
		PutHackLogFileList(G_cTxt);
		return false;
	}
	/*if (m_pClientList[iClientH]->m_iAdminUserLevel != 0) {
		for (i = 0; i < DEF_MAXADMINS; i++) {
			if(strlen(m_stAdminList[i].m_cGMName) == 0) break; //No more GM's on list
			if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(m_pClientList[iClientH]->m_cCharName))) {
				if(memcmp(m_stAdminList[i].m_cGMName, m_pClientList[iClientH]->m_cCharName, strlen(m_pClientList[iClientH]->m_cCharName)) != 0){
				}
				else {
					wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) is an unaccepted admin.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
					PutHackLogFileList(G_cTxt);
					return false;
				}
			}
		}
	}*/
	for (i = 0; i < DEF_MAXBANNED; i++) {
		if (strlen(m_stBannedList[i].m_cBannedIPaddress) == 0) break; //No more GM's on list
		if ((strlen(m_stBannedList[i].m_cBannedIPaddress)) == (strlen(m_pClientList[iClientH]->m_cIPaddress))) {
			if (memcmp(m_stBannedList[i].m_cBannedIPaddress, m_pClientList[iClientH]->m_cIPaddress, strlen(m_pClientList[iClientH]->m_cIPaddress)) == 0) {
				wsprintf(G_cTxt, "Client Rejected: Banned: (%s)", m_pClientList[iClientH]->m_cIPaddress);
				PutLogList(G_cTxt);
				return false;
			} else {
			}
		}
	}
	return true;
}

void CGame::GlobalEndHeldenianMode() {
	if (m_bIsHeldenianMode == false) return;
	LocalEndHeldenianMode();
}

void CGame::LocalEndHeldenianMode() {
	register int i, x;
	if (m_bIsHeldenianMode == false) return;
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
								RemoveEventNpc(x);
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

/*********************************************************************************************************************
 **  bool CGame::bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType)										**
 **  description			:: checks if the player is in the specified map												**
 **  last updated		:: November 22, 2004; 6:19 PM; Hypnotoad													**
 **	return value		:: bool																						**
 **  commentary			::	-	translated from scratch using IDA Pro												**
 **						::	-	changed pTile->m_cOwner to m_iOccupyStatus											**
 **						::	-	added check to prevent access violation if pTile == nullptr							**
 **						::	-	removed 4 return(s) after "iRet = 1;" and placed at end								**
 *********************************************************************************************************************/
bool CGame::bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType) {
	short tX, tY;
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

void CGame::RequestHeldenianTeleport(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char cTmpName[21], * cp, cData[512], cMapName[11];
	int iRet, iWhyReturn, iProcessed, tX, tY, cLoc, * ip;
	uint32_t *dwp;
	iProcessed = 1;
	if (m_pClientList[iClientH] == nullptr) return;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, cp);
	if (strcmp(cTmpName, "Gail") == 0) {
		std::memset(cData, 0, sizeof (cData));
		dwp = (uint32_t *) (cData);
		*dwp = MSGID_REQUEST_GAILTELEPORT;
		cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
		if ((m_bIsHeldenianMode == 1) &&
				  (m_pClientList[iClientH]->m_bIsPlayerCivil != true) &&
				  ((m_pClientList[iClientH]->m_cSide == 2 || (m_pClientList[iClientH]->m_cSide == 1)))) {
			if (m_cHeldenianModeType == 1) {
				std::memset(cMapName, 0, sizeof (cMapName));
				memcpy(cMapName, "BtField", 10);
				if (m_pClientList[iClientH]->m_cSide == 1) {
					tX = 68;
					tY = 225;
					cLoc = 1;
				} else if (m_pClientList[iClientH]->m_cSide == 2) {
					tX = 202;
					tY = 70;
					cLoc = 2;
				}
			} else if (m_cHeldenianModeType == 2) {
				std::memset(cMapName, 0, sizeof (cMapName));
				memcpy(cMapName, "HRampart", 10);
				if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner) {
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
	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 36);
	switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			m_pClientList[iClientH]->DeleteClient(true, true);
			break;
	}
}

void CGame::RequestNpcSpeakTeleport(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char cMapName[11], *cp;
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	cp += 4;
	std::memset(cMapName, 0, sizeof (cMapName));
	switch (iDice(1, 2)) {
		case 1:
			switch (m_pClientList[iClientH]->m_cSide) {
				case 0: m_pClientList[iClientH]->RequestTeleportHandler("2   ", "BtField", 68, 225);
					break;
				case 1: m_pClientList[iClientH]->RequestTeleportHandler("2   ", "BtField", 68, 225);
					break;
				case 2: m_pClientList[iClientH]->RequestTeleportHandler("2   ", "BtField", 202, 70);
					break;
			}
			break;
	}
}

void CGame::HeldenianWarStarter() {
	SYSTEMTIME SysTime;
	int i;
	GetLocalTime(&SysTime);
	for (i = 0; i < DEF_MAXSCHEDULE; i++)
		if ((m_bIsHeldenianScheduleLoaded == false) &&
				  (m_bIsHeldenianSchedule == false) &&
				  (m_stHeldenianSchedule[i].iDay == SysTime.wDayOfWeek) &&
				  (m_stHeldenianSchedule[i].EndiHour == SysTime.wHour) &&
				  (m_stHeldenianSchedule[i].EndiMinute == SysTime.wMinute) &&
				  (m_bIsApocalypseMode != true) &&
				  (m_bIsCrusadeMode != true)) {
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
		if ((m_bIsHeldenianScheduleLoaded == true) &&
				  (m_bIsHeldenianSchedule == true) &&
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
	int i, x, z, iNamingValue;
	char cName[11], cTmp[21], cNpcWaypointIndex[10], cSide, cOwnerType;
	short sOwnerH;
	bool bRet;
	int dX, dY;
	if (m_bIsHeldenianMode == true) return;
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
						RemoveEventNpc(i);
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
									if (m_pNpcConfigList[z] == nullptr) break;
									if (m_pNpcConfigList[z]->m_sType == m_pMapList[x]->m_stHeldenianTower[i].sTypeID) {
										std::memset(cTmp, 0, sizeof (cTmp));
										strcpy(cTmp, m_pNpcConfigList[z]->m_cNpcName);
									}
								}
								std::memset(cName, 0, sizeof (cName));
								wsprintf(cName, "XX%d", iNamingValue);
								cName[0] = 95;
								cName[1] = i + 65;
								bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, nullptr, 0, cSide, false, false, false, true, false);
								if (bRet == false) {
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
									if (m_pNpcConfigList[z] == nullptr) break;
									if (m_pNpcConfigList[z]->m_sType == 91) {
										std::memset(cTmp, 0, sizeof (cTmp));
										strcpy(cTmp, m_pNpcConfigList[z]->m_cNpcName);
									}
								}
								std::memset(cName, 0, sizeof (cName));
								wsprintf(cName, "XX%d", iNamingValue);
								cName[0] = 95;
								cName[1] = i + 65;
								bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, nullptr, 0, cSide, false, false, false, true, false);
								if (bRet == false) {
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

void CGame::ManualStartHeldenianMode(int iClientH, char *pData, uint32_t dwMsgSize) {
	char cHeldenianType, cBuff[256], * token, seps[] = "= \t\n";
	SYSTEMTIME SysTime;
	class CStrTok * pStrTok;
	int iV1;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 3)
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
	if (m_bIsHeldenianMode == true) return;
	if (m_bIsApocalypseMode == true) return;
	if (m_bIsCrusadeMode == true) return;
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
	wsprintf(G_cTxt, "GM Order(%s): begin Heldenian", m_pClientList[iClientH]->m_cCharName);
	PutLogFileList(G_cTxt);
	bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
}

void CGame::ManualEndHeldenianMode(int iClientH, char */*pData*/, uint32_t /*dwMsgSize*/) {
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 3) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
	}
	if (m_bIsHeldenianMode == true) {
		GlobalEndHeldenianMode();
		m_bHeldenianRunning = false;
		wsprintf(G_cTxt, "GM Order(%s): end Heldenian", m_pClientList[iClientH]->m_cCharName);
		bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, G_cTxt);
	}
}

void CGame::GlobalStartApocalypseMode() {
	uint32_t dwApocalypseGUID = timeGetTime();
	LocalStartApocalypseMode(dwApocalypseGUID);
}

void CGame::AutomatedHeldenianTimer() {
	uint32_t dwTime;
	int x, i;
	dwTime = time(nullptr);
	if ((dwTime == m_dwHeldenianStartTime - 300) && (m_bHeldenianInitiated != true) && (m_bIsHeldenianMode != true)) {
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
	if ((dwTime > (m_dwHeldenianFinishTime - 180)) && (m_bHeldenianInitiated == true) && (m_bIsHeldenianMode != false)) {
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

void CGame::CheckCrusadeResultCalculation(int iClientH) {
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_cVar == 1) return;
	if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_dwCrusadeGUID != 0)) {
		if (m_pClientList[iClientH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION) m_pClientList[iClientH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (m_iCrusadeWinnerSide == 0) {
				m_pClientList[iClientH]->m_iExpStock += (m_pClientList[iClientH]->m_iWarContribution / 6);
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, m_pClientList[iClientH]->m_iWarContribution, nullptr);
				_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (m_pClientList[iClientH]->m_iWarContribution / 6), nullptr);
			} else {
				if (m_iCrusadeWinnerSide == m_pClientList[iClientH]->m_cSide) {
					if (m_pClientList[iClientH]->m_iLevel <= 80) {
						m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 100;
					} else if (m_pClientList[iClientH]->m_iLevel <= 100) {
						m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 40;
					} else m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel;
					m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iWarContribution;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, m_pClientList[iClientH]->m_iWarContribution, nullptr);
					_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, m_pClientList[iClientH]->m_iWarContribution, nullptr);
				} else if (m_iCrusadeWinnerSide != m_pClientList[iClientH]->m_cSide) {
					if (m_pClientList[iClientH]->m_iLevel <= 80) {
						m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 100;
					} else if (m_pClientList[iClientH]->m_iLevel <= 100) {
						m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 40;
					} else m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel;
					m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iWarContribution / 10;
					m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, -1 * m_pClientList[iClientH]->m_iWarContribution, nullptr);
					_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (m_pClientList[iClientH]->m_iWarContribution / 10), nullptr);
				}
			}
		} else {
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CRUSADE, (uint32_t) m_bIsCrusadeMode, 0, 0, nullptr, -1);
		}
		m_pClientList[iClientH]->m_iCrusadeDuty = 0;
		m_pClientList[iClientH]->m_iWarContribution = 0;
		m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
		m_pClientList[iClientH]->m_dwSpeedHackCheckTime = timeGetTime();
		m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;
	}
}

void CGame::CheckHeldenianResultCalculation(int iClientH) {
	double dV1, dV2, dV3;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_cWarType != 2) return;
	if ((m_cHeldenianModeType == 0) || (m_pClientList[iClientH]->m_dwCrusadeGUID == 0)) return;
	if (m_pClientList[iClientH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION) m_pClientList[iClientH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
	if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwHeldenianGUID) {
		if (m_sLastHeldenianWinner == 0) {
			m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iWarContribution / 6;
		} else if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner) {
			if (m_pClientList[iClientH]->m_iLevel <= 80) {
				m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 200;
			} else if (m_pClientList[iClientH]->m_iLevel > 100) {
				m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 100;
			} else {
				m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 30;
			}
			dV2 = (double) m_pClientList[iClientH]->m_iExpStock;
			dV3 = (double) m_pClientList[iClientH]->m_iWarContribution * 1.2;
			dV1 = dV2 + dV3;
			m_pClientList[iClientH]->m_iExpStock += dV1;
		} else if (m_pClientList[iClientH]->m_cSide != m_sLastHeldenianWinner) {
			if (m_pClientList[iClientH]->m_iLevel <= 80) {
				m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 200;
			} else if (m_pClientList[iClientH]->m_iLevel > 100) {
				m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 100;
			} else {
				m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 30;
			}
			m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iWarContribution / 5;
		}
		m_pClientList[iClientH]->m_iWarContribution = 0;
		m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
		m_pClientList[iClientH]->m_dwSpeedHackCheckTime = timeGetTime();
		m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;
	}
}

void CGame::NotifyStartHeldenianFightMode() {
	register int i, x;
	if ((m_bIsHeldenianMode != true) && (m_bHeldenianInitiated != true) && (m_bIsHeldenianReady != true)) return;
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
	register int i;
	bool bFlag;
	int iShortCutIndex, iClientH;
	if (m_bIsHeldenianMode != true) return false;
	for (i = 0; i < DEF_MAXMAPS; i++)
		if (m_pMapList[i] != nullptr) {
			if (m_pMapList[i]->m_bIsHeldenianMap == true) {
				bFlag = true;
				iShortCutIndex = 0;
			}
			while (bFlag == true) {
				iClientH = m_iClientShortCut[iShortCutIndex];
				iShortCutIndex++;
				if (iClientH == 0) {
					bFlag = 0;
				} else {
					if ((m_pClientList[iClientH] != nullptr) && (m_pClientList[iClientH]->m_bIsInitComplete == true) && (strcmp(m_pClientList[iClientH]->map_->m_cName, "BtField") == 0)) {
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_HELDENIANCOUNT, m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower, m_iHeldenianAresdenDead, nullptr, m_iHeldenianElvineDead, 0);
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
	int x, i;
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

void CGame::RemoveEventNpc(int iNpcH) {
	if (m_pNpcList[iNpcH] == nullptr) return;
	if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;
	m_pNpcList[iNpcH]->m_bIsKilled = true;
	m_pNpcList[iNpcH]->m_iHP = 0;
	m_pNpcList[iNpcH]->m_iLastDamage = 0;
	m_pNpcList[iNpcH]->m_dwRegenTime = 0;
	m_pNpcList[iNpcH]->map_->m_iTotalAliveObject--;
	ReleaseFollowMode(iNpcH, DEF_OWNERTYPE_NPC);
	m_pNpcList[iNpcH]->m_iTargetIndex = 0;
	m_pNpcList[iNpcH]->m_cTargetType = 0;
	SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, 1, 0);
	if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
		m_pNpcList[iNpcH]->map_->ClearOwner(10, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
	} else {
		m_pNpcList[iNpcH]->map_->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
	}
	m_pNpcList[iNpcH]->map_->SetDeadOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
	m_pNpcList[iNpcH]->m_cBehavior = 4;
	m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
	m_pNpcList[iNpcH]->m_dwDeadTime = timeGetTime();
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
	SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

/*********************************************************************************************************************
 **  bool CGame::RequestHeldenianScroll(int iClientH, char * pData, uint32_t dwMsgSize)									**
 ** description	  :: Reversed and coded by Snoopy																	**
 *********************************************************************************************************************/
void CGame::RequestHeldenianScroll(int iClientH, char * pData, uint32_t /*dwMsgSize*/) {
	char *cp, cData[256], cTmpName[21];
	int iScroll;
	class CItem * pItem;
	int iRet, iEraseReq, iRequiredConstruction;
	char cItemName[21];
	short * sp;
	uint16_t * wp;
	int * ip;
	uint32_t * dwp;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
	if (_iGetItemSpaceLeft(iClientH) == 0) {
		SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	cp = (char *) (pData + DEF_INDEX2_MSGTYPE + 2);
	std::memset(cTmpName, 0, sizeof (cTmpName));
	strcpy(cTmpName, cp);
	cp += 20;
	ip = (int *) cp;
	iScroll = (int) *ip;
	cp += 2;
	wsprintf(G_cTxt, "PC(%s) obtained a Scroll (%d).   %s(%d %d)", m_pClientList[iClientH]->m_cCharName, iScroll, m_pClientList[iClientH]->map_->m_cName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
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
		pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
		pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
		pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
		if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
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
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					m_pClientList[iClientH]->DeleteClient(true, true);
					break;
			}
			m_pClientList[iClientH]->m_iConstructionPoint -= iRequiredConstruction;
			m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, nullptr);
		} else {
			m_pClientList[iClientH]->map_->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);
			m_pClientList[iClientH]->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
			dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					m_pClientList[iClientH]->DeleteClient(true, true);
					break;
			}
		}
	} else {
		delete pItem;
		pItem = nullptr;
	}
}
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

void CGame::GetDkSet(int iClientH) {
	class CItem * pItem;
	int dkM[] = {706, 707, 708, 709, 710, 714, 915, -1};
	int dkW[] = {724, 725, 726, 728, 714, 709, 915, -1};
	int dmM[] = {711, 712, 713, 853, 717, 913, 915, -1};
	int dmW[] = {729, 730, 731, 853, 717, 914, 915, -1};
	int weapon, iItemID, i, iEraseReq;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_iLevel < 200) return;
	for (i = 0; i <= 7; i++) {
		pItem = new class CItem;
		iItemID = -1;
		if (m_pClientList[iClientH]->m_cSex == 1) {
			if (m_pClientList[iClientH]->m_iStr > 100) {
				iItemID = dkM[i];
			} else {
				iItemID = dmM[i];
			}
		} else {
			if (m_pClientList[iClientH]->m_iStr > 100) {
				iItemID = dkW[i];
			} else {
				iItemID = dmW[i];
			}
		}
		if (i == 7) {
			if (m_pClientList[iClientH]->m_iStr > m_pClientList[iClientH]->m_iInt) {
				if (m_pClientList[iClientH]->m_cSex == 1) {
					weapon = 709;
					weapon = 2000;
				} else if (m_pClientList[iClientH]->m_cSex == 2) {
					weapon = 709;
					weapon = 2000;
				}
			} else {
				if (m_pClientList[iClientH]->m_cSex == 1) {
					weapon = 714;
				} else if (m_pClientList[iClientH]->m_cSex == 2) {
					weapon = 714;
				}
			}
			iItemID = weapon;
		}
		if (iItemID != -1) //if any error occures, dont crash character
		{
			_bInitItemAttr(*pItem, iItemID);
			pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
			pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
			pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
			pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
			_bAddClientItemList(iClientH, pItem, &iEraseReq);
			SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, 0);
		}
	}
}

void CGame::AdminOrder_BanIP(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char cMsg[100], cNick[20], cIPtoBan[15];
	int i;
	FILE * pFile;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel <= 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
		return;
	}
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == false) return;
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
			wsprintf(cMsg, "%s was banned ip.", m_pClientList[iClientH]->m_cCharName);
			ShowClientMsg(iClientH, cMsg);
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

void CGame::AFKChecker() {
	register int i;
	int sX, sY;
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

void CGame::PlayerOrder_ShowDamage(int iClientH) {/*ShowDamage - Coded by EvilHit*/
	char cDamageMod[256];
	if (m_pClientList[iClientH]->iDmgShowon == 1) {
		m_pClientList[iClientH]->iDmgShowon = 0;
		std::memset(cDamageMod, 0, sizeof (cDamageMod));
		wsprintf(cDamageMod, "Show Damage is now disabled");
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
	} else {
		m_pClientList[iClientH]->iDmgShowon = 1;
		std::memset(cDamageMod, 0, sizeof (cDamageMod));
		wsprintf(cDamageMod, "Show Damage is now enabled");
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
	}
}

void CGame::PlayerOrder_GetCrits(CClient &client) {
	if (client.m_iSuperAttackLeft >= 300) {
		m_pClientList[client.id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You can't buy more Criticals!.");
		return;
	}
	if (client.m_iAdminUserLevel > 0) {
		client.m_iSuperAttackLeft += 300;
		m_pClientList[client.id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You bought Criticals");
		return;
	}
	uint32_t dwGoldCount;
	if (client.m_iLevel < 202) {
		dwGoldCount = dwGetItemCount(client.id_, "Gold");
		if (dwGoldCount >= 5000) {
			SetItemCount(client.id_, "Gold", dwGoldCount - 5000);
			client.m_iSuperAttackLeft += 300;
			m_pClientList[client.id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You bought Criticals!.");
			return;
		}
	}
	m_pClientList[client.id_]->SendNotifyMsg(0, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You haven't got the needed requirement to use this command!.");
}
//Fin Criticals By MIHD
// MORLA2.2 - Para determinar el Rango devuelve los EKs y PKs del user

void CGame::RequestRango(int iClientH, int iObjectID) {
	if (m_pClientList[iClientH] == nullptr) return;
	if ((iObjectID <= 0) || (iObjectID >= DEF_MAXCLIENTS)) return;
	if (m_pClientList[iObjectID] == nullptr) {
	} else {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_REQRANGO, m_pClientList[iObjectID]->m_iEnemyKillCount, m_pClientList[iObjectID]->m_iRating, 0, nullptr);
	}
}

void CGame::GetTradeEKMantleHandler(int iClientH, int iItemID, char */*pString*/) // TRADE EK - Luqah
{
	int i, iNum, iRet, iEraseReq;
	char * cp, cData[256], cItemName[21];
	class CItem * pItem;
	uint32_t * dwp;
	short * sp;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
	if (_iGetItemSpaceLeft(iClientH) == 0) {
		SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
		return;
	}
	if (m_pItemConfigList[iItemID] == nullptr) return;
	switch (iItemID) {
		case 848://Lighning Blade
			if (m_pClientList[iClientH]->m_iEnemyKillCount <= 7799) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 7800;
			break;
		case 616:// Demon Slayer
			if (m_pClientList[iClientH]->m_iEnemyKillCount <= 6999) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 7000;
			break;
		case 861://Berzerk Wand MS 20
			if (m_pClientList[iClientH]->m_iEnemyKillCount <= 6199) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 6200;
			break;
		case 863://Kloness Wand MS 20
			if (m_pClientList[iClientH]->m_iEnemyKillCount <= 6999) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 6000;
			break;
		case 3043://Strike of the Ghost
			if (m_pClientList[iClientH]->m_iEnemyKillCount <= 5999) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 6000;
			break;
		case 850://Kloness Axe
			if (m_pClientList[iClientH]->m_iEnemyKillCount <= 8999) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 9000;
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
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;
				wsprintf(G_cTxt, "(*) Get Trade EK Item : Char(%s) Player-EK(%d) Item Obtained(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iEnemyKillCount, cItemName);
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
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
				m_pClientList[iClientH]->iCalcTotalWeight();
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						return;
				}
				m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, 0, 0, nullptr);
			} else {
				delete pItem;
				m_pClientList[iClientH]->iCalcTotalWeight();
				dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						m_pClientList[iClientH]->DeleteClient(true, true);
						return;
				}
			}
		}
	}
}

void CGame::PlayerCommandAddMaster(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iGuildRank != 0) {
		// ???? ??????? ?????* ?? ?????? ??????? ????.
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, nullptr);
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
				if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0) {
					// ????? ???????? ???? ?????? ????????.
					m_pClientList[iClientH]->SendNotifyMsg(iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: Cannot Add Master...");
					delete pStrTok;
					return;
				}
				m_pClientList[i]->m_iGuildRank = 0;
				m_pClientList[iClientH]->SendNotifyMsg(iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "New Guild Master Added");
				m_pClientList[i]->SendNotifyMsg(iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "You are Now a GuildMaster");
			}
		delete pStrTok;
		return;
	}
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	delete pStrTok;
	return;
}

void CGame::PlayerOrder_DeleteGuildMaster(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cName[11], cTargetName[11], cBuff[256], cNpcName[21], cNpcWaypoint[11];
	class CStrTok * pStrTok;
	register int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iGuildRank != 0) {
		// ???? ??????? ?????* ?? ?????? ??????? ????.
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, nullptr);
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
				if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0) {
					// ????? ???????? ???? ?????? ????????.
					m_pClientList[iClientH]->SendNotifyMsg(iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Error: No se puede borrar el GuildMaster.");
					delete pStrTok;
					return;
				}
				m_pClientList[i]->m_iGuildRank = 12;
				m_pClientList[iClientH]->SendNotifyMsg(iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "El GuildMaster fue borrado.");
				m_pClientList[i]->SendNotifyMsg(iClientH, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, "Ya no eres GuildMaster.");
			}
		delete pStrTok;
		return;
	}
	m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
	delete pStrTok;
	return;
}

void CGame::AdminOrder_SetCrit(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char *token, cBuff[256];
	class CStrTok *pStrTok;
	char *pk;
	char cNick[20];
	char cPK[5];
	char notice[100];
	int oldpk;
	int i;
	if (m_pClientList[iClientH] == nullptr)
		return;
	if ((dwMsgSize) <= 0)
		return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			wsprintf(notice, "Crit Count has been changed for player %s from %d to %d.", m_pClientList[iClientH]->m_cCharName, oldpk, atoi(cPK));
		}
	}
	delete pStrTok;
}

void CGame::AdminOrder_SetEK(int iClientH, char * pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char * ek;
	char cNick[20];
	char cEK[5];
	char notice[100];
	int oldek;
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel <= 3) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			ShowClientMsg(iClientH, notice);
		}
	}
	delete pStrTok;
}

void CGame::AdminOrder_SetMAJ(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char *token, cBuff[256];
	class CStrTok *pStrTok;
	char *pk;
	char cNick[20];
	char cPK[5];
	char notice[100];
	int oldpk;
	int i;
	if (m_pClientList[iClientH] == nullptr)
		return;
	if ((dwMsgSize) <= 0)
		return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			wsprintf(notice, "Majestic Count has been changed for player %s from %d to %d.", m_pClientList[iClientH]->m_cCharName, oldpk, atoi(cPK));
		}
	}
	delete pStrTok;
}

void CGame::AdminOrder_SetPK(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char *token, cBuff[256];
	class CStrTok *pStrTok;
	char *pk;
	char cNick[20];
	char cPK[5];
	char notice[100];
	int oldpk;
	int i;
	if (m_pClientList[iClientH] == nullptr)
		return;
	if ((dwMsgSize) <= 0)
		return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			wsprintf(notice, "EK Count has been changed for player %s from %d to %d.", m_pClientList[iClientH]->m_cCharName, oldpk, atoi(cPK));
		}
	}
	delete pStrTok;
}

void CGame::AdminOrder_SetRep(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * token, cBuff[256];
	class CStrTok * pStrTok;
	char * rep;
	char cNick[20];
	char cRep[5];
	char notice[100];
	int oldrep;
	int i;
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
			wsprintf(notice, "REP Count has been changed for player %s from %d to %d.", m_pClientList[iClientH]->m_cCharName, oldrep, atoi(cRep));
		}
	}
	delete pStrTok;
}

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

void CGame::RequestBallItem(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * cp, * token, cBuff[256], cWantedItemName[256], cData[256], cInfoString[56];
	class CStrTok * pStrTok;
	class CItem * pItem;
	short * sp, sCounter;
	int i, iEraseReq;
	uint32_t * dwp;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
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
					if (m_pClientList[iClientH]->m_iBallPoints < m_pBallItemConfigList[i]->m_iReqPoints) {
						wsprintf(cInfoString, "Item (%s) cannot be traded, not enough points. ", cWantedItemName);
						m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_IPACCOUNTINFO, 0, 0, 0, cInfoString);
						return;
					}
					m_pClientList[iClientH]->m_iBallPoints -= m_pBallItemConfigList[i]->m_iReqPoints;
					pItem = new class CItem;
					if (_bInitItemAttr(*pItem, cWantedItemName) == false) {
						delete pItem;
						return;
					}
					std::memset(cData, 0, sizeof (cData));
					if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
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
						m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
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

/********************************************************************************
 *****    void CGame::_TradeItem(int iClientH, char *pData, uint32_t dwMsgSize)    *****
 *****    ----------------------------------------------------------------      *****
 *****    Creditos        :: By Centuu.- Modificado por Gigia para Ball Trade   *****
 *****    Descripcion :: Tradear balls por command                              *****
 *****    ----------------------------------------------------------------      *****
 *****    Llamada por el comando "/balltrade [ItemName]"                        *****
 ********************************************************************************/
void CGame::_TradeItem(int iClientH, char *pData, uint32_t dwMsgSize) {
	char seps[] = "= \t\n";
	char * cp, * token, cBuff[256], cItemName[256], cData[256];
	class CStrTok * pStrTok;
	class CItem * pItem;
	short * sp;
	int iEraseReq;
	uint32_t * dwp;
	uint16_t * wp;
	if (m_pClientList[iClientH] == nullptr) return;
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
	if (_iGetItemSpaceLeft(iClientH) == 0) { //Centuu : Dice, que si no tiene espacio en el bag, no le d? el item.
		SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, nullptr, 0);
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
			if (m_pClientList[iClientH]->m_iBallPoints < 20) return;
			m_pClientList[iClientH]->m_iBallPoints -= 20;
			break;
		case 621: //Merien Plates
		case 622:
			if (m_pClientList[iClientH]->m_iBallPoints < 850) return;
			m_pClientList[iClientH]->m_iBallPoints -= 850;
			break;
		case 977: //Merien Chains
		case 978:
			if (m_pClientList[iClientH]->m_iBallPoints < 820) return;
			m_pClientList[iClientH]->m_iBallPoints -= 820;
			break;
		case 650: //Zem
			if (m_pClientList[iClientH]->m_iBallPoints < 10) return;
			m_pClientList[iClientH]->m_iBallPoints -= 10;
			break;
		case 847: //Dark Executor.
			if (m_pClientList[iClientH]->m_iBallPoints < 1100) return;
			m_pClientList[iClientH]->m_iBallPoints -= 1100;
			break;
		case 858: //Neck Of Merien
		case 859: //Neck Of Kloness
		case 860: //Neck Of Xelima
			if (m_pClientList[iClientH]->m_iBallPoints < 700) return;
			m_pClientList[iClientH]->m_iBallPoints -= 700;
			break;
		case 630: //Ring of xelima
		case 631: //Ring of Abaddon
			if (m_pClientList[iClientH]->m_iBallPoints < 560) return;
			m_pClientList[iClientH]->m_iBallPoints -= 560;
		case 618: //Dark Elf Bow
			if (m_pClientList[iClientH]->m_iBallPoints < 750) return;
			m_pClientList[iClientH]->m_iBallPoints -= 750;
			break;
		case 620: //Merien Shield
			if (m_pClientList[iClientH]->m_iBallPoints < 850) return;
			m_pClientList[iClientH]->m_iBallPoints -= 850;
			break;
		case 979: //Sex Change Potion
			if (m_pClientList[iClientH]->m_iBallPoints < 35) return;
			m_pClientList[iClientH]->m_iBallPoints -= 35;
			break;
		case 970: //Xelima Esterk
		case 973: //Xelima B Hammer
		case 976: //Xelima Bow
			if (m_pClientList[iClientH]->m_iBallPoints < 500) return;
			m_pClientList[iClientH]->m_iBallPoints -= 500;
			break;
		default:
			return;
			break;
	}
	std::memset(cData, 0, sizeof (cData));
	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) { //Centuu : Dice que obtuvo el item.
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
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_BALLPOINTS, m_pClientList[iClientH]->m_iBallPoints, 0, 0, nullptr);
		m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
		return;
	} else {
		delete pItem;
		return;
	}
}

void CGame::PURITYLIFE(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3) {
	int iHP, iMaxHP, iSP, iMaxSP;
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

void CGame::DesbugCharacter(int iClientH, char * pData, uint32_t dwMsgSize) {
	CStrTok * pStrTok;
	char cBuff[256], cName[11], cTargetName[11], cFileNamex[255], cFileName[255], cDir[63];
	const char * token;
	char seps[] = "= \t\n";
	std::memset(cTargetName, 0, sizeof (cTargetName));
	if (m_pClientList[iClientH] == nullptr) return;
	if ((dwMsgSize) <= 0) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 2) {
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, nullptr);
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
//Happy para vos xD

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

void CGame::ManualStartHappyHour(int iClientH, char *pData, uint32_t dwMsgSize) {
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
	wsprintf(G_cTxt, "Admin Order(%s): begin Happy-Hour", m_pClientList[iClientH]->m_cCharName);
	PutAdminLogFileList(G_cTxt);
	PutLogList(G_cTxt);
	PutLogEventFileList(G_cTxt);
}
//50Cent - Capture The Flag

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

void CGame::RequestCheckFlag(int iClientH) {
	if (!m_bIsCTFMode) return;
	if (m_pClientList[iClientH] == nullptr) return;
	if (m_pClientList[iClientH]->m_cSide == 0) m_pClientList[iClientH]->RequestTeleportHandler("2   ", "default", -1, -1);
	if ((m_pClientList[iClientH]->m_sX == 151) && (m_pClientList[iClientH]->m_sY == 132))//Elvine Flag Cords
	{
		if (m_pClientList[iClientH]->m_iLevel >= 120) {
			if (m_pClientList[iClientH]->m_cSide == 1) //Aresden Side
			{
				if ((!m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured)) //Aresden Capture Elvine Flag
				{
					PutLogList("Capture The Flag: Aresden Captured Elvine Flag!");
					SetFlagCarrierFlag(iClientH, true);
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
		} else m_pClientList[iClientH]->RequestTeleportHandler("2   ", "elvine", -1, -1);
	} else if ((m_pClientList[iClientH]->m_sX == 151) && (m_pClientList[iClientH]->m_sY == 128))//Aresden Flag Cords
	{
		if (m_pClientList[iClientH]->m_iLevel >= 120) {
			if (m_pClientList[iClientH]->m_cSide == 2) //Elvine Side
			{
				if ((!m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured)) //Elvine Capture Aresden Flag
				{
					PutLogList("Capture The Flag: Elvine Captured Aresden Flag!");
					SetFlagCarrierFlag(iClientH, true);
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
		} else m_pClientList[iClientH]->RequestTeleportHandler("2   ", "aresden", -1, -1);
	}
}

bool CGame::bCheckIfIsFlagCarrier(int iClientH) {
	if (!m_bIsCTFMode) return false;
	if (m_iFlagCarrierIndex == -1) return false;
	if (m_pClientList[iClientH] == nullptr) return false;
	if (iClientH == m_iFlagCarrierIndex) return true;
	else return false;
}

void CGame::SetFlagCarrierFlag(int iClientH, bool bFlagMode) {
	if (!m_bIsCTFMode) return;
	if (m_pClientList[iClientH] == nullptr) return;
	if (bFlagMode) {
		PutLogList("Capture The Flag: New Flag Carrier!");
		m_iFlagCarrierIndex = iClientH;
		if (m_pClientList[iClientH]->m_cSide == 1) {
			m_bIsElvineFlagCaptured = true;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ARESDENCAPTUREDELVINEFLAG, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		} else {
			m_bIsAresdenFlagCaptured = true;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ELVINEFLAGBACKTOCH, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		}
		m_pClientList[iClientH]->m_iEnemyKillCount += 20;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, nullptr);
		if (m_pClientList[iClientH]->m_cSide == 1) m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x80000; //Aresden Flag
		else if (m_pClientList[iClientH]->m_cSide == 2) m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x40000; //Elvine Flag
	} else {
		PutLogList("Capture The Flag: Flag Carrier Reset.");
		m_iFlagCarrierIndex = -1;
		if (m_pClientList[iClientH]->m_cSide == 1) {
			m_bIsElvineFlagCaptured = false;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ELVINECAPTUREDARESDENFLAG, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		} else {
			m_bIsAresdenFlagCaptured = false;
			for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != nullptr) m_pClientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ARESDENFLAGBACKTOCH, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
		}
		m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x00001000;
	}
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

void CGame::CheckDenialServiceAttack(int iClientH, uint32_t dwClientTime) {
	uint32_t dwTime = timeGetTime();
	if (m_pClientList[iClientH] == nullptr) return;
	// Find a Denial of service attack by packet sent time
	if (m_pClientList[iClientH]->m_dwDSLAT == 0)
		// Start with 1st msg
	{
		m_pClientList[iClientH]->m_dwDSLAT = dwClientTime;
		m_pClientList[iClientH]->m_dwDSLATOld = dwClientTime;
		m_pClientList[iClientH]->m_dwDSLATS = dwTime;
		m_pClientList[iClientH]->m_iDSCount = 0;
	} else {
		if (dwClientTime >= m_pClientList[iClientH]->m_dwDSLAT)
			// current message was sent later than previous (normal case)
		{
			m_pClientList[iClientH]->m_dwDSLAT = dwClientTime;
		} else
			// current message was sent before previous
		{
			if (m_pClientList[iClientH]->m_dwDSLATOld == dwClientTime)
				// If we receive more late msg with same time
			{
				m_pClientList[iClientH]->m_iDSCount++;
				if (((dwTime - m_pClientList[iClientH]->m_dwDSLATS) > 10 * 1000)
						  && (m_pClientList[iClientH]->m_iDSCount > 5))
					// Receiving a "late" msg more than 10 sec after !
					// This is an attack!
				{
					wsprintf(G_cTxt, "DS check: PC(%s) - Denial of service attack! (Disc.) \tIP(%s)"
							  , m_pClientList[iClientH]->m_cCharName
							  , m_pClientList[iClientH]->m_cIPaddress);
					PutHackLogFileList(G_cTxt);
					PutLogList(G_cTxt);
					return;
				}
			} else
				// else this message become late msg
			{
				m_pClientList[iClientH]->m_dwDSLATOld = dwClientTime;
				m_pClientList[iClientH]->m_iDSCount = 1;
				m_pClientList[iClientH]->m_dwDSLATS = dwTime;
			}
		}
	}
}
//50Cent - 100% Skill

void CGame::PlayerOrder_GetSkills(int iClientH) {
	int i;
	for (i = 0; i < 23; i++) {
		m_pClientList[iClientH]->m_cSkillMastery[i] = 100;
		m_pClientList[iClientH]->SendNotifyMsg(0, DEF_NOTIFY_SKILL, i, m_pClientList[iClientH]->m_cSkillMastery[i], 0, nullptr);
	}
	ShowClientMsg(iClientH, "Enjoy your new skills!");
}