#pragma once

#include <String>
#include <unordered_map>

namespace dap {
	class Collision {
	private:
		static std::unordered_map<std::string, int> Layers;

	public:
		static const std::unordered_map<std::string, int>* GetAllLayers ();
		static int GetLayer ( std::string key );
		static void Process ();
		static void SetLayer ( std::string key, int value );
	};
}
