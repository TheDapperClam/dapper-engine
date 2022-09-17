#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace dap {
	class Transform : public dap::Component {
	public:
		static const glm::vec2 DEFAULT_POSITION;
		static const float DEFAULT_ROTATION;
		static const glm::vec2 DEFAULT_SCALE;

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	private:
		glm::vec2 interpPositionStart = position;
		glm::vec2 interpPositionCurrent = position;
		float interpRotationStart = rotation;
		float interpRotationCurrent = rotation;
		glm::vec2 interpScaleStart = scale;
		glm::vec2 interpScaleCurrent = scale;
		glm::vec2 matrixTranslateScaling = glm::vec2 ( 1, 1 );
		glm::vec2 position = DEFAULT_POSITION;
		float rotation = DEFAULT_ROTATION;
		glm::vec2 scale = DEFAULT_SCALE;
		glm::mat4 matrix = glm::mat4 ( 1.0f );
		float zOffset = 0.0f;

	public:
		Transform ( glm::vec2 startingPosition = DEFAULT_POSITION, float startingRotation = DEFAULT_ROTATION, glm::vec2 startingScale = DEFAULT_SCALE );

	public:
		glm::vec2 GetInterpPosition () const;
		float GetInterpRotation () const;
		glm::vec2 GetInterpScale () const;
		glm::vec2 GetPosition () const;
		float GetRotation () const;
		glm::vec2 GetScale () const;
		glm::mat4 GetMatrix () const;
		void Interpolate ( float t );
		void SetMatrixTranslateScaling ( glm::vec2 scale );
		void SetPosition ( glm::vec2 newPosition );
		void SetRotation ( float newRotation );
		void SetScale ( glm::vec2 newScale );
		void SetZOffset ( float newOffset );
		void Translate ( glm::vec2 amount, bool interp = true );
		void Rotate ( float amount, bool interp = true );
		void Scale ( glm::vec2 amount, bool interp = true );

	private:
		void BuildMatrix ( glm::mat4& mat );
	};
}
