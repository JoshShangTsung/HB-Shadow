#include "Unit.h"
#include "Game.h"
Unit::~Unit() {
}

UnitPtr Unit::getPtr() {
	return _getPtr();
}

CMap &Unit::getMap() {
	return _getMap();
}

bool Unit::bCheckResistingIceSuccess() {
	int iTargetIceResistRatio = _getEffectiveIceResist();
	if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;
	int iResult = iDice(1, 100);
	return (iResult <= iTargetIceResistRatio);
}

void Unit::SendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3) {
	_sendEventToNearClient_TypeA(dwMsgID, wMsgType, sV1, sV2, sV3);
}

