#pragma once

#include "Scene.h"
#include <string>
#include <vector>

namespace dap {
	class Component;

	class Game {
	private:
		static const std::vector<dap::Component*>* ALL_COMPONENTS;

	private:
		static dap::Game* Current;

	private:
		bool running = false;
		std::string startingScene = "";
		unsigned int tickRate = 1;

	public:
		static dap::Game* const GetCurrent ();

	public:
		~Game ();

	public:
		std::string GetStartingScene ();
		unsigned int GetTickRate ();
		bool IsRunning ();
		void Run ( std::string name, std::string startingScene, unsigned int tickRate = 60 );
		void Stop ();

	public:
		void BaseEnd ();
		void BaseInput ();
		void BaseRegisterFactories ();
		void BaseStart ();
		void BaseTick ();

	protected:
		virtual void End () {};
		virtual void Input () {};
		virtual void RegisterFactories () {};
		virtual void Start () {};
		virtual void Tick () {};
	};
}
