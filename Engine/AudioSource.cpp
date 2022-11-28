#include "AudioSource.h"

#include <Audio.h>
#include <imgui.h>

#include <SoundClip.h>

void dap::AudioSource::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );

	if ( json->contains ( "sound" ) )
		SetSound ( dap::SoundClip::GetSoundClip ( json->at ( "sound" ) ) );
	if ( json->contains ( "loop" ) )
		SetLoop ( json->at ( "loop" ) );
	if ( json->contains ( "playOnStart" ) )
		SetPlayOnStart ( json->at ( "playOnStart" ) );
}

void dap::AudioSource::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	if ( ImGui::BeginCombo ( "Sound", sound ? sound->GetName ().c_str () : "" ) ) {
		for ( const auto& SOUND_PAIR : *dap::SoundClip::GetAllSoundClips () ) {
			if ( ImGui::Selectable ( SOUND_PAIR.first.c_str () ) )
				SetSound ( SOUND_PAIR.second );
		}
		ImGui::EndCombo ();
	}

	bool shouldLoop = loop;
	if ( ImGui::Checkbox ( "Loop", &shouldLoop ) )
		SetLoop ( shouldLoop );

	bool shouldPlayOnStart = playOnStart;
	if ( ImGui::Checkbox ( "Play on start", &shouldPlayOnStart ) )
		SetPlayOnStart ( shouldPlayOnStart );
}

void dap::AudioSource::Play () {
	if ( !sound )
		return;
	dap::Audio::PlaySound ( sound->GetSound (), loop );
}

nlohmann::json dap::AudioSource::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["sound"] = sound ? sound->GetName () : "";
	json["loop"] = loop;
	json["playOnStart"] = playOnStart;
	return json;
}

void dap::AudioSource::SetLoop ( bool shouldLoop ) {
	loop = shouldLoop;
}

void dap::AudioSource::SetPlayOnStart ( bool shouldPlayOnStart ) {
	playOnStart = shouldPlayOnStart;
}

void dap::AudioSource::SetSound ( dap::SoundClip* sound ) {
	this->sound = sound;
}

void dap::AudioSource::Start () {
	if ( playOnStart )
		Play ();
}
