#pragma once

#include "ImGuiWindow.h"
#include <string>
#include <vector>

namespace dap {
	class GameObject;

	class ImGuiGameObjectEditor : public dap::ImGuiWindow {
	private:
		static const std::vector<dap::GameObject*>* ALL_GAMEOBJECTS;

	private:
		std::string loadPrefabPath = "";
		std::string newGameObjectName = "";
		bool openCreateGameObjectPopup = false;
		bool openLoadPrefabPopup = false;
		bool openSavePrefabPopup = false;
		std::string savePrefabPath = "";
		std::string selectedGameObjectName = "";
		dap::GameObject* selectedGameObject = 0;

	public:
		ImGuiGameObjectEditor ( std::string title );

	public:
		dap::GameObject* GetSelectedGameObject ();
		std::string GetSelectedGameObjectName ();
		void SetSelectedGameObject ( dap::GameObject* object );

	private:
		void ShowCreateGameObjectPopup ();
		void ShowLoadPrefabPopup ();
		void ShowSavePrefabPopup ();

	protected:
		virtual void ContentBegin () override;
	};
};
