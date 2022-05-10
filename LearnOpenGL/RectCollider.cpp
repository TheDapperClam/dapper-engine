#include "RectCollider.h"

#include "GameObject.h"

JSON_REGISTER ( dap::RectCollider );
std::vector<dap::RectCollider*> dap::RectCollider::allRectColliders = {};

dap::RectCollider::RectCollider () {
	allRectColliders.push_back ( this );
}

dap::RectCollider::~RectCollider () {
	allRectColliders.erase ( std::remove ( allRectColliders.begin (), allRectColliders.end (), this ), allRectColliders.end () );
}

void dap::RectCollider::Deserialize ( const nlohmann::json* json ) {

}

const nlohmann::json dap::RectCollider::Serialize () {
	const nlohmann::json json {
		{ "type", typeid(*this).name () }
	};
	return json;
}

const bool dap::RectCollider::Overlaps ( const dap::RectCollider* rectA, const dap::RectCollider* rectB ) {
	const glm::vec3 posA = *rectA->owner->GetTransform ()->GetPosition ();
	const glm::vec3 posB = *rectB->owner->GetTransform ()->GetPosition ();
	const glm::vec3 sizeA = *rectA->owner->GetTransform ()->GetScale ();
	const glm::vec3 sizeB = *rectB->owner->GetTransform ()->GetScale ();

	return posA.x <= posB.x + sizeB.x &&
		   posA.x + sizeA.x >= posB.x &&
		   posA.y <= posB.y + sizeB.y &&
		   posA.y + sizeA.y >= posB.x;
}

const bool dap::RectCollider::Inside ( const dap::RectCollider* rectA, const dap::RectCollider* rectB ) {
	const glm::vec3 posA = *rectA->owner->GetTransform ()->GetPosition ();
	const glm::vec3 posB = *rectB->owner->GetTransform ()->GetPosition ();
	const glm::vec3 sizeA = *rectA->owner->GetTransform ()->GetScale ();
	const glm::vec3 sizeB = *rectB->owner->GetTransform ()->GetScale ();

	return posA.x >= posB.x &&
		   posA.x + sizeA.x <= posB.x + sizeB.x &&
		   posA.y >= posB.y &&
		   posA.y + sizeA.y <= posB.y + sizeB.y;
}
