#pragma once

#include <String>
#include <unordered_map>
#include <vector>

namespace dap {
	class Component;
	class RectCollider;

	class Collision {
	private:
		static const std::vector<dap::RectCollider*>* ALL_RECTCOLLIDERS;

	private:
		static std::unordered_map<std::string, int> Layers;

	public:
		static const std::unordered_map<std::string, int>* GetAllLayers ();
		static int GetLayer ( std::string key );
		static void Process ();
		static void SetLayer ( std::string key, int value );
	};
}
