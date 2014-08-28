#pragma once

#define DEF_SKILLEFFECTTYPE_GET			    1
#define DEF_SKILLEFFECTTYPE_PRETEND			2
#define DEF_SKILLEFFECTTYPE_TAMING			3

class CSkill {
public:
	char m_cName[21]{};

	short m_sType = 0;
	short m_sValue1 = 0;
	short m_sValue2 = 0;
	short m_sValue3 = 0;
	short m_sValue4 = 0;
	short m_sValue5 = 0;
	short m_sValue6 = 0;

};
