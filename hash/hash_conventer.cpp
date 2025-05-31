#include "hash_conventer.hpp"

// Преобразует хэш из ctx в mpz_class, всегда возвращая 256 бит
mpz_class stribog_ctx_to_mpz_256(const stribog_ctx_t& ctx) {
    mpz_class result;
    size_t output_size = 32;  // Всегда 32 байта (256 бит)

    // Если хэш 512-битный, берем только первые 256 бит (первые 32 байта)
    const uint8_t* hash_data = ctx.h;

    // Импортируем 256 бит (32 байта) в mpz_class
    mpz_import(
        result.get_mpz_t(),  // Указатель на mpz_t
        output_size,         // Количество байтов (32)
        1,                  // Порядок: little-endian
        sizeof(uint8_t),     // Размер элемента (1 байт)
        0,                  // Endianness (native)
        0,                  // Nails (неиспользуемые биты)
        hash_data           // Указатель на данные
    );

    return result;
}

