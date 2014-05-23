// -------------------------------------------------------------- 
//                      New Game Server  						  
//
//                      1998.11 by Soph
//
// --------------------------------------------------------------







// --------------------------------------------------------------


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

void PutAdminLogFileList(char * cStr);
void PutHackLogFileList(char * cStr);
void PutPvPLogFileList(char * cStr);

// --------------------------------------------------------------

#define WM_USER_TIMERSIGNAL		WM_USER + 500

char			szAppClass[32];
HWND			G_hWnd = NULL;
char			G_cMsgList[120*50];
BOOL            G_cMsgUpdated =	FALSE;
char            G_cTxt[512];
char			G_cData50000[50000];
MMRESULT        G_mmTimer = NULL;


class XSocket * G_pListenSock = NULL;
class XSocket * G_pLogSock    = NULL;
class CGame *   G_pGame       = NULL;

int             G_iQuitProgramCount = 0;
BOOL			G_bIsThread = TRUE;

FILE * pLogFile;

//char			G_cCrashTxt[50000];
// --------------------------------------------------------------

void ThreadProc(void *ch)
{
	while (G_bIsThread == TRUE) {
		if (G_pGame = NULL)	G_pGame->OnTimer(NULL);
		Sleep(100);
	}

	ExitThread(0);
}

