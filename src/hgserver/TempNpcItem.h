#pragma once
#include <windows.h>

class CNpcItem {
public:
	CNpcItem();

	char m_cName[21];
	short m_sItemID;
	short m_sFirstProbability;
	short m_sSecondProbability;
	char m_cFirstTargetValue;
	char m_cSecondTargetValue;

};
