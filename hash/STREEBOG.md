# Streebog Hash Function Usage Guide

## Overview

This implementation provides the Streebog (GOST R 34.11-2012) hash function with both 256-bit and 512-bit variants. The implementation is part of our blockchain project and can be easily integrated with other components.

## Quick Start

### Basic Usage

1. Include the necessary header:
```c
#include "stribog.h"
```

2. Create and initialize a context:
```c
struct stribog_ctx_t ctx;
init(&ctx, HASH256);  // For 256-bit hash
// OR
init(&ctx, HASH512);  // For 512-bit hash
```

3. Hash your data:
```c
// Hash a string
const char *message = "Your message";
stribog(&ctx, (u8*)message, strlen(message));

// OR hash a file
hash_file(&ctx, "input.txt");
```

4. Get the result:
```c
// Write to a specific file
write_hash_to_file(&ctx, "output.txt");
```

## Available Functions

- `init()` - Initialize the hash context
- `stribog()` - Compute hash of data
- `hash_file()` - Hash contents of a file
- `hash_data()` - Hash data and save result
- `write_hash_to_file()` - Save hash to file

## Hash Sizes

- HASH256 (0) - Produces 256-bit (32 bytes) hash
- HASH512 (1) - Produces 512-bit (64 bytes) hash

## Output Format

The hash is written to file in hexadecimal format:
- 256-bit hash: 64 hexadecimal characters
- 512-bit hash: 128 hexadecimal characters

## Example Use Cases

### Hashing a Message
```c
struct stribog_ctx_t ctx;
init(&ctx, HASH256);
const char *msg = "Hello, World!";
stribog(&ctx, (u8*)msg, strlen(msg));
write_hash_to_file(&ctx, "hash.txt");
```

### Hashing a File
```c
struct stribog_ctx_t ctx;
init(&ctx, HASH512);
hash_file(&ctx, "document.txt");
// Result is written to hash.txt
```

## Common Issues

1. Always initialize context before use
2. Make sure input files exist and are readable
3. Check that you have write permissions for output files
4. Use the same hash size (256/512) consistently in your application

## Integration Tips

1. The hash functions are designed to work with the Schnorr signature implementation
2. Hash output is always written in hexadecimal format
3. Default output file is "hash.txt" for most functions
4. All functions handle memory allocation internally