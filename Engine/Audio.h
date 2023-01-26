#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include <vector>

namespace dap {
	class Audio {
	public:

	private:
		static bool Running;
		static SoLoud::Soloud SoLoudEngine;

	public:
		static void End ();
		static unsigned int GetMaxSounds ();
		static void Initialize ();
		static unsigned int PlaySound ( SoLoud::Wav* sound, bool looping );
		static void StopSound ( unsigned int handle );
		static void StopAllSounds ();
	};
}
