#pragma once

#include "glm/glm.hpp"

namespace dap {
	class RectCollider;

	struct CollisionData {
		dap::RectCollider* Other;
		glm::vec2 Push;
	};
}
