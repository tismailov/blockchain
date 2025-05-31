#ifndef PRNG_C_INTERFACE_H
#define PRNG_C_INTERFACE_H

#include <gmp.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wrapper for PRNG pointer
typedef struct PRNG_Handle* PRNG_Handle_t;

// Creates C-handle for existing PRNG instance
PRNG_Handle_t PRNG_create_from_existing(void* existing_prng);

// Destroys handle (but does not delete the PRNG)
void PRNG_destroy(PRNG_Handle_t handle);

// Generates random number and stores it in result
void PRNG_generate_num(PRNG_Handle_t handle, mpz_t result);

#ifdef __cplusplus
}
#endif

#endif // PRNG_C_INTERFACE_H
