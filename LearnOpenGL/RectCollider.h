#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include <vector>

namespace dap {
	class RectCollider : public dap::Component {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	private:
		JSON_PRIVATE;

	private:
		static std::vector<dap::RectCollider*> allRectColliders;

	public:
		RectCollider ();
		RectCollider ( float width, float height );
		virtual ~RectCollider () override;
		bool Overlapping ( const dap::RectCollider* other );
		bool PointInside ( const glm::vec3 point );
		float GetWidth ();
		float GetHeight ();
		void SetWidth ( float w );
		void SetHeight ( float h );

	private:
		float width = 0.0f;
		float height = 0.0f;
	};
}
