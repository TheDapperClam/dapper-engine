#include "KinematicBody.h"

#include "CollisionData.h"
#include "GameObject.h"
#include "Transform.h"

void dap::KinematicBody::Collision ( dap::CollisionData& data ) {
	dap::Component::Collision ( data );
	GetOwner ()->GetTransform ()->Translate ( data.Push, false );
}
