#pragma once
#include <windows.h>

class CBuildItem {
public:
	CBuildItem();
	char m_cName[21];
	short m_sItemID;

	int m_iSkillLimit;

	int m_iMaterialItemID[6];
	int m_iMaterialItemCount[6];
	int m_iMaterialItemValue[6]; // ��� ������ ����ġ 
	int m_iIndex[6];

	int m_iMaxValue;
	int m_iAverageValue;
	int m_iMaxSkill; // �� �������� �����ؼ� �ø� �� �ִ� �ִ� ��ų
	WORD m_wAttribute; // �������� �Ӽ��� �ο��� �� �ִ�.
};
