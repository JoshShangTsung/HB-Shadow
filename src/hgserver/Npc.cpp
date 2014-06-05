#include "Npc.h"
#include <cstring>
#include "Game.h"
extern char G_cTxt[512];

CNpc::CNpc(int id, CGame &game, const char * pName5) : id_(id), game_(game) {
	int i;
	std::memset(m_cName, 0, sizeof (m_cName));
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

void CNpc::RemoveEventNpc() {
	if (this->m_bIsKilled == true) return;
	this->m_bIsKilled = true;
	this->m_iHP = 0;
	this->m_iLastDamage = 0;
	this->m_dwRegenTime = 0;
	this->map_->m_iTotalAliveObject--;
	game_.ReleaseFollowMode(id_, DEF_OWNERTYPE_NPC);
	this->m_iTargetIndex = 0;
	this->m_cTargetType = 0;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, 1, 0);
	if (this->m_sAreaSize == 0) {
		this->map_->ClearOwner(10, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);
	} else {
		this->map_->ClearBigOwner(id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY, this->m_sAreaSize);
	}
	this->map_->SetDeadOwner(id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);
	this->m_cBehavior = 4;
	this->m_sBehaviorTurnCount = 0;
	this->m_dwDeadTime = timeGetTime();
}

void CNpc::_NpcBehavior_GrandMagicGenerator() {
	switch (this->m_cSide) {
		case 1:
			if (game_.m_iAresdenMana > DEF_GMGMANACONSUMEUNIT) {
				game_.m_iAresdenMana = 0;
				this->m_iManaStock++;
				if (this->m_iManaStock > this->m_iMaxMana) {
					game_.MeteorStrikeMsgHandler(1);
					this->m_iManaStock = 0;
					game_.m_iAresdenMana = 0;
				}
				wsprintf(G_cTxt, "(!) Aresden GMG %d/%d", this->m_iManaStock, this->m_iMaxMana);
				PutLogList(G_cTxt);
			}
			break;
		case 2:
			if (game_.m_iElvineMana > DEF_GMGMANACONSUMEUNIT) {
				game_.m_iElvineMana = 0;
				this->m_iManaStock++;
				if (this->m_iManaStock > this->m_iMaxMana) {
					game_.MeteorStrikeMsgHandler(2);
					this->m_iManaStock = 0;
					game_.m_iElvineMana = 0;
				}
				wsprintf(G_cTxt, "(!) Elvine GMG %d/%d", this->m_iManaStock, this->m_iMaxMana);
				PutLogList(G_cTxt);
			}
			break;
	}
}

bool CNpc::_bNpcBehavior_Detector() {
	int dX;
	int dY;
	short sOwnerH;
	char cOwnerType;
	char cSide;
	bool bFlag = false;
	if (this->m_sAppr2 != 0) return false;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	for (dX = this->m_sX - 10; dX <= this->m_sX + 10; dX++)
		for (dY = this->m_sY - 10; dY <= this->m_sY + 10; dY++) {
			this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			cSide = 0;
			if (sOwnerH != 0) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						cSide = clientList[sOwnerH]->m_cSide;
						break;
					case DEF_OWNERTYPE_NPC:
						cSide = npcList[sOwnerH]->m_cSide;
						break;
				}
			}
			if ((cSide != 0) && (cSide != this->m_cSide)) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (clientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
							clientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
							game_.SetInvisibilityFlag(sOwnerH, cOwnerType, false);
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (npcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
							npcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
							game_.SetInvisibilityFlag(sOwnerH, cOwnerType, false);
						}
						break;
				}
				bFlag = true;
			}
		}
	return bFlag;
}

void CNpc::NpcBehavior_Dead() {
	uint32_t dwTime = timeGetTime();
	this->m_sBehaviorTurnCount++;
	if (this->m_sBehaviorTurnCount > 5) {
		this->m_sBehaviorTurnCount = 0;
	}
	if ((dwTime - this->m_dwDeadTime) > this->m_dwRegenTime)
		DeleteNpc();
}

bool CNpc::_bNpcBehavior_ManaCollector() {
	int dX;
	int dY;
	int iMaxMP;
	int iTotal;
	short sOwnerH;
	char cOwnerType;
	double dV1;
	double dV2;
	double dV3;
	bool bRet;
	if (this->m_sAppr2 != 0) return false;
	bRet = false;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	for (dX = this->m_sX - 5; dX <= this->m_sX + 5; dX++) {
		for (dY = this->m_sY - 5; dY <= this->m_sY + 5; dY++) {
			this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (sOwnerH != 0) {
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
					{
						if (this->m_cSide == clientList[sOwnerH]->m_cSide) {
							iMaxMP = (2 * clientList[sOwnerH]->m_iMag) + (2 * clientList[sOwnerH]->m_iLevel) + (clientList[sOwnerH]->m_iInt / 2);
							if (clientList[sOwnerH]->m_iMP < iMaxMP) {
								iTotal = iDice(1, (clientList[sOwnerH]->m_iMag));
								if (clientList[sOwnerH]->m_iAddMP != 0) {
									dV2 = (double) iTotal;
									dV3 = (double) clientList[sOwnerH]->m_iAddMP;
									dV1 = (dV3 / 100.0f) * dV2;
									iTotal += (int) dV1;
								}
								clientList[sOwnerH]->m_iMP += iTotal;
								if (clientList[sOwnerH]->m_iMP > iMaxMP)
									clientList[sOwnerH]->m_iMP = iMaxMP;
								clientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MP, 0, 0, 0, nullptr);
							}
						}
					}
						break;
					case DEF_OWNERTYPE_NPC:
					{
						if ((npcList[sOwnerH]->m_sType == 42) && (npcList[sOwnerH]->m_iV1 > 0)) {
							if (npcList[sOwnerH]->m_iV1 >= 3) {
								game_.m_iCollectedMana[this->m_cSide] += 3;
								npcList[sOwnerH]->m_iV1 -= 3;
								bRet = true;
							} else {
								game_.m_iCollectedMana[this->m_cSide] += npcList[sOwnerH]->m_iV1;
								npcList[sOwnerH]->m_iV1 = 0;
								bRet = true;
							}
						}
					}
						break;
				}
			}
		}
	}
	return bRet;
}

bool CNpc::bCheckEnergySphereDestination(short sAttackerH, char cAttackerType) {
	int i;
	int sX;
	int sY;
	int dX;
	int dY;
	char cResult;
	if (this->map_->m_iCurEnergySphereGoalPointIndex == -1) return false;
	auto &clientList = game_.m_pClientList;
	if (this->map_ != game_.middlelandMap_.lock()) {
		MapPtr goalMap = this->map_;
		sX = this->m_sX;
		sY = this->m_sY;
		cResult = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].cResult;
		dX = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].aresdenX;
		dY = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].aresdenY;
		if ((sX >= dX - 2) && (sX <= dX + 2) && (sY >= dY - 2) && (sY <= dY + 2)) {
			goalMap->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (clientList[sAttackerH] != nullptr)) {
				if (clientList[sAttackerH]->m_cSide == 1) { // Aresden (Side:1)
					clientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Aresden Player (%s)", clientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					clientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((clientList[i] != nullptr) && (clientList[i]->m_bIsInitComplete == true)) {
						clientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, clientList[sAttackerH]->m_cSide, 2, clientList[sAttackerH]->m_cCharName);
					}
			}
			return true;
		}
		dX = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].elvineX;
		dY = goalMap->m_stEnergySphereGoalList[goalMap->m_iCurEnergySphereGoalPointIndex].elvineY;
		if ((sX >= dX - 2) && (sX <= dX + 2) && (sY >= dY - 2) && (sY <= dY + 2)) {
			goalMap->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (clientList[sAttackerH] != nullptr)) {
				if (clientList[sAttackerH]->m_cSide == 2) { // Elvine (Side:2)
					clientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Elvine Player (%s)", clientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					clientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((clientList[i] != nullptr) && (clientList[i]->m_bIsInitComplete == true)) {
						//
						clientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, clientList[sAttackerH]->m_cSide, 1, clientList[sAttackerH]->m_cCharName);
					}
			}
		}
		return false;
	} else {
		sX = this->m_sX;
		sY = this->m_sY;
		auto ml = game_.middlelandMap_.lock();
		cResult = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].cResult;
		dX = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].aresdenX;
		dY = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].aresdenY;
		if ((sX >= dX - 4) && (sX <= dX + 4) && (sY >= dY - 4) && (sY <= dY + 4)) {
			ml->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (clientList[sAttackerH] != nullptr)) {
				if (clientList[sAttackerH]->m_cSide == 1) { // Aresden (Side:1)
					clientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Aresden Player (%s)", clientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					clientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((clientList[i] != nullptr) && (clientList[i]->m_bIsInitComplete == true)) {
						clientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, clientList[sAttackerH]->m_cSide, 2, clientList[sAttackerH]->m_cCharName);
					}
			}
			return true;
		}
		dX = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].elvineX;
		dY = ml->m_stEnergySphereGoalList[ml->m_iCurEnergySphereGoalPointIndex].elvineY;
		if ((sX >= dX - 4) && (sX <= dX + 4) && (sY >= dY - 4) && (sY <= dY + 4)) {
			ml->m_iCurEnergySphereGoalPointIndex = -1;
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (clientList[sAttackerH] != nullptr)) {
				if (clientList[sAttackerH]->m_cSide == 2) { // Elvine (Side:2)
					clientList[sAttackerH]->m_iContribution += 5;
					wsprintf(G_cTxt, "(!) EnergySphere Hit By Aresden Player (%s)", clientList[sAttackerH]->m_cCharName);
					PutLogFileList(G_cTxt);
				} else {
					clientList[sAttackerH]->m_iContribution -= 10;
				}
				for (i = 1; i < DEF_MAXCLIENTS; i++)
					if ((clientList[i] != nullptr) && (clientList[i]->m_bIsInitComplete == true)) {
						//
						clientList[i]->SendNotifyMsg(0, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, clientList[sAttackerH]->m_cSide, 1, clientList[sAttackerH]->m_cCharName);
					}
			}
			return true;
		}
		return false;
	}
}

