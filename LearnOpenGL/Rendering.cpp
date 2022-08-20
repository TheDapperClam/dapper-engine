#include "Rendering.h"

#include "Camera.h"
#include <chrono>
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Input.h"
#include "InputEvent.h"
#include <iostream>
#include "Resources.h"
#include "TextureAtlas.h"

bool dap::Rendering::running = false;
float dap::Rendering::aspectRatio = 1.0f;
double dap::Rendering::time = 0.0f;
float dap::Rendering::delta = 0.0f;
unsigned short dap::Rendering::frameRatesOverFrames[10];
char dap::Rendering::frameRatesOverFramesIndex = 0;
unsigned long dap::Rendering::frame = 0;
dap::Game* dap::Rendering::cGame = NULL;
GLFWwindow* dap::Rendering::mainWindow = NULL;

void dap::Rendering::cursor_pos_callback ( GLFWwindow* window, double xpos, double ypos ) {
	int halfWinX = 0;
	int halfWinY = 0;
	glfwGetWindowSize ( window, &halfWinX, &halfWinY );
	halfWinX *= 0.5f;
	halfWinY *= 0.5f;
	float normXPos = ( xpos - halfWinX ) / halfWinX;
	float normYPos = -( ypos - halfWinY ) / halfWinY;
	dap::Input::SetMousePos ( normXPos, normYPos );
}

void dap::Rendering::framebuffer_size_callback ( GLFWwindow* window, int width, int height ) {
	glViewport ( 0, 0, width, height );
	aspectRatio = (float) width / (float) height;
}

void dap::Rendering::key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if ( action == GLFW_PRESS ) {
		dap::InputEvent* inputEvent = new dap::InputEvent ( key, scancode, action, mods );
		dap::Input::AddKey ( key, inputEvent );
		dap::Input::SetHeld ( frame, key, true );
	} else if ( action == GLFW_RELEASE ) {
		dap::Input::SetHeld ( frame, key, false );
	}
	cGame->Input ();
}

void dap::Rendering::mouse_button_callback ( GLFWwindow* window, int key, int action, int mods ) {
	if ( action == GLFW_PRESS ) {
		dap::InputEvent* inputEvent = new dap::InputEvent ( key, -1, action, mods );
		dap::Input::AddKey ( key, inputEvent );
		dap::Input::SetHeld ( frame, key, true );
	} else if ( action == GLFW_RELEASE ) {
		dap::Input::SetHeld ( frame, key, false );
	}
	cGame->Input ();
}

unsigned short dap::Rendering::GetAverageFrameRate () {
	unsigned short averageFrameRate = 0;
	for ( const auto& frame : frameRatesOverFrames )
		averageFrameRate += frame;
	return averageFrameRate / GetSizeOfFrameRatesOverFrames ();
}

float dap::Rendering::GetDelta () {
	return delta;
}

unsigned long dap::Rendering::GetFrame () {
	return frame;
}

char dap::Rendering::GetSizeOfFrameRatesOverFrames () {
	return sizeof ( frameRatesOverFrames ) / sizeof ( frameRatesOverFrames[0] );
}

double dap::Rendering::GetTime () {
	return time;
}

void dap::Rendering::Render ( GLFWwindow* window ) {
	frame++;

	auto allCameras = dap::Camera::GetAllCameras ();
	for ( int i = 0; i < allCameras->size (); i++ ) {
		allCameras->at ( i )->Render ( aspectRatio );
	}

	ImGui_ImplOpenGL3_NewFrame ();
	ImGui_ImplGlfw_NewFrame ();
	ImGui::NewFrame ();

	// TODO: render imgui windows here

	ImGui::Render ();
	ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );
	glfwSwapBuffers ( window );
	glfwPollEvents ();
}

int dap::Rendering::Start ( std::string name, const int startWidth, const int startHeight, const int tickRate, dap::Game* game ) {
	if ( running ) {
		std::cout << "[ERROR] Tried to start rendering when already running" << std::endl;
		return -1;
	}

	glfwInit ();
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWwindow* const window = glfwCreateWindow ( startWidth, startHeight, name.c_str (), NULL, NULL );
	if ( window == NULL ) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate ();
		return -1;
	}
	glfwMakeContextCurrent ( window );
	glfwSwapInterval ( 0 );

	if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	running = true;
	mainWindow = window;

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor ();
	const GLFWvidmode* primaryVidMode = glfwGetVideoMode ( primaryMonitor );
	int monitorCenterX = primaryVidMode->width / 2;
	int monitorCenterY = primaryVidMode->height / 2;
	int windowCenterX = startWidth / 2;
	int windowCenterY = startHeight / 2;
	glfwSetWindowPos ( window, monitorCenterX - windowCenterX, monitorCenterY - windowCenterY );

	framebuffer_size_callback ( window, 800, 600 );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
	glfwSetKeyCallback ( window, key_callback );
	glfwSetMouseButtonCallback ( window, mouse_button_callback );
	glfwSetCursorPosCallback ( window, cursor_pos_callback );

	std::cout << "[IMGUI]: Initializing" << std::endl;
	IMGUI_CHECKVERSION ();
	ImGui::CreateContext ();
	ImGuiIO& io = ImGui::GetIO (); (void)io;
	ImGui::StyleColorsDark ();
	ImGui_ImplGlfw_InitForOpenGL ( window, true );
	ImGui_ImplOpenGL3_Init ( "#version 330" );

	cGame = game;
	dap::Resources resources;

	const auto start = std::chrono::steady_clock::now ();
	auto lastFrameTime = start;
	float tickAccumulator = 0.0f;

	cGame->Start ();
	while ( !glfwWindowShouldClose ( window ) && !cGame->ShouldClose () ) {
		auto now = std::chrono::steady_clock::now ();
		time = std::chrono::duration_cast<std::chrono::duration<float>>( now - start ).count ();
		delta = std::chrono::duration_cast<std::chrono::duration<float>>( now - lastFrameTime ).count ();
		lastFrameTime = std::chrono::steady_clock::now ();

		const float currentFrameRate = 1.0f / delta;
		frameRatesOverFrames[frameRatesOverFramesIndex] = currentFrameRate;
		if ( ++frameRatesOverFramesIndex >= GetSizeOfFrameRatesOverFrames () )
			frameRatesOverFramesIndex = 0;

		tickAccumulator += tickRate / currentFrameRate;
		while ( tickAccumulator >= 1.0f ) {
			cGame->Tick ();
			tickAccumulator = std::max ( tickAccumulator - 1.0f, 0.0f );
		}
		Render ( window );
	}
	cGame->End ();

	running = false;
	std::cout << "[IMGUI]: Destroying" << std::endl;
	ImGui_ImplOpenGL3_Shutdown ();
	ImGui_ImplGlfw_Shutdown ();
	ImGui::DestroyContext ();
	std::cout << "[OPENGL]: Destroying" << std::endl;
	glBindVertexArray ( NULL );
	glBindBuffer ( GL_ARRAY_BUFFER, NULL );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, NULL );
	glBindTexture ( GL_TEXTURE_2D, NULL );
	std::cout << "[OPENGL]: Terminating" << std::endl;
	glfwTerminate ();
}

void dap::Rendering::Stop () {
	glfwSetWindowShouldClose ( mainWindow, true );
}
