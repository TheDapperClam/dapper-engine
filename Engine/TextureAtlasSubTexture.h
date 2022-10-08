#pragma once

#include "glm/glm.hpp"
#include "JSONObject.h"
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

namespace dap {
	struct TextureAtlasSubTexture : public dap::JSONObject {
	public:
		TextureAtlasSubTexture ( std::string name, int imageX, int imageY, float texX, float texY, unsigned int imageWidth, unsigned int imageHeight, float texWidth, float texHeight );

	public:
		std::string Name = "";
		int ImageX = 0, ImageY = 0;
		float TexX = 0.0f, TexY = 0.0f;
		unsigned int ImageWidth = 0.0f, ImageHeight = 0.0f;
		float TexWidth = 0.0f, TexHeight = 0.0f;

	public:
		std::vector<glm::vec2> GetTexCoords () const;
		void MapToSubTexture ( const dap::TextureAtlasSubTexture* TEXTURE );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;
	};
}
