#include "random.h"

static pcg32_random_t rng0;

uint32_t pcg32_random_r(pcg32_random_t* rng)
{
	uint64_t oldstate = rng->state;
	// Advance internal state
	rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
	// Calculate output function (XSH RR), uses old state for max ILP
	uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
	uint32_t rot = oldstate >> 59u;

	return (xorshifted >> rot) | (xorshifted << ((-(int32_t)rot) & 31));
}

void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq)
{
	rng->state = 0U;
	rng->inc = (initseq << 1u) | 1u;
	pcg32_random_r(rng);
	rng->state += initstate;
	pcg32_random_r(rng);
}

void pcg32_init()
{
	pcg32_srandom_r(&rng0, 42u, 54u);
}

double random_double()
{
	return pcg32_random_r(&rng0) / (PCG_RAND_MAX + 1.0);
}

int random_int()
{
	return pcg32_random_r(&rng0);
}