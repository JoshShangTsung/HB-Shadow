#include "Map.h"
#include "Game.h"
#include <cstring>

extern char G_cTxt[512];
extern void PutLogFileList(char * cStr);

CMap::CMap(int id, CGame &game) : id_(id), m_pClientList(game.m_pClientList), game_(game), m_bIsSnowEnabled(false), npcs_(game.m_pNpcList) {
	int i;
	int ix;
	int iy;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = nullptr;

	for (i = 0; i < DEF_MAXWAYPOINTCFG; i++) {
		m_WaypointList[i].x = -1;
		m_WaypointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXMGAR; i++) {
		m_rcMobGenAvoidRect[i].top = -1;
		m_rcMobGenAvoidRect[i].left = -1;
	}

	for (i = 0; i < DEF_MAXNMR; i++) {
		m_rcNoAttackRect[i].top = -1;
		m_rcNoAttackRect[i].left = -1;
	}

	for (i = 0; i < DEF_MAXSPOTMOBGENERATOR; i++) {
		m_stSpotMobGenerator[i].bDefined = false;
		m_stSpotMobGenerator[i].iTotalActiveMob = 0;
	}

	for (i = 0; i < DEF_MAXFISHPOINT; i++) {
		m_FishPointList[i].x = -1;
		m_FishPointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXMINERALPOINT; i++) {
		m_MineralPointList[i].x = -1;
		m_MineralPointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXINITIALPOINT; i++) {
		m_pInitialPoint[i].x = -1;
		m_pInitialPoint[i].y = -1;
	}

	for (i = 0; i < 1000; i++)
		m_bNamingValueUsingStatus[i] = false;

	for (i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		m_pOccupyFlag[i] = nullptr;

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		m_pStrategicPointList[i] = nullptr;

	for (i = 0; i < DEF_MAXENERGYSPHERES; i++) {
		m_stEnergySphereCreationList[i].cType = 0;
		m_stEnergySphereGoalList[i].cResult = 0;
	}

	m_bIsHeldenianMap = false;
	m_iTotalActiveObject = 0;
	m_iTotalAliveObject = 0;
	m_iTotalItemEvents = 0;
	sMobEventAmount = 15;
	//m_sInitialPointX = 0;
	//m_sInitialPointY = 0;

	m_bIsFixedDayMode = false;

	m_iTotalFishPoint = 0;
	m_iMaxFish = 0;
	m_iCurFish = 0;

	m_iTotalMineralPoint = 0;
	m_iMaxMineral = 0;
	m_iCurMineral = 0;

	m_pTile = nullptr;

	m_cWhetherStatus = 0;
	m_cType = DEF_MAPTYPE_NORMAL;

	m_iLevelLimit = 0;
	m_iUpperLevelLimit = 0; // v1.4
	m_bMineralGenerator = false;

	m_iTotalOccupyFlags = 0;

	m_bIsAttackEnabled = true;
	m_cRandomMobGeneratorLevel = 0;

	m_bIsFightZone = false;

	m_iTotalEnergySphereCreationPoint = 0;
	m_iTotalEnergySphereGoalPoint = 0;

	m_bIsEnergySphereGoalEnabled = false;
	m_iCurEnergySphereGoalPointIndex = -1;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
		for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
			m_stSectorInfo[ix][iy].iNeutralActivity = 0;
			m_stSectorInfo[ix][iy].iAresdenActivity = 0;
			m_stSectorInfo[ix][iy].iElvineActivity = 0;
			m_stSectorInfo[ix][iy].iMonsterActivity = 0;
			m_stSectorInfo[ix][iy].iPlayerActivity = 0;

			m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
			m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
			m_stTempSectorInfo[ix][iy].iElvineActivity = 0;
			m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
			m_stTempSectorInfo[ix][iy].iPlayerActivity = 0;
		}

	m_iMaxNx = m_iMaxNy = m_iMaxAx = m_iMaxAy = m_iMaxEx = m_iMaxEy = m_iMaxMx = m_iMaxMy = m_iMaxPx = m_iMaxPy = 0;

	for (i = 0; i < DEF_MAXHELDENIANDOOR; i++) {
		m_stHeldenianGateDoor[i].cDir = 0;
		m_stHeldenianGateDoor[i].dX = 0;
		m_stHeldenianGateDoor[i].dY = 0;
	}

	for (i = 0; i < DEF_MAXHELDENIANTOWER; i++) {
		m_stHeldenianTower[i].sTypeID = 0;
		m_stHeldenianTower[i].dX = 0;
		m_stHeldenianTower[i].dY = 0;
		m_stHeldenianTower[i].cSide = 0;
	}

	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].dX = 0;
		m_stStrikePoint[i].dY = 0;
		m_stStrikePoint[i].iHP = 0;
		m_stStrikePoint[i].iMapIndex = -1;
		std::memset(m_stStrikePoint[i].cRelatedMapName, 0, sizeof (m_stStrikePoint[i].cRelatedMapName));
	}
	m_iTotalStrikePoints = 0;
	m_bIsDisabled = false;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = 0;
		m_stCrusadeStructureInfo[i].cSide = 0;
		m_stCrusadeStructureInfo[i].sX = 0;
		m_stCrusadeStructureInfo[i].sY = 0;
	}
	m_iTotalCrusadeStructures = 0;
	m_iTotalAgriculture = 0;
}

CMap::~CMap() {

	int i;

	if (m_pTile != nullptr)
		delete []m_pTile;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		if (m_pTeleportLoc[i] != nullptr) delete m_pTeleportLoc[i];

	for (i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pOccupyFlag[i] != nullptr) delete m_pOccupyFlag[i];

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		if (m_pStrategicPointList[i] != nullptr) delete m_pStrategicPointList[i];
}

void CMap::SetOwner(short sOwner, char cOwnerClass, short sX, short sY) {
	class CTile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	pTile->m_sOwner = sOwner;
	pTile->m_cOwnerClass = cOwnerClass;
}

char _tmp_cMoveDirX[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};
char _tmp_cMoveDirY[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};

bool CMap::bCheckFlySpaceAvailable(short sX, char sY, char cDir, short sOwner) {
	class CTile * pTile;
	short dX;
	short dY;

	if ((cDir <= 0) || (cDir > 8)) return 0;
	dX = _tmp_cMoveDirX[cDir] + sX;
	dY = _tmp_cMoveDirY[cDir] + sY;
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return 0;
	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	if (pTile->m_sOwner != 0) return 0;
	pTile->m_sOwner = sOwner;
	return 1;
}

void CMap::SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY) {
	class CTile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	pTile->m_sDeadOwner = sOwner;
	pTile->m_cDeadOwnerClass = cOwnerClass;
}

/*********************************************************************************************************************
 **  void CMap::GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY)										**
 **  description			:: check if the tile contains a player														**
 **  last updated		:: November 17, 2004; 10:48 PM; Hypnotoad													**
 **	return value		:: void																						**
 **  commentary			::	-	added check to see if owner is class 1 or if is greater than max clients 			**
 **********************************************************************************************************************/
void CMap::GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY) {
	class CTile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		*pOwner = 0;
		*pOwnerClass = 0;
		return;
	}

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	*pOwner = pTile->m_sOwner;
	*pOwnerClass = pTile->m_cOwnerClass;

	if ((*pOwnerClass == 1) && (*pOwner > DEF_MAXCLIENTS)) {
		*pOwner = 0;
		*pOwnerClass = 0;
		return;
	}

	if (pTile->m_sOwner == 0) *pOwnerClass = 0;
}

/*********************************************************************************************************************
 **  void CMap::GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY)									**
 **  description			:: check if the tile contains a dead player													**
 **  last updated		:: November 20, 2004; 9:13 PM; Hypnotoad													**
 **	return value		:: void																						**
 **********************************************************************************************************************/
void CMap::GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY) {
	class CTile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		*pOwner = 0;
		*pOwnerClass = 0;
		return;
	}

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	*pOwner = pTile->m_sDeadOwner;
	*pOwnerClass = pTile->m_cDeadOwnerClass;
}

bool CMap::bGetMoveable(short dX, short dY, short * pDOtype, short * pTopItem) {
	class CTile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return false;
	pTile = (class CTile *)(m_pTile + dX + dY * m_sSizeY);

	if (pDOtype != nullptr) *pDOtype = pTile->m_sDynamicObjectType;
	if (pTopItem != nullptr) *pTopItem = pTile->m_cTotalItem;

	if (pTile->m_sOwner != 0) return false;
	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;

	return true;
}

bool CMap::bGetIsMoveAllowedTile(short dX, short dY) {
	class CTile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return false;

	pTile = (class CTile *)(m_pTile + dX + dY * m_sSizeY);

	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;

	return true;
}

bool CMap::bGetIsTeleport(short dX, short dY) {
	class CTile * pTile;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

	pTile = (class CTile *)(m_pTile + dX + dY * m_sSizeY);

	if (pTile->m_bIsTeleport == false) return false;

	return true;
}

void CMap::ClearOwner(int /*iDebugCode*/, short sOwnerH, char cOwnerType, short sX, short sY) {
	class CTile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);


	if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
		pTile->m_sOwner = 0;
		pTile->m_cOwnerClass = 0;
	}

	//
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner = 0;
		pTile->m_cDeadOwnerClass = 0;
	}
}

char _tmp_cEmptyAreaX[] = {0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2};
char _tmp_cEmptyAreaY[] = {0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2};

void CMap::ClearBigOwner(short sOwnerH, char cOwnerType, short pX, short pY, char cArea) {
	short sX;
	short sY;
	short sAreaSquared;
	class CTile * pTile;
	register int i;

	sX = 0;
	sY = 0;
	sAreaSquared = cArea*cArea;
	if ((pX < 0) || (pX >= m_sSizeX) || (pY < 0) || (pY >= m_sSizeY)) return;
	pTile = (class CTile *)(m_pTile + pX + pY * m_sSizeY);
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner = 0;
		pTile->m_cDeadOwnerClass = 0;
	}
	for (i = 0; i < sAreaSquared; i++) {
		sX = pX + _tmp_cEmptyAreaX[i];
		sY = pY + _tmp_cEmptyAreaY[i];
		pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
		if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
			pTile->m_sOwner = 0;
			pTile->m_cOwnerClass = 0;
		}
	}
}

void CMap::ClearDeadOwner(short sX, short sY) {
	class CTile * pTile;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	pTile->m_sDeadOwner = 0;
	pTile->m_cDeadOwnerClass = 0;
}

bool CMap::bSetItem(short sX, short sY, class CItem * pItem) {
	class CTile * pTile;
	register int i;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return 0;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);


	if (pTile->m_pItem[DEF_TILE_PER_ITEMS - 1] != nullptr)
		delete pTile->m_pItem[DEF_TILE_PER_ITEMS - 1];
	else pTile->m_cTotalItem++;

	for (i = DEF_TILE_PER_ITEMS - 2; i >= 0; i--)
		pTile->m_pItem[i + 1] = pTile->m_pItem[i];

	pTile->m_pItem[0] = pItem;
	//pTile->m_cTotalItem++;
	return true;
}

