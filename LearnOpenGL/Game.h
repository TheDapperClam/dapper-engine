#pragma once

#include <string>

namespace dap {
	class Game {
	private:
		bool shouldClose = false;
		std::string projectDir;

	public:
		Game ( std::string projectDir );
		~Game ();

	public:
		void Close ();
		std::string GetProjectDir ();
		bool ShouldClose ();

	public:
		virtual void End () = 0;
		virtual void Input () = 0;
		virtual void Start () = 0;
		virtual void Tick ( const float time, const float delta ) = 0;
	};
}
