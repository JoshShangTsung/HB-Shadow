#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "Msg.h"
#include "strtok.h"

#define DEF_MAXBADWORD		500

class CGameMonitor {
public:
	bool bCheckBadWord(char * pWord);
	int iReadBadWordFileList(const char * pFn);
	CGameMonitor();
	virtual ~CGameMonitor();

	class CMsg * m_pWordList[DEF_MAXBADWORD];

};
