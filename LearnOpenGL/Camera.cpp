#include "Camera.h"
#include "GameObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Sprite.h"

JSON_REGISTER ( dap::Camera );
std::vector<dap::Camera*> dap::Camera::allCameras = {};

dap::Camera::Camera () {
	allCameras.push_back ( this );
}

dap::Camera::~Camera () {
	allCameras.erase ( std::remove ( allCameras.begin (), allCameras.end (), this ), allCameras.end () );
}

void dap::Camera::Initialize () {
	owner->GetTransform ()->Translate ( glm::vec3 ( 0, 0, -1 ) );
}

const std::vector<dap::Camera*>* dap::Camera::GetAllCameras () {
	return &allCameras;
}

glm::vec3 dap::Camera::ScreenToWorld ( glm::vec3 point ) {
	glm::vec4 point4D ( point.x, point.y, point.z, 1.0f );
	glm::mat4 inverseMat4 = glm::inverse ( projectionOrtho * ( *owner->GetTransform ()->GetMatrix () ) );
	point4D = point4D * inverseMat4;
	point4D = point4D / point4D.w;
	return glm::vec3 ( point4D.x, point4D.y, point4D.z );
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

void dap::Camera::Render ( const GLuint* program, const float aspectRatio ) {
	projectionOrtho = glm::ortho ( -orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, near, far);
	
	GLint uTransformLocation = glGetUniformLocation ( *program, "uTransform" );
	GLint uViewLocation = glGetUniformLocation ( *program, "uView" );
	GLint uProjectionLocation = glGetUniformLocation ( *program, "uProjection" );
	glUniformMatrix4fv ( uViewLocation, 1, GL_FALSE, glm::value_ptr ( *owner->GetTransform ()->GetMatrix () ) );
	glUniformMatrix4fv ( uProjectionLocation, 1, GL_FALSE, glm::value_ptr ( projectionOrtho ) );
	
	const auto allSprites = dap::Sprite::GetAllSprites ();
	for ( int i = 0; i < allSprites->size (); i++ ) {
		dap::Sprite* sprite = allSprites->at ( i );
		if ( !sprite->GetTexture () )
			continue;
		const int triCount = sprite->Use ();
		glUniformMatrix4fv ( uTransformLocation, 1, GL_FALSE, glm::value_ptr ( *sprite->GetOwner ()->GetTransform ()->GetMatrix () ) );
		glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL );
	}
}

void dap::Camera::Deserialize ( const nlohmann::json* json ) {
	SetOrthoSize ( json->at ( "orthosize" ) );
	SetNear ( json->at ( "near" ) );
	SetFar ( json->at ( "far" ) );
}

const nlohmann::json dap::Camera::Serialize () {
	nlohmann::json json = {
		{ "type", typeid( *this ).name () },
		{ "orthosize", orthoSize },
		{ "near", near },
		{ "far", far }
	};
	return json;
}
