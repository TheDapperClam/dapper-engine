#include "Sprite.h"

#include <algorithm>
#include "Resources.h"
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"
#include "Vertex.h"

dap::Sprite::Sprite () {
	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
}

void dap::Sprite::BuildElements () {
	vertices = {
		dap::Vertex ( glm::vec3 ( -0.5f,  0.5f, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), glm::vec2 ( 0.0f, 1.0f ) ),
		dap::Vertex ( glm::vec3 ( 0.5f,  0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), glm::vec2 ( 1.0f, 1.0f ) ),
		dap::Vertex ( glm::vec3 ( 0.5f, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), glm::vec2 ( 1.0f, 0.0f ) ),
		dap::Vertex ( glm::vec3 ( -0.5f, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), glm::vec2 ( 0.0f, 0.0f ) )
	};
}

void dap::Sprite::BuildVertices () {
	elements = {
		0, 1, 2,
		2, 3, 0
	};
}

const int* dap::Sprite::GetLayer () const {
	return &layer;
}

void dap::Sprite::RefreshTexCoords () {
	if ( !texture )
		return;
	const glm::vec2* texCoords = texture->GetTexCoords ();
	for ( int i = 0; i < vertices.size (); i++ ) {
		vertices.at ( i ).texCoord = texCoords[i];
	}
}

void dap::Sprite::SetLayer ( const int newLayer ) {
	// TODO: add render object sorting
	//layer = newLayer;
	//std::sort ( allRenderers.begin (), allRenderers.end (), []( const dap::Sprite* spriteA, const dap::Sprite* spriteB ) {
	//	return *spriteA->GetLayer () > *spriteB->GetLayer ();
	//} );
}

void dap::Sprite::SetTexture ( std::string name ) {
	auto* newTexture = dap::TextureAtlas::GetMainAtlas ()->GetSubTexture ( name );
	if ( !newTexture )
		return;
	texture = newTexture;
	RefreshTexCoords ();
	BuildGLObject ();
}

bool dap::Sprite::Use () const {
	if ( !texture )
		return false;
	dap::RenderObject::Use ();
	return true;
}

void dap::Sprite::Deserialize ( const nlohmann::json* json ) {
	SetLayer ( json->at ( "layer" ) );
	SetTexture ( json->at ( "texture" ) );
}

const nlohmann::json dap::Sprite::Serialize () {
	nlohmann::json json = {
		{ "type", typeid( *this ).name () },
		{ "layer", layer },
		{ "texture", texture->name }
	};
	return json;
}
