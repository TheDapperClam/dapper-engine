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
	elements = {
		0, 1, 2,
		2, 3, 0
	};
}

void dap::Sprite::BuildVertices () {
	const auto TEX_COORDS = texture ? texture->GetTexCoords () : 
									  std::vector<glm::vec2> { glm::vec2 ( 0.0f, 1.0f ), glm::vec2 ( 1.0f, 1.0f ), glm::vec2 ( 1.0f, 0.0f ), glm::vec2 ( 0.0f, 0.0f ) };
	vertices = {
		dap::Vertex ( glm::vec3 ( -0.5f,  0.5f, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), TEX_COORDS[0] ),
		dap::Vertex ( glm::vec3 ( 0.5f,  0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), TEX_COORDS[1] ),
		dap::Vertex ( glm::vec3 ( 0.5f, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), TEX_COORDS[2] ),
		dap::Vertex ( glm::vec3 ( -0.5f, -0.5f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), TEX_COORDS[3] )
	};
}

void dap::Sprite::ImGuiWidgetsBegin () {
	dap::RenderObject::ImGuiWidgetsBegin ();

	const ImVec2 PREVIEW_SIZE = ImVec2 ( 128, 128 );
	dap::TextureAtlas* atlas = dap::TextureAtlas::GetMainAtlas ();
	if ( ImGui::BeginCombo ( "Texture", texture ? texture->Name.c_str () : "" ) ) {
		for ( const auto& subTexturePair : *atlas->GetAllSubTextures () ) {
			bool isSelected = subTexturePair.second == texture;
			if ( ImGui::Selectable ( subTexturePair.first.c_str (), isSelected ) )
				SetTexture ( subTexturePair.first );
		}
		ImGui::EndCombo();
	}
	if ( texture ) {
		const std::vector<glm::vec2> TEX_COORDS = texture->GetTexCoords ();
		ImGui::Image ( (void*)*atlas->GetTextureObject (), PREVIEW_SIZE, ImVec2 ( TEX_COORDS[0].x, TEX_COORDS[0].y ), ImVec2 ( TEX_COORDS[2].x, TEX_COORDS[2].y ) );
	} else
		ImGui::Image ( NULL, PREVIEW_SIZE );
}

void dap::Sprite::SetTexture ( std::string name ) {
	auto* newTexture = dap::TextureAtlas::GetMainAtlas ()->GetSubTexture ( name );
	if ( !newTexture )
		return;
	texture = newTexture;
	BuildVertices ();
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
	json["texture"] = texture ? texture->Name : "";
	return json;
}
