#pragma once

#include "Component.h"
#include "nlohmann/json.hpp"

namespace dap {
	class RectCollider;

	class Controller : public dap::Component {
	private:
		bool hasStarted = false;

	public:
		void BaseCollision ( dap::RectCollider* other );
		void BaseEnd ();
		void BaseInput ();
		void BaseStart ();
		void BaseTick ();

	protected:
		virtual void Collision ( dap::RectCollider* other ) {};
		virtual void End () {};
		virtual void Input () {};
		virtual void Start () {};
		virtual void Tick () {};

	protected:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;
	};
}
