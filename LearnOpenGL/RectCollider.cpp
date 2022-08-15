#include "RectCollider.h"
#include "GameObject.h"

// TODO: remove after adding support for multiple shaders, and using a single color shader for collider visualization
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"

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
	SetWidth ( json->at ( "width" ) );
	SetHeight ( json->at ( "height" ) );
}

nlohmann::json dap::RectCollider::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["width"] = width;
	json["height"] = height;
	JSON_APPEND_TYPE ( json )
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

float dap::RectCollider::GetHeight () {
	return height;
}

float dap::RectCollider::GetWidth () {
	return width;
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

bool dap::RectCollider::Use () const {
	dap::RenderObject::Use ();
	return true;
}

bool dap::RectCollider::Overlapping ( const dap::RectCollider* other ) {
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
	const dap::Transform* rectTrans = owner->GetTransform ();
	const glm::vec3 rectPos = rectTrans->GetPosition ();
	const float halfWidthA = width * rectTrans->GetScale ().x * 0.5f;
	const float halfHeightA = height * rectTrans->GetScale ().y * 0.5f;

	return point.x > rectPos.x - halfWidthA &&
		   point.x < rectPos.x + halfWidthA &&
		   point.y > rectPos.y - halfHeightA &&
		   point.y < rectPos.y + halfHeightA;
}
