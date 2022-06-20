#include "Sprite.h"

#include <algorithm>
#include "Rendering.h"

JSON_REGISTER ( dap::Sprite );

const dap::Vertex dap::Sprite::VERTICES[] = {
	dap::Vertex ( glm::vec3 ( -0.5f,  0.5f, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), glm::vec2 ( 0.0f, 1.0f ) ),
	dap::Vertex ( glm::vec3 (  0.5f,  0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), glm::vec2 ( 1.0f, 1.0f ) ),
	dap::Vertex ( glm::vec3 (  0.5f, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), glm::vec2 ( 1.0f, 0.0f ) ),
	dap::Vertex ( glm::vec3 ( -0.5f, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), glm::vec2 ( 0.0f, 0.0f ) ),
};

const unsigned int dap::Sprite::TRIANGLES[] = {
	0, 1, 2,
	2, 3, 0
};

dap::Sprite::Sprite () {
	glGenVertexArrays ( 1, &vertexArrayObject );
	glBindVertexArray ( vertexArrayObject );

	glGenBuffers ( 1, &vertexBufferObject );
	glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferObject );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( VERTICES ), VERTICES, GL_STATIC_DRAW );

	glGenBuffers ( 1, &elementBufferObject );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, elementBufferObject );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( TRIANGLES ), TRIANGLES, GL_STATIC_DRAW );

	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*) 0 );
	glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)( 3 * sizeof ( float ) ) );
	glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)( 6 * sizeof ( float ) ) );
	glEnableVertexAttribArray ( 0 );
	glEnableVertexAttribArray ( 1 );
	glEnableVertexAttribArray ( 2 );

	glBindVertexArray ( NULL );
	allRenderers.push_back ( this );
}

dap::Sprite::~Sprite () {
	glDeleteVertexArrays ( 1, &vertexArrayObject );
	glDeleteBuffers ( 1, &vertexBufferObject );
	glDeleteBuffers ( 1, &elementBufferObject );
	allRenderers.erase ( std::remove ( allRenderers.begin (), allRenderers.end (), this ), allRenderers.end () );
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

unsigned int dap::Sprite::GetTriangleCount () {
	return sizeof ( TRIANGLES ) / sizeof ( TRIANGLES[0] );
}

unsigned int dap::Sprite::GetRenderCount () const {
	return sizeof ( TRIANGLES ) / sizeof ( TRIANGLES[0] ) * 3;
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
	if ( !texture )
		return false;
	texture->Use ();
	glBindVertexArray ( vertexArrayObject );
	return true;
}

void dap::Sprite::Deserialize ( const nlohmann::json* json ) {
	SetLayer ( json->at ( "layer" ) );
	SetTexture ( dap::Rendering::GetTexture ( json->at ( "texture" ) ) );
}

const nlohmann::json dap::Sprite::Serialize () {
	nlohmann::json json = {
		{ "type", typeid( *this ).name () },
		{ "layer", layer },
		{ "texture", *texture->GetName () }
	};
	return json;
}
