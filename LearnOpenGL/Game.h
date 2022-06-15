#pragma once

namespace dap {
	class Game {
	public:
		~Game ();
		void Close ();
		const bool ShouldClose ();
		virtual void End () = 0;
		virtual void Input () = 0;
		virtual void Start () = 0;
		virtual void Tick ( const float time, const float delta ) = 0;
	private:
		bool shouldClose = false;
	};
}
