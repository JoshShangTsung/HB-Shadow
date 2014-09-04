#pragma once

#include <windows.h>
#include "OccupyFlag.h"
#include "Tile.h"
#include "StrategicPoint.h"


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
#define DEF_PLAYER_MAX_EXPERIENCE 203484288 // juan249 - Exp negativa fix.y reemplazan los numeros 121586796 por la exp de un personaje max lvl de su servidor.

#include <windows.h>
#include "Game.h"
#include "StrTok.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"

#define DEF_MAPTYPE_NORMAL				0
#define DEF_MAPTYPE_NOPENALTY_NOREWARD	1
#define DEF_MAXENERGYSPHERES			10

#define DEF_MAXSTRATEGICPOINTS			200
#define DEF_MAXSECTORS					60
#define DEF_MAXSTRIKEPOINTS				20

class CMap {
public:
	CMap(class CGame * pGame);
	~CMap();
	void ClearBigOwner(short sOwnerH, char cOwnerType, short pX, short pY, char cArea);
	bool bCheckFlySpaceAvailable(short sX, char sY, char cDir, short sOwner);
	bool bGetIsFarm(short tX, short tY);
	void RestoreStrikePoints();
	bool bRemoveCrusadeStructureInfo(short sX, short sY);
	bool bAddCrusadeStructureInfo(NpcType cType, short sX, short sY, char cSide);
	int iGetAttribute(int dX, int dY, int iBitMask);
	void _SetupNoAttackArea();
	void ClearTempSectorInfo();
	void ClearSectorInfo();
	int iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI);
	ItemId iCheckItem(short sX, short sY);
	void SetTempMoveAllowedFlag(int dX, int dY, bool bFlag);
	bool bGetIsWater(short dX, short dY);
	void GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY);
	bool bGetIsMoveAllowedTile(short dX, short dY);
	void SetNamingValueEmpty(int iValue);
	int iGetEmptyNamingValue();
	bool bGetDynamicObject(short sX, short sY, short * pType, uint32_t * pRegisterTime, int * pIndex = 0);
	void SetDynamicObject(uint16_t wID, short sType, short sX, short sY, uint32_t dwRegisterTime);
	bool bGetIsTeleport(short dX, short dY);
	bool bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir);
	bool bInit(char * pName);
	bool bIsValidLoc(short sX, short sY);
	class CItem * pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor);
	bool bSetItem(short sX, short sY, class CItem * pItem);
	void ClearDeadOwner(short sX, short sY);
	void ClearOwner(int iDebugCode, short sOwnerH, char cOwnerType, short sX, short sY);
	bool bGetMoveable(short dX, short dY, short * pDOtype = 0, short * pTopItem = 0);
	void GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY);
	void SetOwner(short sOwner, char cOwnerClass, short sX, short sY);
	void SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY);
	bool bRemoveCropsTotalSum();
	bool bAddCropsTotalSum();
	void SetBigOwner(short sOwner, char cOwnerClass, short sX, short sY, char cArea);
	class CTile *getTile(short x, short y);
	bool outOfBounds(int hb, int vb, int x, int y);
	class CTile * m_pTile=0;
	class CGame * m_pGame=0;
	char m_cName[11]{};
	char m_cLocationName[11]{};
	short m_sSizeX=0;
	short m_sSizeY=0;
	short m_sTileDataSize=0;
	class CTeleportLoc * m_pTeleportLoc[DEF_MAXTELEPORTLOC]{};

	//short m_sInitialPointX, m_sInitialPointY;
	POINT m_pInitialPoint[DEF_MAXINITIALPOINT]{};

	bool m_bNamingValueUsingStatus[1000]{}; // 0~999
	bool m_bRandomMobGenerator=0;
	char m_cRandomMobGeneratorLevel=0;
	int m_iTotalActiveObject=0;
	int m_iTotalAliveObject=0;
	int m_iMaximumObject=0;

	char m_cType=0;

	bool m_bIsFixedDayMode=false;

	struct SpotMobGenerator {
		bool bDefined=false;
		char cType=0; // 1:RANDOMAREA   2:RANDOMWAYPOINT

		char cWaypoint[10]{}; // RANDOMWAYPOINT
		RECT rcRect{0,0,0,0}; // RANDOMAREA

		int iTotalActiveMob=0;
		int iMobType=0;
		int iMaxMobs=0;
		int iCurMobs=0;

	};
	SpotMobGenerator m_stSpotMobGenerator[DEF_MAXSPOTMOBGENERATOR]{};

	POINT m_WaypointList[DEF_MAXWAYPOINTCFG]{};
	RECT m_rcMobGenAvoidRect[DEF_MAXMGAR]{};
	RECT m_rcNoAttackRect[DEF_MAXNMR]{};

	POINT m_FishPointList[DEF_MAXFISHPOINT]{};
	int m_iTotalFishPoint=0, m_iMaxFish=0, m_iCurFish=0;

	int m_iApocalypseMobGenType=0, m_iApocalypseBossMobNpcID=0;
	short m_sApocalypseBossMobRectX1=0, m_sApocalypseBossMobRectY1=0, m_sApocalypseBossMobRectX2=0, m_sApocalypseBossMobRectY2=0;
	char m_cDynamicGateType=0;
	short m_sDynamicGateCoordRectX1=0, m_sDynamicGateCoordRectY1=0, m_sDynamicGateCoordRectX2=0, m_sDynamicGateCoordRectY2=0;
	char m_cDynamicGateCoordDestMap[11]{};
	short m_sDynamicGateCoordTgtX=0, m_sDynamicGateCoordTgtY=0;
	bool m_bIsCitizenLimit=0;
	short m_sHeldenianTowerType=0, m_sHeldenianTowerXPos=0, m_sHeldenianTowerYPos=0;
	char m_cHeldenianTowerSide=0;
	char m_cHeldenianModeMap=0;

	bool m_bMineralGenerator=0;
	char m_cMineralGeneratorLevel=0;
	POINT m_MineralPointList[DEF_MAXMINERALPOINT]{};
	int m_iTotalMineralPoint=0, m_iMaxMineral=0, m_iCurMineral=0;

	char m_cWhetherStatus=0;
	uint32_t m_dwWhetherLastTime=0, m_dwWhetherStartTime=0;

	int m_iLevelLimit=0;
	int m_iUpperLevelLimit=0;

	class COccupyFlag * m_pOccupyFlag[DEF_MAXOCCUPYFLAG]{};
	int m_iTotalOccupyFlags=0;

	class CStrategicPoint * m_pStrategicPointList[DEF_MAXSTRATEGICPOINTS]={};
	bool m_bIsAttackEnabled=true;

	bool m_bIsFightZone=false;

	struct EnergySphereCreation {
		char cType=0;
		int sX=0;
		int sY=0;

	};
	EnergySphereCreation m_stEnergySphereCreationList[DEF_MAXENERGYSPHERES];

	int m_iTotalEnergySphereCreationPoint=0;

	struct EnergySphereGoal {
		char cResult=0;
		int aresdenX=0;
		int aresdenY=0;
		int elvineX=0;
		int elvineY=0;
	};
	EnergySphereGoal m_stEnergySphereGoalList[DEF_MAXENERGYSPHERES];

	int m_iTotalEnergySphereGoalPoint=0;

	bool m_bIsEnergySphereGoalEnabled=0;
	int m_iCurEnergySphereGoalPointIndex=0;

	struct DynamicGateCoords {
		bool m_bIsGateMap=0;
		char m_cDynamicGateMap[11]{};
		int m_iDynamicGateX=0;
		int m_iDynamicGateY=0;
	};
	DynamicGateCoords m_stDynamicGateCoords[DEF_MAXDYNAMICGATES];

	struct SectorInfo {
		int iPlayerActivity=0;
		int iNeutralActivity=0;
		int iAresdenActivity=0;
		int iElvineActivity=0;
		int iMonsterActivity=0;

	};
	SectorInfo m_stSectorInfo[DEF_MAXSECTORS][DEF_MAXSECTORS];
	SectorInfo m_stTempSectorInfo[DEF_MAXSECTORS][DEF_MAXSECTORS];
	short sMobEventAmount=0;
	int m_iTotalItemEvents=0;

	struct {
		char cItemName[21]{};
		int iAmount=0;
		int iTotal=0;
		int iMonth=0;
		int iDay=0;
		int iTotalNum=0;
	} m_stItemEventList[DEF_MAXITEMEVENTS];

	struct {
		char cDir=0;
		short dX=0;
		short dY=0;
	} m_stHeldenianGateDoor[DEF_MAXHELDENIANDOOR];

	struct {
		NpcType sTypeID=NpcType::none;
		short dX=0;
		short dY=0;
		char cSide=0;
	} m_stHeldenianTower[DEF_MAXHELDENIANTOWER];

	int m_iMaxNx = 0, m_iMaxNy = 0, m_iMaxAx= 0, m_iMaxAy=0, m_iMaxEx=0, m_iMaxEy=0, m_iMaxMx=0, m_iMaxMy=0, m_iMaxPx=0, m_iMaxPy=0;

	struct {
		char cRelatedMapName[11]{};
		int iMapIndex = 0;
		int dX = 0;
		int dY = 0;
		int iHP = 0;
		int iInitHP = 0;

		int iEffectX[5] = {};
		int iEffectY[5] = {};

	} m_stStrikePoint[DEF_MAXSTRIKEPOINTS];
	int m_iTotalStrikePoints = 0;

	bool m_bIsDisabled = false;
	int m_iTotalAgriculture = 0;

	struct {
		NpcType cType = NpcType::none;
		char cSide = 0;
		short sX = 0;
		short sY = 0;
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iTotalCrusadeStructures = 0;
	bool m_bIsEnergySphereAutoCreation = false;
private:
	bool _bDecodeMapDataFileContents();
public:
	// Snow BOOLean for certain maps to snow instead of rain
	bool m_bIsSnowEnabled = false;
	bool m_bIsRecallImpossible = false;
	bool m_bIsApocalypseMap = false;
	bool m_bIsHeldenianMap = false;
};