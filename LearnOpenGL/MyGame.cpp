#include "MyGame.h"

// Remove this after setting up proper texture and scene loading
#include "Input.h"
#include <iostream>
#include "Rendering.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sprite.h"

#include "Camera.h"
#include "RectCollider.h"

#include "glm/gtx/string_cast.hpp"

dap::MyGame::MyGame ( std::string projectDir ) : Game ( projectDir ) {

}

void dap::MyGame::End () {

}

void dap::MyGame::Input () {
	if ( dap::Input::KeyPressed ( GLFW_KEY_ESCAPE ) ) {
		Close ();
	}

	if ( dap::Input::KeyHeld ( GLFW_KEY_RIGHT ) ) {
		myObject->GetTransform ()->Translate ( glm::vec3 ( 0.1f, 0.0f, 0.0f ) );
	}

	if ( dap::Input::KeyHeld ( GLFW_KEY_LEFT ) ) {
		myObject->GetTransform ()->Translate ( glm::vec3 ( -0.1f, 0.0f, 0.0f ) );
	}

	if ( dap::Input::KeyHeld ( GLFW_KEY_UP ) ) {
		myObject->GetTransform ()->Translate ( glm::vec3 ( 0.0f, 0.1f, 0.0f ) );
	}

	if ( dap::Input::KeyHeld ( GLFW_KEY_DOWN ) ) {
		myObject->GetTransform ()->Translate ( glm::vec3 ( 0.0f, -0.1f, 0.0f ) );
	}
}

void dap::MyGame::Start () {
	// Remove this after setting up proper scene loading
	dap::Scene* myScene = new dap::Scene ( "SceneTestSave.json" );
	myObject = myScene->GetGameObject ( "MySprite" );
}

void dap::MyGame::Tick ( const float time, const float delta ) {
	dap::Scene* myScene = dap::Scene::GetScene ( "MyScene" );
	auto* camComp = myScene->GetGameObject ( "MyCamera" )->GetComponent<dap::Camera> ();
	auto* collCompA = myScene->GetGameObject ( "MySprite" )->GetComponent<dap::RectCollider> ();
	//std::cout << glm::to_string(dap::Input::GetMousePos ()) << std::endl;
	std::cout << collCompA->PointInside ( camComp->ScreenToWorld ( dap::Input::GetMousePos () ) ) << std::endl;
}
