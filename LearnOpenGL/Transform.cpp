#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

dap::Transform::Transform ( const glm::vec3 startingPos ) {
	SetPosition ( startingPos );
	BuildMatrix ( matrix );
}

void dap::Transform::BuildMatrix ( glm::mat4& mat ) {
	mat = glm::mat4 ( 1.0f );
	mat = glm::translate ( mat, position );
	mat = glm::rotate ( mat, glm::radians ( rotation.x ), glm::vec3 ( 1, 0, 0 ) );
	mat = glm::rotate ( mat, glm::radians ( rotation.y ), glm::vec3 ( 0, 1, 0 ) );
	mat = glm::rotate ( mat, glm::radians ( rotation.z ), glm::vec3 ( 0, 0, 1 ) );
	mat = glm::scale ( mat, scale );
}

glm::vec3 dap::Transform::GetPosition () const {
	return position;
}

glm::vec3 dap::Transform::GetRotation () const {
	return rotation;
}

glm::vec3 dap::Transform::GetScale () const {
	return scale;
}

glm::mat4 dap::Transform::GetMatrix () const {
	return matrix;
}

void dap::Transform::SetPosition ( glm::vec3 newPosition ) {
	position = newPosition;
	BuildMatrix ( matrix );
}

void dap::Transform::SetRotation ( glm::vec3 newRotation ) {
	rotation = newRotation;
	BuildMatrix ( matrix );
}

void dap::Transform::SetScale ( glm::vec3 newScale ) {
	scale = newScale;
	BuildMatrix ( matrix );
}

void dap::Transform::Translate ( glm::vec3 amount ) {
	position += amount;
	BuildMatrix ( matrix );
}

void dap::Transform::Translate ( glm::vec2 amount ) {
	position += glm::vec3 ( amount.x, amount.y, 0.0f );
	BuildMatrix ( matrix );
}

void dap::Transform::Rotate ( glm::vec3 amount ) {
	rotation += amount;
	BuildMatrix ( matrix );
}

void dap::Transform::Scale ( glm::vec3 amount ) {
	scale += amount;
	BuildMatrix ( matrix );
}

void dap::Transform::Deserialize ( const nlohmann::json* json ) {
	SetPosition ( glm::vec3 ( json->at("position")[0], json->at("position")[1], json->at("position")[2] ) );
	SetRotation ( glm::vec3 ( json->at ( "rotation" )[0], json->at ( "rotation" )[1], json->at ( "rotation" )[2] ) );
	SetScale ( glm::vec3 ( json->at ( "scale" )[0], json->at ( "scale" )[1], json->at ( "scale" )[2] ) );
}

const nlohmann::json dap::Transform::Serialize () {
	nlohmann::json json = {
		{ "position", { position.x, position.y, position.z } },
		{ "rotation", { rotation.x, rotation.y, rotation.z } },
		{ "scale", { scale.x, scale.y, scale.z } }
	};
	JSON_APPEND_TYPE ( json )
	return json;
}
