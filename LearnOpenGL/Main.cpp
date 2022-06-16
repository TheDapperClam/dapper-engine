#include "Rendering.h"
#include "MyGame.h"

int main () {
	dap::MyGame game ( "../TestGame" );
	dap::Rendering::Start ( 800, 600, 300, &game );
	return 0;
}
