#pragma once

#include <vector>
#include <glad/glad.h>
#include "RenderObject.h"
#include "Texture.h"
#include "Vertex.h"

namespace dap {
	class Sprite : public dap::RenderObject {
	JSON_CLASS

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	private:
		const static dap::Vertex VERTICES[4];
		const static unsigned int TRIANGLES[6];

	private:
		int layer = 0;
		GLuint vertexArrayObject = 0;
		GLuint vertexBufferObject = 0;
		GLuint elementBufferObject = 0;
		dap::Texture* texture = NULL;

	public:
		Sprite ();
		virtual ~Sprite () override;

	public:
		virtual GLuint GetDrawMode () const override;
		const int* GetLayer () const;
		dap::Texture* GetTexture ();
		unsigned int GetTriangleCount ();
		virtual unsigned int GetRenderCount () const override;
		void SetLayer ( const int newLayer );
		void SetTexture ( dap::Texture* newTexture );
		virtual bool Use () const override;
	};
}
