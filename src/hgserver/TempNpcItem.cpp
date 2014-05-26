#include "TempNpcItem.h"
extern void PutLogFileList(char * cStr);

CNpcItem::CNpcItem() {
	std::memset(m_cName, 0, sizeof(m_cName));

	m_sItemID = 0;
	m_sFirstProbability = 0;
	m_cFirstTargetValue = 0;
	m_sSecondProbability = 0;
	m_cSecondTargetValue = 0;

}