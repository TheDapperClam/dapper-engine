#pragma once

#include "Component.h"
#include "ImGuiObject.h"
#include <iostream>
#include "JSONobject.h"
#include <map>
#include <string>
#include <typeindex>
#include <vector>

namespace dap {
	class Scene;
	class Transform;

	class GameObject final : public dap::JSONObject, public dap::ImGuiObject {
		friend dap::Scene;

	private:
		static unsigned long NextId;

	private:
		const unsigned long ID = 0;

	private:
		std::map<std::type_index, std::vector<dap::Component*>> components = {};
		std::string name = "GameObject";
		dap::Scene* owningScene = NULL;
		bool shouldBeDestroyed = false;
		dap::Transform* transform = NULL;

	public:
		virtual void Deserialize ( const nlohmann::json* json );
		virtual nlohmann::json Serialize ();

	public:
		virtual void ImGuiWidgetsBegin () override;

	public:
		~GameObject ();

	public:
		void AddComponent ( dap::Component* const comp );
		void Destroy ();
		const std::vector<dap::Component*> GetAllComponentsList ();
		unsigned long GetId ();
		std::string GetName ();
		dap::Transform* const GetTransform ();
		void RemoveComponent ( dap::Component* component, bool shouldDelete = true );
		void SavePrefab ( std::string fileName );
		bool ShouldBeDestroyed () const;

	private:
		// We only want GameObjects to be created, and deleted through their owning scene
		GameObject ( dap::Scene* owningScene, std::string startingName = "GameObject" );

	private:
		void SetName ( std::string newName );

	public:
		template <class T, class... Args>
		T* AddComponent ( Args... args ) {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of AddComponent() must derive from dap::Component" );
			const std::type_index KEY = typeid( T );
			T* component = new T ( args... );
			if ( !components.count ( KEY ) )
				components.insert ( std::pair<std::type_index, std::vector<dap::Component*>> ( KEY, {} ) );
			components[KEY].push_back ( component );
			component->SetOwner ( this );
			component->Initialize ();
			return component;
		}

		template <class T>
		T* GetComponent () {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of GetComponent() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			if ( !components.count ( key ) )
				std::cout << "GAMEOBJECT ERROR: GameObject " << name << " does not have component " << key.name () << std::endl;
			return (T*)components[key].front ();
		}

		template <class T>
		std::vector<T*> GetComponents () {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of GetComponents() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			std::vector<T*> typeVector;
			if ( !components.count ( key ) )
				std::cout << "GAMEOBJECT ERROR: GameObject " << name << " does not have component " << key.name () << std::endl;
			for ( auto* component : components[typeid( T )] ) {
				typeVector.push_back ( (T*)component );
			}
			return typeVector;
		}
	};
}
