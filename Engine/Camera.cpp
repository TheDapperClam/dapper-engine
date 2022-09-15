#include "Camera.h"
#include "Component.h"
#include "GameObject.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include <limits>
#include "RenderObject.h"
#include "Shader.h"
#include "Transform.h"

const std::map<int, std::vector<dap::RenderObject*>>* dap::Camera::ALL_RENDERERS = dap::RenderObject::GetAllRenderers ();
const float dap::Camera::NEAR = 0.1f;
const float dap::Camera::FAR = 100.0f;

std::vector<dap::Camera*> dap::Camera::allCameras = {};

dap::Camera::Camera () {
	allCameras.push_back ( this );
}

dap::Camera::~Camera () {
	allCameras.erase ( std::remove ( allCameras.begin (), allCameras.end (), this ), allCameras.end () );
}

void dap::Camera::Initialize () {
	dap::Component::Initialize ();
	GetOwner ()->GetTransform ()->SetZOffset ( -1.0f );
}

void dap::Camera::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	if ( ImGui::ColorEdit3 ( "Clear Color", &clearColor.r ) )
		SetClearColor ( clearColor.r, clearColor.g, clearColor.b );
	if ( ImGui::DragFloat ( "Ortho Size", &orthoSize, 0.1f ) )
		SetOrthoSize ( orthoSize );
}

const std::vector<dap::Camera*>* dap::Camera::GetAllCameras () {
	return &allCameras;
}

glm::vec2 dap::Camera::ScreenToWorld ( glm::vec2 point ) {
	glm::vec4 point4D ( point.x, point.y, 0.0f, 1.0f );
	glm::mat4 inverseMat4 = glm::inverse ( projectionOrtho * ( owner->GetTransform ()->GetMatrix () ) );
	point4D = point4D * inverseMat4;
	point4D = point4D / point4D.w;
	return glm::vec2 ( point4D.x, point4D.y );
}

void dap::Camera::SetClearColor ( float r, float g, float b ) {
	clearColor = glm::vec3 ( r, g, b );
}

void dap::Camera::SetOrthoSize ( const float newSize ) {
	orthoSize = newSize;
}

bool dap::Camera::Render ( const float aspectRatio, const float interp ) {
	if ( !isEnabled )
		return false;
	glClearColor ( clearColor.x, clearColor.y, clearColor.z, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );

	projectionOrtho = glm::ortho ( -orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, NEAR, FAR );
	for ( const auto& RENDERER_PAIR : *ALL_RENDERERS ) {
		for ( const auto& RENDERER : RENDERER_PAIR.second ) {
			if ( !RENDERER->GetEnabled () || !RENDERER->Use () )
				continue;
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
}

nlohmann::json dap::Camera::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["clearColor"] = { clearColor.x, clearColor.y, clearColor.z };
	json["orthosize"] = orthoSize;
	return json;
}
