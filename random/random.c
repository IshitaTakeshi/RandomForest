#include <stdint.h>
#include <time.h>


#include "random.h"

uint64_t x = 0;

void init_random(void) {
    x = (int)time(NULL);
}

uint64_t next() {
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	x = x * 2685821657736338717LL;
    return x;
}

int random_int(int high) {
    uint64_t r = next();
    return r%high;
}
