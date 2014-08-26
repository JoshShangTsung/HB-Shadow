#define NAME_WORLDNAME1			"WS1" // Name for default (1st) server
#define MSG_WORLDNAME1			"HB Shadow" // Long name for 1st server...
#define MSG_HOMEPAGE			"http://www.hbshadow.com"
#define DEF_UPPERVERSION		3
#define DEF_LOWERVERSION		82
#define DEF_SERVER_IP			"104.153.104.228"
//#define DEF_SERVER_IP			"127.0.0.1"
#define DEF_SERVER_PORT			2848
#define DEF_MAXSTATS			200
#define DEF_UPPERVERSION_FANTASMA				 3
#define DEF_LOWERVERSION_FANTASMA				 3

//#define DEF_USING_WIN_IME		// Used for unicode characters (unchecked)
#define DEF_MAKE_ACCOUNT		 //Enable acc creation. Missing sprites, fixed!
//#define DEF_SELECTSERVER		// Show/Hides server selection screen.

#define DEF_COMMA_GOLD		// Allows to show comma nbe as original HB (ie: 1,200,000)
// #define DEF_4XXP_PER_LVL		// limits xp niv 140, then 4 x XP per lvl (affects XP shown as % on status)


//#define  _DEBUG				// Multi-Client, Instant-logout, Version check, Badwords,
// Mics debug msgs, CtrlQ for admins, Show status, Cursor position
#define DEF_WINDOWED_MODE		// Shows HB in a windowed mode (for debug purpose only, sprite will bug....)

#ifndef _DEBUG
#undef DEF_WINDOWED_MODE
#endif
