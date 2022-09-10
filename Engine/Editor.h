#pragma once

#include <string>
#include <vector>

namespace dap {
	class GameObject;
	struct FactoryMetaData;

	class Editor {
		friend class Rendering;

	private:
		static const std::vector<dap::GameObject*>* ALL_GAMEOBJECTS;
		static const float SCENE_WINDOW_MAX_HEIGHT_OFFSET;

	private:
		static dap::Editor* CurrentEditor;

	public:
		static dap::Editor* const GetCurrent ();
		static void SetCurrent ( dap::Editor* editor );

	private:
		std::string loadPrefabPath = "";
		std::string newGameObjectName = "";
		bool openCreateGameObjectPopup = false;
		bool openLoadPrefabPopup = false;
		bool openSceneWindow = true;
		bool openSavePrefabPopup = false;
		bool shouldRunGame = false;
		bool shouldQuit = false;
		std::string selectedGameObjectName = "";
		dap::GameObject* selectedGameObject = 0;
		std::string savePrefabPath = "";
		std::string saveScenePath = "";
		bool visualizeColliders = true;

	public:
		void Quit ();
		void RefreshSelected ();
		bool ShouldRunGame ();
		bool ShouldQuit ();
		bool VisualizeColliders () const;

	private:
		void Tick ();
		void SetSelectedGameObject ( dap::GameObject* object );
		void ShowCreateGameObjectPopup ();
		void ShowLoadPrefabPopup ();
		void ShowMenuBar ();
		void ShowSavePrefabPopup ();
		void ShowSceneWindow ();
	};
}
