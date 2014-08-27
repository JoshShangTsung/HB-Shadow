#pragma once

#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "Tile.h"
#include "ActionID.h"
#include "Game.h"
#include "TileSpr.h"

#define MAPDATASIZEX	40
#define MAPDATASIZEY	35

class CMapData {
public:

	void * operator new (size_t size) {
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	};

	void operator delete(void * mem) {
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	};

	CMapData(class CGame * pGame);
	void Init();
	void OpenMapDataFile(char * cFn);
	void GetOwnerStatusByObjectID(uint16_t wObjectID, char * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pStatus, int * pColor, char * pName);
	void ClearDeadChatMsg(short sX, short sY);
	void ClearChatMsg(short sX, short sY);
	void ShiftMapData(char cDir);
	void _bDecodeMapInfo(char * pHeader);
	bool __fastcall bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex);
	bool __fastcall bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int iStatus, char * pName);
	bool __fastcall bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, char * pFrame, char * pName, short * pItemSprite, short * pItemSpriteFrame, int * pCharIndex);
	bool __fastcall bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2);
	bool __fastcall bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int iStatus, char * pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
	bool __fastcall bGetOwner(short sX, short sY, char * pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID);
	bool bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent);
	bool bIsTeleportLoc(short sX, short sY);
	bool bGetIsLocateable(short sX, short sY);
	bool bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, char cItemColor, bool bDropEffect = true);
	int iObjectFrameCounter(char * cPlayerName, short sViewPointX, short sViewPointY);

	class CTile m_pData[MAPDATASIZEX][MAPDATASIZEY];
	class CTile m_pTmpData[MAPDATASIZEX][MAPDATASIZEY];
	class CTileSpr m_tile[752][752];
	class CGame * m_pGame;

	struct {
		short m_sMaxFrame;
		short m_sFrameTime;
	} m_stFrame[DEF_TOTALCHARACTERS][DEF_TOTALACTION];
	uint32_t m_dwFrameTime;
	uint32_t m_dwDOframeTime;
	uint32_t m_dwFrameCheckTime;
	int m_iObjectIDcacheLocX[30000];
	int m_iObjectIDcacheLocY[30000];
	uint32_t m_dwFrameAdjustTime;
	short m_sMapSizeX;
	short m_sMapSizeY;
	short m_sRectX;
	short m_sRectY;
	short m_sPivotX;
	short m_sPivotY;
};
