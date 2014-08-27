#pragma once
#include <windows.h>
#include <string.h>
#include <winbase.h>
#include <cstdint>

class CMsg {
public:

	void * operator new (size_t size) {
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	};

	void operator delete(void * mem) {
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	};

	CMsg(char cType, const char * pMsg, uint32_t dwTime);
	virtual ~CMsg();

	char m_cType;
	char * m_pMsg;
	short m_sX;
	short m_sY;
	uint32_t m_dwTime;

	int m_iObjectID;

};
