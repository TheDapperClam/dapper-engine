#include "RectCollider.h"

#include "Config.h"
#include "Editor.h"
#include "GameObject.h"
#include "imgui.h"
#include <limits>
#include "Transform.h"
#include "Vertex.h"

std::vector<dap::RectCollider*> dap::RectCollider::allRectColliders = {};

dap::RectCollider::RectCollider () {
	glDrawMode = GL_LINE_LOOP;
	SetShader ( "shader_collider" );
	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
	allRectColliders.push_back ( this );
}

dap::RectCollider::~RectCollider () {
	allRectColliders.erase ( std::remove ( allRectColliders.begin (), allRectColliders.end (), this ), allRectColliders.end () );
}

void dap::RectCollider::Deserialize ( const nlohmann::json* json ) {
	dap::RenderObject::Deserialize ( json );
	if ( json->count ( "width" ) )
		SetWidth ( json->at ( "width" ) );
	if ( json->count ( "height" ) )
		SetHeight ( json->at ( "height" ) );
	if ( json->count ( "collisionLayer" ) )
		SetCollisionLayer ( json->at ( "collisionLayer" ) );
	if ( json->count ( "collisionMask" ) )
		SetCollisionMask ( json->at ( "collisionMask" ) );
}

nlohmann::json dap::RectCollider::Serialize () {
	nlohmann::json json = dap::RenderObject::Serialize ();
	json["width"] = width;
	json["height"] = height;
	json["collisionLayer"] = collisionLayer;
	json["collisionMask"] = collisionMask;
	return json;
}

void dap::RectCollider::BuildElements () {
	elements = {
		0, 1, 2,
		2, 3, 0
	};
}

void dap::RectCollider::BuildVertices () {
	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;

	vertices = {
		dap::Vertex ( glm::vec3 ( -halfWidth,  halfHeight, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), glm::vec2(0.0f, 1.0f) ),
		dap::Vertex ( glm::vec3 ( halfWidth,  halfHeight, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), glm::vec2(1.0f, 1.0f) ),
		dap::Vertex ( glm::vec3 ( halfWidth, -halfHeight, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), glm::vec2(1.0f, 0.0f) ),
		dap::Vertex ( glm::vec3 ( -halfWidth, -halfHeight, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), glm::vec2(0.0f, 0.0f) )
	};
}

int dap::RectCollider::GetCollisionLayer () {
	return collisionLayer;
}

int dap::RectCollider::GetCollisionMask () {
	return collisionMask;
}

float dap::RectCollider::GetHeight () {
	return height;
}

float dap::RectCollider::GetWidth () {
	return width;
}

void dap::RectCollider::ImGuiWidgetsBegin () {
	dap::RenderObject::ImGuiWidgetsBegin ();

	if ( ImGui::DragFloat ( "Width", &width, 0.1f, 0.0f, std::numeric_limits<float>::max () ) )
		SetWidth ( width );
	if ( ImGui::DragFloat ( "Height", &height, 0.1f, 0.0f, std::numeric_limits<float>::max () ) )
		SetHeight ( height );
	if ( ImGui::InputInt ( "Collision Layer", &collisionLayer ) )
		SetCollisionLayer ( collisionLayer );
	if ( ImGui::InputInt ( "Collision Mask", &collisionMask ) )
		SetCollisionMask ( collisionMask );
}

void dap::RectCollider::SetCollisionLayer ( int layer ) {
	collisionLayer = layer;
}

void dap::RectCollider::SetCollisionMask ( int mask ) {
	collisionMask = mask;
}

void dap::RectCollider::SetWidth ( float w ) {
	width = w;
	BuildVertices ();
	BuildGLObject ();
}

void dap::RectCollider::SetHeight ( float h ) {
	height = h;
	BuildVertices ();
	BuildGLObject ();
}

bool dap::RectCollider::TestCollisionLayers ( dap::RectCollider* colliderAgainst ) {
	const bool LAYERS_COLLIDING = collisionMask & colliderAgainst->GetCollisionLayer ();
	return LAYERS_COLLIDING;
}

bool dap::RectCollider::Use () const {
	const dap::Editor* CURRENT_EDITOR = dap::Editor::GetCurrent ();
	if ( !CURRENT_EDITOR || !CURRENT_EDITOR->VisualizeColliders () )
		return false;
	dap::RenderObject::Use ();
	return true;
}

bool dap::RectCollider::Overlapping ( const dap::RectCollider* other ) {
	if ( !isEnabled )
		return false;

	const dap::Transform* transA = owner->GetTransform ();
	const dap::Transform* transB = other->owner->GetTransform ();
	const glm::vec3 posA = transA->GetPosition ();
	const glm::vec3 posB = transB->GetPosition ();
	const float halfWidthA = width * transA->GetScale().x * 0.5f;
	const float halfWidthB = other->width * transB->GetScale().x * 0.5f;
	const float halfHeightA = height * transA->GetScale().y * 0.5f;
	const float halfHeightB = other->height * transB->GetScale().y * 0.5f;

	return posA.x - halfWidthA < posB.x + halfWidthB &&
		   posA.x + halfWidthA > posB.x - halfWidthB &&
		   posA.y - halfHeightA < posB.y + halfHeightB &&
		   posA.y + halfHeightA > posB.y - halfHeightB;
}

bool dap::RectCollider::PointInside ( const glm::vec3 point ) {
	if ( !isEnabled )
		return false;

	const dap::Transform* rectTrans = owner->GetTransform ();
	const glm::vec3 rectPos = rectTrans->GetPosition ();
	const float halfWidthA = width * rectTrans->GetScale ().x * 0.5f;
	const float halfHeightA = height * rectTrans->GetScale ().y * 0.5f;

	return point.x > rectPos.x - halfWidthA &&
		   point.x < rectPos.x + halfWidthA &&
		   point.y > rectPos.y - halfHeightA &&
		   point.y < rectPos.y + halfHeightA;
}