class CItem * CMap::pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor) {
	class CTile * pTile;
	class CItem * pItem;
	register int i;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return nullptr;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pItem = pTile->m_pItem[0];
	if (pTile->m_cTotalItem == 0) return nullptr;

	for (i = 0; i <= DEF_TILE_PER_ITEMS - 2; i++)
		pTile->m_pItem[i] = pTile->m_pItem[i + 1];
	pTile->m_cTotalItem--;
	pTile->m_pItem[pTile->m_cTotalItem] = nullptr;

	if (pTile->m_pItem[0] == nullptr) {
		*pRemainItemSprite = 0;
		*pRemainItemSpriteFrame = 0;
		*pRemainItemColor = 0;
	} else {
		*pRemainItemSprite = pTile->m_pItem[0]->m_sSprite;
		*pRemainItemSpriteFrame = pTile->m_pItem[0]->m_sSpriteFrame;
		*pRemainItemColor = pTile->m_pItem[0]->m_cItemColor;
	}

	return pItem;
}

int CMap::iCheckItem(short sX, short sY) {
	class CTile * pTile;
	class CItem * pItem;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return 0;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);
	pItem = pTile->m_pItem[0];
	if (pTile->m_cTotalItem == 0) return 0;

	return pItem->m_sIDnum;
}

bool CMap::bIsValidLoc(short sX, short sY) {
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false;
	return true;
}

bool CMap::bInit(char * pName) {
	int i;

	std::memset(m_cName, 0, sizeof (m_cName));
	strcpy(m_cName, pName);

	std::memset(m_cLocationName, 0, sizeof (m_cLocationName));

	if (_bDecodeMapDataFileContents() == false)
		return false;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = nullptr;

	return true;
}

bool CMap::_bDecodeMapDataFileContents() {
	HANDLE hFile;
	char cMapFileName[256], cHeader[260], cTemp[100];

	register int i, ix, iy;
	char * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok * pStrTok = nullptr;
	class CTile * pTile;
	short * sp;

	std::memset(cMapFileName, 0, sizeof (cMapFileName));
	strcat(cMapFileName, "mapdata\\");
	strcat(cMapFileName, m_cName);
	strcat(cMapFileName, ".amd");

	hFile = CreateFile(cMapFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	std::memset(cHeader, 0, sizeof (cHeader));
	DWORD nRead;
	ReadFile(hFile, (char *) cHeader, 256, &nRead, nullptr);




	for (i = 0; i < 256; i++)
		if (cHeader[i] == 0) cHeader[i] = ' ';

	cReadMode = 0;

	pStrTok = new class CStrTok(cHeader, seps);
	token = pStrTok->pGet();
	//token = strtok( cHeader, seps );
	while (token != nullptr) {

		if (cReadMode != 0) {
			switch (cReadMode) {
				case 1:
					m_sSizeX = atoi(token);
					cReadMode = 0;
					break;
				case 2:
					m_sSizeY = atoi(token);
					cReadMode = 0;
					break;
				case 3:
					m_sTileDataSize = atoi(token);
					cReadMode = 0;
					break;
			}
		} else {
			if (memcmp(token, "MAPSIZEX", 8) == 0) cReadMode = 1;
			if (memcmp(token, "MAPSIZEY", 8) == 0) cReadMode = 2;
			if (memcmp(token, "TILESIZE", 8) == 0) cReadMode = 3;
		}

		//token = strtok( nullptr, seps );
		token = pStrTok->pGet();
	}


	m_pTile = (class CTile *)new class CTile[m_sSizeX * m_sSizeY];


	for (iy = 0; iy < m_sSizeY; iy++)
		for (ix = 0; ix < m_sSizeX; ix++) {
			ReadFile(hFile, (char *) cTemp, m_sTileDataSize, &nRead, nullptr);
			pTile = (class CTile *)(m_pTile + ix + iy * m_sSizeY);
			if ((cTemp[8] & 0x80) != 0) {

				pTile->m_bIsMoveAllowed = false;
			} else pTile->m_bIsMoveAllowed = true;

			if ((cTemp[8] & 0x40) != 0) {

				pTile->m_bIsTeleport = true;
			} else pTile->m_bIsTeleport = false;

			if ((cTemp[8] & 0x20) != 0) {
				pTile->m_bIsFarm = true;
			} else pTile->m_bIsFarm = false;

			sp = (short *) &cTemp[0];
			if (*sp == 19) {

				pTile->m_bIsWater = true;
			} else pTile->m_bIsWater = false;


		}

	CloseHandle(hFile);

	if (pStrTok != nullptr) delete pStrTok;
	return true;
}

bool CMap::bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir) {
	register int i;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		if ((m_pTeleportLoc[i] != nullptr) && (m_pTeleportLoc[i]->m_sSrcX == sX) && (m_pTeleportLoc[i]->m_sSrcY == sY)) {

			memcpy(pMapName, m_pTeleportLoc[i]->m_cDestMapName, 10);
			*pDx = m_pTeleportLoc[i]->m_sDestX;
			*pDy = m_pTeleportLoc[i]->m_sDestY;
			*pDir = m_pTeleportLoc[i]->m_cDir;
			return true;
		}

	return false;
}

void CMap::SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint32_t dwRegisterTime) {
	class CTile * pTile;


	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);

	pTile->m_wDynamicObjectID = wID;
	pTile->m_sDynamicObjectType = sType;
	pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

bool CMap::bGetDynamicObject(short sX, short sY, short *pType, uint32_t *pRegisterTime, int * pIndex) {
	class CTile * pTile;


	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return false;

	pTile = (class CTile *)(m_pTile + sX + sY * m_sSizeY);

	*pType = pTile->m_sDynamicObjectType;
	*pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
	if (pIndex != nullptr) *pIndex = pTile->m_wDynamicObjectID;

	return true;
}

int CMap::iGetEmptyNamingValue() {
	register int i;

	for (i = 0; i < 1000; i++)
		if (m_bNamingValueUsingStatus[i] == false) {

			m_bNamingValueUsingStatus[i] = true;
			return i;
		}

	return -1;
}

void CMap::SetNamingValueEmpty(int iValue) {
	m_bNamingValueUsingStatus[iValue] = false;
}

bool CMap::bGetIsWater(short dX, short dY) {
	class CTile * pTile;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;

	pTile = (class CTile *)(m_pTile + dX + dY * m_sSizeY);

	if (pTile->m_bIsWater == false) return false;

	return true;
}

bool CMap::bRemoveCropsTotalSum() {
	if (m_iTotalAgriculture < DEF_MAXAGRICULTURE) {
		m_iTotalAgriculture--;
		if (m_iTotalAgriculture < 0) {
			m_iTotalAgriculture = 0;
		}
		return true;
	}
	return false;
}

bool CMap::bAddCropsTotalSum() {
	if (m_iTotalAgriculture < DEF_MAXAGRICULTURE) {
		m_iTotalAgriculture++;
		return true;
	}
	return false;
}

bool CMap::bGetIsFarm(short tX, short tY) {
	class CTile * pTile;

	if ((tX < 14) || (tX >= m_sSizeX - 16) || (tY < 12) || (tY >= m_sSizeY - 14)) return false;

	pTile = (class CTile *)(m_pTile + tX + tY * m_sSizeY);

	if (pTile->m_bIsFarm == false) return false;

	return true;
}

void CMap::SetTempMoveAllowedFlag(int dX, int dY, bool bFlag) {
	class CTile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return;

	pTile = (class CTile *)(m_pTile + dX + dY * m_sSizeY);
	pTile->m_bIsTempMoveAllowed = bFlag;
}

int CMap::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI) {
	int i;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pOccupyFlag[i] == nullptr) {

			m_pOccupyFlag[i] = new class COccupyFlag(dX, dY, iSide, iEKNum, iDOI);
			if (m_pOccupyFlag == nullptr) return -1;
			else return i;
		}

	return -1;
}

void CMap::ClearSectorInfo() {
	int ix;
	int iy;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
		for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
			m_stSectorInfo[ix][iy].iNeutralActivity = 0;
			m_stSectorInfo[ix][iy].iAresdenActivity = 0;
			m_stSectorInfo[ix][iy].iElvineActivity = 0;
			m_stSectorInfo[ix][iy].iMonsterActivity = 0;
			m_stSectorInfo[ix][iy].iPlayerActivity = 0;
		}
}

void CMap::ClearTempSectorInfo() {
	int ix;
	int iy;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
		for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
			m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
			m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
			m_stTempSectorInfo[ix][iy].iElvineActivity = 0;
			m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
			m_stTempSectorInfo[ix][iy].iPlayerActivity = 0;
		}
}

void CMap::_SetupNoAttackArea() {
	int i;
	int ix;
	int iy;
	class CTile * pTile;

	for (i = 0; i < DEF_MAXNMR; i++) {
		if ((m_rcNoAttackRect[i].top > 0)) {

			for (ix = m_rcNoAttackRect[i].left; ix <= m_rcNoAttackRect[i].right; ix++)
				for (iy = m_rcNoAttackRect[i].top; iy <= m_rcNoAttackRect[i].bottom; iy++) {
					pTile = (class CTile *)(m_pTile + ix + iy * m_sSizeY);
					pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;
				}
		} else if (m_rcNoAttackRect[i].top == -10) {

			for (ix = 0; ix < m_sSizeX; ix++)
				for (iy = 0; iy < m_sSizeY; iy++) {
					pTile = (class CTile *)(m_pTile + ix + iy * m_sSizeY);
					pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;
				}
		}
	}
}

/*********************************************************************************************************************
 **  int CMap::iGetAttribute(int dX, int dY, int iBitMask)															**
 **  description			:: check if the tile contains a dead player													**
 **  last updated		:: November 20, 2004; 9:55 PM; Hypnotoad													**
 **	return value		:: int																						**
 **********************************************************************************************************************/
int CMap::iGetAttribute(int dX, int dY, int iBitMask) {
	class CTile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

	pTile = (class CTile *)(m_pTile + dX + dY * m_sSizeY);
	return (pTile->m_iAttribute & iBitMask);
}

bool CMap::bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide) {
	register int i;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
		if (m_stCrusadeStructureInfo[i].cType == 0) {
			m_stCrusadeStructureInfo[i].cType = cType;
			m_stCrusadeStructureInfo[i].cSide = cSide;
			m_stCrusadeStructureInfo[i].sX = sX;
			m_stCrusadeStructureInfo[i].sY = sY;

			m_iTotalCrusadeStructures++;
			return true;
		}

	return false;
}

