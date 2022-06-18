#pragma once

#include "Component.h"
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

namespace dap {
	class Texture;

	class Camera : public dap::Component {
	JSON_CLASS

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	private:
		static std::vector<dap::Camera*> allCameras;
		glm::mat4 projectionOrtho;
		float orthoSize = 1.0f;
		float near = 0.1f;
		float far = 100.0f;

	public:
		Camera ();
		virtual ~Camera () override;

	public:
		virtual void Initialize ();
		const static std::vector<dap::Camera*>* GetAllCameras ();
		glm::vec3 ScreenToWorld ( glm::vec3 point );
		void SetOrthoSize ( const float newSize );
		void SetNear ( const float newNear );
		void SetFar ( const float newFar );
		void Render ( const GLuint* program, const float aspectRatio );
	};
}
