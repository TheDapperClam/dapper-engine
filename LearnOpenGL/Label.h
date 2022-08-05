#pragma once

#include "RenderObject.h"
#include <string>

namespace dap {
	class Font;

	class Label : public dap::RenderObject {
	private:
		static const float VERTEX_SCALE;
		static const char LINE_BREAK;

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual const nlohmann::json Serialize () override;

	public:
		Label ();

	public:
		dap::Font* GetFont ();
		void SetFont ( dap::Font* newFont );
		void SetLineHeight ( float newHeight );
		void SetRightToLeft ( bool enabled );
		void SetText ( std::string newText );
		void SetTextAlign ( float x, float y );

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;

	private:
		std::string text = "";
		dap::Font* font = NULL;
		float labelWidth = 0.0f;
		float labelHeight = 0.0f;
		float lineHeight = 0.75f;
		float textAlignX = 1.0f;
		float textAlignY = 1.0f;
		bool rightToLeft = false;
	};
}
