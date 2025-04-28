#include "generators.h"

/**
 * @brief Generate a random prime number of specified bit length
 * @param q Output parameter where the generated prime will be stored
 * @param bits The desired bit length of the prime number
 * @param state GMP random state for number generation
 * @note Uses probabilistic primality testing with 10 rounds
 * @note If the generated number is not prime, finds the next prime
 */
void generate_random_prime(mpz_t q, unsigned long bits, gmp_randstate_t state) {
    mpz_t candidate;
    mpz_init(candidate);
    mpz_urandomb(candidate, state, bits);

    while (!mpz_probab_prime_p(candidate, 10)) {
        mpz_nextprime(candidate, candidate);
    }

    mpz_set(q, candidate);
    mpz_clear(candidate);
}

/**
 * @brief Generate a random generator for the multiplicative group Z_p^*
 * @param g Output parameter where the generator will be stored
 * @param p The prime modulus defining the field
 * @param state GMP random state for number generation
 * @note Generates a random number in range [2, p-1]
 * @note Ensures the generated number is relatively prime to p
 * @note If candidate is not suitable, increments until finding valid generator
 */
void generate_random_generator(mpz_t g, mpz_t p, gmp_randstate_t state) {
    mpz_t candidate;
    mpz_init(candidate);

    mpz_sub_ui(candidate, p, 2);
    mpz_urandomm(candidate, state, candidate);
    mpz_add_ui(candidate, candidate, 2);

    while (!mpz_probab_prime_p(candidate, 10)) {
        mpz_add_ui(candidate, candidate, 1);
        if (mpz_cmp(candidate, p) >= 0) {
            mpz_sub_ui(candidate, p, 2);
        }
    }

    mpz_set(g, candidate);

    mpz_clear(candidate);
}

/**
 * @brief Generate a key pair for the Schnorr signature scheme
 * @param x Output parameter for the private key
 * @param P Output parameter for the public key
 * @param g The generator of the multiplicative group
 * @param p The prime modulus defining the field
 * @param state GMP random state for number generation
 * @note Private key x is randomly chosen from [0, p-1]
 * @note Public key P is computed as P = g^x mod p
 */
void generate_keys(mpz_t x, mpz_t P, mpz_t g, mpz_t p, gmp_randstate_t state) {
    mpz_urandomm(x, state, p);
    mpz_powm(P, g, x, p);
}