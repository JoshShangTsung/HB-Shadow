#pragma once
#include "dsound.h"

#define DEF_MAXSOUNDBUFFERS		2	

class CSoundBuffer {
public:
	void _ReleaseSoundBuffer();
	void bStop(BOOL bIsNoRewind = FALSE);
	void SetVolume(LONG Volume);
	LPDIRECTSOUNDBUFFER GetIdleBuffer();
	BOOL Play(BOOL bLoop = FALSE, long lPan = 0, int iVol = 0);
	BOOL _LoadWavContents(char cBufferIndex, FILE* pFile, DWORD dwSize, DWORD dwPos);
	BOOL bCreateBuffer_LoadWavFileContents(char cBufferIndex);
	BOOL _bCreateSoundBuffer(char cBufferIndex, DWORD dwBufSize, DWORD dwFreq, DWORD dwBitsPerSample, DWORD dwBlkAlign, BOOL bStereo);
	CSoundBuffer(LPDIRECTSOUND lpDS, DSCAPS DSCaps, char * pWavFileName, BOOL bIsSingleLoad = FALSE);
	virtual ~CSoundBuffer();

	LPDIRECTSOUND m_lpDS;
	DSCAPS m_DSCaps;

	char m_cWavFileName[32];
	LPDIRECTSOUNDBUFFER m_lpDSB[DEF_MAXSOUNDBUFFERS];
	char m_cCurrentBufferIndex;

	BOOL m_bIsSingleLoad;
	BOOL m_bIsLooping;
	DWORD m_dwTime;
};
