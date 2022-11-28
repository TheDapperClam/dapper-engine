#pragma once

#include <soloud.h>
#include <soloud_wav.h>

namespace dap {
	class Audio {
	public:

	private:
		static bool Running;
		static SoLoud::Soloud SoLoudEngine;

	public:
		static void PlaySound ( SoLoud::Wav* sound, bool looping );
		static void Start ();
		static void Stop ();
		static void StopAllSounds ();
	};
}
