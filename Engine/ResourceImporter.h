#pragma once
 
#include <string>
#include <vector>

namespace dap {
	class ResourceImporter {
	public:
		virtual void ClearImports () = 0;
		virtual void Import ( std::string key, std::string path ) = 0;
		virtual void TextureAtlasBuilt () {};
		virtual void ImportingComplete () {};
		virtual std::vector<std::string> GetExtensions () = 0;
	};
}