LRESULT CALLBACK WndProc( HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{ 
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
		G_pGame->OnTimer(NULL);
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

	case WM_ONGATESOCKETEVENT:
		G_pGame->OnGateSocketEvent(message, wParam, lParam);
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
	
	return NULL;
}

/*void GetOSName(){
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return;
	}

	//OS Info
	strcat(G_cCrashTxt, "System Information\r\n");
	strcat(G_cCrashTxt, "Operating System : ");

	switch (osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:

		// Test for the specific product family.
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			strcat(G_cCrashTxt,"Microsoft Windows Server 2003 family, ");

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			strcat(G_cCrashTxt,"Microsoft Windows XP ");

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			strcat(G_cCrashTxt,"Microsoft Windows 2000 ");

		if ( osvi.dwMajorVersion <= 4 )
			strcat(G_cCrashTxt,"Microsoft Windows NT ");

		// Test for specific product on Windows NT 4.0 SP6 and later.
		if( bOsVersionInfoEx )
		{
			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				if( osvi.dwMajorVersion == 4 )
					strcat(G_cCrashTxt, "Workstation 4.0 " );
				else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
					strcat(G_cCrashTxt, "Home Edition " );
				else
					strcat(G_cCrashTxt, "Professional " );
			}

			// Test for the server type.
			else if ( osvi.wProductType == VER_NT_SERVER || 
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER )
			{
				if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						strcat(G_cCrashTxt, "Datacenter Edition " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						strcat(G_cCrashTxt, "Enterprise Edition " );
					else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
						strcat(G_cCrashTxt, "Web Edition " );
					else
						strcat(G_cCrashTxt, "Standard Edition " );
				}

				else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						strcat(G_cCrashTxt, "Datacenter Server " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						strcat(G_cCrashTxt, "Advanced Server " );
					else
						strcat(G_cCrashTxt, "Server " );
				}

				else  // Windows NT 4.0 
				{
					if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						strcat(G_cCrashTxt,"Server 4.0, Enterprise Edition " );
					else
						strcat(G_cCrashTxt, "Server 4.0 " );
				}
			}
		}
		else  // Test for specific product on Windows NT 4.0 SP5 and earlier
		{
			#define BUFSIZE 80
			HKEY hKey;
			char szProductType[BUFSIZE];
			DWORD dwBufLen=BUFSIZE;
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				"SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
				0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS )
				return;

			lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
				(LPBYTE) szProductType, &dwBufLen);
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
				return;

			RegCloseKey( hKey );

			if ( lstrcmpi( "WINNT", szProductType) == 0 )
				printf( "Workstation " );
			if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
				printf( "Server " );
			if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
				printf( "Advanced Server " );

			wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
		}

		// Display service pack (if any) and build number.

		if( osvi.dwMajorVersion == 4 && 
			lstrcmpi( osvi.szCSDVersion, "Service Pack 6" ) == 0 )
		{
			HKEY hKey;
			LONG lRet;

			// Test for SP6 versus SP6a.
			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
				0, KEY_QUERY_VALUE, &hKey );
			if( lRet == ERROR_SUCCESS )
				wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "Service Pack 6a (Build %d)\r\n", osvi.dwBuildNumber & 0xFFFF );         
			else // Windows NT 4.0 prior to SP6a
			{
				wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "%s (Build %d)\r\n",
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);
			}

			RegCloseKey( hKey );
		}
		else // Windows NT 3.51 and earlier or Windows 2000 and later
		{
			wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "%s (Build %d)\r\n",
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}


		break;

		// Test for the Windows 95 product family.
	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			strcat(G_cCrashTxt,"Microsoft Windows 95 ");
			if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				strcat(G_cCrashTxt,"OSR2 " );
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			strcat(G_cCrashTxt,"Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1] == 'A' )
				strcat(G_cCrashTxt,"SE " );
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			strcat(G_cCrashTxt,"Microsoft Windows Millennium Edition\r\n");
		} 
		break;

	case VER_PLATFORM_WIN32s:

		strcat(G_cCrashTxt,"Microsoft Win32s\r\n");
		break;
	}
}*/

/*BOOL CALLBACK lpCrashDialogFunc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){
HANDLE outHand;
char cCrashFileName[MAX_PATH];
char cLF[]={0x0d,0x0a};
char cDash ='-';
SYSTEMTIME sysTime;
DWORD written;

	switch(uMsg) {
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDC_CLOSE:
			EndDialog(hDlg, TRUE);
			break;
		}
		break;

	case WM_INITDIALOG:
		//Show Crash Data
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), G_cCrashTxt);
		GetLocalTime(&sysTime);
		wsprintf(cCrashFileName,"CrashData - %d-%d-%d.txt", sysTime.wDay, sysTime.wMonth, sysTime.wYear);
		SetWindowText(GetDlgItem(hDlg, IDC_EDITPATH), cCrashFileName);
		//Open File For Writing
		outHand = CreateFile(cCrashFileName,GENERIC_READ+GENERIC_WRITE,FILE_SHARE_READ+FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		SetFilePointer(outHand, 0, 0, FILE_END);
		WriteFile(outHand, G_cCrashTxt, strlen(G_cCrashTxt), &written, NULL);
		for (int i = 0; i < 80; i++)
			WriteFile(outHand, &cDash, 1, &written, NULL);
		WriteFile(outHand, cLF, 2, &written, NULL);
		WriteFile(outHand, cLF, 2, &written, NULL);
		CloseHandle(outHand);
		break;
//	default:
//		return 0;
	}
	return 0;
}*/

/*LONG lpTopLevelExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo){

	//Shutdown everything
	G_bIsThread = FALSE;
	_StopTimer(G_mmTimer);

	try{
		delete G_pGame;
		G_pGame = NULL;
	}
	catch (...) {
	}

	ZeroMemory(G_cCrashTxt, sizeof(G_cCrashTxt));

	//Format a nice output

	//Reason for crash
	strcpy(G_cCrashTxt, "HGServer Exception Information\r\n");
	strcat(G_cCrashTxt, "Code : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
	strcat(G_cCrashTxt, "Flags : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n", ExceptionInfo->ExceptionRecord->ExceptionFlags);
	strcat(G_cCrashTxt, "Address : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
	strcat(G_cCrashTxt, "Parameters : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n\r\n", ExceptionInfo->ExceptionRecord->NumberParameters);

	//Retrieve OS version
	GetOSName();
	strcat(G_cCrashTxt, "\r\n");

	//Crash Details
	strcat(G_cCrashTxt, "Context :\r\n");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EDI: 0x%.8X\t\tESI: 0x%.8X\t\tEAX: 0x%.8X\r\n",ExceptionInfo->ContextRecord->Edi,
																						ExceptionInfo->ContextRecord->Esi,
																						ExceptionInfo->ContextRecord->Eax);
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EBX: 0x%.8X\t\tECX: 0x%.8X\t\tEDX: 0x%.8X\r\n",ExceptionInfo->ContextRecord->Ebx,
																						ExceptionInfo->ContextRecord->Ecx,
																						ExceptionInfo->ContextRecord->Edx);
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EIP: 0x%.8X\t\tEBP: 0x%.8X\t\tSegCs: 0x%.8X\r\n",ExceptionInfo->ContextRecord->Eip,
																						ExceptionInfo->ContextRecord->Ebp,
																						ExceptionInfo->ContextRecord->SegCs);
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EFlags: 0x%.8X\tESP: 0x%.8X\t\tSegSs: 0x%.8X\r\n",ExceptionInfo->ContextRecord->EFlags,
																						ExceptionInfo->ContextRecord->Esp,
																						ExceptionInfo->ContextRecord->SegSs);
	// Show Dialog
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)lpCrashDialogFunc);
	SendMessage(0, WM_CLOSE, NULL, NULL);
	return EXCEPTION_EXECUTE_HANDLER;
}*/

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
               LPSTR lpCmdLine, int nCmdShow )
{
	// Install SEH
	// SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)lpTopLevelExceptionFilter);
	sprintf( szAppClass, "GameServer%d", hInstance);
	if (!InitApplication( hInstance))		return (FALSE);
    if (!InitInstance(hInstance, nCmdShow)) return (FALSE);
	
	Initialize();
	EventLoop();
    return 0;
}
  


