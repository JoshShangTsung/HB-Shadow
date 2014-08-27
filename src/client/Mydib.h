#pragma once
#include <windows.h>
#include <winbase.h>
#include <cstdint>

class CMyDib {
public:
	CMyDib(char *szFilename, unsigned long dwFilePointer);
	~CMyDib();
	void PaintImage(HDC hDC);
	uint16_t m_wWidthX;
	uint16_t m_wWidthY;
	uint16_t m_wColorNums; //bmp
	LPSTR m_lpDib;
	LPBITMAPINFO m_bmpInfo; //bmp
};