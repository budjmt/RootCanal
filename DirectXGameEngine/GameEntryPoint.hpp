#ifdef GAME_CLASS
#include "DirectXGameCore.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define GAME_START(GC)  \
	GC game(hInstance); \
	if (!game.Init()) return 0; \
	return game.Run();

#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
//the main game file should contain a definition of this that is the name of the main game class
	GAME_START(GAME_CLASS)
}
#pragma endregion
#else
static_assert(true, "GAME_CLASS was not defined");
#endif