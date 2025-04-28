# Streebog Hash Algorithm Implementation Documentation

## Overview

This project provides an implementation of the Streebog hash algorithm (GOST R 34.11-2012), which is a Russian cryptographic hash function standard. The implementation supports both 256-bit and 512-bit hash variants.

## Features

- Supports both Streebog-256 and Streebog-512 hash algorithms
- Can process:
  - Strings
  - Files
  - Binary data
- Includes test vectors from the standard for verification
- Simple command-line interface for basic operations

## Build Instructions

### Prerequisites

- GCC compiler
- GNU Make
- Standard C library

### Building

To build the project:

```bash
make
```

This will create an executable named `stribog_test`.

### Cleaning

To clean up build artifacts:

```bash
make clean
```

### Running Tests

To build and run the tests:

```bash
make run
```

## API Documentation

### Data Types

```c
struct stribog_ctx_t {
    u8 h[BLOCK_SIZE];    // Hash state
    u8 N[BLOCK_SIZE];     // Message length counter
    u8 S[BLOCK_SIZE];     // Checksum
    u8 size;             // Hash size (HASH256 or HASH512)
};
```

### Key Functions

1. **Initialization**
   ```c
   void init(struct stribog_ctx_t *ctx, u8 size);
   ```
   - Initializes the hash context
   - `size`: Either `HASH256` or `HASH512`

2. **Main Hashing Function**
   ```c
   void stribog(struct stribog_ctx_t *ctx, u8 *message, u64 len);
   ```
   - Computes the hash of a message
   - `ctx`: Initialized context
   - `message`: Input data to hash
   - `len`: Length of input data in bytes

3. **File Hashing**
   ```c
   void hash_file(struct stribog_ctx_t *ctx, const char *filename);
   ```
   - Hashes the contents of a file
   - Writes result to "hash.txt"

4. **Output Functions**
   ```c
   void print_hash(struct stribog_ctx_t *ctx);
   void write_hash_to_file(struct stribog_ctx_t *ctx, const char *filename);
   ```

## Usage Examples

### Command Line Interface

The program provides a simple CLI:

```
Usage: stribog_test [OPTIONS] [INPUT]
Options:
  -s <string>  Hash a string
  -f <file>    Hash a file
  -b <size>    Hash size (256 or 512, default: 512)
  -h           Show this help
```

### Programmatic Usage

1. Hashing a string:
```c
struct stribog_ctx_t ctx;
init(&ctx, HASH512);
stribog(&ctx, (u8*)"Hello, world!", 13);
print_hash(&ctx);
```

2. Hashing binary data:
```c
u8 data[] = {0x00, 0x01, 0x02, 0x03};
init(&ctx, HASH256);
stribog(&ctx, data, sizeof(data));
```

## Test Vectors

The implementation includes the standard test vectors from GOST R 34.11-2012, which are automatically verified when running the tests.

## Implementation Details

- Block size: 64 bytes (512 bits)
- Uses the standard S-box and linear transformation matrix
- Implements the compression functions g_N and g_0 as specified
- Follows the standard padding scheme