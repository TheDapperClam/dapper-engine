#pragma once

#include "Component.h"
#include "Game.h"
#include "ImGuiWidget.h"
#include <iostream>
#include "JSONobject.h"
#include <unordered_map>
#include <string>
#include <typeindex>
#include <vector>

namespace dap {
	class Scene;
	class Transform;

	class GameObject final : public dap::JSONObject, public dap::ImGuiWidget {
		FUNC_GET_FILE_EXTENSION(".dpf")

		friend dap::Scene;

	private:
		static unsigned long NextId;

	public:
		static void ResetNextId ();

	private:
		const unsigned long ID = 0;

	private:
		std::unordered_map<std::type_index, std::vector<dap::Component*>> components = {};
		std::string name = "GameObject";
		dap::Scene* owningScene = NULL;
		dap::Transform* transform = NULL;
		bool markedForDeletion = false;

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	public:
		GameObject ( dap::Scene* owningScene, std::string startingName = "GameObject" );
		~GameObject ();

	public:
		void AddComponent ( dap::Component* const comp );
		void Destroy ();
		const std::unordered_map<std::type_index, std::vector<dap::Component*>>* GetAllComponents ();
		unsigned long GetId ();
		std::string GetName ();
		dap::Transform* const GetTransform ();
		bool IsMarkedForDeletion ();
		void RemoveComponent ( dap::Component* component, bool shouldDelete = true );

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
			dap::Game* currentGame = dap::Game::GetCurrent ();
			if ( currentGame && currentGame->IsRunning () ) {
				component->BaseStart ();
			}
			return component;
		}

		template <class T>
		T* GetComponent () {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of GetComponent() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			if ( !components.count ( key ) )
				return NULL;
			return (T*)components[key].front ();
		}

		template <class T>
		std::vector<T*> GetComponents () {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of GetComponents() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			std::vector<T*> typeVector;
			if ( components.count ( key ) ) {
				for ( auto* component : components[typeid( T )] ) {
					typeVector.push_back ( (T*)component );
				}
			}
			return typeVector;
		}
	};
}
