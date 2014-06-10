#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class CMisc {
public:
	void Temp();
	BOOL bCheckValidName(char *pStr);
	BOOL bDecode(char cKey, char *pStr);
	BOOL bEncode(char cKey, char *pStr);
	void GetDirPoint(char cDir, int * pX, int * pY);
	void GetPoint2(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount);
	void GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError);
	char cGetNextMoveDir(short sX, short sY, short dX, short dY);
	void GetMyCursorPos(short * pX, short * pY);
};