void CNpc::NpcDeadItemGenerator(short sAttackerH, char cAttackerType) {
	class CItem * pItem;
	char cColor;
	char cItemName[21];
	int iGenLevel;
	int iResult;
	int iItemID;
	uint32_t dwType;
	uint32_t dwValue;
	double dTmp1;
	double dTmp2;
	double dTmp3;
	if ((cAttackerType != DEF_OWNERTYPE_PLAYER) || (this->m_bIsSummoned == true)) return;
	if (this->m_bIsUnsummoned == true) return;
	std::memset(cItemName, 0, sizeof (cItemName));
	switch (this->m_sType) {
			// NPC not dropping Gold
		case 21: // Guard
		case 34: // Dummy
		case 64: // Crop
			return;
	}
	auto &clientList = game_.m_pClientList;
	// 6500 default; the lower the greater the Weapon/Armor/Wand Drop
	if (iDice(1, 10000) >= game_.m_iPrimaryDropRate) {
		// 35% Drop 60% of that is gold
		// 35% Chance of drop (35/100)
		if (iDice(1, 10000) <= 4000) {
			iItemID = 90; // Gold: (35/100) * (60/100) = 21%
			// If a non-existing itemID is given create no item
			pItem = new class CItem;
			if (game_._bInitItemAttr(*pItem, iItemID) == false) {
				delete pItem;
				return;
			}
			pItem->m_dwCount = (uint32_t) (iDice(1, (this->m_iGoldDiceMax - this->m_iGoldDiceMin)) + this->m_iGoldDiceMin);
			// v1.42 Gold
			if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (clientList[sAttackerH]->m_iAddGold != 0)) {
				dTmp1 = (double) clientList[sAttackerH]->m_iAddGold;
				dTmp2 = (double) pItem->m_dwCount;
				dTmp3 = (dTmp1 / 100.0f) * dTmp2;
				pItem->m_dwCount += (int) dTmp3;
			}
		} else {
			// 9000 default; the lower the greater the Weapon/Armor/Wand Drop
			// 35% Drop 40% of that is an Item
			dTmp1 = clientList[sAttackerH]->m_iRating * game_.m_cRepDropModifier;
			if (dTmp1 > 3000) dTmp1 = 3000;
			if (dTmp1 < -3000) dTmp1 = -3000;
			dTmp2 = (game_.m_iSecondaryDropRate - (dTmp1));
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
						if (((short) SysTime.wMonth == 12) && (this->m_sType == 61 || 55)) {
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
				if (game_._bInitItemAttr(*pItem, iItemID) == false) {
					delete pItem;
					return;
				}
			} else {
				// Valuable Drop Calculation: (35/100) * (40/100) * (10/100) = 1.4%
				// Define iGenLevel using Npc.cfg#
				switch (this->m_sType) {
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
				if (game_._bInitItemAttr(*pItem, iItemID) == false) {
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
				game_._AdjustRareItemValue(*pItem);
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
		this->map_->bSetItem(this->m_sX,
				  this->m_sY,
				  pItem);
		this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
				  this->m_sX, this->m_sY,
				  pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		this->_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, pItem);
	}
}

void CNpc::NpcRequestAssistance() {
	register int ix, iy, sX, sY;
	short sOwnerH;
	char cOwnerType;
	sX = this->m_sX;
	sY = this->m_sY;
	auto &npcList = game_.m_pNpcList;
	for (ix = sX - 8; ix <= sX + 8; ix++)
		for (iy = sY - 8; iy <= sY + 8; iy++) {
			this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
			if ((sOwnerH != 0) &&
					  (cOwnerType == DEF_OWNERTYPE_NPC) &&
					  (npcList[sOwnerH] != nullptr) &&
					  (sOwnerH != id_) &&
					  (npcList[sOwnerH]->m_cSide == this->m_cSide) &&
					  (npcList[sOwnerH]->m_bIsPermAttackMode == false) &&
					  (npcList[sOwnerH]->m_cBehavior == DEF_BEHAVIOR_MOVE)) {
				npcList[sOwnerH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				npcList[sOwnerH]->m_sBehaviorTurnCount = 0;
				npcList[sOwnerH]->m_iTargetIndex = this->m_iTargetIndex;
				npcList[sOwnerH]->m_cTargetType = this->m_cTargetType;
				return;
			}
		}
}

int CNpc::iGetNpcRelationship_SendEvent(int iOpponentH) {
	int iRet;
	auto &clientList = game_.m_pClientList;
	if (clientList[iOpponentH] == nullptr) return 0;
	if (clientList[iOpponentH]->m_bIsInitComplete == false) return 0;
	iRet = 0;
	if (clientList[iOpponentH]->m_iPKCount != 0) {
		if (this->m_cSide == clientList[iOpponentH]->m_cSide)
			iRet = 7;
		else iRet = 2;
	} else {
		if (this->m_cSide != clientList[iOpponentH]->m_cSide) {
			if (this->m_cSide == 10)
				iRet = 2;
			else
				if (this->m_cSide == 0) iRet = 0;
			else
				if (clientList[iOpponentH]->m_cSide == 0)
				iRet = 0;
			else iRet = 2;
		} else iRet = 1;
	}
	return iRet;
}

void CNpc::NpcMagicHandler(short dX, short dY, short sType) {
	short sOwnerH;
	char cOwnerType;
	register int i;
	int iErr;
	int ix;
	int iy;
	int sX;
	int sY;
	int tX;
	int tY;
	int iResult;
	int iWhetherBonus;
	int iMagicAttr;
	uint32_t dwTime = timeGetTime();
	if ((dX < 0) || (dX >= this->map_->m_sSizeX) ||
			  (dY < 0) || (dY >= this->map_->m_sSizeY)) return;
	if ((sType < 0) || (sType >= 100)) return;
	auto &magicCfg = game_.m_pMagicConfigList[sType];
	if (magicCfg == nullptr) return;

	if (this->map_->m_bIsAttackEnabled == false) return;

	iResult = this->m_iMagicHitRatio;

	iWhetherBonus = game_.iGetWhetherMagicBonusEffect(sType, this->map_->m_cWhetherStatus);

	iMagicAttr = magicCfg->m_iAttribute;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	if (magicCfg->m_dwDelayTime == 0) {

		switch (magicCfg->m_sType) {
			case DEF_MAGICTYPE_INVISIBILITY:
				switch (magicCfg->m_sValue4) {
					case 1:

						this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
						switch (cOwnerType) {
							case DEF_OWNERTYPE_PLAYER:
								if (clientList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
								if (clientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto NMH_NOEFFECT;
								clientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) magicCfg->m_sValue4;
								game_.SetInvisibilityFlag(sOwnerH, cOwnerType, true);
								game_.RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER);
								break;
							case DEF_OWNERTYPE_NPC:
								if (npcList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
								if (npcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) goto NMH_NOEFFECT;
								npcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = (char) magicCfg->m_sValue4;
								game_.SetInvisibilityFlag(sOwnerH, cOwnerType, true);

								game_.RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC);
								break;
						}

						game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (magicCfg->m_dwLastTime * 1000),
								  sOwnerH, cOwnerType, 0, 0, 0, magicCfg->m_sValue4, 0, 0);
						if (cOwnerType == DEF_OWNERTYPE_PLAYER)
							clientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, magicCfg->m_sValue4, 0, nullptr);
						break;
					case 2:

						for (ix = dX - 8; ix <= dX + 8; ix++)
							for (iy = dY - 8; iy <= dY + 8; iy++) {
								this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
								if (sOwnerH != 0) {
									switch (cOwnerType) {
										case DEF_OWNERTYPE_PLAYER:
											if (clientList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
											if (clientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												if (clientList[sOwnerH]->m_sType != 66) {
													clientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
													game_.SetInvisibilityFlag(sOwnerH, cOwnerType, false);
													game_.delayEvents_.remove(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
												}
											}
											break;
										case DEF_OWNERTYPE_NPC:
											if (npcList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
											if (npcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] != 0) {
												if (clientList[sOwnerH]->m_sType != 66) {
													npcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ] = 0;
													game_.SetInvisibilityFlag(sOwnerH, cOwnerType, false);
													game_.delayEvents_.remove(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
												}
											}
											break;
									}
								}
							}
						break;
				}
				break;
			case DEF_MAGICTYPE_HOLDOBJECT:

				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
					switch (cOwnerType) {
						case DEF_OWNERTYPE_PLAYER:
							if (clientList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
							if (clientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto NMH_NOEFFECT;
							if (clientList[sOwnerH]->m_iAddPR >= 500) goto NMH_NOEFFECT;
							clientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) magicCfg->m_sValue4;
							break;
						case DEF_OWNERTYPE_NPC:
							if (npcList[sOwnerH] == nullptr) goto NMH_NOEFFECT;
							if (npcList[sOwnerH]->m_cMagicLevel >= 6) goto NMH_NOEFFECT;
							if (npcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) goto NMH_NOEFFECT;
							npcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = (char) magicCfg->m_sValue4;
							break;
					}

					game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (magicCfg->m_dwLastTime * 1000),
							  sOwnerH, cOwnerType, 0, 0, 0, magicCfg->m_sValue4, 0, 0);

					if (cOwnerType == DEF_OWNERTYPE_PLAYER)
						clientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, magicCfg->m_sValue4, 0, nullptr);
				}
				break;
			case DEF_MAGICTYPE_DAMAGE_LINEAR:

				sX = this->m_sX;
				sY = this->m_sY;
				for (i = 2; i < 10; i++) {
					iErr = 0;
					game_.m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
					// tx, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
							  (clientList[sOwnerH]->m_iHP > 0)) {
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx-1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
							  (clientList[sOwnerH]->m_iHP > 0)) {
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx+1, ty
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
							  (clientList[sOwnerH]->m_iHP > 0)) {
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx, ty-1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
							  (clientList[sOwnerH]->m_iHP > 0)) {
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					// tx, ty+1
					this->map_->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
					if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
							  (clientList[sOwnerH]->m_iHP > 0)) {
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
					}
					if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
				}

				for (iy = dY - magicCfg->m_sValue3; iy <= dY + magicCfg->m_sValue3; iy++)
					for (ix = dX - magicCfg->m_sValue2; ix <= dX + magicCfg->m_sValue2; ix++) {

						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
								  (clientList[sOwnerH]->m_iHP > 0)) {
							if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				// dX, dY
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6 + iWhetherBonus, false, iMagicAttr);
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
						  (clientList[sOwnerH]->m_iHP > 0)) {
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6 + iWhetherBonus, false, iMagicAttr);
				}
				break;
			case DEF_MAGICTYPE_DAMAGE_SPOT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
						  (clientList[sOwnerH]->m_iHP > 0)) {
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}
				break;
			case DEF_MAGICTYPE_HPUP_SPOT:

				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				game_.Effect_HpUp_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6);
				break;
			case DEF_MAGICTYPE_PURITYLIFE_SPOT:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				game_.PURITYLIFE(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6);
				break;
			case DEF_MAGICTYPE_DAMAGE_AREA:
				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
				if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
						  (clientList[sOwnerH]->m_iHP > 0)) {
					if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
						game_.Effect_Damage_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6 + iWhetherBonus, true, iMagicAttr);
				}

				for (iy = dY - magicCfg->m_sValue3; iy <= dY + magicCfg->m_sValue3; iy++)
					for (ix = dX - magicCfg->m_sValue2; ix <= dX + magicCfg->m_sValue2; ix++) {

						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot_DamageMove(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
								  (clientList[sOwnerH]->m_iHP > 0)) {
							if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								game_.Effect_Damage_Spot_DamageMove(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;
			case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:

				for (iy = dY - magicCfg->m_sValue3; iy <= dY + magicCfg->m_sValue3; iy++)
					for (ix = dX - magicCfg->m_sValue2; ix <= dX + magicCfg->m_sValue2; ix++) {

						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_Damage_Spot_DamageMove(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						this->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (clientList[sOwnerH] != nullptr) &&
								  (clientList[sOwnerH]->m_iHP > 0)) {
							if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
								game_.Effect_Damage_Spot_DamageMove(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9 + iWhetherBonus, false, iMagicAttr);
						}
					}
				break;
			case DEF_MAGICTYPE_SPDOWN_AREA:

				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
				if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
					game_.Effect_SpDown_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6);

				for (iy = dY - magicCfg->m_sValue3; iy <= dY + magicCfg->m_sValue3; iy++)
					for (ix = dX - magicCfg->m_sValue2; ix <= dX + magicCfg->m_sValue2; ix++) {

						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if (game_.bCheckResistingMagicSuccess(this->m_cDir, sOwnerH, cOwnerType, iResult) == false)
							game_.Effect_SpDown_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9);
					}
				break;
			case DEF_MAGICTYPE_SPUP_AREA:

				this->map_->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

				game_.Effect_SpUp_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue4, magicCfg->m_sValue5, magicCfg->m_sValue6);

				for (iy = dY - magicCfg->m_sValue3; iy <= dY + magicCfg->m_sValue3; iy++)
					for (ix = dX - magicCfg->m_sValue2; ix <= dX + magicCfg->m_sValue2; ix++) {

						this->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);

						game_.Effect_SpUp_Spot(id_, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, magicCfg->m_sValue7, magicCfg->m_sValue8, magicCfg->m_sValue9);
					}
				break;
		}
	} else {

	}
