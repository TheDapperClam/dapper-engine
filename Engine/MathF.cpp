#include "MathF.h"

#include <iostream>

float dap::MathF::Clamp ( float value, float min, float max ) {
	return std::max ( std::min ( value, max ), min );
}

float dap::MathF::Lerp ( float from, float to, float t ) {
	return from + ( to - from ) * t;
}
