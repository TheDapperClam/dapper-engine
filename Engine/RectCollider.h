#pragma once

#include "RenderObject.h"
#include "glm/glm.hpp"
#include <vector>

namespace dap {
	class RectCollider : public dap::RenderObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	private:
		static std::vector<dap::RectCollider*> allRectColliders;

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

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;

	private:
		float width = 0.0f;
		float height = 0.0f;
	};
}
