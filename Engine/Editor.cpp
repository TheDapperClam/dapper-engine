#include "Editor.h"

#include "Camera.h"
#include "Component.h"
#include "Config.h"
#include "FactoryMetaData.h"
#include <filesystem>
#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ImGuiGameObjectEditor.h"
#include "ImGuiTilesetEditor.h"
#include "ImGuiWindow.h"
#include "Input.h"
#include <iostream>
#include "JSONobject.h"
#include "Rendering.h"
#include "Scene.h"
#include "Transform.h"

const std::vector<dap::ImGuiWindow*>* dap::Editor::ALL_IMGUI_WINDOWS = dap::ImGuiWindow::GetAllWindows ();
const glm::vec3 dap::Editor::EDITOR_CAMERA_CLEAR_COLOR = glm::vec3 ( 0.25f, 0.25f, 0.25f );
const float dap::Editor::EDITOR_CAMERA_MOVE_SPEED = 0.1f;
const float dap::Editor::EDITOR_CAMERA_ZOOM_DEFAULT = 10.0f;
const float dap::Editor::EDITOR_CAMERA_ZOOM_SPEED = 0.5f;
const std::string dap::Editor::EDITOR_TEMP_PATH = "editor_temp/";
const std::string dap::Editor::EDITOR_TEMP_PLAY_SCENE_PATH = dap::Editor::EDITOR_TEMP_PATH + "editor_play_scene.json";

dap::Editor* dap::Editor::CurrentEditor = NULL;

dap::Editor::~Editor () {
	for ( const auto& GAMEOBJECT : editorGameObjects ) {
		delete GAMEOBJECT;
	}
	editorGameObjects.clear ();
	if ( std::filesystem::exists ( EDITOR_TEMP_PATH ) )
		std::filesystem::remove_all ( EDITOR_TEMP_PATH );
}

void dap::Editor::BeginEdit () {
	dap::Camera::SetCurrent ( editorCamera );
}

void dap::Editor::GameEnded () {
	shouldRunGame = false;
	dap::Scene::LoadInstant ( EDITOR_TEMP_PLAY_SCENE_PATH );
}

dap::Editor* const dap::Editor::GetCurrent () {
	return CurrentEditor;
}

void dap::Editor::Initialize () {
	editorCamera = SpawnGameObject ()->AddComponent<dap::Camera> ();
	editorCamera->SetClearColor ( EDITOR_CAMERA_CLEAR_COLOR.x, EDITOR_CAMERA_CLEAR_COLOR.y, EDITOR_CAMERA_CLEAR_COLOR.z );
	editorCamera->SetOrthoSize ( EDITOR_CAMERA_ZOOM_DEFAULT );
	gameObjectEditorWindow = new dap::ImGuiGameObjectEditor ( "GameObjects" );
	tilesetEditorWindow = new dap::ImGuiTilesetEditor ( "Tileset Editor" );
	gameObjectEditorWindow->SetShow ( true );
}

void dap::Editor::Input () {
	leftMouseDown = dap::Input::Held ( GLFW_MOUSE_BUTTON_1 );
	rightMouseDown = dap::Input::Held ( GLFW_MOUSE_BUTTON_2 );
	editorCameraMovement.x = dap::Input::Held ( GLFW_KEY_D ) - dap::Input::Held ( GLFW_KEY_A );
	editorCameraMovement.y = dap::Input::Held ( GLFW_KEY_W ) - dap::Input::Held ( GLFW_KEY_S );
}

void dap::Editor::Quit () {
	shouldQuit = true;
}

void dap::Editor::RefreshSelected () {
	if ( !gameObjectEditorWindow )
		return;
	dap::Scene* scene = dap::Scene::GetCurrent ();
	if ( !scene )
		return;
	dap::GameObject* obj = scene->GetGameObject ( gameObjectEditorWindow->GetSelectedGameObjectName () );
	gameObjectEditorWindow->SetSelectedGameObject ( obj );
}

void dap::Editor::SetCurrent ( dap::Editor* editor ) {
	CurrentEditor = editor;
}

bool dap::Editor::ShouldRunGame () {
	return shouldRunGame;
}

void dap::Editor::ShowAllImGuiWindows () {
	for ( const auto& WINDOW : *ALL_IMGUI_WINDOWS ) {
		WINDOW->Begin ();
	}
}

void dap::Editor::ShowMenuBar () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();

	if ( !ImGui::BeginMainMenuBar () ) {
		ImGui::EndMainMenuBar ();
		return;
	}

	if ( ImGui::BeginMenu ( "File" ) ) {
		if ( !shouldRunGame ) {
			if ( ImGui::MenuItem ( "Save Scene" ) )
				currentScene->Save ( currentScene->GetPath () );
			ImGui::Separator ();
		}
		if ( ImGui::MenuItem ( "Quit" ) )
			Quit ();
		ImGui::EndMenu ();
	}

	if ( ImGui::BeginMenu ( "View" ) ) {
		bool isFullscreen = dap::Config::GetProperty ( "rendering", "bFullscreen" ) == "true";
		if ( ImGui::Checkbox ( "Fullscreen", &isFullscreen ) ) {
			dap::Config::SetProperty ( "rendering", "bFullscreen", isFullscreen ? "true" : "false" );
			dap::Rendering::UpdateWindowProperties ();
			dap::Config::Save ( dap::Config::PATH_DEFAULT );
		}
		ImGui::Separator ();
		ImGui::Checkbox ( "Visualize Colliders", &visualizeColliders );
		ImGui::EndMenu ();
	}

	if ( ImGui::BeginMenu ( "Windows" ) ) {
		for ( const auto& WINDOW : *ALL_IMGUI_WINDOWS ) {
			ImGui::Checkbox ( WINDOW->GetTitle ().c_str (), WINDOW->GetShow () );
		}
		ImGui::EndMenu ();
	}

	if ( !shouldRunGame ) {
		if ( ImGui::Button ( "Play" ) ) {
			currentScene->Save ( EDITOR_TEMP_PLAY_SCENE_PATH );
			shouldRunGame = true;
		}
	} else {
		if ( ImGui::Button ( "Stop" ) )
			shouldRunGame = false;
	}

	ImGui::EndMainMenuBar ();
}

bool dap::Editor::ShouldQuit () {
	return shouldQuit;
}

dap::GameObject* dap::Editor::SpawnGameObject () {
	dap::GameObject* newObj = new dap::GameObject ( NULL );
	editorGameObjects.push_back ( newObj );
	return newObj;
}

void dap::Editor::Tick () {
	ShowMenuBar ();
	ShowAllImGuiWindows ();

	if ( !shouldRunGame ) {
		editorCamera->GetOwner ()->GetTransform ()->Translate ( editorCameraMovement * EDITOR_CAMERA_MOVE_SPEED, false );
		editorCamera->SetOrthoSize ( editorCamera->GetOrthoSize () + dap::Input::GetMouseScroll ().y * -EDITOR_CAMERA_ZOOM_SPEED );
	}
}

bool dap::Editor::VisualizeColliders () const {
	return visualizeColliders;
}
