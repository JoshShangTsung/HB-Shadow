#pragma once

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "DXC_ddraw.h"
#include "Mydib.h"

typedef struct stBrushtag {
	short sx;
	short sy;
	short szx;
	short szy;
	short pvx;
	short pvy;
} stBrush;

class CSprite {
public:

	void * operator new (size_t size) {
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	};

	void operator delete(void * mem) {
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	};

	CSprite(HANDLE hPakFile, class DXC_ddraw * pDDraw, const char * cPakFileName, short sNthFile, bool bAlphaEffect = true);
	virtual ~CSprite();

	void PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime);

	void PutSpriteFast(int sX, int sY, int sFrame, DWORD dwTime);
	void PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, DWORD dwTime);
	void PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, DWORD dwTime);
	void PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, DWORD dwTime);
	void PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, DWORD dwTime);
	void PutSpriteFastNoColorKey(int sX, int sY, int sFrame, DWORD dwTime);
	void PutSpriteFastDst(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime);
	void PutSpriteFastNoColorKeyDst(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime);

	void PutTransSprite(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite2(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime);
	void PutTransSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime);
	void PutTransSprite70(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite50(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite25(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime);
	void PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime);

	void PutShadowSpriteClip(int sX, int sY, int sFrame, DWORD dwTime);
	void PutShadowSprite(int sX, int sY, int sFrame, DWORD dwTime);

	void PutRevTransSprite(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth = 0);

	void PutFadeSprite(short sX, short sY, short sFrame, DWORD dwTime);
	void PutFadeSpriteDst(WORD * pDstAddr, short sPitch, short sX, short sY, short sFrame, DWORD dwTime);


	void _SetAlphaDegree();
	bool _bCheckCollison(int sX, int sY, short sFrame, int msX, int msY);
	void _GetSpriteRect(int sX, int sY, int sFrame);
	void _iCloseSprite();
	bool _iOpenSprite();
	void iRestore();
	IDirectDrawSurface7 * _pMakeSpriteSurface();

	RECT m_rcBound;
	DWORD m_dwRefTime;
	bool m_bIsSurfaceEmpty;
	bool m_bOnCriticalSection;
	bool m_bAlphaEffect;
	short m_sPivotX, m_sPivotY;
	class DXC_ddraw * m_pDDraw;
	WORD* m_pSurfaceAddr;
	DWORD m_dwBitmapFileStartLoc;
	short m_sPitch;
	int m_iTotalFrame;
	char m_cAlphaDegree;
	WORD m_wBitmapSizeX, m_wBitmapSizeY;
	WORD m_wColorKey;
	char m_cPakFileName[16];
	stBrush* m_stBrush;
	LPDIRECTDRAWSURFACE7 m_lpSurface;
};
