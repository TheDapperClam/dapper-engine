#pragma once

namespace dap {
	class MathF {
	public:
		static float Clamp ( float value, float min, float max );
		static float Lerp ( float from, float to, float t );
	};
}
