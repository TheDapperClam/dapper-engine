#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace dap {
	class Transform : public dap::Component {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	private:
		glm::vec3 interpPositionStart = position;
		glm::vec3 interpPositionCurrent = position;
		glm::vec3 interpRotationStart = rotation;
		glm::vec3 interpRotationCurrent = rotation;
		glm::vec3 interpScaleStart = scale;
		glm::vec3 interpScaleCurrent = scale;
		glm::vec3 position = glm::vec3 ( 0, 0, 0 );
		glm::vec3 rotation = glm::vec3 ( 0, 0, 0 );
		glm::vec3 scale = glm::vec3 ( 1, 1, 1 );
		glm::mat4 matrix = glm::mat4 ( 1.0f );

	public:
		Transform ( glm::vec3 startingPos = glm::vec3 ( 0, 0, 0 ) );

	public:
		float GetInterpolationDist ();
		glm::vec3 GetPosition () const;
		glm::vec3 GetRotation () const;
		glm::vec3 GetScale () const;
		glm::mat4 GetMatrix () const;
		void Interpolate ( float t );
		void SetPosition ( glm::vec3 newPosition );
		void SetRotation ( glm::vec3 newRotation );
		void SetScale ( glm::vec3 newScale );
		void Translate ( glm::vec3 amount, bool interp = true );
		void Translate ( glm::vec2 amount, bool interp = true );
		void Rotate ( glm::vec3 amount, bool interp = true );
		void Scale ( glm::vec3 amount, bool interp = true );

	private:
		void BuildMatrix ( glm::mat4& mat );
	};
}
