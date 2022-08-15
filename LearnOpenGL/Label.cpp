#include "Label.h"

#include "Font.h"
#include "FontCharacter.h"
#include "glm/glm.hpp"
#include "Resources.h"
#include "TextureAtlasSubTexture.h"
#include <vector>

const float dap::Label::VERTEX_SCALE = 0.01f;
const char dap::Label::LINE_BREAK = '\n';

dap::Label::Label () {
	SetShader ( "shader_label" );
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
	labelWidth = 0.0f;
	labelHeight = 0.0f;
	std::vector<std::string> lines;
	float firstLineHeight = 0.0f;
	float newLineMaxHeight = 0.0f;
	float newLineMaxWidth = 0.0f;
	int subStrStart = 0;
	for ( int i = 0; i < text.size (); i++ ) {
		const char CHARACTER = text[i];
		switch ( CHARACTER ) {
			case LINE_BREAK:
				const std::string SUB_STR = text.substr ( subStrStart, i - subStrStart );
				lines.push_back ( SUB_STR );
				subStrStart = i + 1;
				labelHeight += lineHeight;
				newLineMaxHeight = 0.0f;
				newLineMaxWidth = 0.0f;
				continue;
				break;
		}

		const dap::FontCharacter* CHAR_DATA = font->GetChar ( CHARACTER );
		if ( !CHAR_DATA )
			continue;
		const float ADVANCE = ( CHAR_DATA->advance >> 6 ) * VERTEX_SCALE;
		const float H = CHAR_DATA->bearing.y * VERTEX_SCALE;
		newLineMaxHeight = std::max ( newLineMaxHeight, H );
		newLineMaxWidth += ADVANCE;
		if ( lines.size () <= 0 )
			firstLineHeight = newLineMaxHeight;
		labelWidth = std::max ( newLineMaxWidth, labelWidth );
	}
	lines.push_back ( text.substr ( subStrStart, text.size () - subStrStart ) );
	labelHeight += newLineMaxHeight;

	float alignX = -labelWidth * ( 1.0f - textAlignX ) + ( rightToLeft ? labelWidth : 0.0f );
	float alignY = labelHeight * ( 1.0f - textAlignY );
	float charOffsetY = 0.0f;
	vertices.clear ();
	for ( const auto& line : lines ) {
		float charOffsetX = 0.0f;
		for ( const auto& character : line ) {
			const dap::FontCharacter* CHAR_DATA = font->GetChar ( character );
			if ( !CHAR_DATA )
				continue;
			const glm::vec2* TEX_COORDS = CHAR_DATA->texture->GetTexCoords ();
			const float ADVANCE = ( CHAR_DATA->advance >> 6 ) * VERTEX_SCALE;
			const float X = charOffsetX + CHAR_DATA->bearing.x * VERTEX_SCALE - ( rightToLeft ? ADVANCE : 0.0f ) + alignX;
			const float Y = charOffsetY - ( CHAR_DATA->size.y - CHAR_DATA->bearing.y ) * VERTEX_SCALE + alignY - firstLineHeight;
			const float W = CHAR_DATA->size.x * VERTEX_SCALE;
			const float H = CHAR_DATA->size.y * VERTEX_SCALE;

			vertices.insert ( vertices.end (), {
				dap::Vertex ( glm::vec3 ( X, Y + H, 0.0f ), glm::vec3 ( 1.0f, 1.0f, 1.0f ), TEX_COORDS[0] ),
				dap::Vertex ( glm::vec3 ( X, Y, 0.0f ), glm::vec3 ( 1.0f, 1.0f, 1.0f ), TEX_COORDS[3] ),
				dap::Vertex ( glm::vec3 ( X + W, Y, 0.0f ), glm::vec3 ( 1.0f, 1.0f, 1.0f ), TEX_COORDS[2] ),
				dap::Vertex ( glm::vec3 ( X + W, Y + H, 0.0f ), glm::vec3 ( 1.0f, 1.0f, 1.0f ), TEX_COORDS[1] )
			} );
			charOffsetX += ( rightToLeft ? -ADVANCE : ADVANCE );
		}
		charOffsetY -= lineHeight;
	}
}

dap::Font* dap::Label::GetFont () {
	return font;
}

void dap::Label::SetFont ( dap::Font* font ) {
	this->font = font;
	// TODO: update vertex and element data here, but BuildGLObject crashes
}

void dap::Label::SetLineHeight ( float newHeight ) {
	lineHeight = newHeight;
	BuildVertices ();
	BuildGLObject ();
}

void dap::Label::SetRightToLeft ( bool enabled ) {
	rightToLeft = enabled;
	BuildVertices ();
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
	BuildGLObject ();
}

void dap::Label::Deserialize ( const nlohmann::json* json ) {
	dap::RenderObject::Deserialize ( json );
	SetFont ( static_cast<dap::Font*> ( dap::Resources::Get ( json->at ( "font" ) ) ) );
	SetText ( json->at ( "text" ) );
	SetLineHeight ( json->at ( "lineHeight" ) );
	SetTextAlign ( json->at ( "textAlignX" ), json->at ( "textAlignY" ) );
	SetRightToLeft ( json->at ( "rightToLeft" ) );
}

nlohmann::json dap::Label::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["text"] = text;
	json["font"] = font->GetName ();
	json["lineHeight"] = lineHeight;
	json["textAlignX"] = textAlignX;
	json["textAlignY"] = textAlignY;
	json["rightToLeft"] = rightToLeft;
	JSON_APPEND_TYPE(json)
	return json;
}
