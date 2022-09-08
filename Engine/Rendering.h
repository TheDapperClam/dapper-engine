#pragma once

#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include <unordered_map>
#include <string>

namespace dap {
	class Editor;
	class Game;

	class Rendering final {
	private:
		static float AspectRatio;
		static Game* CurrentGame;
		static ImGuiIO* CurrentImGuiIO;
		static GLFWwindow* CurrentWindow;
		static glm::vec2 CurrentWindowSize;
		static float Delta;
		static unsigned long Frame;
		static unsigned short FrameRatesOverFrames[];
		static char FrameRatesOverFramesIndex;
		static bool GameRunning;
		static bool RenderingRunning;
		static double Time;
		static std::string WindowName;

	public:
		static unsigned short GetAverageFrameRate ();
		static float GetDelta ();
		static unsigned long GetFrame ();
		static double GetTime ();
		static glm::vec2 GetWindowSize ();
		static int Start ( std::string name, dap::Game* game );
		static void Stop ();
		static int StructuredException ();
		static void UpdateWindowProperties ();

	private:
		static void BeginImGuiFrame ();
		static void Cleanup ();
		static void EditorLoop ();
		static void GameLoop ( int tickRate, dap::Editor* editor );
		static char GetSizeOfFrameRatesOverFrames ();
		static void RefreshViewport ( int width, int height );

	private:
		static void cursor_pos_callback ( GLFWwindow* window, double xpos, double ypos );
		static void framebuffer_size_callback ( GLFWwindow* window, int width, int height );
		static void key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods );
		static void mouse_button_callback ( GLFWwindow* window, int button, int action, int mods );
		static void Render ( GLFWwindow* window, float interp );
	};
}
