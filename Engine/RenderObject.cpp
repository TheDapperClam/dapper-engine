#include "RenderObject.h"

#include "imgui.h"
#include "Shader.h"
#include "Vertex.h"

std::map<int, std::vector<dap::RenderObject*>> dap::RenderObject::AllRenderers = {};

dap::RenderObject::RenderObject ( bool processGameFunctions ) : dap::Component::Component ( processGameFunctions ) {
	SetLayer ( layer );
}

dap::RenderObject::~RenderObject () {
	glDeleteVertexArrays ( 1, &vertexArrayObject );
	glDeleteBuffers ( 1, &vertexBufferObject );
	glDeleteBuffers ( 1, &elementBufferObject );
	std::vector<dap::RenderObject*>* list = &AllRenderers[layer];
	list->erase ( std::remove ( list->begin (), list->end (), this ), list->end () );
	if ( list->size () <= 0 )
		AllRenderers.erase ( layer );
}

void dap::RenderObject::BuildGLObject () {
	if ( !vertexArrayObject )
		glGenVertexArrays ( 1, &vertexArrayObject );
	glBindVertexArray ( vertexArrayObject );

	if ( !vertexBufferObject )
		glGenBuffers ( 1, &vertexBufferObject );
	if ( vertices.size () > 0 ) {
		glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferObject );
		glBufferData ( GL_ARRAY_BUFFER, vertices.size () * sizeof ( Vertex ), &vertices[0], GL_DYNAMIC_DRAW );
	}

	if ( elements.size () > 0 ) {
		if ( !elementBufferObject )
			glGenBuffers ( 1, &elementBufferObject );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, elementBufferObject );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, elements.size () * sizeof ( unsigned int ), &elements[0], GL_DYNAMIC_DRAW );
	}

	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)0 );
	glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)( 3 * sizeof ( float ) ) );
	glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( float ) * 8, (GLvoid*)( 6 * sizeof ( float ) ) );
	glEnableVertexAttribArray ( 0 );
	glEnableVertexAttribArray ( 1 );
	glEnableVertexAttribArray ( 2 );

	glBindVertexArray ( NULL );
	glBindBuffer ( GL_ARRAY_BUFFER, NULL );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, NULL );
}

void dap::RenderObject::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );
	if ( json->contains ( "layer" ) )
		SetLayer ( json->at ( "layer" ) );
	if ( json->contains ( "shader" ) )
		SetShader ( json->at ( "shader" ) );
}

const std::map<int, std::vector<dap::RenderObject*>>* dap::RenderObject::GetAllRenderers () {
	return &AllRenderers;
}

GLuint dap::RenderObject::GetDrawMode () {
	return glDrawMode;
}

unsigned int dap::RenderObject::GetElementsSize () {
	return elements.size ();
}

int dap::RenderObject::GetLayer () {
	return layer;
}

dap::Shader* const dap::RenderObject::GetShader () {
	return shader;
}

void dap::RenderObject::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();
	if ( ImGui::BeginCombo ( "Shader", shader ? shader->GetName ().c_str () : "" ) ) {
		for ( const auto& shaderPair : *dap::Shader::GetAllShaders () ) {
			bool isSelected = shaderPair.second == shader;
			if ( ImGui::Selectable ( shaderPair.second->GetName ().c_str (), isSelected ) )
				SetShader ( shaderPair.first );
		}
		ImGui::EndCombo ();
	}
	if ( ImGui::InputInt ( "Draw Layer", &layer ) )
		SetLayer ( layer );
}

nlohmann::json dap::RenderObject::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["layer"] = layer;
	json["shader"] = shader->GetName ();
	return json;
}

void dap::RenderObject::SetLayer ( int newLayer ) {
	const int OLD_LAYER = layer;
	layer = newLayer;
	if ( AllRenderers.count ( OLD_LAYER ) ) {
		std::vector<dap::RenderObject*>* previousList = &AllRenderers[OLD_LAYER];
		previousList->erase ( std::remove ( previousList->begin (), previousList->end (), this ), previousList->end () );
		if ( previousList->size () <= 0 )
			AllRenderers.erase ( OLD_LAYER );
	}
	if ( !AllRenderers.count ( layer ) )
		AllRenderers.insert ( std::pair<int, std::vector<dap::RenderObject*>> ( layer, {} ) );
	AllRenderers[layer].push_back ( this );
}

void dap::RenderObject::SetShader ( std::string key ) {
	shader = dap::Shader::GetShader ( key );
}

bool dap::RenderObject::Use () const {
	if ( isEnabled && vertexArrayObject && shader && vertices.size () > 0 && elements.size () > 0 ) {
		shader->Use ();
		glBindVertexArray ( vertexArrayObject );
		return true;
	} else {
		glBindVertexArray ( NULL );
		return false;
	}
}
