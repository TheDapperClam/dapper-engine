#include "Collision.h"

#include "Controller.h"
#include "GameObject.h"
#include "RectCollider.h"
#include <vector>

void dap::Collision::Process () {
	const std::vector<dap::RectCollider*>* ALL_COLLIDERS = &dap::RectCollider::allRectColliders;
	for ( const auto& COLLIDER_CHECKING : *ALL_COLLIDERS ) {
		for ( const auto& COLLIDER_AGAINST : *ALL_COLLIDERS ) {
			if ( COLLIDER_CHECKING == COLLIDER_AGAINST )
				continue;
			if ( !COLLIDER_CHECKING->Overlapping ( COLLIDER_AGAINST ) )
				continue;
			for ( const auto& CONTROLLER : *COLLIDER_CHECKING->GetOwner ()->GetAllControllers () ) {
				CONTROLLER->BaseCollision ( COLLIDER_AGAINST );
			}
		}
	}
}
