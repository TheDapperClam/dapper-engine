#pragma once


#include "Game.h"
#include <GLFW/glfw3.h>
#include <map>
#include <string>

namespace dap {
	class Rendering final {
	private:
		static bool running;
		static float aspectRatio;
		static double time;
		static float delta;
		static unsigned long frame;
		static unsigned short frameRatesOverFrames[];
		static char frameRatesOverFramesIndex;
		static Game* currentGame;
		static GLFWwindow* currentWindow;
		static std::string windowName;

	public:
		static unsigned short GetAverageFrameRate ();
		static float GetDelta ();
		static unsigned long GetFrame ();
		static bool GetRunning ();
		static double GetTime ();
		static int Start ( std::string name, const int tickRate, dap::Game* game );
		static void Stop ();
		static void UpdateWindowProperties ();

	private:
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