BOOL InitApplication( HINSTANCE hInstance)
{     
 WNDCLASS  wc;

	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WndProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);             
	wc.hInstance     = hInstance;                    
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);  
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);	 
	wc.lpszMenuName  = NULL;                    		 
	wc.lpszClassName = szAppClass;                   
        
	return (RegisterClass(&wc));
}


BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
 char cTitle[100];
// HANDLE hFile;
 SYSTEMTIME SysTime;
	// 서버 부팅시간 기록 
	
	//hFile = hFile = CreateFile("HGserver.exe", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	//if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	//GetFileTime(hFile, &ftCT, &ftLAT, &ftLWT);
	//CloseHandle(hFile);

	GetLocalTime(&SysTime);
	//wsprintf(cTitle, "Helbreath Korean Test Server Version %s.%s %d (Executed at: %d %d %d)", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_BUILDDATE, SysTime.wMonth, SysTime.wDay, SysTime.wHour);
	wsprintf(cTitle, "Revolution Server HG: V%s.%s %d (Executed at: %d %d %d)", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_BUILDDATE, SysTime.wMonth, SysTime.wDay, SysTime.wHour);
	
	G_hWnd = CreateWindowEx(0,  // WS_EX_TOPMOST,
        szAppClass,
        cTitle,
        WS_VISIBLE | // so we don't have to call ShowWindow
        //WS_POPUP |   // non-app window
        //WS_CAPTION | // so our menu doesn't look ultra-goofy
        WS_SYSMENU |  // so we get an icon in the tray
        WS_MINIMIZEBOX, 
		CW_USEDEFAULT,
        CW_USEDEFAULT,
        800, //GetSystemMetrics(SM_CXSCREEN),
        600, //GetSystemMetrics(SM_CYSCREEN),
        NULL,
        NULL,
        hInstance,
        NULL );

    if (!G_hWnd) return (FALSE);
    
	ShowWindow(G_hWnd, nCmdShow);    
	UpdateWindow(G_hWnd);            

	return (TRUE);                 
}



int EventLoop()
{
 static unsigned short _usCnt = 0; 
 register MSG msg;

	while( 1 ) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			if( !GetMessage( &msg, NULL, 0, 0 ) ) {
				return msg.wParam;
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
			UpdateScreen();
		}
		else WaitMessage();
	}
}



