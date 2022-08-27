#include "Editor.h"

#include "imgui.h"
#include "GameObject.h"
#include <iostream>
#include "Scene.h"

dap::Editor* dap::Editor::CurrentEditor = NULL;

dap::Editor* const dap::Editor::GetCurrent () {
	return CurrentEditor;
}

void dap::Editor::SetCurrent ( dap::Editor* editor ) {
	CurrentEditor = editor;
}

void dap::Editor::Tick () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();
	if ( !currentScene )
		return;

	if ( !openSceneWindow )
		return;
	if ( !ImGui::Begin ( ( "Scene - " + currentScene->GetName () ).c_str (), &openSceneWindow, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::End ();
		return;
	}

	if ( ImGui::Button ( "Create Object" ) ) {
		openCreateGameObjectPopup = true;
		ImGui::OpenPopup ( "Create GameObject" );
	}

	if ( ImGui::BeginPopupModal ( "Create GameObject", &openCreateGameObjectPopup ) ) {
		ImGui::Text ( "Hello" );
		ImGui::EndPopup ();
	}

	if ( selectedGameObject ) {
		ImGui::SameLine ();
		if ( ImGui::Button ( "Delete Object" ) ) {
			selectedGameObject->Destroy ();
			selectedGameObject = NULL;
		}
	}

	if ( ImGui::BeginCombo ( "Selected Object", selectedGameObject ? selectedGameObject->GetName ().c_str () : "" ) ) {
		for ( const auto& gameObjPair : *currentScene->GetAllGameObjects () ) {
			dap::GameObject* obj = gameObjPair.second;
			bool isSelected = obj == selectedGameObject;
			if ( ImGui::Selectable ( obj->GetName ().c_str (), isSelected ) )
				selectedGameObject = obj;
		}
		ImGui::EndCombo ();
	}

	if ( selectedGameObject ) {
		ImGui::Separator ();
		if ( ImGui::BeginCombo ( "##label", "" ) ) {
			ImGui::EndCombo ();
		}
		ImGui::SameLine ();
		if ( ImGui::Button ( "Add Component" ) ) {

		}
		ImGui::Separator ();
		selectedGameObject->ImGuiWidgetsBegin ();
	}

	ImGui::End ();
}
