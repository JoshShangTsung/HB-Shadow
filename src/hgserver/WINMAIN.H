#pragma once
#include <stdint.h>

void PutXSocketLogList(const char * cMsg);
void PutXSocketLogFileList(const char * cMsg);
void PutLogList(const char * cMsg);
void PutLogFileList(const char * cStr);
void PutAdminLogFileList(const char * cStr);
void PutItemLogFileList(const char * cStr);
void PutLogEventFileList(const char * cStr);
void PutHackLogFileList(const char * cStr);
void PutPvPLogFileList(const char * cStr);

bool InitApplication(HINSTANCE hInstance);
bool InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void OnAccept();
void OnKeyUp(WPARAM wParam, LPARAM lParam);
void UpdateScreen();
int EventLoop();
void OnPaint();
void OnDestroy();
void Initialize();
void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2);
MMRESULT _StartTimer(DWORD dwTime);
void _StopTimer(MMRESULT timerid);
