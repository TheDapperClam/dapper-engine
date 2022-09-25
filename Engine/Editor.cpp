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
#include "Input.h"
#include <iostream>
#include "JSONobject.h"
#include "Rendering.h"
#include "Scene.h"
#include "Transform.h"

const std::vector<dap::GameObject*>* dap::Editor::ALL_GAMEOBJECTS = dap::Scene::GetAllGameObjects ();
const glm::vec3 dap::Editor::EDITOR_CAMERA_CLEAR_COLOR = glm::vec3 ( 0.25f, 0.25f, 0.25f );
const float dap::Editor::EDITOR_CAMERA_MOVE_SPEED = 0.1f;
const float dap::Editor::EDITOR_CAMERA_ZOOM_DEFAULT = 10.0f;
const float dap::Editor::EDITOR_CAMERA_ZOOM_SPEED = 0.5f;
const float dap::Editor::SCENE_WINDOW_MAX_HEIGHT_OFFSET = -100.0f;

dap::Editor* dap::Editor::CurrentEditor = NULL;

dap::Editor::~Editor () {
	for ( const auto& GAMEOBJECT : editorGameObjects ) {
		delete GAMEOBJECT;
	}
	editorGameObjects.clear ();
}

void dap::Editor::BeginEdit () {
	dap::Camera::SetCurrent ( editorCamera );
}

void dap::Editor::GameEnded () {
	shouldRunGame = false;
	dap::Scene::LoadInstant ( dap::Scene::GetCurrent ()->GetPath () );
}

dap::Editor* const dap::Editor::GetCurrent () {
	return CurrentEditor;
}

void dap::Editor::Initialize () {
	editorCamera = SpawnGameObject ()->AddComponent<dap::Camera> ();
	editorCamera->SetClearColor ( EDITOR_CAMERA_CLEAR_COLOR.x, EDITOR_CAMERA_CLEAR_COLOR.y, EDITOR_CAMERA_CLEAR_COLOR.z );
	editorCamera->SetOrthoSize ( EDITOR_CAMERA_ZOOM_DEFAULT );
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
	dap::GameObject* obj = dap::Scene::GetCurrent ()->GetGameObject ( selectedGameObjectName );
	SetSelectedGameObject ( obj );
}

void dap::Editor::SetCurrent ( dap::Editor* editor ) {
	CurrentEditor = editor;
}

void dap::Editor::SetSelectedGameObject ( dap::GameObject* object ) {
	selectedGameObject = object;
	selectedGameObjectName = object ? object->GetName () : "";
	savePrefabPath = object ? object->GetPrefabPath () : "";
}

bool dap::Editor::ShouldRunGame () {
	return shouldRunGame;
}

