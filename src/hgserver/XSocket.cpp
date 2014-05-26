#include "XSocket.h"
#include "Client.h"
#include "winmain.h"

XSocket::XSocket(HWND hWnd, int iBlockLimit) {
	register int i;

	m_cType = 0;
	m_pRcvBuffer = NULL;
	m_pSndBuffer = NULL;
	m_Sock = INVALID_SOCKET;
	m_dwBufferSize = 0;

	m_cStatus = DEF_XSOCKSTATUS_READINGHEADER;
	m_dwReadSize = 3;
	m_dwTotalReadSize = 0;

	for (i = 0; i < DEF_XSOCKBLOCKLIMIT; i++) {
		m_iUnsentDataSize[i] = 0;
		m_pUnsentDataList[i] = NULL;
	}

	m_sHead = 0;
	m_sTail = 0;

	m_WSAErr = 0;

	m_hWnd = hWnd;
	m_bIsAvailable = false;

	m_iBlockLimit = iBlockLimit;
}

XSocket::~XSocket() {
	register int i;

	if (m_pRcvBuffer != NULL) delete m_pRcvBuffer;
	if (m_pSndBuffer != NULL) delete m_pSndBuffer;

	for (i = 0; i < DEF_XSOCKBLOCKLIMIT; i++)
		if (m_pUnsentDataList[i] != NULL) delete m_pUnsentDataList[i];

	// ������ ���� �а� �ݴ´�.
	_CloseConn();
}

bool XSocket::bInitBufferSize(uint32_t dwBufferSize) {
	if (m_pRcvBuffer != NULL) delete m_pRcvBuffer;
	if (m_pSndBuffer != NULL) delete m_pSndBuffer;

	m_pRcvBuffer = new char[dwBufferSize + 8];
	if (m_pRcvBuffer == NULL) return false;

	m_pSndBuffer = new char[dwBufferSize + 8];
	if (m_pSndBuffer == NULL) return false;

	m_dwBufferSize = dwBufferSize;

	return true;
}

int XSocket::iOnSocketEvent(WPARAM wParam, LPARAM lParam) {
	int WSAEvent;

	// ������ ������ �̺�Ʈ�� ó���� �� ���.
	if (m_cType != DEF_XSOCK_NORMALSOCK) return DEF_XSOCKEVENT_SOCKETMISMATCH;
	// �ʱ�ȭ ���� �ʾƼ� ó���� �� ���.
	if (m_cType == 0) return DEF_XSOCKEVENT_NOTINITIALIZED;

	if ((SOCKET) wParam != m_Sock) return DEF_XSOCKEVENT_SOCKETMISMATCH;
	WSAEvent = WSAGETSELECTEVENT(lParam);

	switch (WSAEvent) {
		case FD_CONNECT:
			if (WSAGETSELECTERROR(lParam) != 0) {
				// �� ������ ������ ���������Ƿ� �������� �õ��Ѵ�.
				if (bConnect(m_pAddr, m_iPortNum, m_uiMsg) == false) return DEF_XSOCKEVENT_SOCKETERROR;

				return DEF_XSOCKEVENT_RETRYINGCONNECTION;
			} else {
				m_bIsAvailable = true;
				return DEF_XSOCKEVENT_CONNECTIONESTABLISH;
			}
			break;

		case FD_READ:
			if (WSAGETSELECTERROR(lParam) != 0) {
				// �д� ���� ������ �߻��ߴ�.
				m_WSAErr = WSAGETSELECTERROR(lParam);
				return DEF_XSOCKEVENT_SOCKETERROR;
			} else return _iOnRead();
			break;

		case FD_WRITE:
			return _iSendUnsentData();
			break;

		case FD_CLOSE:
			m_cType = DEF_XSOCK_SHUTDOWNEDSOCK; // ������ �������Ƿ� �� ������ ��� �Ұ���. 
			return DEF_XSOCKEVENT_SOCKETCLOSED;
			break;
	}

	return DEF_XSOCKEVENT_UNKNOWN;
}

