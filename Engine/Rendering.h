#pragma once


#include <GLFW/glfw3.h>
#include "imgui.h"
#include <map>
#include <string>

namespace dap {
	class Game;

	class Rendering final {
	private:
		static bool Running;
		static float AspectRatio;
		static double Time;
		static float Delta;
		static unsigned long Frame;
		static unsigned short FrameRatesOverFrames[];
		static char FrameRatesOverFramesIndex;
		static Game* CurrentGame;
		static GLFWwindow* CurrentWindow;
		static std::string WindowName;
		static ImGuiIO* CurrentImGuiIO;

	public:
		static unsigned short GetAverageFrameRate ();
		static float GetDelta ();
		static unsigned long GetFrame ();
		static bool GetRunning ();
		static double GetTime ();
		static int Start ( std::string name, const int tickRate, dap::Game* game );
		static void Stop ();
		static int StructuredException ();
		static void UpdateWindowProperties ();

	private:
		static void BeginImGuiFrame ();
		static void Cleanup ();
		static char GetSizeOfFrameRatesOverFrames ();
		static void RefreshViewport ( int width, int height );
		static void RenderLoop ( int tickRate );

	private:
		static void cursor_pos_callback ( GLFWwindow* window, double xpos, double ypos );
		static void framebuffer_size_callback ( GLFWwindow* window, int width, int height );
		static void key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods );
		static void mouse_button_callback ( GLFWwindow* window, int button, int action, int mods );
		static void Render ( GLFWwindow* window );
	};
}
