#include "DynamicObject.h"
#include "Map.h"
#include "Client.h"
#include "Game.h"
#include "DynamicObjectID.h"

CDynamicObject::CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1) {
	m_sOwner = sOwner;
	m_cOwnerType = cOwnerType;

	m_sType = sType;
	m_cMapIndex = cMapIndex;
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
	int ix, iy, iIndex;
	short sOwnerH, iDamage, sType;
	char cOwnerType;
	uint32_t dwTime = timeGetTime(), dwRegisterTime;

	//
	for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] != nullptr) {
			switch (m_pDynamicObjectList[i]->m_sType) {

				case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:

					for (ix = m_pDynamicObjectList[i]->m_sX - 1; ix <= m_pDynamicObjectList[i]->m_sX + 1; ix++)
						for (iy = m_pDynamicObjectList[i]->m_sY - 1; iy <= m_pDynamicObjectList[i]->m_sY + 1; iy++) {

							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
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

												game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_HP, 0, 0, 0, nullptr);

												if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {

													// 1: Hold-Person
													// 2: Paralize
													game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ], 0, nullptr);

													m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
													game_.delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
												}
											}


											if ((game_.bCheckResistingMagicSuccess(1, sOwnerH, DEF_OWNERTYPE_PLAYER, 100) == false) &&
													  (m_pClientList[sOwnerH]->m_bIsPoisoned == false)) {

												m_pClientList[sOwnerH]->m_bIsPoisoned = true;
												m_pClientList[sOwnerH]->m_iPoisonLevel = m_pDynamicObjectList[i]->m_iV1;
												m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;

												game_.SetPoisonFlag(sOwnerH, cOwnerType, true); // poison aura appears from dynamic objects
												game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pClientList[sOwnerH]->m_iPoisonLevel, 0, nullptr);
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

											game_.NpcKilledHandler(sOwnerH, cOwnerType, sOwnerH, 0);
										} else {

											if (iDice(1, 3) == 2)
												game_.m_pNpcList[sOwnerH]->m_dwTime = dwTime;

											if (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
											}


											game_.SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
										}
										break;
								}
							}
						}
					break;

				case DEF_DYNAMICOBJECT_ICESTORM:
					for (ix = m_pDynamicObjectList[i]->m_sX - 2; ix <= m_pDynamicObjectList[i]->m_sX + 2; ix++)
						for (iy = m_pDynamicObjectList[i]->m_sY - 2; iy <= m_pDynamicObjectList[i]->m_sY + 2; iy++) {

							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
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

												game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_HP, 0, 0, 0, nullptr);

												if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] == 1) {

													game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ], 0, nullptr);

													m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
													game_.delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
												}
											}

											if ((game_.bCheckResistingIceSuccess(1, sOwnerH, DEF_OWNERTYPE_PLAYER, m_pDynamicObjectList[i]->m_iV1) == false) &&
													  (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0)) {

												m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
												game_.SetIceFlag(sOwnerH, cOwnerType, true);

												game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (20 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

												game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, nullptr);
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

											game_.NpcKilledHandler(sOwnerH, cOwnerType, sOwnerH, 0);
										} else {

											if (iDice(1, 3) == 2)
												game_.m_pNpcList[sOwnerH]->m_dwTime = dwTime;

											if (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
											}


											game_.SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);


											if ((game_.bCheckResistingIceSuccess(1, sOwnerH, DEF_OWNERTYPE_NPC, m_pDynamicObjectList[i]->m_iV1) == false) &&
													  (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] == 0)) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_ICE ] = 1;
												game_.SetIceFlag(sOwnerH, cOwnerType, true);

												game_.delayEvents_.add(DelayEventType::MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (20 * 1000),
														  sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
											}
										}
										break;
								}
							}


							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {

								iDamage = iDice(3, 2);
								m_pClientList[sOwnerH]->m_iHP -= iDamage;

								if (m_pClientList[sOwnerH]->m_iHP <= 0) {

									m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
								} else {
									if (iDamage > 0) {

										game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_HP, 0, 0, 0, nullptr);
									}
								}
							}


							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(ix, iy, &sType, &dwRegisterTime, &iIndex);
							if (((sType == DEF_DYNAMICOBJECT_FIRE) || (sType == DEF_DYNAMICOBJECT_FIRE3)) && (m_pDynamicObjectList[iIndex] != nullptr))
								m_pDynamicObjectList[iIndex]->m_dwLastTime = m_pDynamicObjectList[iIndex]->m_dwLastTime - (m_pDynamicObjectList[iIndex]->m_dwLastTime / 10);
						}
					break;

				case DEF_DYNAMICOBJECT_FIRE3:
				case DEF_DYNAMICOBJECT_FIRE:

					if (m_pDynamicObjectList[i]->m_iCount == 1) {

						game_.CheckFireBluring(m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY);
					}
					m_pDynamicObjectList[i]->m_iCount++;
					if (m_pDynamicObjectList[i]->m_iCount > 10) m_pDynamicObjectList[i]->m_iCount = 10;


					for (ix = m_pDynamicObjectList[i]->m_sX; ix <= m_pDynamicObjectList[i]->m_sX; ix++)
						for (iy = m_pDynamicObjectList[i]->m_sY; iy <= m_pDynamicObjectList[i]->m_sY; iy++) {

							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
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

												game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_HP, 0, 0, 0, nullptr);

												if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {

													// 1: Hold-Person
													// 2: Paralize
													game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ], 0, nullptr);

													m_pClientList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
													game_.delayEvents_.remove(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
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

											game_.NpcKilledHandler(sOwnerH, cOwnerType, sOwnerH, 0);
										} else {

											if (iDice(1, 3) == 2)
												game_.m_pNpcList[sOwnerH]->m_dwTime = dwTime;

											if (game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] != 0) {

												game_.m_pNpcList[sOwnerH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_HOLDOBJECT ] = 0;
											}


											game_.SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
										}
										break;
								}
							}


							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != nullptr) &&
									  (m_pClientList[sOwnerH]->m_iHP > 0)) {

								iDamage = iDice(1, 6);
								m_pClientList[sOwnerH]->m_iHP -= iDamage;

								if (m_pClientList[sOwnerH]->m_iHP <= 0) {

									m_pClientList[sOwnerH]->ClientKilledHandler(sOwnerH, cOwnerType, iDamage);
								} else {
									if (iDamage > 0) {

										game_.SendNotifyMsg(0, *m_pClientList[sOwnerH], DEF_NOTIFY_HP, 0, 0, 0, nullptr);
									}
								}
							}


							m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(ix, iy, &sType, &dwRegisterTime, &iIndex);
							if ((sType == DEF_DYNAMICOBJECT_ICESTORM) && (m_pDynamicObjectList[iIndex] != nullptr))
								m_pDynamicObjectList[iIndex]->m_dwLastTime = m_pDynamicObjectList[iIndex]->m_dwLastTime - (m_pDynamicObjectList[iIndex]->m_dwLastTime / 10);
						}
					break;
			}
		}
}

