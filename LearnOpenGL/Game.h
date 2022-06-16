#pragma once

#include <string>

namespace dap {
	class Game {
	public:
		Game ( std::string projectDir );
		~Game ();
		void Close ();
		std::string GetProjectDir ();
		const bool ShouldClose ();

	public:
		virtual void End () = 0;
		virtual void Input () = 0;
		virtual void Start () = 0;
		virtual void Tick ( const float time, const float delta ) = 0;
		
	private:
		bool shouldClose = false;
		std::string projectDir;
	};
}
