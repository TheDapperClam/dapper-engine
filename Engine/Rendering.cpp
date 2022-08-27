#include "Rendering.h"

#include "Camera.h"
#include <chrono>
#include "Config.h"
#include "Editor.h"
#include "Game.h"
#include <glad/glad.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Input.h"
#include "InputEvent.h"
#include <iostream>
#include "Resources.h"

bool dap::Rendering::Running = false;
float dap::Rendering::AspectRatio = 1.0f;
double dap::Rendering::Time = 0.0f;
float dap::Rendering::Delta = 0.0f;
unsigned short dap::Rendering::FrameRatesOverFrames[10];
char dap::Rendering::FrameRatesOverFramesIndex = 0;
unsigned long dap::Rendering::Frame = 0;
dap::Game* dap::Rendering::CurrentGame = NULL;
GLFWwindow* dap::Rendering::CurrentWindow = NULL;
std::string dap::Rendering::WindowName = "";
ImGuiIO* dap::Rendering::CurrentImGuiIO = NULL;

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
	RefreshViewport ( width, height );
}

void dap::Rendering::key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if ( action == GLFW_PRESS ) {
		dap::InputEvent* inputEvent = new dap::InputEvent ( key, scancode, action, mods );
		dap::Input::AddKey ( key, inputEvent );
		dap::Input::SetHeld ( Frame, key, true );
	} else if ( action == GLFW_RELEASE ) {
		dap::Input::SetHeld ( Frame, key, false );
	}
	if ( !CurrentImGuiIO->WantCaptureKeyboard )
		CurrentGame->Input ();
}

void dap::Rendering::mouse_button_callback ( GLFWwindow* window, int key, int action, int mods ) {
	if ( action == GLFW_PRESS ) {
		dap::InputEvent* inputEvent = new dap::InputEvent ( key, -1, action, mods );
		dap::Input::AddKey ( key, inputEvent );
		dap::Input::SetHeld ( Frame, key, true );
	} else if ( action == GLFW_RELEASE ) {
		dap::Input::SetHeld ( Frame, key, false );
	}
	if ( !CurrentImGuiIO->WantCaptureMouse )
		CurrentGame->Input ();
}

void dap::Rendering::BeginImGuiFrame () {
	ImGui_ImplOpenGL3_NewFrame ();
	ImGui_ImplGlfw_NewFrame ();
	ImGui::NewFrame ();
}

void dap::Rendering::Cleanup () {
	std::cout << "[OPENGL]: Destroying" << std::endl;
	glBindVertexArray ( NULL );
	glBindBuffer ( GL_ARRAY_BUFFER, NULL );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, NULL );
	glBindTexture ( GL_TEXTURE_2D, NULL );
	glfwTerminate ();
	std::cout << "[IMGUI]: Destroying" << std::endl;
	ImGui_ImplOpenGL3_Shutdown ();
	ImGui_ImplGlfw_Shutdown ();
	ImGui::DestroyContext ();
}

unsigned short dap::Rendering::GetAverageFrameRate () {
	unsigned short averageFrameRate = 0;
	for ( const auto& frame : FrameRatesOverFrames )
		averageFrameRate += frame;
	return averageFrameRate / GetSizeOfFrameRatesOverFrames ();
}

float dap::Rendering::GetDelta () {
	return Delta;
}

unsigned long dap::Rendering::GetFrame () {
	return Frame;
}

char dap::Rendering::GetSizeOfFrameRatesOverFrames () {
	return sizeof ( FrameRatesOverFrames ) / sizeof ( FrameRatesOverFrames[0] );
}

double dap::Rendering::GetTime () {
	return Time;
}

bool dap::Rendering::GetRunning () {
	return Running;
}

void dap::Rendering::Render ( GLFWwindow* window ) {
	Frame++;
	auto allCameras = dap::Camera::GetAllCameras ();
	bool wasFrameCleared = false;
	for ( int i = 0; i < allCameras->size (); i++ ) {
		bool rendered = allCameras->at ( i )->Render ( AspectRatio );
		if ( !wasFrameCleared && rendered )
			wasFrameCleared = true;
	}
	if ( !wasFrameCleared ) {
		glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear ( GL_COLOR_BUFFER_BIT );
	}
	ImGui::Render ();
	ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );
	ImGui::EndFrame ();
	glfwSwapBuffers ( window );
	glfwPollEvents ();
}

void dap::Rendering::RefreshViewport ( int width, int height ) {
	glViewport ( 0, 0, width, height );
	AspectRatio = (float)width / (float)height;
}

