#include "Camera.h"
#include "GameObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "RenderObject.h"
#include "Shader.h"

std::vector<dap::Camera*> dap::Camera::allCameras = {};

dap::Camera::Camera () {
	allCameras.push_back ( this );
}

dap::Camera::~Camera () {
	allCameras.erase ( std::remove ( allCameras.begin (), allCameras.end (), this ), allCameras.end () );
}

void dap::Camera::Initialize () {
	dap::Component::Initialize ();
	owner->GetTransform ()->Translate ( glm::vec3 ( 0, 0, -1 ) );
}

const std::vector<dap::Camera*>* dap::Camera::GetAllCameras () {
	return &allCameras;
}

glm::vec3 dap::Camera::ScreenToWorld ( glm::vec3 point ) {
	glm::vec4 point4D ( point.x, point.y, point.z, 1.0f );
	glm::mat4 inverseMat4 = glm::inverse ( projectionOrtho * ( owner->GetTransform ()->GetMatrix () ) );
	point4D = point4D * inverseMat4;
	point4D = point4D / point4D.w;
	return glm::vec3 ( point4D.x, point4D.y, point4D.z );
}

void dap::Camera::SetClearColor ( float r, float g, float b ) {
	clearColor = glm::vec3 ( r, g, b );
}

void dap::Camera::SetOrthoSize ( const float newSize ) {
	orthoSize = newSize;
}

void dap::Camera::SetNear ( const float newNear ) {
	near = newNear;
}

void dap::Camera::SetFar ( const float newFar ) {
	far = newFar;
}

void dap::Camera::Render ( const float aspectRatio ) {
	projectionOrtho = glm::ortho ( -orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, near, far);
	
	glClearColor ( clearColor.x, clearColor.y, clearColor.z, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );

	const auto* allRenderers = dap::RenderObject::GetAllRenderers ();
	for ( int i = 0; i < allRenderers->size (); i++ ) {
		dap::RenderObject* r = allRenderers->at ( i );
		if ( !r->GetEnabled () || !r->Use () )
			continue;

		dap::Shader* const shader = r->GetShader ();
		const GLuint* program = shader->GetProgram ();
		GLint uTransformLocation = glGetUniformLocation ( *program, "uTransform" );
		GLint uViewLocation = glGetUniformLocation ( *program, "uView" );
		GLint uProjectionLocation = glGetUniformLocation ( *program, "uProjection" );
		glUniformMatrix4fv ( uViewLocation, 1, GL_FALSE, glm::value_ptr ( owner->GetTransform ()->GetMatrix () ) );
		glUniformMatrix4fv ( uProjectionLocation, 1, GL_FALSE, glm::value_ptr ( projectionOrtho ) );
		glUniformMatrix4fv ( uTransformLocation, 1, GL_FALSE, glm::value_ptr ( r->GetOwner ()->GetTransform ()->GetMatrix () ) );
		glDrawElements ( r->GetDrawMode (), r->GetElementsSize (), GL_UNSIGNED_INT, NULL );
	}
}

void dap::Camera::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );
	const auto deserializedClearColor = json->at ( "clearColor" );
	SetClearColor ( deserializedClearColor[0], deserializedClearColor[1], deserializedClearColor[2] );
	SetOrthoSize ( json->at ( "orthosize" ) );
	SetNear ( json->at ( "near" ) );
	SetFar ( json->at ( "far" ) );
}

nlohmann::json dap::Camera::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["clearColor"] = { clearColor.x, clearColor.y, clearColor.z };
	json["orthosize"] = orthoSize;
	json["near"] = near;
	json["far"] = far;
	JSON_APPEND_TYPE(json)
	return json;
}
