#pragma once

#include <chrono>
#include <iostream>
#include <map>
#include <thread>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Camera.h"
#include "Game.h"
#include "Input.h"
#include "KeyEvent.h"
#include "Shader.h"

namespace dap {
	class Rendering final {
	public:
		const static unsigned long GetFrame ();
		static int Start ( const int startWidth, const int startHeight, const int frameCap, dap::Game* game );
		static void Stop ();
		static dap::Texture* const GetTexture ( const std::string id );
		static void LoadTexture ( const std::string id, const std::string fileName );
	private:
		static std::map<std::string, dap::Texture*> textures;
		static bool running;
		static float aspectRatio;
		static float time;
		static float delta;
		static unsigned long frame;
		static unsigned int frameRate;
		static Game* cGame;
		static void framebuffer_size_callback ( GLFWwindow* window, int width, int height );
		static void key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods );
		static GLFWwindow* mainWindow;
		static void Render ( GLFWwindow* window, dap::Shader* shader );
	};
}
