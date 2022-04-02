#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <vector>
#include "Component.h"
#include "JSONobject.h"
#include "Transform.h"

namespace dap {
	class GameObject final : public JSONobject {
	public:
		GameObject ( std::string startingName = "GameObject" );
		~GameObject ();
		const std::string* GetName ();
		dap::Transform* const GetTransform ();
		void SetName ( const std::string newName );
		virtual void Deserialize ( const nlohmann::json* json );
		virtual const nlohmann::json Serialize ();
		void AddComponent ( dap::Component* const comp );

		template <class T, class... Args>
		T* AddComponent ( Args... args ) {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of AddComponent() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			T* component = new T ( args... );
			if ( !components.count ( key ) )
				components.insert ( std::pair<std::type_index, std::vector<dap::Component*>> ( key, {} ) );
			components[key].push_back ( component );
			component->SetOwner ( this );
			component->Initialize ();
			return component;
		}

		template <class T>
		T* GetComponent () {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of GetComponent() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			if ( components.count ( key ) ) {
				return (T*)components[key].front ();
			}
			return NULL;
		}

		template <class T>
		std::vector<T*> GetComponents () {
			static_assert( std::is_base_of<dap::Component, T>::value, "T of GetComponents() must derive from dap::Component" );
			const std::type_index key = typeid( T );
			std::vector<T*> typeVector;
			if ( components.count ( key ) ) {
				for ( auto *component : components[typeid( T )] ) {
					typeVector.push_back ( (T*) component );
				}
			}
			return typeVector;
		}
	protected:
		std::string name = "GameObject";
		std::map<std::type_index, std::vector<dap::Component*>> components = {};
		dap::Transform* transform = NULL;
	private:
		JSON_PRIVATE;
	};
}
