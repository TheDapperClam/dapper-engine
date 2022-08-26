#include "Vertex.h"

dap::Vertex::Vertex ( const glm::vec3 sPos, const glm::vec3 sColor, const glm::vec2 sTexCoord ) {
	position = sPos;
	color = sColor;
	texCoord = sTexCoord;
}
