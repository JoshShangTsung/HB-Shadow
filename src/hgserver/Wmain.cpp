#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <winbase.h>
#include <mmsystem.h>
#include <time.h>				   
#include "winmain.h"
#include "Game.h"
#include "UserMessages.h"
#include "resource.h"
#include <cstring>

void PutAdminLogFileList(const char * cStr);
void PutHackLogFileList(const char * cStr);
void PutPvPLogFileList(const char * cStr);

#define WM_USER_TIMERSIGNAL		WM_USER + 500

char szAppClass[32];
HWND G_hWnd = NULL;
char G_cMsgList[120 * 50];
BOOL G_cMsgUpdated = FALSE;
char G_cTxt[512];
char G_cData50000[50000];
MMRESULT G_mmTimer = 0;


class XSocket * G_pListenSock = NULL;
class XSocket * G_pLogSock = NULL;
class CGame * G_pGame = NULL;

int G_iQuitProgramCount = 0;
BOOL G_bIsThread = TRUE;

FILE * pLogFile;

void ThreadProc(void */*ch*/) {
	while (G_bIsThread == TRUE) {
		if (G_pGame != NULL) G_pGame->OnTimer(0);
		Sleep(100);
	}

	ExitThread(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE:
			break;

		case WM_KEYDOWN:
			G_pGame->OnKeyDown(wParam, lParam);
			return (DefWindowProc(hWnd, message, wParam, lParam));
			break;

		case WM_KEYUP:
			G_pGame->OnKeyUp(wParam, lParam);
			return (DefWindowProc(hWnd, message, wParam, lParam));
			break;

		case WM_USER_STARTGAMESIGNAL:
			G_pGame->OnStartGameSignal();
			break;

		case WM_USER_TIMERSIGNAL:
			G_pGame->OnTimer(0);
			break;

		case WM_USER_ACCEPT:
			OnAccept();
			break;

			//case WM_KEYUP:
			//	OnKeyUp(wParam, lParam);
			//	break;

		case WM_PAINT:
			OnPaint();
			break;

		case WM_DESTROY:
			OnDestroy();
			break;

		case WM_CLOSE:
			if (G_pGame->bOnClose() == TRUE) return (DefWindowProc(hWnd, message, wParam, lParam));
			//G_iQuitProgramCount++;
			//if (G_iQuitProgramCount >= 2) {
			//	return (DefWindowProc(hWnd, message, wParam, lParam));
			//}
			break;

		case WM_ONLOGSOCKETEVENT:
			G_pGame->OnMainLogSocketEvent(message, wParam, lParam);
			break;

		default:
			if ((message >= WM_ONLOGSOCKETEVENT + 1) && (message <= WM_ONLOGSOCKETEVENT + DEF_MAXSUBLOGSOCK))
				G_pGame->OnSubLogSocketEvent(message, wParam, lParam);

			if ((message >= WM_ONCLIENTSOCKETEVENT) && (message < WM_ONCLIENTSOCKETEVENT + DEF_MAXCLIENTS))
				G_pGame->OnClientSocketEvent(message, wParam, lParam);

			return (DefWindowProc(hWnd, message, wParam, lParam));
	}

	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
		  LPSTR /*lpCmdLine*/, int nCmdShow) {
	sprintf(szAppClass, "GameServer%d", (int) hInstance);
	if (!InitApplication(hInstance)) return (FALSE);
	if (!InitInstance(hInstance, nCmdShow)) return (FALSE);

	Initialize();
	EventLoop();
	return 0;
}

BOOL InitApplication(HINSTANCE hInstance) {
	WNDCLASS wc;

	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof (int);
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppClass;

	return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	char cTitle[100];
	// HANDLE hFile;
	SYSTEMTIME SysTime;

	GetLocalTime(&SysTime);
	//wsprintf(cTitle, "Helbreath Korean Test Server Version %s.%s %d (Executed at: %d %d %d)", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_BUILDDATE, SysTime.wMonth, SysTime.wDay, SysTime.wHour);
	wsprintf(cTitle, "Revolution Server HG: V%s.%s %d (Executed at: %d %d %d)", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_BUILDDATE, SysTime.wMonth, SysTime.wDay, SysTime.wHour);

	G_hWnd = CreateWindowEx(0, // WS_EX_TOPMOST,
			  szAppClass,
			  cTitle,
			  WS_VISIBLE | // so we don't have to call ShowWindow
			  //WS_POPUP |   // non-app window
			  //WS_CAPTION | // so our menu doesn't look ultra-goofy
			  WS_SYSMENU | // so we get an icon in the tray
			  WS_MINIMIZEBOX,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  800, //GetSystemMetrics(SM_CXSCREEN),
			  600, //GetSystemMetrics(SM_CYSCREEN),
			  NULL,
			  NULL,
			  hInstance,
			  NULL);

	if (!G_hWnd) return (FALSE);

	ShowWindow(G_hWnd, nCmdShow);
	UpdateWindow(G_hWnd);

	return (TRUE);
}

