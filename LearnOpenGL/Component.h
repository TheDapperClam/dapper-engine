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
		//bool isInitialized = false;
		bool isEnabled = true;

	public:
		virtual ~Component () {};

	public:
		bool GetEnabled ();
		dap::GameObject* const GetOwner ();
		virtual void Initialize ();
		void SetEnabled ( bool enabled );
		void SetOwner ( dap::GameObject* const newOwner );
	};
}
