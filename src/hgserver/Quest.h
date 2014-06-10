#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


#define DEF_QUESTTYPE_MONSTERHUNT				1		// �Ϲ����� ���� ���� 
#define DEF_QUESTTYPE_MONSTERHUNT_TIMELIMIT		2		// �ð� ������ �ɷ��ִ� ���� ���� 
#define DEF_QUESTTYPE_ASSASSINATION 			3		// �ϻ� 
#define DEF_QUESTTYPE_DELIVERY					4		// ���: Ư�� ��ҷ� �������� ����Ѵ�.
#define DEF_QUESTTYPE_ESCORT					5		// ��ȣ: Ư�� ĳ���͸� ��ȣ�Ѵ�.
#define DEF_QUESTTYPE_GUARD						6		// Ư�� ������ ���� ������κ��� ����Ѵ�. 
#define DEF_QUESTTYPE_GOPLACE					7		// Ư�� ��ҷ� ����. �� ħ�� 
#define DEF_QUESTTYPE_BUILDSTRUCTURE			8		// ������ ��ġ �ӹ�
#define DEF_QUESTTYPE_SUPPLYBUILDSTRUCTURE		9		// ������ ��ġ ���� �ӹ�
#define DEF_QUESTTYPE_STRATEGICSTRIKE			10		// ����� ����ӹ� 
#define DEF_QUESTTYPE_SENDTOBATTLE				11		// ��ٷ� ������� ���ϴ� �ӹ�
#define DEF_QUESTTYPE_SETOCCUPYFLAG				12		// ���� ���� ����� ��ġ�ϴ� �ӹ� 

class CQuest {
public:

	char m_cSide; // ��� �? ���ϴ� �ӹ��ΰ�? 

	int m_iType; // Quest ���� 
	int m_iTargetType; // Quest�� ��ǥ ����. �� Type�� ��� �ٸ� ���� �������. 
	int m_iMaxCount; // �ִ� ���� Ƚ�� 

	int m_iFrom; // Quest�� �����ִ� NPC ���� 

	int m_iMinLevel; // Quest�� �ޱ� ���� ���� ����. 
	int m_iMaxLevel; // Quest�� �ޱ� ���� �ִ� ���� 

	int m_iRequiredSkillNum; // ���� �Ǵܿ� �ʿ��� ��ų ��ȣ 
	int m_iRequiredSkillLevel; // ���� �Ǵܿ� �ʿ��� ��ų ���� 

	int m_iTimeLimit; // �ð� ���� ������ 
	int m_iAssignType; // ��Ʈ �Ҵ� Ÿ��. -1�̸� �ƹ�����. 1�̸� Crusade ������϶���.

	// ��ǰ ���� �� ����. 3�� �� 1���� �����ϰ� ������. 0�� �ε����� ��� ����.
	int m_iRewardType[4];
	int m_iRewardAmount[4];

	int m_iContribution; // ���嵵 
	int m_iContributionLimit; // �ִ� ���嵵 ���� 

	int m_iResponseMode; // ���� ���: 0(ok) 1(Accept/Decline) 2(Next)

	char m_cTargetName[21]; // ��Ʈ�� ������ �̸�. �� Ȥ�� ĳ���� �̸�  
	int m_sX, m_sY, m_iRange; // ��Ʈ�� ������ �� ��� 

	int m_iQuestID; // ��Ʈ ID. ���� ��Ʈ�� ������ ����Ǿ��� ��츦 ����� ����.

	int m_iReqContribution; // ��Ʈ�� �ñ� ���� ���� ���嵵 ����. 
};
