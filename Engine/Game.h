#pragma once

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
		void Run ( std::string name, unsigned int tickRate, std::string startingScene );
		bool ShouldStop ();
		void Stop ();

	public:
		virtual void End () = 0;
		virtual void Input () = 0;
		virtual void Start () = 0;
		virtual void Tick () = 0;
	};
}
