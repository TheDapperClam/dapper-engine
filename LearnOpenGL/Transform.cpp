#include "Transform.h"
#include "glm/gtx/string_cast.hpp"

JSON_REGISTER ( dap::Transform );

dap::Transform::Transform ( const glm::vec3 startingPos ) {
	SetPosition ( startingPos );
	BuildMatrix ( &matrix );
}

void dap::Transform::BuildMatrix ( glm::mat4* mat ) {
	matrix = glm::mat4 ( 1.0f );
	matrix = glm::translate ( matrix, position );
	matrix = glm::rotate ( matrix, glm::radians ( rotation.x ), glm::vec3 ( 1, 0, 0 ) );
	matrix = glm::rotate ( matrix, glm::radians ( rotation.y ), glm::vec3 ( 0, 1, 0 ) );
	matrix = glm::rotate ( matrix, glm::radians ( rotation.z ), glm::vec3 ( 0, 0, 1 ) );
	matrix = glm::scale ( matrix, scale );
}

const glm::vec3* dap::Transform::GetPosition () const {
	return &position;
}

const glm::vec3* dap::Transform::GetRotation () const {
	return &rotation;
}

const glm::vec3* dap::Transform::GetScale () const {
	return &scale;
}

const glm::mat4* dap::Transform::GetMatrix () const {
	return &matrix;
}

void dap::Transform::SetPosition ( const glm::vec3 newPosition ) {
	position = newPosition;
	BuildMatrix ( &matrix );
}

void dap::Transform::SetRotation ( const glm::vec3 newRotation ) {
	rotation = newRotation;
	BuildMatrix ( &matrix );
}

void dap::Transform::SetScale ( const glm::vec3 newScale ) {
	scale = newScale;
	BuildMatrix ( &matrix );
}

void dap::Transform::Translate ( const glm::vec3 amount ) {
	position += amount;
	BuildMatrix ( &matrix );
}

void dap::Transform::Rotate ( const glm::vec3 amount ) {
	rotation += amount;
	BuildMatrix ( &matrix );
}

void dap::Transform::Scale ( const glm::vec3 amount ) {
	scale += amount;
	BuildMatrix ( &matrix );
}

void dap::Transform::Deserialize ( const nlohmann::json* json ) {
	SetPosition ( glm::vec3 ( json->at("position")[0], json->at("position")[1], json->at("position")[2] ) );
	SetRotation ( glm::vec3 ( json->at ( "rotation" )[0], json->at ( "rotation" )[1], json->at ( "rotation" )[2] ) );
	SetScale ( glm::vec3 ( json->at ( "scale" )[0], json->at ( "scale" )[1], json->at ( "scale" )[2] ) );
}

const nlohmann::json dap::Transform::Serialize () {
	nlohmann::json json = {
		{ "type", typeid( *this ).name () },
		{ "position", { position.x, position.y, position.z } },
		{ "rotation", { rotation.x, rotation.y, rotation.z } },
		{ "scale", { scale.x, scale.y, scale.z } }
	};
	return json;
}
