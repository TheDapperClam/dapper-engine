#pragma once

#include "Component.h"

namespace dap {
	struct CollisionData;

	class KinematicBody : public dap::Component {
	public:
		virtual void Collision ( dap::CollisionData& data ) override;
	};
}
