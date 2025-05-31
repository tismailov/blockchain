#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <gmpxx.h>
#include "signer.hpp"
#include "generators.h"
#include "prng_c_interface.h"

void print_signature_test(const std::string& message, const mpz_class& r, const mpz_class& s) {
    std::cout << "\n=== Test messages ===" << std::endl;
    std::cout << "Message: " << message << std::endl;
    std::cout << "Signature components:" << std::endl;
    std::cout << "r = " << std::hex << r << std::endl;
    std::cout << "s = " << std::hex << s << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

int main() {
    // Инициализация параметров
    unsigned long bits = 256;
    
    // Создаем PRNG
    PRNG prng;
    PRNG_Handle_t prng_handle = PRNG_create_from_existing(&prng);

    // Инициализируем временные переменные для C-функций
    mpz_t p_tmp, g_tmp, x_tmp, P_tmp;
    mpz_inits(p_tmp, g_tmp, x_tmp, P_tmp, NULL);

    // Генерируем параметры системы
    generate_random_prime(p_tmp, bits, prng_handle);
    generate_random_generator(g_tmp, p_tmp, prng_handle);
    generate_keys(x_tmp, P_tmp, g_tmp, p_tmp, prng_handle);

    // Конвертируем в C++ классы
    mpz_class p(p_tmp), g(g_tmp), x(x_tmp), P(P_tmp);

    std::cout << "=== System parametrs ===" << std::endl;
    std::cout << "p: " << std::hex << p << std::endl;
    std::cout << "g: " << std::hex << g << std::endl;
    std::cout << "x: " << std::hex << x << std::endl;
    std::cout << "P: " << std::hex << P << std::endl;

    // Тестовые сообщения
    std::vector<std::string> messages = {
        "Hello World!",
        "Tagir maimoon",
        "PopaKota"
    };

    // Подписываем каждое сообщение
    for (const auto& message : messages) {
        auto signature = get_signature(message, x, g, p);
        print_signature_test(message, signature.first, signature.second);
    }

    // Очистка
    mpz_clears(p_tmp, g_tmp, x_tmp, P_tmp, NULL);
    PRNG_destroy(prng_handle);

    return 0;
}
