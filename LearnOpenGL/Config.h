#pragma once

#include <any>
#include <map>
#include <string>

namespace dap {
	typedef std::pair<std::string, std::any> ConfigProp;

	class Config {
	public:
		template<class T>
		static T GetProperty ( std::string key ) {
			if ( !properties.count ( key ) )
				return NULL;
			return std::any_cast<T>( properties.at ( key ) );
		};

	private:
		static std::map<std::string, std::any> properties;
	};
}