void DynamicObjects::CheckDynamicObjectList() {
	register int i;
	uint32_t dwTime = timeGetTime(), dwRegisterTime;
	short sType;


	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if ((m_pDynamicObjectList[i] != 0) && (m_pDynamicObjectList[i]->m_dwLastTime != 0)) {

			switch (m_pDynamicObjectList[i]->m_sType) {
				case DEF_DYNAMICOBJECT_FIRE3:
				case DEF_DYNAMICOBJECT_FIRE:

					switch (m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_cWhetherStatus) {
						case 0: break;
						case 1:
						case 2:
						case 3:

							m_pDynamicObjectList[i]->m_dwLastTime = m_pDynamicObjectList[i]->m_dwLastTime -
									  (m_pDynamicObjectList[i]->m_dwLastTime / 10) * m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_cWhetherStatus;
							break;
					}
					break;
			}
		}
	}


	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if ((m_pDynamicObjectList[i] != nullptr) && (m_pDynamicObjectList[i]->m_dwLastTime != 0) &&
				  ((dwTime - m_pDynamicObjectList[i]->m_dwRegisterTime) >= m_pDynamicObjectList[i]->m_dwLastTime)) {


			m_pMapList[ m_pDynamicObjectList[i]->m_cMapIndex ]->bGetDynamicObject(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, &sType, &dwRegisterTime);


			if (dwRegisterTime == m_pDynamicObjectList[i]->m_dwRegisterTime) {
				game_.SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, m_pDynamicObjectList[i]->m_sType, i, 0);

				m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, dwTime);
			}

			switch (sType) {
				case DEF_DYNAMICOBJECT_FISHOBJECT:
				case DEF_DYNAMICOBJECT_FISH:

					game_.bDeleteFish(m_pDynamicObjectList[i]->m_sOwner, 2);
					break;
			}


			m_pDynamicObjectList[i].reset();
		}
	}
}

int DynamicObjects::iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwLastTime, int iV1) {
	register int i;
	short sPreType;
	uint32_t dwTime, dwRegisterTime;

	m_pMapList[cMapIndex]->bGetDynamicObject(sX, sY, &sPreType, &dwRegisterTime);
	if (sPreType != 0) return 0;

	switch (sType) {
		case DEF_DYNAMICOBJECT_FIRE3:
		case DEF_DYNAMICOBJECT_FIRE:
			if (m_pMapList[cMapIndex]->bGetIsMoveAllowedTile(sX, sY) == false)
				return 0;
			if (dwLastTime != 0) {
				switch (m_pMapList[cMapIndex]->m_cWhetherStatus) {
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

		case DEF_DYNAMICOBJECT_FISHOBJECT:
		case DEF_DYNAMICOBJECT_FISH:
			if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false)
				return 0;
			break;

		case DEF_DYNAMICOBJECT_ARESDENFLAG1:
		case DEF_DYNAMICOBJECT_ELVINEFLAG1:
		case DEF_DYNAMICOBJECT_MINERAL1:
		case DEF_DYNAMICOBJECT_MINERAL2:
			if (m_pMapList[cMapIndex]->bGetMoveable(sX, sY) == false)
				return 0;
			m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
			break;

	}

	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++) {
		if (m_pDynamicObjectList[i] == nullptr) {
			dwTime = timeGetTime();

			if (dwLastTime != 0)
				dwLastTime += (iDice(1, 4)*1000);

			m_pDynamicObjectList[i].reset(new class CDynamicObject(sOwner, cOwnerType, sType, cMapIndex, sX, sY, dwTime, dwLastTime, iV1));
			m_pMapList[cMapIndex]->SetDynamicObject(i, sType, sX, sY, dwTime);
			game_.SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_CONFIRM, cMapIndex, sX, sY, sType, i, 0);

			return i;
		}
	}
	return 0;
}