void dap::Editor::ShowCreateGameObjectPopup () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();

	ImGui::OpenPopup ( "Create GameObject" );
	if ( ImGui::BeginPopupModal ( "Create GameObject", &openCreateGameObjectPopup, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Name", &newGameObjectName );

		const bool NAME_ENTERED = newGameObjectName.find_first_not_of ( ' ' ) != std::string::npos;
		const bool NAME_TAKEN = currentScene->GetGameObject ( newGameObjectName );

		if ( NAME_ENTERED ) {
			if ( NAME_TAKEN )
				ImGui::Text ( "Name taken, GameObject ID will be appended to end" );
			if ( ImGui::Button ( "Create" ) ) {
				SetSelectedGameObject ( currentScene->SpawnGameObject ( newGameObjectName ) );
				openCreateGameObjectPopup = false;
				newGameObjectName = "";
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		} else {
			ImGui::Text ( "Please enter a name" );
		}

		if ( ImGui::Button ( "Cancel" ) ) {
			openCreateGameObjectPopup = false;
			ImGui::CloseCurrentPopup ();
		}
		ImGui::EndPopup ();
	}
}

void dap::Editor::ShowLoadPrefabPopup () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();

	ImGui::OpenPopup ( "Load Prefab" );
	if ( ImGui::BeginPopupModal ( "Load Prefab", &openLoadPrefabPopup, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Path", &loadPrefabPath );
		const bool PREFAB_EXISTS = std::filesystem::exists ( loadPrefabPath );

		if ( PREFAB_EXISTS ) {
			if ( ImGui::Button ( "Load" ) ) {
				SetSelectedGameObject ( currentScene->SpawnPrefab ( loadPrefabPath, true ) );
				openLoadPrefabPopup = false;
				loadPrefabPath = "";
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		} else
			ImGui::Text ( "Prefab not found" );

		if ( ImGui::Button ( "Cancel" ) ) {
			openLoadPrefabPopup = false;
			ImGui::CloseCurrentPopup ();
		}
		ImGui::EndPopup ();
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
		if ( selectedGameObject ) {
			if ( ImGui::MenuItem ( "Save Prefab" ) )
				openSavePrefabPopup = true;
		}
		if ( ImGui::MenuItem ( "Load Prefab" ) )
			openLoadPrefabPopup = true;
		ImGui::Separator ();
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
		ImGui::Checkbox ( "Scene Window", &openSceneWindow );
		ImGui::Separator ();
		ImGui::Checkbox ( "Visualize Colliders", &visualizeColliders );
		ImGui::EndMenu ();
	}

	if ( !shouldRunGame ) {
		if ( ImGui::Button ( "Play" ) ) {
			currentScene->Save ( currentScene->GetPath () );
			shouldRunGame = true;
		}
	} else {
		if ( ImGui::Button ( "Stop" ) )
			shouldRunGame = false;
	}

	ImGui::EndMainMenuBar ();
}

void dap::Editor::ShowSavePrefabPopup () {
	const std::string TITLE = "Save Prefab - " + selectedGameObject->GetName ();
	ImGui::OpenPopup ( TITLE.c_str () );
	if ( ImGui::BeginPopupModal ( TITLE.c_str (), &openSavePrefabPopup, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Path", &savePrefabPath );
		const bool PATH_ENTERED = savePrefabPath.find_first_not_of ( ' ' ) != std::string::npos;
		const bool FILE_EXISTS = std::filesystem::exists ( savePrefabPath );

		if ( PATH_ENTERED ) {
			if ( FILE_EXISTS )
				ImGui::Text ( "Warning: file already exists at path" );
			if ( ImGui::Button ( "Save" ) ) {
				selectedGameObject->SavePrefab ( savePrefabPath );
				openSavePrefabPopup = false;
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		} else
			ImGui::Text ( "Please enter a save path" );
		if ( ImGui::Button ( "Cancel" ) ) {
			openSavePrefabPopup = false;
			ImGui::CloseCurrentPopup ();
		}

		ImGui::EndPopup ();
	}
}

void dap::Editor::ShowSceneWindow () {
	if ( !openSceneWindow )
		return;

	const glm::vec2 MAX_WINDOW_SIZE = dap::Rendering::GetWindowSize ();
	ImGui::SetNextWindowSizeConstraints ( ImVec2 ( 0.0f, 0.0f ), ImVec2 ( MAX_WINDOW_SIZE.x, MAX_WINDOW_SIZE.y + SCENE_WINDOW_MAX_HEIGHT_OFFSET ) );
	if ( !ImGui::Begin ( "GameObjects", &openSceneWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar ) ) {
		ImGui::End ();
		return;
	}

	if ( ImGui::Button ( "Create Object" ) )
		openCreateGameObjectPopup = true;

	if ( selectedGameObject ) {
		ImGui::SameLine ();
		if ( ImGui::Button ( "Delete Object" ) ) {
			delete selectedGameObject;
			SetSelectedGameObject ( NULL );
		}
	}

	if ( ImGui::BeginCombo ( "Selected Object", selectedGameObject ? selectedGameObject->GetName ().c_str () : "" ) ) {
		for ( int i = 0; i < ALL_GAMEOBJECTS->size (); i++ ) {
			dap::GameObject* currentObject = ALL_GAMEOBJECTS->at ( i );
			bool isSelected = currentObject == selectedGameObject;
			if ( ImGui::Selectable ( currentObject->GetName ().c_str (), isSelected ) )
				SetSelectedGameObject ( currentObject );
		}
		ImGui::EndCombo ();
	}

	if ( selectedGameObject ) {
		ImGui::Separator ();
		if ( ImGui::BeginCombo ( "##AddComponentCombo", "Add Component" ) ) {
			for ( const auto& jsonPair : *dap::JSONObject::GetRegisteredFactories () ) {
				if ( ImGui::Selectable ( jsonPair.second->Name.c_str (), false ) ) {
					dap::Component* const newComp = static_cast<dap::Component*>( dap::JSONObject::Factory ( jsonPair.second->TypeID ) );
					selectedGameObject->AddComponent ( newComp );
				}
			}
			ImGui::EndCombo ();
		}
		ImGui::Separator ();
		selectedGameObject->ImGuiWidgetsBegin ();
	}

	ImGui::End ();
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
	ShowSceneWindow ();
	if ( openCreateGameObjectPopup )
		ShowCreateGameObjectPopup ();
	if ( openSavePrefabPopup )
		ShowSavePrefabPopup ();
	if ( openLoadPrefabPopup )
		ShowLoadPrefabPopup ();

	if ( !shouldRunGame ) {
		editorCamera->GetOwner ()->GetTransform ()->Translate ( editorCameraMovement * EDITOR_CAMERA_MOVE_SPEED, false );
		editorCamera->SetOrthoSize ( editorCamera->GetOrthoSize () + dap::Input::GetMouseScroll ().y * -EDITOR_CAMERA_ZOOM_SPEED );
	}
}

bool dap::Editor::VisualizeColliders () const {
	return visualizeColliders;
}
