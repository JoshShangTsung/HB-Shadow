#include "DebugDialog.h"
#include <windows.h>
#include "resource.h"

BOOL CALLBACK lpDialogFunc(HWND, UINT, WPARAM, LPARAM);
void DebugWindowThread();
HWND m_DbgWnd;
HWND m_DbgList;
HANDLE outHand;
char crlf[] = {0x0d, 0x0a, 0x0d, 0x0a};

//Constructor

CDebugWindow::CDebugWindow() : m_isVisible(false) {
}

// Startups The Debug Dialog

void CDebugWindow::Startup(void) {
	DWORD lpThreadId;
	//Create a thread for dialog
	m_isVisible = true;
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE) DebugWindowThread, 0, 0, &lpThreadId));
	//Give time for dialog to startup properly
	Sleep(10);
}

void DebugWindowThread() {
	//start dialog
	DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC) lpDialogFunc);
}

BOOL CALLBACK lpDialogFunc(HWND hDlg, UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/) {
	switch (uMsg) {
		case WM_INITDIALOG:
			//Copy HWND's
			m_DbgWnd = hDlg;
			m_DbgList = GetDlgItem(hDlg, IDC_DBGLIST);
			//Open File For Writing
			outHand = CreateFile("PacketData.txt", GENERIC_READ + GENERIC_WRITE, FILE_SHARE_READ + FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			SetEndOfFile(outHand);
			break;
		default:
			return 0;
	}
	return 0;
}

void CDebugWindow::Shutdown(void) {
	//Close Dialog
	EndDialog(m_DbgWnd, true);
	//Close File Handle
	CloseHandle(outHand);
}

void CDebugWindow::AddEventMsg(char* cMsg) {
	DWORD written;
	if (m_isVisible) {
		SendMessage(m_DbgList, LB_ADDSTRING, 0, (LPARAM) cMsg);
		//Highlight Last Active Message
		SendMessage(m_DbgList, LB_SETCURSEL, SendMessage(m_DbgList, LB_GETCOUNT, 0, 0) - 1, 0);
		//Write data to file
		WriteFile(outHand, cMsg, strlen(cMsg), &written, 0);
		WriteFile(outHand, crlf, 4, &written, 0);
	}
}

void CDebugWindow::AddEventMsg(int cMsgType, char* cData, uint32_t dwSize, char cKey) {
	DWORD written;
	char DbgBuffer[10000];

	if (m_isVisible) {
		if (cMsgType == MSG_SEND) strcpy(DbgBuffer, "SEND -> ");
		else strcpy(DbgBuffer, "RECV -> ");

		wsprintf(&DbgBuffer[8], "Size = %lu Key = 0x%.X", dwSize, cKey);
		SendMessage(m_DbgList, LB_ADDSTRING, 0, (LPARAM) DbgBuffer);
		//Write data to file
		WriteFile(outHand, DbgBuffer, strlen(DbgBuffer), &written, 0);
		WriteFile(outHand, crlf, 2, &written, 0);

		uint32_t i = 0;
		while (i < dwSize) {
			memset(DbgBuffer, 0, sizeof (DbgBuffer));
			strcpy(DbgBuffer, "DATA -> ");
			for (uint32_t j = i; j < i + 16 && j < dwSize; j++)
				wsprintf(&DbgBuffer[strlen(DbgBuffer)], "%.2X ", (unsigned char) cData[j]);

			//Align Spacing
			for (int a = strlen(DbgBuffer); a < 56; a += 3)
				strcat(DbgBuffer, "   ");

			strcat(DbgBuffer, "\t\t\t");
			uint32_t j = i;
			for (; j < i + 16 && j < dwSize; j++)
				DbgBuffer[strlen(DbgBuffer)] = isprint((unsigned char) cData[j]) ? cData[j] : '.';

			SendMessage(m_DbgList, LB_ADDSTRING, 0, (LPARAM) DbgBuffer);
			WriteFile(outHand, DbgBuffer, strlen(DbgBuffer), &written, 0);
			WriteFile(outHand, crlf, 2, &written, 0);
			i = j;
		}
		WriteFile(outHand, crlf, 2, &written, 0);

		//Highlight Last Active Message
		SendMessage(m_DbgList, LB_SETCURSEL, SendMessage(m_DbgList, LB_GETCOUNT, 0, 0) - 1, 0);
	}
}

void CDebugWindow::ShowWindow(bool isVisible) {
	Sleep(10);
	if (isVisible) {
		::ShowWindow(m_DbgWnd, SW_SHOW);
		m_isVisible = true;
	} else {
		::ShowWindow(m_DbgWnd, SW_HIDE);
		m_isVisible = false;
	}
}
