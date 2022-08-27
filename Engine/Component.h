#pragma once

#include "ImGuiObject.h"
#include "JSONobject.h"

namespace dap {
	class GameObject;

	class Component : public dap::JSONobject, public dap::ImGuiObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	protected:
		dap::GameObject* owner;
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
