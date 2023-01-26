#include "SoundClip.h"

std::unordered_map<std::string, dap::SoundClip*> dap::SoundClip::AllSoundClips = {};
bool dap::SoundClip::ClearingAllClips = false;

dap::SoundClip::SoundClip ( std::string name, std::string path ) {
	this->name = name;
	sound = new SoLoud::Wav ();
	sound->load ( path.c_str () );
	AllSoundClips.insert ( std::pair<std::string, dap::SoundClip*> ( name, this ) );
}

dap::SoundClip::~SoundClip () {
	delete sound;
	if ( !ClearingAllClips )
		AllSoundClips.erase ( name );
}

void dap::SoundClip::ClearAllSoundClips () {
	ClearingAllClips = true;
	for ( const auto& SOUND_PAIR : AllSoundClips )
		delete SOUND_PAIR.second;
	AllSoundClips.clear ();
	ClearingAllClips = false;
}

const std::unordered_map<std::string, dap::SoundClip*>* dap::SoundClip::GetAllSoundClips () {
	return &AllSoundClips;
}

std::string dap::SoundClip::GetName () {
	return name;
}

dap::SoundClip* const dap::SoundClip::GetSoundClip ( std::string name ) {
	if ( !AllSoundClips.contains ( name ) )
		return NULL;
	return AllSoundClips[name];
}

SoLoud::Wav* const dap::SoundClip::GetWav () {
	return sound;
}
