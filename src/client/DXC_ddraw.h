#pragma once
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <memory.h>
#include <string.h>
#include "ddraw.h"
#include "Misc.h"
#include "GlobalDef.h"

class DXC_ddraw {
public:

	void * operator new (size_t size) {
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	};

	void operator delete(void * mem) {
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	};

	DXC_ddraw();
	virtual ~DXC_ddraw();
	bool bInit(HWND hWnd);
	void ColorTransferRGB(COLORREF fcolor, int * iR, int * iG, int * iB);
	HRESULT InitFlipToGDI(HWND hWnd);
	void ChangeDisplayMode(HWND hWnd);
	void DrawText(LPRECT pRect, char * pString, COLORREF rgb);
	void _ReleaseBackBufferDC();
	void _GetBackBufferDC();
	void TextOut(int x, int y, char * cStr, COLORREF rgb);
	void _TestPixelFormat();
	DWORD _dwColorMatch(IDirectDrawSurface7 * pdds4, WORD wColorKey);
	DWORD _dwColorMatch(IDirectDrawSurface7 * pdds4, COLORREF rgb);
	long _CalcMinValue(int iS, int iD, char cMode);
	long _CalcMaxValue(int iS, int iD, char cMode, char cMethod, double dAlpha);
	HRESULT iSetColorKey(IDirectDrawSurface7 * pdds4, WORD wColorKey);
	void PutPixel(short sX, short sY, WORD wR, WORD wG, WORD wB);
	void DrawShadowBox(short sX, short sY, short dX, short dY, int iType = 0);
	void ClearBackB4();
	IDirectDrawSurface7 * pCreateOffScreenSurface(WORD iSzX, WORD iSzY);
	HRESULT iSetColorKey(IDirectDrawSurface7 * pdds4, COLORREF rgb);
	HRESULT iFlip();
	bool Screenshot(LPCTSTR FileName, LPDIRECTDRAWSURFACE7 lpDDS);

	long m_lTransG100[64][64], m_lTransRB100[64][64];
	long m_lTransG70[64][64], m_lTransRB70[64][64];
	long m_lTransG50[64][64], m_lTransRB50[64][64];
	long m_lTransG25[64][64], m_lTransRB25[64][64];
	long m_lTransG2[64][64], m_lTransRB2[64][64];
	long m_lFadeG[64][64], m_lFadeRB[64][64];

	bool m_bFullMode;

	LPDIRECTDRAW7 m_lpDD4;
	LPDIRECTDRAWSURFACE7 m_lpFrontB4, m_lpBackB4, m_lpBackB4flip;
	LPDIRECTDRAWSURFACE7 m_lpPDBGS; // Pre-Draw Background Surface
	WORD * m_pBackB4Addr;
	RECT m_rcClipArea, m_rcFlipping;
	short m_sBackB4Pitch;
	char m_cPixelFormat;
	HDC m_hDC;
	HFONT m_hFontInUse;
};