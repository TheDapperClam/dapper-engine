#pragma once

#include "JSONObject.h"
#include "nlohmann/json.hpp"

namespace dap {
	struct TextureAtlasSubTexture;

	struct TilesetTile : public dap::JSONObject {
		float X, Y;
		float Width, Height;

		TilesetTile ( float x, float y, float width, float height );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;
	};
};
