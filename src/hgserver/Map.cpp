#include "Map.h"
#include <cstring>

extern void PutLogFileList(char * cStr);

CMap::CMap(class CGame * pGame)
: m_bIsSnowEnabled(false) {
	for (int i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = 0;

	for (int i = 0; i < DEF_MAXWAYPOINTCFG; i++) {
		m_WaypointList[i].x = -1;
		m_WaypointList[i].y = -1;
	}

	for (int i = 0; i < DEF_MAXMGAR; i++) {
		m_rcMobGenAvoidRect[i].top = -1;
		m_rcMobGenAvoidRect[i].left = -1;
	}

	for (int i = 0; i < DEF_MAXNMR; i++) {
		m_rcNoAttackRect[i].top = -1;
		m_rcNoAttackRect[i].left = -1;
	}

	for (int i = 0; i < DEF_MAXSPOTMOBGENERATOR; i++) {
		m_stSpotMobGenerator[i].bDefined = false;
		m_stSpotMobGenerator[i].iTotalActiveMob = 0;
	}

	for (int i = 0; i < DEF_MAXFISHPOINT; i++) {
		m_FishPointList[i].x = -1;
		m_FishPointList[i].y = -1;
	}

	for (int i = 0; i < DEF_MAXMINERALPOINT; i++) {
		m_MineralPointList[i].x = -1;
		m_MineralPointList[i].y = -1;
	}

	for (int i = 0; i < DEF_MAXINITIALPOINT; i++) {
		m_pInitialPoint[i].x = -1;
		m_pInitialPoint[i].y = -1;
	}

	for (int i = 0; i < 1000; i++)
		m_bNamingValueUsingStatus[i] = false;

	for (int i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		m_pOccupyFlag[i] = 0;

	for (int i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		m_pStrategicPointList[i] = 0;

	for (int i = 0; i < DEF_MAXENERGYSPHERES; i++) {
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

	m_pTile = 0;

	m_cWhetherStatus = 0;
	m_cType = DEF_MAPTYPE_NORMAL;

	m_pGame = pGame;

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

	for (int ix = 0; ix < DEF_MAXSECTORS; ix++)
		for (int iy = 0; iy < DEF_MAXSECTORS; iy++) {
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

	for (int i = 0; i < DEF_MAXHELDENIANDOOR; i++) {
		m_stHeldenianGateDoor[i].cDir = 0;
		m_stHeldenianGateDoor[i].dX = 0;
		m_stHeldenianGateDoor[i].dY = 0;
	}

	for (int i = 0; i < DEF_MAXHELDENIANTOWER; i++) {
		m_stHeldenianTower[i].sTypeID = NpcType::none;
		m_stHeldenianTower[i].dX = 0;
		m_stHeldenianTower[i].dY = 0;
		m_stHeldenianTower[i].cSide = 0;
	}

	for (int i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].dX = 0;
		m_stStrikePoint[i].dY = 0;
		m_stStrikePoint[i].iHP = 0;
		m_stStrikePoint[i].iMapIndex = -1;
		std::memset(m_stStrikePoint[i].cRelatedMapName, 0, sizeof (m_stStrikePoint[i].cRelatedMapName));
	}
	m_iTotalStrikePoints = 0;
	m_bIsDisabled = false;

	for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NpcType::none;
		m_stCrusadeStructureInfo[i].cSide = 0;
		m_stCrusadeStructureInfo[i].sX = 0;
		m_stCrusadeStructureInfo[i].sY = 0;
	}
	m_iTotalCrusadeStructures = 0;
	m_iTotalAgriculture = 0;
}

CMap::~CMap() {
	delete []m_pTile;
	for (int i = 0; i < DEF_MAXTELEPORTLOC; i++)
		delete m_pTeleportLoc[i];

	for (int i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		delete m_pOccupyFlag[i];

	for (int i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		delete m_pStrategicPointList[i];
}

void CMap::SetOwner(short sOwner, char cOwnerClass, short sX, short sY) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return;
	pTile->m_sOwner = sOwner;
	pTile->m_cOwnerClass = cOwnerClass;
}
//   -->
//  |
//  V

char _tmp_cMoveDirX[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};
char _tmp_cMoveDirY[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};

bool CMap::bCheckFlySpaceAvailable(short sX, char sY, char cDir, short sOwner) {
	if ((cDir <= 0) || (cDir > 8)) return false;
	short dX = _tmp_cMoveDirX[cDir] + sX;
	short dY = _tmp_cMoveDirY[cDir] + sY;
	if (outOfBounds(20, 20, dX, dY)) return false;
	CTile * pTile = getTile(dX, dY);
	if (pTile->m_sOwner != 0) return false;
	pTile->m_sOwner = sOwner;
	return true;
}

void CMap::SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return;
	pTile->m_sDeadOwner = sOwner;
	pTile->m_cDeadOwnerClass = cOwnerClass;
}

void CMap::GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) {
		*pOwner = 0;
		*pOwnerClass = 0;
		return;
	}
	*pOwner = pTile->m_sOwner;
	*pOwnerClass = pTile->m_cOwnerClass;

	if ((*pOwnerClass == 1) && (*pOwner > DEF_MAXCLIENTS)) {
		*pOwner = 0;
		*pOwnerClass = 0;
		return;
	}

	if (pTile->m_sOwner == 0) *pOwnerClass = 0;
}

void CMap::GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) {
		*pOwner = 0;
		*pOwnerClass = 0;
		return;
	}
	*pOwner = pTile->m_sDeadOwner;
	*pOwnerClass = pTile->m_cDeadOwnerClass;
}

