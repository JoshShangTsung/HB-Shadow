#include "Magic.h"
CMagic::CMagic() {
	std::memset(m_cName, 0, sizeof(m_cName));
	m_iAttribute = 0;
}

