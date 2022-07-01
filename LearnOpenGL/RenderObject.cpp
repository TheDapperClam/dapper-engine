#include "RenderObject.h"

std::vector<dap::RenderObject*> dap::RenderObject::allRenderers = {};

void dap::RenderObject::BuildRO () {
	if ( !vertexArrayObject )
		glGenVertexArrays ( 1, &vertexArrayObject );
	glBindVertexArray ( vertexArrayObject );

	if ( !vertexBufferObject )
		glGenBuffers ( 1, &vertexBufferObject );
	glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferObject );
	glBufferData ( GL_ARRAY_BUFFER, vertices.size () * sizeof ( Vertex ), &vertices[0], GL_STATIC_DRAW );

	if ( !elementBufferObject )
		glGenBuffers ( 1, &elementBufferObject );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, elementBufferObject );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, elements.size () * sizeof ( unsigned int ), &elements[0], GL_STATIC_DRAW );

	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)0 );
	glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)( 3 * sizeof ( float ) ) );
	glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)( 6 * sizeof ( float ) ) );
	glEnableVertexAttribArray ( 0 );
	glEnableVertexAttribArray ( 1 );
	glEnableVertexAttribArray ( 2 );

	glBindVertexArray ( NULL );
	if ( !objectInRenderList )
		allRenderers.push_back ( this );
	objectInRenderList = true;
}

const std::vector<dap::RenderObject*>* dap::RenderObject::GetAllRenderers () {
	return &allRenderers;
}

bool dap::RenderObject::IsVisible () const {
	return visible;
}

bool dap::RenderObject::Use () const {
	glBindVertexArray ( vertexArrayObject );
	return true;
}
