#include "Collision.h"

#include "CollisionData.h"
#include "Component.h"
#include "GameObject.h"
#include "RectCollider.h"

const std::vector<dap::RectCollider*>* dap::Collision::ALL_RECTCOLLIDERS = dap::RectCollider::GetAllRectColliders ();

std::unordered_map<std::string, int> dap::Collision::Layers = {
	{ "Default", 1 },
	{ "Friend", 2 },
	{ "Enemy", 4 },
	{ "Attack_Friend", 8 },
	{ "Attack_Enemy", 16 }
};

const std::unordered_map<std::string, int>* dap::Collision::GetAllLayers () {
	return &Layers;
}

int dap::Collision::GetLayer ( std::string key ) {
	if ( !Layers.count ( key ) )
		return NULL;
	return Layers[key];
}

void dap::Collision::Process () {
	for ( const auto& COLLIDER_CHECKING : *ALL_RECTCOLLIDERS ) {
		for ( const auto& COLLIDER_AGAINST : *ALL_RECTCOLLIDERS ) {
			if ( COLLIDER_CHECKING->GetOwner ()->IsMarkedForDeletion () )
				continue;
			if ( COLLIDER_CHECKING == COLLIDER_AGAINST )
				continue;
			if ( !COLLIDER_CHECKING->TestCollisionLayers ( COLLIDER_AGAINST ) )
				continue;
			if ( !COLLIDER_CHECKING->Overlapping ( COLLIDER_AGAINST ) )
				continue;

			glm::vec2 PUSH_FORCE = COLLIDER_CHECKING->GetCollisionPush ( COLLIDER_AGAINST );
			dap::CollisionData collisionData {
				glm::normalize ( PUSH_FORCE ),
				COLLIDER_AGAINST,
				PUSH_FORCE
			};

			for ( const auto& COMPONENT_PAIR : *COLLIDER_CHECKING->GetOwner()->GetAllComponents()) {
				for ( int i = 0; i < COMPONENT_PAIR.second.size (); i++ ) {
					COMPONENT_PAIR.second.at ( i )->BaseCollision ( collisionData );
				}
			}
		}
	}
}

void dap::Collision::SetLayer ( std::string key, int value ) {
	if ( Layers.count ( key ) )
		Layers.insert ( std::pair<std::string, int> ( key, value ) );
	else
		Layers[key] = value;
}
