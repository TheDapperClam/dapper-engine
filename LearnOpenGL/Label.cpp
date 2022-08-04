#include "Label.h"

#include "Font.h"
#include "FontCharacter.h"
#include "glm/glm.hpp"
#include "Resources.h"
#include "TextureAtlasSubTexture.h"

const float dap::Label::VERTEX_SCALE = 0.01f;

dap::Label::Label () {

}

void dap::Label::BuildElements () {
	unsigned int elementOffset = 0;

	elements.clear ();
	for ( const auto& character : text ) {
		elements.insert ( elements.end (), {
			elementOffset,
			elementOffset + 1,
			elementOffset + 2,
			elementOffset + 2,
			elementOffset + 3,
			elementOffset
		} );
		elementOffset += 4;
	}
}

void dap::Label::BuildVertices () {
	width = 0;
	for ( const auto& character : text ) {
		const dap::FontCharacter* CHAR_DATA = font->GetChar ( character );
		const float ADVANCE = ( CHAR_DATA->advance >> 6 ) * VERTEX_SCALE;
		const float H = CHAR_DATA->size.y * VERTEX_SCALE;

		height = std::max ( height, H );
		width += ADVANCE;
	}

	float charOffsetX = ( rightToLeft ? width : -width ) * (1.0f - textAlignX);
	float charOffsetY = -height * (1.0f - textAlignY);
	vertices.clear ();
	for ( const auto& character : text ) {
		const dap::FontCharacter* CHAR_DATA = font->GetChar ( character );
		const glm::vec2* TEX_COORDS = CHAR_DATA->texture->GetTexCoords ();
		const float ADVANCE = ( CHAR_DATA->advance >> 6 ) * VERTEX_SCALE;
		const float X = charOffsetX + CHAR_DATA->bearing.x * VERTEX_SCALE - ( rightToLeft ? ADVANCE : 0.0f );
		const float Y = charOffsetY - ( CHAR_DATA->size.y - CHAR_DATA->bearing.y) * VERTEX_SCALE;
		const float W = CHAR_DATA->size.x * VERTEX_SCALE;
		const float H = CHAR_DATA->size.y * VERTEX_SCALE;
	
		vertices.insert ( vertices.end (), {
			dap::Vertex ( glm::vec3 ( X, Y + H, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), TEX_COORDS[0] ),
			dap::Vertex ( glm::vec3 ( X, Y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), TEX_COORDS[3] ),
			dap::Vertex ( glm::vec3 ( X + W, Y, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), TEX_COORDS[2] ),
			dap::Vertex ( glm::vec3 ( X + W, Y + H, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), TEX_COORDS[1] )
		} );
		charOffsetX += ( rightToLeft ? -ADVANCE : ADVANCE );
	}
}

dap::Font* dap::Label::GetFont () {
	return font;
}

void dap::Label::SetFont ( dap::Font* font ) {
	this->font = font;
	// TODO: update vertex and element data here, but BuildGLObject crashes
}

void dap::Label::SetRightToLeft ( bool enabled ) {
	rightToLeft = enabled;
	BuildVertices ();
	BuildElements ();
	BuildGLObject ();
}

void dap::Label::SetText ( std::string newText ) {
	text = newText;
	BuildVertices ();
	BuildElements ();
	BuildGLObject ();
}

void dap::Label::SetTextAlign ( float x, float y ) {
	textAlignX = x;
	textAlignY = y;
	BuildVertices ();
	BuildElements ();
	BuildGLObject ();
}

void dap::Label::Deserialize ( const nlohmann::json* json ) {
	SetFont ( static_cast<dap::Font*> ( dap::Resources::Get ( json->at ( "font" ) ) ) );
	SetText ( json->at ( "text" ) );
	SetTextAlign ( json->at ( "textAlignX" ), json->at ( "textAlignY" ) );
	SetRightToLeft ( json->at ( "rightToLeft" ) );
}

const nlohmann::json dap::Label::Serialize () {
	nlohmann::json json = {
		{ "text", text },
		{ "font", font->GetName () },
		{ "textAlignX", textAlignX },
		{ "textAlignY", textAlignY },
		{ "rightToLeft", rightToLeft }
	};
	JSON_APPEND_TYPE(json)
	return json;
}