bool CMap::bGetMoveable(short dX, short dY, short * pDOtype, short * pTopItem) {
	if (outOfBounds(20, 20, dX, dY)) return false;
	CTile * pTile = getTile(dX, dY);
	if (pDOtype != 0) *pDOtype = pTile->m_sDynamicObjectType;
	if (pTopItem != 0) *pTopItem = pTile->m_cTotalItem;

	if (pTile->m_sOwner != 0) return false;
	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;
	return true;
}

bool CMap::bGetIsMoveAllowedTile(short dX, short dY) {
	if (outOfBounds(20, 20, dX, dY)) return false;
	CTile * pTile = getTile(dX, dY);
	if (pTile->m_bIsMoveAllowed == false) return false;
	if (pTile->m_bIsTempMoveAllowed == false) return false;
	return true;
}

bool CMap::bGetIsTeleport(short dX, short dY) {
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;
	CTile *pTile = getTile(dX, dY);
	return pTile->m_bIsTeleport;
}

void CMap::ClearOwner(int /*iDebugCode*/, short sOwnerH, char cOwnerType, short sX, short sY) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return;
	if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
		pTile->m_sOwner = 0;
		pTile->m_cOwnerClass = 0;
	}
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner = 0;
		pTile->m_cDeadOwnerClass = 0;
	}
}

char _tmp_cEmptyAreaX[] = {0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2};
char _tmp_cEmptyAreaY[] = {0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2};

CTile *CMap::getTile(short x, short y) {
	if ((x < 0) || (x >= m_sSizeX) || (y < 0) || (y >= m_sSizeY)) return nullptr;
	return (class CTile *)(m_pTile + x + y * m_sSizeY);
}

void CMap::ClearBigOwner(short sOwnerH, char cOwnerType, short pX, short pY, char cArea) {
	CTile * pTile = getTile(pX, pY);
	if (!pTile) return;
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner = 0;
		pTile->m_cDeadOwnerClass = 0;
	}
	const short sAreaSquared = cArea*cArea;
	for (int i = 0; i < sAreaSquared; i++) {
		const short sX = pX + _tmp_cEmptyAreaX[i];
		const short sY = pY + _tmp_cEmptyAreaY[i];
		pTile = getTile(sX, sY);
		if (pTile && (pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
			pTile->m_sOwner = 0;
			pTile->m_cOwnerClass = 0;
		}
	}
}

void CMap::ClearDeadOwner(short sX, short sY) {
	CTile *pTile = getTile(sX, sY);
	if (!pTile) return;
	pTile->m_sDeadOwner = 0;
	pTile->m_cDeadOwnerClass = 0;
}

bool CMap::bSetItem(short sX, short sY, class CItem * pItem) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return false;
	if (pTile->m_pItem[DEF_TILE_PER_ITEMS - 1] != 0)
		delete pTile->m_pItem[DEF_TILE_PER_ITEMS - 1];
	else pTile->m_cTotalItem++;

	for (int i = DEF_TILE_PER_ITEMS - 2; i >= 0; i--) {
		pTile->m_pItem[i + 1] = pTile->m_pItem[i];
	}
	pTile->m_pItem[0] = pItem;
	return true;
}

class CItem * CMap::pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return 0;
	if (pTile->m_cTotalItem == 0) return 0;
	CItem * pItem = pTile->m_pItem[0];

	for (int i = 0; i <= DEF_TILE_PER_ITEMS - 2; i++) {
		pTile->m_pItem[i] = pTile->m_pItem[i + 1];
	}
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

ItemId CMap::iCheckItem(short sX, short sY) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return ItemId::none;
	CItem *pItem = pTile->m_pItem[0];
	if (pTile->m_cTotalItem == 0) return ItemId::none;
	return pItem->m_sIDnum;
}

bool CMap::bIsValidLoc(short sX, short sY) {
	return getTile(sX, sY);
}

