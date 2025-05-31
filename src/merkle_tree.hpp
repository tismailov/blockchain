#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <gmpxx.h>
#include <iostream>
#include "../hash/stribog.h"
#include "../hash/hash_conventer.hpp"

struct MerkleTree{        
    private:
        std::vector<mpz_class> files_hashes;
        std::vector<std::vector<mpz_class>> fades;
        mpz_class top_hash;
    public:   
        MerkleTree(const std::vector<std::string>& files_paths);
        void calc_top_hash();
        mpz_class get_top_hash();
};