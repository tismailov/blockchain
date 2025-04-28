#include "signature.h"

int main() {
    mpz_t p, g, x, P, r, s;
    mpz_inits(p, g, x, P, r, s, NULL);

    unsigned long bits = 256;

    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Set the random seed based on the current time
    unsigned long seed = (unsigned long)time(NULL);
    gmp_randseed_ui(state, seed);

    // Generate a random prime number p
    generate_random_prime(p, bits, state);

    // Generate a random generator g of the group Z_p^*
    generate_random_generator(g, p, state);

    // Generate keys
    generate_keys(x, P, g, p, state);

    // Message to sign
    const char *message = "This is a test message for Schnorr signature.";

    // Sign the message
    struct stribog_ctx_t ctx;
    sign_message(message, r, s, x, g, p, state, &ctx);

    // Verify the signature
    if (verify_signature(message, r, s, P, g, p, &ctx)) {
        printf("Signature is valid.\n");
    } else {
        printf("Signature is invalid.\n");
    }

    gmp_randclear(state);
    mpz_clears(p, g, x, P, r, s, NULL);

    return 0;
}