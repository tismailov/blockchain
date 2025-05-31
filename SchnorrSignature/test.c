#include <stdio.h>
#include <time.h>
#include <gmp.h>
#include "signature.h"
#include "prng_c_interface.h"

// Forward declaration of PRNG class
#ifdef __cplusplus
class PRNG;
#else
typedef struct PRNG PRNG;
#endif

// Function to create PRNG instance (implemented in C++)
#ifdef __cplusplus
extern "C"
#endif
PRNG* create_prng_instance(void);

int main() {
    mpz_t p, g, x, P, r, s;
    mpz_inits(p, g, x, P, r, s, NULL);

    unsigned long bits = 256;

    // Create PRNG instance
    PRNG* prng_instance = create_prng_instance();
    PRNG_Handle_t prng_handle = PRNG_create_from_existing(prng_instance);

    // Generate a random prime number p
    generate_random_prime(p, bits, prng_handle);

    // Generate a random generator g of the group Z_p^*
    generate_random_generator(g, p, prng_handle);

    // Generate keys
    generate_keys(x, P, g, p, prng_handle);
    
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
    sign_message(message, r, s, x, g, p, prng_handle, &ctx);
    
    printf("Signature components:\n");
    printf("r: ");
    mpz_out_str(stdout, 16, r);
    printf("\ns: ");
    mpz_out_str(stdout, 16, s);
    printf("\n\n");

    // Verify the signature
    printf("=== Verification Process ===\n");
    if (verify_signature(message, r, s, P, g, p, &ctx)) {
        printf("Signature is valid!\n");
    } else {
        printf("Signature is invalid!\n");
    }

    // Cleanup
    PRNG_destroy(prng_handle);
    mpz_clears(p, g, x, P, r, s, NULL);
    
    return 0;
}