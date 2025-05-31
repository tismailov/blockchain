#include <gmpxx.h>
#include <iostream>
#include "merkle_tree.hpp"
#include "prng.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <bitset>

class BlockBuilder{
    private:
        mpz_class pre_block_header_hash;
        std::string timestamp;
        mpz_class nonce;
        mpz_class data_hash;
        mpz_class block_size;
        mpz_class block_header_hash;
    public:
        BlockBuilder(const std::vector<std::string>& files_paths, PRNG& prng);
        void build_block();
        bool check_pow_condition(const mpz_class& hash, int zero_bits);
        void print_block();
        void print_pow_proof();
}; 