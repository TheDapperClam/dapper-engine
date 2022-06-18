#pragma once


#include "Game.h"
#include <GLFW/glfw3.h>
#include <map>
#include "Shader.h"
#include "Texture.h"

namespace dap {
	class Rendering final {
	private:
		static std::map<std::string, dap::Texture*> textures;
		static bool running;
		static float aspectRatio;
		static float time;
		static float delta;
		static unsigned long frame;
		static unsigned int frameRate;
		static Game* cGame;
		static GLFWwindow* mainWindow;

	private:
		static void cursor_pos_callback ( GLFWwindow* window, double xpos, double ypos );
		static void framebuffer_size_callback ( GLFWwindow* window, int width, int height );
		static void key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods );
		static void mouse_button_callback ( GLFWwindow* window, int button, int action, int mods );
		static void Render ( GLFWwindow* window, dap::Shader* shader );

	public:
		static unsigned long GetFrame ();
		static dap::Texture* const GetTexture ( const std::string id );
		static void LoadTexture ( const std::string id, const std::string fileName );
		static int Start ( const int startWidth, const int startHeight, const int frameCap, dap::Game* game );
		static void Stop ();
	};
}
