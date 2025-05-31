#ifndef _STRIBOG_H
#define _STRIBOG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLOCK_SIZE      64
#define BLOCK_BIT_SIZE  (BLOCK_SIZE * 8)

#define OUTPUT_SIZE_512 64
#define OUTPUT_SIZE_256 32

#define HASH256         0
#define HASH512         1

#define HELP_MSG "Usage: %s [OPTIONS] [INPUT]\n"\
                "Options:\n"\
                "  -s <string>  Hash a string\n"\
                "  -f <file>    Hash a file\n"\
                "  -b <size>    Hash size (256 or 512, default: 512)\n"\
                "  -h           Show this help\n"

struct stribog_ctx_t {
    u8 h[BLOCK_SIZE];    // Hash value
    u8 N[BLOCK_SIZE];    // Message length counter
    u8 S[BLOCK_SIZE];    // Checksum
    u8 size;             // Hash size (HASH256 or HASH512)
};

// Internal functions for hash computation
void addmod512(u8 *dst, u8 *src, u8 *add);
void addmod512_u32(u8 *dst, u8 *src, u32 add);
void xor512(u8 *dst, u8 *a, u8 *b);
void S(u8 *vect);
void LP(u8 *vect);
void X(u8 *dst, u8 *a, u8 *b);
void E(u8 *dst, u8 *k, u8 *m);
void g_N(u8 *h, u8 *N, u8 *m);
void g_0(u8 *h, u8 *m);

/**
 * @brief Compute the Streebog hash of a message
 * @param ctx Pointer to the initialized context structure
 * @param message Pointer to the input message
 * @param len Length of the message in bytes
 * @note The result will be stored in ctx->h
 */
void stribog(struct stribog_ctx_t *ctx, u8 *message, u64 len);

/**
 * @brief Initialize a Streebog context for hashing
 * @param ctx Pointer to the context structure to initialize
 * @param size Hash size to use (HASH256 or HASH512)
 * @note HASH256 produces a 256-bit (32-byte) hash
 * @note HASH512 produces a 512-bit (64-byte) hash
 */
void init(struct stribog_ctx_t *ctx, u8 size);

/**
 * @brief Print the computed hash to stdout in hexadecimal format
 * @param ctx Pointer to the context containing the computed hash
 */
void print_hash(struct stribog_ctx_t *ctx);

/**
 * @brief Compute the hash of a file's contents
 * @param ctx Pointer to the initialized context structure
 * @param filename Path to the file to hash
 * @note The result will be written to "hash.txt"
 */
void hash_file(struct stribog_ctx_t *ctx, const char *filename);

/**
 * @brief Process command line arguments for the hash program
 * @param argc Number of arguments
 * @param argv Array of argument strings
 */
void process_input(int argc, char *argv[]);

/**
 * @brief Write the computed hash to a file in hexadecimal format
 * @param ctx Pointer to the context containing the computed hash
 * @param filename Name of the file to write the hash to
 */
void write_hash_to_file(struct stribog_ctx_t *ctx, const char *filename);

/**
 * @brief Hash a block of data and write the result to hash.txt
 * @param ctx Pointer to the Streebog context
 * @param data Pointer to the data block to be hashed
 * @param len Length of the data block in bytes
 */
void hash_data(struct stribog_ctx_t *ctx, const u8 *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif