#pragma once

#include "Component.h"
#include "glad/glad.h"
#include <vector>

namespace dap {
	class RenderObject : public dap::Component {
	protected:
		static std::vector<dap::RenderObject*> allRenderers;

	public:
		const static std::vector<dap::RenderObject*>* GetAllRenderers ();
		virtual unsigned int GetRenderCount () const = 0;

	public:
		virtual GLuint GetDrawMode () const = 0;
		virtual bool Use () const = 0;
	};
}
