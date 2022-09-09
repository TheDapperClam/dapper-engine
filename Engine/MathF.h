#pragma once

namespace dap {
	class MathF {
	public:
		static const double PI;

	public:
		static float Deg2Rad ( float angle );
		static float Clamp ( float value, float min, float max );
		static float Lerp ( float from, float to, float t );
		static float Rad2Deg ( float angle );
	};
}
