#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

enum {
	CODE_ENG, CODE_HAN1, CODE_HAN2
};

class CMisc {
public:

	int iGetTextLengthLoc(HDC hDC, char * pStr, int iLength);
	int _iGetFileCheckSum(char * pFn);
	BOOL _iConvertFileXor(char *pFn, char *pDestFn, char cKey);
	BOOL bCheckValidName(char *pStr);
	BOOL bCheckIMEString(char *str);
	BOOL bDecode(char cKey, char * pStr);
	BOOL bEncode(char cKey, char * pStr);
	void ColorTransfer(char cPixelFormat, COLORREF fcolor, WORD * wR, WORD * wG, WORD * wB);
	char cCalcDirection(short sX, short sY, short dX, short dY);
	void ReplaceString(char * pStr, char cFrom, char cTo);
	BOOL bCheckValidString(char * str);
	void GetDirPoint(char cDir, int * pX, int * pY);
	void GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount);
	char cGetNextMoveDir(short sX, short sY, short dX, short dY);
	BOOL bIsValidSSN(char * pStr);
	BOOL bIsValidEmail(char * pStr);
};
