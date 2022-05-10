#pragma once

#include "Component.h"
#include <glad/glad.h>
#include <vector>

namespace dap {
	class Camera : public dap::Component {
	public:
		Camera ();
		virtual ~Camera () override;
		virtual void Initialize ();
		const static std::vector<dap::Camera*>* GetAllCameras ();
		void SetOrthoSize ( const float newSize );
		void SetNear ( const float newNear );
		void SetFar ( const float newFar );
		void Render ( const GLuint* program, const float aspectRatio );
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;
	private:
		JSON_PRIVATE;
		static std::vector<dap::Camera*> allCameras;
		float orthoSize = 1.0f;
		float near = 0.1f;
		float far = 100.0f;
	};
}
