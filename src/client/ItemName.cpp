#include "ItemName.h"
#include <cstring>
CItemName::CItemName() {
	std::memset(m_cOriginName, 0, sizeof(m_cOriginName));
	std::memset(m_cName, 0, sizeof(m_cName));
}
