#include "DynamicObject.h"
#include "Map.h"
#include "Client.h"
#include "Game.h"
#include "DynamicObjectID.h"

CDynamicObject::CDynamicObject(short sOwner, char cOwnerType, DynamicObjectType sType, MapPtr map, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1) {
	m_sOwner = sOwner;
	m_cOwnerType = cOwnerType;

	m_sType = sType;
	map_ = map;
	m_sX = sX;
	m_sY = sY;

	m_dwRegisterTime = dwRegisterTime;
	m_dwLastTime = dwLastTime;

	m_iCount = 0;
	m_iV1 = iV1;
}

DynamicObjects::DynamicObjects(CGame &game, Maps &maps, Clients &clients) : game_(game), m_pMapList(maps), m_pClientList(clients) {
}

void DynamicObjects::DynamicObjectEffectProcessor() {
	register int i;
	int ix;
	int iy;
	int iIndex;
	short sOwnerH;
	short iDamage;
	DynamicObjectType sType;
	char cOwnerType;
	uint32_t dwTime = timeGetTime(), dwRegisterTime;

	//
	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] != nullptr) {
			switch (m_pDynamicObjectList[i]->m_sType) {

				case DynamicObjectType::PCLOUD_BEGIN:

					for (ix = m_pDynamicObjectList[i]->m_sX - 1; ix <= m_pDynamicObjectList[i]->m_sX + 1; ix++)
						for (iy = m_pDynamicObjectList[i]->m_sY - 1; iy <= m_pDynamicObjectList[i]->m_sY + 1; iy++) {

							m_pDynamicObjectList[i]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (sOwnerH != 0) {

								switch (cOwnerType) {
									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) break;
										if (m_pClientList[sOwnerH]->m_bIsKilled == true) break;

										//if ((m_pClientList[sOwnerH]->m_bIsNeutral == true) && (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) == 0) break;


										if (m_pDynamicObjectList[i]->m_iV1 < 20)
											iDamage = iDice(1, 6);
										else iDamage = iDice(1, 8);

										// New 17/05/2004 Changed
										if (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)
											m_pClientList[sOwnerH]->m_iHP -= iDamage;

										if (m_pClientList[sOwnerH]->m_iHP <= 0) {

											m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
										} else {
											if (iDamage > 0) {

												game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);

												if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {

													// 1: Hold-Person
													// 2: Paralize
													game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT], 0, nullptr);

													m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
													game_.delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, MagicType::HOLDOBJECT);
												}
											}


											if ((game_.bCheckResistingMagicSuccess(1, sOwnerH, DEF_OWNERTYPE_PLAYER, 100) == false) &&
													  (m_pClientList[sOwnerH]->m_bIsPoisoned == false)) {

												m_pClientList[sOwnerH]->m_bIsPoisoned = true;
												m_pClientList[sOwnerH]->m_iPoisonLevel = m_pDynamicObjectList[i]->m_iV1;
												m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;

												game_.SetPoisonFlag(sOwnerH, cOwnerType, true); // poison aura appears from dynamic objects
												game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, MagicType::POISON, m_pClientList[sOwnerH]->m_iPoisonLevel, 0, nullptr);
											}
										}
										break;

									case DEF_OWNERTYPE_NPC:

										if (game_.m_pNpcList[sOwnerH] == nullptr) break;


										if (m_pDynamicObjectList[i]->m_iV1 < 20)
											iDamage = iDice(1, 6);
										else iDamage = iDice(1, 8);


										switch (game_.m_pNpcList[sOwnerH]->m_sType) {
											case 40: // ESG
											case 41: // GMG
											case 67: // McGaffin
											case 68: // Perry
											case 69: // Devlin
												iDamage = 0;
												break;
										}


										switch (game_.m_pNpcList[sOwnerH]->m_cActionLimit) {
											case 0:
											case 3:
											case 5:
												game_.m_pNpcList[sOwnerH]->m_iHP -= iDamage;
												break;
										}
										//if (m_pNpcList[sOwnerH]->m_cActionLimit == 0)
										//	m_pNpcList[sOwnerH]->m_iHP -= iDamage;

										if (game_.m_pNpcList[sOwnerH]->m_iHP <= 0) {

											game_.m_pNpcList[sOwnerH]->NpcKilledHandler(sOwnerH, cOwnerType, 0);
										} else {

											if (iDice(1, 3) == 2)
												game_.m_pNpcList[sOwnerH]->m_dwTime = dwTime;

											if (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
											}


											game_.m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
										}
										break;
								}
							}
						}
					break;

				case DynamicObjectType::ICESTORM:
					for (ix = m_pDynamicObjectList[i]->m_sX - 2; ix <= m_pDynamicObjectList[i]->m_sX + 2; ix++)
						for (iy = m_pDynamicObjectList[i]->m_sY - 2; iy <= m_pDynamicObjectList[i]->m_sY + 2; iy++) {

							m_pDynamicObjectList[i]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (sOwnerH != 0) {
								switch (cOwnerType) {
									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) break;
										if (m_pClientList[sOwnerH]->m_bIsKilled == true) break;

										iDamage = iDice(3, 3) + 5;

										if (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)
											m_pClientList[sOwnerH]->m_iHP -= iDamage;

										if (m_pClientList[sOwnerH]->m_iHP <= 0) {

											m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
										} else {
											if (iDamage > 0) {

												game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);

												if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] == 1) {

													game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT], 0, nullptr);

													m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
													game_.delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, MagicType::HOLDOBJECT);
												}
											}

											if ((m_pClientList[sOwnerH]->bCheckResistingIceSuccess() == false) &&
													  (m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::ICE] == 0)) {

												m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::ICE] = 1;
												game_.SetIceFlag(sOwnerH, cOwnerType, true);

												game_.delayEvents_.add(DelayEventType::MAGICRELEASE, MagicType::ICE, dwTime + (20 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

												game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTON, MagicType::ICE, 1, 0, nullptr);
											}
										}
										break;

									case DEF_OWNERTYPE_NPC:
										if (game_.m_pNpcList[sOwnerH] == nullptr) break;

										iDamage = iDice(3, 3) + 5;

										switch (game_.m_pNpcList[sOwnerH]->m_sType) {
											case 40: // ESG
											case 41: // GMG
											case 67: // McGaffin
											case 68: // Perry
											case 69: // Devlin
												iDamage = 0;
												break;
										}

										switch (game_.m_pNpcList[sOwnerH]->m_cActionLimit) {
											case 0:
											case 3:
											case 5:
												game_.m_pNpcList[sOwnerH]->m_iHP -= iDamage;
												break;
										}

										if (game_.m_pNpcList[sOwnerH]->m_iHP <= 0) {

											game_.m_pNpcList[sOwnerH]->NpcKilledHandler(sOwnerH, cOwnerType, 0);
										} else {

											if (iDice(1, 3) == 2)
												game_.m_pNpcList[sOwnerH]->m_dwTime = dwTime;

											if (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
											}


											game_.m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);


											if ((game_.m_pNpcList[sOwnerH]->bCheckResistingIceSuccess() == false) &&
													  (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::ICE] == 0)) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::ICE] = 1;
												game_.SetIceFlag(sOwnerH, cOwnerType, true);

												game_.delayEvents_.add(DelayEventType::MAGICRELEASE, MagicType::ICE, dwTime + (20 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											}
										}
										break;
								}
							}


							m_pDynamicObjectList[i]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {

								iDamage = iDice(3, 2);
								m_pClientList[sOwnerH]->m_iHP -= iDamage;

								if (m_pClientList[sOwnerH]->m_iHP <= 0) {

									m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
								} else {
									if (iDamage > 0) {

										game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
									}
								}
							}


							m_pDynamicObjectList[i]->map_->bGetDynamicObject(ix, iy, &sType, &dwRegisterTime, &iIndex);
							if (((sType == DynamicObjectType::FIRE) || (sType == DynamicObjectType::FIRE3)) && (m_pDynamicObjectList[iIndex] != nullptr))
								m_pDynamicObjectList[iIndex]->m_dwLastTime = m_pDynamicObjectList[iIndex]->m_dwLastTime - (m_pDynamicObjectList[iIndex]->m_dwLastTime / 10);
						}
					break;

				case DynamicObjectType::FIRE3:
				case DynamicObjectType::FIRE:

					if (m_pDynamicObjectList[i]->m_iCount == 1) {

						m_pDynamicObjectList[i]->map_->CheckFireBluring(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY);
					}
					m_pDynamicObjectList[i]->m_iCount++;
					if (m_pDynamicObjectList[i]->m_iCount > 10) m_pDynamicObjectList[i]->m_iCount = 10;


					for (ix = m_pDynamicObjectList[i]->m_sX; ix <= m_pDynamicObjectList[i]->m_sX; ix++)
						for (iy = m_pDynamicObjectList[i]->m_sY; iy <= m_pDynamicObjectList[i]->m_sY; iy++) {

							m_pDynamicObjectList[i]->map_->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
							if (sOwnerH != 0) {

								switch (cOwnerType) {

									case DEF_OWNERTYPE_PLAYER:
										if (m_pClientList[sOwnerH] == nullptr) break;
										if (m_pClientList[sOwnerH]->m_bIsKilled == true) break;

										//if ((m_pClientList[sOwnerH]->m_bIsNeutral == true) && (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) == 0) break;

										iDamage = iDice(1, 6);
										// New 17/05/2004
										if (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)
											m_pClientList[sOwnerH]->m_iHP -= iDamage;

										if (m_pClientList[sOwnerH]->m_iHP <= 0) {

											m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
										} else {
											if (iDamage > 0) {

												game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);

												if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {

													// 1: Hold-Person
													// 2: Paralize
													game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, MagicType::HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT], 0, nullptr);

													m_pClientList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
													game_.delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, MagicType::HOLDOBJECT);
												}
											}
										}
										break;

									case DEF_OWNERTYPE_NPC:
										if (game_.m_pNpcList[sOwnerH] == nullptr) break;

										iDamage = iDice(1, 6);


										switch (game_.m_pNpcList[sOwnerH]->m_sType) {
											case 40: // ESG
											case 41: // GMG
											case 67: // McGaffin
											case 68: // Perry
											case 69: // Devlin
												iDamage = 0;
												break;
										}


										switch (game_.m_pNpcList[sOwnerH]->m_cActionLimit) {
											case 0:
											case 3:
											case 5:
												game_.m_pNpcList[sOwnerH]->m_iHP -= iDamage;
												break;
										}
										//if (m_pNpcList[sOwnerH]->m_cActionLimit == 0)
										//	m_pNpcList[sOwnerH]->m_iHP -= iDamage;

										if (game_.m_pNpcList[sOwnerH]->m_iHP <= 0) {

											game_.m_pNpcList[sOwnerH]->NpcKilledHandler(sOwnerH, cOwnerType, 0);
										} else {

											if (iDice(1, 3) == 2)
												game_.m_pNpcList[sOwnerH]->m_dwTime = dwTime;

											if (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] != 0) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MagicType::HOLDOBJECT] = 0;
											}


											game_.m_pNpcList[sOwnerH]->SendEventToNearClient_TypeA(MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
										}
										break;
								}
							}


							m_pDynamicObjectList[i]->map_->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {

								iDamage = iDice(1, 6);
								m_pClientList[sOwnerH]->m_iHP -= iDamage;

								if (m_pClientList[sOwnerH]->m_iHP <= 0) {

									m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
								} else {
									if (iDamage > 0) {

										game_.m_pClientList[sOwnerH]->SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
									}
								}
							}


							m_pDynamicObjectList[i]->map_->bGetDynamicObject(ix, iy, &sType, &dwRegisterTime, &iIndex);
							if ((sType == DynamicObjectType::ICESTORM) && (m_pDynamicObjectList[iIndex] != nullptr))
								m_pDynamicObjectList[iIndex]->m_dwLastTime = m_pDynamicObjectList[iIndex]->m_dwLastTime - (m_pDynamicObjectList[iIndex]->m_dwLastTime / 10);
						}
					break;
				default: break;
			}
		}
}

