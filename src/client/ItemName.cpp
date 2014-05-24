#include "ItemName.h"

CItemName::CItemName() {
	ZeroMemory(m_cOriginName, sizeof (m_cOriginName));
	ZeroMemory(m_cName, sizeof (m_cName));
}