NMH_NOEFFECT:
	;

	this->m_iMana -= magicCfg->m_sValue1;
	if (this->m_iMana < 0)
		this->m_iMana = 0;

	this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC,
			  this->m_sX, this->m_sY, dX, dY, (sType + 100), this->m_sType);
}

void CNpc::NpcBehavior_Stop() {
	char cTargetType;
	short sTarget = 0;
	bool bFlag;
	this->m_sBehaviorTurnCount++;
	switch (this->m_cActionLimit) {
		case 5:
			switch (this->m_sType) {
				case 38:
					if (this->m_sBehaviorTurnCount >= 3) {
						this->m_sBehaviorTurnCount = 0;
						bFlag = this->_bNpcBehavior_ManaCollector();
						if (bFlag == true) {
							this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, this->m_sX, this->m_sY, 1);
						}
					}
					break;
				case 39: // Detector
					if (this->m_sBehaviorTurnCount >= 3) {
						this->m_sBehaviorTurnCount = 0;
						bFlag = this->_bNpcBehavior_Detector();
						if (bFlag == true) {
							this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, this->m_sX, this->m_sY, 1);
						}
					}
					break;
				case 40: // Energy Shield Generator
					break;
				case 41: // Grand Magic Generator
					if (this->m_sBehaviorTurnCount >= 3) {
						this->m_sBehaviorTurnCount = 0;
						this->_NpcBehavior_GrandMagicGenerator();
					}
					break;
				case 42:
					this->m_sBehaviorTurnCount = 0;
					this->m_iV1 += 5;
					if (this->m_iV1 >= 5) this->m_iV1 = 5;
					break;
				default:
					this->TargetSearch(&sTarget, &cTargetType);
					break;
			}
			break;
	}
	if ((sTarget != 0)) {

		this->m_cBehavior = DEF_BEHAVIOR_ATTACK;
		this->m_sBehaviorTurnCount = 0;
		this->m_iTargetIndex = sTarget;
		this->m_cTargetType = cTargetType;

		return;
	}
}

