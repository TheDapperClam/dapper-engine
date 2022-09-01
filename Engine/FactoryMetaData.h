#pragma once

#include <functional>
#include <string>

namespace dap {
	class JSONObject;

	struct FactoryMetaData {
		friend class JSONObject;

	public:
		bool IsComponent = false;
		std::string Name = "";
		std::string TypeID = "";
		
	private:
		std::function<dap::JSONObject* const( )>* factory = NULL;

	public:
		FactoryMetaData ();
		FactoryMetaData ( std::string name, bool isComponent );
		~FactoryMetaData ();
	};
}
