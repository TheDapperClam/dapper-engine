#include "Camera.h"

#include <Component.h>
#include <GameObject.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuiDrawCallStepper.h>
#include <limits>
#include <RenderObject.h>
#include <Shader.h>
#include <Transform.h>

const std::map<int, std::vector<dap::RenderObject*>>* dap::Camera::ALL_RENDERERS = dap::RenderObject::GetAllRenderers ();
const float dap::Camera::MIN_ORTHO_SIZE = 0.1f;
const float dap::Camera::NEAR_PLANE = 0.1f;
const float dap::Camera::FAR_PLANE = 100.0f;

dap::Camera* dap::Camera::Current = NULL;
unsigned int dap::Camera::DrawCallCount = 0;
dap::Camera* dap::Camera::StartingCamera = NULL;

dap::Camera::~Camera () {
	BaseEnd ();
}

void dap::Camera::End () {
	if ( Current == this )
		SetCurrent ( NULL );
	if ( StartingCamera == this )
		SetStartingCamera ( NULL );
}

dap::Camera* const dap::Camera::GetCurrent () {
	return Current;
}

float dap::Camera::GetOrthoSize () {
	return orthoSize;
}

dap::Camera* const dap::Camera::GetStartingCamera () {
	return StartingCamera;
}

void dap::Camera::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	bool isStarting = IsStartingCamera ();
	if ( ImGui::Checkbox ( "Starting Camera", &isStarting ) ) {
		if ( isStarting )
			SetStartingCamera ( this );
		else
			SetStartingCamera ( NULL );
	}
	if ( ImGui::ColorEdit3 ( "Clear Color", &clearColor.r ) )
		SetClearColor ( clearColor.r, clearColor.g, clearColor.b );
	if ( ImGui::DragFloat ( "Ortho Size", &orthoSize, 0.1f ) )
		SetOrthoSize ( orthoSize );
}

void dap::Camera::Initialize () {
	dap::Component::Initialize ();
	GetOwner ()->GetTransform ()->SetZOffset ( -1.0f );
	GetOwner ()->GetTransform ()->SetMatrixTranslateScaling ( glm::vec2 ( -1.0f, -1.0f ) );
}

bool dap::Camera::IsStartingCamera () {
	return StartingCamera == this;
}

void dap::Camera::ResetDrawCallCount () {
	DrawCallCount = 0;
}

glm::vec2 dap::Camera::ScreenPointToWorld ( glm::vec2 point ) {
	const dap::Transform* CAMERA_TRANSFORM = GetOwner ()->GetTransform ();
	const glm::vec2 CAMERA_POSITION = CAMERA_TRANSFORM->GetPosition ();
	glm::vec4 point4D ( point.x, point.y, 0.0f, 1.0f );
	glm::mat4 inverseMat4 = glm::inverse ( projectionOrtho * CAMERA_TRANSFORM->GetMatrix () );
	point4D = point4D * inverseMat4;
	point4D.x += CAMERA_POSITION.x;
	point4D.y += CAMERA_POSITION.y;
	return point4D;
}

void dap::Camera::SetClearColor ( float r, float g, float b ) {
	clearColor = glm::vec3 ( r, g, b );
}

void dap::Camera::SetCurrent ( dap::Camera* camera ) {
	Current = camera;
}

void dap::Camera::SetOrthoSize ( const float newSize ) {
	orthoSize = std::max ( MIN_ORTHO_SIZE, newSize );
}

void dap::Camera::SetStartingCamera ( dap::Camera* camera ) {
	StartingCamera = camera;
}

bool dap::Camera::Render ( const float aspectRatio, const float interp ) {
	if ( !isEnabled )
		return false;

	bool stopRendering = false;
	glClearColor ( clearColor.x, clearColor.y, clearColor.z, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	projectionOrtho = glm::ortho ( -orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, NEAR_PLANE, FAR_PLANE );
	for ( const auto& RENDERER_PAIR : *ALL_RENDERERS ) {
		if ( stopRendering )
			break;
		for ( const auto& RENDERER : RENDERER_PAIR.second ) {
			if ( dap::ImGuiDrawCallStepper::GetIsStepping () && dap::ImGuiDrawCallStepper::GetCurrentStep () <= DrawCallCount ) {
				stopRendering = true;
				break;
			}
			if ( !RENDERER->GetEnabled () || !RENDERER->Use () )
				continue;
			DrawCallCount++;
			dap::Shader* const shader = RENDERER->GetShader ();
			const GLuint* program = shader->GetProgram ();
			GLint uTransformLocation = glGetUniformLocation ( *program, "uTransform" );
			GLint uViewLocation = glGetUniformLocation ( *program, "uView" );
			GLint uProjectionLocation = glGetUniformLocation ( *program, "uProjection" );
			dap::Transform* modelTransform = RENDERER->GetOwner ()->GetTransform ();
			modelTransform->Interpolate ( interp );
			glUniformMatrix4fv ( uViewLocation, 1, GL_FALSE, glm::value_ptr ( owner->GetTransform ()->GetMatrix () ) );
			glUniformMatrix4fv ( uProjectionLocation, 1, GL_FALSE, glm::value_ptr ( projectionOrtho ) );
			glUniformMatrix4fv ( uTransformLocation, 1, GL_FALSE, glm::value_ptr ( modelTransform->GetMatrix () ) );
			glDrawElements ( RENDERER->GetDrawMode (), RENDERER->GetElementsSize (), GL_UNSIGNED_INT, NULL );
		}
	}
	return true;
}

void dap::Camera::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );
	if ( json->contains ( "clearColor" ) ) {
		const auto deserializedClearColor = json->at ( "clearColor" );
			SetClearColor ( deserializedClearColor[0], deserializedClearColor[1], deserializedClearColor[2] );
	}
	if ( json->contains ( "orthosize" ) )
		SetOrthoSize ( json->at ( "orthosize" ) );
	if ( json->contains ( "isStartingCamera" ) && json->at ( "isStartingCamera" ) )
		SetStartingCamera ( this );
}

nlohmann::json dap::Camera::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["clearColor"] = { clearColor.x, clearColor.y, clearColor.z };
	json["orthosize"] = orthoSize;
	json["isStartingCamera"] = IsStartingCamera ();
	return json;
}
