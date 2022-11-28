#pragma once

#include "soloud.h"
#include "soloud_wav.h"

namespace dap {
	class Audio {
	public:
		static void Start ();
		static void Stop ();

	private:
		static SoLoud::Soloud gSoloud;
	};
}
