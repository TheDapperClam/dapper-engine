#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace dap {
	class Transform : public dap::Component {
	JSON_CLASS

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	public:
		Transform ( const glm::vec3 startingPos = glm::vec3 ( 0, 0, 0 ) );

	public:
		const glm::vec3* GetPosition () const;
		const glm::vec3* GetRotation () const;
		const glm::vec3* GetScale () const;
		const glm::mat4* GetMatrix () const;
		void SetPosition ( const glm::vec3 newPosition );
		void SetRotation ( const glm::vec3 newRotation );
		void SetScale ( const glm::vec3 newScale );
		void Translate ( const glm::vec3 amount );
		void Rotate ( const glm::vec3 amount );
		void Scale ( const glm::vec3 amount );
		
	private:
		glm::vec3 position = glm::vec3 ( 0, 0, 0 );
		glm::vec3 rotation = glm::vec3 ( 0, 0, 0 );
		glm::vec3 scale = glm::vec3 ( 1, 1, 1 );
		glm::mat4 matrix = glm::mat4 ( 1.0f );

	private:
		void BuildMatrix ( glm::mat4* mat );
	};
}
