#pragma once

#include <iostream>
#include <string>
#include <gmpxx.h>
#include "../hash/stribog.h"
#include "../hash/hash_conventer.hpp"

class PRNG{
    private:
        mpz_class counter;
        const std::string seed = "IsmailovTagir";
        mpz_class seed_hash;
    public:
        PRNG();
        mpz_class generate_num();
};