#include "TextureAtlasSubTexture.h"

#include "glm/glm.hpp"

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

std::vector<glm::vec2> dap::TextureAtlasSubTexture::GetTexCoords () {
	std::vector<glm::vec2> coords;
	coords.insert ( coords.end (), { 
		glm::vec2 ( TexX, TexY ),
		glm::vec2 ( TexX + TexWidth, TexY ),
		glm::vec2 ( TexX + TexWidth, TexY + TexHeight ),
		glm::vec2 ( TexX, TexY + TexHeight )
	} );
	return coords;
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
