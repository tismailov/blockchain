#ifndef _GENERATORS_H
#define _GENERATORS_H

#include <gmp.h>

/**
 * Generates a random prime number of a specified bit length.
 *
 * @param q The output variable where the generated prime number will be stored.
 * @param bits The bit length of the prime number to be generated.
 * @param state The GMP random state used for generating random numbers.
 */
void generate_random_prime(mpz_t q, unsigned long bits, gmp_randstate_t state);

/**
 * Generates a random generator of the group Z_p^*.
 *
 * @param g The output variable where the generated generator will be stored.
 * @param p The prime number defining the group Z_p^*.
 * @param state The GMP random state used for generating random numbers.
 */
void generate_random_generator(mpz_t g, mpz_t p, gmp_randstate_t state);

/**
 * Generates a secret key and the corresponding public key.
 *
 * @param x The output variable where the secret key will be stored.
 * @param P The output variable where the public key will be stored.
 * @param g The generator of the group Z_p^*.
 * @param p The prime number defining the group Z_p^*.
 * @param state The GMP random state used for generating random numbers.
 */
void generate_keys(mpz_t x, mpz_t P, mpz_t g, mpz_t p, gmp_randstate_t state);

#endif