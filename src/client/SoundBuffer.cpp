#include <windows.h>
#include <stdio.h>
#include "dsound.h"
#include "SoundBuffer.h"

struct Waveheader {
	BYTE RIFF[4]; // "RIFF"
	DWORD dwSize; // Size of data to follow
	BYTE WAVE[4]; // "WAVE"
	BYTE fmt_[4]; // "fmt "
	DWORD dw16; // 16
	WORD wOne_0; // 1
	WORD wChnls; // Number of Channels
	DWORD dwSRate; // Sample Rate
	DWORD BytesPerSec; // Sample Rate
	WORD wBlkAlign; // 1
	WORD BitsPerSample; // Sample size
	BYTE DATA[4]; // "DATA"
	DWORD dwDSize; // Number of Samples
};

CSoundBuffer::CSoundBuffer(LPDIRECTSOUND lpDS, DSCAPS DSCaps, char * pWavFileName, bool bIsSingleLoad) {
	int i;

	m_lpDS = lpDS;
	m_DSCaps = DSCaps;

	ZeroMemory(m_cWavFileName, sizeof (m_cWavFileName));
	strcpy(m_cWavFileName, pWavFileName);

	if (bIsSingleLoad == true) {
		//m_lpDSB[0] = 0;
		//bCreateBuffer_LoadWavFileContents(0);
	} else {
		/*
		for (i = 0; i < DEF_MAXSOUNDBUFFERS; i++) {
			m_lpDSB[i] = 0;
			bCreateBuffer_LoadWavFileContents(i);
		}
		 */
	}

	for (i = 0; i < DEF_MAXSOUNDBUFFERS; i++) m_lpDSB[i] = 0;

	m_cCurrentBufferIndex = 0;
	m_bIsSingleLoad = bIsSingleLoad;

	m_dwTime = 0;

	m_bIsLooping = false;
}

CSoundBuffer::~CSoundBuffer() {
	for (int i = 0; i < DEF_MAXSOUNDBUFFERS; i++) {
		if (m_lpDSB[i] != 0) {
			m_lpDSB[i]->Release();
			m_lpDSB[i] = 0;
		}
	}
}

