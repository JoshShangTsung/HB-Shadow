#include "GuildsMan.h"
#include <cstring>

CGuildsMan::CGuildsMan() {
	std::memset(m_cName, 0, sizeof (m_cName));
}
