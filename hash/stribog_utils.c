#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "stribog.h"

/**
 * @brief Write the computed hash value to a file in hexadecimal format
 * @param ctx Pointer to the Streebog context containing the computed hash
 * @param filename Name of the file to write the hash to
 * @note The function will write the hash as a hexadecimal string
 * @note For HASH256, writes 32 bytes (64 hex chars)
 * @note For HASH512, writes 64 bytes (128 hex chars)
 */
void write_hash_to_file(struct stribog_ctx_t *ctx, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    u8 i;
    u8 output_size = ctx->size ? OUTPUT_SIZE_512 : OUTPUT_SIZE_256;
    for (i = 0; i < output_size; i++) {
        fprintf(file, "%02x", ctx->h[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

/**
 * @brief Hash a block of data and write the result to hash.txt
 * @param ctx Pointer to the Streebog context
 * @param data Pointer to the data block to be hashed
 * @param len Length of the data block in bytes
 * @note The function will automatically initialize the context
 * @note The hash will be written to "hash.txt" in hexadecimal format
 */
void hash_data(struct stribog_ctx_t *ctx, const u8 *data, size_t len) {
    init(ctx, ctx->size ? HASH512 : HASH256);
    stribog(ctx, (u8*)data, len);
    write_hash_to_file(ctx, "hash.txt");
}