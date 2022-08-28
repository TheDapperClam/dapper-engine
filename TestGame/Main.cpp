#include "MyGame.h"

int main () {
	dap::MyGame game;
	game.Run ( "Test Game", 30, "Resources/Scenes/MyScene.json" );
	return 0;
}
