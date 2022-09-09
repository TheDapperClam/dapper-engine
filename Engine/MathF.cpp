#include "MathF.h"

#include <iostream>

const double dap::MathF::PI = 3.14159265358979323846;

float dap::MathF::Deg2Rad ( float angle ) {
	return PI / 180.0f * angle;
}

float dap::MathF::Clamp ( float value, float min, float max ) {
	return std::max ( std::min ( value, max ), min );
}

float dap::MathF::Lerp ( float from, float to, float t ) {
	return from + ( to - from ) * t;
}

float dap::MathF::Rad2Deg ( float angle ) {
	return 180.0f / PI * angle;
}
