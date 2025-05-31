#include "../src/prng.hpp"

extern "C" PRNG* create_prng_instance() {
    return new PRNG();
}
