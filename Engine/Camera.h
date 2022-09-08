#pragma once

#include "Component.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

namespace dap {
	class Camera : public dap::Component {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	private:
		static std::vector<dap::Camera*> allCameras;

	private:
		glm::mat4 projectionOrtho;
		float orthoSize = 1.0f;
		float near = 0.1f;
		float far = 100.0f;
		glm::vec3 clearColor = glm::vec3 ( 0, 0, 0 );

	public:
		Camera ();
		virtual ~Camera () override;

	public:
		virtual void Initialize ();
		const static std::vector<dap::Camera*>* GetAllCameras ();
		glm::vec3 ScreenToWorld ( glm::vec3 point );
		void SetClearColor ( float r, float g, float b );
		void SetOrthoSize ( const float newSize );
		void SetNear ( const float newNear );
		void SetFar ( const float newFar );
		bool Render ( const float aspectRatio, const float interp );
	};
}
