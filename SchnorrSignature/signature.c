#include "signature.h"

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

/**
 * @brief Sign a message using the Schnorr signature scheme
 * @param message The message to be signed
 * @param r Output parameter for the first part of the signature (r = g^k mod p)
 * @param s Output parameter for the second part of the signature (s = k + xe mod (p-1))
 * @param x The signer's private key
 * @param g The generator of the multiplicative group
 * @param p The prime modulus defining the field
 * @param state Random state for generating k
 * @param ctx Unused parameter for hash context compatibility
 * @note The function uses HASH256 for computing e = H(r || message)
 * @note The signature (r,s) satisfies the equation g^s = r * P^e mod p
 */
void sign_message(const char *message, mpz_t r, mpz_t s, mpz_t x, mpz_t g, mpz_t p, gmp_randstate_t state, struct stribog_ctx_t *ctx UNUSED) {
    mpz_t k, e, p_minus_1;
    mpz_inits(k, e, p_minus_1, NULL);

    // Compute p-1
    mpz_sub_ui(p_minus_1, p, 1);

    // Generate a random k
    mpz_urandomm(k, state, p_minus_1);
    mpz_powm(r, g, k, p);

    // Compute e = H(r || message)
    struct stribog_ctx_t ctx_r;
    init(&ctx_r, HASH256);
    unsigned char r_bytes[32];
    size_t count;
    mpz_export(r_bytes, &count, 1, 1, 0, 0, r);
    stribog(&ctx_r, r_bytes, count);
    stribog(&ctx_r, (u8*)message, strlen(message));
    mpz_import(e, 1, 1, sizeof(ctx_r.h[0]), 0, 0, ctx_r.h);

    // Compute s = (k + xe) mod (p-1)
    mpz_mul(s, x, e);
    mpz_add(s, k, s);
    mpz_mod(s, s, p_minus_1);

    mpz_clears(k, e, p_minus_1, NULL);
}

/**
 * @brief Verify a Schnorr signature
 * @param message The message that was signed
 * @param r First part of the signature (r = g^k mod p)
 * @param s Second part of the signature (s = k + xe mod (p-1))
 * @param P The signer's public key
 * @param g The generator of the multiplicative group
 * @param p The prime modulus defining the field
 * @param ctx Unused parameter for hash context compatibility
 * @return 1 if signature is valid, 0 otherwise
 * @note The function verifies if g^s = r * P^e mod p
 * @note Uses HASH256 for computing e = H(r || message)
 */
int verify_signature(const char *message, mpz_t r, mpz_t s, mpz_t P, mpz_t g, mpz_t p, struct stribog_ctx_t *ctx UNUSED) {
    mpz_t e, v;
    mpz_inits(e, v, NULL);

    // Compute e = H(r || message)
    struct stribog_ctx_t ctx_r;
    init(&ctx_r, HASH256);
    unsigned char r_bytes[32];
    size_t count;
    mpz_export(r_bytes, &count, 1, 1, 0, 0, r);
    stribog(&ctx_r, r_bytes, count);
    stribog(&ctx_r, (u8*)message, strlen(message));
    mpz_import(e, 1, 1, sizeof(ctx_r.h[0]), 0, 0, ctx_r.h);

    // Verify g^s = r * P^e mod p
    mpz_powm(v, g, s, p);
    
    mpz_t temp;
    mpz_init(temp);
    mpz_powm(temp, P, e, p);
    mpz_mul(temp, r, temp);
    mpz_mod(temp, temp, p);

    int result = (mpz_cmp(v, temp) == 0);

    mpz_clears(e, v, temp, NULL);
    return result;
}