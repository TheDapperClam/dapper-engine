#include "InputEvent.h"

dap::InputEvent::InputEvent ( int key, int scancode, int action, int mods ) {
	this->key = key;
	this->scancode = scancode;
	this->action = action;
	this->mods = mods;
}
