#include "MyGame.h"

#include "Input.h"
#include <iostream>
#include "Rendering.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "RectCollider.h"
#include "Random.h"

dap::MyGame::MyGame ( std::string projectDir ) : Game ( projectDir ) {

}

void dap::MyGame::End () {

}

void dap::MyGame::Input () {
	if ( dap::Input::Pressed ( GLFW_KEY_ESCAPE ) ) {
		Close ();
	}

	if ( dap::Input::Pressed ( GLFW_MOUSE_BUTTON_1 ) )
		wantsToClick = true;
}

void dap::MyGame::Start () {
	scene = new dap::Scene ( "SceneTestSave.json" );
	appleObject = scene->GetGameObject ( "MySprite" );
	cameraObject = scene->GetGameObject ( "MyCamera" );
}

void dap::MyGame::Tick ( const float time, const float delta ) {
	if ( wantsToClick && appleObject && cameraObject ) {
		dap::RectCollider* appleCollComp = appleObject->GetComponent<dap::RectCollider> ();
		dap::Camera* cameraComp = cameraObject->GetComponent<dap::Camera> ();

		if ( appleCollComp->PointInside ( cameraComp->ScreenToWorld ( dap::Input::GetMousePos () ) ) ) {
			appleClicks++;
			glm::vec3 newPos ( dap::Random::Range ( -2.0f, 2.0f ), dap::Random::Range ( -1.0f, 1.0f ), 0.0f );
			appleObject->GetTransform ()->SetPosition ( newPos );
		}
	}
	wantsToClick = false;
}