int EventLoop() {
	MSG msg;
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&msg, NULL, 0, 0)) {
				return msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			UpdateScreen();
		} else WaitMessage();
	}
}

void Initialize() {

	if (_InitWinsock() == FALSE) {
		MessageBox(G_hWnd, "Socket 1.1 not found! Cannot execute program.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}

	G_pGame = new class CGame(G_hWnd);
	if (G_pGame->bInit() == FALSE) {
		PutLogList("(!!!) STOPPED!");
		return;
	}

	// ���� ����� Ÿ�̸� 
	G_mmTimer = _StartTimer(300);

	G_pListenSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	if (G_pGame->m_iGameServerMode == 1) {
		G_pListenSock->bListen(G_pGame->m_cGameServerAddrInternal, G_pGame->m_iGameServerPort, WM_USER_ACCEPT);
	}
	if (G_pGame->m_iGameServerMode == 2) {
		G_pListenSock->bListen(G_pGame->m_cGameServerAddr, G_pGame->m_iGameServerPort, WM_USER_ACCEPT);
	}
	pLogFile = NULL;
	//pLogFile = fopen("test.log","wt+");
}

void OnDestroy() {
	if (G_pListenSock != NULL) delete G_pListenSock;
	if (G_pLogSock != NULL) delete G_pLogSock;

	if (G_pGame != NULL) {
		G_pGame->Quit();
		delete G_pGame;
	}

	if (G_mmTimer != 0) _StopTimer(G_mmTimer);
	_TermWinsock();

	if (pLogFile != NULL) fclose(pLogFile);

	PostQuitMessage(0);
}

void PutLogList(const char * cMsg) {
	char cTemp[120 * 50];

	G_cMsgUpdated = TRUE;
	std::memset(cTemp, 0, sizeof(cTemp));
	memcpy((cTemp + 120), G_cMsgList, 120 * 49);
	memcpy(cTemp, cMsg, strlen(cMsg));
	memcpy(G_cMsgList, cTemp, 120 * 50);
	PutAdminLogFileList(cMsg);
}

void PutXSocketLogList(const char * cMsg) {
	PutXSocketLogFileList(cMsg);

}

void UpdateScreen() {
	if (G_cMsgUpdated == TRUE) {
		InvalidateRect(G_hWnd, NULL, TRUE);
		G_cMsgUpdated = FALSE;
	}
}

void OnPaint() {
	HDC hdc;
	PAINTSTRUCT ps;
	register short i;
	char * cMsg;

	hdc = BeginPaint(G_hWnd, &ps);

	SetBkMode(hdc, TRANSPARENT);

	for (i = 0; i < 20; i++) {
		cMsg = (char *) (G_cMsgList + i * 120);
		TextOut(hdc, 5, 5 + 350 - i * 16, cMsg, strlen(cMsg));
	}

	if (G_pGame != NULL)
		G_pGame->DisplayInfo(hdc);

	EndPaint(G_hWnd, &ps);
}

void OnKeyUp(WPARAM /*wParam*/, LPARAM /*lParam*/) {
}

void OnAccept() {
	G_pGame->bAccept(G_pListenSock);
}

void CALLBACK _TimerFunc(UINT wID, UINT /*wUser*/, DWORD /*dwUSer*/, DWORD /*dw1*/, DWORD /*dw2*/) {
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, 0);
}

MMRESULT _StartTimer(DWORD dwTime) {
	TIMECAPS caps;
	MMRESULT timerid;

	timeGetDevCaps(&caps, sizeof (caps));
	timeBeginPeriod(caps.wPeriodMin);
	timerid = timeSetEvent(dwTime, 0, _TimerFunc, 0, (UINT) TIME_PERIODIC);

	return timerid;
}

void _StopTimer(MMRESULT timerid) {
	TIMECAPS caps;

	if (timerid != 0) {
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof (caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}

void PutLogFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	// Original:
	// pFile = fopen("Events.log", "at");
	pFile = fopen("GameLogs\\Events.log", "at");
	if (pFile == NULL) return;
	std::memset(cBuffer, 0, sizeof(cBuffer));
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutAdminLogFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[1024 * 5];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\AdminEvents.log", "at");
	if (pFile == NULL) return;

	std::memset(cBuffer, 0, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutHackLogFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\HackEvents.log", "at");
	if (pFile == NULL) return;

	std::memset(cBuffer, 0, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutPvPLogFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\PvPEvents.log", "at");
	if (pFile == NULL) return;

	std::memset(cBuffer, 0, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutXSocketLogFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\XSocket.log", "at");
	if (pFile == NULL) return;

	std::memset(cBuffer, 0, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutItemLogFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\ItemEvents.log", "at");
	if (pFile == NULL) return;

	std::memset(cBuffer, 0, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutLogEventFileList(const char * cStr) {
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\LogEvents.log", "at");
	if (pFile == NULL) return;

	std::memset(cBuffer, 0, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}