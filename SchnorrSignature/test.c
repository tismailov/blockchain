#include <stdio.h>
#include <time.h>
#include <gmp.h>
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
    
    printf("\n=== Key Generation ===\n");
    printf("Prime p: ");
    mpz_out_str(stdout, 16, p);
    printf("\nGenerator g: ");
    mpz_out_str(stdout, 16, g);
    printf("\nPrivate key x: ");
    mpz_out_str(stdout, 16, x);
    printf("\nPublic key P: ");
    mpz_out_str(stdout, 16, P);
    printf("\n\n");

    // Message to sign
    const char *message = "This is a test message for Schnorr signature.";
    printf("=== Signing Process ===\n");
    printf("Message to sign: %s\n", message);

    // Sign the message
    struct stribog_ctx_t ctx;
    sign_message(message, r, s, x, g, p, state, &ctx);
    
    printf("Signature components:\n");
    printf("r: ");
    mpz_out_str(stdout, 16, r);
    printf("\ns: ");
    mpz_out_str(stdout, 16, s);
    printf("\n\n");

    // Verify the signature
    printf("=== Verification Process ===\n");
    if (verify_signature(message, r, s, P, g, p, &ctx)) {
        printf("Signature verification SUCCESSFUL\n");
        printf("All components match and the signature is valid!\n");
    } else {
        printf("Signature verification FAILED\n");
        printf("The signature is invalid or has been tampered with.\n");
    }

    gmp_randclear(state);
    mpz_clears(p, g, x, P, r, s, NULL);

    return 0;
}