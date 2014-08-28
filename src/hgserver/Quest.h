#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


#define DEF_QUESTTYPE_MONSTERHUNT				1
#define DEF_QUESTTYPE_MONSTERHUNT_TIMELIMIT		2
#define DEF_QUESTTYPE_ASSASSINATION 			3
#define DEF_QUESTTYPE_DELIVERY					4
#define DEF_QUESTTYPE_ESCORT					5
#define DEF_QUESTTYPE_GUARD						6
#define DEF_QUESTTYPE_GOPLACE					7
#define DEF_QUESTTYPE_BUILDSTRUCTURE			8
#define DEF_QUESTTYPE_SUPPLYBUILDSTRUCTURE		9
#define DEF_QUESTTYPE_STRATEGICSTRIKE			10
#define DEF_QUESTTYPE_SENDTOBATTLE				11
#define DEF_QUESTTYPE_SETOCCUPYFLAG				12

class CQuest {
public:

	char m_cSide;

	int m_iType;
	NpcType m_iTargetType;
	int m_iMaxCount;

	int m_iFrom;

	int m_iMinLevel;
	int m_iMaxLevel;

	int m_iRequiredSkillNum;
	int m_iRequiredSkillLevel;

	int m_iTimeLimit;
	int m_iAssignType; 

	int m_iRewardType[4];
	int m_iRewardAmount[4];

	int m_iContribution;
	int m_iContributionLimit;

	int m_iResponseMode;

	char m_cTargetName[21];
	int m_sX;
	int m_sY;
	int m_iRange;

	int m_iQuestID;

	int m_iReqContribution;
};