void Initialize()
{

	if (_InitWinsock() == FALSE) {
		MessageBox(G_hWnd, "Socket 1.1 not found! Cannot execute program.","ERROR", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}

	G_pGame = new class CGame(G_hWnd);
	if (G_pGame->bInit() == FALSE) {
		PutLogList("(!!!) STOPPED!");
		return;
	}

	// 게임 진행용 타이머 
	G_mmTimer = _StartTimer(300);

	G_pListenSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	if (G_pGame->m_iGameServerMode == 1)
	{
		G_pListenSock->bListen(G_pGame->m_cGameServerAddrInternal, G_pGame->m_iGameServerPort, WM_USER_ACCEPT);
	}
	if (G_pGame->m_iGameServerMode == 2)
	{
		G_pListenSock->bListen(G_pGame->m_cGameServerAddr, G_pGame->m_iGameServerPort, WM_USER_ACCEPT);
	}
	pLogFile = NULL;
	//pLogFile = fopen("test.log","wt+");
}

void OnDestroy()
{
	if (G_pListenSock != NULL) delete G_pListenSock;
	if (G_pLogSock != NULL) delete G_pLogSock;

	if (G_pGame != NULL) {
		G_pGame->Quit();
		delete G_pGame;
	}

	if (G_mmTimer != NULL) _StopTimer(G_mmTimer);
	_TermWinsock();

	if (pLogFile != NULL) fclose(pLogFile);

	PostQuitMessage(0);
}


void PutLogList(char * cMsg)
{
 char cTemp[120*50];
	
	G_cMsgUpdated = TRUE;
	ZeroMemory(cTemp, sizeof(cTemp));
	memcpy((cTemp + 120), G_cMsgList, 120*49);
	memcpy(cTemp, cMsg, strlen(cMsg));
	memcpy(G_cMsgList, cTemp, 120*50);
	PutAdminLogFileList(cMsg);
}

void PutXSocketLogList(char * cMsg)
{
// char cTemp[120*50];
	
	//G_cMsgUpdated = TRUE;
	//ZeroMemory(cTemp, sizeof(cTemp));
	//memcpy((cTemp + 120), G_cMsgList, 120*49);
	//memcpy(cTemp, cMsg, strlen(cMsg));
	//memcpy(G_cMsgList, cTemp, 120*50);
	PutXSocketLogFileList(cMsg);

}

void UpdateScreen()
{
	if (G_cMsgUpdated == TRUE) {
		InvalidateRect(G_hWnd, NULL, TRUE);
		G_cMsgUpdated = FALSE;
	}
}


void OnPaint()
{
 HDC hdc;
 PAINTSTRUCT ps;
 register short i;
 char * cMsg;

	hdc = BeginPaint(G_hWnd, &ps);
	
	SetBkMode(hdc,TRANSPARENT);

	for (i = 0; i < 20; i++) {
		cMsg = (char *)(G_cMsgList + i*120);
		TextOut(hdc, 5, 5 + 350 - i*16, cMsg, strlen(cMsg));
	}
	
	if (G_pGame	!= NULL)
		G_pGame->DisplayInfo(hdc);

	EndPaint(G_hWnd, &ps);
}



void  OnKeyUp(WPARAM wParam, LPARAM lParam)
{
}


void OnAccept()
{
	G_pGame->bAccept(G_pListenSock);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}



MMRESULT _StartTimer(DWORD dwTime)
{
 TIMECAPS caps;
 MMRESULT timerid;

	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	timerid = timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);

	return timerid;
}



void _StopTimer(MMRESULT timerid)
{
 TIMECAPS caps;

	if (timerid != 0) {
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}

/*********************************************************************************************************************
**  void PutLogFileList(char * cStr)																				**
**  description			:: writes data into "Events.log"															**
**  last updated		:: November 22, 2004; 5:40 PM; Hypnotoad													**
**	return value		:: void																						**
**********************************************************************************************************************/
void PutLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	// Original:
	// pFile = fopen("Events.log", "at");
	pFile = fopen("GameLogs\\Events.log", "at");
	if (pFile == NULL) return;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutAdminLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\AdminEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutHackLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\HackEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutPvPLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\PvPEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutXSocketLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\XSocket.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutItemLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\ItemEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutLogEventFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\LogEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}