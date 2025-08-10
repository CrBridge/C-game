#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "types.h"

#define M_PI 3.1415926535f

f32 degree_to_rad(f32 degrees);
void float_clamp(f32* f, f32 min, f32 max);

#endif