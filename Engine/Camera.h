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

	private:
		static dap::Camera* Current;
		static dap::Camera* StartingCamera;

	public:
		static dap::Camera* const GetCurrent ();
		static dap::Camera* const GetStartingCamera ();
		static void SetCurrent ( dap::Camera* camera );
		static void SetStartingCamera ( dap::Camera* camera );

	private:
		glm::vec3 clearColor = glm::vec3 ( 0, 0, 0 );
		glm::mat4 projectionOrtho;
		float orthoSize = 1.0f;

	public:
		float GetOrthoSize ();
		virtual void Initialize ();
		bool IsStartingCamera ();
		glm::vec2 ScreenToWorld ( glm::vec2 point );
		void SetClearColor ( float r, float g, float b );
		void SetOrthoSize ( const float newSize );
		bool Render ( const float aspectRatio, const float interp );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	protected:
		virtual void End () override;
	};
}
