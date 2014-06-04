#pragma once
#include <memory>

struct CMap;
struct Unit;
typedef std::shared_ptr<Unit> UnitPtr;
struct Unit {
	UnitPtr getPtr();
	CMap &getMap();
	bool bCheckResistingIceSuccess();
	void SendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3);
	virtual ~Unit();
protected:
	virtual UnitPtr _getPtr() = 0;
	virtual CMap &_getMap() = 0;
	virtual int _getEffectiveIceResist()=0;
	virtual void _sendEventToNearClient_TypeA(uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3)=0;
};

