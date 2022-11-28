#include "Audio.h"

#include <iostream>
#include <SDL.h>

bool dap::Audio::Running = false;
SoLoud::Soloud dap::Audio::SoLoudEngine;

void dap::Audio::PlaySound ( SoLoud::Wav* sound, bool looping ) {
	if ( !Running || !sound )
		return;
	SoLoud::handle handle = SoLoudEngine.play ( *sound );
	SoLoudEngine.setLooping ( handle, looping );
}

void dap::Audio::Start () {
	if ( Running )
		return;

	if ( SDL_Init ( SDL_INIT_AUDIO ) < 0 ) {
		std::cout << "[AUDIO] Failed to initialize SDL" << std::endl;
		return;
	}
	std::cout << "[AUDIO] SDL initialized" << std::endl;

	if ( SoLoudEngine.init () != SoLoud::SO_NO_ERROR ) {
		std::cout << "[AUDIO] Failed to initialize SoLoud" << std::endl;
		return;
	}
	Running = true;
	std::cout << "[AUDIO] SoLoud initialized" << std::endl;
}

void dap::Audio::Stop () {
	if ( !Running )
		return;
	Running = false;
	SoLoudEngine.deinit ();
	SDL_Quit ();
}

void dap::Audio::StopAllSounds () {
	if ( !Running )
		return;
	SoLoudEngine.stopAll ();
}
