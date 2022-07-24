#pragma once

#include "Component.h"
#include "glad/glad.h"
#include <vector>
#include "Vertex.h"

namespace dap {
	class RenderObject : public dap::Component {
	protected:
		static std::vector<dap::RenderObject*> allRenderers;

	public:
		const static std::vector<dap::RenderObject*>* GetAllRenderers ();

	protected:
		GLuint glDrawMode = GL_TRIANGLES;
		GLuint elementBufferObject = 0;
		GLuint vertexArrayObject = 0;
		GLuint vertexBufferObject = 0;
		std::vector<unsigned int> elements = {};
		std::vector<dap::Vertex> vertices = {};
		bool visible = true;

	public:
		RenderObject ();
		~RenderObject ();

	public:
		GLuint GetDrawMode ();
		unsigned int GetElementsSize ();
		bool IsVisible () const;
		virtual void RefreshTexCoords ();
		virtual bool Use () const;

	protected:
		virtual void BuildElements () = 0;
		virtual void BuildVertices () = 0;
		void BuildGLObject ();
	};
}
