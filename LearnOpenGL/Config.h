#pragma once

#include <any>
#include "mini/ini.h"
#include <string>

namespace dap {
	class Config {
	public:
		static const std::string PATH_DEFAULT;

	private:
		static mINI::INIStructure properties;

	public:
		static std::string GetProperty ( std::string section, std::string key );
		static void Load ( std::string path );
		static void Save ( std::string path );

	private:
		static void Initialize ();
	};
}
