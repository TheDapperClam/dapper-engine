#pragma once
 
#include <string>
#include <vector>

namespace dap {
	class ResourceImporter {
	public:
		virtual void Import ( std::string key, std::string path ) const = 0;
		virtual std::vector<std::string> GetExtensions () const = 0;
	};
}
