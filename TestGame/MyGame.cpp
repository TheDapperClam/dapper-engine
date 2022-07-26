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

void dap::MyGame::End () {

}

void dap::MyGame::Input () {
	if ( dap::Input::Pressed ( GLFW_KEY_ESCAPE ) ) {
		Close ();
	}

	playerMoveInput.x = dap::Input::Held ( GLFW_KEY_D ) - dap::Input::Held ( GLFW_KEY_A );
	playerMoveInput.y = dap::Input::Held ( GLFW_KEY_W ) - dap::Input::Held ( GLFW_KEY_S );
	glm::normalize ( playerMoveInput );
}

void dap::MyGame::Start () {
	scene = new dap::Scene ( "SceneTestSave.json" );
	appleObject = scene->GetGameObject ( "MyApple" );
	cameraObject = scene->GetGameObject ( "MyCamera" );
	playerObject = scene->GetGameObject ( "MyPlayer" );
}

void dap::MyGame::Tick () {
	playerVelocity = playerMoveInput * playerMoveSpeed;
	playerObject->GetTransform ()->Translate ( playerVelocity );

	if ( playerObject->GetComponent<dap::RectCollider> ()->Overlapping ( appleObject->GetComponent<dap::RectCollider> () ) ) {
		appleObject->GetTransform ()->SetPosition ( glm::vec3 ( dap::Random::Range ( -5.0f, 5.0f ), dap::Random::Range ( -1.0f, 1.0f ), 0.0f ) );
	}
}
