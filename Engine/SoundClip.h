#pragma once

#include <soloud_wav.h>
#include <string>
#include <unordered_map>

namespace dap {
	class SoundClip {
	public:
		static std::unordered_map<std::string, dap::SoundClip*> AllSoundClips;
		static bool ClearingAllClips;

	public:
		static void ClearAllSoundClips ();
		const static std::unordered_map<std::string, dap::SoundClip*>* GetAllSoundClips ();
		static dap::SoundClip* const GetSoundClip ( std::string name );

	private:
		std::string name;
		SoLoud::Wav* sound;

	public:
		SoundClip ( std::string name, std::string path );
		~SoundClip ();

	public:
		std::string GetName ();
		SoLoud::Wav* const GetSound ();
	};
}
