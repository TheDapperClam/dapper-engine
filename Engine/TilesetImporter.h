#pragma once

#include "ResourceImporter.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace dap {
	class Tileset;

	class TilesetImporter : public dap::ResourceImporter {
	private:
		static std::unordered_map<dap::Tileset*, std::string> TilesetPaths;

	public:
		virtual void ClearImports () override;
		virtual void Import ( std::string key, std::string path ) override;
		virtual void TextureAtlasBuilt () override;
		virtual std::vector<std::string> GetExtensions () override;
	};
}