bool CMap::bRemoveCrusadeStructureInfo(short sX, short sY) {
	register int i;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
		if ((m_stCrusadeStructureInfo[i].sX == sX) && (m_stCrusadeStructureInfo[i].sY == sY)) {
			m_stCrusadeStructureInfo[i].cType = 0;
			m_stCrusadeStructureInfo[i].cSide = 0;
			m_stCrusadeStructureInfo[i].sX = 0;
			m_stCrusadeStructureInfo[i].sY = 0;
			goto RCSI_REARRANGE;
		}

	return false;

RCSI_REARRANGE:
	;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES - 1; i++)
		if ((m_stCrusadeStructureInfo[i].cType == 0) && (m_stCrusadeStructureInfo[i + 1].cType != 0)) {
			m_stCrusadeStructureInfo[i].cType = m_stCrusadeStructureInfo[i + 1].cType;
			m_stCrusadeStructureInfo[i].cSide = m_stCrusadeStructureInfo[i + 1].cSide;
			m_stCrusadeStructureInfo[i].sX = m_stCrusadeStructureInfo[i + 1].sX;
			m_stCrusadeStructureInfo[i].sY = m_stCrusadeStructureInfo[i + 1].sY;

			m_stCrusadeStructureInfo[i + 1].cType = 0;
			m_stCrusadeStructureInfo[i + 1].cSide = 0;
			m_stCrusadeStructureInfo[i + 1].sX = 0;
			m_stCrusadeStructureInfo[i + 1].sY = 0;
		}

	m_iTotalCrusadeStructures--;
	return true;
}

void CMap::RestoreStrikePoints() {
	int i;

	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].iInitHP = m_stStrikePoint[i].iHP;
	}
}

constexpr char _tmp_cEmptyPosX[] = {0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2};
constexpr char _tmp_cEmptyPosY[] = {0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2};

bool CMap::bGetEmptyPosition(short * pX, short * pY) {

	for (int i = 0; i < 25; i++) {
		if ((this->bGetMoveable(*pX + _tmp_cEmptyPosX[i], *pY + _tmp_cEmptyPosY[i]) == true) &&
				  (this->bGetIsTeleport(*pX + _tmp_cEmptyPosX[i], *pY + _tmp_cEmptyPosY[i]) == false)) {
			*pX += _tmp_cEmptyPosX[i];
			*pY += _tmp_cEmptyPosY[i];
			return true;
		}
	}
	short sX;
	short sY;
	this->GetMapInitialPoint(&sX, &sY);
	*pX = sX;
	*pY = sY;
	return false;
}

void CMap::GetMapInitialPoint(short *pX, short *pY, char * pPlayerLocation) {
	int i;
	int iTotalPoint;
	POINT pList[DEF_MAXINITIALPOINT];
	iTotalPoint = 0;
	for (i = 0; i < DEF_MAXINITIALPOINT; i++) {
		if (this->m_pInitialPoint[i].x != -1) {
			pList[iTotalPoint].x = this->m_pInitialPoint[i].x;
			pList[iTotalPoint].y = this->m_pInitialPoint[i].y;
			iTotalPoint++;
		}
	}
	if (iTotalPoint == 0) return;
	if ((pPlayerLocation != nullptr) && (memcmp(pPlayerLocation, "NONE", 4) == 0))
		i = 0;
	else i = iDice(1, iTotalPoint) - 1;
	*pX = pList[i].x;
	*pY = pList[i].y;
}

int CMap::_iGetPlayerNumberOnSpot(short dX, short dY, char cRange) {
	int iSum = 0;
	for (int ix = dX - cRange; ix <= dX + cRange; ix++) {
		for (int iy = dY - cRange; iy <= dY + cRange; iy++) {
			short sOwnerH;
			char cOwnerType;
			this->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
			if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {
				iSum++;
			}
		}
	}
	return iSum;
}

void CMap::SendEventToNearClient_TypeB(uint32_t dwMsgID, uint16_t wMsgType, short sX, short sY, short sV1, short sV2, short sV3, short sV4) {
	char cData[100];
	std::memset(cData, 0, sizeof (cData));
	uint32_t *dwp = (uint32_t *) (cData + DEF_INDEX4_MSGID);
	*dwp = dwMsgID;
	uint16_t *wp = (uint16_t *) (cData + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;
	char *cp = (char *) (cData + DEF_INDEX2_MSGTYPE + 2);
	short *sp = (short *) cp;
	*sp = sX;
	cp += 2;
	sp = (short *) cp;
	*sp = sY;
	cp += 2;
	sp = (short *) cp;
	*sp = sV1;
	cp += 2;
	sp = (short *) cp;
	*sp = sV2;
	cp += 2;
	sp = (short *) cp;
	*sp = sV3;
	cp += 2;
	sp = (short *) cp;
	*sp = sV4;
	cp += 2;
	auto self = shared_from_this();
	for (auto &clientIter : this->m_pClientList) {
		if ((clientIter.map_ == self) &&
				  (clientIter.m_sX >= sX - 10) &&
				  (clientIter.m_sX <= sX + 10) &&
				  (clientIter.m_sY >= sY - 8) &&
				  (clientIter.m_sY <= sY + 8)) {
			clientIter.m_pXSock->iSendMsg(cData, 18);
		}
	}
}

void CMap::CheckFireBluring(int sX, int sY) {
	for (int ix = sX - 1; ix <= sX + 1; ix++) {
		for (int iy = sY - 1; iy <= sY + 1; iy++) {
			int iItemNum = this->iCheckItem(ix, iy);
			switch (iItemNum) {
				case 355:
				{
					short sSpr;
					short sSprFrame;
					char cItemColor;
					CItem *pItem = this->pGetItem(ix, iy, &sSpr, &sSprFrame, &cItemColor);
					if (pItem != nullptr) delete pItem;
					game_.dynamicObjects_.iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_FIRE, shared_from_this(), ix, iy, 6000);
					this->SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM,
							  ix, iy, sSpr, sSprFrame, cItemColor);
				}
					break;
			}
		}
	}
}

