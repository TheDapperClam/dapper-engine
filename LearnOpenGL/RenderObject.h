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
		GLuint vertexArrayObject = 0;
		GLuint vertexBufferObject = 0;
		GLuint elementBufferObject = 0;
		std::vector<dap::Vertex> vertices = {};
		std::vector<unsigned int> elements = {};
		bool objectInRenderList = false;
		bool visible = true;

	public:
		virtual GLuint GetDrawMode () const = 0;
		virtual unsigned int GetRenderCount () const = 0;
		bool IsVisible () const;
		virtual bool Use () const;

	protected:
		virtual void BuildElements () = 0;
		virtual void BuildVertices () = 0;
		void BuildRO ();
	};
}
