#pragma once

#include <Component.h>
#include <nlohmann/json.hpp>

namespace dap {
	class SoundClip;

	class AudioSource : public dap::Component {
	private:
		dap::SoundClip* sound = NULL;
		bool loop = false;
		bool playOnStart = true;

	public:
		void Play ();
		void SetLoop ( bool shouldLoop );
		void SetPlayOnStart ( bool shouldPlay );
		void SetSound ( dap::SoundClip* sound );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual void ImGuiWidgetsBegin () override;
		virtual nlohmann::json Serialize () override;
		virtual void Start () override;
	};
}