void CNpc::DeleteNpc() {
	int i;
	int iNamingValue;
	int iNumItem;
	int iItemID;
	int iItemIDs[MAX_NPCITEMDROP], iSlateID;
	char cTmp[21], cItemName[21];
	class CItem * pItem, * pItem2;
	uint32_t dwCount;
	uint32_t dwTime;
	POINT ItemPositions[MAX_NPCITEMDROP];
	char cTemp[256];
	SYSTEMTIME SysTime;
	dwTime = timeGetTime();
	//Init number of items to 1 unless its a multidrop;
	iNumItem = 0;
	iItemID = 0; // No current item
	this->SendEventToNearClient_TypeA(MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
	this->map_->ClearOwner(11, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);
	std::memset(cTmp, 0, sizeof (cTmp));
	strcpy(cTmp, (char *) (this->m_cName + 2));
	iNamingValue = atoi(cTmp);
	this->map_->SetNamingValueEmpty(iNamingValue);
	this->map_->m_iTotalActiveObject--;
	if (this->m_iSpotMobIndex != 0)
		this->map_->m_stSpotMobGenerator[this->m_iSpotMobIndex].iCurMobs--;
	game_.RemoveFromTarget(id_, DEF_OWNERTYPE_NPC);
	switch (this->m_sType) {
		case 36:
		case 37:
		case 38:
		case 39:
		case 42:
			this->map_->bRemoveCrusadeStructureInfo(this->m_sX, this->m_sY);
			for (i = 0; i < DEF_MAXGUILDS; i++)
				if (game_.m_pGuildTeleportLoc[i].m_iV1 == this->m_iGuildGUID) {
					game_.m_pGuildTeleportLoc[i].m_dwTime = dwTime;
					game_.m_pGuildTeleportLoc[i].m_iV2--;
					if (game_.m_pGuildTeleportLoc[i].m_iV2 < 0) game_.m_pGuildTeleportLoc[i].m_iV2 = 0;
					break;
				}
			break;
		case 64: this->map_->bRemoveCropsTotalSum();
			break;
	}
	game_.delayEvents_.remove(id_, DEF_OWNERTYPE_NPC, 0);
	if ((this->m_bIsSummoned == false) && (this->m_bIsUnsummoned == false)) {
		pItem = new class CItem;
		std::memset(cItemName, 0, sizeof (cItemName));
		switch (this->m_sType) {
			case 10: // Slime
				if (iDice(1, 25) == 1) iItemID = 220;
				break; // SlimeJelly
				break;
			case 11: // Skeleton
				switch (iDice(1, 2)) {
					case 1:if (iDice(1, 20) == 1) iItemID = 219;
						break; // SkeletonBones
					case 2: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 12: // Stone-Golem
				switch (iDice(1, 2)) {
					case 1: if (iDice(1, 30) == 1) iItemID = 221;
						break; // StoneGolemPiece
					case 2: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 13: // Cyclops
				switch (iDice(1, 6)) {
					case 1: if (iDice(1, 36) == 1) iItemID = 194;
						break; // CyclopsEye
					case 2: if (iDice(1, 40) == 1) iItemID = 195;
						break; // CyclopsHandEdge
					case 3: if (iDice(1, 30) == 1) iItemID = 196;
						break; // CyclopsHeart
					case 4: if (iDice(1, 22) == 1) iItemID = 197;
						break; // CyclopsMeat
					case 5: if (iDice(1, 40) == 1) iItemID = 198;
						break; // CyclopsLeather
					case 6: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 14: // Orc, Orc-Mage
				switch (iDice(1, 4)) {
					case 1: if (iDice(1, 11) == 1) iItemID = 206;
						break; // OrcMeat
					case 2: if (iDice(1, 20) == 1) iItemID = 207;
						break; // OrcLeather
					case 3: if (iDice(1, 21) == 1) iItemID = 208;
						break; // OrcTeeth
					case 4: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 16: //Giant-Ant
				switch (iDice(1, 3)) {
					case 1: if (iDice(1, 9) == 1) iItemID = 192;
						break; // AntLeg
					case 2: if (iDice(1, 10) == 1) iItemID = 193;
						break; // AntFeeler
					case 3: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 17: //Scorpion
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 50) == 1) iItemID = 215;
						break; // ScorpionPincers
					case 2: if (iDice(1, 20) == 1) iItemID = 216;
						break; // ScorpionMeat
					case 3: if (iDice(1, 50) == 1) iItemID = 217;
						break; // ScorpionSting
					case 4: if (iDice(1, 40) == 1) iItemID = 218;
						break; // ScorpionSkin
					case 5: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 18: //Zombie
				game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
				break;
				break;
			case 22: //Amphis
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 15) == 1) iItemID = 188;
						break; // SnakeMeat
					case 2: if (iDice(1, 16) == 1) iItemID = 189;
						break; // SnakeSkin
					case 3: if (iDice(1, 16) == 1) iItemID = 190;
						break; // SnakeTeeth
					case 4: if (iDice(1, 17) == 1) iItemID = 191;
						break; // SnakeTongue
					case 5: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 23: //Clay-Golem
				switch (iDice(1, 2)) {
					case 1: if (iDice(1, 30) == 1) iItemID = 205;
						break; // LumpofClay
					case 2: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 27: //Hellbound
				switch (iDice(1, 7)) {
					case 1: if (iDice(1, 40) == 1) iItemID = 199;
						break; // HelboundHeart
					case 2: if (iDice(1, 38) == 1) iItemID = 200;
						break; // HelboundLeather
					case 3: if (iDice(1, 38) == 1) iItemID = 201;
						break; // HelboundTail
					case 4: if (iDice(1, 36) == 1) iItemID = 202;
						break; // HelboundTeeth
					case 5: if (iDice(1, 36) == 1) iItemID = 203;
						break; // HelboundClaw
					case 6: if (iDice(1, 50) == 1) iItemID = 204;
						break; // HelboundTongue
					case 7: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 28: //Troll
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 35) == 1) iItemID = 222;
						break; // TrollHeart
					case 2: if (iDice(1, 23) == 1) iItemID = 223;
						break; // TrollMeat
					case 3: if (iDice(1, 25) == 1) iItemID = 224;
						break; // TrollLeather
					case 4: if (iDice(1, 27) == 1) iItemID = 225;
						break; // TrollClaw
					case 5: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 29: //Orge
				switch (iDice(1, 7)) {
					case 1: if (iDice(1, 20) == 1) iItemID = 209;
						break; // OgreHair
					case 2: if (iDice(1, 22) == 1) iItemID = 210;
						break; // OgreHeart
					case 3: if (iDice(1, 25) == 1) iItemID = 211;
						break; // OgreMeat
					case 4: if (iDice(1, 25) == 1) iItemID = 212;
						break; // OgreLeather
					case 5: if (iDice(1, 28) == 1) iItemID = 213;
						break; // OgreTeeth
					case 6: if (iDice(1, 28) == 1) iItemID = 214;
						break; // OgreClaw
					case 7: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 30: //Liche
				game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
				break;
				break;
			case 31: //Demon
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 400) == 123) iItemID = 541;
						break; // DemonHeart
					case 2: if (iDice(1, 1000) == 123) iItemID = 542;
						break; // DemonMeat
					case 3: if (iDice(1, 200) == 123) iItemID = 543;
						break; // DemonLeather
					case 4: if (iDice(1, 300) == 123) iItemID = 540;
						break; // DemonEye
					case 5: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 32: //Unicorn
				switch (iDice(1, 5)) {
					case 1: if (iDice(1, 3000) == 396) iItemID = 544;
						break; // UnicornHeart
					case 2: if (iDice(1, 500) == 3) iItemID = 545;
						break; // UnicornHorn
					case 3: if (iDice(1, 100) == 3) iItemID = 546;
						break; // UnicornMeat
					case 4: if (iDice(1, 200) == 3) iItemID = 547;
						break; // UnicornLeather
					case 5: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 33: //WereWolf
				switch (iDice(1, 8)) {
					case 1: if (iDice(1, 30) == 3) iItemID = 551;
						break; // WerewolfTail
					case 2: if (iDice(1, 28) == 3) iItemID = 548;
						break; // WerewolfHeart
					case 3: if (iDice(1, 25) == 3) iItemID = 550;
						break; // WerewolfMeat
					case 4: if (iDice(1, 35) == 3) iItemID = 553;
						break; // WerewolfLeather
					case 5: if (iDice(1, 28) == 3) iItemID = 552;
						break; // WerewolfTeeth
					case 6: if (iDice(1, 28) == 3) iItemID = 554;
						break; // WerewolfClaw
					case 7: if (iDice(1, 38) == 3) iItemID = 549;
						break; // WerewolfNail
					case 8: game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
						break;
					default: break;
				}
				break;
			case 48: //Stalker
			case 49: //Hellclaw
			case 50: //Tigerworm
			case 52: //Gagoyle
			case 53: //Beholder
			case 54: //Dark-Elf
			case 55: //Rabbit
			case 56: //Cat
			case 57: //Giant-Frog
			case 58: //Mountain-Giant
			case 59: //Ettin
			case 60: //Cannibal-Plant
			case 61: //Rudolph
			case 62: //DireBoar
			case 63: //Frost
			case 65: //Ice
				game_.bGetItemNameWhenDeleteNpc(iItemID, this->m_sType);
				break;
				// new 05/10/2004
			case 66: // Wyvern
				game_.bGetMultipleItemNamesWhenDeleteNpc(this->m_sType, // NPC Type
						  50,
						  5,
						  15,
						  this->m_sX,
						  this->m_sY,
						  DEF_ITEMSPREAD_FIXED,
						  4,
						  iItemIDs,
						  ItemPositions,
						  &iNumItem);
				break;
			case 73: // Fire-Wyvern
				game_.bGetMultipleItemNamesWhenDeleteNpc(this->m_sType, // NPC Type
						  50,
						  5,
						  15,
						  this->m_sX,
						  this->m_sY,
						  2,
						  4,
						  iItemIDs,
						  ItemPositions,
						  &iNumItem);
				break;
			case 81:
				game_.bGetMultipleItemNamesWhenDeleteNpc(this->m_sType, // NPC Type
						  50,
						  12,
						  20,
						  this->m_sX,
						  this->m_sY,
						  DEF_ITEMSPREAD_FIXED,
						  65,
						  iItemIDs,
						  ItemPositions,
						  &iNumItem);
				break;
		}
		dwCount = 1;
		if (iNumItem > 0) {
			GetLocalTime(&SysTime);
			wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
			for (int j = 0; j < iNumItem; j++) {
				if (pItem == nullptr) {
					pItem = new class CItem;
				}
				if (game_._bInitItemAttr(*pItem, iItemIDs[j]) == false ||
						  this->map_->bGetIsMoveAllowedTile(ItemPositions[j].x, ItemPositions[j].y) == false) {
					delete pItem;
					pItem = nullptr;
				} else {
					if (iItemIDs[j] == 90) // Gold
						pItem->m_dwCount = iDice(10, 15000);
					else
						pItem->m_dwCount = dwCount;
					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = (short) timeGetTime();
					this->map_->bSetItem(ItemPositions[j].x, ItemPositions[j].y, pItem);
					this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
							  ItemPositions[j].x, ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
					this->_bItemLog(DEF_ITEMLOG_NEWGENDROP, this->m_cNpcName, pItem);
					pItem = nullptr;
				}
			}
		} else {
			if (game_._bInitItemAttr(*pItem, iItemID) == false) {
				delete pItem;
				pItem = nullptr;
			} else {
				pItem->m_dwCount = dwCount;
				pItem->m_sTouchEffectType = DEF_ITET_ID;
				pItem->m_sTouchEffectValue1 = iDice(1, 100000);
				pItem->m_sTouchEffectValue2 = iDice(1, 100000);
				pItem->m_sTouchEffectValue3 = (short) timeGetTime();
				this->map_->bSetItem(this->m_sX, this->m_sY, pItem);
				this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
						  this->m_sX, this->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
				this->_bItemLog(DEF_ITEMLOG_NEWGENDROP, this->m_cNpcName, pItem);
			}
		}
		if (iDice(1, 100000) < 10) {
			pItem2 = new class CItem;
			switch (iDice(1, 4)) {
				case 1: iSlateID = 868;
					break;
				case 2: iSlateID = 869;
					break;
				case 3: iSlateID = 870;
					break;
				case 4: iSlateID = 871;
					break;
			}
			if (game_._bInitItemAttr(*pItem2, iSlateID) == false) {
				delete pItem2;
				pItem2 = nullptr;
			} else {
				pItem2->m_dwCount = 1;
				pItem2->m_sTouchEffectType = DEF_ITET_ID;
				pItem2->m_sTouchEffectValue1 = iDice(1, 100000);
				pItem2->m_sTouchEffectValue2 = iDice(1, 100000);
				pItem2->m_sTouchEffectValue3 = (short) timeGetTime();
				this->map_->bSetItem(this->m_sX, this->m_sY, pItem2);
				this->map_->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP,
						  this->m_sX, this->m_sY, pItem2->m_sSprite, pItem2->m_sSpriteFrame, pItem2->m_cItemColor);
				this->_bItemLog(DEF_ITEMLOG_NEWGENDROP, this->m_cNpcName, pItem2);
			}
		}
	}
	markedForDeletion_ = true;
}

void CNpc::CalcNextWayPointDestination() {
	short sRange;
	short sX;
	short sY;
	int i;
	int j;
	bool bFlag;
	switch (this->m_cMoveType) {
		case DEF_MOVETYPE_GUARD:
			break;
		case DEF_MOVETYPE_SEQWAYPOINT:
			this->m_cCurWaypoint++;
			if (this->m_cCurWaypoint >= this->m_cTotalWaypoint)
				this->m_cCurWaypoint = 1;
			this->m_dX = (short) (this->map_->m_WaypointList[ this->m_iWayPointIndex[ this->m_cCurWaypoint ] ].x);
			this->m_dY = (short) (this->map_->m_WaypointList[ this->m_iWayPointIndex[ this->m_cCurWaypoint ] ].y);
			break;
		case DEF_MOVETYPE_RANDOMWAYPOINT:
			this->m_cCurWaypoint = (short) ((rand() % (this->m_cTotalWaypoint - 1)) + 1);
			this->m_dX = (short) (this->map_->m_WaypointList[ this->m_iWayPointIndex[ this->m_cCurWaypoint ] ].x);
			this->m_dY = (short) (this->map_->m_WaypointList[ this->m_iWayPointIndex[ this->m_cCurWaypoint ] ].y);
			break;
		case DEF_MOVETYPE_RANDOMAREA:
			sRange = (short) (this->m_rcRandomArea.right - this->m_rcRandomArea.left);
			this->m_dX = (short) ((rand() % sRange) + this->m_rcRandomArea.left);
			sRange = (short) (this->m_rcRandomArea.bottom - this->m_rcRandomArea.top);
			this->m_dY = (short) ((rand() % sRange) + this->m_rcRandomArea.top);
			break;
		case DEF_MOVETYPE_RANDOM:
		{
			//this->m_dX = (rand() % (this->map_->m_sSizeX - 50)) + 15;
			//this->m_dY = (rand() % (this->map_->m_sSizeY - 50)) + 15;
			auto map = this->map_;
			for (i = 0; i <= 30; i++) {
				sX = (rand() % (map->m_sSizeX - 50)) + 15;
				sY = (rand() % (map->m_sSizeY - 50)) + 15;
				bFlag = true;
				for (j = 0; j < DEF_MAXMGAR; j++)
					if (map->m_rcMobGenAvoidRect[j].left != -1) {
						if ((sX >= map->m_rcMobGenAvoidRect[j].left) &&
								  (sX <= map->m_rcMobGenAvoidRect[j].right) &&
								  (sY >= map->m_rcMobGenAvoidRect[j].top) &&
								  (sY <= map->m_rcMobGenAvoidRect[j].bottom)) {
							bFlag = false;
						}
					}
				if (bFlag == true) goto CNW_GET_VALIDLOC_SUCCESS;
			}
			// Fail!
			this->m_tmp_iError = 0;
			return;
CNW_GET_VALIDLOC_SUCCESS:
			;
			this->m_dX = sX;
			this->m_dY = sY;
		}
			break;
	}
	this->m_tmp_iError = 0; // @@@ !!! @@@
}

int CNpc::iGetDangerValue(short dX, short dY) {
	register int ix, iy, iDangerValue;
	short sOwner;
	short sDOType;
	char cOwnerType;
	uint32_t dwRegisterTime;
	iDangerValue = 0;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	for (ix = dX - 2; ix <= dX + 2; ix++)
		for (iy = dY - 2; iy <= dY + 2; iy++) {
			this->map_->GetOwner(&sOwner, &cOwnerType, ix, iy);
			this->map_->bGetDynamicObject(ix, iy, &sDOType, &dwRegisterTime);
			if (sDOType == 1) iDangerValue++;
			switch (cOwnerType) {
				case 0:
					break;
				case DEF_OWNERTYPE_PLAYER:
					if (clientList[sOwner] == nullptr) break;
					if (this->m_cSide != clientList[sOwner]->m_cSide)
						iDangerValue++;
					else iDangerValue--;
					break;
				case DEF_OWNERTYPE_NPC:
					if (npcList[sOwner] == nullptr) break;
					if (this->m_cSide != npcList[sOwner]->m_cSide)
						iDangerValue++;
					else iDangerValue--;
					break;
			}
		}
	return iDangerValue;
}

void CNpc::NpcBehavior_Flee() {
	char cDir;
	short sX;
	short sY;
	short dX;
	short dY;
	short sTarget;
	char cTargetType;
	if (this->m_bIsKilled == true) return;

	this->m_sBehaviorTurnCount++;

	switch (this->m_iAttackStrategy) {
		case DEF_ATTACKAI_EXCHANGEATTACK:
		case DEF_ATTACKAI_TWOBYONEATTACK:
			if (this->m_sBehaviorTurnCount >= 2) {

				this->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				this->m_sBehaviorTurnCount = 0;
				return;
			}
			break;
		default:
			if (iDice(1, 2) == 1) this->NpcRequestAssistance();
			break;
	}
	if (this->m_sBehaviorTurnCount > 10) {

		this->m_sBehaviorTurnCount = 0;
		this->m_cBehavior = DEF_BEHAVIOR_MOVE;
		this->m_tmp_iError = 0;
		if (this->m_iHP <= 3) {
			this->m_iHP += iDice(1, this->m_iHitDice);
			if (this->m_iHP <= 0) this->m_iHP = 1;
		}
		return;
	}
	this->TargetSearch(&sTarget, &cTargetType);
	if (sTarget != 0) {
		this->m_iTargetIndex = sTarget;
		this->m_cTargetType = cTargetType;
	}

	sX = this->m_sX;
	sY = this->m_sY;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	switch (this->m_cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			dX = clientList[this->m_iTargetIndex]->m_sX;
			dY = clientList[this->m_iTargetIndex]->m_sY;
			break;
		case DEF_OWNERTYPE_NPC:
			dX = npcList[this->m_iTargetIndex]->m_sX;
			dY = npcList[this->m_iTargetIndex]->m_sY;
			break;
	}
	dX = sX - (dX - sX);
	dY = sY - (dY - sY);
	cDir = this->map_->cGetNextMoveDir(sX, sY, dX, dY, this->m_cTurn, &this->m_tmp_iError);
	if (cDir == 0) {

	} else {
		dX = this->m_sX + _tmp_cTmpDirX[cDir];
		dY = this->m_sY + _tmp_cTmpDirY[cDir];

		this->map_->ClearOwner(11, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);

		this->map_->SetOwner(id_, DEF_OWNERTYPE_NPC, dX, dY);
		this->m_sX = dX;
		this->m_sY = dY;
		this->m_cDir = cDir;
		this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
	}
}

void CNpc::NpcKilledHandler(short sAttackerH, char cAttackerType, short sDamage) {
	short sAttackerWeapon;
	int i;
	int iQuestIndex;
	int iExp;
	int iConstructionPoint;
	int iWarContribution;
	double dTmp1;
	double dTmp2;
	double dTmp3;
	char cQuestRemain;
	if (this->m_bIsKilled == true) return;
	this->m_bIsKilled = true;
	this->m_iHP = 0;
	this->m_iLastDamage = sDamage;
	this->map_->m_iTotalAliveObject--;
	game_.RemoveFromTarget(id_, DEF_OWNERTYPE_NPC);
	game_.ReleaseFollowMode(id_, DEF_OWNERTYPE_NPC);
	this->m_iTargetIndex = 0;
	this->m_cTargetType = 0;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		sAttackerWeapon = ((clientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
	} else sAttackerWeapon = 1;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
	this->map_->ClearOwner(10, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);
	this->map_->SetDeadOwner(id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);
	this->m_cBehavior = DEF_BEHAVIOR_DEAD;
	this->m_sBehaviorTurnCount = 0;
	this->m_dwDeadTime = timeGetTime();
	if (this->map_->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;
	this->NpcDeadItemGenerator(sAttackerH, cAttackerType);
	if ((this->m_bIsSummoned != true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) &&
			  (clientList[sAttackerH] != nullptr)) {
		//Happy Hour para vos
		if (game_.m_bHappyHour == true) {
			iExp = (this->m_iExp);
		} else {
			iExp = (this->m_iExp / 3);
		}
		if (this->m_iNoDieRemainExp > 0)
			iExp += this->m_iNoDieRemainExp;
		if (clientList[sAttackerH]->m_iAddExp != 0) {
			dTmp1 = (double) clientList[sAttackerH]->m_iAddExp;
			dTmp2 = (double) iExp;
			dTmp3 = (dTmp1 / 100.0f) * dTmp2;
			iExp += (int) dTmp3;
		}
		if (this->m_sType == 81) {
			for (i = 1; i < DEF_MAXCLIENTS; i++) {
				if (clientList[i] != nullptr) {
					clientList[i]->SendNotifyMsg(sAttackerH, DEF_NOTIFY_ABADDONKILLED, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, nullptr);
				}
			}
		}
		if (game_.m_bIsCrusadeMode == true) {
			if (iExp > 10) iExp = iExp / 3;
		}
		if (this->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] == 1) {
			iExp *= 2;
		}
		// New 09/05/2004
		//clientList[sAttackerH]->m_iExpStock += iExp;
		clientList[sAttackerH]->GetExp(iExp, true);
		iQuestIndex = clientList[sAttackerH]->m_iQuest;
		if (iQuestIndex != 0) {
			auto &questPtr = game_.m_pQuestConfigList[iQuestIndex];
			if (questPtr != nullptr) {
				switch (questPtr->m_iType) {
					case DEF_QUESTTYPE_MONSTERHUNT:
						if ((clientList[sAttackerH]->m_bQuestMatchFlag_Loc == true) &&
								  (questPtr->m_iTargetType == this->m_sType)) {
							clientList[sAttackerH]->m_iCurQuestCount++;
							cQuestRemain = (questPtr->m_iMaxCount - clientList[sAttackerH]->m_iCurQuestCount);
							clientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, nullptr);
							clientList[sAttackerH]->_bCheckIsQuestCompleted();
						}
						break;
				}
			}
		}
	}
	if (cAttackerType == DEF_OWNERTYPE_PLAYER) {
		switch (this->m_sType) {
			case 32:
				clientList[sAttackerH]->m_iRating -= 0;

				clientList[sAttackerH]->m_iRating -= 0;
				if (clientList[sAttackerH]->m_iRating > 10000) clientList[sAttackerH]->m_iRating = 10000;
				if (clientList[sAttackerH]->m_iRating < -10000) clientList[sAttackerH]->m_iRating = -10000;
				break;
			case 33:
				break;
		}
	}
	// Crusade
	iConstructionPoint = 0;
	switch (this->m_sType) {
		case 1: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 2: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 3: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 4: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 5: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 6: iConstructionPoint = 50;
			iWarContribution = 100;
			break;
		case 36: iConstructionPoint = 700;
			iWarContribution = 4000;
			break;
		case 37: iConstructionPoint = 700;
			iWarContribution = 4000;
			break;
		case 38: iConstructionPoint = 500;
			iWarContribution = 2000;
			break;
		case 39: iConstructionPoint = 500;
			iWarContribution = 2000;
			break;
		case 40: iConstructionPoint = 1500;
			iWarContribution = 5000;
			break;
		case 41: iConstructionPoint = 5000;
			iWarContribution = 10000;
			break;
		case 43: iConstructionPoint = 500;
			iWarContribution = 1000;
			break;
		case 44: iConstructionPoint = 1000;
			iWarContribution = 2000;
			break;
		case 45: iConstructionPoint = 1500;
			iWarContribution = 3000;
			break;
		case 46: iConstructionPoint = 1000;
			iWarContribution = 2000;
			break;
		case 47: iConstructionPoint = 1500;
			iWarContribution = 3000;
			break;
		case 64: this->map_->bRemoveCropsTotalSum();
			break;
	}
	// Crusade
	if (iConstructionPoint != 0) {
		switch (cAttackerType) {
			case DEF_OWNERTYPE_PLAYER:
				if (clientList[sAttackerH]->m_cSide != this->m_cSide) {
					clientList[sAttackerH]->m_iConstructionPoint += iConstructionPoint;
					if (clientList[sAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
						clientList[sAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
					clientList[sAttackerH]->m_iWarContribution += iWarContribution;
					if (clientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
						clientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
					//testcode
					wsprintf(G_cTxt, "Enemy NULLNpc Killed by player! Construction: +%d WarContribution: +%d", iConstructionPoint, iWarContribution);
					PutLogList(G_cTxt);
					clientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, clientList[sAttackerH]->m_iConstructionPoint, clientList[sAttackerH]->m_iWarContribution, 0, nullptr);
				} else {
					clientList[sAttackerH]->m_iWarContribution -= (iWarContribution * 2);
					if (clientList[sAttackerH]->m_iWarContribution < 0)
						clientList[sAttackerH]->m_iWarContribution = 0;
					//testcode
					wsprintf(G_cTxt, "Friendly Npc Killed by player! WarContribution: -%d", iWarContribution);
					PutLogList(G_cTxt);
					clientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, clientList[sAttackerH]->m_iConstructionPoint, clientList[sAttackerH]->m_iWarContribution, 0, nullptr);
				}
				break;
			case DEF_OWNERTYPE_NPC:
				if (npcList[sAttackerH]->m_iGuildGUID != 0) {
					if (npcList[sAttackerH]->m_cSide != this->m_cSide) {
						for (i = 1; i < DEF_MAXCLIENTS; i++)
							if ((clientList[i] != nullptr) && (clientList[i]->m_iGuildGUID == npcList[sAttackerH]->m_iGuildGUID) &&
									  (clientList[i]->m_iCrusadeDuty == 3)) {
								clientList[i]->m_iConstructionPoint += iConstructionPoint;
								if (clientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
									clientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;
								//testcode
								wsprintf(G_cTxt, "Enemy Npc Killed by Npc! Construct point +%d", iConstructionPoint);
								PutLogList(G_cTxt);
								clientList[i]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, clientList[i]->m_iConstructionPoint, clientList[i]->m_iWarContribution, 0, nullptr);
								goto NKH_GOTOPOINT1;
							}
					}
				}
				break;
		}
	}
	// new - heldenian
	if ((game_.m_bIsHeldenianMode == true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) && (clientList[sAttackerH]->m_cSide != this->m_cSide)) {
		switch (this->m_sType) {
			case 82: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 83: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 84: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 85: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 86: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 87: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 88: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
			case 89: iConstructionPoint = 0;
				iWarContribution = 1000;
				break;
		}
		if (iWarContribution > 0) clientList[sAttackerH]->m_iWarContribution += iWarContribution;
		if (clientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION) clientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
		clientList[sAttackerH]->SendNotifyMsg(0, DEF_NOTIFY_CONSTRUCTIONPOINT, clientList[sAttackerH]->m_iConstructionPoint, clientList[sAttackerH]->m_iWarContribution, 1, 0, 0, 0);
	}
NKH_GOTOPOINT1:
	;
	if (this->m_cSpecialAbility == 7) {
		this->m_iMana = 100;
		this->m_iMagicHitRatio = 100;
		this->NpcMagicHandler(this->m_sX, this->m_sY, 30);
	} else if (this->m_cSpecialAbility == 8) {
		this->m_iMana = 100;
		this->m_iMagicHitRatio = 100;
		this->NpcMagicHandler(this->m_sX, this->m_sY, 61);
	}
	if ((game_.m_bIsHeldenianMode == true) && (this->map_->m_bIsHeldenianMap == true) && (game_.m_cHeldenianModeType == 1)) {
		if ((this->m_sType == 87) || (this->m_sType == 89)) {
			if (this->m_cSide == 1) {
				game_.m_iHeldenianAresdenLeftTower--;
				wsprintf(G_cTxt, "Aresden Tower Broken, Left TOWER %d", game_.m_iHeldenianAresdenLeftTower);
			} else if (this->m_cSide == 2) {
				game_.m_iHeldenianElvineLeftTower--;
				wsprintf(G_cTxt, "Elvine Tower Broken, Left TOWER %d", game_.m_iHeldenianElvineLeftTower);
			}
			PutLogList(G_cTxt);
			game_.UpdateHeldenianStatus();
		}
		if ((game_.m_iHeldenianElvineLeftTower == 0) || (game_.m_iHeldenianAresdenLeftTower == 0)) {
			game_.GlobalEndHeldenianMode();
		}
	}
}

void CNpc::NpcBehavior_Move() {
	char cDir;
	short sX;
	short sY;
	short dX;
	short dY;
	short absX;
	short absY;
	short sTarget;
	short sDistance;
	char cTargetType;
	if (this->m_bIsKilled == true) return;
	if ((this->m_bIsSummoned == true) &&
			  (this->m_iSummonControlMode == 1)) return;
	if (this->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) return;

	switch (this->m_cActionLimit) {
		case 2:
		case 3:
		case 5:
			this->m_cBehavior = DEF_BEHAVIOR_STOP;
			this->m_sBehaviorTurnCount = 0;
			return;
	}

	int iStX;

	int iStY;
	if (this->map_) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iMonsterActivity++;
	}

	this->m_sBehaviorTurnCount++;
	if (this->m_sBehaviorTurnCount > 5) {

		this->m_sBehaviorTurnCount = 0;

		absX = abs(this->m_vX - this->m_sX);
		absY = abs(this->m_vY - this->m_sY);
		if ((absX <= 2) && (absY <= 2)) {

			this->CalcNextWayPointDestination();
		}
		this->m_vX = this->m_sX;
		this->m_vY = this->m_sY;
	}

	this->TargetSearch(&sTarget, &cTargetType);
	if (sTarget != 0) {

		if (this->m_dwActionTime < 1000) {

			if (iDice(1, 3) == 3) {
				this->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				this->m_sBehaviorTurnCount = 0;
				this->m_iTargetIndex = sTarget;
				this->m_cTargetType = cTargetType;

				return;
			}
		} else {

			this->m_cBehavior = DEF_BEHAVIOR_ATTACK;
			this->m_sBehaviorTurnCount = 0;
			this->m_iTargetIndex = sTarget;
			this->m_cTargetType = cTargetType;

			return;
		}
	}

	if ((this->m_bIsMaster == true) && (iDice(1, 3) == 2)) return;
	if (this->m_cMoveType == DEF_MOVETYPE_FOLLOW) {
		sX = this->m_sX;
		sY = this->m_sY;
		auto &clientList = game_.m_pClientList;
		auto &npcList = game_.m_pNpcList;
		switch (this->m_cFollowOwnerType) {
			case DEF_OWNERTYPE_PLAYER:
				if (clientList[this->m_iFollowOwnerIndex] == nullptr) {
					this->m_cMoveType = DEF_MOVETYPE_RANDOM;
					return;
				}
				dX = clientList[this->m_iFollowOwnerIndex]->m_sX;
				dY = clientList[this->m_iFollowOwnerIndex]->m_sY;
				break;
			case DEF_OWNERTYPE_NPC:
				if (npcList[this->m_iFollowOwnerIndex] == nullptr) {
					this->m_cMoveType = DEF_MOVETYPE_RANDOM;
					this->m_iFollowOwnerIndex = 0;
					//bSerchMaster(iNpcH);
					return;
				}
				dX = npcList[this->m_iFollowOwnerIndex]->m_sX;
				dY = npcList[this->m_iFollowOwnerIndex]->m_sY;
				break;
		}
		if (abs(sX - dX) >= abs(sY - dY))
			sDistance = abs(sX - dX);
		else sDistance = abs(sY - dY);
		if (sDistance >= 3) {

			cDir = this->map_->cGetNextMoveDir(sX, sY, dX, dY, this->m_cTurn, &this->m_tmp_iError);
			if (cDir == 0) {

			} else {
				dX = this->m_sX + _tmp_cTmpDirX[cDir];
				dY = this->m_sY + _tmp_cTmpDirY[cDir];

				this->map_->ClearOwner(3, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);

				this->map_->SetOwner(id_, DEF_OWNERTYPE_NPC, dX, dY);
				this->m_sX = dX;
				this->m_sY = dY;
				this->m_cDir = cDir;
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
			}
		}
	} else {

		cDir = this->map_->cGetNextMoveDir(this->m_sX, this->m_sY,
				  this->m_dX, this->m_dY,
				  this->m_cTurn, &this->m_tmp_iError);
		if (cDir == 0) {

			if (iDice(1, 10) == 3) this->CalcNextWayPointDestination();
		} else {
			dX = this->m_sX + _tmp_cTmpDirX[cDir];
			dY = this->m_sY + _tmp_cTmpDirY[cDir];

			this->map_->ClearOwner(4, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);

			this->map_->SetOwner(id_, DEF_OWNERTYPE_NPC, dX, dY);
			this->m_sX = dX;
			this->m_sY = dY;
			this->m_cDir = cDir;
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
		}
	}
}

void CNpc::TargetSearch(short * pTarget, char * pTargetType) {
	register int ix, iy, iPKCount;
	register short sX, sY, rX, rY, dX, dY;
	short sOwner;
	short sTargetOwner;
	short sDistance;
	short sTempDistance;
	char cOwnerType;
	char cTargetType;
	char cTargetSide;
	int iInv;
	sTargetOwner = 0;
	cTargetType = 0;
	sDistance = 100;
	sX = this->m_sX;
	sY = this->m_sY;
	rX = this->m_sX - this->m_cTargetSearchRange;
	rY = this->m_sY - this->m_cTargetSearchRange;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	for (ix = rX; ix < rX + this->m_cTargetSearchRange * 2 + 1; ix++)
		for (iy = rY; iy < rY + this->m_cTargetSearchRange * 2 + 1; iy++) {
			this->map_->GetOwner(&sOwner, &cOwnerType, ix, iy);
			if (sOwner != 0) {
				if ((sOwner == id_) && (cOwnerType == DEF_OWNERTYPE_NPC)) break;
				iPKCount = 0;
				switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (clientList[sOwner] == nullptr) {
							this->map_->ClearOwner(5, sOwner, DEF_OWNERTYPE_PLAYER, ix, iy);
						} else {
							if (clientList[sOwner]->m_iAdminUserLevel > 0) goto SKIP_SEARCH;
							if (clientList[sOwner]->m_cSide == 0) goto SKIP_SEARCH;
							dX = clientList[sOwner]->m_sX;
							dY = clientList[sOwner]->m_sY;
							cTargetSide = clientList[sOwner]->m_cSide;
							iPKCount = clientList[sOwner]->m_iPKCount;
							iInv = clientList[sOwner]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ];
						}
						break;
					case DEF_OWNERTYPE_NPC:
						if (npcList[sOwner] == nullptr) {
							this->map_->ClearOwner(6, sOwner, DEF_OWNERTYPE_NPC, ix, iy);
						} else {
							dX = npcList[sOwner]->m_sX;
							dY = npcList[sOwner]->m_sY;
							cTargetSide = npcList[sOwner]->m_cSide;
							iPKCount = 0;
							iInv = npcList[sOwner]->m_cMagicEffectStatus[ DEF_MAGICTYPE_INVISIBILITY ];
							if (this->m_sType == 21) {
								if (npcList[sOwner]->map_->_iCalcPlayerNum(dX, dY, 2) != 0) {
									sOwner = 0;
									cOwnerType = 0;
								}
							}
						}
						break;
				}
				if (this->m_cSide < 10) {
					if (cTargetSide == 0) {
						if (iPKCount == 0) goto SKIP_SEARCH;
					} else {
						if ((iPKCount == 0) && (cTargetSide == this->m_cSide)) goto SKIP_SEARCH;
						if (this->m_cSide == 0) goto SKIP_SEARCH;
					}
				} else {
					if ((cOwnerType == DEF_OWNERTYPE_NPC) && (cTargetSide == 0)) goto SKIP_SEARCH;
					if (cTargetSide == this->m_cSide) goto SKIP_SEARCH;
				}
				if ((iInv != 0) && (this->m_cSpecialAbility != 1)) goto SKIP_SEARCH;
				if (abs(sX - dX) >= abs(sY - dY))
					sTempDistance = abs(sX - dX);
				else sTempDistance = abs(sY - dY);
				if (sTempDistance < sDistance) {
					sDistance = sTempDistance;
					sTargetOwner = sOwner;
					cTargetType = cOwnerType;
				}
SKIP_SEARCH:
				;
			}
		}
	*pTarget = sTargetOwner;
	*pTargetType = cTargetType;
	return;
}

