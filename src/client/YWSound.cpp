#include <windows.h>
#include "dsound.h"
#include "YWSound.h"

YWSound::YWSound() {
	m_lpDS = 0;
}

YWSound::~YWSound() {
	if (m_lpDS) m_lpDS->Release();
}

bool YWSound::Create(HWND hWnd) {
	HRESULT rval;
	LPDIRECTSOUNDBUFFER lpDsb;
	DSBUFFERDESC dsbdesc;
	WAVEFORMATEX wfm;

	rval = DirectSoundCreate(0, &m_lpDS, 0);
	if (rval != DS_OK) {
		OutputDebugString("DirectSoundCreate error...\n");
		return false;
	}

	rval = m_lpDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (rval != DS_OK) {
		OutputDebugString("DirectSoundCreate error...\n");
		return false;
	}

	memset(&dsbdesc, 0, sizeof (DSBUFFERDESC));
	dsbdesc.dwSize = sizeof (DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = 0;

	memset(&wfm, 0, sizeof (WAVEFORMATEX));
	wfm.wFormatTag = WAVE_FORMAT_PCM;
	wfm.nChannels = 2;
	wfm.nSamplesPerSec = 44100;
	wfm.wBitsPerSample = 16;
	wfm.nBlockAlign = wfm.wBitsPerSample / 8 * wfm.nChannels;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;

	rval = m_lpDS->CreateSoundBuffer(&dsbdesc, &lpDsb, 0);
	if (rval != DS_OK) return false;

	lpDsb->SetFormat(&wfm);

	rval = m_lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
	if (rval != DS_OK) {
		OutputDebugString("DirectSoundCreate error...\n");
		return false;
	}

	return true;
}
