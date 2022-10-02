#include "ImGuiGameObjectEditor.h"

#include <filesystem>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include "Rendering.h"
#include "Scene.h"

const std::vector<dap::GameObject*>* dap::ImGuiGameObjectEditor::ALL_GAMEOBJECTS = dap::Scene::GetAllGameObjects ();
const float dap::ImGuiGameObjectEditor::SCENE_WINDOW_MAX_HEIGHT_OFFSET = -100.0f;

dap::ImGuiGameObjectEditor::ImGuiGameObjectEditor ( std::string title ) : dap::ImGuiWindow ( title ) {

}

void dap::ImGuiGameObjectEditor::ContentBegin () {
	const glm::vec2 MAX_WINDOW_SIZE = dap::Rendering::GetWindowSize ();
	ImGui::SetNextWindowSizeConstraints ( ImVec2 ( 0.0f, 0.0f ), ImVec2 ( MAX_WINDOW_SIZE.x, MAX_WINDOW_SIZE.y + SCENE_WINDOW_MAX_HEIGHT_OFFSET ) );

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
}

dap::GameObject* dap::ImGuiGameObjectEditor::GetSelectedGameObject () {
	return selectedGameObject;
}

std::string dap::ImGuiGameObjectEditor::GetSelectedGameObjectName () {
	return selectedGameObjectName;
}

void dap::ImGuiGameObjectEditor::SetSelectedGameObject ( dap::GameObject* object ) {
	selectedGameObject = object;
	selectedGameObjectName = object ? object->GetName () : "";
	savePrefabPath = object ? object->GetPrefabPath () : "";
}

void dap::ImGuiGameObjectEditor::ShowCreateGameObjectPopup () {
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

void dap::ImGuiGameObjectEditor::ShowLoadPrefabPopup () {
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

void dap::ImGuiGameObjectEditor::ShowSavePrefabPopup () {
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