void CNpc::NpcBehavior_Attack() {
	int iMagicType;
	short sX;
	short sY;
	short dX;
	short dY;
	char cDir;
	uint32_t dwTime = timeGetTime();
	if (this->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) return;
	if (this->m_bIsKilled == true) return;
	switch (this->m_cActionLimit) {
		case 1:
		case 2:
		case 3:
		case 4:
			return;
		case 5:
			if (this->m_iBuildCount > 0) return;
	}

	int iStX;

	int iStY;
	if (this->map_ != nullptr) {
		iStX = this->m_sX / 20;
		iStY = this->m_sY / 20;
		this->map_->m_stTempSectorInfo[iStX][iStY].iMonsterActivity++;
	}

	if (this->m_sBehaviorTurnCount == 0)
		this->m_iAttackCount = 0;

	this->m_sBehaviorTurnCount++;
	if (this->m_sBehaviorTurnCount > 20) {

		this->m_sBehaviorTurnCount = 0;
		if ((this->m_bIsPermAttackMode == false))
			this->m_cBehavior = DEF_BEHAVIOR_MOVE;

		return;
	}

	sX = this->m_sX;
	sY = this->m_sY;
	auto &clientList = game_.m_pClientList;
	auto &npcList = game_.m_pNpcList;
	switch (this->m_cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (clientList[this->m_iTargetIndex] == nullptr) {

				this->m_sBehaviorTurnCount = 0;
				this->m_cBehavior = DEF_BEHAVIOR_MOVE;
				return;
			}
			dX = clientList[this->m_iTargetIndex]->m_sX;
			dY = clientList[this->m_iTargetIndex]->m_sY;
			break;
		case DEF_OWNERTYPE_NPC:
			if (npcList[this->m_iTargetIndex] == nullptr) {

				this->m_sBehaviorTurnCount = 0;
				this->m_cBehavior = DEF_BEHAVIOR_MOVE;
				return;
			}
			dX = npcList[this->m_iTargetIndex]->m_sX;
			dY = npcList[this->m_iTargetIndex]->m_sY;
			break;
	}

	if ((this->iGetDangerValue(dX, dY) > this->m_cBravery) &&
			  (this->m_bIsPermAttackMode == false) &&
			  (this->m_cActionLimit != 5)) {
		this->m_sBehaviorTurnCount = 0;
		this->m_cBehavior = DEF_BEHAVIOR_FLEE;
		return;
	}

	if ((this->m_iHP <= 2) && (iDice(1, this->m_cBravery) <= 3) &&
			  (this->m_bIsPermAttackMode == false) &&
			  (this->m_cActionLimit != 5)) {
		this->m_sBehaviorTurnCount = 0;
		this->m_cBehavior = DEF_BEHAVIOR_FLEE;
		return;
	}
	auto &magicCfgs = game_.m_pMagicConfigList;
	if ((abs(sX - dX) <= 1) && (abs(sY - dY) <= 1)) {

		cDir = game_.m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
		if (cDir == 0) return;

		this->m_cDir = cDir;
		if (this->m_cActionLimit == 5) {

			switch (this->m_sType) {
				case 89:
					this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
					this->m_iMagicHitRatio = 1000;
					this->NpcMagicHandler(dX, dY, 61);
					break;
				case 87:
					this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2);
					game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 2);
					break;
				case 36:
					this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, this->m_sX + _tmp_cTmpDirX[cDir], this->m_sY + _tmp_cTmpDirY[cDir], 2); // Ȱ
					game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 2, false, false, false);
					break;
				case 37: // Cannon Guard Tower:
					this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
					this->m_iMagicHitRatio = 1000;
					this->NpcMagicHandler(dX, dY, 61);
					break;
			}
		} else {

			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, this->m_sX + _tmp_cTmpDirX[cDir], this->m_sY + _tmp_cTmpDirY[cDir], 1);
			game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 1, false, false);
		}

		this->m_iAttackCount++;
		if ((this->m_bIsPermAttackMode == false) && (this->m_cActionLimit == 0)) {
			switch (this->m_iAttackStrategy) {
				case DEF_ATTACKAI_EXCHANGEATTACK:

					this->m_sBehaviorTurnCount = 0;
					this->m_cBehavior = DEF_BEHAVIOR_FLEE;
					break;
				case DEF_ATTACKAI_TWOBYONEATTACK:

					if (this->m_iAttackCount >= 2) {
						this->m_sBehaviorTurnCount = 0;
						this->m_cBehavior = DEF_BEHAVIOR_FLEE;
					}
					break;
			}
		}
	} else {
		cDir = game_.m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
		if (cDir == 0) return;
		this->m_cDir = cDir;
		if ((this->m_cMagicLevel > 0) && (iDice(1, 2) == 1) &&
				  (abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
			iMagicType = -1;
			switch (this->m_cMagicLevel) {
				case 1:
					if (magicCfgs[0]->m_sValue1 <= this->m_iMana)
						iMagicType = 0;
					break;
				case 2:
					if (magicCfgs[10]->m_sValue1 <= this->m_iMana)
						iMagicType = 10;
					else if (magicCfgs[0]->m_sValue1 <= this->m_iMana)
						iMagicType = 0;
					break;
				case 3: // Orc-Mage
					if (magicCfgs[20]->m_sValue1 <= this->m_iMana)
						iMagicType = 20;
					else if (magicCfgs[10]->m_sValue1 <= this->m_iMana)
						iMagicType = 10;
					break;
				case 4:
					if (magicCfgs[30]->m_sValue1 <= this->m_iMana)
						iMagicType = 30;
					else if (magicCfgs[37]->m_sValue1 <= this->m_iMana)
						iMagicType = 37;
					else if (magicCfgs[20]->m_sValue1 <= this->m_iMana)
						iMagicType = 20;
					else if (magicCfgs[10]->m_sValue1 <= this->m_iMana)
						iMagicType = 10;
					break;
				case 5: // Rudolph, Cannibal-Plant, Cyclops
					if (magicCfgs[43]->m_sValue1 <= this->m_iMana)
						iMagicType = 43;
					else if (magicCfgs[30]->m_sValue1 <= this->m_iMana)
						iMagicType = 30;
					else if (magicCfgs[37]->m_sValue1 <= this->m_iMana)
						iMagicType = 37;
					else if (magicCfgs[20]->m_sValue1 <= this->m_iMana)
						iMagicType = 20;
					else if (magicCfgs[10]->m_sValue1 <= this->m_iMana)
						iMagicType = 10;
					break;
				case 6: // Tentocle, Liche
					if (magicCfgs[51]->m_sValue1 <= this->m_iMana)
						iMagicType = 51;
					else if (magicCfgs[43]->m_sValue1 <= this->m_iMana)
						iMagicType = 43;
					else if (magicCfgs[30]->m_sValue1 <= this->m_iMana)
						iMagicType = 30;
					else if (magicCfgs[37]->m_sValue1 <= this->m_iMana)
						iMagicType = 37;
					else if (magicCfgs[20]->m_sValue1 <= this->m_iMana)
						iMagicType = 20;
					else if (magicCfgs[10]->m_sValue1 <= this->m_iMana)
						iMagicType = 10;
					break;
				case 7: // Barlog, Fire-Wyvern, MasterMage-Orc , LightWarBeatle, GHK, GHKABS, TK, BG
					// Sor, Gagoyle, Demon
					if ((magicCfgs[70]->m_sValue1 <= this->m_iMana) && (iDice(1, 5) == 3))
						iMagicType = 70;
					else if (magicCfgs[61]->m_sValue1 <= this->m_iMana)
						iMagicType = 61;
					else if (magicCfgs[60]->m_sValue1 <= this->m_iMana)
						iMagicType = 60;
					else if (magicCfgs[51]->m_sValue1 <= this->m_iMana)
						iMagicType = 51;
					else if (magicCfgs[43]->m_sValue1 <= this->m_iMana)
						iMagicType = 43;
					break;
				case 8: // Unicorn, Centaurus
					if ((magicCfgs[35]->m_sValue1 <= this->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 35;
					else if (magicCfgs[60]->m_sValue1 <= this->m_iMana)
						iMagicType = 60;
					else if (magicCfgs[51]->m_sValue1 <= this->m_iMana)
						iMagicType = 51;
					else if (magicCfgs[43]->m_sValue1 <= this->m_iMana)
						iMagicType = 43;
					break;
				case 9: // Tigerworm
					if ((magicCfgs[74]->m_sValue1 <= this->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 74; // Lightning-Strike
					break;
				case 10: // Frost, Nizie
					break;
				case 11: // Ice-Golem
					break;
				case 12: // Wyvern
					if ((magicCfgs[91]->m_sValue1 <= this->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 91; // Blizzard
					else if (magicCfgs[63]->m_sValue1 <= this->m_iMana)
						iMagicType = 63; // Mass-Chill-Wind
					break;
				case 13: // Abaddon
					if ((magicCfgs[96]->m_sValue1 <= this->m_iMana) && (iDice(1, 3) == 2))
						iMagicType = 92; // Earth Shock Wave
					else if (magicCfgs[81]->m_sValue1 <= this->m_iMana)
						iMagicType = 96; // Metoer Strike
					break;
			}
			if (iMagicType != -1) {
				if (this->m_iAILevel >= 2) {
					switch (this->m_cTargetType) {
						case DEF_OWNERTYPE_PLAYER:
							if (clientList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] == 2) {

								if ((abs(sX - dX) > this->m_iAttackRange) || (abs(sY - dY) > this->m_iAttackRange)) {
									this->m_sBehaviorTurnCount = 0;
									this->m_cBehavior = DEF_BEHAVIOR_MOVE;
									return;
								} else goto NBA_CHASE;
							}

							if ((iMagicType == 35) && (clientList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0)) goto NBA_CHASE;
							break;
						case DEF_OWNERTYPE_NPC:
							if (npcList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_PROTECT ] == 2) {

								if ((abs(sX - dX) > this->m_iAttackRange) || (abs(sY - dY) > this->m_iAttackRange)) {
									this->m_sBehaviorTurnCount = 0;
									this->m_cBehavior = DEF_BEHAVIOR_MOVE;
									return;
								} else goto NBA_CHASE;
							}

							if ((iMagicType == 35) && (npcList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0)) goto NBA_CHASE;
							break;
					}
				}
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, this->m_sX + _tmp_cTmpDirX[cDir], this->m_sY + _tmp_cTmpDirY[cDir], 1);
				this->NpcMagicHandler(dX, dY, iMagicType);
				this->m_dwTime = dwTime + 2000;
				return;
			}
		}

		if ((this->m_cMagicLevel < 0) && (iDice(1, 2) == 1) &&
				  (abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
			iMagicType = -1;
			if (magicCfgs[43]->m_sValue1 <= this->m_iMana)
				iMagicType = 43;
			else if (magicCfgs[37]->m_sValue1 <= this->m_iMana)
				iMagicType = 37;
			else if (magicCfgs[0]->m_sValue1 <= this->m_iMana)
				iMagicType = 0;
			if (iMagicType != -1) {
				this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, this->m_sX + _tmp_cTmpDirX[cDir], this->m_sY + _tmp_cTmpDirY[cDir], 1);
				this->NpcMagicHandler(dX, dY, iMagicType);
				this->m_dwTime = dwTime + 2000;
				return;
			}
		}

		if ((this->m_iAttackRange > 1) &&
				  (abs(sX - dX) <= this->m_iAttackRange) && (abs(sY - dY) <= this->m_iAttackRange)) {
			cDir = game_.m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
			if (cDir == 0) return;

			this->m_cDir = cDir;
			if (this->m_cActionLimit == 5) {
				switch (this->m_sType) {
					case 36: // Crossbow Guard Tower
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2);
						game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 2);
						break;
					case 37:
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
						this->m_iMagicHitRatio = 1000;
						this->NpcMagicHandler(dX, dY, 61);
						break;
				}
			} else {
				switch (this->m_sType) {
					case 51:
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
						this->m_iMagicHitRatio = 1000;
						this->NpcMagicHandler(dX, dY, 61);
						break;
					case 54:
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2);
						game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 2);
						break;
					case 63: // Frost
					case 79: // Nizie
						switch (this->m_cTargetType) {
							case DEF_OWNERTYPE_PLAYER:
								if (clientList[this->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((magicCfgs[57]->m_sValue1 <= this->m_iMana) && (iDice(1, 3) == 2))
									this->NpcMagicHandler(dX, dY, 57);
								if ((clientList[this->m_iTargetIndex]->m_iHP > 0) &&
										  (this->bCheckResistingIceSuccess() == false)) {
									if (clientList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										clientList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										game_.SetIceFlag(this->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, true);

										game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  this->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);

										clientList[this->m_iTargetIndex]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (npcList[this->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((magicCfgs[57]->m_sValue1 <= this->m_iMana) && (iDice(1, 3) == 2))
									this->NpcMagicHandler(dX, dY, 57);
								if ((npcList[this->m_iTargetIndex]->m_iHP > 0) &&
										  (this->bCheckResistingIceSuccess() == false)) {
									if (npcList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										npcList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										game_.SetIceFlag(this->m_iTargetIndex, DEF_OWNERTYPE_NPC, true);

										game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  this->m_iTargetIndex, DEF_OWNERTYPE_NPC, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
					case 53:
						switch (this->m_cTargetType) {
							case DEF_OWNERTYPE_PLAYER:
								if (clientList[this->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((clientList[this->m_iTargetIndex]->m_iHP > 0) &&
										  (this->bCheckResistingIceSuccess() == false)) {
									if (clientList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										clientList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										game_.SetIceFlag(this->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, true);

										game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  this->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);

										clientList[this->m_iTargetIndex]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
									}
								}
								break;
							case DEF_OWNERTYPE_NPC:
								if (npcList[this->m_iTargetIndex] == nullptr) goto NBA_BREAK1;
								if ((npcList[this->m_iTargetIndex]->m_iHP > 0) &&
										  (this->bCheckResistingIceSuccess() == false)) {
									if (npcList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0) {
										npcList[this->m_iTargetIndex]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
										game_.SetIceFlag(this->m_iTargetIndex, DEF_OWNERTYPE_NPC, true);

										game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
												  this->m_iTargetIndex, DEF_OWNERTYPE_NPC, 0, 0, 0, 1, 0, 0);
									}
								}
								break;
						}
NBA_BREAK1:
						;
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 20);
						game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 20);
						break;
					default:
						this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 20);
						game_.iCalculateAttackEffect(this->m_iTargetIndex, this->m_cTargetType, id_, DEF_OWNERTYPE_NPC, dX, dY, 20);
						break;
				}
			}

			this->m_iAttackCount++;
			if ((this->m_bIsPermAttackMode == false) && (this->m_cActionLimit == 0)) {
				switch (this->m_iAttackStrategy) {
					case DEF_ATTACKAI_EXCHANGEATTACK:

						this->m_sBehaviorTurnCount = 0;
						this->m_cBehavior = DEF_BEHAVIOR_FLEE;
						break;
					case DEF_ATTACKAI_TWOBYONEATTACK:

						if (this->m_iAttackCount >= 2) {
							this->m_sBehaviorTurnCount = 0;
							this->m_cBehavior = DEF_BEHAVIOR_FLEE;
						}
						break;
				}
			}
			return;
		}

NBA_CHASE:
		;
		if (this->m_cActionLimit != 0) return;

		this->m_iAttackCount = 0;
		{

			cDir = this->map_->cGetNextMoveDir(sX, sY, dX, dY, this->m_cTurn, &this->m_tmp_iError);
			if (cDir == 0) {
				return;
			}
			dX = this->m_sX + _tmp_cTmpDirX[cDir];
			dY = this->m_sY + _tmp_cTmpDirY[cDir];

			this->map_->ClearOwner(9, id_, DEF_OWNERTYPE_NPC, this->m_sX, this->m_sY);

			this->map_->SetOwner(id_, DEF_OWNERTYPE_NPC, dX, dY);
			this->m_sX = dX;
			this->m_sY = dY;
			this->m_cDir = cDir;
			this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
		}
	}
}

UnitPtr CNpc::_getPtr() {
	return shared_from_this();
}

CMap &CNpc::_getMap() {
	return *map_;
}

int CNpc::_getEffectiveIceResist() {
	return (this->m_cResistMagic) - (this->m_cResistMagic / 3);
}

void CNpc::_sendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3) {
	int * ip;
	char * cp_a, * cp_s, * cp_sv, cData_All[200], cData_Srt[200], cData_Srt_Av[200];
	uint32_t * dwp;
	uint16_t * wp;
	int * ipStatus, iDumm;
	short * sp, sRange, sX, sY;
	char cKey;
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

	wp = (uint16_t *) cp_a;
	*wp = id_ + 10000;
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
	memcpy(cp_a, this->m_cName, 5);
	cp_a += 5;
	sp = (short *) cp_a;
	*sp = this->m_sAppr2;
	cp_a += 2;
	ip = (int *) cp_a;
	ipStatus = ip;
	*ip = this->m_iStatus;
	cp_a += 4;
	if (wMsgType == DEF_OBJECTNULLACTION) {
		if (this->m_bIsKilled == true)
			*cp_a = 1;
		else *cp_a = 0;
	} else *cp_a = 0;
	cp_a++;
	wp = (uint16_t *) cp_s;
	*wp = id_ + 40000;
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
	*wp = id_ + 40000;
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
		if ((clientIter.map_ == this->map_) &&
				  (clientIter.m_sX >= this->m_sX - 10 - sRange) &&
				  (clientIter.m_sX <= this->m_sX + 10 + sRange) &&
				  (clientIter.m_sY >= this->m_sY - 8 - sRange) &&
				  (clientIter.m_sY <= this->m_sY + 8 + sRange)) {
			iTemp = *ipStatus;
			iTemp = 0x0FFFFFFF & iTemp;
			iTemp2 = game_.iGetNpcRelationship(id_, clientIter.id_);
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
						clientIter.m_pXSock->iSendMsg(cData_All, 27, cKey);
						break;
					case DEF_OBJECTDYING:
						clientIter.m_pXSock->iSendMsg(cData_Srt, 15, cKey);
						break;
					case DEF_OBJECTDAMAGE:
					case DEF_OBJECTDAMAGEMOVE:
						clientIter.m_pXSock->iSendMsg(cData_Srt, 11, cKey);
						break;
					case DEF_OBJECTATTACK:
					case DEF_OBJECTATTACKMOVE:
						clientIter.m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
						break;
					default:
						clientIter.m_pXSock->iSendMsg(cData_Srt, 9, cKey);
						break;
				} //Switch
			} else {
				switch (wMsgType) {
					case DEF_MSGTYPE_CONFIRM:
					case DEF_MSGTYPE_REJECT:
					case DEF_OBJECTNULLACTION:
						clientIter.m_pXSock->iSendMsg(cData_All, 27, cKey);
						break;
					case DEF_OBJECTDYING:
						clientIter.m_pXSock->iSendMsg(cData_Srt, 15, cKey);
						break;
					case DEF_OBJECTDAMAGE:
					case DEF_OBJECTDAMAGEMOVE:
						clientIter.m_pXSock->iSendMsg(cData_Srt, 11, cKey);
						break;
					case DEF_OBJECTATTACK:
					case DEF_OBJECTATTACKMOVE:
						clientIter.m_pXSock->iSendMsg(cData_Srt_Av, 13, cKey);
						break;
					default:
						clientIter.m_pXSock->iSendMsg(cData_All, 27, cKey);
						break;
				} //Switch
			}
		}
	}
}

