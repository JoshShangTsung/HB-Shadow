#pragma once
#include <windows.h>
#include "item.h"

class CFish {
public:
	CFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty);
	virtual ~CFish();

	char m_cMapIndex;
	short m_sX, m_sY; // �ʻ��� ��ġ 

	short m_sType; // ����� ����  
	class CItem * m_pItem; // �������� �Ҵ�Ǵ� ������(����� ���� ����)

	short m_sDynamicObjectHandle;

	short m_sEngagingCount; // ���� �� ����⸦ �븮�� �ִ� �÷��̾� �� 
	int m_iDifficulty; // ���̵�. �ּ� 1�� �Ǿ�� �Ѵ�.
};
