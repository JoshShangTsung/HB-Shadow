#pragma once
#include <cstdint>

class CBuildItem {
public:
	CBuildItem();
	bool m_bBuildEnabled;
	char m_cName[21];
	int m_iSkillLimit;
	int m_iMaxSkill;
	int m_iSprH;
	int m_iSprFrame;
	char m_cElementName1[21], m_cElementName2[21], m_cElementName3[21];
	char m_cElementName4[21], m_cElementName5[21], m_cElementName6[21];
	uint32_t m_iElementCount[7];
	bool m_bElementFlag[7];

};