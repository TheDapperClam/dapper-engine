#include "MyGame.h"

// Remove this after setting up proper texture and scene loading
#include "Input.h"
#include "Rendering.h"
#include "Scene.h"

void dap::MyGame::End () {

}

void dap::MyGame::Input () {
	if ( dap::Input::KeyPressed ( GLFW_KEY_ESCAPE ) ) {
		Close ();
	}
}

void dap::MyGame::Start () {
	// Remove this after setting up proper texture and scene loading
	dap::Rendering::LoadTexture ( "Apple", "./Resources/Textures/apple.png" );
	new dap::Scene ( "myfile.json", "MyScene" );
}

void dap::MyGame::Tick ( const float time, const float delta ) {

}