bool XSocket::bConnect(char * pAddr, int iPort, unsigned int uiMsg) {
	SOCKADDR_IN saTemp;
	u_long arg;
	int iRet;
	uint32_t dwOpt;

	// ������ �������� �ʱ�ȭ�� Ŭ������ �� �Լ��� ����� �� ���.
	if (m_cType == DEF_XSOCK_LISTENSOCK) return false;
	if (m_Sock != INVALID_SOCKET) closesocket(m_Sock);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)
		return false;

	// ������ ����ŷ ���� 
	arg = 1;
	ioctlsocket(m_Sock, FIONBIO, &arg);

	// �ּҸ� ���ε��Ѵ�.
	memset(&saTemp, 0, sizeof (saTemp));
	saTemp.sin_family = AF_INET;
	saTemp.sin_addr.s_addr = inet_addr(pAddr);
	saTemp.sin_port = htons(iPort);

	iRet = connect(m_Sock, (struct sockaddr *) &saTemp, sizeof (saTemp));
	if (iRet == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			m_WSAErr = WSAGetLastError();
			return false;
		}
	}

	WSAAsyncSelect(m_Sock, m_hWnd, uiMsg, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	// ���� �ɼ��� �����Ѵ�. 
	dwOpt = 8192 * 5;
	setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *) &dwOpt, sizeof (dwOpt));
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *) &dwOpt, sizeof (dwOpt));


	strcpy(m_pAddr, pAddr);
	m_iPortNum = iPort;

	m_uiMsg = uiMsg;
	m_cType = DEF_XSOCK_NORMALSOCK;

	return true;
}

int XSocket::_iOnRead() {
	int iRet, WSAErr;
	uint16_t * wp;

	if (m_cStatus == DEF_XSOCKSTATUS_READINGHEADER) {

		iRet = recv(m_Sock, (char *) (m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);

		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			} else return DEF_XSOCKEVENT_BLOCK;
		} else
			if (iRet == 0) {
			// ������ �����. 
			m_cType = DEF_XSOCK_SHUTDOWNEDSOCK;
			return DEF_XSOCKEVENT_SOCKETCLOSED;
		}

		m_dwReadSize -= iRet;
		m_dwTotalReadSize += iRet;

		if (m_dwReadSize == 0) {
			// ����� �� �о��. 
			m_cStatus = DEF_XSOCKSTATUS_READINGBODY;
			// �о�� �� ��ü ����� ����Ѵ�.
			wp = (uint16_t *) (m_pRcvBuffer + 1);
			m_dwReadSize = (int) (*wp - 3); // ��� ������� �������� �ʴ´�. 

			if (m_dwReadSize == 0) {
				// ��ü ����� 0�̸� ��ü�κ��� ���� �ʿ䰡 �����Ƿ� 
				m_cStatus = DEF_XSOCKSTATUS_READINGHEADER;
				m_dwReadSize = 3;
				m_dwTotalReadSize = 0;
				return DEF_XSOCKEVENT_READCOMPLETE;
			} else
				if (m_dwReadSize > m_dwBufferSize) {
				m_cStatus = DEF_XSOCKSTATUS_READINGHEADER;
				m_dwReadSize = 3;
				m_dwTotalReadSize = 0;
				return DEF_XSOCKEVENT_MSGSIZETOOLARGE;
			}
		}
		return DEF_XSOCKEVENT_ONREAD;
	} else
		if (m_cStatus == DEF_XSOCKSTATUS_READINGBODY) {

		iRet = recv(m_Sock, (char *) (m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);

		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			} else return DEF_XSOCKEVENT_BLOCK;
		} else
			if (iRet == 0) {
			// ������ �����. 
			m_cType = DEF_XSOCK_SHUTDOWNEDSOCK;
			return DEF_XSOCKEVENT_SOCKETCLOSED;
		}

		m_dwReadSize -= iRet;
		m_dwTotalReadSize += iRet;

		if (m_dwReadSize == 0) {
			// ��ü�� �� �о��. ������ �̺�Ʈ�� ���� ���¸� �ٲ۴�. 
			m_cStatus = DEF_XSOCKSTATUS_READINGHEADER;
			m_dwReadSize = 3;
			m_dwTotalReadSize = 0;
		} else return DEF_XSOCKEVENT_ONREAD;
	}

	// �޽����� ��� �о��. �� �޽����� ������ Ŭ���� ������ ������ �о� �ﰢ ó���ؾ� �Ѵ�.
	return DEF_XSOCKEVENT_READCOMPLETE;
}

