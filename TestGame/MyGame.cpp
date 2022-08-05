#include "MyGame.h"

#include "Input.h"
#include <iostream>
#include "Rendering.h"
#include "Scene.h"
#include "GameObject.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include "RectCollider.h"
#include "Random.h"
#include <math.h>

#include "Font.h"
#include "Label.h"

void dap::MyGame::End () {

}

void dap::MyGame::Input () {
	if ( dap::Input::Pressed ( GLFW_KEY_ESCAPE ) )
		Close ();

	if ( dap::Input::Pressed ( GLFW_MOUSE_BUTTON_1 ) )
		isClicking = true;

	mousePoint = cameraObject->GetComponent<dap::Camera> ()->ScreenToWorld ( dap::Input::GetMousePos () );
}

void dap::MyGame::Start () {
	scene = new dap::Scene ( "NewScene.json" );
	appleObject = scene->GetGameObject ( "MyApple" );
	cameraObject = scene->GetGameObject ( "MyCamera" );
	labelObject = scene->GetGameObject ( "MyLabel" );
}

void dap::MyGame::Tick () {
	if ( isClicking && appleObject->GetComponent<dap::RectCollider> ()->PointInside ( mousePoint ) ) {
		appleObject->GetTransform ()->SetPosition ( glm::vec3 ( dap::Random::Range ( -5.0f, 5.0f ), dap::Random::Range ( -1.0f, 1.0f ), 0.0f ) );
		applesCollected++;
	}
	isClicking = false;

	labelObject->GetComponent<dap::Label> ()->SetText ( "Score: " + std::to_string(applesCollected) );
}
