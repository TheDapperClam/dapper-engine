#pragma once

#include <vector>
#include <glad/glad.h>
#include "RenderObject.h"
#include <string>

namespace dap {
	struct TextureAtlasSubTexture;

	class Sprite : public dap::RenderObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;	

	private:
		int layer = 0;
		dap::TextureAtlasSubTexture* texture = NULL;

	public:
		Sprite ();

	public:
		const int* GetLayer () const;
		virtual void RefreshTexCoords () override;
		void SetLayer ( const int newLayer );
		void SetTexture ( std::string name );
		virtual bool Use () const override;

	protected:
		virtual void BuildVertices ();
		virtual void BuildElements ();
	};
}