int XSocket::_iSend(char * cData, int iSize, bool bSaveFlag) {
	int iOutLen, iRet, WSAErr;

	if (m_pUnsentDataList[m_sHead] != NULL) {
		if (bSaveFlag == true) {
			// ���� ��⿭�� �����Ͱ� ���� �ְ� �� ������ �ϴ� �����Ͷ�� 
			// �޽����� �� ���߱� ���� ������ ��⿭�� �����ؾ� �Ѵ�. 
			iRet = _iRegisterUnsentData(cData, iSize);
			switch (iRet) {
				case -1:
					// �Ҵ��� �޸𸮰� ���. �̰��� ������ ����� ���.
					return DEF_XSOCKEVENT_CRITICALERROR;
				case 0:
					// ť�� ����á��. �� ���� Ŭ������ �����Ǿ�߸� �Ѵ�.
					return DEF_XSOCKEVENT_QUENEFULL;
				case 1:
					// ���������� �����͸� ����ߴ�.
					break;
			}
			return DEF_XSOCKEVENT_BLOCK;
		} else return 0;
	}

	iOutLen = 0;
	while (iOutLen < iSize) {

		iRet = send(m_Sock, (cData + iOutLen), iSize - iOutLen, 0);

		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				// ���Ͽ� ������ �߻��ߴ�.
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			} else {
				// �?�����̸� ���̻� ���� �� �����Ƿ� �����ִ� �����͸� ����Ʈ�� ����ϰ� ���� 
				if (bSaveFlag == true) {
					iRet = _iRegisterUnsentData((cData + iOutLen), (iSize - iOutLen));
					switch (iRet) {
						case -1:
							// �Ҵ��� �޸𸮰� ���. �̰��� ������ ����� ���.
							return DEF_XSOCKEVENT_CRITICALERROR;
							break;
						case 0:
							// ť�� ����á��. �� ���� Ŭ������ �����Ǿ�߸� �Ѵ�.
							return DEF_XSOCKEVENT_QUENEFULL;
							break;
						case 1:
							// ���������� �����͸� ����ߴ�.
							break;
					}
				}
				return DEF_XSOCKEVENT_BLOCK;
			}
		} else iOutLen += iRet;

	}

	return iOutLen;
}


// �� �Լ��� _SendUnsentData������ ����Ѵ�. ����� �ƴ� �����ÿ��� ��������, �?���¸� ������ ��ۻ��¿����� ���� ��ŭ�� ���� ��ȯ.

int XSocket::_iSend_ForInternalUse(char * cData, int iSize) {
	int iOutLen, iRet, WSAErr;

	iOutLen = 0;
	while (iOutLen < iSize) {

		iRet = send(m_Sock, (cData + iOutLen), iSize - iOutLen, 0);

		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				// ���Ͽ� ������ �߻��ߴ�.
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			} else {
				// �?�����̸� ���̻� ���� �� �����Ƿ� ���ݱ��� ���� ������ ������� ��ȯ  
				return iOutLen;
			}
		} else iOutLen += iRet;
	}

	return iOutLen;
}

int XSocket::_iRegisterUnsentData(char * cData, int iSize) {
	// ť�� �������� ���̻� �����͸� ��⿭�� ������ �� ���.
	if (m_pUnsentDataList[m_sTail] != NULL) return 0;

	m_pUnsentDataList[m_sTail] = new char[iSize];
	if (m_pUnsentDataList[m_sTail] == NULL) return -1; // �޸� �Ҵ翡 �����ߴ�.

	// ������ ���� 
	memcpy(m_pUnsentDataList[m_sTail], cData, iSize);
	m_iUnsentDataSize[m_sTail] = iSize;

	// ���� ������ ���� 
	m_sTail++;
	//if (m_sTail >= DEF_XSOCKBLOCKLIMIT) m_sTail = 0;
	if (m_sTail >= m_iBlockLimit) m_sTail = 0;

	return 1;
}