void CNpc::SetIceFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x00000040;
	else this->m_iStatus = this->m_iStatus & 0xFFFFFFBF;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CNpc::SetProtectionFromArrowFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x08000000;
	else this->m_iStatus = this->m_iStatus & 0xF7FFFFFF;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CNpc::SetMagicProtectionFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x04000000;
	else this->m_iStatus = this->m_iStatus & 0xFBFFFFFF;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

}

void CNpc::SetDefenseShieldFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x02000000;
	else this->m_iStatus = this->m_iStatus & 0xFDFFFFFF;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CNpc::SetInvisibilityFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x00000010;
	else this->m_iStatus = this->m_iStatus & 0xFFFFFFEF;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CNpc::RemoveFromTarget(int iCode) {
	for (int i = 0; i < DEF_MAXNPCS; i++) {
		if (game_.m_pNpcList[i] != nullptr) {
			if ((game_.m_pNpcList[i]->m_iTargetIndex == id_) &&
					  (game_.m_pNpcList[i]->m_cTargetType == DEF_OWNERTYPE_NPC)) {
				switch (iCode) {
					case DEF_MAGICTYPE_INVISIBILITY:
						if (game_.m_pNpcList[i]->m_cSpecialAbility == 1) {
						} else {
							game_.m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
							game_.m_pNpcList[i]->m_iTargetIndex = 0;
							game_.m_pNpcList[i]->m_cTargetType = 0;
						}
						break;
					default:
						game_.m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
						game_.m_pNpcList[i]->m_iTargetIndex = 0;
						game_.m_pNpcList[i]->m_cTargetType = 0;
						break;
				}
			}
		}
	}
}

