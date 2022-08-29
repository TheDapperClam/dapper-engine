#pragma once

#include "Component.h"
#include "ImGuiObject.h"
#include <iostream>
#include "JSONobject.h"
#include <map>
#include <string>
#include <typeindex>

namespace dap {
	class Scene;
	class Transform;

	class GameObject final : public dap::JSONobject, public dap::ImGuiObject {
		friend dap::Scene;

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
		unsigned int GetId ();
		std::string GetName ();
		dap::Transform* const GetTransform ();
		void RemoveComponent ( dap::Component* component );
		void SavePrefab ( std::string fileName );
		
	public:
		template <class T, class... Args>
		T* AddComponent ( Args... args ) {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of AddComponent() must derive from dap::Component" );
			const std::type_index KEY = typeid( T );
			std::cout << "[GAMEOBJECT] Adding component: " << KEY.name () << " to " << name << std::endl;
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

	protected:
		static unsigned int nextId;
		std::string name = "GameObject";
		const unsigned int id = -1;
		std::map<std::type_index, std::vector<dap::Component*>> components = {};
		dap::Transform* transform = NULL;
		dap::Scene* owningScene = NULL;

	private:
		// We only want GameObjects to be created, and deleted through their owning scene
		GameObject ( dap::Scene* owningScene, std::string startingName = "GameObject" );
		void operator delete ( void* obj ) { free ( obj ); };

	private:
		void SetName ( std::string newName );
	};
}
