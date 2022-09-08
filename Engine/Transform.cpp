#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include "imgui.h"

dap::Transform::Transform ( const glm::vec3 startingPos ) {
	SetPosition ( startingPos );
}

void dap::Transform::BuildMatrix ( glm::mat4& mat ) {
	mat = glm::mat4 ( 1.0f );
	mat = glm::translate ( mat, interpPositionCurrent );
	mat = glm::rotate ( mat, glm::radians ( interpRotationCurrent.x ), glm::vec3 ( 1, 0, 0 ) );
	mat = glm::rotate ( mat, glm::radians ( interpRotationCurrent.y ), glm::vec3 ( 0, 1, 0 ) );
	mat = glm::rotate ( mat, glm::radians ( interpRotationCurrent.z ), glm::vec3 ( 0, 0, 1 ) );
	mat = glm::scale ( mat, interpScaleCurrent );
}

float dap::Transform::GetInterpolationDist () {
	return glm::length ( position - interpPositionStart );
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

void dap::Transform::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	if ( ImGui::DragFloat3 ( "Position", &position.x, 0.1f ) )
		SetPosition ( position );
	if ( ImGui::DragFloat3 ( "Rotation", &rotation.x, 0.1f) )
		SetRotation ( rotation );
	if ( ImGui::DragFloat3 ( "Scale", &scale.x, 0.1f ) )
		SetScale ( scale );
}

void dap::Transform::Interpolate ( float t ) {
	interpPositionCurrent = glm::mix ( interpPositionStart, position, std::min ( t, 1.0f ) );
	interpRotationCurrent = glm::mix ( interpRotationStart, rotation, std::min ( t, 1.0f ) );
	interpScaleCurrent = glm::mix ( interpScaleStart, scale, std::min ( t, 1.0f ) );
	if ( t >= 1.0f ) {
		interpPositionStart = position;
		interpRotationStart = rotation;
		interpScaleStart = scale;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::SetPosition ( glm::vec3 newPosition ) {
	position = newPosition;
	interpPositionStart = position;
	interpPositionCurrent = position;
	BuildMatrix ( matrix );
}

void dap::Transform::SetRotation ( glm::vec3 newRotation ) {
	rotation = newRotation;
	interpRotationStart = rotation;
	interpRotationCurrent = rotation;
	BuildMatrix ( matrix );
}

void dap::Transform::SetScale ( glm::vec3 newScale ) {
	scale = newScale;
	interpScaleStart = scale;
	interpScaleCurrent = scale;
	BuildMatrix ( matrix );
}

void dap::Transform::Translate ( glm::vec3 amount, bool interp ) {
	interpPositionStart = position;
	position += amount;
	if ( !interp ) {
		interpPositionStart = position;
		interpPositionCurrent = position;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::Translate ( glm::vec2 amount, bool interp ) {
	interpPositionStart = position;
	position += glm::vec3 ( amount.x, amount.y, 0.0f );
	if ( !interp ) {
		interpPositionStart = position;
		interpPositionCurrent = position;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::Rotate ( glm::vec3 amount, bool interp ) {
	interpRotationStart = rotation;
	rotation += amount;
	if ( !interp ) {
		interpRotationStart = rotation;
		interpPositionCurrent = rotation;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::Scale ( glm::vec3 amount, bool interp ) {
	interpScaleStart = scale;
	scale += amount;
	if ( !interp ) {
		interpScaleStart = scale;
		interpScaleCurrent = scale;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );
	if ( json->count ( "position" ) )
		SetPosition ( glm::vec3 ( json->at ( "position" )[0], json->at ( "position" )[1], json->at ( "position" )[2] ) );
	if ( json->count ( "rotation" ) )
		SetRotation ( glm::vec3 ( json->at ( "rotation" )[0], json->at ( "rotation" )[1], json->at ( "rotation" )[2] ) );
	if ( json->count ( "scale" ) )
		SetScale ( glm::vec3 ( json->at ( "scale" )[0], json->at ( "scale" )[1], json->at ( "scale" )[2] ) );
}

nlohmann::json dap::Transform::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["position"] = { position.x, position.y, position.z };
	json["rotation"] = { rotation.x, rotation.y, rotation.z };
	json["scale"] = { scale.x, scale.y, scale.z };
	return json;
}
