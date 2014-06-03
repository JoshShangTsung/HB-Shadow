#pragma once
#include <stdint.h>
#include <array>
#include <memory>
#include "Map.h"
class CDynamicObject {
public:
	CDynamicObject(short sOwner, char cOwnerType, short sType, MapPtr map, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1);

	short m_sOwner;
	char m_cOwnerType;

	short m_sType;
	MapPtr map_;
	short m_sX, m_sY;
	uint32_t m_dwRegisterTime;
	uint32_t m_dwLastTime;

	int m_iCount;
	int m_iV1;
};


#define DEF_MAXDYNAMICOBJECTS	60000

struct Maps;
struct Clients;
struct CGame;

struct DynamicObjects {
	typedef std::unique_ptr<CDynamicObject> value_type;
	typedef value_type &ref_type;
	DynamicObjects(CGame &game, Maps &maps, Clients &clients);
	void DynamicObjectEffectProcessor();
	void CheckDynamicObjectList();
	int iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, MapPtr map, short sX, short sY, uint32_t dwLastTime, int iV1 = 0);

	void clear() {
		m_pDynamicObjectList = {
			{}};
	}
	ref_type operator[](size_t index) {
		return m_pDynamicObjectList[index];
	}
private:
	CGame &game_;
	Maps &m_pMapList;
	Clients &m_pClientList;
	typedef std::array<std::unique_ptr<CDynamicObject>, DEF_MAXDYNAMICOBJECTS> List;
	List m_pDynamicObjectList;
};
