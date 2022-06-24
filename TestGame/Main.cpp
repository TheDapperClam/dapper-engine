#ifdef _NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "MyGame.h"

int main () {
	dap::MyGame game;
	game.Run ( 800, 600, 300 );
	return 0;
}
