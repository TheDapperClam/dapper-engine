#include "RenderObject.h"

std::vector<dap::RenderObject*> dap::RenderObject::allRenderers = {};

const std::vector<dap::RenderObject*>* dap::RenderObject::GetAllRenderers () {
	return &allRenderers;
}
