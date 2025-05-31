#include "prng.hpp"
#include "../hash/hash_c_interface.h"

PRNG::PRNG(){
    struct stribog_ctx_t ctx;
    init(&ctx, HASH256);

    stribog(&ctx, (u8*)seed.c_str(), strlen(seed.c_str())); // Вычисляем хэш из полученной строки 
    stribog_ctx_to_mpz_256_c(&ctx, seed_hash.get_mpz_t()); // Используем C-совместимую версию

    counter = 0;
}

mpz_class PRNG::generate_num(){
    std::string temp = seed_hash.get_str() + "|" + counter.get_str();

    struct stribog_ctx_t ctx;
    init(&ctx, HASH256);
    
    stribog(&ctx, (u8*)temp.c_str(), strlen(temp.c_str())); // Вычисляем хэш из полученной строки 
    
    mpz_class rand_num;
    stribog_ctx_to_mpz_256_c(&ctx, rand_num.get_mpz_t()); // Используем C-совместимую версию

    counter += 1;

    return rand_num;
}