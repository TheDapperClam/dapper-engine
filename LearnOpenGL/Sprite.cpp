#include "Sprite.h"

#include <algorithm>
#include "Resources.h"

dap::Sprite::Sprite () {
	BuildRO ();
}

dap::Sprite::~Sprite () {
	glDeleteVertexArrays ( 1, &vertexArrayObject );
	glDeleteBuffers ( 1, &vertexBufferObject );
	glDeleteBuffers ( 1, &elementBufferObject );
	allRenderers.erase ( std::remove ( allRenderers.begin (), allRenderers.end (), this ), allRenderers.end () );
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

GLuint dap::Sprite::GetDrawMode () const {
	return GL_TRIANGLES;
}

const int* dap::Sprite::GetLayer () const {
	return &layer;
}

dap::Texture* dap::Sprite::GetTexture () {
	return texture;
}

unsigned int dap::Sprite::GetRenderCount () const {
	return elements.size () * 3;
}

void dap::Sprite::SetLayer ( const int newLayer ) {
	//layer = newLayer;
	//std::sort ( allRenderers.begin (), allRenderers.end (), []( const dap::Sprite* spriteA, const dap::Sprite* spriteB ) {
	//	return *spriteA->GetLayer () > *spriteB->GetLayer ();
	//} );
}

void dap::Sprite::SetTexture ( dap::Texture* newTexture ) {
	texture = newTexture;
}

bool dap::Sprite::Use () const {
	dap::RenderObject::Use ();
	if ( !texture )
		return false;
	texture->Use ();
	return true;
}

void dap::Sprite::Deserialize ( const nlohmann::json* json ) {
	SetLayer ( json->at ( "layer" ) );
	SetTexture ( static_cast<dap::Texture*>( dap::Resources::Get ( json->at ( "texture" ) ) ) );
}

const nlohmann::json dap::Sprite::Serialize () {
	nlohmann::json json = {
		{ "type", typeid( *this ).name () },
		{ "layer", layer },
		{ "texture", *texture->GetName () }
	};
	return json;
}
