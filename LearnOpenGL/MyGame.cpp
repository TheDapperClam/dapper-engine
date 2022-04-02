#include "MyGame.h"
#include "Rendering.h"

bool reloadScene = true;

void dap::MyGame::End () {
	const dap::Scene* myScene = dap::Scene::GetScene ( "MyScene" );
	if ( myScene ) {
		delete myScene;
	}
}

void dap::MyGame::Input () {
	velocity.x = dap::Input::KeyHeld ( GLFW_KEY_D ) - dap::Input::KeyHeld ( GLFW_KEY_A );
	velocity.y = dap::Input::KeyHeld ( GLFW_KEY_W ) - dap::Input::KeyHeld ( GLFW_KEY_S );

	if ( dap::Input::KeyPressed ( GLFW_KEY_SPACE ) ) {
		reloadScene = !reloadScene;
	}

	if ( dap::Input::KeyPressed ( GLFW_KEY_ESCAPE ) ) {
		Close ();
	}
}

void dap::MyGame::Start () {
	dap::Rendering::LoadTexture ( "Apple", "./Resources/Textures/apple.png" );
	new dap::Scene ( "myfile.json", "MyScene" );
}

void dap::MyGame::Tick ( const float time, const float delta ) {
	const dap::Scene* myScene = dap::Scene::GetScene ( "MyScene" );
	if ( reloadScene ) {
		delete myScene;
		new dap::Scene ( "myfile.json", "MyScene" );
	}
}
