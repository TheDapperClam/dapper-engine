#include "RectCollider.h"
#include "GameObject.h"

JSON_REGISTER ( dap::RectCollider );
std::vector<dap::RectCollider*> dap::RectCollider::allRectColliders = {};

dap::RectCollider::RectCollider () {
	allRectColliders.push_back ( this );
}

dap::RectCollider::RectCollider ( float width, float height ) {
	SetWidth ( width );
	SetHeight ( height );
	allRectColliders.push_back ( this );
}

dap::RectCollider::~RectCollider () {
	allRectColliders.erase ( std::remove ( allRectColliders.begin (), allRectColliders.end (), this ), allRectColliders.end () );
}

void dap::RectCollider::Deserialize ( const nlohmann::json* json ) {
	SetWidth ( json->at ( "width" ) );
	SetHeight ( json->at ( "height" ) );
}

const nlohmann::json dap::RectCollider::Serialize () {
	const nlohmann::json json {
		{ "type", typeid(*this).name () },
		{ "width", width },
		{ "height", height }
	};
	return json;
}

float dap::RectCollider::GetWidth () {
	return width;
}

float dap::RectCollider::GetHeight () {
	return height;
}

void dap::RectCollider::SetWidth ( float w ) {
	width = w;
}

void dap::RectCollider::SetHeight ( float h ) {
	height = h;
}

bool dap::RectCollider::Overlapping ( const dap::RectCollider* other ) {
	const dap::Transform* transA = owner->GetTransform ();
	const dap::Transform* transB = other->owner->GetTransform ();
	const glm::vec3 posA = *transA->GetPosition ();
	const glm::vec3 posB = *transB->GetPosition ();
	const float halfWidthA = width * transA->GetScale()->x * 0.5f;
	const float halfWidthB = other->width * transB->GetScale()->x * 0.5f;
	const float halfHeightA = height * transA->GetScale()->y * 0.5f;
	const float halfHeightB = other->height * transB->GetScale()->y * 0.5f;

	return posA.x - halfWidthA < posB.x + halfWidthB &&
		   posA.x + halfWidthA > posB.x - halfWidthB &&
		   posA.y - halfHeightA < posB.y + halfHeightB &&
		   posA.y + halfHeightA > posB.x - halfHeightB;
}

bool dap::RectCollider::PointInside ( const glm::vec3 point ) {
	const dap::Transform* transA = owner->GetTransform ();
	const glm::vec3 posA = *transA->GetPosition ();
	const float halfWidthA = transA->GetScale ()->x * 0.5f;
	const float halfHeightA = transA->GetScale ()->y * 0.5f;

	return point.x > posA.x - halfWidthA &&
		   point.x < posA.x + halfWidthA &&
		   point.y > posA.y - halfHeightA &&
		   point.y < posA.y + halfHeightA;
}
