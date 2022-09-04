#pragma once

#include "FactoryMetaData.h"
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <typeindex>
#include <iostream>

namespace dap {
	class JSONObject {
	private:
		static std::map<std::string, dap::FactoryMetaData*> RegisteredFactories;

	public:
		static void ClearFactories ();
		static dap::FactoryMetaData* const GetMetaData ( const std::string typeID );
		static const std::map<std::string, dap::FactoryMetaData*>* GetRegisteredFactories ();
		static void RegisterEngineObjects ();

	public:
		virtual void Deserialize ( const nlohmann::json* json ) = 0;
		virtual nlohmann::json Serialize ();

	public:
		static dap::JSONObject* const Factory ( const std::string typeID ) {
			if ( !RegisteredFactories.count ( typeID ) ) {
				std::cout << "[JSONOBJECT] Factory for type " + typeID + " not found" << std::endl;
				return NULL;
			}
			dap::JSONObject* const newObject = ( *RegisteredFactories.at ( typeID )->factory ) ( );
			return newObject;
		}

		template <class T>
		static bool RegisterFactory ( dap::FactoryMetaData* metaData ) {
			static_assert( std::is_base_of<JSONObject, T>::value, "T of RegisterFactory must derive from JSONobject" );
			const std::string key = typeid( T ).name ();
			std::cout << "[JSONOBJECT] Registering factory for type " + key << std::endl;
			if ( RegisteredFactories.count ( key ) ) {
				std::cout << "[JSONOBJECT] Factory for type " + key + " already registered";
				return false;
			}
			metaData->TypeID = key;
			metaData->factory = new std::function<dap::JSONObject* const()> ( []() -> dap::JSONObject* const {
				dap::JSONObject* const newObject = new T ();
				return newObject;
			} );
			RegisteredFactories.insert ( std::pair<std::string, dap::FactoryMetaData*> ( key, metaData ) );
			return true;
		}
	};
}
