#pragma once

#include "JSONobject.h"

namespace dap {
	class GameObject;

	class Component : public JSONobject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	protected:
		dap::GameObject* owner;

	public:
		virtual ~Component () {};

	public:
		virtual void Initialize ();
		dap::GameObject* const GetOwner ();
		void SetOwner ( dap::GameObject* const newOwner );
	};
}
