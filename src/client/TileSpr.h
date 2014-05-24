#pragma once
#include <windows.h>
class CTileSpr {
public:
	CTileSpr();
	short m_sTileSprite;
	short m_sTileSpriteFrame;
	short m_sObjectSprite;
	short m_sObjectSpriteFrame;
	bool m_bIsMoveAllowed;
	bool m_bIsTeleport;
};
