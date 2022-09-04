#pragma once

#include "ImGuiObject.h"
#include "JSONobject.h"
#include <string>

namespace dap {
	class GameObject;
	class RectCollider;

	class Component : public dap::JSONObject, public dap::ImGuiObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	protected:
		bool hasStarted = false;
		bool isEnabled = true;
		dap::GameObject* owner = 0;
		
	public:
		// Destructor must be virtual, otherwise derived destructors aren't called
		virtual ~Component ();

	public:
		bool GetEnabled ();
		dap::GameObject* const GetOwner ();
		virtual void Initialize ();
		void SetEnabled ( bool enabled );
		void SetOwner ( dap::GameObject* const newOwner );

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
	};
}