void DynamicObjects::CheckDynamicObjectList() {
	register int i;
	uint32_t dwTime = timeGetTime(), dwRegisterTime;
	DynamicObjectType sType;


	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if ((m_pDynamicObjectList[i] != 0) && (m_pDynamicObjectList[i]->m_dwLastTime != 0)) {

			switch (m_pDynamicObjectList[i]->m_sType) {
				case DynamicObjectType::FIRE3:
				case DynamicObjectType::FIRE:

					switch (m_pDynamicObjectList[i]->map_->m_cWhetherStatus) {
						case 0: break;
						case 1:
						case 2:
						case 3:

							m_pDynamicObjectList[i]->m_dwLastTime = m_pDynamicObjectList[i]->m_dwLastTime -
									  (m_pDynamicObjectList[i]->m_dwLastTime / 10) * m_pDynamicObjectList[i]->map_->m_cWhetherStatus;
							break;
					}
					break;
				default: break;
			}
		}
	}


	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if ((m_pDynamicObjectList[i] != nullptr) && (m_pDynamicObjectList[i]->m_dwLastTime != 0) &&
				  ((dwTime - m_pDynamicObjectList[i]->m_dwRegisterTime) >= m_pDynamicObjectList[i]->m_dwLastTime)) {


			m_pDynamicObjectList[i]->map_->bGetDynamicObject(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, &sType, &dwRegisterTime);


			if (dwRegisterTime == m_pDynamicObjectList[i]->m_dwRegisterTime) {
				m_pDynamicObjectList[i]->map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, (short) m_pDynamicObjectList[i]->m_sType, i, 0);

				m_pDynamicObjectList[i]->map_->SetDynamicObject(0, DynamicObjectType::_0, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, dwTime);
			}

			switch (sType) {
				case DynamicObjectType::FISHOBJECT:
				case DynamicObjectType::FISH:

					game_.bDeleteFish(m_pDynamicObjectList[i]->m_sOwner, 2);
					break;
				default: break;
			}


			m_pDynamicObjectList[i].reset();
		}
	}
}

