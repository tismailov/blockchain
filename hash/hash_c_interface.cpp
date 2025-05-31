#include "hash_c_interface.h"
#include "hash_conventer.hpp"

extern "C" {

void stribog_ctx_to_mpz_256_c(const stribog_ctx_t* ctx, mpz_t result) {
    mpz_class temp = stribog_ctx_to_mpz_256(*ctx);
    mpz_set(result, temp.get_mpz_t());
}

}
