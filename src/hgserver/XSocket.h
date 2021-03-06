#pragma once

#define _WINSOCK2API_
//#define  FD_SETSIZE 2000
#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
//#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <iostream>
#include <string>

#define DEF_XSOCK_LISTENSOCK			1
#define DEF_XSOCK_NORMALSOCK			2				
#define DEF_XSOCK_SHUTDOWNEDSOCK		3				

#define DEF_XSOCKSTATUS_READINGHEADER	11
#define DEF_XSOCKSTATUS_READINGBODY		12

#define DEF_XSOCKEVENT_SOCKETMISMATCH			-121
#define DEF_XSOCKEVENT_CONNECTIONESTABLISH		-122
#define DEF_XSOCKEVENT_RETRYINGCONNECTION		-123
#define DEF_XSOCKEVENT_ONREAD					-124
#define DEF_XSOCKEVENT_READCOMPLETE				-125
#define DEF_XSOCKEVENT_UNKNOWN					-126
#define DEF_XSOCKEVENT_SOCKETCLOSED				-127
#define DEF_XSOCKEVENT_BLOCK					-128
#define DEF_XSOCKEVENT_SOCKETERROR				-129
#define DEF_XSOCKEVENT_CRITICALERROR			-130
#define DEF_XSOCKEVENT_NOTINITIALIZED			-131
#define DEF_XSOCKEVENT_MSGSIZETOOLARGE			-132
#define DEF_XSOCKEVENT_CONFIRMCODENOTMATCH		-133
#define DEF_XSOCKEVENT_QUENEFULL                -134
#define DEF_XSOCKEVENT_UNSENTDATASENDBLOCK		-135
#define DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE	-136

#define DEF_XSOCKBLOCKLIMIT						300	

class XSocket {
public:
	int iGetPeerAddress(char * pAddrString);
	char * pGetRcvDataPointer(uint32_t * pMsgSize, char * pKey = 0);
	SOCKET iGetSocket();
	bool bAccept(class XSocket * pXSock, unsigned int uiMsg);
	bool bListen(char * pAddr, int iPort, unsigned int uiMsg);
	int iSendMsg(char * cData, uint32_t dwSize, char cKey = 0);
	bool bConnect(char * pAddr, int iPort, unsigned int uiMsg);
	int iOnSocketEvent(WPARAM wParam, LPARAM lParam);
	bool bInitBufferSize(uint32_t dwBufferSize);
	XSocket(HWND hWnd, int iBlockLimit);
	virtual ~XSocket();

	int m_WSAErr = 0;
	bool m_bIsAvailable = false;

private:
	void _CloseConn();

	int _iSendUnsentData();
	int _iRegisterUnsentData(char * cData, int iSize);
	int _iSend(char * cData, int iSize, bool bSaveFlag);
	int _iSend_ForInternalUse(char * cData, int iSize);
	int _iOnRead();

	char m_cType = 0;
	char * m_pRcvBuffer = nullptr;
	char * m_pSndBuffer = nullptr;
	uint32_t m_dwBufferSize = 0;

	SOCKET m_Sock = INVALID_SOCKET;
	char m_cStatus = DEF_XSOCKSTATUS_READINGHEADER;
	uint32_t m_dwReadSize = 3;
	uint32_t m_dwTotalReadSize = 0;
	char m_pAddr[30]{};
	int m_iPortNum = 0;

	char * m_pUnsentDataList[DEF_XSOCKBLOCKLIMIT]{};
	int m_iUnsentDataSize[DEF_XSOCKBLOCKLIMIT]{};
	short m_sHead = 0;
	short m_sTail = 0;

	unsigned int m_uiMsg = 0;
	HWND m_hWnd = 0;

	int m_iBlockLimit = 0;
};


bool _InitWinsock();
void _TermWinsock();
