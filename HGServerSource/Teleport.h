// Teleport.h: interface for the CTeleport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELEPORT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_TELEPORT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTeleport  
{
public:
	CTeleport(); 
	virtual ~CTeleport();

	char  m_cTeleportNum;
	char  m_cTeleportNpcName[21];								// The npc with teleport ability.
	char  m_cSourceMap[10];										// The map the teleport npc is located in.
	char  m_cTargetMap[10];											// The map the player will be teleported to.
	short m_sDestinationX, m_sDestinationY;						// The destination coordinates on the map.
	short m_sTeleportCost;										// The amount of gold paid to teleport.
	short m_sTeleportMinLevel, m_sTeleportMaxLevel;				// The required level range for the player.
	char  m_cTeleportSide[7];										// The side the player must be for a successful teleport.
			// both
			// elvine
			// aresden
	BOOL m_bTeleportHunt, m_bTeleportNtrl, m_bTeleportCrmnl;	// Check if player is a Civilian, Neutral, or Criminal.
};

#endif // AFX_TELEPORT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_
