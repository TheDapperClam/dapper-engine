#pragma once

#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <typeindex>

#define JSON_PRIVATE static const bool json_factory_registered;
#define JSON_REGISTER(type) const bool type::json_factory_registered = dap::JSONobject::RegisterFactory<type>();

namespace dap {
	class JSONobject {
	public:
		static void ClearFactories ();
		virtual void Deserialize ( const nlohmann::json* json ) = 0;
		virtual const nlohmann::json Serialize () = 0;

		static dap::JSONobject* const Factory ( const std::string typeID ) {
			if ( !GetRegisteredFactories ().count ( typeID ) )
				return NULL;
			dap::JSONobject* const newObject = ( *GetRegisteredFactories ().at ( typeID ) ) ( );
			return newObject;
		}
	protected:
		template <class T>
		static bool RegisterFactory () {
			static_assert( std::is_base_of<JSONobject, T>::value, "T of RegisterFactory must derive from JSONobject" );
			const std::string key = typeid( T ).name ();
			if ( GetRegisteredFactories ().count ( key ) )
				return false;
			const std::function<dap::JSONobject* const( )>* factory = new std::function<dap::JSONobject* const()> ( []() -> dap::JSONobject* const {
				dap::JSONobject* const newObject = new T ();
				return newObject;
			} );
			GetRegisteredFactories ().insert ( std::pair<std::string, const std::function<dap::JSONobject* const ( )>*> ( key, factory ) );
			return true;
		}
	private:
		static std::map<std::string, const std::function<dap::JSONobject* const( )>*>& GetRegisteredFactories () {
			static std::map<std::string, const std::function<dap::JSONobject* const( )>*> registeredFactories;
			return registeredFactories;
		}
	};
}
