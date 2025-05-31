#ifndef HASH_C_INTERFACE_H
#define HASH_C_INTERFACE_H

#include <gmp.h>
#include "stribog.h"

#ifdef __cplusplus
extern "C" {
#endif

// C-compatible version of stribog_ctx_to_mpz_256
void stribog_ctx_to_mpz_256_c(const stribog_ctx_t* ctx, mpz_t result);

#ifdef __cplusplus
}
#endif

#endif // HASH_C_INTERFACE_H
