#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include <vector>

namespace dap {
	class RectCollider : public dap::Component {
	public:
		RectCollider ();
		virtual ~RectCollider () override;
		static const bool Overlaps ( const dap::RectCollider* rectA, const dap::RectCollider* rectB );
		static const bool Inside ( const dap::RectCollider* rectA, const dap::RectCollider* rectB );
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;
	private:
		JSON_PRIVATE;
		static std::vector<dap::RectCollider*> allRectColliders;
	};
}
