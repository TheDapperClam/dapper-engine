#include "Audio.h"

#include <iostream>
#include <SDL.h>

SoLoud::Soloud dap::Audio::gSoloud;

void dap::Audio::Start () {
	if ( SDL_Init ( SDL_INIT_AUDIO ) < 0 ) {
		std::cout << "[AUDIO] Failed to initialize SDL" << std::endl;
		return;
	}
	std::cout << "[AUDIO] SDL initialized" << std::endl;

	if ( gSoloud.init () != SoLoud::SO_NO_ERROR ) {
		std::cout << "[AUDIO] Failed to initialize SoLoud" << std::endl;
		return;
	}
	std::cout << "[AUDIO] SoLoud initialized" << std::endl;
}

void dap::Audio::Stop () {
	gSoloud.deinit ();
	SDL_Quit ();
}
