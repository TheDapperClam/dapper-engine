#pragma once

#include <string>

namespace dap {
	class GameObject;
	struct FactoryMetaData;

	class Editor {
		friend class Rendering;

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
