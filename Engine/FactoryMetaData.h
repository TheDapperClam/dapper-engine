#pragma once

#include <functional>
#include <string>

namespace dap {
	class JSONobject;

	struct FactoryMetaData {
		friend class JSONobject;

	public:
		bool IsComponent = false;
		std::string Name = "";
		std::string TypeID = "";
		
	private:
		std::function<dap::JSONobject* const( )>* factory = NULL;

	public:
		FactoryMetaData ();
		FactoryMetaData ( std::string name, bool isComponent );
		~FactoryMetaData ();
	};
}
