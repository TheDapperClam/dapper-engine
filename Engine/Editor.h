#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

namespace dap {
	class Camera;
	class GameObject;
	struct FactoryMetaData;

	class Editor {
		friend class Rendering;

	private:
		static const std::vector<dap::GameObject*>* ALL_GAMEOBJECTS;
		static const glm::vec3 EDITOR_CAMERA_CLEAR_COLOR;
		static const float EDITOR_CAMERA_MOVE_SPEED;
		static const float EDITOR_CAMERA_ZOOM_DEFAULT;
		static const float EDITOR_CAMERA_ZOOM_SPEED;
		static const float SCENE_WINDOW_MAX_HEIGHT_OFFSET;

	private:
		static dap::Editor* CurrentEditor;

	public:
		static dap::Editor* const GetCurrent ();
		static void SetCurrent ( dap::Editor* editor );

	private:
		dap::Camera* editorCamera = NULL;
		glm::vec2 editorCameraMovement;
		std::vector<dap::GameObject*> editorGameObjects = {};
		bool leftMouseDown = false;
		bool rightMouseDown = false;
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
		~Editor ();

	public:
		void BeginEdit ();
		void GameEnded ();
		void Initialize ();
		void Quit ();
		void RefreshSelected ();
		bool ShouldRunGame ();
		bool ShouldQuit ();
		bool VisualizeColliders () const;

	private:
		void Input ();
		void Tick ();
		void SetSelectedGameObject ( dap::GameObject* object );
		void ShowCreateGameObjectPopup ();
		void ShowLoadPrefabPopup ();
		void ShowMenuBar ();
		void ShowSavePrefabPopup ();
		void ShowSceneWindow ();
		dap::GameObject* SpawnGameObject ();
	};
}
