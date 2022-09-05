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
		int collisionLayer = 1;
		int collisionMask = 1;
		float width = 1.0f;
		float height = 1.0f;
	
	public:
		RectCollider ();
		virtual ~RectCollider () override;

	public:
		int GetCollisionLayer ();
		int GetCollisionMask ();
		float GetHeight ();
		float GetWidth ();
		bool Overlapping ( const dap::RectCollider* other );
		bool PointInside ( const glm::vec3 point );
		void SetCollisionLayer ( int layer );
		void SetCollisionMask ( int mask );
		void SetWidth ( float w );
		void SetHeight ( float h );
		bool TestCollisionLayers ( dap::RectCollider* colliderAgainst );
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
