#ifndef RANDOM_H
#define RANDOM_H

// Implementation of a mt19937-like uniform random number generator

#include <stdint.h>

#define PONGRNG_STATEVEC_LEN 624

typedef struct PongRng
{
	uint64_t mt[PONGRNG_STATEVEC_LEN];
	uint32_t idx;
} PongRng_t;

/**
 * @brief Initialise rng
 * 
 * @param seed Rng seed
 * @return PongRng_t Rng object
 */
PongRng_t PongRng_init(uint64_t seed);

/**
 * @brief Generate random number from rng
 * 
 * @param rng Pointer to PongRng_t object
 * @return uint64_t 
 */
uint64_t PongRng_rand(void * rng);


#endif
