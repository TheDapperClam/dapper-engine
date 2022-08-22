#pragma once

#include <string>

namespace dap {
	class Game {
	private:
		bool shouldClose = false;

	public:
		~Game ();

	public:
		void Close ();
		bool ShouldClose ();
		void Run ( std::string name, unsigned int tickRate );

	public:
		virtual void End () = 0;
		virtual void Input () = 0;
		virtual void Start () = 0;
		virtual void Tick () = 0;
	};
}
