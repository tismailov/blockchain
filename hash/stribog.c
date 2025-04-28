#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "stribog.h"
#include "stribog_data.h"

void addmod512(u8 *dst, u8 *src, u8 *add) {
    u8 i;
    u16 overrun = 0;

    for (i = BLOCK_SIZE; i-- > 0;) {
        overrun = src[i] + add[i] + (overrun >> 8);
        dst[i] = (u8) overrun;
    }
}

void addmod512_u32(u8 *dst, u8 *src, u32 add) {
    u8 i;

    for (i = BLOCK_SIZE; i-- > 0;) {
        add = (u32) src[i] + add;
        dst[i] = add;
        add >>= 8;
    }
}

void xor512(u8 *dst, u8 *a, u8 *b) {
    u8 i;

    for (i = 0; i < BLOCK_SIZE; i++) {
        dst[i] = a[i] ^ b[i];
    }
}

void S(u8 *vect) {
    u8 i;

    for (i = 0; i < BLOCK_SIZE; i++) {
        vect[i] = sbox[vect[i]];
    }
}

void LP(u8 *vect) {
    u8 i, j, k;
    u8 tmp[64];
    memcpy(tmp, vect, 64);
    u64 c;

    for (i = 0; i < 8; i++) {
        c = 0;
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 8; k++) {
                if (tmp[j*8+i] & (0x80 >> k))
                    c ^= A[j*8+k];
            }
        }
        for (j = 0; j < 8; j++) {
            vect[i*8+j] = c >> ((7 - j) * 8);
        }
    }
}

void X(u8 *dst, u8 *a, u8 *b) {
    xor512(dst, a, b);
}

void E(u8 *dst, u8 *k, u8 *m) {
    u8 i;
    u8 K[64];
    memcpy(K, k, BLOCK_SIZE);
    X(dst, K, m);

    for (i = 1; i < 13; i++) {
        S(dst);
        LP(dst);
        X(K, K, C[i-1]);
        S(K);
        LP(K);
        X(dst, K, dst);
    }
}

void g_N(u8 *h, u8 *N, u8 *m) {
    u8 hash[BLOCK_SIZE];
    memcpy(hash, h, BLOCK_SIZE);
    xor512(h, h, N);
    S(h);
    LP(h);
    E(h, h, m);
    xor512(h, h, hash);
    xor512(h, h, m);
}

void g_0(u8 *h, u8 *m) {
    u8 hash[64];
    memcpy(hash, h, BLOCK_SIZE);
    S(h);
    LP(h);
    E(h, h, m);
    xor512(h, h, hash);
    xor512(h, h, m);
}

void stribog(struct stribog_ctx_t *ctx, u8 *message, u64 len) {
    u8 m[BLOCK_SIZE];
    u8 padding;

    while (len >= BLOCK_SIZE) {
        memcpy(m, message + len - BLOCK_SIZE, BLOCK_SIZE);
        g_N(ctx->h, ctx->N, m);
        len -= BLOCK_SIZE;
        addmod512_u32(ctx->N, ctx->N, BLOCK_BIT_SIZE);
        addmod512(ctx->S, ctx->S, m);
    }

    padding = BLOCK_SIZE - len;

    if (padding) {
        memset(m, 0x00, padding - 1);
        memset(m + padding - 1, 0x01, 1);
        memcpy(m + padding, message, len);
    }

    g_N(ctx->h, ctx->N, m);
    addmod512_u32(ctx->N, ctx->N, len*8);
    addmod512(ctx->S, ctx->S, m);

    g_0(ctx->h, ctx->N);
    g_0(ctx->h, ctx->S);
}

void init(struct stribog_ctx_t *ctx, u8 size) {
    memset(ctx->N, 0x00, BLOCK_SIZE);
    memset(ctx->S, 0x00, BLOCK_SIZE);
    ctx->size = !!size;

    if (ctx->size)
        memcpy(ctx->h, iv512, BLOCK_SIZE);
    else
        memcpy(ctx->h, iv256, BLOCK_SIZE);
}

void print_hash(struct stribog_ctx_t *ctx) {
    u8 i;
    u8 output_size = ctx->size ? OUTPUT_SIZE_512 : OUTPUT_SIZE_256;

    for (i = 0; i < output_size; i++)
        printf("%02x", ctx->h[i]);
    putchar('\n');
}

