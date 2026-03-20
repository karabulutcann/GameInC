#include "core/types.h"
#include "pcg_basic.h"
#include <math.h>
#include <time.h>

pcg32_random_t core_randomSeed(u64 initialState, u64 initialSequence){
    pcg32_random_t temp;
    pcg32_srandom_r(&temp, initialState, initialSequence);
    return temp;
}

//Needs rng_state to run and passing same state in multiple thread is unsafe so use new rng_state for every thread
i32 core_randomInt(pcg32_random_t* rng_state,i32 min, i32 max){
    return min + pcg32_boundedrand_r(rng_state, max - min + 1);
}

f32 core_randomFloat(pcg32_random_t* rng_state){
    return ldexp(pcg32_random_r(rng_state), -32);
}
