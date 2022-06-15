#include "MyGame.h"

// Remove this after setting up proper texture and scene loading
#include "Input.h"
#include <iostream>
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
	// Remove this after setting up proper scene loading
	new dap::Scene ( "SceneTestSave.json" );

	//auto fp = tickEvent.Subscribe<dap::MyGame> ( &dap::MyGame::Tick, this );
}

void dap::MyGame::Tick ( const float time, const float delta ) {
	//std::cout << "My Tick " << time << std::endl;
}
