#include "Rendering.h"

#include "Camera.h"
#include <chrono>
#include <glad/glad.h>
#include "Input.h"
#include "InputEvent.h"
#include <iostream>
#include "Resources.h"

std::map<std::string, dap::Texture*> dap::Rendering::textures = {};
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

dap::Texture* const dap::Rendering::GetTexture ( const std::string id ) {
	if ( !textures.count ( id ) )
		return NULL;
	return textures[id];
}

double dap::Rendering::GetTime () {
	return time;
}

void dap::Rendering::LoadTexture ( const std::string id, const std::string fileName ) {
	if ( textures.contains ( id ) ) {
		std::cout << "Texture with name " + id + " already exists" << std::endl;
		return;
	}
	dap::Texture* tex = new dap::Texture ( fileName, id );
	textures.insert ( std::pair<std::string, dap::Texture*> ( id, tex ) );
}

void dap::Rendering::Render ( GLFWwindow* window, dap::Shader* shader ) {
	frame++;
	glClear ( GL_COLOR_BUFFER_BIT );
	glfwPollEvents ();

	shader->Use ();
	auto allCameras = dap::Camera::GetAllCameras ();
	for ( int i = 0; i < allCameras->size (); i++ ) {
		allCameras->at ( i )->Render ( shader->GetProgram (), aspectRatio );
	}
	glfwSwapBuffers ( window );
}

int dap::Rendering::Start ( const int startWidth, const int startHeight, const int frameCap, dap::Game* game ) {
	if ( running ) {
		std::cout << "[ERROR] Tried to start rendering when already running" << std::endl;
		return -1;
	}

	glfwInit ();
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	//glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // Mac OS X only

	GLFWwindow* const window = glfwCreateWindow ( startWidth, startHeight, "LearnOpenGL", NULL, NULL );
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
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );

	glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
	glfwSetKeyCallback ( window, key_callback );
	glfwSetMouseButtonCallback ( window, mouse_button_callback );
	glfwSetCursorPosCallback ( window, cursor_pos_callback );

	//dap::Shader defaultShader ( "Shaders/shader.vert", "Shaders/shader.frag" );
	dap::Shader defaultShader ( "Shaders/shader_default.shader" );
	defaultShader.Use ();
	cGame = game;

	const auto start = std::chrono::steady_clock::now ();
	auto lastFrameTime = start;
	const float renderInterval = 1.0f / frameCap;
	double nextRenderTime = 0;

	dap::Resources resources;
	cGame->Start ();
	while ( !glfwWindowShouldClose ( window ) && !cGame->ShouldClose () ) {
		auto now = std::chrono::steady_clock::now ();
		time = std::chrono::duration_cast<std::chrono::duration<float>>( now - start ).count ();

		cGame->Tick ();
		if ( time >= nextRenderTime ) {
			delta = std::chrono::duration_cast<std::chrono::duration<float>>( now - lastFrameTime ).count ();
			lastFrameTime = std::chrono::steady_clock::now ();
			frameRatesOverFrames[frameRatesOverFramesIndex] = round ( 1.0f / delta );
			if ( ++frameRatesOverFramesIndex >= GetSizeOfFrameRatesOverFrames () )
				frameRatesOverFramesIndex = 0;
			nextRenderTime = time + renderInterval;
			Render ( window, &defaultShader );
		}
		glfwSetWindowTitle ( window, ( "FPS: " + std::to_string ( GetAverageFrameRate () ) ).c_str () );
	}
	cGame->End ();

	glBindVertexArray ( NULL );
	glBindBuffer ( GL_ARRAY_BUFFER, NULL );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, NULL );
	glBindTexture ( GL_TEXTURE_2D, NULL );
	for ( auto pair : textures ) {
		const std::string key = pair.first;
		for ( int i = 0; i < textures.size (); i++ ) {
			delete textures[key];
			textures[key] = NULL;
		}
	}
	textures.clear ();
	running = false;
	glfwTerminate ();
}

void dap::Rendering::Stop () {
	glfwSetWindowShouldClose ( mainWindow, true );
}