void dap::Rendering::RenderLoop ( int tickRate ) {
	const auto TIME_START = std::chrono::steady_clock::now ();
	auto lastFrameTime = TIME_START;
	float tickAccumulator = 0.0f;

	CurrentGame->Start ();
	while ( !glfwWindowShouldClose ( CurrentWindow ) && !CurrentGame->ShouldClose () ) {
		const auto TIME_NOW = std::chrono::steady_clock::now ();
		Time = std::chrono::duration_cast<std::chrono::duration<float>>( TIME_NOW - TIME_START ).count ();
		Delta = std::chrono::duration_cast<std::chrono::duration<float>>( TIME_NOW - lastFrameTime ).count ();
		lastFrameTime = std::chrono::steady_clock::now ();

		const float currentFrameRate = 1.0f / Delta;
		FrameRatesOverFrames[FrameRatesOverFramesIndex] = currentFrameRate;
		if ( ++FrameRatesOverFramesIndex >= GetSizeOfFrameRatesOverFrames () )
			FrameRatesOverFramesIndex = 0;

		BeginImGuiFrame ();
		tickAccumulator += tickRate / currentFrameRate;
		while ( tickAccumulator >= 1.0f ) {
			CurrentGame->Tick ();
			tickAccumulator = std::max ( tickAccumulator - 1.0f, 0.0f );
		}
		if ( dap::Editor::GetCurrent () )
			dap::Editor::GetCurrent ()->Tick ();
		Render ( CurrentWindow );
	}
	CurrentGame->End ();
}

int dap::Rendering::Start ( std::string name, const int tickRate, dap::Game* game ) {
	if ( Running ) {
		std::cout << "[ERROR::RENDERING] Tried to start rendering when already running" << std::endl;
		return -1;
	}
	WindowName = name;
	CurrentGame = game;

	// Init GLFW
	glfwInit ();
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint ( GLFW_RESIZABLE, GL_FALSE );

	CurrentWindow = glfwCreateWindow ( 800, 600, WindowName.c_str (), NULL, NULL );
	if ( !CurrentWindow ) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate ();
		return -1;
	}
	glfwMakeContextCurrent ( CurrentWindow );

	// Init GLAD
	if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	// GLFW callbacks must be assigned before initiating ImGui so that we don't override ImGui's callbacks
	glfwSetFramebufferSizeCallback ( CurrentWindow, framebuffer_size_callback );
	glfwSetKeyCallback ( CurrentWindow, key_callback );
	glfwSetMouseButtonCallback ( CurrentWindow, mouse_button_callback );
	glfwSetCursorPosCallback ( CurrentWindow, cursor_pos_callback );

	// Init ImGui
	std::cout << "[IMGUI]: Initializing" << std::endl;
	IMGUI_CHECKVERSION ();
	ImGui::CreateContext ();
	ImGuiIO& io = ImGui::GetIO (); (void)io;
	CurrentImGuiIO = &io;
	ImGui::StyleColorsDark ();
	ImGui_ImplGlfw_InitForOpenGL ( CurrentWindow, true );
	ImGui_ImplOpenGL3_Init ( "#version 330" );
	io.IniFilename = NULL;
	io.LogFilename = NULL;

	Running = true;
	dap::Resources resources;
	UpdateWindowProperties ();
	RenderLoop ( tickRate );
	Running = false;
	Cleanup ();
}

void dap::Rendering::Stop () {
	glfwSetWindowShouldClose ( CurrentWindow, true );
	Running = false;
}

int dap::Rendering::StructuredException () {
	return 0;
}

void dap::Rendering::UpdateWindowProperties () {
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor ();
	const GLFWvidmode* PRIMARY_VIDEO_MODE = glfwGetVideoMode ( primaryMonitor );
	const bool FULLSCREEN = dap::Config::GetProperty ( "rendering", "bFullscreen" ) == "true";
	const bool VSYNC = dap::Config::GetProperty ( "rendering", "bVSync" ) == "true";
	const int WINDOW_DEFAULT_WIDTH = std::stoi ( dap::Config::GetProperty ( "rendering", "iWindowW" ) );
	const int WINDOW_DEFAULT_HEIGHT = std::stoi ( dap::Config::GetProperty ( "rendering", "iWindowH" ) );
	const int MONITOR_CENTER_X = PRIMARY_VIDEO_MODE->width / 2;
	const int MONITOR_CENTER_Y = PRIMARY_VIDEO_MODE->height / 2;
	const int WINDOW_CENTER_X = WINDOW_DEFAULT_WIDTH / 2;
	const int WINDOW_CENTER_Y = WINDOW_DEFAULT_HEIGHT / 2;

	GLFWmonitor* windowFinalMonitor = FULLSCREEN ? primaryMonitor : NULL;
	const int WINDOW_FINAL_WIDTH = FULLSCREEN ? PRIMARY_VIDEO_MODE->width : WINDOW_DEFAULT_WIDTH;
	const int WINDOW_FINAL_HEIGHT = FULLSCREEN ? PRIMARY_VIDEO_MODE->height : WINDOW_DEFAULT_HEIGHT;
	const int WINDOW_FINAL_POS_X = MONITOR_CENTER_X - WINDOW_CENTER_Y;
	const int WINDOW_FINAL_POS_Y = MONITOR_CENTER_Y - WINDOW_CENTER_Y;

	RefreshViewport ( WINDOW_FINAL_WIDTH, WINDOW_FINAL_HEIGHT );
	glfwSetWindowMonitor ( 
		CurrentWindow,
		windowFinalMonitor,
		WINDOW_FINAL_POS_X,
		WINDOW_FINAL_POS_Y,
		WINDOW_FINAL_WIDTH,
		WINDOW_FINAL_HEIGHT,
		PRIMARY_VIDEO_MODE->refreshRate
	);
	glfwSwapInterval ( VSYNC );
}