bool CMap::bInit(char * pName) {
	std::memset(m_cName, 0, sizeof (m_cName));
	strcpy(m_cName, pName);
	std::memset(m_cLocationName, 0, sizeof (m_cLocationName));
	if (_bDecodeMapDataFileContents() == false)
		return false;
	for (int i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = 0;
	return true;
}

bool CMap::_bDecodeMapDataFileContents() {
	char cMapFileName[256] = {};
	strcat(cMapFileName, "mapdata\\");
	strcat(cMapFileName, m_cName);
	strcat(cMapFileName, ".amd");

	HANDLE hFile = CreateFile(cMapFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	char cHeader[260] = {};
	DWORD nRead = 0;
	ReadFile(hFile, (char *) cHeader, 256, &nRead, 0);

	for (int i = 0; i < 256; i++)
		if (cHeader[i] == 0) cHeader[i] = ' ';

	enum class ReadMode {
		read_header,
		read_size_x,
		read_size_y,
		read_tile_size
	};
	ReadMode cReadMode = ReadMode::read_header;

	const char seps[] = "= \t\n";
	CStrTok pStrTok(cHeader, seps);
	while (const char *token = pStrTok.pGet()) {
		switch (cReadMode) {
			case ReadMode::read_header:
				if (memcmp(token, "MAPSIZEX", 8) == 0) cReadMode = ReadMode::read_size_x;
				if (memcmp(token, "MAPSIZEY", 8) == 0) cReadMode = ReadMode::read_size_y;
				if (memcmp(token, "TILESIZE", 8) == 0) cReadMode = ReadMode::read_tile_size;
				break;
			case ReadMode::read_size_x:
				m_sSizeX = atoi(token);
				cReadMode = ReadMode::read_header;
				break;
			case ReadMode::read_size_y:
				m_sSizeY = atoi(token);
				cReadMode = ReadMode::read_header;
				break;
			case ReadMode::read_tile_size:
				m_sTileDataSize = atoi(token);
				cReadMode = ReadMode::read_header;
				break;
		}
	}

	m_pTile = (class CTile *)new class CTile[m_sSizeX * m_sSizeY];

	char cTemp[100];
	for (int iy = 0; iy < m_sSizeY; iy++) {
		for (int ix = 0; ix < m_sSizeX; ix++) {
			ReadFile(hFile, (char *) cTemp, m_sTileDataSize, &nRead, 0);
			CTile *pTile = getTile(ix, iy);
			const char c8 = cTemp[8];
			pTile->m_bIsMoveAllowed = (c8 & 0x80) == 0;
			pTile->m_bIsTeleport = (c8 & 0x40);
			pTile->m_bIsFarm = (c8 & 0x20);
			const short s0 = *((short *) &cTemp[0]);
			pTile->m_bIsWater = s0 == 19;
		}
	}
	CloseHandle(hFile);
	return true;
}

bool CMap::bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir) {
	for (int i = 0; i < DEF_MAXTELEPORTLOC; i++) {
		if ((m_pTeleportLoc[i]) && (m_pTeleportLoc[i]->m_sSrcX == sX) && (m_pTeleportLoc[i]->m_sSrcY == sY)) {
			memcpy(pMapName, m_pTeleportLoc[i]->m_cDestMapName, 10);
			*pDx = m_pTeleportLoc[i]->m_sDestX;
			*pDy = m_pTeleportLoc[i]->m_sDestY;
			*pDir = m_pTeleportLoc[i]->m_cDir;
			return true;
		}
	}
	return false;
}

void CMap::SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint32_t dwRegisterTime) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return;
	pTile->m_wDynamicObjectID = wID;
	pTile->m_sDynamicObjectType = sType;
	pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

bool CMap::bGetDynamicObject(short sX, short sY, short *pType, uint32_t *pRegisterTime, int * pIndex) {
	CTile * pTile = getTile(sX, sY);
	if (!pTile) return false;
	*pType = pTile->m_sDynamicObjectType;
	*pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
	if (pIndex != 0) *pIndex = pTile->m_wDynamicObjectID;
	return true;
}

int CMap::iGetEmptyNamingValue() {
	for (int i = 0; i < 1000; i++) {
		if (m_bNamingValueUsingStatus[i] == false) {
			m_bNamingValueUsingStatus[i] = true;
			return i;
		}
	}
	return -1;
}

void CMap::SetNamingValueEmpty(int iValue) {
	m_bNamingValueUsingStatus[iValue] = false;
}

bool CMap::bGetIsWater(short dX, short dY) {
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return false;
	CTile * pTile = getTile(dX, dY);
	return pTile->m_bIsWater;
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
	if ((tX < 14) || (tX >= m_sSizeX - 16) || (tY < 12) || (tY >= m_sSizeY - 14)) return false;
	CTile * pTile = getTile(tX, tY);
	return pTile->m_bIsFarm;
}

