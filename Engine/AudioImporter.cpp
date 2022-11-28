#include "AudioImporter.h"

#include <SoundClip.h>

void dap::AudioImporter::ClearImports () {
	dap::SoundClip::ClearAllSoundClips ();
}

std::vector<std::string> dap::AudioImporter::GetExtensions () {
	return { ".wav", ".ogg" };
}

void dap::AudioImporter::Import ( std::string key, std::string path ) {
	dap::SoundClip* sc = new dap::SoundClip ( key, path );
}
