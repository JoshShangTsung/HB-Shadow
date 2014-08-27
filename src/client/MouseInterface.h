#pragma once
#include <windows.h>
#include <time.h>
#include <cstdint>

#define DEF_MAXRECTS	30
#define DEF_MIRESULT_NONE		0
#define DEF_MIRESULT_PRESS		1
#define DEF_MIRESULT_CLICK		2

class CMouseInterface {
public:
	int iGetStatus(int msX, int msY, char cLB, char * pResult);
	void AddRect(long sx, long sy, long dx, long dy);
	CMouseInterface();
	virtual ~CMouseInterface();
	RECT * m_pRect[DEF_MAXRECTS];
	char m_cPrevPress;
	uint32_t m_dwTime;
};
