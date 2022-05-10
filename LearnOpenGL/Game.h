#pragma once

#include "Event.h"

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
	protected:
		friend class Rendering;
		dap::Event<const float, const float> tickEvent;
	private:
		bool shouldClose = false;
	};
}
