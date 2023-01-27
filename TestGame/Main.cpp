#include "MyGame.h"

#include "Editor.h"

int main () {
	dap::Editor editor;
	dap::Editor::SetCurrent ( &editor );
	dap::MyGame game;
	game.Run ( "Test Game", "Resources/Scenes/MyScene.dsn" );
	return 0;
}
