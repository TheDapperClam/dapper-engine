#pragma once

#include "glm/glm.hpp"

namespace dap {
	class RectCollider;

	struct CollisionData {
		glm::vec2 Normal;
		dap::RectCollider* Other;
		glm::vec2 Push;
	};
}
