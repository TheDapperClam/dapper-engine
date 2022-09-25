#include "TextureAtlasSubTexture.h"

#include "glm/glm.hpp"
#include "TextureAtlas.h"

dap::TextureAtlasSubTexture::TextureAtlasSubTexture ( std::string name, int imageX, int imageY, float texX, float texY, unsigned int imageWidth, unsigned int imageHeight, float texWidth, float texHeight ) {
	Name = name;
	ImageX = imageX;
	ImageY = imageY;
	TexX = texX;
	TexY = texY;
	ImageWidth = imageWidth;
	ImageHeight = imageHeight;
	TexHeight = texHeight;
	TexWidth = texWidth;
}

void dap::TextureAtlasSubTexture::Deserialize ( const nlohmann::json* json ) {
	if ( json->contains ( "imageX" ) )
		ImageX = json->at ( "imageX" );
	if ( json->contains ( "imageY" ) )
		ImageY = json->at ( "imageY" );
	if ( json->contains ( "x" ) )
		TexX = json->at ( "x" );
	if ( json->contains ( "y" ) )
		TexY = json->at ( "y" );
	if ( json->contains ( "imageWidth" ) )
		ImageWidth = json->at ( "imageWidth" );
	if ( json->contains ( "imageHeight" ) )
		ImageHeight = json->at ( "imageHeight" );
	if ( json->contains ( "width" ) )
		TexWidth = json->at ( "width" );
	if ( json->contains ( "height" ) )
		TexHeight = json->at ( "height" );
}

std::vector<glm::vec2> dap::TextureAtlasSubTexture::GetTexCoords () const {
	std::vector<glm::vec2> coords;
	coords.insert ( coords.end (), { 
		glm::vec2 ( TexX, TexY ),
		glm::vec2 ( TexX + TexWidth, TexY ),
		glm::vec2 ( TexX + TexWidth, TexY + TexHeight ),
		glm::vec2 ( TexX, TexY + TexHeight )
	} );
	return coords;
}

void dap::TextureAtlasSubTexture::MapToSubTexture ( dap::TextureAtlasSubTexture* texture ) {
	if ( !texture )
		return;
	const unsigned int ATLAS_WIDTH = dap::TextureAtlas::GetMainAtlas ()->GetWidth ();
	const unsigned int ATLAS_HEIGHT = dap::TextureAtlas::GetMainAtlas ()->GetHeight ();
	TexX = (float) ImageX / (float) ATLAS_WIDTH + texture->TexX;
	TexY = (float) ImageY / (float) ATLAS_HEIGHT + texture->TexY;
	TexWidth = (float) ImageWidth / (float) ATLAS_WIDTH;
	TexHeight = (float) ImageHeight / (float) ATLAS_HEIGHT;
	std::cout << "[TEXTUREATLASSUBTEXTURE] Finished mapping: " << TexX << ", " << TexY << ", " << TexWidth << ", " << TexHeight << std::endl;
}

nlohmann::json dap::TextureAtlasSubTexture::Serialize () {
	nlohmann::json json = dap::JSONObject::Serialize ();
	json["imageX"] = ImageX;
	json["imageY"] = ImageY;
	json["x"] = TexX;
	json["y"] = TexY;
	json["imageWidth"] = ImageWidth;
	json["imageHeight"] = ImageHeight;
	json["width"] = TexWidth;
	json["height"] = TexHeight;
	return json;
}
