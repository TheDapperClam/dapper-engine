#ifdef _NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "MyGame.h"
#include "Rendering.h"

int main () {
	dap::MyGame game;
	dap::Rendering::Start ( 800, 600, 300, &game );
	return 0;
}