int DynamicObjects::iAddDynamicObjectList(short sOwner, char cOwnerType, DynamicObjectType sType, MapPtr map, short sX, short sY, uint32_t dwLastTime, int iV1) {
	register int i;
	uint32_t dwTime;
	uint32_t dwRegisterTime;

	DynamicObjectType sPreType;
	map->bGetDynamicObject(sX, sY, &sPreType, &dwRegisterTime);
	if (sPreType != DynamicObjectType::_0) return 0;

	switch (sType) {
		case DynamicObjectType::FIRE3:
		case DynamicObjectType::FIRE:
			if (map->bGetIsMoveAllowedTile(sX, sY) == false)
				return 0;
			if (dwLastTime != 0) {
				switch (map->m_cWhetherStatus) {
					case 1: dwLastTime = dwLastTime - (dwLastTime / 2);
						break;
					case 2: dwLastTime = (dwLastTime / 2) - (dwLastTime / 3);
						break;
					case 3: dwLastTime = (dwLastTime / 3) - (dwLastTime / 4);
						break;
				}

				if (dwLastTime == 0) dwLastTime = 1000;
			}
			break;

		case DynamicObjectType::FISHOBJECT:
		case DynamicObjectType::FISH:
			if (map->bGetIsWater(sX, sY) == false)
				return 0;
			break;

		case DynamicObjectType::ARESDENFLAG1:
		case DynamicObjectType::ELVINEFLAG1:
		case DynamicObjectType::MINERAL1:
		case DynamicObjectType::MINERAL2:
			if (map->bGetMoveable(sX, sY) == false)
				return 0;
			map->SetTempMoveAllowedFlag(sX, sY, false);
			break;
		default: break;
	}

	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if (m_pDynamicObjectList[i] == nullptr) {
			dwTime = timeGetTime();

			if (dwLastTime != 0)
				dwLastTime += (iDice(1, 4)*1000);

			m_pDynamicObjectList[i].reset(new class CDynamicObject(sOwner, cOwnerType, sType, map, sX, sY, dwTime, dwLastTime, iV1));
			map->SetDynamicObject(i, sType, sX, sY, dwTime);
			map->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_CONFIRM, sX, sY, (short) sType, i, 0);
			return i;
		}
	}
	return 0;
}
