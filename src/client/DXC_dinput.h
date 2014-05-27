#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>

class DXC_dinput {
public:
	DXC_dinput();
	virtual ~DXC_dinput();
	void UpdateMouseState(short * pX, short * pY, short * pZ, char * pLB, char * pRB);
	void SetAcquire(BOOL bFlag);
	BOOL bInit(HWND hWnd, HINSTANCE hInst);

	DIMOUSESTATE dims;
	IDirectInput * m_pDI;
	IDirectInputDevice * m_pMouse;
	short m_sX, m_sY, m_sZ;
};