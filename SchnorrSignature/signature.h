#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <gmp.h>
#include "../hash/stribog.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "generators.h"
#include "prng_c_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Signs a message using the Schnorr signature scheme.
 *
 * @param message The message to be signed.
 * @param r The output variable where the signature component r will be stored.
 * @param s The output variable where the signature component s will be stored.
 * @param x The secret key.
 * @param g The generator of the group Z_p^*.
 * @param p The prime number defining the group Z_p^*.
 * @param prng The PRNG handle used for generating random numbers.
 * @param ctx The Streebog context used for hashing.
 */
void sign_message(const char *message, mpz_t r, mpz_t s, mpz_t x, mpz_t g, mpz_t p, PRNG_Handle_t prng, struct stribog_ctx_t *ctx);

/**
 * Verifies a Schnorr signature.
 *
 * @param message The message that was signed.
 * @param r The signature component r.
 * @param s The signature component s.
 * @param P The public key.
 * @param g The generator of the group Z_p^*.
 * @param p The prime number defining the group Z_p^*.
 * @param ctx The Streebog context used for hashing.
 * @return 1 if the signature is valid, 0 otherwise.
 */
int verify_signature(const char *message, mpz_t r, mpz_t s, mpz_t P, mpz_t g, mpz_t p, struct stribog_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // SIGNATURE_H