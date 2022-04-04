#pragma once

#include <vector>
#include <glad/glad.h>
#include "Component.h"
#include "Texture.h"
#include "Vertex.h"

namespace dap {
	class Sprite : public Component {
	public:
		Sprite ();
		virtual ~Sprite () override;
		const static std::vector<dap::Sprite*>* GetAllSprites ();
		const int* GetLayer () const;
		const dap::Texture* GetTexture ();
		void SetLayer ( const int newLayer );
		void SetTexture ( dap::Texture* newTexture );
		unsigned int Use () const;
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;
	private:
		JSON_PRIVATE;
		const static dap::Vertex VERTICES[4];
		const static unsigned int TRIANGLES[6];
		static std::vector<dap::Sprite*> allSprites;
		int layer = 0;
		GLuint vertexArrayObject = 0;
		GLuint vertexBufferObject = 0;
		GLuint elementBufferObject = 0;
		dap::Texture* texture = NULL;
	};
}
