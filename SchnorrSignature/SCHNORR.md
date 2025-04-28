# Schnorr Signature Implementation Guide

## Overview

This implementation provides the Schnorr digital signature scheme integrated with the Streebog hash function. It is designed to be simple to use while maintaining cryptographic security.

## Quick Start

### 1. Initialize System Parameters

```c
// Initialize random state
gmp_randstate_t state;
gmp_randinit_default(state);
gmp_randseed_ui(state, time(NULL));

// Generate prime and generator
mpz_t p, g;
mpz_inits(p, g, NULL);
generate_random_prime(p, 256, state);      // 256-bit prime
generate_random_generator(g, p, state);     // Generator of Z_p^*
```

### 2. Generate Keys

```c
mpz_t x, P;    // x - private key, P - public key
mpz_inits(x, P, NULL);
generate_keys(x, P, g, p, state);
```

### 3. Sign a Message

```c
// Create signature variables
mpz_t r, s;
mpz_inits(r, s, NULL);

// Sign message
const char *message = "Your message";
struct stribog_ctx_t ctx;
sign_message(message, r, s, x, g, p, state, &ctx);
```

### 4. Verify a Signature

```c
if (verify_signature(message, r, s, P, g, p, &ctx)) {
    printf("Valid signature\n");
} else {
    printf("Invalid signature\n");
}
```

## Function Reference

### Key Generation

```c
void generate_random_prime(mpz_t q, unsigned long bits, gmp_randstate_t state);
```
- Generates a random prime number of specified bit length
- Use at least 256 bits for security

```c
void generate_random_generator(mpz_t g, mpz_t p, gmp_randstate_t state);
```
- Generates a random generator of the multiplicative group Z_p^*
- p must be prime

```c
void generate_keys(mpz_t x, mpz_t P, mpz_t g, mpz_t p, gmp_randstate_t state);
```
- Generates private key x and public key P = g^x mod p
- Keep x secret, P can be public

### Signature Operations

```c
void sign_message(const char *message, mpz_t r, mpz_t s, mpz_t x, 
                 mpz_t g, mpz_t p, gmp_randstate_t state, 
                 struct stribog_ctx_t *ctx);
```
- Signs a message using private key x
- Returns signature components (r, s)
- Uses Streebog-256 for hashing

```c
int verify_signature(const char *message, mpz_t r, mpz_t s, mpz_t P,
                    mpz_t g, mpz_t p, struct stribog_ctx_t *ctx);
```
- Verifies a signature using public key P
- Returns 1 if valid, 0 if invalid
- Uses the same hash function as signing

## Memory Management

Always clear GMP variables when done:
```c
mpz_clears(p, g, x, P, r, s, NULL);
gmp_randclear(state);
```

## Error Handling

- All functions assume valid input parameters
- Check return values from verify_signature()
- Handle memory allocation failures
- Verify file operations success

## Security Notes

1. Never reuse the random k value for different signatures
2. Keep private key x secure
3. Use cryptographically secure random number generator
4. Verify input parameters
5. Use sufficient prime size (≥256 bits)

## Integration with Hash Function

The implementation uses Streebog-256 hash function:
- Hash output is automatically converted to a number mod p
- Hash input is r || message (concatenation)
- Hash context is handled internally