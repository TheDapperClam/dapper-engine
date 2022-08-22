#include "Config.h"

std::map<std::string, std::any> dap::Config::properties = {
	dap::ConfigProp ( "bFullscreen", true ),
	dap::ConfigProp ( "iWindowW", 800 ),
	dap::ConfigProp ( "iWindowH", 600 ),
	dap::ConfigProp ( "bVSync", true ),
	dap::ConfigProp ( "bShowColliders", false )
};
