#pragma once
#include "dsound.h"
#include <cstdint>
#define DEF_MAXSOUNDBUFFERS		2	

class CSoundBuffer {
public:
	void _ReleaseSoundBuffer();
	void bStop(bool bIsNoRewind = false);
	void SetVolume(LONG Volume);
	LPDIRECTSOUNDBUFFER GetIdleBuffer();
	bool Play(bool bLoop = false, long lPan = 0, int iVol = 0);
	bool _LoadWavContents(char cBufferIndex, FILE* pFile, uint32_t dwSize, uint32_t dwPos);
	bool bCreateBuffer_LoadWavFileContents(char cBufferIndex);
	bool _bCreateSoundBuffer(char cBufferIndex, uint32_t dwBufSize, uint32_t dwFreq, uint32_t dwBitsPerSample, uint32_t dwBlkAlign, bool bStereo);
	CSoundBuffer(LPDIRECTSOUND lpDS, DSCAPS DSCaps, char * pWavFileName, bool bIsSingleLoad = false);
	virtual ~CSoundBuffer();

	LPDIRECTSOUND m_lpDS;
	DSCAPS m_DSCaps;

	char m_cWavFileName[32];
	LPDIRECTSOUNDBUFFER m_lpDSB[DEF_MAXSOUNDBUFFERS];
	char m_cCurrentBufferIndex;

	bool m_bIsSingleLoad;
	bool m_bIsLooping;
	uint32_t m_dwTime;
};
