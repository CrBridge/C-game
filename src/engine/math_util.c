#include "math_util.h"

f32 degree_to_rad(f32 degrees) {
	return degrees * (M_PI / 180.0f);
}

void float_clamp(f32* f, f32 min, f32 max) {
	if (*f < min) {
		*f = min;
	}
	if (*f > max) {
		*f = max;
	}
}