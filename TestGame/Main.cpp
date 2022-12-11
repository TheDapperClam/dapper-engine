#include "MyGame.h"

#include "Editor.h"

int main () {
#ifdef _DEBUG
	dap::Editor editor;
	dap::Editor::SetCurrent ( &editor );
#endif
	dap::MyGame game;
	game.Run ( "Test Game", "Resources/Scenes/MyScene.dsn" );
	return 0;
}
