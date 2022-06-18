#pragma once

#include <vector>
#include <glad/glad.h>
#include "Component.h"
#include "Texture.h"
#include "Vertex.h"

namespace dap {
	class Sprite : public dap::Component {
	JSON_CLASS

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	private:
		const static dap::Vertex VERTICES[4];
		const static unsigned int TRIANGLES[6];

	public:
		const static std::vector<dap::Sprite*>* GetAllSprites ();

	private:
		static std::vector<dap::Sprite*> allSprites;

	public:
		Sprite ();
		virtual ~Sprite () override;

	public:
		const int* GetLayer () const;
		dap::Texture* GetTexture ();
		unsigned int GetTriangleCount ();
		void SetLayer ( const int newLayer );
		void SetTexture ( dap::Texture* newTexture );
		void Use () const;
		
	private:
		int layer = 0;
		GLuint vertexArrayObject = 0;
		GLuint vertexBufferObject = 0;
		GLuint elementBufferObject = 0;
		dap::Texture* texture = NULL;
	};
}
