#pragma once

#include "Scene.h"
#include <string>

namespace dap {
	class Game {
	private:
		bool shouldStop = false;
		std::string startingScene = "";
		unsigned int tickRate = 1;

	public:
		~Game ();

	public:
		std::string GetStartingScene ();
		unsigned int GetTickRate ();
		void Run ( std::string name, std::string startingScene, unsigned int tickRate = 60 );
		bool ShouldStop ();
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
