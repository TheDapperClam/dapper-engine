#pragma once

#include "Component.h"
#include "glm/gtc/matrix_transform.hpp"
#include <map>
#include <vector>

namespace dap {
	class RenderObject;

	class Camera : public dap::Component {
	public:
		static const float NEAR;
		static const float FAR;

	private:
		static const std::map<int, std::vector<dap::RenderObject*>>* ALL_RENDERERS;

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
		glm::vec3 clearColor = glm::vec3 ( 0, 0, 0 );

	public:
		Camera ();
		virtual ~Camera () override;

	public:
		virtual void Initialize ();
		const static std::vector<dap::Camera*>* GetAllCameras ();
		glm::vec2 ScreenToWorld ( glm::vec2 point );
		void SetClearColor ( float r, float g, float b );
		void SetOrthoSize ( const float newSize );
		bool Render ( const float aspectRatio, const float interp );
	};
}
