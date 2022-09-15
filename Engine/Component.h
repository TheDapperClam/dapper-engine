#pragma once

#include "CollisionData.h"
#include "ImGuiObject.h"
#include "JSONobject.h"
#include <string>
#include <vector>

namespace dap {
	class GameObject;

	class Component : public dap::JSONObject, public dap::ImGuiObject {
	private:
		static std::vector<dap::Component*> ComponentsToProcess;

	public:
		static const std::vector<dap::Component*>* GetComponentsToProcess ();

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	private:
		const bool PROCESS_GAME_FUNCTIONS = true;

	protected:
		bool hasStarted = false;
		bool isEnabled = true;
		dap::GameObject* owner = 0;
		
	public:
		Component ( bool processGameFunctions = true );
		// Destructor must be virtual, otherwise derived destructors aren't called
		virtual ~Component ();

	public:
		bool GetEnabled ();
		dap::GameObject* const GetOwner () const;
		virtual void Initialize ();
		void SetEnabled ( bool enabled );
		void SetOwner ( dap::GameObject* const newOwner );

	public:
		void BaseCollision ( dap::CollisionData& data );
		void BaseEnd ();
		void BaseInput ();
		void BaseStart ();
		void BaseTick ();

	protected:
		virtual void Collision ( dap::CollisionData& data ) {};
		virtual void End () {};
		virtual void Input () {};
		virtual void Start () {};
		virtual void Tick () {};
	};
}
