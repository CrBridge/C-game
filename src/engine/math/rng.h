#ifndef RNG_H
#define RNG_H

#include "../types.h"

// functions for initialising state, setting seeds etc.
// it wont work by default because it's zeroed

void rng_set_seed(u64 seed);
f64 rng_next_double(f64 min, f64 max);

#endif