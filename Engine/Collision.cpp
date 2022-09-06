#include "Collision.h"

#include "GameObject.h"
#include "RectCollider.h"
#include <vector>

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
	const std::vector<dap::RectCollider*>* ALL_COLLIDERS = &dap::RectCollider::allRectColliders;

	for ( const auto& COLLIDER_CHECKING : *ALL_COLLIDERS ) {
		for ( const auto& COLLIDER_AGAINST : *ALL_COLLIDERS ) {
			if ( COLLIDER_CHECKING->GetOwner ()->IsMarkedForDeletion () )
				continue;
			if ( COLLIDER_CHECKING == COLLIDER_AGAINST )
				continue;
			if ( !COLLIDER_CHECKING->TestCollisionLayers ( COLLIDER_AGAINST ) )
				continue;
			if ( !COLLIDER_CHECKING->Overlapping ( COLLIDER_AGAINST ) )
				continue;
			for ( const auto& COMP : COLLIDER_CHECKING->GetOwner ()->GetAllComponentsList () ) {
				COMP->BaseCollision ( COLLIDER_AGAINST );
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
