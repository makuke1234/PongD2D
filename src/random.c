#include "random.h"

#define PONGRNG_F 6364136223846793005ULL

#define PONGRNG_W 64ULL
#define PONGRNG_N 312ULL
#define PONGRNG_M 156ULL
#define PONGRNG_R 31ULL

#define PONGRNG_A 0xB5026F5AA96619E9

#define PONGRNG_U 29ULL
#define PONGRNG_D 0x5555555555555555

#define PONGRNG_S 17ULL
#define PONGRNG_B 0x71D67FFFEDA60000

#define PONGRNG_T 37ULL
#define PONGRNG_C 0xFFF7EEE000000000

#define PONGRNG_L 43ULL


#define PONGRNG_LOWER_MASK ((1ULL << PONGRNG_R) - 1ULL)
#define PONGRNG_UPPER_MASK (~((1ULL << PONGRNG_R) - 1ULL))


PongRng_t PongRng_init(uint64_t seed)
{
	PongRng_t rng = { .mt = { [0] = seed } };
	for (uint64_t i = 1; i < PONGRNG_STATEVEC_LEN; ++i)
	{
		rng.mt[i] = PONGRNG_F * (rng.mt[i-1] ^ (rng.mt[i-1] >> (PONGRNG_W - 2ULL))) + i;
	}
	rng.idx = PONGRNG_STATEVEC_LEN;
	return rng;
}

/**
 * @brief Inline function which "twists" the rng
 * 
 * @param rng Pointer to rng object
 */
static inline void PongRng_twist(PongRng_t * restrict rng)
{
	for (uint64_t i = 0; i < PONGRNG_STATEVEC_LEN; ++i)
	{
		uint64_t x = (rng->mt[i] & PONGRNG_UPPER_MASK) + 
			(rng->mt[(i + 1) % PONGRNG_STATEVEC_LEN] & PONGRNG_LOWER_MASK);
		uint64_t xA = x >> 1ULL;
		if ((x % 2) != 0)
		{
			xA = xA ^ PONGRNG_A;
		}
		rng->mt[i] = rng->mt[(i + PONGRNG_M) % PONGRNG_STATEVEC_LEN] ^ xA;
	}
	rng->idx = 0;
}

uint64_t PongRng_rand(void * rng)
{
	PongRng_t * restrict gen = rng;

	if (gen->idx >= PONGRNG_STATEVEC_LEN)
	{
		PongRng_twist(gen);
	}

	// Algorithm
	uint64_t y = gen->mt[gen->idx];
	y = y ^ ((y >> PONGRNG_U) & PONGRNG_D);
	y = y ^ ((y << PONGRNG_S) & PONGRNG_B);
	y = y ^ ((y << PONGRNG_T) & PONGRNG_C);
	
	y = y ^ (y >> PONGRNG_L);
	++gen->idx;

	return y;
}

double PongRng_rand_norm(void * rng, uint64_t resolution)
{
	return (double)(PongRng_rand(rng) % resolution) / (double)(resolution - 1ULL);
}
float PongRng_rand_normf(void * rng, uint64_t resolution)
{
	return (float)(PongRng_rand(rng) % resolution) / (float)(resolution - 1ULL);
}