char CMap::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cTurn, int * pError) {
	if ((sX == dstX) && (sY == dstY)) return 0;
	int dX = sX;
	int dY = sY;

	int iResX;
	int iResY;
	CMisc m_Misc;
	if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1)) {
		iResX = dstX;
		iResY = dstY;
	} else m_Misc.GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);
	char cDir = m_Misc.cGetNextMoveDir(dX, dY, iResX, iResY);
	if (cTurn == 0) {
		for (int i = cDir; i <= cDir + 7; i++) {
			char cTmpDir = i;
			if (cTmpDir > 8) cTmpDir -= 8;
			int aX = _tmp_cTmpDirX[cTmpDir];
			int aY = _tmp_cTmpDirY[cTmpDir];
			if (this->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
		}
	}
	if (cTurn == 1) {
		for (int i = cDir; i >= cDir - 7; i--) {
			char cTmpDir = i;
			if (cTmpDir < 1) cTmpDir += 8;
			int aX = _tmp_cTmpDirX[cTmpDir];
			int aY = _tmp_cTmpDirY[cTmpDir];
			if (this->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
		}
	}
	return 0;
}

int CMap::_iCalcPlayerNum(short dX, short dY, char cRadius) {
	int iRet = 0;
	for (int ix = dX - cRadius; ix <= dX + cRadius; ix++) {
		if ((ix >= 0) && (ix < this->m_sSizeX)) {
			for (int iy = dY - cRadius; iy <= dY + cRadius; iy++) {
				if ((iy >= 0) && (iy < this->m_sSizeY)) {
					CTile *pTile = (CTile *) (this->m_pTile + ix + iy * this->m_sSizeY);
					if ((pTile->m_sOwner != 0) && (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER)) {
						iRet++;
					}
				}
			}
		}
	}
	return iRet;
}

bool CMap::__bReadMapInfo() {
	char * pContents, * token, * pTile, cTxt[250], cFn[255];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int iTeleportLocIndex = 0;
	int iWayPointCfgIndex = 0;
	int iTotalNpcSetting = 0;
	int iMGARCfgIndex = 0;
	int iSMGRCfgIndex = 0;
	int iNMRCfgIndex = 0;
	int iFishPointIndex = 0;
	int iMineralPointIndex = 0;
	int iStrategicPointIndex = 0;
	int iIndex = 0;
	int iNamingValue;
	class CStrTok * pStrTok;
	HANDLE hFile;
	uint32_t dwFileSize;
	FILE * pFile;
	char cName[6], cNpcName[21], cNpcMoveType, cNpcWaypointIndex[10], cNamePrefix;
	short sIPindex;
	short dX;
	short dY;
	if (memcmp(this->m_cName, "fightzone", 9) == 0)
		this->m_bIsFightZone = true;
	if (memcmp(this->m_cName, "icebound", 8) == 0)
		this->m_bIsSnowEnabled = true;
	std::memset(cFn, 0, sizeof (cFn));
	strcat(cFn, "mapdata\\");
	strcat(cFn, this->m_cName);
	strcat(cFn, ".txt");
	hFile = CreateFile(cFn, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) return false;
	dwFileSize = GetFileSize(hFile, nullptr);
	CloseHandle(hFile);
	pContents = new char[dwFileSize + 1];
	ZeroMemory(pContents, dwFileSize + 1);
	pFile = fopen(cFn, "rt");
	if (pFile == nullptr) {
		wsprintf(cTxt, "(!) Cannot open file : %s", cFn);
		PutLogList(cTxt);
		return false;
	} else {
		wsprintf(cTxt, "(!) Reading Map info file : %s", cFn);
		PutLogList(cTxt);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	//token = strtok(pContents, seps);
	while (token != nullptr) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 1 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pTeleportLoc[iTeleportLocIndex]->m_sSrcX = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 2 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pTeleportLoc[iTeleportLocIndex]->m_sSrcY = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							ZeroMemory(this->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName,
									  sizeof (this->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName));
							strcpy(this->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName, token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 3 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pTeleportLoc[iTeleportLocIndex]->m_sDestX = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 4 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pTeleportLoc[iTeleportLocIndex]->m_sDestY = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 5 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pTeleportLoc[iTeleportLocIndex]->m_cDir = atoi(token);
							iTeleportLocIndex++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 2:
					switch (cReadModeB) {
						case 1:
							// waypoint ¹øÈ£
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 6 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iWayPointCfgIndex = atoi(token);
							if (this->m_WaypointList[iWayPointCfgIndex].x != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 7 - Duplicated waypoint");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 8 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_WaypointList[iWayPointCfgIndex].x = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 9 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_WaypointList[iWayPointCfgIndex].y = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 3:
					switch (cReadModeB) {
						case 1:
							std::memset(cNpcName, 0, sizeof (cNpcName));
							strcpy(cNpcName, token);
							cReadModeB = 2;
							break;
						case 2:
							// NpcMoveType
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 10 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cNpcMoveType = atoi(token);
							cReadModeB = 3;
							break;
						default:
							// WayPoint0~waypoint9
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 11 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cNpcWaypointIndex[cReadModeB - 3] = atoi(token);
							cReadModeB++;
							break;
						case 13:
							// cNamePrefix
							cNamePrefix = token[0];
							iNamingValue = this->iGetEmptyNamingValue();
							if (iNamingValue == -1) {
							} else {
								std::memset(cName, 0, sizeof (cName));
								wsprintf(cName, "XX%d", iNamingValue);
								cName[0] = cNamePrefix;
								cName[1] = id_ + 65;
								if (this->bCreateNewNpc(cNpcName, cName, 0, 0, cNpcMoveType, nullptr, nullptr, cNpcWaypointIndex, nullptr, 0, -1, false) == false) {
									this->SetNamingValueEmpty(iNamingValue);
								}
							}
							cReadModeA = 0;
							cReadModeB = 0;
							iTotalNpcSetting++;
							break;
					}
					break;
				case 4:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 12 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_bRandomMobGenerator = (bool) atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							// Mob- Level
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 13 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_cRandomMobGeneratorLevel = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 5:
					// Maximum object
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 14 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iMaximumObject = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 6:
					switch (cReadModeB) {
						case 1:
							// Rect ¹øÈ£
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 15 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iMGARCfgIndex = atoi(token);
							if (this->m_rcMobGenAvoidRect[iMGARCfgIndex].left != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 16 - Duplicated Mob Gen Rect Number!");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							// left
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 17 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcMobGenAvoidRect[iMGARCfgIndex].left = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							// top
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 18 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcMobGenAvoidRect[iMGARCfgIndex].top = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// right
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 19 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcMobGenAvoidRect[iMGARCfgIndex].right = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// bottom
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 20 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcMobGenAvoidRect[iMGARCfgIndex].bottom = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 7:
					switch (cReadModeB) {
						case 1:
							// Rect ¹øÈ£ m_stSpotMobGenerator[]
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 21 - Wrong Data format(MGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iSMGRCfgIndex = atoi(token);
							if (this->m_stSpotMobGenerator[iSMGRCfgIndex].bDefined == true) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error - ");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							this->m_stSpotMobGenerator[iSMGRCfgIndex].bDefined = true;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 22 - Wrong Data format(SMGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].cType = atoi(token);
							if (this->m_stSpotMobGenerator[iSMGRCfgIndex].cType == 1)
								cReadModeB = 3;
							else if (this->m_stSpotMobGenerator[iSMGRCfgIndex].cType == 2)
								cReadModeB = 9;
							break;
						case 3:
							// left
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 23 - Wrong Data format(SMGAR num).");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.left = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							// top
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 24 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.top = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							// right
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 25 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.right = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							// bottom
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 26 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.bottom = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							// spot mob type
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 27 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].iMobType = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							// Max Mobs
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 28 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].iMaxMobs = atoi(token);
							this->m_stSpotMobGenerator[iSMGRCfgIndex].iCurMobs = 0;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
						default:
							// WayPoint0~waypoint9
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 29 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].cWaypoint[cReadModeB - 9] = atoi(token);
							cReadModeB++;
							break;
						case 19:
							// spot mob type
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 30 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].iMobType = atoi(token);
							cReadModeB = 20;
							break;
						case 20:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 31 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stSpotMobGenerator[iSMGRCfgIndex].iMaxMobs = atoi(token);
							this->m_stSpotMobGenerator[iSMGRCfgIndex].iCurMobs = 0;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 8:
					std::memset(this->m_cLocationName, 0, sizeof (this->m_cLocationName));
					memcpy(this->m_cLocationName, token, 10);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 9:
					switch (cReadModeB) {
						case 1:
							// Initial-Point Index
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 32:1 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							sIPindex = atoi(token);
							if (this->m_pInitialPoint[sIPindex].x != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Duplicate Initial Point Index!");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							// Initial-Point X
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 32 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pInitialPoint[sIPindex].x = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							// Initial-Point Y
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 33 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pInitialPoint[sIPindex].y = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 10:
					switch (cReadModeB) {
						case 1:
							//
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 34 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iNMRCfgIndex = atoi(token);
							if (this->m_rcNoAttackRect[iNMRCfgIndex].top != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 35 - Duplicate No-Magic-Rect number");
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 36 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcNoAttackRect[iNMRCfgIndex].left = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 37 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcNoAttackRect[iNMRCfgIndex].top = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 38 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcNoAttackRect[iNMRCfgIndex].right = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 39 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_rcNoAttackRect[iNMRCfgIndex].bottom = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 11:
					this->m_bIsFixedDayMode = (bool) atoi(token);
					if (this->m_bIsFixedDayMode == true)
						this->m_bIsSnowEnabled = false;
					cReadModeA = 0;
					break;
				case 12:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 40 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iFishPointIndex = atoi(token);
							if (this->m_FishPointList[iFishPointIndex].x != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 41 - Duplicate FishPoint number");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iTotalFishPoint++;
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 42 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_FishPointList[iFishPointIndex].x = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 43 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_FishPointList[iFishPointIndex].y = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 13:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 44 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iMaxFish = atoi(token);
					cReadModeA = 0;
					break;
				case 14:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 45 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cType = atoi(token);
					cReadModeA = 0;
					break;
				case 15:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 46 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iLevelLimit = atoi(token);
					cReadModeA = 0;
					break;
				case 16:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 47 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_bMineralGenerator = (bool) atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 48 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_cMineralGeneratorLevel = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 17:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 49 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iMineralPointIndex = atoi(token);
							if (this->m_MineralPointList[iMineralPointIndex].x != -1) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 50 - Duplicate MineralPoint number");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iTotalMineralPoint++;
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 51 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_MineralPointList[iMineralPointIndex].x = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 52 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_MineralPointList[iMineralPointIndex].y = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 18:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 53 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iMaxMineral = atoi(token);
					cReadModeA = 0;
					break;
				case 19:
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error 54 - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iUpperLevelLimit = atoi(token);
					cReadModeA = 0;
					break;
				case 20:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 55 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iStrategicPointIndex = atoi(token);
							if (this->m_pStrategicPointList[iStrategicPointIndex] != nullptr) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 56 - Duplicate Strategic Point number");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pStrategicPointList[iStrategicPointIndex] = new class CStrategicPoint;
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 57 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pStrategicPointList[iStrategicPointIndex]->m_iSide = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 58 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pStrategicPointList[iStrategicPointIndex]->m_iValue = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 59 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pStrategicPointList[iStrategicPointIndex]->m_iX = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 60 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_pStrategicPointList[iStrategicPointIndex]->m_iY = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 21:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 61 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);
							if (this->m_stEnergySphereCreationList[iIndex].cType != 0) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 62 - Duplicate EnergySphereCreation number");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iTotalEnergySphereCreationPoint++;
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 63 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereCreationList[iIndex].cType = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 64 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereCreationList[iIndex].sX = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 65 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereCreationList[iIndex].sY = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 22:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 66 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);
							if (this->m_stEnergySphereGoalList[iIndex].cResult != 0) {
								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 67 - Duplicate EnergySphereGoal number(%d:%d)", iIndex, this->m_stEnergySphereGoalList[iIndex].cResult);
								PutLogList(G_cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iTotalEnergySphereGoalPoint++;
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 68 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereGoalList[iIndex].cResult = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 69 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereGoalList[iIndex].aresdenX = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 70 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereGoalList[iIndex].aresdenY = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 71 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereGoalList[iIndex].elvineX = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 72 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stEnergySphereGoalList[iIndex].elvineY = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 23:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 73 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);
							if (strlen(this->m_stStrikePoint[iIndex].cRelatedMapName) != 0) {
								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 74 - Duplicate Strike Point number(%d)", iIndex);
								PutLogList(G_cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 75 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].dX = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 76 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].dY = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iHP = atoi(token);
							this->m_stStrikePoint[iIndex].iInitHP = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectX[0] = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectY[0] = atoi(token);
							cReadModeB = 7;
							break;
						case 7:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectX[1] = atoi(token);
							cReadModeB = 8;
							break;
						case 8:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectY[1] = atoi(token);
							cReadModeB = 9;
							break;
						case 9:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectX[2] = atoi(token);
							cReadModeB = 10;
							break;
						case 10:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectY[2] = atoi(token);
							cReadModeB = 11;
							break;
						case 11:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectX[3] = atoi(token);
							cReadModeB = 12;
							break;
						case 12:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectY[3] = atoi(token);
							cReadModeB = 13;
							break;
						case 13:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectX[4] = atoi(token);
							cReadModeB = 14;
							break;
						case 14:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stStrikePoint[iIndex].iEffectY[4] = atoi(token);
							cReadModeB = 15;
							break;
						case 15:
							std::memset(this->m_stStrikePoint[iIndex].cRelatedMapName, 0, sizeof (this->m_stStrikePoint[iIndex].cRelatedMapName));
							strcpy(this->m_stStrikePoint[iIndex].cRelatedMapName, token);
							this->m_iTotalStrikePoints++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break; // end 23
				case 24:
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex = atoi(token);
							if (strlen(this->m_stItemEventList[iIndex].cItemName) != 0) {

								wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 79 - Duplicate Item-Event number(%d:%s)", iIndex, this->m_stItemEventList[iIndex].cItemName);
								PutLogList(G_cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iTotalItemEvents++;
							cReadModeB = 2;
							break;
						case 2:
							strcpy(this->m_stItemEventList[iIndex].cItemName, token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 81 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stItemEventList[iIndex].iAmount = atoi(token);
							cReadModeB = 4;
							break;
						case 4:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 82 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stItemEventList[iIndex].iTotalNum = atoi(token);
							cReadModeB = 5;
							break;
						case 5:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stItemEventList[iIndex].iMonth = atoi(token);
							cReadModeB = 6;
							break;
						case 6:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stItemEventList[iIndex].iDay = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 25: //mobevent-amount
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->sMobEventAmount = atoi(token);
							cReadModeB = 0;
							break;
					}
					break;
				case 26: //ApocalypseMobGenType
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMobGenType - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_iApocalypseMobGenType = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 27: // ApocalypseBossMob
					switch (cReadModeB) {
						case 1: // 3CB6Ch m_pMapList[]->m_ApocalypseBossMobNpcID
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_iApocalypseBossMobNpcID = atoi(token);
							cReadModeB = 2;
							break;
						case 2: // 3CB70h m_pMapList[]->ApocalypseBossMobRectX1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sApocalypseBossMobRectX1 = atoi(token);
							cReadModeB = 3;
							break;
						case 3: // 3CB74h m_pMapList[]->ApocalypseBossMobRectY1
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sApocalypseBossMobRectY1 = atoi(token);
							cReadModeB = 4;
							break;
						case 4: // 3CB78h m_pMapList[]->ApocalypseBossMobRectX2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sApocalypseBossMobRectX2 = atoi(token);
							cReadModeB = 5;
							break;
						case 5: // 3CB7Ch m_pMapList[]->ApocalypseBossMobRectY2
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sApocalypseBossMobRectY2 = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 28: //DynamicGateType // 28
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error DynamicGateType - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cDynamicGateType = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 29: //DynamicGateCoord // 29
					// DynamicGateCoord	= 59 196 60 197        abaddon	   -1  -1
					switch (cReadModeB) {
						case 1: // 3CA20h
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sDynamicGateCoordRectX1 = atoi(token);
							cReadModeB = 2;
							break;
						case 2: // 3CA24h
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sDynamicGateCoordRectY1 = atoi(token);
							cReadModeB = 3;
							break;
						case 3: // 3CA28h
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sDynamicGateCoordRectX2 = atoi(token);
							cReadModeB = 4;
							break;
						case 4: // 3CA2Ch
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sDynamicGateCoordRectY2 = atoi(token);
							cReadModeB = 5;
							break;
						case 5: // 3CA30h
							memcpy(this->m_cDynamicGateCoordDestMap, token, strlen(token));
							cReadModeB = 6;
							break;
						case 6: // 3CA3Ch
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sDynamicGateCoordTgtX = atoi(token);
							cReadModeB = 7;
							break;
						case 7: // (ty = 3CB60h) unknown (3CA3Eh)
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_sDynamicGateCoordTgtY = atoi(token);
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 30: // RecallImpossible // 30
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error RecallImpossible -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_bIsRecallImpossible = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 31: // ApocalypseMap // 31
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMap -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_bIsApocalypseMap = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 32: // CitizenLimit // 32
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error CitizenLimit -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_bIsCitizenLimit = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 33: // HeldenianMap
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error CitizenLimit -  Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_bIsHeldenianMap = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 34: // HeldenianTower
					switch (cReadModeB) {
						case 1: // NpcID
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian tower type id - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stHeldenianTower[iIndex].sTypeID = atoi(token);
							cReadModeB = 2;
							break;
						case 2: // side
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Side - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stHeldenianTower[iIndex].cSide = atoi(token);
							cReadModeB = 3;
							break;
						case 3: // sX
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower X pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stHeldenianTower[iIndex].dX = atoi(token);
							cReadModeB = 4;
							break;
						case 4: // sY
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Y pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							iIndex++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 35: // HeldenianModeMap
					if (_bGetIsStringIsNumber(token) == false) {
						PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
						delete pContents;
						delete pStrTok;
						return false;
					}
					this->m_cHeldenianModeMap = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				case 36: // HeldenianWinningZone
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							dX = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							dY = atoi(token);
							pTile = 0;
							pTile = (char *) (this->m_pTile + dX + dY * this->m_sSizeY);
							if (pTile == 0) {
								wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map Info file error HeldenianWinningZone - pTile is Null dx(%d), dy(%d).", dX, dY);
								PutLogList(cTxt);
								delete pContents;
								delete pStrTok;
								return false;
							}
							//pTile->m_iAttribute = 1;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				case 37: // HeldenianGateDoor // 37
					switch (cReadModeB) {
						case 1:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Direction - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stHeldenianGateDoor[iIndex].cDir = atoi(token);
							cReadModeB = 2;
							break;
						case 2:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door X pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stHeldenianGateDoor[iIndex].dX = atoi(token);
							cReadModeB = 3;
							break;
						case 3:
							if (_bGetIsStringIsNumber(token) == false) {
								PutLogList("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Y pos - Wrong Data format.");
								delete pContents;
								delete pStrTok;
								return false;
							}
							this->m_stHeldenianGateDoor[iIndex].dY = atoi(token);
							iIndex++;
							cReadModeA = 0;
							cReadModeB = 0;
							break;
					}
					break;
				default:
					break;
			}
		} else {
			if (memcmp(token, "teleport-loc", 12) == 0) {
				this->m_pTeleportLoc[iTeleportLocIndex] = new class CTeleportLoc;
				cReadModeA = 1;
				cReadModeB = 1;
			}
			if (memcmp(token, "waypoint", 8) == 0) {
				cReadModeA = 2;
				cReadModeB = 1;
			}
			if (memcmp(token, "npc", 3) == 0) {
				cReadModeA = 3;
				cReadModeB = 1;
			}
			if (memcmp(token, "random-mob-generator", 20) == 0) {
				cReadModeA = 4;
				cReadModeB = 1;
			}
			if (memcmp(token, "maximum-object", 14) == 0)
				cReadModeA = 5;
			if (memcmp(token, "npc-avoidrect", 13) == 0) {
				cReadModeA = 6;
				cReadModeB = 1;
			}
			if (memcmp(token, "spot-mob-generator", 18) == 0) {
				cReadModeA = 7;
				cReadModeB = 1;
			}
			if (memcmp(token, "map-location", 12) == 0)
				cReadModeA = 8;
			if (memcmp(token, "initial-point", 13) == 0) {
				cReadModeA = 9;
				cReadModeB = 1;
			}
			if (memcmp(token, "no-attack-area", 14) == 0) {
				cReadModeA = 10;
				cReadModeB = 1;
			}
			if (memcmp(token, "fixed-dayornight-mode", 21) == 0) cReadModeA = 11;
			if (memcmp(token, "fish-point", 10) == 0) {
				cReadModeA = 12;
				cReadModeB = 1;
			}
			if (memcmp(token, "max-fish", 8) == 0) cReadModeA = 13;
			if (memcmp(token, "type", 4) == 0) cReadModeA = 14;
			if (memcmp(token, "level-limit", 11) == 0) cReadModeA = 15;
			if (memcmp(token, "mineral-generator", 17) == 0) {
				cReadModeA = 16;
				cReadModeB = 1;
			}
			if (memcmp(token, "mineral-point", 13) == 0) {
				cReadModeA = 17;
				cReadModeB = 1;
			}
			if (memcmp(token, "max-mineral", 11) == 0) cReadModeA = 18;
			if (memcmp(token, "upper-level-limit", 17) == 0) cReadModeA = 19; // v1.4
			if (memcmp(token, "strategic-point", 15) == 0) { // v1.41
				cReadModeA = 20;
				cReadModeB = 1;
			}
			if (memcmp(token, "energy-sphere-creation-point", 28) == 0) {
				cReadModeA = 21;
				cReadModeB = 1;
			}
			if (memcmp(token, "energy-sphere-goal-point", 24) == 0) {
				cReadModeA = 22;
				cReadModeB = 1;
			}
			if (memcmp(token, "strike-point", 12) == 0) {
				cReadModeA = 23;
				cReadModeB = 1;
			}
			if (memcmp(token, "item-event", 10) == 0) {
				cReadModeA = 24;
				cReadModeB = 1;
			}
			if (memcmp(token, "energy-sphere-auto-creation", 27) == 0) {
				cReadModeA = 0;
				cReadModeB = 0;
				this->m_bIsEnergySphereAutoCreation = true;
			}
			if (memcmp(token, "mobevent-amount", 15) == 0) {
				cReadModeA = 25;
				cReadModeB = 1;
			}
			if (memcmp(token, "ApocalypseMobGenType", 20) == 0) {
				cReadModeA = 26;
				cReadModeB = 1;
			}
			if (memcmp(token, "ApocalypseBossMob", 17) == 0) {
				cReadModeA = 27;
				cReadModeB = 1;
			}
			if (memcmp(token, "DynamicGateType", 15) == 0) {
				cReadModeA = 28;
				cReadModeB = 1;
			}
			if (memcmp(token, "DynamicGateCoord", 16) == 0) {
				cReadModeA = 29;
				cReadModeB = 1;
			}
			if (memcmp(token, "RecallImpossible", 16) == 0) {
				cReadModeA = 30;
				cReadModeB = 1;
			}
			if (memcmp(token, "ApocalypseMap", 13) == 0) {
				cReadModeA = 31;
				cReadModeB = 1;
			}
			if (memcmp(token, "CitizenLimit", 12) == 0) {
				cReadModeA = 32;
				cReadModeB = 1;
			}
			if (memcmp(token, "HeldenianMap", 12) == 0) {
				cReadModeA = 33;
				cReadModeB = 1;
			}
			if (memcmp(token, "HeldenianTower", 14) == 0) {
				cReadModeA = 34;
				cReadModeB = 1;
			}
			if (memcmp(token, "HeldenianModeMap", 16) == 0) {
				cReadModeA = 35;
				cReadModeB = 1;
			}
			if (memcmp(token, "HeldenianWinningZone", 20) == 0) {
				cReadModeA = 36;
				cReadModeB = 1;
			}
			if (memcmp(token, "HeldenianGateDoor", 17) == 0) {
				cReadModeA = 37;
				cReadModeB = 1;
			}
			if (memcmp(token, "[END-MAP-INFO]", 14) == 0) {
				cReadModeA = 0;
				cReadModeB = 0;
				goto RMI_SKIPDECODING;
			}
		}
		token = pStrTok->pGet();
	}
RMI_SKIPDECODING:
	;
	delete pStrTok;
	delete pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! map info file contents error!");
		return false;
	}
	wsprintf(cTxt, "(!) Map info file decoding(%s) - success! TL(%d) WP(%d) LNPC(%d) MXO(%d) RMG(%d / %d)", cFn, iTeleportLocIndex, iWayPointCfgIndex, iTotalNpcSetting, this->m_iMaximumObject, this->m_bRandomMobGenerator, this->m_cRandomMobGeneratorLevel);
	PutLogList(cTxt);
	this->_SetupNoAttackArea();
	//Heldenian 3.00 Source
	strncpy(game_.m_cHeldenianMapName, this->m_cName, 11);
	game_.m_bIsHeldenianReady = true;
	return true;
}

void CMap::RemoveOccupyFlags() {
	uint32_t dwTime = timeGetTime();
	register int i;
	short dX;
	short dY;
	int iDynamicObjectIndex;
	class CTile * pTile;
	int iy;
	int ix;
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (this->m_pOccupyFlag[i] != false) return;
	if (this->m_pOccupyFlag[i] != false) {
		dX = this->m_pOccupyFlag[i]->m_sX;
		dY = this->m_pOccupyFlag[i]->m_sY;
		pTile = (class CTile *)(this->m_pTile + dX + dY * this->m_sSizeY);
		this->m_iTotalOccupyFlags--;
		iDynamicObjectIndex = this->m_pOccupyFlag[i]->m_iDynamicObjectIndex;
		auto &dynObj = game_.dynamicObjects_[iDynamicObjectIndex];
		if (dynObj == 0) return;
		dynObj->map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT,
				  dynObj->m_sX, dynObj->m_sY,
				  dynObj->m_sType, iDynamicObjectIndex, 0);
		dynObj->map_->SetDynamicObject(0, 0, dynObj->m_sX, dynObj->m_sY, dwTime);
		if (dynObj == 0) {
			for (ix = dX - 2; ix <= dX + 2; ix++)
				for (iy = dY - 2; iy <= dY + 2; iy++) {
					pTile = (class CTile *)(this->m_pTile + ix + iy * this->m_sSizeY);
					pTile->m_sOwner = 0;
				}
		}
	}
}

void CMap::MeteorStrikeHandler() {
	int i;
	int ix;
	int iy;
	int dX;
	int dY;
	int iIndex;
	int iTargetIndex;
	int iTotalESG;
	int iEffect;
	int iTargetArray[DEF_MAXSTRIKEPOINTS];
	short sOwnerH;
	char cOwnerType;
	uint32_t dwTime = timeGetTime();
	PutLogList("(!) Beginning Meteor Strike Procedure...");
	if (this->m_iTotalStrikePoints == 0) {
		PutLogList("(X) MeteorStrikeHandler Error! No Strike Points!");
		return;
	}
	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) iTargetArray[i] = -1;
	iIndex = 0;
	for (i = 1; i <= this->m_iTotalStrikePoints; i++) {
		if (this->m_stStrikePoint[i].iHP > 0) {
			iTargetArray[iIndex] = i;
			iIndex++;
		}
	}
	wsprintf(G_cTxt, "(!) Map(%s) has %d available strike points", this->m_cName, iIndex);
	PutLogList(G_cTxt);
	game_.m_stMeteorStrikeResult.iCasualties = 0;
	game_.m_stMeteorStrikeResult.iCrashedStructureNum = 0;
	game_.m_stMeteorStrikeResult.iStructureDamageAmount = 0;
	if (iIndex == 0) {
		PutLogList("(!) No strike points!");
		game_.delayEvents_.add(DelayEventType::CALCMETEORSTRIKEEFFECT, 0, dwTime + 6000, 0, 0, shared_from_this(), 0, 0, 0, 0, 0);
	} else {
		auto self = shared_from_this();
		for (auto &clientIter : m_pClientList) {
			if ((clientIter.m_bIsInitComplete == true) && (clientIter.map_ == self)) {
				clientIter.SendNotifyMsg(0, DEF_NOTIFY_METEORSTRIKEHIT, 0, 0, 0, nullptr);
			}
		}
		for (i = 0; i < iIndex; i++) {
			iTargetIndex = iTargetArray[i];
			if (iTargetIndex == -1) {
				PutLogList("(X) Strike Point MapIndex: -1!");
				goto MSH_SKIP_STRIKE;
			}
			dX = this->m_stStrikePoint[iTargetIndex].dX;
			dY = this->m_stStrikePoint[iTargetIndex].dY;
			iTotalESG = 0;
			for (ix = dX - 10; ix <= dX + 10; ix++)
				for (iy = dY - 10; iy <= dY + 10; iy++) {
					this->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_NPC) && (game_.m_pNpcList[sOwnerH] != nullptr) && (game_.m_pNpcList[sOwnerH]->m_sType == 40)) {
						iTotalESG++;
					}
				}
			wsprintf(G_cTxt, "(!) Meteor Strike Target(%d, %d) ESG(%d)", dX, dY, iTotalESG);
			PutLogList(G_cTxt);
			if (iTotalESG < 2) {
				this->m_stStrikePoint[iTargetIndex].iHP -= (2 - iTotalESG);
				if (this->m_stStrikePoint[iTargetIndex].iHP <= 0) {
					this->m_stStrikePoint[iTargetIndex].iHP = 0;
					game_.m_pMapList[this->m_stStrikePoint[iTargetIndex].iMapIndex]->m_bIsDisabled = true;
					game_.m_stMeteorStrikeResult.iCrashedStructureNum++;
				} else {
					game_.m_stMeteorStrikeResult.iStructureDamageAmount += (2 - iTotalESG);
					iEffect = iDice(1, 5) - 1;
					game_.dynamicObjects_.iAddDynamicObjectList(0, DEF_OWNERTYPE_PLAYER_INDIRECT, DEF_DYNAMICOBJECT_FIRE2, self, this->m_stStrikePoint[iTargetIndex].iEffectX[iEffect] +(iDice(1, 3) - 2), this->m_stStrikePoint[iTargetIndex].iEffectY[iEffect] +(iDice(1, 3) - 2), 60 * 1000 * 50);
				}
			}
MSH_SKIP_STRIKE:
			;
		}
		game_.delayEvents_.add(DelayEventType::DOMETEORSTRIKEDAMAGE, 0, dwTime + 1000, 0, 0, self, 0, 0, 0, 0, 0);
		game_.delayEvents_.add(DelayEventType::DOMETEORSTRIKEDAMAGE, 0, dwTime + 4000, 0, 0, self, 0, 0, 0, 0, 0);
		game_.delayEvents_.add(DelayEventType::CALCMETEORSTRIKEEFFECT, 0, dwTime + 6000, 0, 0, self, 0, 0, 0, 0, 0);
	}
}

void CMap::CalcMeteorStrikeEffectHandler() {
	int i;
	int iActiveStructure;
	char cTempData[120];
	if (game_.m_bIsCrusadeMode == false) return;
	iActiveStructure = 0;
	for (i = 1; i <= this->m_iTotalStrikePoints; i++) {
		if (this->m_stStrikePoint[i].iHP > 0) {
			iActiveStructure++;
		}
	}
	wsprintf(G_cTxt, "ActiveStructure:%d  MapIndex:%d AresdenMap:%d ElvineMap:%d", iActiveStructure, id_, game_.m_iAresdenMapIndex, game_.m_iElvineMapIndex);
	PutLogList(G_cTxt);
	if (iActiveStructure == 0) {
		if (id_ == game_.m_iAresdenMapIndex) {
			game_.LocalEndCrusadeMode(2);
		} else if (id_ == game_.m_iElvineMapIndex) {
			game_.LocalEndCrusadeMode(1);
		} else {
			game_.LocalEndCrusadeMode(0);
		}
	} else {
		game_.GrandMagicResultHandler(this->m_cName, game_.m_stMeteorStrikeResult.iCrashedStructureNum, game_.m_stMeteorStrikeResult.iStructureDamageAmount, game_.m_stMeteorStrikeResult.iCasualties, iActiveStructure, this->m_iTotalStrikePoints, cTempData);
	}
	game_.m_stMeteorStrikeResult.iCasualties = 0;
	game_.m_stMeteorStrikeResult.iCrashedStructureNum = 0;
	game_.m_stMeteorStrikeResult.iStructureDamageAmount = 0;
}

void CMap::DoMeteorStrikeDamageHandler() {
	int iDamage;
	auto self = shared_from_this();
	for(auto &clientIter: m_pClientList) {
		if ((clientIter.m_cSide != 0) && (clientIter.map_ == self)) {
			if (clientIter.m_iLevel < 80)
				iDamage = clientIter.m_iLevel + iDice(1, 10);
			else iDamage = clientIter.m_iLevel * 2 + iDice(1, 10);
			iDamage = iDice(1, clientIter.m_iLevel) + clientIter.m_iLevel;
			if (iDamage > 255) iDamage = 255;
			if (clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2) {
				iDamage = (iDamage / 2) - 2;
			}
			if (clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) {
				iDamage = 0;
			}
			if (clientIter.m_iAdminUserLevel > 0) {
				iDamage = 0;
			}
			clientIter.m_iHP -= iDamage;
			if (clientIter.m_iHP <= 0) {
				clientIter.ClientKilledHandler(0, 0, iDamage);
				game_.m_stMeteorStrikeResult.iCasualties++;
			} else {
				if (iDamage > 0) {
					clientIter.SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
					game_.SendEventToNearClient_TypeA(clientIter.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
					if (clientIter.m_bSkillUsingStatus[19] != true) {
						clientIter.map_->ClearOwner(0, clientIter.id_, DEF_OWNERTYPE_PLAYER, clientIter.m_sX, clientIter.m_sY);
						clientIter.map_->SetOwner(clientIter.id_, DEF_OWNERTYPE_PLAYER, clientIter.m_sX, clientIter.m_sY);
					}
					if (clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
						clientIter.SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, nullptr);
						clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
						game_.delayEvents_.remove(clientIter.id_, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
					}
				}
			}
		}
	}
}

void CMap::_DeleteRandomOccupyFlag() {
	int i;
	int iCount;
	int iTotalFlags;
	int iDynamicObjectIndex;
	int tx;
	int ty;
	int fx;
	int fy;
	int iLocalSide;
	int iLocalEKNum;
	class CTile * pTile;
	uint32_t dwTime;
	dwTime = timeGetTime();
	iTotalFlags = 0;
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (this->m_pOccupyFlag[i] != nullptr) {
			iTotalFlags++;
		}
	iCount = 0;
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++) {
		if (this->m_pOccupyFlag[i] != nullptr) {
			iCount++;
			if ((iCount == iTotalFlags) && (this->m_pOccupyFlag[i] != nullptr)) {
				//testcode
				wsprintf(G_cTxt, "(*)Delete OccupyFlag: Side(%d) XY(%d, %d)", this->m_pOccupyFlag[i]->m_cSide, this->m_pOccupyFlag[i]->m_sX, this->m_pOccupyFlag[i]->m_sY);
				PutLogList(G_cTxt);
				fx = this->m_pOccupyFlag[i]->m_sX;
				fy = this->m_pOccupyFlag[i]->m_sY;
				iLocalSide = this->m_pOccupyFlag[i]->m_cSide;
				iLocalEKNum = this->m_pOccupyFlag[i]->m_iEKCount;
				pTile = (class CTile *)(this->m_pTile + this->m_pOccupyFlag[i]->m_sX +
						  this->m_pOccupyFlag[i]->m_sY * this->m_sSizeY);
				this->m_iTotalOccupyFlags--;
				iDynamicObjectIndex = this->m_pOccupyFlag[i]->m_iDynamicObjectIndex;
				auto &dynObj = game_.dynamicObjects_[iDynamicObjectIndex];
				dynObj->map_->SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT,
						  dynObj->m_sX, dynObj->m_sY,
						  dynObj->m_sType, iDynamicObjectIndex, 0);
				dynObj->map_->SetDynamicObject(0, 0, dynObj->m_sX, dynObj->m_sY, dwTime);
				delete this->m_pOccupyFlag[i];
				this->m_pOccupyFlag[i] = nullptr;
				pTile->m_iOccupyFlagIndex = 0;
				dynObj.reset();
				for (tx = fx - 10; tx <= fx + 10; tx++)
					for (ty = fy - 8; ty <= fy + 8; ty++) {
						if ((tx < 0) || (tx >= this->m_sSizeX) ||
								  (ty < 0) || (ty >= this->m_sSizeY)) {
						} else {
							pTile = (class CTile *)(this->m_pTile + tx + ty * this->m_sSizeY);
							switch (iLocalSide) {
								case 1:
									pTile->m_iOccupyStatus += iLocalEKNum;
									if (pTile->m_iOccupyStatus > 0) pTile->m_iOccupyStatus = 0;
									break;
								case 2:
									pTile->m_iOccupyStatus -= iLocalEKNum;
									if (pTile->m_iOccupyStatus < 0) pTile->m_iOccupyStatus = 0;
									break;
							}
						}
					}
				//
				return;
			}
		}
	}
}

bool CMap::_bReadMapInfoFiles() {
	return this->__bReadMapInfo();
}

bool CMap::__bSetOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iClientH, bool bAdminFlag) {
	register int ix, iy;
	int iDynamicObjectIndex;
	int iIndex;
	class CTile * pTile;
	if (((game_.m_bIsHeldenianMode == false) || (game_.m_bIsHeldenianMode != game_.m_cHeldenianModeType)) && (game_.m_bHeldenianInitiated == 1)) return false;
	if ((game_.m_cHeldenianModeType == 1) && (game_.m_iBTFieldMapIndex == -1)) return false;
	if ((game_.m_cHeldenianModeType == 2) && (game_.m_iGodHMapIndex == -1)) return false;
	if ((m_pClientList[iClientH]->m_iGuildRank == 0)) return false;
	pTile = (class CTile *)(this->m_pTile + dX + dY * this->m_sSizeY);
	if (pTile->m_iAttribute != 0) return false;
	iSide = game_.m_sLastHeldenianWinner;
	if ((dX < 25) || (dX >= this->m_sSizeX - 25) ||
			  (dY < 25) || (dY >= this->m_sSizeY - 25)) return false;
	if ((iClientH > 0) && (m_pClientList[iClientH] != nullptr)) {
		if ((bAdminFlag == false) && (m_pClientList[iClientH]->m_cSide != iSide)) return false;
	}
	pTile = (class CTile *)(this->m_pTile + dX + dY * this->m_sSizeY);
	if (pTile->m_iOccupyFlagIndex != 0) return false;
	if (pTile->m_bIsMoveAllowed == false) return false;
	for (ix = dX - 3; ix <= dX + 3; ix++)
		for (iy = dY - 3; iy <= dY + 3; iy++) {
			if ((ix == dX) && (iy == dY)) {
			} else {
				pTile = (class CTile *)(this->m_pTile + ix + iy * this->m_sSizeY);
				if ((pTile->m_iOccupyFlagIndex != 0) && (pTile->m_iOccupyFlagIndex > 0) &&
						  (pTile->m_iOccupyFlagIndex < DEF_MAXOCCUPYFLAG) && (this->m_pOccupyFlag[pTile->m_iOccupyFlagIndex] != nullptr)) {
					if (this->m_pOccupyFlag[pTile->m_iOccupyFlagIndex]->m_cSide == iSide) return false;
				}
			}
		}
	if (this->m_iTotalOccupyFlags >= DEF_MAXOCCUPYFLAG) {
		return false;
	}
	switch (iSide) {
		case 1: iDynamicObjectIndex = game_.dynamicObjects_.iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_ARESDENFLAG1, shared_from_this(), dX, dY, 0, 0);
			break;
		case 2: iDynamicObjectIndex = game_.dynamicObjects_.iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_ELVINEFLAG1, shared_from_this(), dX, dY, 0, 0);
			break;
		default: iDynamicObjectIndex = 0;
	}
	iEKNum = 1;
	iIndex = this->iRegisterOccupyFlag(dX, dY, iSide, iEKNum, iDynamicObjectIndex);
	if (iIndex < 0) {
		if (iDynamicObjectIndex > DEF_MAXGUILDS)
			return true;
	}
	pTile = (class CTile *)(this->m_pTile + dX + dY * this->m_sSizeY);
	pTile->m_iOccupyFlagIndex = iIndex;
	this->m_iTotalOccupyFlags++;
	if (game_.m_cHeldenianModeType == 1) {
		for (ix = dX - 3; ix <= dX + 3; ix++)
			for (iy = dY - 3; iy <= dY + 3; iy++) {
				if ((ix < 0) || (ix >= this->m_sSizeX) ||
						  (iy < 0) || (iy >= this->m_sSizeY)) {
				} else {
					pTile = (class CTile *)(this->m_pTile + ix + iy * this->m_sSizeY);
					switch (iSide) {
						case 1:
							pTile->m_iOccupyStatus -= iEKNum;
							break;
						case 2:
							pTile->m_iOccupyStatus += iEKNum;
							break;
					}
				}
			}
	}
	if (game_.m_cHeldenianModeType == 2) {
		if (iSide == game_.m_sLastHeldenianWinner) {
			game_.m_cHeldenianVictoryType = iSide;
			game_.GlobalEndHeldenianMode();
		}
	}
	return true;
}

void CMap::MineralGenerator() {
	if ((iDice(1, 4) == 1) && (this->m_bMineralGenerator == true) &&
			  (this->m_iCurMineral < this->m_iMaxMineral)) {
		int iP = iDice(1, this->m_iTotalMineralPoint) - 1;
		if ((this->m_MineralPointList[iP].x == -1) || (this->m_MineralPointList[iP].y == -1)) return;
		int tX = this->m_MineralPointList[iP].x;
		int tY = this->m_MineralPointList[iP].y;
		iCreateMineral(tX, tY, this->m_cMineralGeneratorLevel);
	}
}

int CMap::iCreateMineral(int tX, int tY, char cLevel) {
	register int i, iDynamicHandle, iMineralType;
	auto self = shared_from_this();
	for (i = 1; i < DEF_MAXMINERALS; i++) {
		auto &mineral = game_.m_pMineral[i];
		if (mineral == nullptr) {
			iMineralType = iDice(1, cLevel);
			mineral = new class CMineral(iMineralType, self, tX, tY, 1);
			if (mineral == nullptr) return 0;
			iDynamicHandle = 0;
			auto &dynObjects = game_.dynamicObjects_;
			switch (iMineralType) {
				case 1:
				case 2:
				case 3:
				case 4:
					iDynamicHandle = dynObjects.iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL1, self, tX, tY, 0, i);
					break;
				case 5:
				case 6:
					iDynamicHandle = dynObjects.iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL2, self, tX, tY, 0, i);
					break;
				default:
					iDynamicHandle = dynObjects.iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL1, self, tX, tY, 0, i);
					break;
			}
			if (iDynamicHandle == 0) {
				delete mineral;
				mineral = nullptr;
				return 0;
			}
			mineral->m_sDynamicObjectHandle = iDynamicHandle;
			mineral->map_ = self;
			switch (iMineralType) {
				case 1: mineral->m_iDifficulty = 10;
					mineral->m_iRemain = 20;
					break;
				case 2: mineral->m_iDifficulty = 15;
					mineral->m_iRemain = 15;
					break;
				case 3: mineral->m_iDifficulty = 20;
					mineral->m_iRemain = 10;
					break;
				case 4: mineral->m_iDifficulty = 50;
					mineral->m_iRemain = 8;
					break;
				case 5: mineral->m_iDifficulty = 70;
					mineral->m_iRemain = 6;
					break;
				case 6: mineral->m_iDifficulty = 90;
					mineral->m_iRemain = 4;
					break;
				default: mineral->m_iDifficulty = 10;
					mineral->m_iRemain = 20;
					break;
			}
			this->m_iCurMineral++;
			return i;
		}
	}
	return 0;
}

