#include "MyGame.h"

#include "Input.h"
#include "Rendering.h"
#include "Scene.h"
#include "GameObject.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include "RectCollider.h"
#include "Random.h"
#include <math.h>
#include "Transform.h"

#include "Config.h"
#include "Label.h"

const float dap::MyGame::TIME_MAX = 30.0f;
const std::string dap::MyGame::APPLE_PREFAB = "Resources/Prefabs/apple.json";

void dap::MyGame::End () {

}

void dap::MyGame::Input () {
	if ( dap::Input::Pressed ( GLFW_KEY_ESCAPE ) )
		Stop ();

	if ( dap::Input::Pressed ( GLFW_MOUSE_BUTTON_1 ) )
		isClicking = true;

	if ( dap::Input::Pressed ( GLFW_KEY_F1 ) ) {
		dap::Config::SetProperty ( "rendering", "bFullscreen", dap::Config::GetProperty("rendering", "bFullscreen") == "true" ? "false" : "true" );
		dap::Rendering::UpdateWindowProperties ();
	}

	mousePoint = cameraObject->GetComponent<dap::Camera> ()->ScreenToWorld ( dap::Input::GetMousePos () );
}

void dap::MyGame::Start () {
	scene = dap::Scene::GetCurrent ();
	appleObject = scene->SpawnPrefab ( APPLE_PREFAB );
	cameraObject = scene->GetGameObject ( "MyCamera" );
	labelObject = scene->GetGameObject ( "MyLabel" );
	isGameStarted = false;
}

void dap::MyGame::Tick () {
	const float TIME_CURRENT = dap::Rendering::GetTime ();
	if ( !isGameStarted )
		timeStart = TIME_CURRENT + TIME_MAX;
	const float TIME_REMAINING = timeStart - TIME_CURRENT;

	if ( isGameStarted ) {
		labelObject->GetComponent<dap::Label> ()->SetText ( "Score: " + std::to_string ( applesCollected ) + "\nTime: " + std::to_string ( (int)std::ceil ( TIME_REMAINING ) ) );
		if ( TIME_REMAINING <= 0.0f ) {
			isGameStarted = false;
			labelObject->GetComponent<dap::Label> ()->SetText ( "Final Score: " + std::to_string ( applesCollected ) );
			appleObject->GetTransform ()->SetPosition ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
		}
	}

	if ( isClicking && appleObject->GetComponent<dap::RectCollider> ()->PointInside ( mousePoint ) ) {
		if ( !isGameStarted )
			applesCollected = 0;
		appleObject->Destroy ();
		appleObject = scene->SpawnPrefab ( APPLE_PREFAB );
		glm::vec3 randApplePos = glm::vec3 ( dap::Random::Range ( -5.0f, 5.0f ), dap::Random::Range ( -1.0f, 1.0f ), 0.0f );
		appleObject->GetTransform ()->SetPosition ( randApplePos );
		applesCollected++;
		isGameStarted = true;
		labelObject->GetComponent<dap::Label> ()->SetEnabled ( true );
	}
	isClicking = false;
}
