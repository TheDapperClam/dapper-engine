#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include "imgui.h"
#include "MathF.h"

const glm::vec2 dap::Transform::DEFAULT_POSITION = glm::vec2 ( 0, 0 );
const float dap::Transform::DEFAULT_ROTATION = 0.0f;
const glm::vec2 dap::Transform::DEFAULT_SCALE = glm::vec2 ( 1, 1 );

dap::Transform::Transform ( glm::vec2 startingPosition, float startingRotation, glm::vec2 startingScale ) {
	SetPosition ( startingPosition );
	SetRotation ( startingRotation );
	SetScale ( startingScale );
}

void dap::Transform::BuildMatrix ( glm::mat4& mat ) {
	mat = glm::mat4 ( 1.0f );
	mat = glm::translate ( mat, glm::vec3 ( interpPositionCurrent * matrixTranslateScaling, zOffset ) );
	mat = glm::rotate ( mat, glm::radians ( interpRotationCurrent ), glm::vec3 ( 0, 0, 1 ) );
	mat = glm::scale ( mat, glm::vec3 ( interpScaleCurrent, 1.0f ) );
}

glm::vec2 dap::Transform::GetInterpPosition () const {
	return interpPositionCurrent;
}

float dap::Transform::GetInterpRotation () const {
	return interpRotationCurrent;
}

glm::vec2 dap::Transform::GetInterpScale () const {
	return interpScaleCurrent;
}

glm::vec2 dap::Transform::GetPosition () const {
	return position;
}

float dap::Transform::GetRotation () const {
	return rotation;
}

glm::vec2 dap::Transform::GetScale () const {
	return scale;
}

glm::mat4 dap::Transform::GetMatrix () const {
	return matrix;
}

void dap::Transform::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	if ( ImGui::DragFloat2 ( "Position", &position.x, 0.1f ) )
		SetPosition ( position );
	if ( ImGui::DragFloat ( "Rotation", &rotation, 0.1f) )
		SetRotation ( rotation );
	if ( ImGui::DragFloat2 ( "Scale", &scale.x, 0.1f ) )
		SetScale ( scale );
}

void dap::Transform::Interpolate ( float t ) {
	interpPositionCurrent = glm::mix ( interpPositionStart, position, std::min ( t, 1.0f ) );
	interpRotationCurrent = dap::MathF::Lerp ( interpRotationStart, rotation, std::min ( t, 1.0f ) );
	interpScaleCurrent = glm::mix ( interpScaleStart, scale, std::min ( t, 1.0f ) );
	if ( t >= 1.0f ) {
		interpPositionStart = position;
		interpRotationStart = rotation;
		interpScaleStart = scale;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::SetPosition ( glm::vec2 newPosition ) {
	position = newPosition;
	interpPositionStart = position;
	interpPositionCurrent = position;
	BuildMatrix ( matrix );
}

void dap::Transform::SetMatrixTranslateScaling ( glm::vec2 scale ) {
	matrixTranslateScaling = scale;
}

void dap::Transform::SetRotation ( float newRotation ) {
	rotation = newRotation;
	interpRotationStart = rotation;
	interpRotationCurrent = rotation;
	BuildMatrix ( matrix );
}

void dap::Transform::SetScale ( glm::vec2 newScale ) {
	scale = newScale;
	interpScaleStart = scale;
	interpScaleCurrent = scale;
	BuildMatrix ( matrix );
}

void dap::Transform::SetZOffset ( float newOffset ) {
	zOffset = newOffset;
	BuildMatrix ( matrix );
}

void dap::Transform::Translate ( glm::vec2 amount, bool interp ) {
	interpPositionStart = position;
	position += amount;
	if ( !interp ) {
		interpPositionStart = position;
		interpPositionCurrent = position;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::Rotate ( float amount, bool interp ) {
	interpRotationStart = rotation;
	rotation += amount;
	if ( !interp ) {
		interpRotationStart = rotation;
		interpRotationCurrent = rotation;
	}
	BuildMatrix ( matrix );
}

void dap::Transform::Scale ( glm::vec2 amount, bool interp ) {
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
	if ( json->contains ( "position" ) )
		SetPosition ( glm::vec2 ( json->at ( "position" )[0], json->at ( "position" )[1] ) );
	if ( json->contains ( "rotation" ) )
		SetRotation ( json->at ( "rotation" ).size () > 1 ? json->at ( "rotation" )[0] : json->at ( "rotation" ) );
	if ( json->contains ( "scale" ) )
		SetScale ( glm::vec2 ( json->at ( "scale" )[0], json->at ( "scale" )[1] ) );
}

nlohmann::json dap::Transform::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["position"] = { position.x, position.y };
	json["rotation"] = rotation;
	json["scale"] = { scale.x, scale.y };
	return json;
}
