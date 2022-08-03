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
	float xOffset = 0.0f;

	vertices.clear ();
	for ( const auto& character : text ) {
		const dap::FontCharacter* charData = font->GetChar ( character );
		const glm::vec2* texCoords = charData->texture->GetTexCoords ();

		vertices.insert ( vertices.end (), {
			dap::Vertex ( glm::vec3 ( -0.5f + xOffset,  0.5f, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), texCoords[0] ),
			dap::Vertex ( glm::vec3 ( 0.5f + xOffset,  0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[1] ),
			dap::Vertex ( glm::vec3 ( 0.5f + xOffset, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), texCoords[2] ),
			dap::Vertex ( glm::vec3 ( -0.5f + xOffset, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[3] )
		} );
		xOffset += 1.0f;
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
