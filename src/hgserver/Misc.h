#pragma once

class CMisc {
public:
	void Temp();
	bool bCheckValidName(char *pStr);
	bool bDecode(char cKey, char *pStr);
	bool bEncode(char cKey, char *pStr);
	void GetDirPoint(char cDir, int * pX, int * pY);
	void GetPoint2(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount);
	void GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError);
	char cGetNextMoveDir(short sX, short sY, short dX, short dY);
	void GetMyCursorPos(short * pX, short * pY);
};
