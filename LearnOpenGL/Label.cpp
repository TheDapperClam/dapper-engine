#include "Label.h"

#include "Font.h"
#include "FontCharacter.h"
#include "glm/glm.hpp"
#include "Resources.h"
#include "TextureAtlasSubTexture.h"

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
	const float SCALE = 0.01f;
	float charOffsetX = 0.0f;
	float charOffsetY = 0.0f;

	vertices.clear ();
	for ( const auto& character : text ) {
		const dap::FontCharacter* charData = font->GetChar ( character );
		const glm::vec2* texCoords = charData->texture->GetTexCoords ();

		const float x = charOffsetX + charData->bearing.x * SCALE;
		const float y = charOffsetY - (charData->size.y - charData->bearing.y) * SCALE;
		const float w = charData->size.x * SCALE;
		const float h =  charData->size.y * SCALE;

		vertices.insert ( vertices.end (), {
			dap::Vertex ( glm::vec3 ( x, y + h, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), texCoords[0] ),
			dap::Vertex ( glm::vec3 ( x, y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[3] ),
			dap::Vertex ( glm::vec3 ( x + w, y, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), texCoords[2] ),
			dap::Vertex ( glm::vec3 ( x + w, y + h, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[1] )
		} );
		charOffsetX += ( charData->advance >> 6 ) * SCALE;
	}
}

dap::Font* dap::Label::GetFont () {
	return font;
}

void dap::Label::SetFont ( dap::Font* font ) {
	this->font = font;
	// TODO: update vertex and element data here, but BuildGLObject crashes
}

void dap::Label::SetText ( std::string newText ) {
	text = newText;
	BuildVertices ();
	BuildElements ();
	BuildGLObject ();
}

void dap::Label::Deserialize ( const nlohmann::json* json ) {
	SetFont ( static_cast<dap::Font*> ( dap::Resources::Get ( json->at ( "font" ) ) ) );
	SetText ( json->at ( "text" ) );
}

const nlohmann::json dap::Label::Serialize () {
	nlohmann::json json = {
		{ "text", text },
		{ "font", font->GetName () }
	};
	JSON_APPEND_TYPE(json)
	return json;
}
