#pragma once

#include <stdint.h>

#define PCG_RAND_MAX UINT32_MAX

typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;

uint32_t pcg32_random_r(pcg32_random_t* rng);
void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq);
void pcg32_init();
double random_double();
int random_int();

inline double random_double_range(double min, double max) 
{
	return min + (max - min)*random_double();
}

inline int random_int_range(int min, int max)
{
	return min + random_int() % (max - min + 1);
}
