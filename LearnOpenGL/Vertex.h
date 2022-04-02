#pragma once

#include "glm/glm.hpp"

namespace dap {
	struct Vertex {
		Vertex ( const glm::vec3 sPos, const glm::vec3 sColor, const glm::vec2 sTexCoord );
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
	};
}