bool CNpc::bCheckResistingPoisonSuccess() {
	int iResist;
	int iResult;
	iResist = 0;
	iResult = iDice(1, 100);
	if (iResult >= iResist)
		return false;
	return true;
}

void CNpc::SetPoisonFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x00000080;
	else this->m_iStatus = this->m_iStatus & 0xFFFFFF7F;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CNpc::SetBerserkFlag(bool bStatus) {
	if (bStatus)
		this->m_iStatus = this->m_iStatus | 0x00000020;
	else this->m_iStatus = this->m_iStatus & 0xFFFFFFDF;
	this->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

}

bool CNpc::_bItemLog(int iAction, char * cName, class CItem * pItem) {
	if (pItem == nullptr) return false;
	if (game_._bCheckGoodItem(pItem) == false) return false;
	if (iAction != DEF_ITEMLOG_NEWGENDROP) {
		if (this->markedForDeletion_) return false;
	}
	char cTxt[200];
	std::memset(cTxt, 0, sizeof (cTxt));
	switch (iAction) {
		case DEF_ITEMLOG_NEWGENDROP:
			if (pItem == nullptr) return false;
			wsprintf(cTxt, "NPC(%s)\tDrop\t%s(%d %d %d %d)", cName, pItem->m_cName, pItem->m_dwCount,
					  pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
			break;
		default:
			return false;
	}
	game_.bSendMsgToLS(MSGID_GAMEITEMLOG, cTxt);
	return true;
}