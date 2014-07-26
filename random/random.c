#include <stdint.h>

uint64_t x = 10;

void set_seed(unsigned int seed) {
    x = (uint64_t)seed;
}

uint64_t next() {
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	return x * 2685821657736338717LL;
}