bool CMap::outOfBounds(int hb, int vb, int x, int y) {
	return (x < hb) || (x >= m_sSizeX - hb) || (y < vb) || (y >= m_sSizeY - vb);
}

void CMap::SetTempMoveAllowedFlag(int dX, int dY, bool bFlag) {
	if (outOfBounds(20, 20, dX, dY)) return;
	CTile * pTile = getTile(dX, dY);
	pTile->m_bIsTempMoveAllowed = bFlag;
}

int CMap::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI) {
	if (outOfBounds(20, 20, dX, dY)) return -1;
	for (int i = 1; i < DEF_MAXOCCUPYFLAG; i++) {
		if (!m_pOccupyFlag[i]) {
			m_pOccupyFlag[i] = new COccupyFlag(dX, dY, iSide, iEKNum, iDOI);
			if (!m_pOccupyFlag) return -1;
			else return i;
		}
	}
	return -1;
}

void CMap::ClearSectorInfo() {
	for (int ix = 0; ix < DEF_MAXSECTORS; ix++) {
		for (int iy = 0; iy < DEF_MAXSECTORS; iy++) {
			m_stSectorInfo[ix][iy].iNeutralActivity = 0;
			m_stSectorInfo[ix][iy].iAresdenActivity = 0;
			m_stSectorInfo[ix][iy].iElvineActivity = 0;
			m_stSectorInfo[ix][iy].iMonsterActivity = 0;
			m_stSectorInfo[ix][iy].iPlayerActivity = 0;
		}
	}
}

void CMap::ClearTempSectorInfo() {
	for (int ix = 0; ix < DEF_MAXSECTORS; ix++)
		for (int iy = 0; iy < DEF_MAXSECTORS; iy++) {
			m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
			m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
			m_stTempSectorInfo[ix][iy].iElvineActivity = 0;
			m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
			m_stTempSectorInfo[ix][iy].iPlayerActivity = 0;
		}
}

void CMap::_SetupNoAttackArea() {
	class CTile * pTile;

	for (int i = 0; i < DEF_MAXNMR; i++) {
		const auto &nar = m_rcNoAttackRect[i];
		if (nar.top <= 0 && nar.top != -10) continue;
		RECT r = nar;
		if (nar.top == -10) {
			r.bottom = m_sSizeY;
			r.top = 0;
			r.left = 0;
			r.right = m_sSizeX;
		}
		// No-Magic-Area
		for (int ix = r.left; ix <= r.right; ix++) {
			for (int iy = r.top; iy <= r.bottom; iy++) {
				CTile *pTile = getTile(ix, iy);
				if (pTile) {
					pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;
				}
			}
		}
	}
}

int CMap::iGetAttribute(int dX, int dY, int iBitMask) {
	if (outOfBounds(20, 20, dX, dY)) return -1;
	CTile * pTile = getTile(dX, dY);
	return (pTile->m_iAttribute & iBitMask);
}

bool CMap::bAddCrusadeStructureInfo(NpcType cType, short sX, short sY, char cSide) {
	for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		if (m_stCrusadeStructureInfo[i].cType == NpcType::none) {
			m_stCrusadeStructureInfo[i].cType = cType;
			m_stCrusadeStructureInfo[i].cSide = cSide;
			m_stCrusadeStructureInfo[i].sX = sX;
			m_stCrusadeStructureInfo[i].sY = sY;

			m_iTotalCrusadeStructures++;
			return true;
		}
	}
	return false;
}

bool CMap::bRemoveCrusadeStructureInfo(short sX, short sY) {
	for(auto &csi: m_stCrusadeStructureInfo) {
		if (csi.sX == sX && csi.sY == sY) {
			csi.cType = NpcType::none;
			csi.cSide = 0;
			csi.sX = 0;
			csi.sY = 0;
			for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES - 1; i++) {
				auto &c = m_stCrusadeStructureInfo[i];
				auto &n = m_stCrusadeStructureInfo[i + 1];
				if ((c.cType == NpcType::none) && (n.cType != NpcType::none)) {
					c.cType = n.cType;
					c.cSide = n.cSide;
					c.sX = n.sX;
					c.sY = n.sY;

					n.cType = NpcType::none;
					n.cSide = 0;
					n.sX = 0;
					n.sY = 0;
				}
			}
			m_iTotalCrusadeStructures--;
			return true;
		}
	}
	return false;
}

void CMap::RestoreStrikePoints() {
	for(auto &sp: m_stStrikePoint)
		sp.iInitHP = sp.iHP;
	for (int i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].iInitHP = m_stStrikePoint[i].iHP;
	}
}