void hash_file(struct stribog_ctx_t *ctx, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file); // Change to size_t
    rewind(file);

    u8 *buffer = (u8*)malloc(file_size);
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, 1, file_size, file) != file_size) {
        fclose(file);
        free(buffer);
        fprintf(stderr, "File read error\n");
        exit(EXIT_FAILURE);
    }

    init(ctx, ctx->size ? HASH512 : HASH256);
    stribog(ctx, buffer, file_size);
    write_hash_to_file(ctx, "hash.txt");

    free(buffer);
    fclose(file);
}

void process_input(int argc, char *argv[]) {
    struct stribog_ctx_t ctx;
    ctx.size = HASH512; // Default to 512-bit hash
    char *input_string = NULL;
    char *filename = NULL;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            if (++i >= argc) {
                fprintf(stderr, "Missing string argument\n");
                exit(EXIT_FAILURE);
            }
            input_string = argv[i];
        } else if (strcmp(argv[i], "-f") == 0) {
            if (++i >= argc) {
                fprintf(stderr, "Missing filename argument\n");
                exit(EXIT_FAILURE);
            }
            filename = argv[i];
        } else if (strcmp(argv[i], "-b") == 0) {
            if (++i >= argc) {
                fprintf(stderr, "Missing hash size argument\n");
                exit(EXIT_FAILURE);
            }
            ctx.size = (atoi(argv[i]) == 256) ? HASH256 : HASH512;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf(HELP_MSG, argv[0]);
            exit(EXIT_SUCCESS);
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    if (input_string && filename) {
        fprintf(stderr, "Cannot process both string and file input\n");
        exit(EXIT_FAILURE);
    }

    if (input_string) {
        hash_data(&ctx, (u8*)input_string, strlen(input_string));
    } else if (filename) {
        hash_file(&ctx, filename);
    } else {
        u8 message1[63] = {
            0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35,
            0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37,
            0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x39,
            0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31,
            0x30, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33,
            0x32, 0x31, 0x30, 0x39, 0x38, 0x37, 0x36, 0x35,
            0x34, 0x33, 0x32, 0x31, 0x30, 0x39, 0x38, 0x37,
            0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30
        };

        u8 message2[72] = {
            0xfb, 0xe2, 0xe5, 0xf0, 0xee, 0xe3, 0xc8, 0x20,
            0xfb, 0xea, 0xfa, 0xeb, 0xef, 0x20, 0xff, 0xfb,
            0xf0, 0xe1, 0xe0, 0xf0, 0xf5, 0x20, 0xe0, 0xed,
            0x20, 0xe8, 0xec, 0xe0, 0xeb, 0xe5, 0xf0, 0xf2,
            0xf1, 0x20, 0xff, 0xf0, 0xee, 0xec, 0x20, 0xf1,
            0x20, 0xfa, 0xf2, 0xfe, 0xe5, 0xe2, 0x20, 0x2c,
            0xe8, 0xf6, 0xf3, 0xed, 0xe2, 0x20, 0xe8, 0xe6,
            0xee, 0xe1, 0xe8, 0xf0, 0xf2, 0xd1, 0x20, 0x2c,
            0xe8, 0xf0, 0xf2, 0xe5, 0xe2, 0x20, 0xe5, 0xd1
        };

        printf("Original test vectors:\n");

        printf("Message 1:\n");
        init(&ctx, HASH512);
        stribog(&ctx, message1, sizeof(message1));
        printf("H^{512}: ");
        print_hash(&ctx);

        init(&ctx, HASH256);
        stribog(&ctx, message1, sizeof(message1));
        printf("H^{256}: ");
        print_hash(&ctx);

        printf("\nMessage 2:\n");
        init(&ctx, HASH512);
        stribog(&ctx, message2, sizeof(message2));
        printf("H^{512}: ");
        print_hash(&ctx);

        init(&ctx, HASH256);
        stribog(&ctx, message2, sizeof(message2));
        printf("H^{256}: ");
        print_hash(&ctx);
    }
}

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

void hash_data(struct stribog_ctx_t *ctx, const u8 *data, size_t len) {
    init(ctx, ctx->size ? HASH512 : HASH256);
    stribog(ctx, (u8*)data, len);
    write_hash_to_file(ctx, "hash.txt");
}