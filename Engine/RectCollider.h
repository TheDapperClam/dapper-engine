#pragma once

#include "RenderObject.h"
#include "glm/glm.hpp"
#include <vector>

namespace dap {
	class RectCollider : public dap::RenderObject {
		friend class Collision;

	private:
		static std::vector<dap::RectCollider*> allRectColliders;

	protected:
		float width = 1.0f;
		float height = 1.0f;
	
	public:
		RectCollider ();
		virtual ~RectCollider () override;

	public:
		float GetHeight ();
		float GetWidth ();
		bool Overlapping ( const dap::RectCollider* other );
		bool PointInside ( const glm::vec3 point );
		void SetWidth ( float w );
		void SetHeight ( float h );
		virtual bool Use () const override;

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual void ImGuiWidgetsBegin () override;
		virtual nlohmann::json Serialize () override;

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;
	};
}
