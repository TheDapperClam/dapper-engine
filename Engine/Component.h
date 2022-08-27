#pragma once

#include "ImGuiObject.h"
#include "JSONobject.h"
#include <string>

namespace dap {
	class GameObject;

	class Component : public dap::JSONobject, public dap::ImGuiObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	protected:
		dap::GameObject* owner = 0;
		bool isEnabled = true;
		std::string name = "Component";
		
	public:
		virtual ~Component () {};

	public:
		bool GetEnabled ();
		std::string GetName ();
		dap::GameObject* const GetOwner ();
		virtual void Initialize ();
		void SetEnabled ( bool enabled );
		void SetOwner ( dap::GameObject* const newOwner );
	};
}