void CMap::_CheckStrategicPointOccupyStatus() {
	class CTile * pTile;
	int i;
	int iX;
	int iY;
	int iValue;
	game_.m_iStrategicStatus = 0;
	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++) {
		if (this->m_pStrategicPointList[i] != nullptr) {
			iValue = this->m_pStrategicPointList[i]->m_iValue;
			iX = this->m_pStrategicPointList[i]->m_iX;
			iY = this->m_pStrategicPointList[i]->m_iY;
			pTile = (class CTile *)(this->m_pTile + iX + iY * this->m_sSizeY);
			game_.m_iStrategicStatus += pTile->m_iOccupyStatus * iValue;
		}
	}
}

void CMap::DoAbaddonThunderDamageHandler() {
	int iResult;
	//if (m_bIsApocalypseMode != true) return;
	if (iDice(1, 15) != 13) return;
	uint32_t dwTime = timeGetTime();
	auto self = shared_from_this();
	for (auto &clientIter : m_pClientList) {
		if ((memcmp(clientIter.map_->m_cName, "abaddon", 7) == 0)
				  || (clientIter.map_ == self)) {
			switch (iDice(1, 4)) {
				case 1:
					game_.SendThunder(clientIter.id_, clientIter.m_sX, clientIter.m_sY, 203, clientIter.m_sType);
					game_.SendThunder(clientIter.id_, clientIter.m_sX - iDice(1, 7), clientIter.m_sY + iDice(1, 5), 161, clientIter.m_sType);
					break;
				case 2:
					game_.SendThunder(clientIter.id_, clientIter.m_sX, clientIter.m_sY, 202, clientIter.m_sType);
					game_.SendThunder(clientIter.id_, clientIter.m_sX + iDice(1, 7), clientIter.m_sY - iDice(1, 5), 161, clientIter.m_sType);
					break;
				case 3:
					game_.SendThunder(clientIter.id_, clientIter.m_sX, clientIter.m_sY, 201, clientIter.m_sType);
					game_.SendThunder(clientIter.id_, clientIter.m_sX + iDice(1, 7), clientIter.m_sY + 5 - iDice(1, 9), 161, clientIter.m_sType);
					break;
				case 4:
					game_.SendThunder(clientIter.id_, clientIter.m_sX, clientIter.m_sY, 200, clientIter.m_sType);
					game_.SendThunder(clientIter.id_, clientIter.m_sX + 5 - iDice(1, 9), clientIter.m_sY + 7 - iDice(1, 4), 161, clientIter.m_sType);
					break;
			}
			if (clientIter.m_iAdminUserLevel > 0) continue;
			iResult = iDice(1, 20) + 100;
			if ((clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
					  || (clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5)) {
				iResult /= 2;
			}
			// Not for v3.51: clientIter.SendNotifyMsg(0,DEF_NOTIFY_0BE5, 0, 0, 0, nullptr);
			clientIter.m_iHP -= iResult;
			if (clientIter.m_iHP <= 0) {
				clientIter.ClientKilledHandler(0, 0, iResult);
			} else if (iResult > 0) {
				clientIter.m_dwLastDamageTime = dwTime;
				clientIter.SendNotifyMsg(0, DEF_NOTIFY_HP, 0, 0, 0, nullptr);
				game_.SendEventToNearClient_TypeA(clientIter.id_, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iResult, 0, 0);
				if (clientIter.m_bSkillUsingStatus[19] != true) {
					clientIter.map_->ClearOwner(0, clientIter.id_, DEF_OWNERTYPE_PLAYER, clientIter.m_sX, clientIter.m_sY);
					clientIter.map_->SetOwner(clientIter.id_, DEF_OWNERTYPE_PLAYER, clientIter.m_sX, clientIter.m_sY);
				}
				if (clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
					clientIter.SendNotifyMsg(0, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, 2, 0, nullptr);
					game_.delayEvents_.remove(clientIter.id_, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
					clientIter.m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
				}
			}
		}
	}
}

int CMap::bCreateNewNpc(const char * pNpcName, char * pName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned, bool bFirmBerserk, bool bIsMaster, int iGuildGUID) {
	int t, j, k;
	char cTxt[120];
	short sX;
	short sY;
	short sRange;
	bool bFlag;
	if (strlen(pName) == 0) return false;
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	for (int i = 1; i < DEF_MAXNPCS; i++) {
		if (npcs_[i] == nullptr) {
			npcs_[i] = std::make_shared<CNpc>(i, game_, pName);
			if (game_._bInitNpcAttr(&*npcs_[i], pNpcName, sClass, cSA) == false) {
				wsprintf(cTxt, "(!) Not existing NPC creation request! (%s) Ignored.", pNpcName);
				PutLogList(cTxt);
				npcs_[i].reset();
				return false;
			}
			if (npcs_[i]->m_cDayOfWeekLimit < 10) {
				if (npcs_[i]->m_cDayOfWeekLimit != SysTime.wDayOfWeek) {
					npcs_[i].reset();
					return false;
				}
			}
			switch (cMoveType) {
				case DEF_MOVETYPE_GUARD:
				case DEF_MOVETYPE_RANDOM:
					if ((poX != nullptr) && (poY != nullptr) && (*poX != 0) && (*poY != 0)) {
						sX = *poX;
						sY = *poY;
					} else {
						for (j = 0; j <= 30; j++) {
							sX = (rand() % (this->m_sSizeX - 50)) + 15;
							sY = (rand() % (this->m_sSizeY - 50)) + 15;
							bFlag = true;
							for (k = 0; k < DEF_MAXMGAR; k++)
								if (this->m_rcMobGenAvoidRect[k].left != -1) {
									if ((sX >= this->m_rcMobGenAvoidRect[k].left) &&
											  (sX <= this->m_rcMobGenAvoidRect[k].right) &&
											  (sY >= this->m_rcMobGenAvoidRect[k].top) &&
											  (sY <= this->m_rcMobGenAvoidRect[k].bottom)) {
										bFlag = false;
									}
								}
							if (bFlag == true) goto GET_VALIDLOC_SUCCESS;
						}
						npcs_[i].reset();
						return false;
GET_VALIDLOC_SUCCESS:
						;
					}
					break;
				case DEF_MOVETYPE_RANDOMAREA:
					sRange = (short) (pArea->right - pArea->left);
					sX = (short) ((rand() % sRange) + pArea->left);
					sRange = (short) (pArea->bottom - pArea->top);
					sY = (short) ((rand() % sRange) + pArea->top);
					break;
				case DEF_MOVETYPE_RANDOMWAYPOINT:
					sX = (short) this->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].x;
					sY = (short) this->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].y;
					break;
				default:
					if ((poX != nullptr) && (poY != nullptr) && (*poX != 0) && (*poY != 0)) {
						sX = *poX;
						sY = *poY;
					} else {
						sX = (short) this->m_WaypointList[pWaypointList[0]].x;
						sY = (short) this->m_WaypointList[pWaypointList[0]].y;
					}
					break;
			}
			if (this->bGetEmptyPosition(&sX, &sY) == false) {
				npcs_[i].reset();
				return false;
			}
			if ((bHideGenMode == true) && (this->_iGetPlayerNumberOnSpot(sX, sY, 7) != 0)) {
				npcs_[i].reset();
				return false;
			}
			if ((poX != nullptr) && (poY != nullptr)) {
				*poX = sX;
				*poY = sY;
			}
			npcs_[i]->m_sX = sX;
			npcs_[i]->m_sY = sY;
			npcs_[i]->m_vX = sX;
			npcs_[i]->m_vY = sY;
			for (t = 0; t < 10; t++)
				npcs_[i]->m_iWayPointIndex[t] = pWaypointList[t];
			npcs_[i]->m_cTotalWaypoint = 0;
			for (t = 0; t < 10; t++)
				if (npcs_[i]->m_iWayPointIndex[t] != -1) npcs_[i]->m_cTotalWaypoint++;
			if (pArea != nullptr) {
				// RANDOMAREA Copy
				SetRect(&npcs_[i]->m_rcRandomArea, pArea->left, pArea->top, pArea->right, pArea->bottom);
			}
			switch (cMoveType) {
				case DEF_MOVETYPE_GUARD:
					npcs_[i]->m_dX = npcs_[i]->m_sX;
					npcs_[i]->m_dY = npcs_[i]->m_sY;
					break;
				case DEF_MOVETYPE_SEQWAYPOINT:
					npcs_[i]->m_cCurWaypoint = 1;
					npcs_[i]->m_dX = (short) this->m_WaypointList[ npcs_[i]->m_iWayPointIndex[ npcs_[i]->m_cCurWaypoint ] ].x;
					npcs_[i]->m_dY = (short) this->m_WaypointList[ npcs_[i]->m_iWayPointIndex[ npcs_[i]->m_cCurWaypoint ] ].y;
					break;
				case DEF_MOVETYPE_RANDOMWAYPOINT:
					npcs_[i]->m_cCurWaypoint = (rand() % (npcs_[i]->m_cTotalWaypoint - 1)) + 1;
					npcs_[i]->m_dX = (short) this->m_WaypointList[ npcs_[i]->m_iWayPointIndex[ npcs_[i]->m_cCurWaypoint ] ].x;
					npcs_[i]->m_dY = (short) this->m_WaypointList[ npcs_[i]->m_iWayPointIndex[ npcs_[i]->m_cCurWaypoint ] ].y;
					break;
				case DEF_MOVETYPE_RANDOMAREA:
					npcs_[i]->m_cCurWaypoint = 0;
					sRange = (short) (npcs_[i]->m_rcRandomArea.right - npcs_[i]->m_rcRandomArea.left);
					npcs_[i]->m_dX = (short) ((rand() % sRange) + npcs_[i]->m_rcRandomArea.left);
					sRange = (short) (npcs_[i]->m_rcRandomArea.bottom - npcs_[i]->m_rcRandomArea.top);
					npcs_[i]->m_dY = (short) ((rand() % sRange) + npcs_[i]->m_rcRandomArea.top);
					break;
				case DEF_MOVETYPE_RANDOM:
					npcs_[i]->m_dX = (short) ((rand() % (this->m_sSizeX - 50)) + 15);
					npcs_[i]->m_dY = (short) ((rand() % (this->m_sSizeY - 50)) + 15);
					break;
			}
			npcs_[i]->m_tmp_iError = 0;
			npcs_[i]->m_cMoveType = cMoveType;
			switch (npcs_[i]->m_cActionLimit) {
				case 2:
				case 3:
				case 5:
					npcs_[i]->m_cBehavior = DEF_BEHAVIOR_STOP;
					switch (npcs_[i]->m_sType) {
						case 15: // ShopKeeper-W
						case 19: // Gandlf
						case 20: // Howard
						case 24: // Tom
						case 25: // William
						case 26: // Kennedy
							npcs_[i]->m_cDir = 4 + iDice(1, 3) - 1;
							break;
						default:
							npcs_[i]->m_cDir = iDice(1, 8);
							break;
					}
					break;
				default:
					npcs_[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
					npcs_[i]->m_cDir = 5;
					break;
			}
			npcs_[i]->m_iFollowOwnerIndex = 0;
			npcs_[i]->m_iTargetIndex = 0;
			npcs_[i]->m_cTurn = (rand() % 2);
			switch (npcs_[i]->m_sType) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					npcs_[i]->m_sAppr2 = (short) 0xF000;
					npcs_[i]->m_sAppr2 = npcs_[i]->m_sAppr2 | ((rand() % 13) << 4); // ¹«±â
					npcs_[i]->m_sAppr2 = npcs_[i]->m_sAppr2 | (rand() % 9); // ¹æÆÐ
					break;
				case 36: // AGT-Aresden/AGT-Elvine
				case 37: // CGT-Aresden/CGT-Elvine
				case 38: // MS-Aresden/MS-Elvine
				case 39: // DT-Aresden/DT-Elvine
					npcs_[i]->m_sAppr2 = 3;
					break;
					//case 64: // Crop
					//	npcs_[i]->m_sAppr2 = 1;
					//	break;
					// appr2 = 2 seems to be enemy detection for crusade
					/*case 91: // gate

						npcs_[i]->m_sAppr2 = 0xF000; // 10 aura no sphere 13 no name movable with magic (crash) test to 29
						break;*/
				case 64: // Crop
					npcs_[i]->m_sAppr2 = 1; // 1 bud; 2 grown; 3 large
					break;
					// case 66: // Wyvern
					//	npcs_[i]->m_iStatus = 0x00000010;
					//	// npcs_[i]->m_cSide = 2;
					//	break;
				default:
					npcs_[i]->m_sAppr2 = 0;
					break;
			}
			npcs_[i]->map_ = shared_from_this();
			npcs_[i]->m_dwTime = timeGetTime() + (rand() % 10000);
			npcs_[i]->m_dwActionTime += (rand() % 300);
			npcs_[i]->m_dwMPupTime = timeGetTime();
			npcs_[i]->m_dwHPupTime = npcs_[i]->m_dwMPupTime;
			npcs_[i]->m_sBehaviorTurnCount = 0;
			npcs_[i]->m_bIsSummoned = bIsSummoned;
			npcs_[i]->m_bIsMaster = bIsMaster;
			if (bIsSummoned == true)
				npcs_[i]->m_dwSummonedTime = timeGetTime();
			if (bFirmBerserk == true) {
				npcs_[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = 1;
				npcs_[i]->m_iStatus = npcs_[i]->m_iStatus | 0x20;
				//iExpRoll = iDice(npcs_[i]->m_iExpDiceMin, npcs_[i]->m_iExpDiceMax);
				//iExpRoll *= 2;
			}
			// !!!
			if (cChangeSide != -1) npcs_[i]->m_cSide = cChangeSide;
			npcs_[i]->m_cBravery = (rand() % 3) + npcs_[i]->m_iMinBravery;
			npcs_[i]->m_iSpotMobIndex = iSpotMobIndex;
			npcs_[i]->m_iGuildGUID = iGuildGUID;
			//testcode
			if (iGuildGUID != 0) {
				wsprintf(G_cTxt, "Summon War Unit(%d) GUID(%d)", npcs_[i]->m_sType, iGuildGUID);
				PutLogList(G_cTxt);
			}
			this->SetOwner(i, DEF_OWNERTYPE_NPC, sX, sY);
			this->m_iTotalActiveObject++;
			this->m_iTotalAliveObject++;
			switch (npcs_[i]->m_sType) {
				case 36: // AGT-Aresden/AGT-Elvine
				case 37: // CGT-Aresden/CGT-Elvine
				case 38: // MS-Aresden/MS-Elvine
				case 39: // DT-Aresden/DT-Elvine
				case 42: // ManaStone
					this->bAddCrusadeStructureInfo(npcs_[i]->m_sType, sX, sY, npcs_[i]->m_cSide);
					break;
				case 64:
					this->bAddCropsTotalSum();
					break;
			}
			game_.SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
			return true;
		}
	}
	return false;
}

