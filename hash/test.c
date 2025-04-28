#include <stdio.h>
#include <string.h>
#include "stribog.h"

// Function for outputting hash to console
void print_hash_hex(const char* label, struct stribog_ctx_t* ctx) {
    printf("%s: ", label);
    print_hash(ctx);
}

int main() {
    struct stribog_ctx_t ctx;

    printf("=== Testing the Streebog (GOST R 34.11-2012) algorithm ===\n\n");

    // Test 1: Hashing a string with different output sizes
    const char* test_str = "Hello, Streebog!";
    printf("Test 1: Hashing the string \"%s\"\n", test_str);

    // 512-bit hash
    init(&ctx, HASH512);
    stribog(&ctx, (u8*)test_str, strlen(test_str));
    print_hash_hex("  GOST 512-bit", &ctx);

    // 256-bit hash
    init(&ctx, HASH256);
    stribog(&ctx, (u8*)test_str, strlen(test_str));
    print_hash_hex("  GOST 256-bit", &ctx);
    printf("\n");

    // Test 2: Hashing a file (example with creating a temporary file)
    printf("Test 2: Hashing a file\n");
    FILE* tmp_file = fopen("test_file.txt", "w");
    if (tmp_file) {
        fputs("This is a test file for Streebog hashing.", tmp_file);
        fclose(tmp_file);

        // 512-bit hash of the file
        hash_file(&ctx, "test_file.txt");
        printf("  File 'test_file.txt' hashed. Result written to 'hash.txt'\n");
    } else {
        printf("  Error: unable to create test file\n");
    }
    printf("\n");

    // Test 3: Testing with empty data
    printf("Test 3: Hashing an empty string\n");
    init(&ctx, HASH512);
    stribog(&ctx, (u8*)"", 0);
    print_hash_hex("  GOST 512-bit (empty)", &ctx);
    printf("\n");

    // Test 4: Demonstrating work with binary data
    printf("Test 4: Hashing binary data\n");
    u8 binary_data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    init(&ctx, HASH512);
    stribog(&ctx, binary_data, sizeof(binary_data));
    print_hash_hex("  GOST 512-bit (binary)", &ctx);
    printf("\n");

    // Test 5: Comparing with test vectors from the standard
    printf("Test 5: Checking test vectors from the standard\n");
    process_input(1, (char*[]){""}); // Running built-in test vectors

    return 0;
}