#pragma once

#include "JSONobject.h"

namespace dap {
	class GameObject;

	class Component : public JSONobject {
	public:
		virtual ~Component () {};
		virtual void Initialize ();
		void SetOwner ( dap::GameObject* const newOwner );
		dap::GameObject* const GetOwner ();
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;
	protected:
		dap::GameObject* owner;
	};
}
