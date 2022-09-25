#include "TilesetTile.h"

dap::TilesetTile::TilesetTile ( float x, float y, float width, float height ) {
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

void dap::TilesetTile::Deserialize ( const nlohmann::json* json ) {
	if ( json->contains ( "x" ) )
		X = json->at ( "x" );
	if ( json->contains ( "y" ) )
		Y = json->at ( "y" );
	if ( json->contains ( "width" ) )
		Width = json->at ( "width" );
	if ( json->contains ( "height" ) )
		Height = json->at ( "height" );
}

nlohmann::json dap::TilesetTile::Serialize () {
	nlohmann::json json = dap::JSONObject::Serialize ();
	json["x"] = X;
	json["y"] = Y;
	json["width"] = Width;
	json["height"] = Height;
	return json;
}
