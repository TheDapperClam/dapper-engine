#include "Rendering.h"

#include "Camera.h"
#include <chrono>
#include "Config.h"
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Input.h"
#include "InputEvent.h"
#include <iostream>
#include "Resources.h"

bool dap::Rendering::running = false;
float dap::Rendering::aspectRatio = 1.0f;
double dap::Rendering::time = 0.0f;
float dap::Rendering::delta = 0.0f;
unsigned short dap::Rendering::frameRatesOverFrames[10];
char dap::Rendering::frameRatesOverFramesIndex = 0;
unsigned long dap::Rendering::frame = 0;
dap::Game* dap::Rendering::currentGame = NULL;
GLFWwindow* dap::Rendering::currentWindow = NULL;
std::string dap::Rendering::windowName = "";

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
	currentGame->Input ();
}

void dap::Rendering::mouse_button_callback ( GLFWwindow* window, int key, int action, int mods ) {
	if ( action == GLFW_PRESS ) {
		dap::InputEvent* inputEvent = new dap::InputEvent ( key, -1, action, mods );
		dap::Input::AddKey ( key, inputEvent );
		dap::Input::SetHeld ( frame, key, true );
	} else if ( action == GLFW_RELEASE ) {
		dap::Input::SetHeld ( frame, key, false );
	}
	currentGame->Input ();
}

void dap::Rendering::Cleanup () {
	std::cout << "[OPENGL]: Destroying" << std::endl;
	glBindVertexArray ( NULL );
	glBindBuffer ( GL_ARRAY_BUFFER, NULL );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, NULL );
	glBindTexture ( GL_TEXTURE_2D, NULL );
	glfwTerminate ();
	std::cout << "[IMGUI]: Destroying" << std::endl;
	//ImGui_ImplOpenGL3_Shutdown ();
	//ImGui_ImplGlfw_Shutdown ();
	//ImGui::DestroyContext ();
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

bool dap::Rendering::GetRunning () {
	return running;
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

void dap::Rendering::RenderLoop ( int tickRate ) {
	const auto TIME_START = std::chrono::steady_clock::now ();
	auto lastFrameTime = TIME_START;
	float tickAccumulator = 0.0f;

	currentGame->Start ();
	while ( !glfwWindowShouldClose ( currentWindow ) && !currentGame->ShouldClose () ) {
		const auto TIME_NOW = std::chrono::steady_clock::now ();
		time = std::chrono::duration_cast<std::chrono::duration<float>>( TIME_NOW - TIME_START ).count ();
		delta = std::chrono::duration_cast<std::chrono::duration<float>>( TIME_NOW - lastFrameTime ).count ();
		lastFrameTime = std::chrono::steady_clock::now ();

		const float currentFrameRate = 1.0f / delta;
		frameRatesOverFrames[frameRatesOverFramesIndex] = currentFrameRate;
		if ( ++frameRatesOverFramesIndex >= GetSizeOfFrameRatesOverFrames () )
			frameRatesOverFramesIndex = 0;

		tickAccumulator += tickRate / currentFrameRate;
		while ( tickAccumulator >= 1.0f ) {
			currentGame->Tick ();
			tickAccumulator = std::max ( tickAccumulator - 1.0f, 0.0f );
		}
		Render ( currentWindow );
	}
	currentGame->End ();
}

int dap::Rendering::Start ( std::string name, const int tickRate, dap::Game* game ) {
	if ( running ) {
		std::cout << "[ERROR::RENDERING] Tried to start rendering when already running" << std::endl;
		return -1;
	}
	windowName = name;
	currentGame = game;

	// Init GLFW
	glfwInit ();
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor ();
	const GLFWvidmode* PRIMARY_VIDEO_MODE = glfwGetVideoMode ( primaryMonitor );
	const bool FULLSCREEN = dap::Config::GetProperty<bool> ( "bFullscreen" );
	const int WINDOW_START_WIDTH = dap::Config::GetProperty<int> ( "iWindowW" );
	const int WINDOW_START_HEIGHT = dap::Config::GetProperty<int> ( "iWindowH" );
	const int MONITOR_CENTER_X = PRIMARY_VIDEO_MODE->width / 2;
	const int MONITOR_CENTER_Y = PRIMARY_VIDEO_MODE->height / 2;
	const int WINDOW_CENTER_X = WINDOW_START_WIDTH / 2;
	const int WINDOW_CENTER_Y = WINDOW_START_HEIGHT / 2;

	currentWindow = glfwCreateWindow ( WINDOW_START_WIDTH, WINDOW_START_HEIGHT, windowName.c_str (), NULL, NULL );
	if ( !currentWindow ) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate ();
		return -1;
	}
	glfwMakeContextCurrent ( currentWindow );

	// Init GLAD
	if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// Init ImGui
	std::cout << "[IMGUI]: Initializing" << std::endl;
	IMGUI_CHECKVERSION ();
	ImGui::CreateContext ();
	ImGuiIO& io = ImGui::GetIO (); (void)io;
	ImGui::StyleColorsDark ();
	ImGui_ImplGlfw_InitForOpenGL ( currentWindow, true );
	ImGui_ImplOpenGL3_Init ( "#version 330" );

	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glfwSetWindowPos ( currentWindow, MONITOR_CENTER_X - WINDOW_CENTER_X, MONITOR_CENTER_Y - WINDOW_CENTER_Y );
	glfwSwapInterval ( dap::Config::GetProperty<bool> ( "bVSync" ) );

	if ( FULLSCREEN ) {
		glfwSetWindowMonitor ( currentWindow, primaryMonitor, 0, 0, PRIMARY_VIDEO_MODE->width, PRIMARY_VIDEO_MODE->height, PRIMARY_VIDEO_MODE->refreshRate );
		framebuffer_size_callback ( currentWindow, PRIMARY_VIDEO_MODE->width, PRIMARY_VIDEO_MODE->height );
	} else {
		framebuffer_size_callback ( currentWindow, WINDOW_START_WIDTH, WINDOW_START_HEIGHT );
	}
	glfwSetFramebufferSizeCallback ( currentWindow, framebuffer_size_callback );
	glfwSetKeyCallback ( currentWindow, key_callback );
	glfwSetMouseButtonCallback ( currentWindow, mouse_button_callback );
	glfwSetCursorPosCallback ( currentWindow, cursor_pos_callback );

	running = true;
	dap::Resources resources;
	RenderLoop ( tickRate );
	running = false;
	Cleanup ();
}

void dap::Rendering::Stop () {
	glfwSetWindowShouldClose ( currentWindow, true );
	running = false;
}
