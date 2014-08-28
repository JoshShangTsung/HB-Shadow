#include "Tile.h"

CTile::~CTile() {
	for (int i = 0; i < DEF_TILE_PER_ITEMS; i++)
		delete m_pItem[i];
}
