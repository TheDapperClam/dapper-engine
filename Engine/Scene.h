#pragma once

#include <map>
#include <string>

namespace dap {
	class GameObject;

	class Scene {
	private:
		static dap::Scene* CurrentScene;

	public:
		static dap::Scene* const GetCurrent ();
		static dap::Scene* const Load ( const std::string fileName );
		static void UnloadCurrent ();

	private:
		std::string name;
		std::string path;
		std::map<std::string, dap::GameObject*> allGameObjects;
		bool eraseOnDelete = true;

	private:
		Scene ( const std::string fileName );

	public:
		~Scene ();

	public:
		void DeleteGameObject ( dap::GameObject* object );
		std::map<std::string, dap::GameObject*>* const GetAllGameObjects ();
		dap::GameObject* GetGameObject ( std::string name );
		std::string GetName ();
		std::string GetPath ();
		void Save ( const std::string fileName );
		dap::GameObject* SpawnGameObject ( std::string name );
	};
}
