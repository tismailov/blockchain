#include <iostream>
#include <filesystem>
#include <fstream>
#include <gmpxx.h>
#include "src/prng.hpp"
#include "hash/stribog.h"
#include "src/merkle_tree.hpp"
#include "src/create_transactions.hpp"
#include "src/block_builder.hpp"

int main() {
    PRNG prng;

    std::string name = "IsmailovTagir";

    create_transactions(name, prng);

    std::cout << "Transactions saved in folder 'transactions'\n";
    std::cout << "================================\n";

    std::vector<std::string> files = {
        "transactions/transaction0.txt",
        "transactions/transaction1.txt",
        "transactions/transaction2.txt",
        "transactions/transaction3.txt",
        "transactions/transaction4.txt"
    };

    BlockBuilder block_builder(files, prng);

    block_builder.build_block();
    std::cout << "Block Header saved in 'block_header.txt'\n";
    std::cout << "================================\n";

    block_builder.print_block();
    std::cout << "================================\n";
    
    block_builder.print_pow_proof();
    
    return 0;
} 