int XSocket::_iSendUnsentData() {
	int iRet;
	char * pTemp;

	// ������ �� ��⿭�� �����͸� ������. 
	while (m_pUnsentDataList[m_sHead] != NULL) {

		iRet = _iSend_ForInternalUse(m_pUnsentDataList[m_sHead], m_iUnsentDataSize[m_sHead]);

		if (iRet == m_iUnsentDataSize[m_sHead]) {
			// Headť�� �����͸� �� ���´�.	���� �����͸� ������.
			delete m_pUnsentDataList[m_sHead];
			m_pUnsentDataList[m_sHead] = NULL;
			m_iUnsentDataSize[m_sHead] = 0;
			// ��� ������ ���� 
			m_sHead++;
			//if (m_sHead >= DEF_XSOCKBLOCKLIMIT) m_sHead = 0;
			if (m_sHead >= m_iBlockLimit) m_sHead = 0;
		} else {
			// ������ �� ���� ������ �߻��ϸ� �׳� �����Ѵ�. (���ŵǾ߸� �Ѵ�)
			if (iRet < 0)
				return iRet;

			// �����͸� �� ������ ���ϰ� �� ��� ���°� �߻��ߴ�. ������ ���� �����͸� ���ܳ��´�.
			pTemp = new char[m_iUnsentDataSize[m_sHead] - iRet];
			memcpy(pTemp, m_pUnsentDataList[m_sHead] + iRet, m_iUnsentDataSize[m_sHead] - iRet);

			delete m_pUnsentDataList[m_sHead];
			m_pUnsentDataList[m_sHead] = pTemp;

			return DEF_XSOCKEVENT_UNSENTDATASENDBLOCK;
		}
	}

	return DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE;
}

int XSocket::iSendMsg(char * cData, uint32_t dwSize, char cKey) {
	uint16_t * wp;
	int iRet;

	//m_pSndBuffer = cData;
	// �޽��� ũ�Ⱑ ���ۺ��� ũ�� ���� �� ���.
	if (dwSize > m_dwBufferSize) return DEF_XSOCKEVENT_MSGSIZETOOLARGE;

	// ������ ���� Ȥ�� ���� �������� �޽����� ���� ���� ���.
	if (m_cType != DEF_XSOCK_NORMALSOCK) return DEF_XSOCKEVENT_SOCKETMISMATCH;
	// �ʱ�ȭ ���� �ʾƼ� �޽����� ���� �� ���.
	if (m_cType == 0) return DEF_XSOCKEVENT_NOTINITIALIZED;

	// Ű �Է� 
	m_pSndBuffer[0] = cKey;

	wp = (uint16_t *) (m_pSndBuffer + 1);
	*wp = dwSize + 3;

	memcpy((char *) (m_pSndBuffer + 3), cData, dwSize);
	// v.14 : m_pSndBuffer +3 ���� dwSize���� cKey�� 0�� �ƴ϶�� ��ȣȭ�Ѵ�.
	if (cKey != 0) {//Encryption
		for (std::size_t i = 0; i < dwSize; i++) {
			m_pSndBuffer[3 + i] += (i ^ cKey);
			m_pSndBuffer[3 + i] = m_pSndBuffer[3 + i] ^ (cKey ^ (dwSize - i));
		}
	}

	iRet = _iSend(m_pSndBuffer, dwSize + 3, true);

	if (iRet < 0) return iRet;
	else return (iRet - 3);
}

bool XSocket::bListen(char * pAddr, int iPort, unsigned int uiMsg) {
	SOCKADDR_IN saTemp;

	if (m_cType != 0) return false;
	if (m_Sock != INVALID_SOCKET) closesocket(m_Sock);

	// ������ ���Ѵ�. 
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)
		return false;

	// �ּҸ� ���ε��Ѵ�.
	memset(&saTemp, 0, sizeof (saTemp));
	saTemp.sin_family = AF_INET;
	saTemp.sin_addr.s_addr = inet_addr(pAddr);
	saTemp.sin_port = htons(iPort);
	if (bind(m_Sock, (PSOCKADDR) & saTemp, sizeof (saTemp)) == SOCKET_ERROR) {
		closesocket(m_Sock);
		return false;
	}

	if (listen(m_Sock, 5) == SOCKET_ERROR) {
		closesocket(m_Sock);
		return false;
	}

	WSAAsyncSelect(m_Sock, m_hWnd, uiMsg, FD_ACCEPT);

	m_uiMsg = uiMsg;
	m_cType = DEF_XSOCK_LISTENSOCK;

	return true;
}

