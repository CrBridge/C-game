#include "rng.h"

static inline u64 rotl(const u64 x, i32 k) {
	return (x << k) | (x >> (64 - k));
}

static u64 state[4];

static u64 next(void) {
	const u64 result = rotl(state[1] * 5, 7) * 9;

	const u64 t = state[1] << 17;

	state[2] ^= state[0];
	state[3] ^= state[1];
	state[1] ^= state[2];
	state[0] ^= state[3];

	state[2] ^= t;

	state[3] = rotl(state[3], 45);

	return result;
}

static void jump(void) {
	static const u64 JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	u64 s0 = 0;
	u64 s1 = 0;
	u64 s2 = 0;
	u64 s3 = 0;

	for (int i = 0; i < sizeof JUMP / sizeof *JUMP; i++) 
		for (int b = 0; b < 64; b++) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= state[0];
				s1 ^= state[1];
				s2 ^= state[2];
				s3 ^= state[3];
			}
			next();
		}
	
	state[0] = s0;
	state[1] = s1;
	state[2] = s2;
	state[3] = s3;
}

static void long_jump(void) {
	static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

	u64 s0 = 0;
	u64 s1 = 0;
	u64 s2 = 0;
	u64 s3 = 0;

	for (int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
		for (int b = 0; b < 64; b++) {
			if (LONG_JUMP[i] & UINT64_C(1) << b) {
				s0 ^= state[0];
				s1 ^= state[1];
				s2 ^= state[2];
				s3 ^= state[3];
			}
			next();
		}

	state[0] = s0;
	state[1] = s1;
	state[2] = s2;
	state[3] = s3;
}

f64 rng_next_double(f64 min, f64 max) {
	f64 base = (next() >> 11) * (1.0 / (1ULL << 53));

	return min + (max - min) * base;
}

static u64 split_mix64(u64* seed) {
	u64 z = (*seed += 0x9E3779B97F4A7C15);
	z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EB;
	return z ^ (z >> 31);
}

void rng_set_seed(u64 seed) {
	state[0] = split_mix64(&seed);
	state[1] = split_mix64(&seed);
	state[2] = split_mix64(&seed);
	state[3] = split_mix64(&seed);

	for (int i = 0; i < 25; i++) next();
}