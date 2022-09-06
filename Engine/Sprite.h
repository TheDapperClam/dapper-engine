#pragma once

#include <glad/glad.h>
#include "RenderObject.h"
#include <string>
#include <vector>

namespace dap {
	struct TextureAtlasSubTexture;

	class Sprite : public dap::RenderObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;	

	public:
		virtual void ImGuiWidgetsBegin () override;

	private:
		dap::TextureAtlasSubTexture* texture = NULL;

	public:
		Sprite ();

	public:
		virtual void RefreshTexCoords () override;
		void SetTexture ( std::string name );
		virtual bool Use () const override;

	protected:
		virtual void BuildVertices ();
		virtual void BuildElements ();
	};
}
