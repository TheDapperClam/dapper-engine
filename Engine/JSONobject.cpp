#include "JSONobject.h"

#include "Transform.h"
#include "Camera.h"
#include "Sprite.h"
#include "RectCollider.h"
#include "Label.h"

void dap::JSONobject::ClearFactories () {
	std::cout << "[JSONOBJECT] Clearing factories" << std::endl;
	for ( auto &pair : GetRegisteredFactories () ) {
		std::string key = pair.first;
		delete GetRegisteredFactories ()[key];
		GetRegisteredFactories ()[key] = NULL;
	}
	GetRegisteredFactories ().clear ();
}

void dap::JSONobject::RegisterEngineObjects () {
	RegisterFactory<dap::Transform> ();
	RegisterFactory<dap::Camera> ();
	RegisterFactory<dap::Sprite> ();
	RegisterFactory<dap::RectCollider> ();
	RegisterFactory<dap::Label> ();
}