bool CSoundBuffer::_bCreateSoundBuffer(char cBufferIndex, DWORD dwBufSize, DWORD dwFreq, DWORD dwBitsPerSample, DWORD dwBlkAlign, bool bStereo) {
	PCMWAVEFORMAT pcmwf;
	DSBUFFERDESC dsbdesc;
	if (m_lpDSB[cBufferIndex] != 0) return false;

	memset(&pcmwf, 0, sizeof (PCMWAVEFORMAT));
	pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels = bStereo ? 2 : 1;
	pcmwf.wf.nSamplesPerSec = dwFreq;
	pcmwf.wf.nBlockAlign = (WORD) dwBlkAlign;
	pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample = (WORD) dwBitsPerSample;

	memset(&dsbdesc, 0, sizeof (DSBUFFERDESC));
	dsbdesc.dwSize = sizeof (DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;
	dsbdesc.dwBufferBytes = dwBufSize;
	dsbdesc.lpwfxFormat = (LPWAVEFORMATEX) & pcmwf;

	if (m_lpDS->CreateSoundBuffer(&dsbdesc, &m_lpDSB[cBufferIndex], 0) != DS_OK) return false;

	return true;
}

bool CSoundBuffer::bCreateBuffer_LoadWavFileContents(char cBufferIndex) {
	FILE * pFile;
	Waveheader Wavhdr;
	DWORD dwSize;
	bool bStereo;

	if (m_lpDSB[cBufferIndex] != 0) return false;

	pFile = fopen(m_cWavFileName, "rb");
	if (pFile == 0) return false;

	if (fread(&Wavhdr, sizeof (Wavhdr), 1, pFile) != 1) {
		fclose(pFile);
		return false;
	}

	dwSize = Wavhdr.dwDSize;
	bStereo = Wavhdr.wChnls > 1 ? true : false;

	if (_bCreateSoundBuffer(cBufferIndex, dwSize, Wavhdr.dwSRate, Wavhdr.BitsPerSample, Wavhdr.wBlkAlign, bStereo) == false) {
		fclose(pFile);
		return false;
	}

	if (!_LoadWavContents(cBufferIndex, pFile, dwSize, sizeof (Wavhdr))) {
		fclose(pFile);
		return false;
	}

	fclose(pFile);
	return true;
}

bool CSoundBuffer::_LoadWavContents(char cBufferIndex, FILE * pFile, DWORD dwSize, DWORD dwPos) {
	LPVOID pData1;
	DWORD dwData1Size;
	LPVOID pData2;
	DWORD dwData2Size;
	HRESULT rval;

	if (m_lpDSB[cBufferIndex] == 0) return false;
	if (dwPos == 0xffffffff) return false;
	if (fseek(pFile, dwPos, SEEK_SET) != 0) return false;

	rval = m_lpDSB[cBufferIndex]->Lock(0, dwSize, &pData1, &dwData1Size, &pData2, &dwData2Size, DSBLOCK_ENTIREBUFFER);
	//DSBLOCK_FROMWRITECURSOR); // DSBLOCK_ENTIREBUFFER
	if (rval != DS_OK) return false;

	if (dwData1Size > 0)
		if (fread(pData1, dwData1Size, 1, pFile) != 1)
			return false;

	if (dwData2Size > 0)
		if (fread(pData2, dwData2Size, 1, pFile) != 1)
			return false;

	rval = m_lpDSB[cBufferIndex]->Unlock(pData1, dwData1Size, pData2, dwData2Size);
	if (rval != DS_OK) return false;

	// v1.3
	m_lpDSB[cBufferIndex]->SetFrequency(DSBFREQUENCY_ORIGINAL);

	return true;
}

bool CSoundBuffer::Play(bool bLoop, long lPan, int iVol) {
	HRESULT rval;
	LPDIRECTSOUNDBUFFER Buffer = 0;

	if (m_lpDS == 0) return false;

	Buffer = GetIdleBuffer();
	if (Buffer == 0) return false;

	SetVolume(iVol);

	if (lPan < DSBPAN_LEFT) lPan = DSBPAN_LEFT;
	else if (lPan > DSBPAN_RIGHT) lPan = DSBPAN_RIGHT;
	Buffer->SetPan(lPan);

	m_bIsLooping = bLoop;

	if (bLoop == false)
		rval = Buffer->Play(0, 0, 0);
	else rval = Buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (rval != DS_OK) return false;

	return true;
}

LPDIRECTSOUNDBUFFER CSoundBuffer::GetIdleBuffer(void) {
	DWORD Status;
	HRESULT rval;
	LPDIRECTSOUNDBUFFER Buffer;

	Buffer = 0;
	if (m_lpDSB[m_cCurrentBufferIndex] != 0) {
		rval = m_lpDSB[m_cCurrentBufferIndex]->GetStatus(&Status);
		if (rval < 0) Status = 0;

		if (Status & DSBSTATUS_BUFFERLOST) {
			m_lpDSB[m_cCurrentBufferIndex]->Release();
			m_lpDSB[m_cCurrentBufferIndex] = 0;
			bCreateBuffer_LoadWavFileContents(m_cCurrentBufferIndex);
		} else if ((Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING) {
			if (m_bIsSingleLoad == true) {
				m_lpDSB[m_cCurrentBufferIndex]->Stop();
				m_lpDSB[m_cCurrentBufferIndex]->SetCurrentPosition(0);
				Buffer = m_lpDSB[m_cCurrentBufferIndex];
				m_dwTime = timeGetTime();
				return Buffer;
			}

			m_cCurrentBufferIndex++;
			if (m_cCurrentBufferIndex >= DEF_MAXSOUNDBUFFERS) m_cCurrentBufferIndex = 0;

			if (m_lpDSB[m_cCurrentBufferIndex] != 0) {
				rval = m_lpDSB[m_cCurrentBufferIndex]->GetStatus(&Status);
				if (rval < 0) Status = 0;
				if (Status & DSBSTATUS_BUFFERLOST) {
					m_lpDSB[m_cCurrentBufferIndex]->Release();
					m_lpDSB[m_cCurrentBufferIndex] = 0;
					bCreateBuffer_LoadWavFileContents(m_cCurrentBufferIndex);
				} else if ((Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING) {
					m_lpDSB[m_cCurrentBufferIndex]->Stop();
					m_lpDSB[m_cCurrentBufferIndex]->SetCurrentPosition(0);
				}
			} else {
				bCreateBuffer_LoadWavFileContents(m_cCurrentBufferIndex);
			}

			//m_lpDSB[m_cCurrentBufferIndex]->Stop();
			//m_lpDSB[m_cCurrentBufferIndex]->SetCurrentPosition(0);
		}

		Buffer = m_lpDSB[m_cCurrentBufferIndex];
	} else {
		bCreateBuffer_LoadWavFileContents(m_cCurrentBufferIndex);
		Buffer = m_lpDSB[m_cCurrentBufferIndex];
	}

	m_dwTime = timeGetTime();
	return Buffer;
}

void CSoundBuffer::SetVolume(LONG Volume) {
	int i;

	for (i = 0; i < DEF_MAXSOUNDBUFFERS; i++)
		if (m_lpDSB[i] != 0) m_lpDSB[i]->SetVolume(Volume);
}

void CSoundBuffer::bStop(bool bIsNoRewind) {
	for (int i = 0; i < DEF_MAXSOUNDBUFFERS; i++) {
		if (m_lpDSB[i] != 0) {
			if (m_lpDSB[i]->Stop() != DS_OK) return;
			if (bIsNoRewind == false) m_lpDSB[i]->SetCurrentPosition(0);
		}
	}
}

void CSoundBuffer::_ReleaseSoundBuffer() {
	int i;

	for (i = 0; i < DEF_MAXSOUNDBUFFERS; i++)
		if (m_lpDSB[i] != 0) {
			m_lpDSB[i]->Release();
			m_lpDSB[i] = 0;
		}
}
