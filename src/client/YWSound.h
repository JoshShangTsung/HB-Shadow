#pragma once

#include "dsound.h"

class YWSound {
public:
	YWSound();
	virtual ~YWSound();
	bool Create(HWND hWnd);
	LPDIRECTSOUND m_lpDS;
	DSCAPS m_DSCaps;
};