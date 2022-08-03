#pragma once

#include "RenderObject.h"
#include <string>

namespace dap {
	class Font;

	class Label : public dap::RenderObject {
	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	public:
		Label ();

	public:
		dap::Font* GetFont ();
		void SetText ( std::string newText );
		void SetFont ( dap::Font* newFont );

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;

	private:
		std::string text = "";
		dap::Font* font = NULL;
	};
}
