#pragma once
#include <windows.h>

class CDynamicObject {
public:
	CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwRegisterTime, DWORD dwLastTime, int iV1);

	short m_sOwner;
	char m_cOwnerType;

	short m_sType;
	char m_cMapIndex;
	short m_sX, m_sY;
	DWORD m_dwRegisterTime;
	DWORD m_dwLastTime;

	int m_iCount; 
	int m_iV1; 
};
