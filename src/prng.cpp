#include "prng.hpp"

PRNG::PRNG(){
    struct stribog_ctx_t ctx;
    init(&ctx, HASH256);

    stribog(&ctx, (u8*)seed.c_str(), strlen(seed.c_str())); // Вычисляем хэш из полученной строки 
    seed_hash = stribog_ctx_to_mpz_256(ctx); // Конвертируем полученный хэш в mpz_class 

    counter = 0;
}

mpz_class PRNG::generate_num(){
    std::string temp = seed_hash.get_str() + "|" + counter.get_str();

    struct stribog_ctx_t ctx;
    init(&ctx, HASH256);
    
    stribog(&ctx, (u8*)temp.c_str(), strlen(temp.c_str())); // Вычисляем хэш из полученной строки 
    mpz_class rand_num = stribog_ctx_to_mpz_256(ctx); // Конвертируем полученный хэш в mpz_class 

    counter += 1;

    return rand_num;
}