#include "prng_c_interface.h"
#include "../src/prng.hpp"

struct PRNG_Handle {
    PRNG* prng;
    bool owns_prng;  // false if existing_prng is provided
};

extern "C" {

PRNG_Handle_t PRNG_create_from_existing(void* existing_prng) {
    if (!existing_prng) return nullptr;
    
    PRNG_Handle_t handle = new PRNG_Handle;
    handle->prng = static_cast<PRNG*>(existing_prng);
    handle->owns_prng = false;
    return handle;
}

void PRNG_destroy(PRNG_Handle_t handle) {
    if (handle) {
        // Delete PRNG only if we created it
        if (handle->owns_prng && handle->prng) {
            delete handle->prng;
        }
        delete handle;
    }
}

void PRNG_generate_num(PRNG_Handle_t handle, mpz_t result) {
    if (handle && handle->prng) {
        mpz_class num = handle->prng->generate_num();
        mpz_set(result, num.get_mpz_t());
    }
}

}
