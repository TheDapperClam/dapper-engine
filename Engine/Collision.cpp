#include "Collision.h"

#include "GameObject.h"
#include "RectCollider.h"
#include <vector>

void dap::Collision::Process () {
	const std::vector<dap::RectCollider*>* ALL_COLLIDERS = &dap::RectCollider::allRectColliders;

	for ( const auto& COLLIDER_CHECKING : *ALL_COLLIDERS ) {
		for ( const auto& COLLIDER_AGAINST : *ALL_COLLIDERS ) {
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
