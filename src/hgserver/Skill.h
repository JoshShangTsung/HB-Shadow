#pragma once
#include <windows.h>



#define DEF_SKILLEFFECTTYPE_GET			    1
#define DEF_SKILLEFFECTTYPE_PRETEND			2
#define DEF_SKILLEFFECTTYPE_TAMING			3

class CSkill {
public:
	CSkill();
	char m_cName[21];

	short m_sType;
	short m_sValue1;
	short m_sValue2;
	short m_sValue3;
	short m_sValue4;
	short m_sValue5;
	short m_sValue6;

};
