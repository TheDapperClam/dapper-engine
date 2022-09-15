#include "RectCollider.h"

#include "Collision.h"
#include "Config.h"
#include "Editor.h"
#include "GameObject.h"
#include "imgui.h"
#include <limits>
#include "Shader.h"
#include "Transform.h"
#include "Vertex.h"

std::vector<dap::RectCollider*> dap::RectCollider::AllRectColliders = {};

dap::RectCollider::RectCollider () {
	glDrawMode = GL_LINE_LOOP;
	SetShader ( dap::Shader::DEFAULT_COLLIDER );
	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
	SetLayer ( std::numeric_limits<int>::max () );
	AllRectColliders.push_back ( this );
}

dap::RectCollider::~RectCollider () {
	AllRectColliders.erase ( std::remove ( AllRectColliders.begin (), AllRectColliders.end (), this ), AllRectColliders.end () );
}

void dap::RectCollider::Deserialize ( const nlohmann::json* json ) {
	dap::RenderObject::Deserialize ( json );
	if ( json->contains ( "width" ) )
		SetWidth ( json->at ( "width" ) );
	if ( json->contains ( "height" ) )
		SetHeight ( json->at ( "height" ) );
	if ( json->contains ( "collisionLayer" ) )
		SetCollisionLayer ( json->at ( "collisionLayer" ) );
	if ( json->contains ( "collisionMask" ) )
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

const std::vector<dap::RectCollider*>* dap::RectCollider::GetAllRectColliders () {
	return &AllRectColliders;
}

int dap::RectCollider::GetCollisionLayer () {
	return collisionLayer;
}

int dap::RectCollider::GetCollisionMask () {
	return collisionMask;
}

glm::vec2 dap::RectCollider::GetCollisionPush ( const dap::RectCollider* other ) {
	glm::vec2 push = glm::vec2 ( 0, 0 );
	if ( !other )
		return push;
	const dap::Transform* TRANSFORM_CHECKING = owner->GetTransform ();
	const dap::Transform* TRANSFORM_AGAINST = other->GetOwner ()->GetTransform ();
	const glm::vec2 POSITION_CHECKING = TRANSFORM_CHECKING->GetInterpPosition ();
	const glm::vec2 POSITION_AGAINST = TRANSFORM_AGAINST->GetInterpPosition ();
	const float WIDTH_HALF_CHECKING = width * TRANSFORM_CHECKING->GetInterpScale ().x * 0.5f;
	const float WIDTH_HALF_AGAINST = other->width * TRANSFORM_AGAINST->GetInterpScale ().x * 0.5f;
	const float HEIGHT_HALF_CHECKING = height * TRANSFORM_CHECKING->GetInterpScale ().y * 0.5f;
	const float HEIGHT_HALF_AGAINST = other->height * TRANSFORM_AGAINST->GetInterpScale ().y * 0.5f;
	const bool RIGHT_AGAINST_LEFT = POSITION_CHECKING.x + WIDTH_HALF_CHECKING > POSITION_AGAINST.x - WIDTH_HALF_AGAINST && POSITION_CHECKING.x + WIDTH_HALF_CHECKING < POSITION_AGAINST.x + WIDTH_HALF_AGAINST;
	const bool LEFT_AGAINST_RIGHT = POSITION_CHECKING.x - WIDTH_HALF_CHECKING > POSITION_AGAINST.x - WIDTH_HALF_AGAINST && POSITION_CHECKING.x - WIDTH_HALF_CHECKING < POSITION_AGAINST.x + WIDTH_HALF_AGAINST;
	const bool TOP_AGAINST_BOTTOM = POSITION_CHECKING.y + HEIGHT_HALF_CHECKING > POSITION_AGAINST.y - HEIGHT_HALF_AGAINST && POSITION_CHECKING.y + HEIGHT_HALF_CHECKING < POSITION_AGAINST.y + HEIGHT_HALF_AGAINST;
	const bool BOTTOM_AGAINST_TOP = POSITION_CHECKING.y - HEIGHT_HALF_CHECKING > POSITION_AGAINST.y - HEIGHT_HALF_AGAINST && POSITION_CHECKING.y - HEIGHT_HALF_CHECKING < POSITION_AGAINST.y + HEIGHT_HALF_AGAINST;
	const float FORCE_LEFT = ( POSITION_AGAINST.x - WIDTH_HALF_AGAINST ) - ( POSITION_CHECKING.x + WIDTH_HALF_CHECKING );
	const float FORCE_RIGHT = ( POSITION_AGAINST.x + WIDTH_HALF_AGAINST ) - ( POSITION_CHECKING.x - WIDTH_HALF_CHECKING );
	const float FORCE_DOWN = ( POSITION_AGAINST.y - HEIGHT_HALF_AGAINST ) - ( POSITION_CHECKING.y + HEIGHT_HALF_CHECKING );
	const float FORCE_UP = ( POSITION_AGAINST.y + HEIGHT_HALF_AGAINST ) - ( POSITION_CHECKING.y - HEIGHT_HALF_CHECKING );

	const bool RESPONSE_UP = BOTTOM_AGAINST_TOP &&
		!TOP_AGAINST_BOTTOM &&
		std::abs ( FORCE_LEFT ) > std::abs ( FORCE_UP ) &&
		std::abs ( FORCE_RIGHT ) > std::abs ( FORCE_UP );
	const bool RESPONSE_DOWN = TOP_AGAINST_BOTTOM &&
		!BOTTOM_AGAINST_TOP &&
		std::abs ( FORCE_LEFT ) > std::abs ( FORCE_DOWN ) &&
		std::abs ( FORCE_RIGHT ) > std::abs ( FORCE_DOWN );
	const bool RESPONSE_LEFT = RIGHT_AGAINST_LEFT &&
		!LEFT_AGAINST_RIGHT &&
		std::abs ( FORCE_UP ) > std::abs ( FORCE_LEFT ) &&
		std::abs ( FORCE_DOWN ) > std::abs ( FORCE_LEFT );
	const bool RESPONSE_RIGHT = LEFT_AGAINST_RIGHT &&
		!RIGHT_AGAINST_LEFT &&
		std::abs ( FORCE_UP ) > std::abs ( FORCE_RIGHT ) &&
		std::abs ( FORCE_DOWN ) > std::abs ( FORCE_RIGHT );
								   
	if ( RESPONSE_UP )
		push.y = FORCE_UP;
	else if ( RESPONSE_DOWN )
		push.y = FORCE_DOWN;
	else if ( RIGHT_AGAINST_LEFT && !LEFT_AGAINST_RIGHT )
		push.x = FORCE_LEFT;
	else if ( LEFT_AGAINST_RIGHT && !RIGHT_AGAINST_LEFT )
		push.x = FORCE_RIGHT;
	return push;
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

	ImGui::Text ( "Layer | Mask" );
	for ( const auto& LAYER_PAIR : *dap::Collision::GetAllLayers () ) {
		ImGui::CheckboxFlags ( ( "##CollisionLayer_" + LAYER_PAIR.first ).c_str (), &collisionLayer, LAYER_PAIR.second );
		ImGui::SameLine ();
		ImGui::CheckboxFlags ( ( LAYER_PAIR.first + "##CollisionMask" ).c_str (), &collisionMask, LAYER_PAIR.second );
	}
}

bool dap::RectCollider::Overlapping ( const dap::RectCollider* other ) {
	if ( !isEnabled )
		return false;

	const dap::Transform* TRANSFORM_CHECKING = owner->GetTransform ();
	const dap::Transform* TRANSFORM_AGAINST = other->GetOwner ()->GetTransform ();
	const glm::vec2 POSITION_CHECKING = TRANSFORM_CHECKING->GetInterpPosition ();
	const glm::vec2 POSITION_AGAINST = TRANSFORM_AGAINST->GetInterpPosition ();
	const float WIDTH_HALF_CHECKING = width * TRANSFORM_CHECKING->GetInterpScale ().x * 0.5f;
	const float WIDTH_HALF_AGAINST = other->width * TRANSFORM_AGAINST->GetInterpScale ().x * 0.5f;
	const float HEIGHT_HALF_CHECKING = height * TRANSFORM_CHECKING->GetInterpScale ().y * 0.5f;
	const float HEIGHT_HALF_AGAINST = other->height * TRANSFORM_AGAINST->GetInterpScale ().y * 0.5f;

	return POSITION_CHECKING.x - WIDTH_HALF_CHECKING < POSITION_AGAINST.x + WIDTH_HALF_AGAINST &&
		POSITION_CHECKING.x + WIDTH_HALF_CHECKING > POSITION_AGAINST.x - WIDTH_HALF_AGAINST &&
		POSITION_CHECKING.y - HEIGHT_HALF_CHECKING < POSITION_AGAINST.y + HEIGHT_HALF_AGAINST &&
		POSITION_CHECKING.y + HEIGHT_HALF_CHECKING > POSITION_AGAINST.y - HEIGHT_HALF_AGAINST;
}

bool dap::RectCollider::PointInside ( const glm::vec2 point ) {
	if ( !isEnabled )
		return false;

	const dap::Transform* rectTrans = owner->GetTransform ();
	const glm::vec2 rectPos = rectTrans->GetInterpPosition ();
	const float WIDTH_HALF_CHECKING = width * rectTrans->GetInterpScale ().x * 0.5f;
	const float HEIGHT_HALF_CHECKING = height * rectTrans->GetInterpScale ().y * 0.5f;

	return point.x > rectPos.x - WIDTH_HALF_CHECKING &&
		point.x < rectPos.x + WIDTH_HALF_CHECKING &&
		point.y > rectPos.y - HEIGHT_HALF_CHECKING &&
		point.y < rectPos.y + HEIGHT_HALF_CHECKING;
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
