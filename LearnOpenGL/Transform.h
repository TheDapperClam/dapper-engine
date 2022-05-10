#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace dap {
	class Transform : public dap::Component {
	public:
		Transform ( const glm::vec3 startingPos = glm::vec3 ( 0, 0, 0 ) );
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
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;
	private:
		JSON_PRIVATE;
		glm::vec3 position = glm::vec3 ( 0, 0, 0 );
		glm::vec3 rotation = glm::vec3 ( 0, 0, 0 );
		glm::vec3 scale = glm::vec3 ( 1, 1, 1 );
		glm::mat4 matrix = glm::mat4 ( 1.0f );
		void BuildMatrix ( glm::mat4* mat );
	};
}
