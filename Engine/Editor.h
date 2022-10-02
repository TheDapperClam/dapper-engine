#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

namespace dap {
	class Camera;
	class GameObject;
	struct FactoryMetaData;
	class ImGuiGameObjectEditor;
	class ImGuiTilesetEditor;
	class ImGuiWindow;

	class Editor {
		friend class Rendering;

	private:
		static const std::vector<dap::ImGuiWindow*>* ALL_IMGUI_WINDOWS;
		static const glm::vec3 EDITOR_CAMERA_CLEAR_COLOR;
		static const float EDITOR_CAMERA_MOVE_SPEED;
		static const float EDITOR_CAMERA_ZOOM_DEFAULT;
		static const float EDITOR_CAMERA_ZOOM_SPEED;

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
		bool shouldRunGame = false;
		bool shouldQuit = false;
		std::string saveScenePath = "";
		bool visualizeColliders = true;

	private:
		dap::ImGuiGameObjectEditor* gameObjectEditorWindow = NULL;
		dap::ImGuiTilesetEditor* tilesetEditorWindow = NULL;

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
		void ShowAllImGuiWindows ();
		void ShowMenuBar ();
		dap::GameObject* SpawnGameObject ();
	};
}
