#include "DXC_dinput.h"

DXC_dinput::DXC_dinput() {
	m_pDI = 0;
	m_pMouse = 0;
	m_sX = 0;
	m_sY = 0;
	m_sZ = 0;
}

DXC_dinput::~DXC_dinput() {
	if (m_pMouse != 0) {
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = 0;
	}
	if (m_pDI != 0) {
		m_pDI->Release();
		m_pDI = 0;
	}
}

bool DXC_dinput::bInit(HWND hWnd, HINSTANCE hInst) {
	HRESULT hr;
	DIMOUSESTATE dims;
	POINT Point;

	GetCursorPos(&Point);
	m_sX = (short) (Point.x);
	m_sY = (short) (Point.y);

	hr = DirectInputCreate(hInst, DIRECTINPUT_VERSION, &m_pDI, 0);
	if (hr != DI_OK) return false;
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, 0);
	if (hr != DI_OK) return false;
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (hr != DI_OK) return false;
	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (hr != DI_OK) return false;

	//	m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	if (m_pMouse->GetDeviceState(sizeof (DIMOUSESTATE), &dims) != DI_OK) {
		m_pMouse->Acquire();
		//return true;
	}

	return true;
}

void DXC_dinput::SetAcquire(bool bFlag) {
	DIMOUSESTATE dims;

	if (m_pMouse == 0) return;
	if (bFlag == true) {
		m_pMouse->Acquire();
		m_pMouse->GetDeviceState(sizeof (DIMOUSESTATE), &dims);
	} else m_pMouse->Unacquire();
}

void DXC_dinput::UpdateMouseState(short * pX, short * pY, short * pZ, char * pLB, char * pRB) {
	if (m_pMouse->GetDeviceState(sizeof (DIMOUSESTATE), &dims) != DI_OK) {
		m_pMouse->Acquire();
		return;
	}
	m_sX += (short) dims.lX;
	m_sY += (short) dims.lY;
	if ((short) dims.lZ != 0)m_sZ = (short) dims.lZ;
	if (m_sX < 0) m_sX = 0;
	if (m_sY < 0) m_sY = 0;
	if (m_sX > 639) m_sX = 639;
	if (m_sY > 479) m_sY = 479;
	*pX = m_sX;
	*pY = m_sY;
	*pZ = m_sZ;
	*pLB = (char) dims.rgbButtons[0];
	*pRB = (char) dims.rgbButtons[1];
}