bool XSocket::bAccept(class XSocket * pXSock, unsigned int uiMsg) {
	SOCKET AcceptedSock;
	sockaddr Addr;
	int iLength;
	uint32_t dwOpt;

	if (m_cType != DEF_XSOCK_LISTENSOCK) return false;
	if (pXSock == NULL) return false;

	iLength = sizeof (Addr);
	// Ŭ���̾�Ʈ�� ������ �޴´� . 
	AcceptedSock = accept(m_Sock, (struct sockaddr FAR *) &Addr, (int FAR *) &iLength);
	if (AcceptedSock == INVALID_SOCKET)
		return false;

	pXSock->m_Sock = AcceptedSock;
	WSAAsyncSelect(pXSock->m_Sock, m_hWnd, uiMsg, FD_READ | FD_WRITE | FD_CLOSE);

	// Accept�� ������ ���� �������� �ʱ�ȭ �ȴ�.
	pXSock->m_uiMsg = uiMsg;
	pXSock->m_cType = DEF_XSOCK_NORMALSOCK;

	// ���� �ɼ��� �����Ѵ�. 
	dwOpt = 8192 * 5;
	setsockopt(pXSock->m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *) &dwOpt, sizeof (dwOpt));
	setsockopt(pXSock->m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *) &dwOpt, sizeof (dwOpt));

	return true;
}

void XSocket::_CloseConn() {
	char cTmp[100];
	bool bFlag = true;
	int iRet;

	if (m_Sock == INVALID_SOCKET) return; // v1.4

	shutdown(m_Sock, 0x01);
	while (bFlag == true) {
		iRet = recv(m_Sock, cTmp, sizeof (cTmp), 0);
		if (iRet == SOCKET_ERROR) bFlag = false;
		if (iRet == 0) bFlag = false;
	}

	closesocket(m_Sock);

	m_cType = DEF_XSOCK_SHUTDOWNEDSOCK;
}

SOCKET XSocket::iGetSocket() {
	return m_Sock;
}

char * XSocket::pGetRcvDataPointer(uint32_t * pMsgSize, char * pKey) {
	uint16_t * wp;
	uint32_t dwSize;
	char cKey;

	cKey = m_pRcvBuffer[0];
	if (pKey != NULL) *pKey = cKey; // v1.4

	wp = (uint16_t *) (m_pRcvBuffer + 1);
	*pMsgSize = (*wp) - 3; // ���ũ��� �����ؼ� ��ȯ�Ѵ�. 
	dwSize = (*wp) - 3;

	if (dwSize > DEF_MSGBUFFERSIZE) dwSize = DEF_MSGBUFFERSIZE;

	// v.14 : m_pSndBuffer +3 ���� dwSize���� cKey�� 0�� �ƴ϶�� ��ȣȭ�� Ǭ��.
	if (cKey != 0) {//Encryption
		for (std::size_t i = 0; i < dwSize; i++) {
			m_pRcvBuffer[3 + i] = m_pRcvBuffer[3 + i] ^ (cKey ^ (dwSize - i));
			m_pRcvBuffer[3 + i] -= (i ^ cKey);
		}
	}
	return (m_pRcvBuffer + 3);
}

bool _InitWinsock() {
	int iErrCode;
	uint16_t wVersionRequested;
	WSADATA wsaData;

	// ������ ������ üũ�Ѵ�.
	wVersionRequested = MAKEWORD(2, 2);
	iErrCode = WSAStartup(wVersionRequested, &wsaData);
	if (iErrCode) return false;

	return true;
}

void _TermWinsock() {
	WSACleanup();
}

///

int XSocket::iGetPeerAddress(char * pAddrString) {
	SOCKADDR_IN sockaddr;
	int iRet, iLen;

	iLen = sizeof (sockaddr);
	iRet = getpeername(m_Sock, (struct sockaddr *) &sockaddr, &iLen);
	strcpy(pAddrString, (const char *) inet_ntoa(sockaddr.sin_addr));

	return iRet;
}
