#pragma once
#include <memory>
#include <cstring>
#include "OccupyFlag.h"
#include "Tile.h"
#include "StrategicPoint.h"
#include "StrTok.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"

#define DEF_OWNERTYPE_PLAYER			1
#define DEF_OWNERTYPE_NPC				2
#define DEF_OWNERTYPE_PLAYER_INDIRECT	3
// for Client Data
#define DEF_MAPDATASIZEX		30
#define DEF_MAPDATASIZEY		25
#define DEF_MAXTELEPORTLOC		200
#define DEF_MAXWAYPOINTCFG		200
#define DEF_MAXMGAR				50
#define DEF_MAXNMR				50
#define DEF_MAXSPOTMOBGENERATOR	100
#define DEF_MAXFISHPOINT		200
#define DEF_MAXITEMEVENTS		200
#define DEF_MAXMINERALPOINT		200
#define DEF_MAXHELDENIANDOOR	200
#define DEF_MAXOCCUPYFLAG		20001 //3001
#define	DEF_MAXINITIALPOINT		20
#define DEF_MAXAGRICULTURE		200
#define DEF_MAXDYNAMICGATES		10
#define DEF_MAXHELDENIANTOWER	200
#define DEF_PLAYER_MAX_EXPERIENCE 203484288 // Exp negativa fix.y reemplazan los numeros 121586796 por la exp de un personaje max lvl de su servidor.
#define DEF_MAPTYPE_NORMAL				0
#define DEF_MAPTYPE_NOPENALTY_NOREWARD	1
#define DEF_MAXENERGYSPHERES			10
#define DEF_MAXSTRATEGICPOINTS			200
#define DEF_MAXSECTORS					60
#define DEF_MAXSTRIKEPOINTS				20
struct Clients;
struct CGame;
struct CMap;
struct Npcs;
class CMap : public std::enable_shared_from_this<CMap> {
public:
	CMap(int id, CGame &game);
	virtual ~CMap();
	void ClearBigOwner(short sOwnerH, char cOwnerType, short pX, short pY, char cArea);
	bool bCheckFlySpaceAvailable(short sX, char sY, char cDir, short sOwner);
	bool bGetIsFarm(short tX, short tY);
	void RestoreStrikePoints();
	bool bRemoveCrusadeStructureInfo(short sX, short sY);
	bool bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide);
	int iGetAttribute(int dX, int dY, int iBitMask);
	void _SetupNoAttackArea();
	void ClearTempSectorInfo();
	void ClearSectorInfo();
	int iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI);
	int iCheckItem(short sX, short sY);
	void SetTempMoveAllowedFlag(int dX, int dY, bool bFlag);
	bool bGetIsWater(short dX, short dY);
	void GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY);
	bool bGetIsMoveAllowedTile(short dX, short dY);
	void SetNamingValueEmpty(int iValue);
	int iGetEmptyNamingValue();
	bool bGetDynamicObject(short sX, short sY, DynamicObjectType * pType, uint32_t * pRegisterTime, int * pIndex = nullptr);
	void SetDynamicObject(uint16_t wID, DynamicObjectType sType, short sX, short sY, uint32_t dwRegisterTime);
	bool bGetIsTeleport(short dX, short dY);
	bool bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir);
	bool bInit(char * pName);
	bool bIsValidLoc(short sX, short sY);
	class CItem * pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor);
	bool bSetItem(short sX, short sY, class CItem * pItem);
	void ClearDeadOwner(short sX, short sY);
	void ClearOwner(int iDebugCode, short sOwnerH, char cOwnerType, short sX, short sY);
	bool bGetMoveable(short dX, short dY, DynamicObjectType * pDOtype = nullptr, short * pTopItem = nullptr);
	void GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY);
	void SetOwner(short sOwner, char cOwnerClass, short sX, short sY);
	void SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY);
	bool bRemoveCropsTotalSum();
	bool bAddCropsTotalSum();
	void SetBigOwner(short sOwner, char cOwnerClass, short sX, short sY, char cArea);
	const int id_;
	Clients &m_pClientList;
	class CTile * m_pTile;
	CGame &game_;
	char m_cName[11];
	char m_cLocationName[11];
	short m_sSizeX;
	short m_sSizeY;
	short m_sTileDataSize;
	class CTeleportLoc * m_pTeleportLoc[DEF_MAXTELEPORTLOC];
	//short m_sInitialPointX, m_sInitialPointY;
	POINT m_pInitialPoint[DEF_MAXINITIALPOINT];
	bool m_bNamingValueUsingStatus[1000]; // 0~999
	bool m_bRandomMobGenerator;
	char m_cRandomMobGeneratorLevel;
	int m_iTotalActiveObject;
	int m_iTotalAliveObject;
	int m_iMaximumObject;
	char m_cType;
	bool m_bIsFixedDayMode;
	struct {
		bool bDefined;
		char cType; // 1:RANDOMAREA   2:RANDOMWAYPOINT
		char cWaypoint[10];
		RECT rcRect;
		int iTotalActiveMob;
		int iMobType;
		int iMaxMobs;
		int iCurMobs;
	} m_stSpotMobGenerator[DEF_MAXSPOTMOBGENERATOR];
	POINT m_WaypointList[DEF_MAXWAYPOINTCFG];
	RECT m_rcMobGenAvoidRect[DEF_MAXMGAR];
	RECT m_rcNoAttackRect[DEF_MAXNMR];
	POINT m_FishPointList[DEF_MAXFISHPOINT];
	int m_iTotalFishPoint;
	int m_iMaxFish;
	int m_iCurFish;
	int m_iApocalypseMobGenType;
	int m_iApocalypseBossMobNpcID;
	short m_sApocalypseBossMobRectX1;
	short m_sApocalypseBossMobRectY1;
	short m_sApocalypseBossMobRectX2;
	short m_sApocalypseBossMobRectY2;
	char m_cDynamicGateType;
	short m_sDynamicGateCoordRectX1;
	short m_sDynamicGateCoordRectY1;
	short m_sDynamicGateCoordRectX2;
	short m_sDynamicGateCoordRectY2;
	char m_cDynamicGateCoordDestMap[11];
	short m_sDynamicGateCoordTgtX;
	short m_sDynamicGateCoordTgtY;
	bool m_bIsCitizenLimit;
	short m_sHeldenianTowerType;
	short m_sHeldenianTowerXPos;
	short m_sHeldenianTowerYPos;
	char m_cHeldenianTowerSide;
	char m_cHeldenianModeMap;
	bool m_bMineralGenerator;
	char m_cMineralGeneratorLevel;
	POINT m_MineralPointList[DEF_MAXMINERALPOINT];
	int m_iTotalMineralPoint;
	int m_iMaxMineral;
	int m_iCurMineral;
	char m_cWhetherStatus;
	uint32_t m_dwWhetherLastTime;
	uint32_t m_dwWhetherStartTime;
	int m_iLevelLimit;
	int m_iUpperLevelLimit;
	class COccupyFlag * m_pOccupyFlag[DEF_MAXOCCUPYFLAG];
	int m_iTotalOccupyFlags;
	class CStrategicPoint * m_pStrategicPointList[DEF_MAXSTRATEGICPOINTS];
	bool m_bIsAttackEnabled;
	bool m_bIsFightZone;
	struct {
		char cType;
		int sX;
		int sY;
	} m_stEnergySphereCreationList[DEF_MAXENERGYSPHERES];
	int m_iTotalEnergySphereCreationPoint;
	struct {
		char cResult;
		int aresdenX;
		int aresdenY;
		int elvineX;
		int elvineY;
	} m_stEnergySphereGoalList[DEF_MAXENERGYSPHERES];
	int m_iTotalEnergySphereGoalPoint;
	bool m_bIsEnergySphereGoalEnabled;
	int m_iCurEnergySphereGoalPointIndex;
	struct {
		bool m_bIsGateMap;
		char m_cDynamicGateMap[11];
		int m_iDynamicGateX;
		int m_iDynamicGateY;
	} m_stDynamicGateCoords[DEF_MAXDYNAMICGATES];
	struct {
		int iPlayerActivity;
		int iNeutralActivity;
		int iAresdenActivity;
		int iElvineActivity;
		int iMonsterActivity;
	} m_stSectorInfo[DEF_MAXSECTORS][DEF_MAXSECTORS], m_stTempSectorInfo[DEF_MAXSECTORS][DEF_MAXSECTORS];
	short sMobEventAmount;
	int m_iTotalItemEvents;
	struct {
		char cItemName[21];
		int iAmount;
		int iTotal;
		int iMonth;
		int iDay;
		int iTotalNum;
	} m_stItemEventList[DEF_MAXITEMEVENTS];
	struct {
		char cDir;
		short dX;
		short dY;
	} m_stHeldenianGateDoor[DEF_MAXHELDENIANDOOR];
	struct {
		short sTypeID;
		short dX;
		short dY;
		char cSide;
	} m_stHeldenianTower[DEF_MAXHELDENIANTOWER];
	int m_iMaxNx;
	int m_iMaxNy;
	int m_iMaxAx;
	int m_iMaxAy;
	int m_iMaxEx;
	int m_iMaxEy;
	int m_iMaxMx;
	int m_iMaxMy;
	int m_iMaxPx;
	int m_iMaxPy;
	struct {
		char cRelatedMapName[11];
		int iMapIndex;
		int dX;
		int dY;
		int iHP;
		int iInitHP;
		int iEffectX[5];
		int iEffectY[5];
	} m_stStrikePoint[DEF_MAXSTRIKEPOINTS];
	int m_iTotalStrikePoints;
	bool m_bIsDisabled;
	int m_iTotalAgriculture;
	struct {
		char cType;
		char cSide;
		short sX;
		short sY;
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iTotalCrusadeStructures;
	bool m_bIsEnergySphereAutoCreation;
private:
	bool _bDecodeMapDataFileContents();
public:
	// Snow BOOLean for certain maps to snow instead of rain
	bool m_bIsSnowEnabled;
	bool m_bIsRecallImpossible;
	bool m_bIsApocalypseMap;
	bool m_bIsHeldenianMap;
	bool bGetEmptyPosition(short * pX, short * pY);
	void GetMapInitialPoint(short * pX, short * pY, char * pPlayerLocation = nullptr);
	int _iGetPlayerNumberOnSpot(short dX, short dY, char cRange);
	void SendEventToNearClient_TypeB(uint32_t dwMsgID, uint16_t wMsgType, short sX, short sY, short sV1, short sV2, short sV3, short sV4 = 0);
	void CheckFireBluring(int sX, int sY);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cTurn, int * pError);
	int _iCalcPlayerNum(short dX, short dY, char cRadius);
	bool __bReadMapInfo();
	void RemoveOccupyFlags();
	void MeteorStrikeHandler();
	void CalcMeteorStrikeEffectHandler();
	void DoMeteorStrikeDamageHandler();
	void _DeleteRandomOccupyFlag();
	bool _bReadMapInfoFiles();
	bool __bSetOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iClientH, bool bAdminFlag);
	int iCreateMineral(int tX, int tY, char cLevel);
	void _CheckStrategicPointOccupyStatus();
	void DoAbaddonThunderDamageHandler();
	void MineralGenerator();
	int bCreateNewNpc(const char * pNpcName, char * pName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned = false, bool bFirmBerserk = false, bool bIsMaster = false, int iGuildGUID = 0);
	Npcs &npcs_;
};
#define DEF_MAXMAPS					100

typedef std::shared_ptr<CMap> MapPtr;
typedef std::weak_ptr<CMap> MapWPtr;
struct Maps {
	typedef std::shared_ptr<CMap> value_type;
	typedef value_type &ref_type;
	ref_type operator[](size_t index) {
		return m_pMapList[index];
	}
	value_type byName(const char *name) {
		for (auto &m : m_pMapList) {
			if (m && std::strcmp(m->m_cName, name) == 0) {
				return m;
			}
		}
		return
		{
		};
	}
	void clear() {
		m_pMapList = {
			{}
		};
	}
private:
	std::array<value_type, DEF_MAXMAPS> m_pMapList;
};