#include "Sprite.h"

#include <algorithm>
#include "imgui.h"
#include "Resources.h"
#include "Shader.h"
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"
#include "Vertex.h"

dap::Sprite::Sprite () {
	SetShader ( dap::Shader::DEFAULT_SPRITE );
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

void dap::Sprite::ImGuiWidgetsBegin () {
	dap::RenderObject::ImGuiWidgetsBegin ();

	const ImVec2 PREVIEW_SIZE = ImVec2 ( 128, 128 );
	dap::TextureAtlas* atlas = dap::TextureAtlas::GetMainAtlas ();
	if ( ImGui::BeginCombo ( "Texture", texture ? texture->name.c_str () : "" ) ) {
		for ( const auto& subTexturePair : *atlas->GetAllSubTextures () ) {
			bool isSelected = subTexturePair.second == texture;
			if ( ImGui::Selectable ( subTexturePair.first.c_str (), isSelected ) )
				SetTexture ( subTexturePair.first );
		}
		ImGui::EndCombo();
	}
	if ( texture ) {
		const auto texCoords = texture->GetTexCoords ();
		ImGui::Image ( (void*)*atlas->GetTextureObject (), PREVIEW_SIZE, ImVec2 ( texCoords[0].x, texCoords[0].y ), ImVec2 ( texCoords[2].x, texCoords[2].y ) );
	} else
		ImGui::Image ( NULL, PREVIEW_SIZE );
}

void dap::Sprite::RefreshTexCoords () {
	if ( !texture )
		return;
	const glm::vec2* texCoords = texture->GetTexCoords ();
	for ( int i = 0; i < vertices.size (); i++ ) {
		vertices.at ( i ).texCoord = texCoords[i];
	}
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
	dap::RenderObject::Deserialize ( json );
	SetTexture ( json->at ( "texture" ) );
}

nlohmann::json dap::Sprite::Serialize () {
	nlohmann::json json = dap::RenderObject::Serialize ();
	json["texture"] = texture ? texture->name : "";
	return json;
}
