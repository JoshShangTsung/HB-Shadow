#pragma once
#include <windows.h>
#include <string.h>
#include <winbase.h>

class CMsg {
public:

	void * operator new (size_t size) {
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	};

	void operator delete(void * mem) {
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	};

	CMsg(char cType, char * pMsg, DWORD dwTime);
	virtual ~CMsg();

	char m_cType;
	char * m_pMsg;
	short m_sX, m_sY;
	DWORD m_dwTime;

	int m_iObjectID;

};
